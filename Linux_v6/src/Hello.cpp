#include "Hello.h"

using namespace std;

HelloNeighborList::HelloNeighborList(uint8_t linkCode,
		std::list<IPv6*> NeighborsAddrList) {
	mLinkCode = linkCode;
	mReserved = 0x0;
	mNeighborsAddrList = NeighborsAddrList;
	mLinkMessageSize = (mNeighborsAddrList.size() * 16) + 4;
}
uint8_t HelloNeighborList::getLinkCode() {
	return mLinkCode;
}

uint8_t HelloNeighborList::getReserved() {
	return mReserved;
}

uint16_t HelloNeighborList::getLinkMessageSize() {
	return mLinkMessageSize;
}

std::list<IPv6*> HelloNeighborList::getNeighborsAddrList() {
	return mNeighborsAddrList;
}


/**
 *  IMPORTANT !
 */
Hello::Hello(uint16_t packetSequenceNumber,
		IPv6 * originatorAddress,
		uint16_t messageSequenceNumber, std::list<IPv6*> neighborList,
		std::list<IPv6*> mprList) : Message() {
	mPacketLength = 0;
	mPacketSequenceNumber= packetSequenceNumber;
	mMessageSequenceNumber = messageSequenceNumber;
	mMessageType = HELLO_TYPE;
	mMessageSize = 0;
	mVTime= 0;
	mWillingness = WILL_DEFAULT;
	mReserved = 0x0;
	mHTime = 0; //todo combien dans HTIME ?
	mHopCount = 0;
	mOriginatorAddress=originatorAddress;
	mTimeToLive = 2;
	if (neighborList.size() != 0) {
		HelloNeighborList *nghb = new HelloNeighborList(10, neighborList);
		mNeighbors.push_back(nghb);
	}
	if (mprList.size() != 0) {
		HelloNeighborList *mpr = new HelloNeighborList(01, mprList);
		mNeighbors.push_back(mpr);
	}

}
Hello::Hello(uint8_t willingness,uint8_t hTime,uint16_t packetLength, uint16_t packetSequenceNumber,
		uint8_t messageType, uint8_t vTime, uint16_t messageSize,
		IPv6 * originatorAddress, uint8_t timeToLive, uint8_t hopCount,
		uint16_t messageSequenceNumber,std::list<HelloNeighborList*> neighbor):Message() {
	mWillingness = willingness;
	mReserved = 0x0;
	mHTime = hTime;
	mPacketLength= packetLength;
	mPacketSequenceNumber = packetSequenceNumber;
	mMessageSequenceNumber= messageSize;
	mMessageSize = messageSize;
	mMessageType = messageType;
	mVTime= vTime;
	mOriginatorAddress= originatorAddress;
	mTimeToLive = timeToLive;
	mHopCount= hopCount;
	mNeighbors = neighbor;
}

uint16_t Hello::getReserved() {
	return mReserved;
}

void Hello::setReserved(uint16_t res) {
	mReserved = res;
}

uint8_t Hello::getHTime() {
	return mHTime;
}

void Hello::setHTime(uint8_t n) {
	mHTime = n;
}

uint8_t Hello::getWillingness() {
	return mWillingness;
}

void Hello::setWillingness(uint8_t n) {
	mWillingness = n;
}

std::list<HelloNeighborList*> Hello::getNeighbors() {
	return mNeighbors;
}

void Hello::setNeighbors(HelloNeighborList* n) {
	mNeighbors.push_back(n);
}

void Hello::printData() {
	std::cout << "Htime = ";
	std::cout << (int) mHTime << std::endl;
	std::cout << "Willingness = ";
	std::cout << (int) mWillingness << std::endl;

	std::list<HelloNeighborList*> liste = mNeighbors;
	for (std::list<HelloNeighborList*>::iterator ipv = liste.begin();
			ipv != liste.end(); ipv++) {
		std::list<IPv6*> ip = (*ipv)->getNeighborsAddrList();
		std::cout << " LINK CODE :" << (int) (*ipv)->getLinkCode() << std::endl;
		std::cout << "Link msg Size : "<< (int) (*ipv)->getLinkMessageSize() << std::endl;
		for (std::list<IPv6*>::iterator it = ip.begin(); it != ip.end(); it++) {
			std::cout << " IP : " << (*it)->toChar() << std::endl;
		}
	}
}

int Hello::sendHello() {
	char* send_buf;
	bool sendagain=false;
	send_buf = (char*) malloc(sizeof(char) * BUFF_SIZE);
	mHopCount++;
	mTimeToLive--;
	//je dois remplir toutes les données de l'entete du msg cad PacketLenght PacketsequenceNumber messagetype Vtime Massage Size Originator Address TTL hopcount Messagesequence number

	//packetSequenceNumber = (packetSequenceNumber++)%(0xFFFF); //2o
	mVTime = 0xF;	//1o

	// IPV6
	//*(uint16_t*)(send_buf+8) = originatorAddress;
	//On doit faire sur 16 mais comme on ne s'occupe pas des 8 premier c'est pour ça qu'on commence à 16 et non 8
	int c = 8;
	for (int y = 0; y < 8; y++) {
		*(uint16_t*) (send_buf + c) = mOriginatorAddress->getScope(y);
		c += 2;
	}
	//Time To Live
	*(send_buf + 24) = mTimeToLive;
	//*(send_buf+24) = 0x3C;
	//HOP COUNT
	*(send_buf + 25) = mHopCount;
	//*(send_buf+25) = 0x3C;
	//Message Sequence Number
	*(uint16_t*) (send_buf + 26) = mMessageSequenceNumber;
	//*(uint16_t*)(send_buf+26) = 0x4444;

	//HELLO MESSAGE :

	//RESERVED
	*(uint16_t*) (send_buf + 28) = mReserved;
	// HTIME
	*(send_buf + 30) = mHTime;
	// WillingNess
	*(send_buf + 31) = mWillingness;

	c = 32;

	uint16_t helloSize = 0;
	std::list<HelloNeighborList*> listNeighbor = mNeighbors;

	for (std::list<HelloNeighborList*>::iterator iv = listNeighbor.begin();
			iv != listNeighbor.end(); iv++) {
		// link code
		//helloSize += 4;
		*(uint8_t*) (send_buf + c) = (*iv)->getLinkCode();
		// reserved
		c++;
		*(uint8_t*) (send_buf + c) = (*iv)->getReserved();
		// link message size
		c++;
		*(uint16_t*) (send_buf + c) = (*iv)->getLinkMessageSize();
		helloSize += (*iv)->getLinkMessageSize();
		c += 2;
		std::list<IPv6*> list = (*iv)->getNeighborsAddrList();
		for (std::list<IPv6*>::iterator it = list.begin();
				it != list.end(); it++) { // voir le nbr d'ip max
			for (int j = 0; j < 8; j++) {
				*(uint16_t*) (send_buf + c) = (*it)->getScope(j);
				c += 2;
			}
		}
	}

	mMessageSize = helloSize + 20;
	mPacketLength = mMessageSize + 4; // 2o

	/*A la fin car on a besoin du parcours des itérateurs pour connaître certaines données donc au lieu que le début soit éparpillé en haut et en bas, tout a été mis en bas.
	 */
//packetHeader
	// packetLength
	*(uint16_t*) send_buf = mPacketLength;
	//*(uint16_t*)send_buf = 0xA2B8;
	// PacketSequenceNumber
	*(uint16_t*) (send_buf + 2) = mPacketSequenceNumber;
	//*(uint16_t*)(send_buf+2) = 0xB0C3;

	//messageHeader
	// messageType
	*(send_buf + 4) = mMessageType;
	//*(send_buf+4) = 0x01;
	//Vtime
	*(send_buf + 5) = mVTime;
	//*(send_buf+5) = 0x22;
	// messageSize
	*(uint16_t*) (send_buf + 6) = mMessageSize;
	//*(uint16_t*)(send_buf+6) = (uint16_t)0x42;

	std::string container(send_buf, c);
	boost::asio::io_service io_service;
	boost::asio::ip::udp::resolver resolver(io_service);

	boost::asio::ip::udp::resolver::query query1(boost::asio::ip::udp::v6(),
			"ff02::1", "698");
	boost::asio::ip::udp::resolver::query query2(boost::asio::ip::udp::v6(),
			mOriginatorAddress->toChar(), "698");

	boost::asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(
			query1);
	boost::asio::ip::udp::endpoint sender_endpoint = *resolver.resolve(query2);
	boost::asio::ip::udp::socket socket(io_service);
	socket.open(boost::asio::ip::udp::v6());
	socket.send_to(boost::asio::buffer(container), receiver_endpoint);
	socket.close();
	return 0;
}
