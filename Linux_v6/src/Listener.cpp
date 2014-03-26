/**
 * Listener.cpp
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


#include "Listener.h"

#define HELLO_MESSAGE 1
#define Tc_MESSAGE 2

Listener::~Listener() {
	delete mIoService;
	delete mSocket;
}

Listener::Listener() {
	try {
		this->mIoService = new boost::asio::io_service();
		boost::asio::ip::udp::resolver resolver(*mIoService);
		boost::asio::ip::udp::resolver::query query1(boost::asio::ip::udp::v6(),
				"698");

		mRemoteEndpoint = *resolver.resolve(query1);
		mSocket = new boost::asio::ip::udp::socket(*this->mIoService);
		mSem_cons = new boost::interprocess::interprocess_semaphore(MIN_LENGTH);
		mSem_prod = new boost::interprocess::interprocess_semaphore(MAX_LENGTH);
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

int Listener::run() {
	boost::thread threadListen = boost::thread(&Listener::listenSocket, this);
	return 1;
}

void Listener::listenSocket() {
	boost::system::error_code error;
	boost::asio::ip::udp::endpoint sender_endpoint;
	mSocket->open(boost::asio::ip::udp::v6());
	mSocket->bind(mRemoteEndpoint);
	try {
		for (;;) {

			mSem_prod->wait();
			boost::array<char, BUFF_SIZE> recvBuf;
			size_t len = mSocket->receive_from(boost::asio::buffer(recvBuf),
					sender_endpoint);
			if (error && error != boost::asio::error::message_size)
				throw boost::system::system_error(error);


			Message * msg;

			char* temp;
			char * originAddr = temp;
			temp = recvBuf.data();

			uint16_t packetLength = (*(uint16_t*) temp);
			temp += 2;
			
			uint16_t packetSequenceNumber = (*(uint16_t*) temp);
			temp += 2;

			uint8_t messageType = (*(uint8_t*) temp);

			temp = temp + 1;

			uint8_t vTime = (*(uint8_t*) temp);
			temp = temp + 1;

			uint16_t messageSize = (*(uint16_t*) temp);
			temp += 2;

			IPv6* originatorAddress = new IPv6(*(unsigned short*) (temp + 8),
					*(unsigned short*) (temp + 10),
					*(unsigned short*) (temp + 12),
					*(unsigned short*) (temp + 14));

			temp += 16;

			uint8_t timeToLive = (*(uint8_t*) temp);
			temp = temp + 1;

			uint8_t hopCount = ((uint8_t) *temp);
			temp = temp + 1;

			uint16_t messageSequenceNumber = ((uint16_t) *temp);
			temp = temp + 2;


			if (messageType == HELLO_TYPE) {
				uint8_t hTime, willingness;
				std::list<HelloNeighborList*> listhNl;
				/*
				 HELLO HEADER  :
				 |Reserved(16 Bits)| Htime(8Bits)|Willingness(8Bits)|
				 */
				//hMH->reserved = *(uint16_t*)temp;
				temp += 2;

				hTime = ((uint8_t) *temp);
				++temp;
				willingness = ((uint8_t) *temp);
				++temp;
				uint16_t compt = 0; //4
				while (compt < (messageSize - 20)) {

					uint8_t linkCode = (uint8_t) *temp;
					++temp;
					//hNL->reserved = (uint8_t)*temp;
					++temp;

					uint16_t linkMessageSize = *(uint16_t*) temp;
					temp += 2;
					int nb = (linkMessageSize - 4) / 16;
					std::list<IPv6*> listIp;
					while (nb > 0) {
						nb--;
						IPv6* currentV6 = new IPv6(
								(*(unsigned short*) (temp + 8)),
								(*(unsigned short*) (temp + 10)),
								(*(unsigned short*) (temp + 12)),
								(*(unsigned short*) (temp + 14)));
						temp = temp + 16; //for each address ipv6 we add 16

						listIp.push_back(currentV6);
					}
					HelloNeighborList* hNL = new HelloNeighborList(linkCode, listIp);

					compt += linkMessageSize;
					listhNl.push_back(hNL);
				}

				Hello* hMH = new Hello(willingness, hTime, packetLength,
						packetSequenceNumber, messageType, vTime, messageSize,
						originatorAddress, timeToLive, hopCount,
						messageSequenceNumber, listhNl);
				msg = hMH;

			} else if (messageType == TC_TYPE) {
				std::string address = sender_endpoint.address().to_string();
				address = address.substr(4,address.length());
				address.insert(0,"2014");



				uint16_t Ansn;
				std::list<IPv6*> advertisedList;

				Ansn = (*(uint16_t*) temp);
				temp += 2;
				// reserved zone
				temp += 2;
				uint16_t nb = (messageSize - 24) / 16;
				while (nb>0) {
					IPv6* currentV6 = new IPv6(*(unsigned short*) (temp + 8),
							*(unsigned short*) (temp + 10),
							*(unsigned short*) (temp + 12),
							*(unsigned short*) (temp + 14));
					temp = temp + 16; //for each address ipv6 we add 16
					advertisedList.push_back(currentV6);
					nb--;
				}
				Tc* tMH = new Tc(packetLength, packetSequenceNumber,
						messageType, vTime, messageSize, originatorAddress,
						timeToLive, hopCount, messageSequenceNumber, Ansn,address,advertisedList);
				msg = (Message*) tMH;
			}

			mProtectList.lock();

			mListMsg.push_back(msg);
			mProtectList.unlock();

			mSem_cons->post();

		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

Message* Listener::getMsg() {

	mSem_cons->wait();

	mProtectList.lock();
	Message *message = mListMsg.front();

	mListMsg.pop_front();
	
	mProtectList.unlock();

	mSem_prod->post();

	return message;
}

