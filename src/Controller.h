/*
 * Controller.h
 *
 *  Created on: 6 mars 2014
 *      Author: maxime
 */

#include "Node.h"
#include "Message.h"
#include "Hello.h"
#include "Tc.h"
#include "RoutingTable.h"
#include "Listener.h"
#include "const.h"
#include "IPv6.h"
#include "Route.h"

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

class Controller {
private:
	Node *mNode;
	Listener *mListener;
	RoutingTable *mRoutingTable;
	class Destination {
	private:
		IPv6 *mIP;
		int mMetric;
		int mState;
		boost::thread *mThreadRun;
		boost::asio::io_service *mIo;
		boost::asio::deadline_timer *mTimer;
		bool mDel;
		void changeState();
	public:
		Destination(IPv6 *ip, int metric);
		void resetTimer();
		void run();
		~Destination();
	};
	std::list<Destination> mDestination;
public:
	Controller();
	virtual ~Controller();
	void run();
	void traitementHello (Hello* msg);
	void traitementTc (Tc* msg);
};

#endif /* CONTROLLER_H_ */
