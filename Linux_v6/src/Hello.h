/*!
 * \file Hello.h
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

/*!
 *\class HelloNeighborList
 *\brief Class HelloNeighborList contain a list of neighbor node address neighbor node.
 *		
 */

using namespace std;
using boost::asio::ip::udp;

class HelloNeighborList {

private:
	uint8_t mLinkCode;/*!< specifies information about the link between the
         interface of the sender and the following list of neighbor
         interfaces.*/
	uint8_t mReserved;
	uint16_t mLinkMessageSize;/*!<The size of the link message.*/
	std::list<IPv6*> mNeighborsAddrList;/*!< list of neighbor node address neighbor node.*/

/*!<*/
public:

	/*!
	 * \fn HelloNeighborList(uint8_t linkCode, std::list<IPv6*> NeighborsAddrList)
	 * \brief Create your NeighborList
	 */
	HelloNeighborList(uint8_t linkCode, std::list<IPv6*> NeighborsAddrList);

	/*!
	 * \fn uint8_t getLinkCode()
	 * \brief Allow you to get LinkCode
	 * \return uint8_t 
	 */
	uint8_t getLinkCode();

	/*!
	 * \fn std::list<IPv6*> getNeighborsAddrList()
	 * \brief Allow you to get your NeighborsAddrList
	 * \return list<IPv6*>
	 */
	std::list<IPv6*> getNeighborsAddrList();

	/*!
	 * \fn uint16_t getLinkMessageSize()
	 * \brief Allow you to get LinkMessageSize
	 * \return uint16_t 
	 */
	uint16_t getLinkMessageSize();

	/*!
	 * \fn uint8_t getLinkMessageSize()
	 * \brief Allow you to get Reserved
	 * \return uint8_t 
	 */
	uint8_t getReserved();
};

/*!
 *\class Hello
 *\brief Class Hello create a message Hello and send it
 *		
 */

class Hello: public Message {

private:
	uint16_t mReserved;
	uint8_t mHTime;/*!<specifies the HELLO emission interval used by the
         node*/
	uint8_t mWillingness;/*!<This field specifies the willingness of a node to carry and
         forward traffic for other nodes.*/
	std::list<HelloNeighborList*> mNeighbors;/*!< list of helloNeighbor node address neighbor node.*/

public:

	/*!
	 * \fn Hello(uint16_t packetSequenceNumber, IPv6 * originatorAdress,
			uint16_t messageSequenceNumber, std::list<IPv6*> neighborList,
			std::list<IPv6*> mprList)
	 * \brief Create your paquet Hello
	 */
	Hello(uint16_t packetSequenceNumber, IPv6 * originatorAdress,
			uint16_t messageSequenceNumber, std::list<IPv6*> neighborList,
			std::list<IPv6*> mprList);

	/*!
	 * \fn Hello(uint8_t willingness, uint8_t hTime, uint16_t packetLength,
			uint16_t packetSequenceNumber, uint8_t messageType, uint8_t vTime,
			uint16_t messageSize, IPv6 * originatorAddress, uint8_t timeToLive,
			uint8_t hopCount, uint16_t messageSequenceNumber,
			std::list<HelloNeighborList*> neighbors)
	 * \brief Create your paquet Hello
	 */
	Hello(uint8_t willingness, uint8_t hTime, uint16_t packetLength,
			uint16_t packetSequenceNumber, uint8_t messageType, uint8_t vTime,
			uint16_t messageSize, IPv6 * originatorAddress, uint8_t timeToLive,
			uint8_t hopCount, uint16_t messageSequenceNumber,
			std::list<HelloNeighborList*> neighbors);

	/*!
	 * \fn uint16_t getReserved()
	 * \brief Allow you to get Reserved
	 * \return uint16_t 
	 */
	uint16_t getReserved();

	/*!
	 * \fn uint8_t getHTime()
	 * \brief Allow you to get HTime
	 * \return uint8_t
	 */
	uint8_t getHTime();

	/*!
	 * \fn uint8_t getWillingness()
	 * \brief Allow you to get Willingness
	 * \return uint8_t
	 */
	uint8_t getWillingness();

	/*!
	 * \fn std::list<HelloNeighborList*> getNeighbors()
	 * \brief Allow you to get your list of Neighbors
	 * \return list<HelloNeighborList*>
	 */
	std::list<HelloNeighborList*> getNeighbors();

	/*!
	 * \fn void setReserved(uint16_t reserved)
	 * \brief Replace the reserved by a new one
	 */
	void setReserved(uint16_t reserved);


	/*!
	 * \fn void setHTime(uint8_t n)
	 * \brief Replace the HTime by a new one
	 */
	void setHTime(uint8_t n);

	/*!
	 * \fn void setWillingness(uint8_t n)
	 * \brief Replace the Willingness by a new one
	 */
	void setWillingness(uint8_t n);

	/*!
	 * \fn void setNeighbors(HelloNeighborList* n)
	 * \brief Replace the Neighbor list by a new one
	 */
	void setNeighbors(HelloNeighborList* n);

	/*!
	 * \fn void printData()
	 * \brief Allow you to print all information that you have enter
	 */
	void printData();

	/*!
	 * \fn int sendHello()
	 * \brief Allow you to send your paquet Hello
	 */
	int sendHello();
};
#endif
