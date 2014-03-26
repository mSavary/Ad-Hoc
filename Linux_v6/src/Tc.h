/*!
 * \file Node.h
 *
 * \date 1 Février 2014
 * \author Aminatou Mohamadou & Corentin Soriot
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

/*!
 *\class Tc
 *\brief Class Tc represents the Tc type message in the Message structure.
 */
class Tc: public Message {
private:
	uint16_t mANSN;
	uint16_t mReserved;
	IPv6 *mFromIp;/*!< sender of the packet */
	std::list<IPv6*> mAdvertisedNeighborMainAddress;

public:
	/*!
	 * \fn 	Tc(uint16_t packetSequenceNumber, IPv6 * originatorAddress, uint16_t messageSequenceNumber, std::list<IPv6*> advertisedList)
	 * \brief Construct a Tc message with partial informations
	 *
	 * 	\param partial informations
	 */
	Tc(uint16_t packetSequenceNumber, IPv6 * originatorAddress,
			uint16_t messageSequenceNumber, std::list<IPv6*> advertisedList);

	/*!
	 * \fn 	Tc(uint16_t packetLength, uint16_t packetSequenceNumber, uint8_t messageType, uint8_t vTime, uint16_t messageSize, IPv6 * originatorAddress, uint8_t timeToLive, uint8_t hopCount, uint16_t messageSequenceNumber,uint16_t Ansn, std::string ip, std::list<IPv6*> advertisedList)
	 * \brief Construct a Tc message with a complete informations
	 *
	 * 	\param complete informations
	 */
	Tc(uint16_t packetLength, uint16_t packetSequenceNumber,
			uint8_t messageType, uint8_t vTime, uint16_t messageSize,
			IPv6 * originatorAddress, uint8_t timeToLive, uint8_t hopCount,
			uint16_t messageSequenceNumber,uint16_t Ansn, std::string ip,
			std::list<IPv6*> advertisedList);

	/*!
	 * \fn uint16_t getANSN()
	 *
	 * \brief Return the Advertised Neighbor Sequence Number of the Tc field
	 */
	uint16_t getANSN();

	/*!
	 * \fn uint16_t getReserved()
	 *
	 * \brief Return value of the field reserved
	 */
	uint16_t getReserved();

	/*!
	 * \fn 	void setReserved(uint16_t res)
	 * \brief Set the value of the reserved field
	 *
	 * 	\param value of the field
	 */
	void setReserved(uint16_t res);

	/*!
	 * \fn std::list<IPv6*> getAdvertisedNeighborMainAddress()
	 *
	 * \brief Return the list of the AdvertisedNeighborMainAddress
	 */
	std::list<IPv6*> getAdvertisedNeighborMainAddress();

	/*!
	 * \fn 	void setANSN(uint16_t n)
	 * \brief Set the value of the Advertised Neighbor Sequence Number field
	 *
	 * 	\param value of the field
	 */
	void setANSN(uint16_t n);

	/*!
	 * \fn inline IPv6* getForwarder()
	 *
	 * \brief Return the IP of the originator of the packet
	 */
	inline IPv6* getForwarder(){
		return mFromIp;
	}

	/*!
	 * \fn void	addAdvertisedNeighborMainAddress(short n, short o, short p, short q)
	 * \brief Set the IP of the AdvertisedNeighborMainAddress
	 *
	 * 	\param the 4 last values of the IPv6 address
	 */
	void addAdvertisedNeighborMainAddress(short n, short o, short p, short q);

	/*!
	 * \fn void	addAdvertisedNeighborMainAddress(IPv6* n)
	 * \brief Set the IP of the AdvertisedNeighborMainAddress
	 *
	 * 	\param the IPv6 addressof the AdvertisedNeighborMainAddress
	 */
	void addAdvertisedNeighborMainAddress(IPv6* n);

	/*!
	 * \fn void printData()
	 *
	 * \brief Print all the field of the Tc class
	 */
	void printData();

	/*!
	 * \fn int sendTc()
	 *
	 * \brief Send the Tc message with the values set in the fields
	 */
	int sendTc();
};
#endif
