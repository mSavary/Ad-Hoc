/*
 * Controller.h
 *
 *  Created on: 6 mars 2014
 *      Author: maxime
 */

#include "Message.h"
#include "Hello.h"
#include "Tc.h"
#include "RoutingTable.h"
#include "Listener.h"
#include "const.h"
#include "IPv6.h"
#include "Route.h"
#include "Destination.h"

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

class Controller {
private:

	Node *mNode;
	Listener *mListener;
	RoutingTable *mRoutingTable;
	std::list<Destination> mDestination;

public:
	Controller();
	virtual ~Controller();
	void run();
	void traitementHello(Hello* msg);
	void traitementTc(Tc* msg);
};

#endif /* CONTROLLER_H_ */
