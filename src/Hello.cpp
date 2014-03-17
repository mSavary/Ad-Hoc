#include "Hello.h"

using namespace std;

HelloNeighborList::HelloNeighborList(uint8_t linkCode,
		std::list<IPv6> NeighborsAddrList) {
	mLinkCode = linkCode;
	mReserved = 0x0;
	mNeighborsAddrList = NeighborsAddrList;
	//todo calcul de link message size
}
HelloNeighborList::HelloNeighborList(uint8_t linkCode,uint16_t linkMessageSize,
		std::list<IPv6> NeighborsAddrList) {
	mLinkCode = linkCode;
	mReserved = 0x0;
	mNeighborsAddrList = NeighborsAddrList;
	mLinkMessageSize = linkMessageSize;
	//todo calcul de link message size
}
uint8_t HelloNeighborList::getLinkCode() {
	return mLinkCode;
}

uint16_t HelloNeighborList::getReserved() {
	return mReserved;
}

uint8_t HelloNeighborList::getLinkMessageSize() {
	return mLinkMessageSize;
}

std::list<IPv6> HelloNeighborList::getNeighborsAddrList() {
	return mNeighborsAddrList;
}
/**
 *  IMPORTANT !
 */
//TODO calcul automatique :

//to see : != entre packet et mess sequence number
Hello::Hello(uint16_t packetSequenceNumber, uint8_t messageType,
		IPv6 * originatorAddress, uint8_t hopCount,
		uint16_t messageSequenceNumber, std::list<IPv6> neighborList,
		std::list<IPv6> mprList) :
		Message(packetSequenceNumber, messageType, originatorAddress, hopCount,
				messageSequenceNumber) {
	HelloNeighborList *nghb = new HelloNeighborList(10,
			neighborList);
	HelloNeighborList *mpr = new HelloNeighborList(01, mprList);
	mNeighbors.push_back(*nghb);
	mNeighbors.push_back(*mpr);
}
Hello::Hello(uint16_t packetLength, uint16_t packetSequenceNumber,
		uint8_t messageType, uint8_t vTime, uint16_t messageSize,
		IPv6 * originatorAddress, uint8_t timeToLive, uint8_t hopCount,
		uint16_t messageSequenceNumber) :
		Message(packetLength, packetSequenceNumber, messageType, vTime,
				messageSize, originatorAddress, timeToLive, hopCount,
				messageSequenceNumber) {

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

std::list<HelloNeighborList> Hello::getNeighbors() {
	return mNeighbors;
}

void Hello::setNeighbors(HelloNeighborList* n) {
	mNeighbors.push_back(*n);
}

void Hello::printData() {
	std::cout << "Htime = ";
	std::cout << (int) mHTime << std::endl;
	std::cout << "Willingness = ";
	std::cout << (int) mWillingness << std::endl;

	std::list<HelloNeighborList> liste = mNeighbors;
	for (std::list<HelloNeighborList>::iterator ipv = liste.begin();
			ipv != liste.end(); ipv++) {
		std::cout << " LINK CODE :" << (int) ipv->getLinkCode() << std::endl;
		std::list<IPv6> ip = ipv->getNeighborsAddrList();

		for (std::list<IPv6>::iterator it = ip.begin(); it != ip.end(); it++) {
			std::cout << " IP : " << it->toChar() << std::endl;
		}
	}
}

int Hello::sendHello() {
	boost::asio::io_service *io_service;
	boost::asio::ip::udp::endpoint *receiver_endpoint;
	boost::asio::ip::udp::endpoint *sender_endpoint;
	boost::asio::ip::udp::socket *socket;

	char* send_buf;
	send_buf = (char*) malloc(sizeof(char) * BUFF_SIZE);

	//je dois remplir toutes les données de l'entete du msg cad PacketLenght PacketsequenceNumber messagetype Vtime Massage Size Originator Address TTL hopcount Messagesequence number

	//packetSequenceNumber
	//packetSequenceNumber = (packetSequenceNumber++)%(0xFFFF); //2o
	//packetSequenceNumber=packetSequenceNumberE;
	mMessageType = 0x01;	//1o
	mVTime = 0x00;	//1o
	//messageSize = ;//2o
	//originatorAddress = ; //16o
	mTimeToLive = 0x01; //1o
	//TTL TC=255
	mHopCount = 0x00;	//1o
	//messageSequenceNumber =(messageSequenceNumber++)%(0xFFFF) ; //2o
	//packetLength 4+ messagesize
	//messageSize  24+ taille hello
	//comme hello utilise des listes pour avoir la taille du hello j'aurai qu'à faire reserved+hmtim+willingness+taille(list )

	// IPV6
	//*(uint16_t*)(send_buf+8) = originatorAddress;
	//On doit faire sur 16 mais comme on ne s'occupe pas des 8 premier c'est pour ça qu'on commence à 16 et non 8
	*(uint16_t*) (send_buf + 8) = 0x0000;	//todo entete ipv6
	*(uint16_t*) (send_buf + 8) = 0x0000;
	*(uint16_t*) (send_buf + 8) = 0x0000;
	*(uint16_t*) (send_buf + 8) = 0x0000;
	*(uint16_t*) (send_buf + 16) = mOriginatorAddress->getScope(4);
	// *(send_buf+17) =0xF2;
	*(uint16_t*) (send_buf + 18) = mOriginatorAddress->getScope(5);
	// *(send_buf+19) =0xF4;
	*(uint16_t*) (send_buf + 20) = mOriginatorAddress->getScope(6);
	// *(send_buf+21) =0xF6;
	*(uint16_t*) (send_buf + 22) = mOriginatorAddress->getScope(7);
	// *(send_buf+23) =0xF8;

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

	int c = 32;

	uint16_t helloSize = 0;

	for (std::list<HelloNeighborList>::iterator iv = mNeighbors.begin();
			iv != mNeighbors.end(); iv++) {
		// link code
		*(uint8_t*) (send_buf + c) = iv->getLinkCode();
		// reserved
		*(uint8_t*) (send_buf + c + 1) = iv->getReserved();
		// link message size
		*(uint16_t*) (send_buf + c + 2) = iv->getLinkMessageSize();
		helloSize += iv->getLinkMessageSize();
		c += 2;

		// IPV6  :  Neighbor Interface Address
		// TODO PAREIL QUE TC tout les SCOPE + si 31 IPs
		std::list<IPv6> ip = iv->getNeighborsAddrList();
		c += 2;
		for (std::list<IPv6>::iterator it = ip.begin(); it != ip.end(); it++) {
			c += 8;
			*(uint16_t*) (send_buf + c) = it->getScope(4);
			c += 2;
			*(uint16_t*) (send_buf + c) = it->getScope(5);
			c += 2;
			*(uint16_t*) (send_buf + c) = it->getScope(6);
			c += 2;
			*(uint16_t*) (send_buf + c) = it->getScope(7);
			c += 2;
		}
	}

	helloSize += 4;
	mMessageSize = helloSize + 24;
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
	io_service = new boost::asio::io_service();
	receiver_endpoint = new boost::asio::ip::udp::endpoint(
			boost::asio::ip::address::from_string("127.0.0.1"), 7171);
	sender_endpoint = new boost::asio::ip::udp::endpoint(
			boost::asio::ip::address::from_string("127.0.0.1"), 2050);
	socket = new boost::asio::ip::udp::socket(*io_service, *sender_endpoint);
	socket->send_to(boost::asio::buffer(container), *receiver_endpoint);

	return 0;
}
