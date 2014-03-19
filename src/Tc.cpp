#include "Tc.h"

/* constants */
#define SIZE_MAX_UDP 512
#define C_TIME 0.0625
#define a 8
#define b 6

using namespace std;
//todo CALCUL AUTO de :uint16_t packetLength,uint8_t vTime,uint16_t messageSize, uint8_t timeToLive
Tc::Tc(uint16_t packetSequenceNumber, uint8_t messageType,
		IPv6 * originatorAddress, uint8_t hopCount,
		uint16_t messageSequenceNumber,std::list<IPv6>advertisedList, std::string ip) :
		Message(packetSequenceNumber, messageType, originatorAddress, hopCount,
				messageSequenceNumber) {
	mAdvertisedNeighborMainAddress=advertisedList;
	mFromIp = new IPv6(ip);
}

Tc::Tc(uint16_t packetLength, uint16_t packetSequenceNumber,
		uint8_t messageType, uint8_t vTime, uint16_t messageSize,
		IPv6 * originatorAddress, uint8_t timeToLive, uint8_t hopCount,
		uint16_t messageSequenceNumber,std::string ip) :
		Message(packetLength, packetSequenceNumber, messageType, vTime,
				messageSize, originatorAddress, timeToLive, hopCount,
				messageSequenceNumber) {
	mFromIp = new IPv6(ip);
} // end construct

uint16_t Tc::getANSN() {
	return mANSN;
}

uint16_t Tc::getReserved() {
	return mReserved;
}

void Tc::setReserved(uint16_t res) {
	mReserved = res;
}

std::list<IPv6> Tc::getAdvertisedNeighborMainAddress() {
	return mAdvertisedNeighborMainAddress;
}

void Tc::setANSN(uint16_t n) {
	mANSN = n;
}

void Tc::setAdvertisedNeighborMainAddress(short n, short o, short p, short q) {
	IPv6* temp = new IPv6(n, o, p, q);
	mAdvertisedNeighborMainAddress.push_back(*temp);
}

void Tc::setAdvertisedNeighborMainAddress(IPv6* n) {
	mAdvertisedNeighborMainAddress.push_back(*n);
}

void Tc::printData() {
	std::cout << "ANSN = ";
	std::cout << (int) mANSN << std::endl;

	std::list<IPv6> ip = mAdvertisedNeighborMainAddress;

	for (std::list<IPv6>::iterator it = ip.begin(); it != ip.end(); it++) {
		std::cout << " IP : " << it->toChar() << std::endl;
	}
}

int Tc::sendTc() {
	boost::asio::io_service *io_service;
	boost::asio::ip::udp::endpoint *receiver_endpoint;
	boost::asio::ip::udp::endpoint *sender_endpoint;
	boost::asio::ip::udp::socket *socket;

	char* send_buf;
	send_buf = (char*) malloc(sizeof(char) * BUFF_SIZE);

	uint16_t TcSize = 4;
	this->mMessageType = 0x02;
	this->mVTime = (C_TIME * (1 + a / 16) * pow(2, b));
	this->mTimeToLive = 0xFF;
	this->mHopCount = 0x00;
	this->mANSN = getANSN();
	this->mReserved = 0x0000;

	// IPV6
	int c = 8;
	for(int y=0; y<8; y++){
		*(uint16_t*) (send_buf + c) = mOriginatorAddress->getScope(y);
		c+=2;
	}
	//Time To Live
	*(send_buf + 24) = mTimeToLive;
	//HOP COUNT
	*(send_buf + 25) = mHopCount;
	//Message Sequence Number
	*(uint16_t*) (send_buf + 26) = mMessageSequenceNumber;

	//Tc MESSAGE :
	// ANSN
	*(uint16_t*) (send_buf + 28) = this->getANSN();
	//+2 pour le reserved
	// IPV6 : AdvertisedNeighborMainAddress 
	*(send_buf + 30) = this->getReserved();

	int c = 32;
	int i = 0;
//todo : scope 0/1/2/3 IP !!
	for (std::list<IPv6>::iterator it = mAdvertisedNeighborMainAddress.begin();
			it != mAdvertisedNeighborMainAddress.end() && i < 31; it++) {

		for(int j = 0; j<8;j++){
			*(uint16_t*) (send_buf + c) = it->getScope(j);
			c+=2;
		}

		mAdvertisedNeighborMainAddress.erase(it);
		i++;
	}

	TcSize += mAdvertisedNeighborMainAddress.size();
	this->mMessageSize = TcSize + 24;
	this->mPacketLength = mMessageSize + 4; // 2o

	//packetHeader
	// packetLength
	*(uint16_t*) send_buf = mPacketLength;
	// PacketSequenceNumber
	*(uint16_t*) (send_buf + 2) = mPacketSequenceNumber;

	//messageHeader
	// messageType 
	*(send_buf + 4) = mMessageType;
	//Vtime
	*(send_buf + 5) = mVTime;
	// messageSize
	*(uint16_t*) (send_buf + 6) = mMessageSize;

	printf("c = %d\n", c);
	printf("mMessageSize = %d\n", mMessageSize);

	std::string container(send_buf, c);
	io_service = new boost::asio::io_service();
	// TODO PAS IPV4 mais IPV6
	receiver_endpoint = new boost::asio::ip::udp::endpoint(
			boost::asio::ip::address_v6::from_string("2014::ffff:ffff:ffff:ffff"), 7171);// todo broadcast
	sender_endpoint = new boost::asio::ip::udp::endpoint(
			boost::asio::ip::address_v6::from_string(mFromIp->toChar()), 2050);
	socket = new boost::asio::ip::udp::socket(*io_service, *sender_endpoint);
	socket->send_to(boost::asio::buffer(container), *receiver_endpoint);

	if (mAdvertisedNeighborMainAddress.size() > 0)//IMPORTANT retourne 1 si la liste des voisins n'est pas vide ! Besoin de rappeler la fonction send !
			{
		sendTc();
	}

	else //liste des voisins vide
	{
		return 0;
	}
}
