/*!
 * \file Listener.h
 *
 * \author Efflam Lemaillet & Céline Merlet
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

/*!
* \class Listener
* \brief Listener store differents types of OLSR's mesages
* Listener class's goal is to store differents types of OLSR's mesages in a list in order to save the receive messages when an other message is treated.
*/
class Listener {

private:
	boost::asio::io_service *mIoService; /*!< to the inputs and outputs */
	boost::asio::ip::udp::endpoint mRemoteEndpoint; /*!< associated with the socket */
	boost::asio::ip::udp::socket *mSocket; /*!< socket to listen on the network */
	std::list<Message*> mListMsg; /*!< list to store messages */
	boost::mutex mProtectList; /*!< mutex to protect the list of messages */
	boost::interprocess::interprocess_semaphore *mSem_prod, *mSem_cons; /*!< sempahore to protect the list of messages when the Controller and the Listener want to access it  */

	/*!
	* \fn listenSocket();
	* \brief Listening on the socket, cutting a message.
	* Listening on the socket, cutting of the receive packet and store it in a mesagge in the list
	*/
	void listenSocket();

public:
	/*!
	* \fn Listener();
	* \brief Create an instance of Listener
	*/
	Listener();
	~Listener();
	
	/*!
	* \fn run();
	* \brief Creation of the thread
	* Creation of the thread where we run the method ListendSocket
	*/
	int run();


	/*!
	* \fn Message* getMsg();
	* \brief Get the first message of the list, for the Controller and delete it from the list
	* \return Message* return the first message of the list
	*/
	Message* getMsg();
};

#endif
