#include "Listener.h"

#define HELLO_MESSAGE 1
#define Tc_MESSAGE 2

Listener::~Listener() {
	delete mIoService;
	delete mRemoteEndpoint;
	delete mSenderEndpoint;
	delete mSocket;
}

Listener::Listener() {
	try {
		this->mIoService = new boost::asio::io_service();
		this->mRemoteEndpoint = new boost::asio::ip::udp::endpoint(
				boost::asio::ip::udp::v6(), 7171);
		this->mSocket = new boost::asio::ip::udp::socket(*this->mIoService,
				*this->mRemoteEndpoint);
		mSem_cons= new boost::interprocess::interprocess_semaphore(MIN_LENGTH);
		mSem_prod= new boost::interprocess::interprocess_semaphore(MAX_LENGTH);
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void Listener::listenSocket() {
	boost::system::error_code error;
	try {
		for (;;) {
			// P() du producteur (initialisé a MAX_LENGHT)
			mSem_prod->wait();
			boost::array<char, BUFF_SIZE> recvBuf;
			size_t len = mSocket->receive_from(boost::asio::buffer(recvBuf),
					*mRemoteEndpoint, 0, error);
			if (error && error != boost::asio::error::message_size)
				throw boost::system::system_error(error);

			std::cout << "taille message : " << len << "\n";

			// future stored message
			Message * msg;

			char* temp; // = (char*)malloc(len*sizeof(char));
			char * originAddr = temp;
			temp = recvBuf.data(); //put the buffer we receive in a char*

			// store packetLength (2 bytes)
			uint16_t packetLength = (*(uint16_t*) temp);
			temp += 2;
			// store packetSequenceNumber (2 bytes )
			uint16_t packetSequenceNumber = (*(uint16_t*) temp);
			temp += 2;

			// store messageType (1 b)
			uint8_t messageType = (*(uint8_t*) temp);

			temp = temp + 1;

			// store vTime (1 b)
			uint8_t vTime = (*(uint8_t*) temp);
			temp = temp + 1;

			// store messageSize (2 b)
			uint16_t messageSize = (*(uint16_t*) temp);
			temp += 2;

			// store IPV6 address (16 b)
			IPv6* originatorAddress = new IPv6(*(unsigned short*) (temp + 8),
					*(unsigned short*) (temp + 10),
					*(unsigned short*) (temp + 12),
					*(unsigned short*) (temp + 14));

			temp += 16;

			// store TTL (1 b)
			uint8_t timeToLive = (*(uint8_t*) temp);
			temp = temp + 1;

			// store hopCount (1b)
			uint8_t hopCount = (*(uint8_t*) temp);
			temp = temp + 1;

			// store MessageSequenceNumber
			uint16_t messageSequenceNumber = (*(uint16_t*) temp);
			temp = temp + 2;

			if (messageType == HELLO_MESSAGE) {

				Hello* hMH = new Hello(packetLength, packetSequenceNumber,
						messageType, vTime, messageSize, originatorAddress,
						timeToLive, hopCount, messageSequenceNumber);
				/*
				 HELLO HEADER  :
				 |Reserved(16 Bits)| Htime(8Bits)|Willingness(8Bits)|
				 */
				//hMH->reserved = *(uint16_t*)temp;
				temp += 2;

				hMH->setHTime(*(uint8_t*) temp);
				++temp;
				hMH->setWillingness(*(uint8_t*) temp);
				++temp;
				uint16_t compt = 0; //4

				while (compt < messageSize - 28) {

					//
					uint8_t linkCode = (uint8_t) *temp;
					//hNL->setLinkCode((uint8_t) *temp);
					++temp;
					//hNL->reserved = (uint8_t)*temp;
					++temp;

					uint16_t linkMessageSize = *(uint16_t*) temp;
					temp += 2;

					int nb = (linkMessageSize - 4) / 16;
					int i = 0;
					std::list<IPv6> listIp;
					for (; i < nb; i++) {
						IPv6* currentV6 = new IPv6(
								*(unsigned short*) (temp + 8),
								*(unsigned short*) (temp + 10),
								*(unsigned short*) (temp + 12),
								*(unsigned short*) (temp + 14));
						temp = temp + 16; //for each address ipv6 we add 16

						//hMH->neighbors->neighborsAddrList.push_front(*currentV6);
						//hNL->neighborsAddrList.emplace_back(*currentV6);
						listIp.push_back(*currentV6);
					}
					HelloNeighborList* hNL = new HelloNeighborList(linkCode,linkMessageSize,listIp);
					//compt+=hMH->neighbors->linkMessageSize;
					compt += linkMessageSize;

					hMH->setNeighbors(hNL);
				}
				msg = hMH;

			} else if (messageType == Tc_MESSAGE) {
				Tc* tMH = new Tc(packetLength, packetSequenceNumber,
						messageType, vTime, messageSize, originatorAddress,
						timeToLive, hopCount, messageSequenceNumber);

				tMH->setANSN(*(uint16_t*) temp);
				temp += 2;
				// reserved zone
				temp += 2;

				uint16_t nb = (messageSize - 28) / 16;
				uint16_t i = 0;
				for (; i < nb; i++) {
					IPv6* currentV6 = new IPv6(*(unsigned short*) (temp + 8),
							*(unsigned short*) (temp + 10),
							*(unsigned short*) (temp + 12),
							*(unsigned short*) (temp + 14));
					temp = temp + 16; //for each address ipv6 we add 16
					tMH->setAdvertisedNeighborMainAddress(currentV6);
				}
				msg = (Message*) tMH;
			}

			/*msg->printData();
			if (messageType == HELLO_MESSAGE) {
				(*(Hello*) msg).printData();
			} else if (messageType == Tc_MESSAGE) {
				(*(Tc*) msg).printData();
			}*/

			//Bloque accès a la liste
			mProtectList.lock();
			//si + de 10 elem dans la liste on produit rien

			receptionMsg(msg);
			
			mProtectList.unlock();

			// V() du conso
			mSem_cons->post();

		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void Listener::receptionMsg(Message* msg) {

	mListMsg.push_back(*msg);
}

Message Listener::getMsg() {
	// P() du conso
	mSem_cons->wait();

	//bloque la liste
	mProtectList.lock();

	Message message = mListMsg.front();
	mListMsg.pop_front();
	
	// libère la liste
	mProtectList.unlock();

	// V() du prod
	mSem_prod->post();

	return message;
}

