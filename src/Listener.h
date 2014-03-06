#ifndef LISTENER_H
#define LISTENER_H

#include <iostream>
#include <list>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "Message.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <boost/asio.hpp>
#include "Message.h"
#include "Hello.h"
#include "Tc.h"

#define PORT 698
#define BUFF_SIZE 1280 // size in bytes
class Listener {

private:
	boost::asio::io_service *mIoService;
	boost::asio::ip::udp::endpoint *mRemoteEndpoint;
	boost::asio::ip::udp::endpoint *mSenderEndpoint;
	boost::asio::ip::udp::socket *mSocket;
	std::list<Message> mListMsg;

public:
	Listener();
	~Listener();

	/**
	 * Listening on the socket
	 */
	void listenSocket();

	/**
	 * Put the received message on the list
	 */
	void receptionMsg(Message* msg);

	/**
	 * Get the last message of the list, for the Controller
	 */
	Message getMsg();
};

#endif
