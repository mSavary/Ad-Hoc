/*
 * main.cpp
 *
 *  Created on: 20 mars 2014
 *      Author: Joran LeCallonec & Savary Maxime & Merlet Céline
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

#include <iostream>
#include <string>
#include <stdio.h>
#include <list>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "Hello.h"
#include "Tc.h"
#include "Message.h"
#include "Listener.h"
#include "Destination.h"
#include "RoutingTable.h"
#include <boost/thread/mutex.hpp>
/**
 * Global Objects
 *
 */
Node *mNode;
std::list<Destination*> mDestination;
boost::mutex mMutexDest;
RoutingTable * kernelTable;

/**
 * FUNC taitementHello
 *
 * 		Analyze Hello informations and apply every change to the kernel routing table
 * 	@params msg the message to analyze
 */

int traitementHello(Hello* msg) {
	IPv6* origIp = msg->getOriginatorAddress();
	if (origIp->isEgal(mNode->getMyIp())) {
		return 0;
	}
	std::list<IPv6*> listNghbNode = mNode->getNeighborIP();
	bool find = false;
	for (std::list<IPv6*>::iterator itListNghbNode = listNghbNode.begin();
			itListNghbNode != listNghbNode.end(); itListNghbNode++) {
		if ((*itListNghbNode)->isEgal(origIp)) {
			find = true;
			mMutexDest.lock();
			for (std::list<Destination*>::iterator itDest =
					mDestination.begin(); itDest != mDestination.end();
					itDest++) {
				if (origIp->isEgal((*itDest)->getIp())) {
					std::cout << " RESET VOISIN : " << origIp->toChar()
							<< " metric 1 main:hello l36 !\n";
					(*itDest)->resetTimer();
					break;
				}
			}
			mMutexDest.unlock();
			//break;
		}

	}
	if (!find) {
		std::cout << " AJOUT VOISIN : " << origIp->toChar()
				<< " metric 1 main::hello l47 !\n";
		int result = mNode->addNeighbor(origIp, NULL, 1, mNode->getInterface());
		if (result == 0) {
			Destination *dest = new Destination(origIp, 1);
			mMutexDest.lock();
			mDestination.push_back(dest);
			mMutexDest.unlock();
		} else {
			std::cout << " rendu de addngb : " << result << std::endl;
		}
	}
	find = false;
//fin traitement originator ADDRESSE

	std::list<HelloNeighborList*> listNeighbor = msg->getNeighbors();

	for (std::list<HelloNeighborList*>::iterator itListNeighbor =
			listNeighbor.begin(); itListNeighbor != listNeighbor.end();
			itListNeighbor++) {
		if ((*itListNeighbor)->getLinkCode() == LINK_CODE_MPR) {

			std::list<IPv6*> listIp = (*itListNeighbor)->getNeighborsAddrList();
			bool advertise = false;
			for (std::list<IPv6*>::iterator itIp = listIp.begin();
					itIp != listIp.end(); itIp++) {
				if ((*itIp)->isEgal(mNode->getMyIp())) {
					advertise = true;
					std::cout << " JE DEVIENS MPR de " << origIp->toChar()
							<< " ! main::hello l71\n";
					mNode->addAdvertisedNeighbor(origIp);
				} else {
					int result = mNode->addTwoHopNeighbor((*itIp), origIp, 2,
							mNode->getInterface());
					if (result == 0) {
						std::cout << " AJOUT VOISIN+1 : " << (*itIp)->toChar()
								<< " metric 2 linkcode MPR main:hello l76 !\n";
						Destination *dest = new Destination((*itIp), 2);
						mMutexDest.lock();
						mDestination.push_back(dest);
						mMutexDest.unlock();
					} else if (result == 3) {
						mMutexDest.lock();
						for (std::list<Destination*>::iterator itDest =
								mDestination.begin();
								itDest != mDestination.end(); itDest++) {
							if ((*itIp)->isEgal((*itDest)->getIp())) {
								std::cout << " Reset VOISIN+1 : "
										<< (*itIp)->toChar()
										<< " linkcode MPR main:hello l87 !\n";
								(*itDest)->resetTimer();
								break;
							}
						}
						mMutexDest.unlock();
					}
				}
			}
			if (!advertise) {
				std::cout << " Del Adv : " << origIp->toChar()
						<< " main::hello l96  !\n";
				mNode->delAdvertisedNeighbor(origIp);
				std::cout << " JE SUIS ENCORE MPR ? :" << mNode->isMpr()
						<< std::endl;
			}
			advertise = false;
		} else if ((*itListNeighbor)->getLinkCode() == LINK_CODE_NGHB) {
			std::list<IPv6*> listIp = (*itListNeighbor)->getNeighborsAddrList();
			for (std::list<IPv6*>::iterator itIp = listIp.begin();
					itIp != listIp.end(); itIp++) {
				if ((*itIp)->isEgal(mNode->getMyIp()) == false) {
					int result = mNode->addTwoHopNeighbor((*itIp), origIp, 2,
							mNode->getInterface());
					if (result == 0) {
						std::cout << " AJOUT VOISIN+1 : " << origIp->toChar()
								<< " metric 2 main::Hello l106 !\n";
						Destination *dest = new Destination((*itIp), 2);
						mMutexDest.lock();
						mDestination.push_back(dest);
						mMutexDest.unlock();
					} else if (result == 3) {
						mMutexDest.lock();
						for (std::list<Destination*>::iterator itDest =
								mDestination.begin();
								itDest != mDestination.end(); itDest++) {
							if ((*itIp)->isEgal((*itDest)->getIp())) {
								std::cout << " Reset VOISIN+1 : "
										<< (*itIp)->toChar()
										<< " linkcode NGb main:hello l119 !\n";
								(*itDest)->resetTimer();
								break;
							}
						}
						mMutexDest.unlock();
					}
				} else {
					std::cout << " LIEN SYMETRIQUE ! main::hello l127 \n";
					mNode->delAdvertisedNeighbor(origIp);
				}
			}
		} else {
			std::cout << " ERREUR LINK CODE\n";
		}
	}

	return 1;
}

/**
 * FUNC traitementTc
 *
 * 		Analyze Tc informations and apply every change to the kernel routing table
 *
 * @params msg The message to analyze
 */
int traitementTc(Tc* msg) {
	IPv6* origIp = msg->getOriginatorAddress();
	IPv6* fromIp = msg->getForwarder();
	if (origIp->isEgal(mNode->getMyIp())) {
		return 0;
	}
	if (fromIp->isEgal(mNode->getMyIp())) {
		return 0;
	}
	if (msg->getHopCount() > 2) {
		std::list<IPv6*> destList = mNode->getDestIP();
		std::list<IPv6*> listAdvertised =
				msg->getAdvertisedNeighborMainAddress();
		for (std::list<IPv6*>::iterator itMsg = listAdvertised.begin();
				itMsg != listAdvertised.end(); itMsg++) {
			bool find = false;
			if (!((*itMsg)->isEgal(mNode->getMyIp()))) {
				for (std::list<IPv6*>::iterator itNode = destList.begin();
						itNode != destList.end(); itNode++) {
					IPv6 * ipToComp = (*itNode);

					if ((*itMsg)->isEgal(ipToComp)) {
						find = true;
						mMutexDest.lock();
						for (std::list<Destination*>::iterator itDest =
								mDestination.begin();
								itDest != mDestination.end(); itDest++) {
							if ((*itMsg)->isEgal((*itDest)->getIp())) {
								std::cout << " reset DEST main::TC l180 "
										<< (*itMsg)->toChar() << std::endl;
								(*itDest)->resetTimer();
								break;
							}
						}
						mMutexDest.unlock();
						break;
					}

				}
			} else {
				std::cout << " MON IP dans TC main::TC l190 "
						<< (*itMsg)->toChar() << std::endl;
			}
			if (!find) {
				Route *route = new Route((*itMsg), msg->getForwarder(),
						msg->getHopCount(), mNode->getInterface());
				std::cout << " AJOUT VOISIN +x main::TC l198 "
						<< (*itMsg)->toChar() << std::endl;
				mNode->addDestTable(route);
				Destination *dest = new Destination((*itMsg),
						msg->getHopCount());
				mMutexDest.lock();
				mDestination.push_back(dest);
				mMutexDest.unlock();
			}
			find = false;
		}
	} else {
		std::cout << " TRAITE PAS LE TC car HOPCOUNT <=2\n main::TC l205 \n";
	}

	if (mNode->isMpr()) {
		msg->sendTc();
	}
	return 0;
}

/**
 * FUNC checkDestination
 *
 * 		Check if a Destination we add before reach out of his life time
 * 		and then notify to delete it if necessary
 */
int checkDestination() {
	while (1) {
		sleep(NEIGHB_HOLD_TIME);
		mMutexDest.lock();
		std::list<Destination*> list = mDestination;
		mMutexDest.unlock();
		for (std::list<Destination*>::iterator itDest = list.begin();
				itDest != list.end(); itDest++) {
			if ((*itDest)->getState() == DEL) {
				if ((*itDest)->getMetric() == 1) {
					mNode->delNeighbor((*itDest)->getIp());
					std::cout << "  DEL VOISIN : "
							<< (*itDest)->getIp()->toChar() << std::endl;
				} else if ((*itDest)->getMetric() == 2) {
					mNode->delTwoHopNeighbor((*itDest)->getIp(), false);
					std::cout << " DEL VOISIN+1 : "
							<< (*itDest)->getIp()->toChar() << std::endl;
				} else {
					mNode->delDest((*itDest)->getIp());
					std::cout << " DEL dest : " << (*itDest)->getIp()->toChar()
							<< std::endl;
				}

				itDest = list.erase(itDest);
			}
		}
		mMutexDest.lock();
		mDestination = list;
		mMutexDest.unlock();
		/*	std::list<Route*> twoHopList = mNode->getTwoHopNeighborTable();
		 std::list<Route*> ngbList = mNode->getNeighborTable();
		 std::list<Route*> destList = mNode->getDestTable();
		 std::cout << "***************\n ngb TABLE : \n***************\n";
		 for (std::list<Route*>::iterator it = ngbList.begin();
		 it != ngbList.end(); ++it) {
		 std::cout << " DEST : " << (*it)->getIpDest()->toChar()
		 << " Next Hop : " << (*it)->getNextHop()->toChar()
		 << std::endl << " METRIC : " << (*it)->getMetric()
		 << std::endl;
		 }
		 std::cout << "***************\n dest TABLE : \n***************\n";
		 for (std::list<Route*>::iterator it = destList.begin();
		 it != destList.end(); ++it) {
		 std::cout << " DEST : " << (*it)->getIpDest()->toChar()
		 << " Next Hop : " << (*it)->getNextHop()->toChar()
		 << std::endl << " Action : " << (*it)->getAction()
		 << std::endl;
		 }

		 std::cout << "***************\n TWO HOP TABLE : \n***************\n";
		 for (std::list<Route*>::iterator it = twoHopList.begin();
		 it != twoHopList.end(); ++it) {
		 std::cout << " DEST : " << (*it)->getIpDest()->toChar()
		 << " Next Hop : " << (*it)->getNextHop()->toChar()
		 << std::endl << " Action : " << (*it)->getAction()
		 << std::endl;
		 }
		 std::cout << "***************\n list Destination : \n***************\n";
		 for (std::list<Destination*>::iterator itDest = list.begin();
		 itDest != list.end(); itDest++) {
		 std::cout << " IP DEST : " << (*itDest)->getIp()->toChar()
		 << std::endl;
		 }*/
	}
	return 0;
}

/**
 * FUNC runKernel
 *
 * 		Every Hello interval apply or delete route that changes.
 */
int runKernel() {
	while (1) {
		sleep(HELLO_INTERVAL);
		kernelTable->systemTableUpdate(mNode);
		/*	std::ostringstream syscall;
		 syscall << "route -6 ";

		 if (system((syscall.str()).c_str()))
		 std::cerr << " erreur syscall main \n";*/
	}
	return 0;
}
/**
 * FUNC main
 *
 * 		listen the network and get the message we receive.
 */
int main() {

	Listener * Listen = new Listener();
	Listen->run();
	mNode = new Node();
	kernelTable = new RoutingTable();
	boost::thread runCheckDest = boost::thread(checkDestination);
	boost::thread kernelUpd = boost::thread(runKernel);
//while(1);
	int i = 0;
	mNode->start();
	while (1) {
		i++;
		Message* tmp = Listen->getMsg();
		if (!(tmp->getTimeToLive() < 0)) {
			int type = tmp->getMessageType();
			if (type == HELLO_TYPE) {
				Hello *helloMsg = (Hello *) tmp;
				mNode->lockSystem();
				traitementHello(helloMsg);
				mNode->releaseSystem();
			} else if (type == TC_TYPE) {
				Tc *TcMsg = (Tc *) tmp;
				mNode->lockSystem();
				traitementTc(TcMsg);
				mNode->releaseSystem();
			} else {
				std::cout << "Unknown type message" << std::endl;
			}
			/*	std::cout << i << " messages recus! Je suis mpr ? : "
			 << mNode->isMpr() << std::endl;*/

		}
		std::list<IPv6*> mprList = mNode->getMprList();
		for (std::list<IPv6*>::iterator it = mprList.begin();
				it != mprList.end(); ++it) {
			std::cout << " EST MON MPR : " << (*it)->toChar() << std::endl;
		}
	}

	/*
	 if ((i % 2 == 0)) {
	 kernelTable->systemTableUpdate(mNode);
	 std::ostringstream syscall;
	 syscall << "route -6 ";
	 system((syscall.str()).c_str());

	 }*/

	return 0;
}

