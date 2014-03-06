#include "Tc.h"

using namespace std;
Tc::Tc() {
}

Tc::Tc(
       uint16_t packetLength,
       uint16_t  packetSequenceNumber,
       uint8_t messageType,
       uint8_t vTime,
       uint16_t messageSize,
       IPv6 * originatorAddress,
       uint8_t timeToLive,
       uint8_t hopCount,
       uint16_t messageSequenceNumber
       ): 
Message(packetLength,
	  packetSequenceNumber,
	  messageType, 
	  vTime,
	  messageSize, 
	  originatorAddress,
	  timeToLive,
	  hopCount,
	  messageSequenceNumber
	)
{}// end construct

uint16_t Tc::getANSN()
{
  	return mANSN;
}

uint16_t Tc::getReserved() {
	return mReserved;
}

void Tc::setReserved(uint16_t res)  {
	mReserved = res;
}

std::list<IPv6> Tc::getAdvertisedNeighborMainAddress()  {
  	return mAdvertisedNeighborMainAddress;
}

void Tc::setANSN(uint16_t n)  {
  	mANSN = n;
}

void Tc::setAdvertisedNeighborMainAddress(short n, short o, short p, short q) {
  	IPv6* temp = new IPv6(n, o, p, q);
  	mAdvertisedNeighborMainAddress.push_back(*temp);
}

void Tc::setAdvertisedNeighborMainAddress(IPv6* n)  {
  	mAdvertisedNeighborMainAddress.push_back(*n);
}

void Tc::printData() {
	std::cout << "ANSN = "; 
	std::cout << (int)mANSN << std::endl;
	
	std::list<IPv6> ip = mAdvertisedNeighborMainAddress;
		
    for(std::list<IPv6>::iterator it = ip.begin();it!=ip.end();it++){
      std::cout<<" IP : "<<it->toChar() <<std::endl;
    }
}

int Tc::sendTc() {
	boost::asio::io_service *io_service;
  	boost::asio::ip::udp::endpoint *receiver_endpoint;
  	boost::asio::ip::udp::endpoint *sender_endpoint;
  	boost::asio::ip::udp::socket *socket;
  
	char* send_buf;
  	send_buf = (char*)malloc(sizeof(char)*BUFF_SIZE);

	uint16_t TcSize = 4;
	this->mMessageType = 0x02;
	this->mVTime = 0x00 ;
	this->mTimeToLive = 0x01;	
	this->mHopCount = 0x00;

  	
  	

	// IPV6
  	*(uint16_t*)(send_buf+8) = 0x0000;
	*(uint16_t*)(send_buf+8) = 0x0000;
	*(uint16_t*)(send_buf+8) = 0x0000;
	*(uint16_t*)(send_buf+8) = 0x0000;
  	*(uint16_t*)(send_buf+16) = mOriginatorAddress->getScope(4); 
	// *(send_buf+17) =0xF2;
	*(uint16_t*)(send_buf+18) = mOriginatorAddress->getScope(5);
	// *(send_buf+19) =0xF4;
	*(uint16_t*)(send_buf+20) = mOriginatorAddress->getScope(6);
	// *(send_buf+21) =0xF6;
	*(uint16_t*)(send_buf+22) = mOriginatorAddress->getScope(7);
	// *(send_buf+23) =0xF8;

  	//Time To Live
  	*(send_buf+24) = mTimeToLive;
  	//HOP COUNT
  	*(send_buf+25) = mHopCount;
  	//Message Sequence Number 
  	*(uint16_t*)(send_buf+26) = mMessageSequenceNumber;
  	
	//Tc MESSAGE :
	// ANSN
	*(uint16_t*)(send_buf+28) = this->getANSN();
	//+2 pour le reserved
	// IPV6 : AdvertisedNeighborMainAddress 
	*(send_buf+30) = this->getReserved();
	
	int c = 32;
	int i = 0;

	for(std::list<IPv6>::iterator it = mAdvertisedNeighborMainAddress.begin(); it != mAdvertisedNeighborMainAddress.end(); it ++){
  		c += 8;
		*(uint16_t*)(send_buf+c) = it->getScope(4);
		c += 2;
		*(uint16_t*)(send_buf+c) = it->getScope(5);
		c += 2;
		*(uint16_t*)(send_buf+c) = it->getScope(6);
		c += 2;
		*(uint16_t*)(send_buf+c) = it->getScope(7);
		c += 2;
		
		TcSize += 16;
		printf("TcSize = %d\n", TcSize);
	}
	
		
	this->mMessageSize = TcSize + 24 ;
	this->mPacketLength = mMessageSize + 4; // 2o

	//packetHeader
	// packetLength
  	*(uint16_t*)send_buf = mPacketLength;
  	// PacketSequenceNumber 
  	*(uint16_t*)(send_buf+2) = mPacketSequenceNumber;
  	
  	//messageHeader
	// messageType 
  	*(send_buf+4) = mMessageType;
	//Vtime
  	*(send_buf+5) = mVTime;
	// messageSize
  	*(uint16_t*)(send_buf+6) = mMessageSize;
  	
  	printf("c = %d\n", c);
  	printf("mMessageSize = %d\n", mMessageSize);

	std::string container (send_buf, c);
	io_service = new  boost::asio::io_service();
	receiver_endpoint = new boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 7171); 
	sender_endpoint = new boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 2050);
	socket = new boost::asio::ip::udp::socket(*io_service,*sender_endpoint);
	socket->send_to(boost::asio::buffer(container),*receiver_endpoint);

	return 0;
}
