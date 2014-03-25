#ifndef HELLO_H
#define HELLO_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <list>
#include <math.h>
#include <cstdlib>
#include "const.h"
#include <string.h>
#include "IPv6.h"
#include "Message.h"

#include <iterator>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;

class HelloNeighborList {

private:
	uint8_t mLinkCode;
	uint8_t mReserved;
	uint16_t mLinkMessageSize;
	std::list<IPv6*> mNeighborsAddrList;

public:
	HelloNeighborList(uint8_t linkCode, std::list<IPv6*> NeighborsAddrList);

	uint8_t getLinkCode();
	std::list<IPv6*> getNeighborsAddrList();
	uint16_t getLinkMessageSize();
	uint8_t getReserved();
};

class Hello: public Message {

private:
	uint16_t mReserved;
	uint8_t mHTime;
	uint8_t mWillingness;
	std::list<HelloNeighborList*> mNeighbors;

public:
	Hello(uint16_t packetSequenceNumber, IPv6 * originatorAdress,
			uint16_t messageSequenceNumber, std::list<IPv6*> neighborList,
			std::list<IPv6*> mprList);
	Hello(uint8_t willingness, uint8_t hTime, uint16_t packetLength,
			uint16_t packetSequenceNumber, uint8_t messageType, uint8_t vTime,
			uint16_t messageSize, IPv6 * originatorAddress, uint8_t timeToLive,
			uint8_t hopCount, uint16_t messageSequenceNumber,
			std::list<HelloNeighborList*> neighbors);

	uint16_t getReserved();
	uint8_t getHTime();
	uint8_t getWillingness();
	std::list<HelloNeighborList*> getNeighbors();
	void setReserved(uint16_t reserved);
	void setHTime(uint8_t n);
	void setWillingness(uint8_t n);
	void setNeighbors(HelloNeighborList* n);
	void printData();
	int sendHello();
};
#endif
