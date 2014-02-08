/**
 *Class Node
 *    regroup all elements for the node like the tableroute of all Neigbor and two hop neighbor
 *    and the interface use by the machine
 *    this class also send Hello and Tc Messages at each times requires by the OLSR rfc
 */

//#ifndef NODE_H
//#define NODE_H
#define HELLO_INTERVAL 2
#define TC_INTERVAL 5

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

class Node {
private:

	/*
	 * *attributes
	 */

	bool mMpr;
	IPv6 *mInterface;
	boost::thread mTimerHello, mTimerTc;
	boost::mutex mMutexIP, mMutexTwoHopTable, mMutexNeighborTable;
	std::list<Route> mNeighborTable, mTwoHopNeighborTable;
	std::list<IPv6> mNeighborIP, mMyMprList;
	std::list<std::list<IPv6> > mTwoHopNeighborIP;

	/*
	 * Private methods
	 */

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
	 * Allowing to add a route to the Table route ( call by public function see below )
	 */
	int addTwoHopNeighborTable(Route *route);
	int addNeighborTable(Route *route);

	/**
	 * Func selectMpr()
	 *
	 * 	Look at the TwoHopNeighborIP and choose the node's MPR by adding the MPR to the mMyMprList
	 */
	int selectMpr(std::list<std::list<IPv6> > TwoHopList,
			std::list<IPv6> NeighborList);

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
	std::string getMacAdress(char * cmd);

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

	inline std::list<IPv6> getMprList() {
		return mMyMprList;
	}

	inline IPv6* getInterface() {
		return mInterface;
	}

	inline std::list<Route> getNeighborTable() {
		std::cout << mNeighborTable.size() << std::endl;
		return mNeighborTable;
	}
	inline std::list<Route> getTwoHopNeighborTable() {
		return mTwoHopNeighborTable;
	}

	inline std::list<IPv6> getNeighborIP() {
		return mNeighborIP;
	}

	inline std::list<std::list<IPv6> > getTwoHopNeighborIP() {
		return mTwoHopNeighborIP;
	}
	/**
	 * Func imMpr()
	 *
	 *   set mMpr to true to say if the node is MPR for someone ( call this function when node interface is in a received HELLO: MPR section)
	 */

	inline void imMpr() {
		mMpr = true;
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
	int addNeighbor(IPv6* ipDest, IPv6* ipSource, int metric, IPv6* nextHop);

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
	int addTwoHopNeighbor(IPv6* ipDest, IPv6* ipSource, int metric,
			IPv6* nextHop);

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

	/**
	 * Func erase**
	 * 	Erase a route from the differents Table after we delete it from the kernel
	 *  params : IP of the route to del
	 */
	int eraseTwoHop(IPv6 *ipDestToDel, IPv6 *ipNextHopToDel);
	int eraseNeighbor(IPv6 *ipToDel);

	/**
	 * Func macToIPv6
	 * Create the node's IPv6 adress by using the node's mac adress
	 *
	 * @return an IPv6
	 */
	std::string macToIpv6();
};

//#endif // CLIENT_H
