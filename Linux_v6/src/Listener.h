/**
 * Listener.h
 *
 *      \author Efflam Lemailler & Céline Merlet
 */

/**
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


#ifndef LISTENER_H
#define LISTENER_H

#include <iostream>
#include <list>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include "Message.h"
#include "Hello.h"
#include "Tc.h"
#include "const.h"

class Listener {

private:
	boost::asio::io_service *mIoService;
	boost::asio::ip::udp::endpoint mRemoteEndpoint;
	boost::asio::ip::udp::socket *mSocket;
	std::list<Message*> mListMsg;
	boost::mutex mProtectList;
	boost::interprocess::interprocess_semaphore *mSem_prod, *mSem_cons;

	/**
	 * Listening on the socket and cuttinfg of the receive packet
	 */
	void listenSocket();

public:
	/**
	* Creation of the socket
	*/
	Listener();
	~Listener();
	
	/**
	* Creation of the thread to do other when the Listener listens on the network
	*/
	int run();


	/**
	* Get the last message of the list, for the Controller
	* \return return the last message of the list
	*/
	Message* getMsg();
};

#endif
