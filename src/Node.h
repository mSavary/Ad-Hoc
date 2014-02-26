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

class Dest{
private:
	int mHops;
	IPv6 *mIp;
public:
	Dest(int hop, IPv6* ip);
	inline IPv6* getIp(){
		return mIp;
	}
	inline int getHops(){
		return mHops;
	}

};

class Node {
private:

	/*
	 * *attributes
	 */

	bool mMpr;
	std::string mInterface;
	IPv6 *mMyIp;
	boost::thread mTimerHello, mTimerTc;
	boost::mutex mMutexIP, mMutexTwoHopTable, mMutexNeighborTable,mMutexSystem,mMutexDestTable;
	std::list<Route> mNeighborTable, mTwoHopNeighborTable, mDestTable;
	std::list<IPv6> mNeighborIP, mMyMprList;
	std::list<std::list<IPv6> > mTwoHopNeighborIP;
	std::list<Dest> mDestPath;

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
	 * Func addDestTable()
	 *
	 * @params : route is the route to add to the destTable
	 */
	int addDestTable(Route *route);



	/**
	 * Func addDestPath()
	 *  	add a new Dest to mDestPath
	 */
	int addDestPath(IPv6 *ip,int hop);

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

	inline std::string getInterface() {
		return mInterface;
	}

	inline IPv6* getMyIp(){
		return mMyIp;
	}

	inline std::list<Route> getNeighborTable() {
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

	inline void lockSystem(){
		mMutexSystem.lock();
	}

	inline void releaseSystem(){
		mMutexSystem.unlock();
	}
	inline void setNeighborTable (std::list<Route> liste){
		mNeighborTable=liste;
	}

	inline void setTwoHopNeighborTable (std::list<Route> liste){
		mTwoHopNeighborTable=liste;
	}

	inline std::list<Route> getDestTable(){
		return mDestTable;
	}

	inline void setDestTable(std::list<Route> liste){
		mDestTable=liste;
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

};

//#endif // CLIENT_H
