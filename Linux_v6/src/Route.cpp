#include "Route.h"

Route::Route() {
	mIpDest = new IPv6();
	mNextHop = new IPv6();
	mMetric = 0;
	mInterface = "wlan0";
	mAction = 0;
}

Route::Route(IPv6 *ipDest,
				IPv6 *nextHop,
				int metric,
				std::string interface){
	mIpDest = ipDest;
	mNextHop = nextHop;
	mMetric = metric;
	mInterface = interface;
	mAction = NONE;
}

IPv6* Route::getIpDest(){
	return mIpDest;
}

IPv6* Route::getNextHop(){
	return mNextHop;
}

int Route::getMetric(){
	return mMetric;
}

std::string Route::getInterface(){
	return mInterface;
}

int Route::getAction(){
	return mAction;
}

void Route::setAction(int action){
	mAction=action;
}

void Route::setRoute(Route *route){
	mIpDest->setIPv6(route->getIpDest());
	mNextHop->setIPv6(route->getNextHop());
	mMetric = route->getMetric();
	mInterface = route->getInterface();
	mAction = UPD;
}

Route::~Route() {
	//delete mIpDest, mNextHop, mInterface;
}

