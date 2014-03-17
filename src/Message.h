#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <list>
#include <math.h>
#include <cstdlib>
#include <string.h>
#include "IPv6.h"

class Message {
protected:
	// packet info
	uint16_t mPacketLength;
	uint16_t mPacketSequenceNumber;
	// message info
	uint8_t mMessageType;
	uint8_t mVTime;
	uint16_t mMessageSize;
	IPv6* mOriginatorAddress;
	uint8_t mTimeToLive;
	uint8_t mHopCount;
	uint16_t mMessageSequenceNumber;

public:
	Message(uint16_t packetSequenceNumber, uint8_t messageType,
			IPv6* originatorAddress, uint8_t hopCount,
			uint16_t messageSequenceNumber);
	Message(uint16_t packetLength, uint16_t packetSequenceNumber,
			uint8_t messageType, uint8_t vTime, uint16_t messageSize,
			IPv6 * originatorAdress, uint8_t timeToLive, uint8_t hopCount,
			uint16_t messageSequenceNumber);

	uint16_t getPacketLength();
	uint16_t getPacketSequenceNumber();
	uint8_t getMessageType();
	uint8_t getVTime();
	uint16_t getMessageSize();
	IPv6* getOriginatorAddress();
	uint8_t getTimeToLive();
	uint8_t getHopCount();
	uint16_t getMessageSequenceNumber();

	void setPacketLength(uint16_t n);
	void setPacketSequenceNumber(uint16_t n);
	void setMessageType(uint8_t n);
	void setVTime(uint8_t n);
	void setMessageSize(uint16_t n);
	void setOriginatorAddress(IPv6* n);
	void setTimeToLive(uint8_t n);
	void setHopCount(uint8_t n);
	void setMessageSequenceNumber(uint16_t n);
	void printData();
};
#endif
