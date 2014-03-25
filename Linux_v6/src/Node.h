/*
 * Node.h
 *
 *  Created on: 1 Février 2014
 *      Author: Joran LeCallonec & Savary Maxime
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

//#ifndef NODE_H
//#define NODE_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <list>
#include <boost/thread/thread.hpp>
#include <bitset>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "Route.h"
#include "IPv6.h"
#include "const.h"
#include "Tc.h"
#include "Hello.h"

class Node {
private:
	bool mMpr;
	std::string mInterface;
	IPv6 *mMyIp;
	boost::mutex mMutexMprIP,
				mMutexNeighborIP,
				mMutexAdvNeighborIP,
				mMutexDestIP,
				mMutexTwoHopIP,
				mMutexTwoHopTable,
				mMutexNeighborTable,
				mMutexSystem,
				mMutexDestTable;
	std::list<Route*>   mNeighborTable,
						mTwoHopNeighborTable,
						mDestTable;
	std::list<IPv6*> mNeighborIP,
					 mMyMprList,
					 mAdvertisedNeighborList,
					 mDestIP;
	std::list<std::list<IPv6*> > mTwoHopNeighborIP;

	/**
	 * Func sendHello()
	 * 	send a Hello to every one when mTimerHello is reach
	 *	(MUST NOT be transmitted)
	 */
	void sendHello();

	/**
	 * Func sendTc()
	 * 	send a Tc message to every one when mTimerTc is reach
	 *	(MUST be transmitted by every MPR neighbor)
	 *	(MUST be a MPR)
	 */
	void sendTc();

	/**
	 * FUNC addTable
	 * 		Allowing to add a route to the Table route ( call by public function see below )
	 *
	 * 	@params route the route to add
	 */
	int addTwoHopNeighborTable(Route *route);
	int addNeighborTable(Route *route);

	/**
	 * Func updDestTable
	 * 		update a route to dest with a shortest path
	 * 	@param : route new route to replace
	 */
	int updDestTable(Route *route);
	/**
	 * Func delDestTable
	 * 		set route to DEL for system
	 * 	@params : route route to del
	 */
	int delDestTable(Route *route);

	/**
	 * Func selectMpr()
	 *
	 * 	Look at the TwoHopNeighborIP and choose the node's MPR by adding the MPR to the mMyMprList
	 */
	int selectMpr(std::list<std::list<IPv6*> > TwoHopList,
			std::list<IPv6*> NeighborList);

	/**
	 * Func clearMpr()
	 *
	 *	Clear the MPR list before refreshing selectMpr()
	 */

	inline void clearMpr() {
		mMyMprList.clear();
	}

	/**
	 * Func getMacAdress
	 * Get the mac adress of the wireless interface
	 *
	 * @ param : command to get the mac adress
	 *
	 */
	std::string getResCmd(char * cmd);

	/**
	 * Func Hex2Bin
	 * Convert hexadecimal number to binary number
	 *
	 * @param a hexadecimal string
	 */
	std::string hex2Bin(const std::string& s);

	/**
	 * Func bin2Hex
	 * Convert binary number to hexadecimal number
	 *
	 * @param a binary string
	 */
	std::string bin2Hex(const std::string& s);

	std::string setInterface();
	/**
	 * Func imMpr()
	 *
	 *   set mMpr to true to say if the node is MPR for someone ( call this function when node interface is in a received HELLO: MPR section)
	 */

	inline void imMpr() {
		mMpr = true;
	}

public:

	/**
	 * Constructor Node()
	 *
	 * @init Create your IP and starts all timer for messages sending
	 *       mMpr at false
	 */
	Node();

	virtual ~Node();

	inline bool isMpr() {
		return mMpr;
	}

	inline std::list<IPv6*> getMprList() {
		return mMyMprList;
	}

	inline std::string getInterface() {
		return mInterface;
	}

	inline IPv6* getMyIp() {
		return mMyIp;
	}

	inline std::list<Route*> getNeighborTable() {
		return mNeighborTable;
	}
	inline std::list<Route*> getTwoHopNeighborTable() {
		return mTwoHopNeighborTable;
	}

	inline std::list<IPv6*> getNeighborIP() {
		return mNeighborIP;
	}

	inline std::list<IPv6*> getAdvertiseIP (){
		return mAdvertisedNeighborList;
	}

	inline std::list<std::list<IPv6*> > getTwoHopNeighborIP() {
		return mTwoHopNeighborIP;
	}

	inline void lockSystem() {
		mMutexSystem.lock();
	}

	inline void releaseSystem() {
		mMutexSystem.unlock();
	}
	inline void setNeighborTable(std::list<Route*> liste) {
		mNeighborTable = liste;
	}

	inline void setTwoHopNeighborTable(std::list<Route*> liste) {
		mTwoHopNeighborTable = liste;
	}

	inline std::list<Route*> getDestTable() {
		return mDestTable;
	}

	inline void setDestTable(std::list<Route*> liste) {
		mDestTable = liste;
	}

	inline std::list<IPv6*> getDestIP() {
		return mDestIP;
	}

	/**
	 * Func addNeighbor
	 *	Add a neighbor to the mNeighborTable
	 *
	 * @ param Route or member of the struct Route
	 *
	 * @ return 0 if success
	 *	   1 if !success
	 */
	int addNeighbor(Route* route);
	int addNeighbor(IPv6* ipDest, IPv6* nextHop, int metric, std::string iface);

	/**
	 * Func addTwoHopNeighbor
	 *	Add a neighbor to the mTwoHopNeighborTable
	 *
	 * @ param Route or member of the struct Route
	 *
	 * @ return 0 if success
	 *	   1 if !success
	 */
	int addTwoHopNeighbor(Route* route);
	int addTwoHopNeighbor(IPv6* ipDest, IPv6* nextHop, int metric,
			std::string iface);

	/**
	 * Func addDestTable()
	 *
	 * @params : route is the route to add to the destTable
	 */
	int addDestTable(Route *route);

	/**
	 * Func addAdvertisedNeighbor
	 * add the IP of neighbor which choose me as MPR
	 *
	 *@params : ip of the neighbor
	 */
	int addAdvertisedNeighbor(IPv6* ip);

	/**
	 * Func delAdvertisedNeighbor
	 * delete from the advertisedlist a neighbor which erase me from his mpr
	 *
	 * @params : ip of the neighbor
	 */
	int delAdvertisedNeighbor(IPv6* ip);

	/**
	 * Func delNeighbor
	 *	Delete a neighbor from the mNeighborTable
	 *
	 * @ param ipToDelete ( ip destination to delete)
	 *
	 * @ return 0 if success
	 *	   1 if !success
	 */

	int delNeighbor(Route* route);
	int delNeighbor(IPv6* ipToDelete);

	/**
	 * Func delTwoHopNeighbor
	 *	Delete a neighbor from the mTwoHopNeighborTable
	 *
	 * @ param ipToDelete ( ip destination to delete)
	 *
	 * @ return 0 if success
	 *	   1 if !success
	 */

	int delTwoHopNeighbor(Route* route);
	int delTwoHopNeighbor(IPv6* ipToDelete, IPv6* ipHopToDelete);
	int delTwoHopNeighbor(IPv6* ipToDelete, bool nextHop);

	/**
	 * Func macToIPv6
	 * Create the node's IPv6 adress by using the node's mac adress
	 *
	 * @return an IPv6
	 */
	std::string macToIpv6();

	/**
	 * Func checkDest
	 *     check if a destination is already set or not and see if it's a shorter path if it exists
	 */
	int checkDest(Route *route);

	/**
	 * Func delDest
	 * 	delete a dest from mDestPath and set the route to DEL
	 * 	@params: route route to del
	 */
	int delDest(Route *route);
	int delDest(IPv6 *ip);

	/**
	 * FUNC start
	 *
	 * 		start to send Hello and TC at regular interval see OLSR's rfc
	 */
	inline int start() {
		boost::thread TimerHello = boost::thread(&Node::sendHello, this);
		boost::thread TimerTc = boost::thread(&Node::sendTc, this);
		return 1;
	}
};

//#endif
