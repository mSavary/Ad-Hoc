#include "Route.h"

Route::Route() {
	mIpDest = new IPv6();
	mNextHop = new IPv6();
	mMetric = 0;
	mInterface = new IPv6();
	mAction = 0;
}

Route::Route(IPv6 *ipDest,
				IPv6 *nextHop,
				int metric,
				IPv6 *interface,
				int action){
	mIpDest = ipDest;
	mNextHop = nextHop;
	mMetric = metric;
	mInterface = interface;
	mAction = action;
}

IPv6 Route::getIpDest(){
	return *mIpDest;
}

IPv6 Route::getNextHop(){
	return *mNextHop;
}

int Route::getMetric(){
	return mMetric;
}

IPv6 Route::getInterface(){
	return *mInterface;
}

int Route::getAction(){
	return mAction;
}

Route::~Route() {
	// TODO Auto-generated destructor stub
}

