/*
 * DestinationTimer.h
 *
 *  Created on: 17 mars 2014
 *      Author: Le Callonec Joran & Savary Maxime
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

#ifndef DESTINATION_H_
#define DESTINATION_H_

#include "const.h"
#include "IPv6.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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

		/**
		 * FUNC resetTimer
		 *
		 * 		Restart the timer for a destination before it's set to DEL
		 */
		void resetTimer();

		/**
		 * FUNC run
		 *
		 * 		launch the timer in a thread
		 */
		void run();

		inline IPv6* getIp() {
			return mIP;
		}
		inline int getState(){
			return mState;
		}
		inline int getMetric(){
			return mMetric;
		}
		~Destination();

	};

#endif /* DESTINATION_H_ */
