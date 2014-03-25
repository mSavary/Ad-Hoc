#ifndef Tc_H
#define Tc_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <list>
#include <math.h>
#include <cstdlib>
#include <string.h>
#include "IPv6.h"
#include "Message.h"

#include <iterator>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;

class Tc: public Message {
private:
	uint16_t mANSN;
	uint16_t mReserved;
	IPv6 *mFromIp;
	std::list<IPv6*> mAdvertisedNeighborMainAddress;

public:
	Tc(uint16_t packetSequenceNumber, IPv6 * originatorAddress,
			uint16_t messageSequenceNumber, std::list<IPv6*> advertisedList);
	Tc(uint16_t packetLength, uint16_t packetSequenceNumber,
			uint8_t messageType, uint8_t vTime, uint16_t messageSize,
			IPv6 * originatorAddress, uint8_t timeToLive, uint8_t hopCount,
			uint16_t messageSequenceNumber,uint16_t Ansn, std::string ip,
			std::list<IPv6*> advertisedList);

	uint16_t getANSN();
	uint16_t getReserved();
	void setReserved(uint16_t res);
	std::list<IPv6*> getAdvertisedNeighborMainAddress();
	void setANSN(uint16_t n);
	inline IPv6* getForwarder(){
		return mFromIp;
	}
	void addAdvertisedNeighborMainAddress(short n, short o, short p, short q);
	void addAdvertisedNeighborMainAddress(IPv6* n);
	void printData();
	int sendTc();
};
#endif
