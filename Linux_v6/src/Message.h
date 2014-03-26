/*!
 * \file Message.h
 *
 * \date 1 Février 2014
 * \author Mohamadou Aminatou & Soriot Corentin & Jiang Zheng & Li Jitao
 */

/*
 * This file is part of Ad-Hoc Networks an app base on OLSR to handle Ad-Hoc
 *  network.
 *
 * Copyright (c) 2014-2014 Gilles Guette <>
 * Copyright (c) 2014-2014 ISTIC http://www.istic.univ-rennes1.fr/
 * Copyright (c) 2014-2014 SUPELEC http://www.supelec.fr/rennes
 *
 * See the AUTHORS or Authors.txt file for copyright owners and
 * contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

/*!
 *\class Message
 *\brief Class Message create a message
 *		
 */

class Message {
protected:

	
	// packet info
	uint16_t mPacketLength;/*!<The Packet Length*/
	uint16_t mPacketSequenceNumber;/*!<The Packet Sequence Number*/
	// message info
	uint8_t mMessageType;/*!<indicates which type of message is to be found in
         the "MESSAGE" part*/
	uint8_t mVTime;/*!<indicates for how long time after reception a node
         MUST consider the information contained in the message as
         valid*/
	uint16_t mMessageSize;/*!<the size of this message*/
	IPv6* mOriginatorAddress;/*!<the main address of the node*/
	uint8_t mTimeToLive;/*!<contains the maximum number of hops a message will
         be transmitted*/
	uint8_t mHopCount;/*!<contains the number of hops a message has attained*/
	uint16_t mMessageSequenceNumber;/*!<The message Sequence Number*/

public:
	
	/*!
	 * \fn Message()
	 * \brief Create your Message
	 */	
	Message();

	/*!
	 * \fn Message(uint16_t packetSequenceNumber, uint8_t messageType,
			IPv6* originatorAddress, uint8_t hopCount,
			uint16_t messageSequenceNumber)
	 * \brief Create your Message
	 * TODO \param !
	 */
	Message(uint16_t packetSequenceNumber, uint8_t messageType,
			IPv6* originatorAddress, uint8_t hopCount,
			uint16_t messageSequenceNumber);

	/*!
	 * \fn Message(uint16_t packetLength, uint16_t packetSequenceNumber,
			uint8_t messageType, uint8_t vTime, uint16_t messageSize,
			IPv6 * originatorAdress, uint8_t timeToLive, uint8_t hopCount,
			uint16_t messageSequenceNumber)
	 * \brief Create your Message
	 * TODO \param !
	 */
	Message(uint16_t packetLength, uint16_t packetSequenceNumber,
			uint8_t messageType, uint8_t vTime, uint16_t messageSize,
			IPv6 * originatorAdress, uint8_t timeToLive, uint8_t hopCount,
			uint16_t messageSequenceNumber);


	/*!
	 * \fn uint16_t getPacketLength()
	 * \brief Allow you to get PacketLength
	 * \return uint16_t 
	 */
	uint16_t getPacketLength();

	/*!
	 * \fn uint16_t getPacketSequenceNumber()
	 * \brief Allow you to get PacketSequenceNumber
	 * \return uint16_t 
	 */
	uint16_t getPacketSequenceNumber();

	/*!
	 * \fn uint8_t getMessageType()
	 * \brief Allow you to get MessageType
	 * \return uint8_t 
	 */
	uint8_t getMessageType();

	/*!
	 * \fn uint8_t getVTime()
	 * \brief Allow you to get VTime()
	 * \return uint8_t 
	 */
	uint8_t getVTime();

	/*!
	 * \fn uint16_t getMessageSize()
	 * \brief Allow you to get MessageSize
	 * \return uint16_t 
	 */
	uint16_t getMessageSize();

	/*!
	 * \fn IPv6* getOriginatorAddress()
	 * \brief Allow you to get OriginatorAddress
	 * \return IPv6*
	 */
	IPv6* getOriginatorAddress();

	/*!
	 * \fn uint8_t getTimeToLive()
	 * \brief Allow you to get TimeToLive
	 * \return uint8_t 
	 */
	uint8_t getTimeToLive();

	/*!
	 * \fn uint8_t getHopCount()
	 * \brief Allow you to get HopCount
	 * \return uint16_t 
	 */
	uint8_t getHopCount();

	/*!
	 * \fn uint16_t getMessageSequenceNumber()
	 * \brief Allow you to get MessageSequenceNumber
	 * \return uint16_t 
	 */
	uint16_t getMessageSequenceNumber();


	/*!
	 * \fn void setPacketLength(uint16_t n)
	 * \brief Replace the PacketLength by a new one
	 * TODO \param !
	 */
	void setPacketLength(uint16_t n);

	/*!
	 * \fn void setPacketSequenceNumber(uint16_t n)
	 * \brief Replace the PacketSequenceNumber by a new one
	 * TODO \param !
	 */
	void setPacketSequenceNumber(uint16_t n);

	/*!
	 * \fn void setMessageType(uint8_t n)
	 * \brief Replace the MessageType by a new one
	 * TODO \param !
	 */
	void setMessageType(uint8_t n);

	/*!
	 * \fn void setVTime(uint8_t n)
	 * \brief Replace the VTime by a new one
	 * TODO \param !
	 */
	void setVTime(uint8_t n);

	/*!
	 * \fn void setMessageSize(uint16_t n)
	 * \brief Replace the MessageSize by a new one
	 * TODO \param !
	 */
	void setMessageSize(uint16_t n);

	/*!
	 * \fn void setOriginatorAddress(IPv6* n)
	 * \brief Replace the OriginatorAddress by a new one
	 * TODO \param !
	 */
	void setOriginatorAddress(IPv6* n);

	/*!
	 * \fn void setTimeToLive(uint8_t n)
	 * \brief Replace the TimeToLive by a new one
	 * TODO \param !
	 */
	void setTimeToLive(uint8_t n);

	/*!
	 * \fn void setHopCount(uint8_t n)
	 * \brief Replace the HopCount by a new one
	 * TODO \param !
	 */
	void setHopCount(uint8_t n);

	/*!
	 * \fn void setMessageSequenceNumber(uint16_t n)
	 * \brief Replace the MessageSequenceNumber by a new one
	 * TODO \param !
	 */
	void setMessageSequenceNumber(uint16_t n);

	/*!
	 * \fn void printData()
	 * \brief Allow you to print all information that you have enter
	 */
	void printData();
};
#endif
