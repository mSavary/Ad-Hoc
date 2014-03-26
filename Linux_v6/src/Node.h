/*!
 * \file Node.h
 *
 * \date 1 Février 2014
 * \author Joran LeCallonec & Savary Maxime
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

#ifndef NODE_H
#define NODE_H

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

/*!
 *\class Node
 *\brief Class Node represent yourself into the network and handle every route that you need to communicate through it.
 *		It allow you to notify the kernel to Delete, Update or Add new route and also it sends your Hello and Tc.
 *		It defines your IPv6, interface.
 *		It selects your MPR and also save which other node as select you for MPR.
 */
class Node {
private:
	bool mMpr;/*!< define if you are MPR or not*/
	std::string mInterface; /*!< your interface */
	IPv6 *mMyIp;/*< your IPv6*/
	boost::mutex mMutexMprIP,
				mMutexNeighborIP,
				mMutexAdvNeighborIP,
				mMutexDestIP,
				mMutexTwoHopIP,
				mMutexTwoHopTable,
				mMutexNeighborTable,
				mMutexSystem,
				mMutexDestTable;/*!< every Mutex needed to protect the access to critical Data*/
	std::list<Route*>   mNeighborTable,
						mTwoHopNeighborTable,
						mDestTable;/*!< every routing table of the software store to apply changements to the kernel routing table*/
	std::list<IPv6*> mNeighborIP,
					 mMyMprList,
					 mAdvertisedNeighborList,
					 mDestIP;/*!< every IP of your network and IP of nodes that select you as MPR*/
	std::list<std::list<IPv6*> > mTwoHopNeighborIP;/*!< IP of TWO HOPS neighbor */

	/*!
	 * \fn void sendHello()
	 * 	\brief Send a Hello to every neighbor when mTimerHello is reach
	 *			(MUST NOT be transmitted)
	 */
	void sendHello();

	/*!
	 * \fn void sendTc()
	 * 	\brief Send a Tc message to every neighbor when mTimerTc is reach
	 *			(MUST be transmitted by every MPR neighbor)
	 *			(MUST be a MPR)
	 */
	void sendTc();

	/*!
	 * \fn 	int addTwoHopNeighborTable(Route *route);
	 * \brief Allowing to add a route to the TwoHop Table route
	 *
	 * 	\param route the route to add
	 * 	\return int result of the operation : success or not
	 */
	int addTwoHopNeighborTable(Route *route);

	/*!
	 * \fn 	int addNeighborTable(Route *route);
	 * \brief Allowing to add a route to the neighbor Table route
	 *
	 * 	\param route the route to add
	 * 	\return int result of the operation : success or not
	 */
	int addNeighborTable(Route *route);

	/*!
	 * \fn int updDestTable (Route *route)
	 * \brief Update a route to dest with a shortest path
	 * 	\param route new route to test
	 * 	\return int result of the operation if it change the route or add or nothing.
	 */
	int updDestTable(Route *route);
	/*!
	 * \fn int delDestTable (Route *route)
	 * \brief Set route to DEL for system
	 * \param route route to del
	 * \return int result of the method if it delete it or not.
	 */
	int delDestTable(Route *route);

	/*!
	 * \fn int selectMpr(std::list<std::list<IPv6*> > TwoHopList, std::list<IPv6*> NeighborList)
	 *
	 * 	\brief Look at the TwoHopNeighborIP and choose the node's MPR by adding the MPR to the mMyMprList.
	 * 			Recursive methods to select every mpr
	 *
	 *  \param TwoHopList the list of twoHop neighbor with their next hop
	 *  \param NeighborList the List of every neighbor which aren't MPR yet
	 * 	\return int 0 if the methods runs well.
	 */
	int selectMpr(std::list<std::list<IPv6*> > TwoHopList,
			std::list<IPv6*> NeighborList);

	/*!
	 * \fn void clearMpr()
	 *
	 *	\brief Clear the MPR list before refreshing selectMpr()
	 */

	inline void clearMpr() {
		mMyMprList.clear();
	}

	/*!
	 * \fn std::string getResCmd(char * cmd)
	 * \brief Getting the result of a syscall
	 *
	 * \param cmd command to get the result
	 * \return std::string result of the syscall
	 *
	 */
	std::string getResCmd(char * cmd);

	/*!
	 * \fn std::string hex2Bin(const std::string& s)
	 * \brief Convert hexadecimal number to binary number
	 *
	 * \param s a hexadecimal string
	 * \return std::string the binary format in string.
	 */
	std::string hex2Bin(const std::string& s);

	/*!
	 * \fn std::string bin2Hex(const std::string& s)
	 * \brief Convert binary number to hexadecimal number
	 *
	 * \param s a binary string
	 * \return std::string the Hexadecimal format in string.
	 */
	std::string bin2Hex(const std::string& s);

	/*!
	 * \fn std::string setInterface()
	 *
	 * \brief get the WiFi interface of your computer by differents syscall
	 *
	 * \return std::string your interface name in string.
	 */
	std::string setInterface();

	/*!
	 * \fn void imMpr()
	 *
	 * \brief  Set mMpr to true to say if the node is MPR for someone
	 * 			call this function when node interface is in a received HELLO: MPR section
	 */

	inline void imMpr() {
		mMpr = true;
	}

public:

	/*!
	 * \fn Node()
	 *
	 * \brief Create your IP and starts all timer for messages sending
	 *       mMpr at false
	 */
	Node();

	virtual ~Node();


	/*!
	 * \fn bool isMpr()
	 * \brief Allow you to know if you are MPR or not
	 * \return bool true if you are MPR else false.
	 */
	inline bool isMpr() {
		return mMpr;
	}

	/*!
	 * \fn std::list<IPv6*> getMprList()
	 * \brief Allow you to get the IP of MPR you select
	 * \return std::list<IPv6*> the list of IPv6 of your MPR.
	 */
	inline std::list<IPv6*> getMprList() {
		return mMyMprList;
	}

	/*!
	 * \fn std::string getInterface()
	 * \brief Allow you to get your interface name.
	 * \return std::string your interface.
	 */
	inline std::string getInterface() {
		return mInterface;
	}

	/*!
	 * \fn IPv6* getMyIp()
	 * \brief Allow you to get your IP
	 * \return IPv6* your IP
	 */
	inline IPv6* getMyIp() {
		return mMyIp;
	}

	/*!
	 * \fn std::list<Route*> getNeighborTable()
	 * \brief Allow you to get your Neighbor routing Table
	 * \return std::list<Route*> the software routing table
	 */
	inline std::list<Route*> getNeighborTable() {
		return mNeighborTable;
	}

	/*!
	 * \fn std::list<Route*> getTwoHopNeighborTable()
	 * \brief Allow you to get your TwoHopNeighbor routing Table
	 * \return std::list<Route*> the software routing table
	 */
	inline std::list<Route*> getTwoHopNeighborTable() {
		return mTwoHopNeighborTable;
	}

	/*!
	 * \fn std::list<IPv6*> getNeighborIP()
	 * \brief Allow you to get the IPv6 list of your neighbor
	 * \return std::list<IPv6*> the list of IPv6
	 */
	inline std::list<IPv6*> getNeighborIP() {
		return mNeighborIP;
	}

	/*!
	 * \fn std::list<IPv6*> getAdvertiseIP()
	 * \brief Allow you to get the IPv6 list of your advertise list (those who select you as MRP).
	 * \return std::list<IPv6*> the list of IPv6
	 */
	inline std::list<IPv6*> getAdvertiseIP (){
		return mAdvertisedNeighborList;
	}

	/*!
	 * \fn std::list<IPv6*> getTwoHopNeighborIP()
	 * \brief Allow you to get the IPv6 dest and next hop list of your two hop neighbor
	 * \return std::list<std::list<IPv6*>> the list of dest and hop IPv6
	 */
	inline std::list<std::list<IPv6*> > getTwoHopNeighborIP() {
		return mTwoHopNeighborIP;
	}

	/*!
	 * \fn void lockSystem()
	 * \brief lock the mutex for every routing table for kernel update
	 */
	inline void lockSystem() {
		mMutexSystem.lock();
	}

	/*!
	 * \fn void releaseSystem()
	 * \brief unlock the mutex for every routing table when kernel update is done.
	 */
	inline void releaseSystem() {
		mMutexSystem.unlock();
	}

	/*!
	 * \fn void setNeighborTable(std::list<Route*> liste)
	 * \brief Replace the neighbor table by a new one
	 * \param liste  the new neighbor table.
	 */
	inline void setNeighborTable(std::list<Route*> liste) {
		mNeighborTable = liste;
	}

	/*!
	 * \fn void setTwoHopNeighborTable(std::list<Route*> liste)
	 * \brief Replace the two hop neighbor table by a new one
	 * \param liste  the new two hop neighbor table.
	 */
	inline void setTwoHopNeighborTable(std::list<Route*> liste) {
		mTwoHopNeighborTable = liste;
	}

	/*!
	 * \fn std::list<Route*> getDestTable()
	 * \brief Allow you to get the dest routing table which regroup every route with a metric >2
	 * \return std::list<Route*> the software routing table
	 */
	inline std::list<Route*> getDestTable() {
		return mDestTable;
	}

	/*!
	 * \fn void setDestTable(std::list<Route*> liste)
	 * \brief Replace the Dest routing table by a new one
	 * \param liste the new routing table
	 */
	inline void setDestTable(std::list<Route*> liste) {
		mDestTable = liste;
	}

	/*!
	 * \fn std::list<IPv6*> getDestIP()
	 * \brief Allow yu to get every Destination IP with a metric > 2
	 * \return std::list<IPv6*> the IPv6 list of every dest IP
	 */
	inline std::list<IPv6*> getDestIP() {
		return mDestIP;
	}

	/*!
	 * \fn int addNeighbor(Route* route);
		\fn int addNeighbor(IPv6* ipDest, IPv6* nextHop, int metric, std::string iface);
	 *	\brief Add a neighbor to the NeighborTable and neighborIP
	 *			also it guarantee that the neighbor you want to add isn't already present.
	 *
	 * \param route the route to add
	 * \param ipDest  destination ip of the route to add
	 * \param nextHop nexthop ip of the route to add ( here it might be NULL)
	 * \param metric metric of the route to add ( here must be 1 )
	 * \param iface interface on which you want to add the route
	 *
	 * \return int if success or not success
	 */
	int addNeighbor(Route* route);
	int addNeighbor(IPv6* ipDest, IPv6* nextHop, int metric, std::string iface);

	/*!
	 * \fn int addTwoHopNeighbor(Route* route);
		\fn int addTwoHopNeighbor(IPv6* ipDest, IPv6* nextHop, int metric, std::string iface);
	 *	\brief Add a two hop neighbor to the mTwoHopNeighborTable and to the twoHopIP
	 *			it also guarantee that the route doesn't exist yet and that there is
	 *			a valid nexthop.
	 *
	 * \param route the route to add
	 * \param ipDest  destination ip of the route to add
	 * \param nextHop nexthop ip of the route to add ( here it might be a neighbor IP)
	 * \param metric metric of the route to add ( here must be 2 )
	 * \param iface interface on which you want to add the route
	 *
	 * \return int if success or not success
	 */
	int addTwoHopNeighbor(Route* route);
	int addTwoHopNeighbor(IPv6* ipDest, IPv6* nextHop, int metric,
			std::string iface);

	/*!
	 * \fn int addDestTable(Route *route)
	 * \brief Add a route with a metric >2 to the dest table
	 *
	 * \param  route  the route to add to the destTable
	 */
	int addDestTable(Route *route);

	/*!
	 * \fn int addAdvertisedNeighbor(IPv6* ip)
	 * \brief Add the IP of neighbor which choose you as MPR
	 *
	 * \param  ip IPv6 of the neighbor who choose you
	 */
	int addAdvertisedNeighbor(IPv6* ip);

	/*!
	 * \fn int delAdvertisedNeighbor(IPv6* ip)
	 * \brief Delete from the advertisedlist a neighbor which erase me from his mpr
	 *
	 * \params  ip IPv6 of the neighbor that unselect you
	 */
	int delAdvertisedNeighbor(IPv6* ip);

	/*!
	 * \fn int delNeighbor(Route* route)
	 *	\brief Notify to the neighbor routing table to erase the route from the kernel or erase it if it wasn't already into the kernel
	 *			Delete the IP of the route from the Neighbor IP list.
	 *			Once these are done it update MPR list.
	 *
	 * \param route  route to delete
	 *
	 * \return int if success or not.
	 */

	int delNeighbor(Route* route);

	/*!
	 * \fn int delNeighbor(IPv6* ipToDelete)
	 *	\brief Notify to the neighbor routing table to erase the route from the kernel or erase it if it wasn't already into the kernel
	 *			Delete the IP of the route from the Neighbor IP list.
	 *			Once these are done it update MPR list.
	 *
	 * \param ipToDelete IPv6 destination of the route to delete
	 *
	 * \return int if success or not.
	 */
	int delNeighbor(IPv6* ipToDelete);

	/*!
	 * \fn int delTwoHopNeighbor(Route* route)
	 *	\brief Delete a neighbor+1 from the mTwoHopNeighborTable and also
	 *			delete from the Two Hop neighbor IP and guarantee if the route exist.
	 *			it refresh the Mpr list.
	 *
	 *
	 * \param route the route you want to Delete
	 *
	 * \return int if success or not
	 */

	int delTwoHopNeighbor(Route* route);

	/*!
	 * \fn int delTwoHopNeighbor (IPv6* ipToDelete, IPv6* ipHopToDelete)
	 *	\brief Delete a neighbor+1 from the mTwoHopNeighborTable and also
	 *			delete from the Two Hop neighbor IP and guarantee if the ipToDelete with the nextHop exist.
	 *			it refresh the mpr list
	 *
	 *
	 * \param ipToDelete ip destination to delete
	 * \param ipHopToDelete IPv6 of the nextHOP to delete
	 * \return int if success or not
	 */
	int delTwoHopNeighbor(IPv6* ipToDelete, IPv6* ipHopToDelete);

	/*!
	 * \fn int delTwoHopNeighbor (IPv6* ipToDelete, bool nextHop)
	 *	\brief Delete a neighbor+1 from the mTwoHopNeighborTable and also
	 *			delete from the Two Hop neighbor IP and guarantee if the route exist.
	 *			It refresh the Mpr list.
	 *
	 *
	 * \param ipToDelete IPv6 to delete
	 * \param nextHop to know if ipToDelete is the next hop or the destination
	 *
	 * \return int if success or not
	 */
	int delTwoHopNeighbor(IPv6* ipToDelete, bool nextHop);

	/*!
	 * \fn std::string macToIpv6 ()
	 * \brief Create the node's IPv6 adress by using the node's mac adress
	 *
	 * \return std::string the string format of the IPv6
	 */
	std::string macToIpv6();

	/*!
	 * \fn int checkDest(Route *route)
	 *  \brief Check if a destination is already set or not and see if it's a shorter path if it exists.
	 *
	 *	\param route the route we want to test for adding, updating to the destination table
	 *  \return int depends if we update or add a route.
	 */
	int checkDest(Route *route);

	/*!
	 * \fn int delDest(Route *route)
	 * \brief delete a destination with a metric > 2 and also delete it from the software routing table
	 * \param route the route you want to delete from kernel and destIP
	 * \return int result of your request.
	 */
	int delDest(Route *route);

	/*!
	 * \fn int delDest(IPv6 *ip)
	 * \brief delete a destination with a metric > 2 and also delete it from the software routing table
	 * \param route the route you want to delete from kernel and destIP
	 * \return int result of your request.
	 */
	int delDest(IPv6 *ip);

	/*!
	 * \fn int start ()
	 *
	 * 	\brief	Start to send Hello and TC at regular interval see OLSR's rfc
	 */
	inline int start() {
		boost::thread TimerHello = boost::thread(&Node::sendHello, this);
		boost::thread TimerTc = boost::thread(&Node::sendTc, this);
		return 1;
	}
};

#endif
