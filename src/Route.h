/**
*Class Route
*
*
*
*/

#ifndef ROUTE_H_
#define ROUTE_H_

#include "IPv6.h"

#define NONE 0
#define ADD 1
#define DEL 2
#define UPD 3

class Route
{

private:
	IPv6 	*mIpDest,
			*mNextHop;
	int		mMetric;
	std::string 	mInterface;
	int 	mAction;

public:
	Route();
	Route(IPv6 *ipDest,
			IPv6 *nextHop,
			int metric,
			std::string interface);
	IPv6* getIpDest();
	IPv6* getNextHop();
	int getMetric();
	std::string getInterface();
	int getAction();
	void setAction(int action);
	void setRoute(Route *route);
	virtual ~Route();
};

#endif /* ROUTE_H_ */
