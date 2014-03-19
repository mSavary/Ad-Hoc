/*
 * DestinationTimer.h
 *
 *  Created on: 17 mars 2014
 *      Author: maxime
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
		void resetTimer();
		void run();
		inline IPv6* getIp() {
			return mIP;
		}
		~Destination();

	};

#endif /* DESTINATION_H_ */
