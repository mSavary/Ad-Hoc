/*!
 * \file Destination.cpp
 *
 * \date 17 mars 2014
 * \author Le Callonec Joran & Savary Maxime
 */

/*
 * This file is part of Ad-Hoc Networks an app base on OLSR to handle Ad-Hoc
 *  network.
 *
 * Copyright (c) 2014-2014 Gilles Guette <>
 * Copyright (c) 2014-2014 ISTIC http://www.istic.univ-rennes1.fr/
 * Copyright (c) 2014-2014 SUPELEC http://www.supelec.fr/rennes
 *
 * See the AUTHORS or Authors.txt file for copyright owners and
 * contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
	delete mIP;
}

void Destination::resetTimer() {
	mDel = false;

	mTimer->cancel();
	if (mMetric == 1) {
		mTimer->expires_from_now(boost::posix_time::seconds(NEIGHB_HOLD_TIME));
	} else {
		mTimer->expires_from_now(boost::posix_time::seconds(TOP_HOLD_TIME));
	}
	mTimer->async_wait(boost::bind(&Destination::changeState, this));

}
