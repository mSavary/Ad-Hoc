/*!
 * \file Node.cpp
 *
 *  \date 1 Février 2014
 *      \author LeCallonec Joran & Savary Maxime
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

#include "Node.h"

Node::Node() {

	mMpr = false;
	mInterface = setInterface();
	mMyIp = new IPv6(macToIpv6());

}

Node::~Node() {
}

void Node::sendHello() {
	int pqSeqNum = 1, mgSeqNum = 1;
	while (1) {
		std::list<IPv6*> nghb;

		mMutexMprIP.lock();
		std::list<IPv6*> mpr = mMyMprList;
		mMutexMprIP.unlock();
		mMutexNeighborIP.lock();
		for (std::list<IPv6*>::iterator it1 = mNeighborIP.begin();
				it1 != mNeighborIP.end(); it1++) {
			bool find = false;
			for (std::list<IPv6*>::iterator it2 = mpr.begin(); it2 != mpr.end();
					it2++) {
				if (!(*it2)->isEgal((*it1))) {
					find = true;
				}
			}
			if (!find) {
				nghb.push_back((*it1));
			}
		}
		mMutexNeighborIP.unlock();
		sleep(HELLO_INTERVAL);

		Hello *msg = new Hello(pqSeqNum, mMyIp, mgSeqNum, nghb, mpr);
		pqSeqNum = (pqSeqNum + 1) % 65530;
		mgSeqNum = (mgSeqNum + 1) % 65530;
		msg->printData();
		msg->sendHello();
	}
}

void Node::sendTc() {
	int pqSeqNum = 1, mgSeqNum = 1;
	while (1) {
		sleep(TC_INTERVAL);
		if (isMpr()) {
			mMutexAdvNeighborIP.lock();
			std::list<IPv6*> advertiseList = mAdvertisedNeighborList;
			mMutexAdvNeighborIP.unlock();
			for (std::list<IPv6*>::iterator it = advertiseList.begin();
					it != advertiseList.end(); it++) {
				if ((*it)->isEgal(mMyIp)) {
					advertiseList.erase(it);
					if (advertiseList.size() == 0) {
						mMpr = false;
					}
					break;
				}
			}
			if (mMpr) {

				Tc *msg = new Tc(pqSeqNum, mMyIp, mgSeqNum, advertiseList);
				pqSeqNum = (pqSeqNum + 1) % 65530;
				mgSeqNum = (mgSeqNum + 1) % 65530;
				msg->sendTc();
			}
			mMutexAdvNeighborIP.lock();
			mAdvertisedNeighborList = advertiseList;
			mMutexAdvNeighborIP.unlock();
		}
	}
	/* appeler fonctions de construction et d'envoi de message Tc */
}

int Node::selectMpr(std::list<std::list<IPv6*> > TwoHopList,
		std::list<IPv6*> NeighborList) {
	if (TwoHopList.size() == 0) {
		return 1;
	}

	std::list<std::list<IPv6*> > twoHopNeighborIP = TwoHopList;
	std::list<IPv6*> neighborIP = NeighborList;

	int nbrNgbMpr = 0;
	IPv6* tmpMpr;
	std::list<IPv6*> listNgbMpr;
	for (std::list<IPv6*>::iterator itNgbIp = neighborIP.begin();
			itNgbIp != neighborIP.end(); itNgbIp++) {
		IPv6 *ipToTest = (*itNgbIp);
		int nbrNgb = 0;
		std::list<IPv6*> listTmpNgbMpr;
		for (std::list<std::list<IPv6*> >::iterator itTwoHopNgbIp =
				twoHopNeighborIP.begin();
				itTwoHopNgbIp != twoHopNeighborIP.end(); itTwoHopNgbIp++) {

			if ((itTwoHopNgbIp->back())->isEgal(ipToTest)) {
				nbrNgb++;
				listTmpNgbMpr.push_back(itTwoHopNgbIp->front());
			}
		}
		if (nbrNgb > nbrNgbMpr) {
			nbrNgbMpr = nbrNgb;
			tmpMpr = ipToTest;
			listNgbMpr = listTmpNgbMpr;
		}
	}
	for (std::list<IPv6*>::iterator itNgbIp = listNgbMpr.begin();
			itNgbIp != listNgbMpr.end(); itNgbIp++) {
		IPv6* ipToTest = (*itNgbIp);
		for (std::list<std::list<IPv6*> >::iterator itTwoHopNgbIp =
				twoHopNeighborIP.begin();
				itTwoHopNgbIp != twoHopNeighborIP.end(); itTwoHopNgbIp++) {
			if (itTwoHopNgbIp->front()->isEgal(ipToTest)) {
				itTwoHopNgbIp = twoHopNeighborIP.erase(itTwoHopNgbIp);
			}
		}
	}

	mMutexTwoHopTable.lock();
	for (std::list<IPv6*>::iterator itNgbIp = listNgbMpr.begin();
			itNgbIp != listNgbMpr.end(); itNgbIp++) {
		IPv6* ipToTest = (*itNgbIp);
		for (std::list<Route*>::iterator tableroute =
				mTwoHopNeighborTable.begin();
				tableroute != mTwoHopNeighborTable.end(); tableroute++) {
			IPv6* nextHop = (*tableroute)->getNextHop();
			IPv6* dest = (*tableroute)->getIpDest();
			if (ipToTest->isEgal(dest)) {
				if ((nextHop->isEgal(tmpMpr))) {
					//(*tableroute)->setAction(ADD);
				} else {
					if ((*tableroute)->getAction() == ADD) {
						tableroute = mTwoHopNeighborTable.erase(tableroute);
					} else if ((*tableroute)->getAction() == NONE) {
						(*tableroute)->setAction(DEL);
					}
				}
			}
		}
	}
	mMutexTwoHopTable.unlock();

	mMyMprList.push_back(tmpMpr);
	for (std::list<IPv6*>::iterator vi = listNgbMpr.begin();
			vi != listNgbMpr.end(); ++vi) {
		IPv6* ipToComp = (*vi);
		for (std::list<std::list<IPv6*> >::iterator itTwoHop =
				twoHopNeighborIP.begin(); itTwoHop != twoHopNeighborIP.end();
				++itTwoHop) {
			if (((*itTwoHop).front())->isEgal(ipToComp)) {
				itTwoHop = twoHopNeighborIP.erase(itTwoHop);
			}
		}
	}
	for (std::list<IPv6*>::iterator vi = neighborIP.begin();
			vi != neighborIP.end(); ++vi) {
		if ((*vi)->isEgal(tmpMpr)) {
			vi = neighborIP.erase(vi);
			break;
		}
	}
	if (twoHopNeighborIP.size() != 0) {
		selectMpr(twoHopNeighborIP, neighborIP);
	}
	return 0;
}

std::string Node::setInterface() {
	std::string iface = getResCmd(
			"iwconfig 2>&1 |grep -e 'IEEE 802.11' |cut -f1 -d ' '");
	iface = iface.substr(0, (unsigned) iface.length() - 1);
	return iface;
}

int Node::addNeighborTable(Route *route) {
	if (route->getIpDest()->isEgal(mMyIp)) {
		std::cout << " Try to add route to myself \n";
		return 4;
	}
	if (route->getMetric() != 1) {
		std::cout << "ERROR : Add Neighbor with Metric > 1" << std::endl;
		return 2;
	} else {
		mMutexTwoHopTable.lock();
		std::list<Route*> list = mTwoHopNeighborTable;
		mMutexTwoHopTable.unlock();

		for (std::list<Route*>::iterator it = list.begin(); it != list.end();
				++it) {
			if ((*it)->getIpDest()->isEgal(route->getIpDest())) {
				delTwoHopNeighbor((*it)->getIpDest(), (*it)->getNextHop());
				break;
			}
		}
		mMutexNeighborTable.lock();
		std::list<Route*> listNgb = mNeighborTable;
		mMutexNeighborTable.unlock();
		for (std::list<Route*>::iterator it = listNgb.begin();
				it != listNgb.end(); ++it) {
			if ((*it)->getIpDest()->isEgal(route->getIpDest())) {
				std::cout << " route action  : " << route->getAction()
						<< std::endl;
				std::cout << " IP fucked ! : " << route->getIpDest()->toChar()
						<< " hop : " << route->getNextHop()->toChar()
						<< " metric : " << route->getMetric() << std::endl;
				std::cout << "ERROR : Neighbor Already Exist" << std::endl;
				return 3;
			}
		}
		mMutexNeighborTable.lock();
		mNeighborTable.push_back(route);
		mMutexNeighborTable.unlock();

		return 0;
	}
	return 1;
}

int Node::addNeighbor(Route* route) {
	int result = addNeighborTable(route);
	if (result == 0) {
		mMutexNeighborIP.lock();
		mNeighborIP.push_back(route->getIpDest());
		mMutexNeighborIP.unlock();
		return result;
	}
	std::cout << "ERROR : Adding Neighbor Route\n";
	return result;

}
int Node::addNeighbor(IPv6* ipDest, IPv6* nextHop, int metric,
		std::string iface) {
	Route *route = new Route(ipDest, nextHop, metric, iface);
	return addNeighbor(route);
}

int Node::addTwoHopNeighborTable(Route *route) {

	if (route->getIpDest()->isEgal(mMyIp)) {
		std::cout << " Try to add route to myself \n";
		return 9;
	}
	if (route->getMetric() != 2) {
		std::cout << "ERROR : Add Two Hop Neighbor with Metric != 2"
				<< std::endl;
		return 2;
	} else {
		bool destExist = false;
		bool nextHopExist = false;
		mMutexTwoHopTable.lock();
		for (std::list<Route*>::iterator it = mTwoHopNeighborTable.begin();
				it != mTwoHopNeighborTable.end(); ++it) {
			if ((*it)->getIpDest()->isEgal(route->getIpDest())) {

				if ((*it)->getNextHop()->isEgal(route->getNextHop())) {
					mMutexTwoHopTable.unlock();
					return 3;
				}
				destExist = true;
			}
		}
		mMutexNeighborTable.lock();
		for (std::list<Route*>::iterator it = mNeighborTable.begin();
				it != mNeighborTable.end(); ++it) {
			if ((*it)->getIpDest()->isEgal(route->getIpDest())) {
				mMutexNeighborTable.unlock();
				mMutexTwoHopTable.unlock();
				return 4;
			}
			if ((*it)->getIpDest()->isEgal(route->getNextHop())) {
				nextHopExist = true;
			}
		}
		mMutexNeighborTable.unlock();
		if (nextHopExist) {
			mTwoHopNeighborTable.push_back(route);
			mMutexTwoHopTable.unlock();
			if (destExist) {
				return 5;
			}
			return 0;
		} else {
			std::cout << "ERROR : Hop to access Destination doesn't exists\n";
			mMutexTwoHopTable.unlock();
			return 8;
		}
	}
	return 1;
}

int Node::addTwoHopNeighbor(Route* route) {
	std::list<IPv6*> TwoHop;
	int result = addTwoHopNeighborTable(route);
	if (result == 0) {
		TwoHop.push_back(route->getIpDest());
		TwoHop.push_back(route->getNextHop());
		mMutexTwoHopIP.lock();
		mTwoHopNeighborIP.push_back(TwoHop);
		std::list<std::list<IPv6*> > listTwoHop = mTwoHopNeighborIP;
		mMutexTwoHopIP.unlock();
		mMutexNeighborIP.lock();
		std::list<IPv6*> nghb = mNeighborIP;
		mMutexNeighborIP.unlock();
		clearMpr();
		selectMpr(listTwoHop, nghb);
		return result;
	} else if (result == 5) {
		TwoHop.push_back(route->getIpDest());
		TwoHop.push_back(route->getNextHop());
		mMutexTwoHopIP.lock();
		mTwoHopNeighborIP.push_back(TwoHop);
		std::list<std::list<IPv6*> > listTwoHop = mTwoHopNeighborIP;
		mMutexTwoHopIP.unlock();
		mMutexNeighborIP.lock();
		std::list<IPv6*> nghb = mNeighborIP;
		mMutexNeighborIP.unlock();
		clearMpr();
		selectMpr(listTwoHop, nghb);
		return result;
	}
	return result;
}
int Node::addTwoHopNeighbor(IPv6* ipDest, IPv6* nextHop, int metric,
		std::string iface) {
	Route *route = new Route(ipDest, nextHop, metric, iface);
	return addTwoHopNeighbor(route);
}

int Node::addAdvertisedNeighbor(IPv6* ip) {
	if (!(ip->isEgal(mMyIp))) {
		mMutexAdvNeighborIP.lock();
		for (std::list<IPv6*>::iterator it = mAdvertisedNeighborList.begin();
				it != mAdvertisedNeighborList.end(); it++) {
			if ((*it)->isEgal(ip)) {
				mMutexAdvNeighborIP.unlock();
				return 0;
			}
		}
		mAdvertisedNeighborList.push_back(ip);
		imMpr();
		mMutexAdvNeighborIP.unlock();
	}
	return 1;
}

int Node::delAdvertisedNeighbor(IPv6* ip) {
	mMutexAdvNeighborIP.lock();
	for (std::list<IPv6*>::iterator it = mAdvertisedNeighborList.begin();
			it != mAdvertisedNeighborList.end(); it++) {
		if ((*it)->isEgal(ip)) {
			it = mAdvertisedNeighborList.erase(it);
			if (mAdvertisedNeighborList.size() == 0) {
				mMpr = false;
			}
			mMutexAdvNeighborIP.unlock();
			return 1;
		}
	}
	mMutexAdvNeighborIP.unlock();
	return 0;
}

int Node::delNeighbor(IPv6* ipToDelete) {

	mMutexNeighborTable.lock();
	for (std::list<Route*>::iterator it = mNeighborTable.begin();
			it != mNeighborTable.end(); ++it) {
		if ((*it)->getIpDest()->isEgal(ipToDelete)) {
			if ((*it)->getAction() == ADD) {
				it = mNeighborTable.erase(it);
			} else if ((*it)->getAction() == NONE) {
				(*it)->setAction(DEL);
			}
			break;
		}
	}
	mMutexNeighborTable.unlock();
	delTwoHopNeighbor(ipToDelete, true);
	bool unlock = false;
	mMutexNeighborIP.lock();
	for (std::list<IPv6*>::iterator it = mNeighborIP.begin();
			it != mNeighborIP.end(); ++it) {
		if ((*it)->isEgal(ipToDelete)) {
			it = mNeighborIP.erase(it);
			clearMpr();

			std::list<IPv6*> nghb = mNeighborIP;
			mMutexNeighborIP.unlock();
			mMutexTwoHopIP.lock();

			std::list<std::list<IPv6*> > hopNghb = mTwoHopNeighborIP;
			mMutexTwoHopIP.unlock();

			selectMpr(hopNghb, nghb);
			unlock = true;

			if (delAdvertisedNeighbor(ipToDelete)) {
				std::cout << " adv delete : " << ipToDelete->toChar()
						<< std::endl;
			}

			return 0;
		}
	}
	mMutexNeighborIP.unlock();
	std::cout << "ERROR : Neighbor not Found \n";
	return 1;
}
int Node::delNeighbor(Route* route) {
	IPv6* ip = route->getIpDest();
	return delNeighbor(ip);
}

int Node::delTwoHopNeighbor(IPv6* ipToDelete, bool nxtHop) {
	mMutexTwoHopIP.lock();
	std::list<std::list<IPv6*> > list = mTwoHopNeighborIP;
	mMutexTwoHopIP.unlock();
	if (!nxtHop) {
		for (std::list<std::list<IPv6*> >::iterator it = list.begin();
				it != list.end(); ++it) {
			if (((*it).front())->isEgal(ipToDelete)) {
				IPv6* nextHop = (*it).back();
				delTwoHopNeighbor(ipToDelete, nextHop);
				return 0;
			}
		}
	} else if (nxtHop) {
		for (std::list<std::list<IPv6*> >::iterator it = list.begin();
				it != list.end(); ++it) {
			if (((*it).back())->isEgal(ipToDelete)) {
				IPv6* dest = (*it).front();
				std::cout << " DELETE node.Cpp : " << dest->toChar()
						<< " hop : " << (*it).back()->toChar() << std::endl;
				delTwoHopNeighbor(dest, ipToDelete);
			}
		}
		return 0;
	}

	return 1;
}

int Node::delTwoHopNeighbor(IPv6* ipToDelete, IPv6* ipHopToDelete) {

	mMutexTwoHopTable.lock();
	for (std::list<Route*>::iterator it = mTwoHopNeighborTable.begin();
			it != mTwoHopNeighborTable.end(); ++it) {
		if ((*it)->getIpDest()->isEgal(ipToDelete)
				&& (*it)->getNextHop()->isEgal(ipHopToDelete)) {
			if ((*it)->getAction() == ADD) {
				it = mNeighborTable.erase(it);
				std::cout << " erase : " << ipToDelete->toChar() << " hop : "
						<< ipHopToDelete->toChar() << std::endl;
			} else if ((*it)->getAction() == NONE) {
				std::cout << " ACTION to DEL : " << ipToDelete->toChar()
						<< " hop : " << ipHopToDelete->toChar() << std::endl;
				(*it)->setAction(DEL);
				break;
			}

		}
	}
	mMutexTwoHopTable.unlock();
	bool unlock = false;
	mMutexTwoHopIP.lock();
	for (std::list<std::list<IPv6*> >::iterator it2 = mTwoHopNeighborIP.begin();
			it2 != mTwoHopNeighborIP.end(); ++it2) {
		std::list<IPv6*> temp = (*it2);
		IPv6* ip = temp.front();
		if ((ip)->isEgal(ipToDelete)) {
			if ((temp.back())->isEgal(ipHopToDelete)) {
				it2 = mTwoHopNeighborIP.erase(it2);
				std::list<std::list<IPv6*> > twoHop = mTwoHopNeighborIP;
				mMutexTwoHopIP.unlock();
				clearMpr();
				mMutexNeighborIP.lock();
				std::list<IPv6*> nghb = mNeighborIP;
				mMutexNeighborIP.unlock();
				selectMpr(twoHop, nghb);
				unlock = true;
				return 0;
			}
		}
	}
	mMutexTwoHopIP.unlock();
	std::cout << "ERROR : Two Hop Neighbor not Found \n";
	return 1;
}
int Node::delTwoHopNeighbor(Route* route) {
	IPv6* ip = route->getIpDest();
	IPv6* ip2 = route->getNextHop();
	return delTwoHopNeighbor(ip, ip2);
}

std::string Node::getResCmd(char * cmd) {
	FILE* pipe = popen(cmd, "r");
	if (!pipe)
		return "ERROR";
	char buffer[128];
	std::string result = "";
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return result;
}

std::string Node::hex2Bin(const std::string& s) {
	std::stringstream ss;
	ss << std::hex << s;
	unsigned n;
	ss >> n;
	std::bitset<32> b(n);

	unsigned x = 0;
	if (boost::starts_with(s, "0x") || boost::starts_with(s, "0X"))
		x = 2;
	return b.to_string().substr(32 - 4 * (s.length() - x));
}

std::string Node::bin2Hex(const std::string& s) {
	std::bitset<32> bs(s);
	unsigned n = bs.to_ulong();
	std::stringstream ss;
	ss << std::hex << n;
	return "0x" + boost::to_upper_copy(ss.str());
}

std::string Node::macToIpv6() {
	std::string commande =
			"ifconfig wlan0 | grep wlan0 | awk '$0 ~ /HWaddr/ { print $5 }'";
	char * cmd = (char*) commande.c_str();
	std::string result = getResCmd(cmd);
	std::string IPv6;
	if (result.empty()) {
		std::cout << "MAC adress not found\n";
	}

	// reformat to IPv6 notation
	IPv6 = result.substr(0, 2) + result.substr(3, 5) + "ff:fe"
			+ result.substr(9, 5) + result.substr(15, 2);

	// convertion of first octet
	std::string hexaStr = IPv6.substr(0, 2);
	std::string binaryStr = hex2Bin(hexaStr);

	if (binaryStr[6] == '0') {
		binaryStr[6] = '1';
	} else {
		binaryStr[6] = '0';
	}

	hexaStr = bin2Hex(binaryStr);

	if (hexaStr.size() == 3) {
		hexaStr.insert(2, "0");
	}

	// insert of first octet modified
	IPv6.replace(0, 2, hexaStr.substr(2, 4));

	// créer un flux de sortie
	std::ostringstream enTeteIPv6Temp;
	// écrire un nombre dans le flux
	enTeteIPv6Temp << EN_TETE_IPv6;
	// récupérer une chaîne de caractères
	std::string enTeteIPv6 = enTeteIPv6Temp.str() + "::";

	IPv6.insert(0, enTeteIPv6);

	return IPv6;
}

int Node::addDestTable(Route *route) {
	if (!(route->getIpDest()->isEgal(mMyIp))) {
		mMutexNeighborIP.lock();
		for (std::list<IPv6*>::iterator it = mNeighborIP.begin();
				it != mNeighborIP.end(); it++) {
			if ((*it)->isEgal(route->getIpDest())) {
				mMutexNeighborIP.unlock();
				return 0;
			}
		}
		mMutexNeighborIP.unlock();
		mMutexTwoHopIP.lock();
		for (std::list<std::list<IPv6*> >::iterator it =
				mTwoHopNeighborIP.begin(); it != mTwoHopNeighborIP.end();
				it++) {
			if (((*it).front())->isEgal(route->getIpDest())) {
				mMutexTwoHopIP.unlock();
				return 0;
			}
		}
		mMutexTwoHopIP.unlock();
		int result = updDestTable(route);
		if (result == 1) {
			mMutexDestTable.lock();
			mDestTable.push_back(route);
			mMutexDestTable.unlock();
			mMutexDestIP.lock();
			mDestIP.push_back((route->getIpDest()));
			mMutexDestIP.unlock();
			return 1;
		} else if (result == 2) {
			std::cout << " better route exist \n";
			return 2;
		} else if (result == 0) {
			std::cout << " update route for a shorter one \n";
			return 0;
		}
	} else {
		std::cerr << " erreur adding my IP \n";
		return 2;
	}
	return 10;
}

int Node::updDestTable(Route *route) {
	mMutexDestTable.lock();
	IPv6 *ipToComp = route->getIpDest();
	for (std::list<Route*>::iterator it = mDestTable.begin();
			it != mDestTable.end(); it++) {
		if ((*it)->getIpDest()->isEgal(ipToComp)) {
			if ((*it)->getMetric() > route->getMetric()) {
				route->setAction(UPD);
				(*it)->setRoute(route);
				mMutexDestTable.unlock();
				return 0;
			}
			mMutexDestTable.unlock();
			return 2;
		}
	}
	mMutexDestTable.unlock();
	return 1;
}

int Node::delDestTable(Route *route) {
	mMutexDestTable.lock();
	IPv6 *dest = route->getIpDest();
	IPv6 *nextHop = route->getNextHop();
	bool erase = false;
	for (std::list<Route*>::iterator it = mDestTable.begin();
			it != mDestTable.end(); it++) {
		if ((*it)->getIpDest()->isEgal(dest)
				&& (*it)->getIpDest()->isEgal(nextHop)) {
			erase = true;
			if ((*it)->getAction() == ADD) {
				it = mNeighborTable.erase(it);
			} else if ((*it)->getAction() == NONE) {
				(*it)->setAction(DEL);
			}
			break;
		}
	}
	mMutexDestTable.unlock();
	if (erase) {
		mMutexDestIP.lock();
		for (std::list<IPv6*>::iterator it = mDestIP.begin();
				it != mDestIP.end(); it++) {
			if ((*it)->isEgal(route->getIpDest())) {
				it = mDestIP.erase(it);
				mMutexDestIP.unlock();
				return 0;
			}
		}
		mMutexDestIP.unlock();
	}
	std::cerr << " Error Deleting unexisting Destination\n";
	return 1;

}

int Node::delDest(Route *route) {
	return delDestTable(route);
}

int Node::delDest(IPv6 *ip) {
	mMutexDestTable.lock();
	std::list<Route*> list = mDestTable;
	mMutexDestTable.unlock();
	for (std::list<Route*>::iterator it = list.begin(); it != list.end();
			it++) {
		if ((*it)->getIpDest()->isEgal(ip)) {
			return delDest((*it));
		}
	}
	std::cerr << "Fail to delete route with metric > 2 \n";
	return 1;
}

