/*!
 * \file Node.h
 *
 * \date 1 Février 2014
 * \author Aminatou Mohamadou et Corentin Soriot
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
	IPv6 *mFromIp;/*!< sender of the paquet */
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
