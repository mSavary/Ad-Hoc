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
#include "Route.h"
#include "IPv6.h"
class Node
{
    private:

	bool mMpr; // true if MPR
	IPv6 *mInterface;
    int mTimerHello,
        mTimerTc;

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

    Route *mNeighborTable[MAX_NEIGHBOR],
	      *mTwoHopNeighborTable[MAX_TWOHOPNEIGHBOR];

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
	
	inline void beMpr(){
	    mMpr =true;
	}

	inline IPv6* getInterface(){
		return mInterface;
	}

        inline Route* getNeighbor(int i){
	    return mNeighborTable[i];
	}
        inline Route* getTwoHopNeighbor(int i){
	    return mTwoHopNeighborTable[i];
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
        int addNeighbor(Route *route);
        int addNeighbor(int ipDest, int ipSource, int metric, int nextHop);
	
	/**
	* Func addTwoHopNeighbor 
	*	Add a neighbor to the mTwoHopNeighborTable
	*
	* @ param Route or member of the struct Route
	*
	* @ return 0 if success
	*	   1 if !success
	*/
	int addTwoHopNeighbor(Route *route);
	int addTwoHopNeighbor(int ipDest, int ipSource, int metric, int nextHop);

	/**
	* Func delNeighbor 
	*	Delete a neighbor from the mNeighborTable
	*
	* @ param ipToDelete ( ip destination to delete)
	*
	* @ return 0 if success
	*	   1 if !success
	*/
	int delNeighbor(int ipToDelete);
	
	/**
	* Func delTwoHopNeighbor 
	*	Delete a neighbor from the mTwoHopNeighborTable
	*	
	* @ param ipToDelete ( ip destination to delete)
	*
	* @ return 0 if success
	*	   1 if !success
	*/
	int delTwoHopNeighbor(int ipToDelete);
};

//#endif // CLIENT_H
