#include "Message.h"

using namespace std;

Message::Message(){

}

Message::Message(uint16_t packetSequenceNumber, uint8_t messageType,
		IPv6* originatorAddress, uint8_t hopCount,
		uint16_t messageSequenceNumber) :
		mPacketSequenceNumber(packetSequenceNumber),mMessageType(messageType), mOriginatorAddress(
				originatorAddress), mHopCount(hopCount), mMessageSequenceNumber(
				messageSequenceNumber) {
}

Message::Message(uint16_t packetLength, uint16_t packetSequenceNumber,
		uint8_t messageType, uint8_t vTime, uint16_t messageSize,
		IPv6 * originatorAddress, uint8_t timeToLive, uint8_t hopCount,
		uint16_t messageSequenceNumber) :
		mPacketLength(packetLength), mPacketSequenceNumber(
				packetSequenceNumber),mMessageType(messageType), mVTime(vTime), mOriginatorAddress(
				originatorAddress), mTimeToLive(timeToLive), mHopCount(
				hopCount), mMessageSequenceNumber(messageSequenceNumber) {
}

uint16_t Message::getPacketLength() {
	return mPacketLength;
}

uint16_t Message::getPacketSequenceNumber() {
	return mPacketSequenceNumber;
}

uint8_t Message::getMessageType() {
	return mMessageType;
}

uint8_t Message::getVTime() {
	return mVTime;
}

uint16_t Message::getMessageSize() {
	return mMessageSize;
}

IPv6* Message::getOriginatorAddress() {
	return mOriginatorAddress;
}

uint8_t Message::getTimeToLive() {
	return mTimeToLive;
}

uint8_t Message::getHopCount() {
	return mHopCount;
}

uint16_t Message::getMessageSequenceNumber() {
	return mMessageSequenceNumber;
}

void Message::setPacketLength(uint16_t n) {
	mPacketLength = n;
}

void Message::setPacketSequenceNumber(uint16_t n) {
	mPacketSequenceNumber = n;
}

void Message::setMessageType(uint8_t n) {
	mMessageType = n;
}

void Message::setVTime(uint8_t n) {
	mVTime = n;
}

void Message::setMessageSize(uint16_t n) {
	mMessageSize = n;
}

void Message::setOriginatorAddress(IPv6* n) {
	mOriginatorAddress = n;
}

void Message::setTimeToLive(uint8_t n) {
	mTimeToLive = n;
}

void Message::setHopCount(uint8_t n) {
	mHopCount = n;
}

void Message::setMessageSequenceNumber(uint16_t n) {
	mMessageSequenceNumber = n;
}

void Message::printData() {
	std::cout << "packetLength = " << mPacketLength << std::endl;
	std::cout << "packetSequenceNumber = " << mPacketSequenceNumber
			<< std::endl;
	std::cout << "messageType = " << (int) mMessageType << std::endl;
	std::cout << "vTime = " << (int) mVTime << std::endl;
	std::cout << "messageSize = " << mMessageSize << std::endl;
	std::cout << "originatorAddress = " << mOriginatorAddress->toChar()
			<< std::endl;
	std::cout << "timeToLive = " << (int) mTimeToLive << std::endl;
	std::cout << "hopCount = " << (int) mHopCount << std::endl;
	std::cout << "messageSequenceNumber = " << mMessageSequenceNumber
			<< std::endl;
}
