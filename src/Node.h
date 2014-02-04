/**
*Class Node 
*    regroup all elements for the node like the tableroute of all Neigbor and two hop neighbor
*    and the interface use by the machine
*    this class also send Hello and Tc Messages at each times requires by the OLSR rfc
*/

//#ifndef NODE_H
//#define NODE_H

#define MAX_NEIGHBOR 30
#define MAX_TWOHOPNEIGHBOR MAX_NEIGHBOR*MAX_NEIGHBOR

#include <iostream>
#include <list>
#include "Route.h"
#include "IPv6.h"
class Node
{
    private:

	bool mMpr; // true if MPR
	IPv6 *mInterface;
    int mTimerHello,
        mTimerTc;

    std::list<Route> mNeighborTable,
					 mTwoHopNeighborTable;

    std::list<IPv6> mNeighborIP,
    				mMyMprList;
    std::list<std::list<IPv6> >	mTwoHopNeighborIP;
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
     * Private func to add a route to the Table route ( call by public function see below )
     */
	int addTwoHopNeighborTable(Route *route);
    int addNeighborTable(Route *route);

    /**
     * Func selectMpr()
     *
     * 	Look at the TwoHopNeighborIP and choose the node's MPR by adding the MPR to the mMyMprList
     */
    void selectMpr();

    /**
     * Func recursivSelectMpr()
     *
     *   call by selectMPR and select all MPR for all 2HOP neighbor
     */
    void recursivSelectMpr(std::list<std::list<IPv6> > liste);

    public:
	/**
	* Constructor Node()
	*
	* @init Create your IP and starts all timer for messages sending
	*       mMpr at false
	*/
    Node();
	inline bool isMpr(){
	    return mMpr;
	}
	
	inline std::list<IPv6> getMprList(){
		return mMyMprList;
	}

	inline IPv6* getInterface(){
		return mInterface;
	}

	inline std::list<Route> getNeighborTable(){
        	std::cout<<mNeighborTable.size()<<std::endl;
	    return mNeighborTable;
	}
	inline std::list<Route> getTwoHopNeighborTable(){
	    return mTwoHopNeighborTable;
	}

	inline std::list<IPv6> getNeighborIP(){
		return mNeighborIP;
	}

	inline std::list<std::list<IPv6> > getTwoHopNeighborIP(){
		return mTwoHopNeighborIP;
	}
    /**
     * Func imMpr()
     *
     *   set mMpr to true to say if the node is MPR for someone ( call this function when node interface is in a received HELLO: MPR section)
     */

	inline void imMpr(){
		mMpr=true;
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
    int addNeighbor(IPv6* ipDest, IPv6* ipSource, int metric, IPv6* nextHop,int action);
	
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
	int addTwoHopNeighbor(IPv6* ipDest, IPv6* ipSource, int metric, IPv6* nextHop,int action);

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
	int delTwoHopNeighbor(IPv6* ipToDelete);
};

//#endif // CLIENT_H
