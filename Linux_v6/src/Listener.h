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
