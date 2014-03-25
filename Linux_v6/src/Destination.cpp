/*
 * DestinationTimer.cpp
 *
 *  Created on: 17 mars 2014
 *      Author: maxime
 */

#include "Destination.h"

void Destination::changeState() {
	if (mDel) {
		mState = DEL;
	} else {
		mDel = true;
	}
}

void Destination::run() {
	mIo->run();
}

Destination::Destination(IPv6 *ip, int metric) {
	// TODO Auto-generated constructor stub
	mIP = ip;
	mMetric = metric;
	mState = NONE;
	mDel = true;
	mIo = new boost::asio::io_service();
	if (mMetric == 1) {
		mTimer = new boost::asio::deadline_timer(*mIo,
				boost::posix_time::seconds(NEIGHB_HOLD_TIME));
	} else {
		mTimer = new boost::asio::deadline_timer(*mIo,
				boost::posix_time::seconds(TOP_HOLD_TIME));
	}
	mTimer->async_wait(boost::bind(&Destination::changeState, this));
	mThreadRun = new boost::thread(&Destination::run, this);
}

Destination::~Destination() {
	// TODO Auto-generated destructor stub
	delete mIP;
}

void Destination::resetTimer() {
	//if(mTimer->expires_at()<)
	mDel = false;

	mTimer->cancel();
	if (mMetric == 1) {
		mTimer->expires_from_now(boost::posix_time::seconds(NEIGHB_HOLD_TIME));
	} else {
		mTimer->expires_from_now(boost::posix_time::seconds(TOP_HOLD_TIME));
	}
	mTimer->async_wait(boost::bind(&Destination::changeState, this));

}
