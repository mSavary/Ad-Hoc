#include "Tc.h"

/* constants */
#define SIZE_MAX_UDP 512
#define C_TIME 0.0625
#define a 8
#define b 6

using namespace std;
//todo CALCUL AUTO de :uint16_t packetLength,uint8_t vTime,uint16_t messageSize, uint8_t timeToLive
Tc::Tc(uint16_t packetSequenceNumber, IPv6 * originatorAddress,
		uint16_t messageSequenceNumber, std::list<IPv6*> advertisedList) :
		Message() {
	mPacketLength = 0;
	mPacketSequenceNumber = packetSequenceNumber;
	mMessageType = TC_TYPE;
	mVTime = C_TIME * (1 + a / 16) * pow(2, b);// todo a et B ?
	mMessageSize = 0;
	mOriginatorAddress = originatorAddress;
	mTimeToLive = 255;
	mHopCount = 0;
	mMessageSequenceNumber = messageSequenceNumber;
	mANSN = 55; //todo combien ca vaut ? c'est quoi  ?
	mReserved = 0x0;
	mFromIp = originatorAddress;
	mAdvertisedNeighborMainAddress = advertisedList;

}

Tc::Tc(uint16_t packetLength, uint16_t packetSequenceNumber,
		uint8_t messageType, uint8_t vTime, uint16_t messageSize,
		IPv6 * originatorAddress, uint8_t timeToLive, uint8_t hopCount,
		uint16_t messageSequenceNumber,uint16_t Ansn, std::string ip,
		std::list<IPv6*> advertisedList) :
		Message() {
	mPacketLength = packetLength;
	mPacketSequenceNumber = packetSequenceNumber;
	mMessageType = messageType;
	mVTime = vTime;
	mMessageSize = messageSize;
	mOriginatorAddress = originatorAddress;
	mTimeToLive = timeToLive;
	mHopCount = hopCount;
	mMessageSequenceNumber = messageSequenceNumber;
	mANSN = Ansn; //todo combien ca vaut ? c'est quoi  ?
	mReserved = 0x0;
	mFromIp = new IPv6(ip);
	mAdvertisedNeighborMainAddress = advertisedList;
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

std::list<IPv6*> Tc::getAdvertisedNeighborMainAddress() {
	return mAdvertisedNeighborMainAddress;
}

void Tc::setANSN(uint16_t n) {
	mANSN = n;
}

void Tc::addAdvertisedNeighborMainAddress(short n, short o, short p, short q) {
	IPv6* temp = new IPv6(n, o, p, q);
	mAdvertisedNeighborMainAddress.push_back(temp);
}

void Tc::addAdvertisedNeighborMainAddress(IPv6* n) {
	mAdvertisedNeighborMainAddress.push_back(n);
}

void Tc::printData() {
	std::cout << "ANSN = ";
	std::cout << (int) mANSN << std::endl;

	std::list<IPv6*> ip = mAdvertisedNeighborMainAddress;
	std::cout << "Taille = " << mAdvertisedNeighborMainAddress.size()
			<< std::endl;

	for (std::list<IPv6*>::iterator it = ip.begin(); it != ip.end(); it++) {
		std::cout << " IP : " << (*it)->toChar() << std::endl;
	}
}

int Tc::sendTc() {

	char* send_buf;
	send_buf = (char*) malloc(sizeof(char) * BUFF_SIZE);
	mHopCount++;
	mTimeToLive--;
	// IPV6
	int c = 8;
	for (int y = 0; y < 8; y++) {
		*(uint16_t*) (send_buf + c) = mOriginatorAddress->getScope(y);
		c += 2;
	}
	//Time To Live
	*(send_buf + 24) = mTimeToLive;
	//HOP COUNT
	*(send_buf + 25) = mHopCount;
	//Message Sequence Number
	*(uint16_t*) (send_buf + 26) = mMessageSequenceNumber;

	//Tc MESSAGE :
	// ANSN
	*(uint16_t*) (send_buf + 28) = mANSN;
	// IPV6 : AdvertisedNeighborMainAddress 
	*(uint16_t*) (send_buf + 30) = mReserved;

	c = 32;
//todo : scope 0/1/2/3 IP !!
	for (std::list<IPv6*>::iterator it = mAdvertisedNeighborMainAddress.begin();
			it != mAdvertisedNeighborMainAddress.end(); it++) {
		std::cout << "push : " << (*it)->toChar() << std::endl;
		for (int j = 0; j < 8; j++) {
			*(uint16_t*) (send_buf + c) = (*it)->getScope(j);
			c += 2;
		}
	}

	uint16_t TcSize = (mAdvertisedNeighborMainAddress.size()*16)+4;
	std::cout << "Taille1 = " << mAdvertisedNeighborMainAddress.size()
			<< std::endl;
	mMessageSize = TcSize + 20;
	mPacketLength = mMessageSize + 4;	//2o=32+mAdvertisedNeighborMainAddress.size()

	//packetHeader
	// packetLength
	*(uint16_t*) send_buf = mPacketLength;
	// PacketSequenceNumber
	*(uint16_t*) (send_buf + 2) = mPacketSequenceNumber;

	//messageHeader
	// messageType 
	* (send_buf + 4) = mMessageType;
	//Vtime
	* (send_buf + 5) = mVTime;
	// messageSize
	*(uint16_t*) (send_buf + 6) = mMessageSize;

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
