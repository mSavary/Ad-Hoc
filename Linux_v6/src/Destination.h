/*!
 * \file DestinationTimer.h
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

#ifndef DESTINATION_H_
#define DESTINATION_H_

#include "const.h"
#include "IPv6.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

/*!
 * \class Destination
 * \brief Object representing a destination we can reach.
 * 		 Her validity ( Alive or not).
 *
 */
class Destination {
private:
	IPv6 *mIP;/*!< Ip of the destination.*/
	int mMetric;/*!< Metric of the destination.*/
	int mState;/*!< State of the Destination.*/
	boost::thread *mThreadRun;
	boost::asio::io_service *mIo;
	boost::asio::deadline_timer *mTimer;
	bool mDel;

	/*!
	 * \fn void changeState()
	 * \brief Method call to change the state of the Destination if it reaches the Timeout.
	 *
	 */
	void changeState();
public:
	Destination(IPv6 *ip, int metric);

	/*!
	 * \fn 	void resetTimer()
	 *
	 * \brief Restart the timer for a destination before it's set to DEL
	 */
	void resetTimer();

	/*!
	 * \fn void run()
	 *
	 * 	\brief	launch the timer in a thread
	 */
	void run();

	/*!
	 * \fn IPv6* getIp ()
	 * \return IPv6* the ip of the Destination's object.
	 */
	inline IPv6* getIp() {
		return mIP;
	}
	/*!
	 * \fn int getState ()
	 * \return int the state of the destination ( DEAD or ALIVE)
	 */
	inline int getState() {
		return mState;
	}
	/*!
	 * \fn int getMetric ()
	 * \return int the metric of the destination
	 */
	inline int getMetric() {
		return mMetric;
	}
	~Destination();

};

#endif /* DESTINATION_H_ */
