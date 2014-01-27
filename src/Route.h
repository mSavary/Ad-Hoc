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

class Route
{

private:
	IPv6 	*mIpDest,
			*mNextHop;
	int		mMetric;
	IPv6 	*mInterface;
	int 	mAction;

public:
	Route();
	Route(IPv6 *ipDest,
			IPv6 *nextHop,
			int metric,
			IPv6 *interface,
			int action);
	IPv6 getIpDest();
	IPv6 getNextHop();
	int getMetric();
	IPv6 getInterface();
	int getAction();
	virtual ~Route();
};

#endif /* ROUTE_H_ */
