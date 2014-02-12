#include "Node.h"

Node::Node() {

	mMpr = false;
	mInterface = setInterface();
	mMyIp = new IPv6(macToIpv6());
	mTimerHello = boost::thread(&Node::sendHello, this);
	mTimerTc = boost::thread(&Node::sendTc, this);

}

Node::~Node() {
	mTimerHello.join();
	mTimerTc.join();
}

void Node::sendHello() {

	sleep(HELLO_INTERVAL);
	mMutexIP.lock();
	std::cout << "\n***** ENVOI HELLO *****";
	/*std::cout << "\n NEIGHBOR LISTE \n****************" << std::endl;
	 for (std::list<IPv6>::iterator ipv = mNeighborIP.begin();
	 ipv != mNeighborIP.end(); ipv++) {
	 std::cout << "" << ipv->toChar() << "" << std::endl;
	 }
	 std::cout << "******************\n";*/
	mMutexIP.unlock();
	sendHello();
	/* appeler fonctions de construction et d'envoi de message Hello */
}

void Node::sendTc() {

	sleep(TC_INTERVAL);
	if (isMpr()) {
		mMutexIP.lock();
		std::cout << "\n***** ENVOI TC *****\n";
		/*std::cout << "\n MPR LISTE \n**************** " << std::endl;
		 for (std::list<IPv6>::iterator ipv = mMyMprList.begin();
		 ipv != mMyMprList.end(); ipv++) {
		 std::cout << ipv->toChar() << std::endl;
		 }
		 std::cout << "********************\n";*/
		mMutexIP.unlock();
		sendTc();
	}

	/* appeler fonctions de construction et d'envoi de message Tc */
}

int Node::selectMpr(std::list<std::list<IPv6> > TwoHopList,
		std::list<IPv6> NeighborList) {
	if (TwoHopList.size() == 0) {
		return 1;
	}

	std::list<std::list<IPv6> > twoHopNeighborIP = TwoHopList;
	std::list<IPv6> neighborIP = NeighborList;
	IPv6 mpr;
	std::list<IPv6> vMpr;
	int nbrVoisMpr = 0;
	bool ajoutMpr = false;

	for (std::list<IPv6>::iterator v = neighborIP.begin();
			v != neighborIP.end(); ++v) {
		int cptV = 0;
		std::list<IPv6> listVoisin;
		for (std::list<std::list<IPv6> >::iterator vi =
				twoHopNeighborIP.begin(); vi != twoHopNeighborIP.end(); ++vi) {
			std::list<IPv6> listIP = *vi;
			bool passage = false;
			for (std::list<IPv6>::iterator ipv = listIP.begin();
					ipv != listIP.end(); ipv++) {

				std::list<IPv6>::iterator fv;
				if (passage) {
					fv = listIP.begin();
					IPv6 ipToComp = *ipv;
					if (v->isEgal(&ipToComp)) {
						cptV++;
						listVoisin.push_back(*fv);
					}
				} else {
					passage = true;
				}
			}

		}
		if (cptV > nbrVoisMpr) {
			ajoutMpr = true;
			mpr = *v;
			nbrVoisMpr = cptV;
			vMpr.clear();
			vMpr = listVoisin;
		}
	}

	mMyMprList.push_back(mpr);

	for (std::list<IPv6>::iterator ipv = neighborIP.begin();
			ipv != neighborIP.end(); ipv++) {
		if (ipv->isEgal(&mpr)) {
			neighborIP.erase(ipv);
			break;
		}
	}

	/*
	 * on place les route a twoHOP suprimé a supprimé sur la route systeme !
	 */

	mMutexTwoHopTable.lock();
	for (std::list<Route>::iterator tableroute = mTwoHopNeighborTable.begin();
			tableroute != mTwoHopNeighborTable.end(); tableroute++) {
		for (std::list<IPv6>::iterator ipv = vMpr.begin(); ipv != vMpr.end();
				ipv++) {
			IPv6 ipToComp = *ipv;
			if (tableroute->getIpDest()->isEgal(&ipToComp)) {
				if (!tableroute->getNextHop()->isEgal(&mpr)) {
					tableroute->setAction(DEL);
				}
			}
		}
	}
	mMutexTwoHopTable.unlock();

	for (std::list<std::list<IPv6> >::iterator vi = twoHopNeighborIP.begin();
			vi != twoHopNeighborIP.end(); ++vi) {
		IPv6 ipToComp = *(vi->begin());
		for (std::list<IPv6>::iterator ipv = vMpr.begin(); ipv != vMpr.end();
				ipv++) {
			if (ipv->isEgal(&ipToComp)) {
				vi = twoHopNeighborIP.erase(vi);
				vi--;
			}
		}
	}

	if (twoHopNeighborIP.size() != 0) {
		selectMpr(twoHopNeighborIP, neighborIP);
	}
	return 0;
}

std::string Node::setInterface() {
	std::string iface = getResCmd("iwconfig | awk '$0 ~ /IEEE/ { print $1 }'");
	iface = iface.substr(0, (unsigned) iface.length() - 1);
	return iface;
}

int Node::addNeighborTable(Route *route) {
// maté le tableau dès qu'on tombe sur un NULL on ajoute
// erreur si Plein
// erreur si metric !=1
	if (route->getMetric() != 1) {
		std::cout << "ERROR : Add Neighbor with Metric > 1" << std::endl;
		return 2;
	} else {
		mMutexNeighborTable.lock();
		for (std::list<Route>::iterator it = mNeighborTable.begin();
				it != mNeighborTable.end(); ++it) {
			if (it->getIpDest()->isEgal(route->getIpDest())) {
				std::cout << "ERROR : Neighbor Already Exist" << std::endl;
				mMutexNeighborTable.unlock();
				return 3;
			}
		}
		route->setAction(ADD);
		mNeighborTable.push_back(*route);
		mMutexNeighborTable.unlock();
		return 0;
	}
	return 1;
}

int Node::addNeighbor(Route* route) {
	if (addNeighborTable(route) == 0) {
		mMutexIP.lock();
		mNeighborIP.push_back(*route->getIpDest());
		mMutexIP.unlock();
		return 0;
	}
	std::cout << "ERROR : Adding Neighbor Route\n";
	return 1;

}
int Node::addNeighbor(IPv6* ipDest, IPv6* nextHop, int metric,
		std::string iface) {
	Route *route = new Route(ipDest, nextHop, metric, iface);
	return addNeighbor(route);
}

int Node::addTwoHopNeighborTable(Route *route) {

	if (route->getMetric() != 2) {
		std::cout << "ERROR : Add Two Hop Neighbor with Metric != 2"
				<< std::endl;
		return 2;
	} else {
		bool nextHopExist = false;
		mMutexTwoHopTable.lock();
		for (std::list<Route>::iterator it = mTwoHopNeighborTable.begin();
				it != mTwoHopNeighborTable.end(); ++it) {
			if (it->getIpDest()->isEgal(route->getIpDest())) {
				std::cout << "ERROR : Two Hop Neighbor Already Exist"
						<< std::endl;
				mMutexTwoHopTable.unlock();
				return 3;
			}
		}
		mMutexNeighborTable.lock();
		for (std::list<Route>::iterator it = mNeighborTable.begin();
				it != mNeighborTable.end(); ++it) {
			if (it->getIpDest()->isEgal(route->getIpDest())) {
				std::cout << "ERROR : Dest is a Neighbor" << std::endl;
				mMutexNeighborTable.unlock();
				mMutexTwoHopTable.unlock();
				return 4;
			}
			if (it->getIpDest()->isEgal(route->getNextHop())) {
				nextHopExist = true;
			}
		}
		mMutexNeighborTable.unlock();
		if (nextHopExist) {
			route->setAction(ADD);
			mTwoHopNeighborTable.push_back(*route);
			mMutexTwoHopTable.unlock();
			return 0;
		} else {
			std::cout << "ERROR : Hop to access Destination doesn't exists\n";
			mMutexTwoHopTable.unlock();
			return 5;
		}
	}
	return 1;
}

int Node::addTwoHopNeighbor(Route* route) {
	std::list<IPv6> TwoHop;
	if (addTwoHopNeighborTable(route) == 0) {
		TwoHop.push_back(*route->getIpDest());
		TwoHop.push_back(*route->getNextHop());
		mMutexIP.lock();
		mTwoHopNeighborIP.push_back(TwoHop);
		clearMpr();
		selectMpr(mTwoHopNeighborIP, mNeighborIP);
		mMutexIP.unlock();
		return 0;
	}
	std::cout << " ERROR : Adding TwoHop route" << std::endl;
	return 1;
}
int Node::addTwoHopNeighbor(IPv6* ipDest, IPv6* nextHop, int metric,
		std::string iface) {
	Route *route = new Route(ipDest, nextHop, metric, iface);
	return addTwoHopNeighbor(route);
}

int Node::delNeighbor(IPv6* ipToDelete) {

	/*
	 * On place action de la route sur mNeighborTable a DEL
	 */
	mMutexNeighborTable.lock();
	for (std::list<Route>::iterator it = mNeighborTable.begin();
			it != mNeighborTable.end(); ++it) {
		if (it->getIpDest()->isEgal(ipToDelete)) {
			it->setAction(DEL);
			break;
		}
	}
	mMutexNeighborTable.unlock();

	/*
	 * Supprime le neighbor de la liste des HopNgihborIP
	 */
	bool unlock = false;
	mMutexIP.lock();
	for (std::list<IPv6>::iterator it = mNeighborIP.begin();
			it != mNeighborIP.end(); ++it) {
		if (it->isEgal(ipToDelete)) {
			mNeighborIP.erase(it);
			clearMpr();
			selectMpr(mTwoHopNeighborIP, mNeighborIP);
			unlock = true;
			mMutexIP.unlock();
			return 0;
		}
	}
	if (!unlock) {
		mMutexIP.unlock();
	}
	std::cout << "ERROR : Neighbor not Found \n";
	return 1;
}
int Node::delNeighbor(Route* route) {
	IPv6* ip = route->getIpDest();
	return delNeighbor(ip);
}

int Node::delTwoHopNeighbor(IPv6* ipToDelete, IPv6* ipHopToDelete) {
// maté route.sIpDest du tableau
// Remonter tout les élém en dessous
// ajouter NULL a la derniere ligne
	/*
	 * Place l'action de la route sur mTwoHopNeighborTable a DEL
	 */
	mMutexTwoHopTable.lock();
	for (std::list<Route>::iterator it = mTwoHopNeighborTable.begin();
			it != mTwoHopNeighborTable.end(); ++it) {
		if (it->getIpDest()->isEgal(ipToDelete)
				&& it->getNextHop()->isEgal(ipHopToDelete)) {
			it->setAction(DEL);
			break;
		}
	}
	mMutexTwoHopTable.unlock();
	/*
	 * Supprime le 2 hop neighbor de la liste des TwoHopNgihborIP
	 */
	bool unlock = false;
	mMutexIP.lock();
	for (std::list<std::list<IPv6> >::iterator it2 = mTwoHopNeighborIP.begin();
			it2 != mTwoHopNeighborIP.end(); ++it2) {
		std::list<IPv6> temp = *it2;
		std::list<IPv6>::iterator tempIt = temp.begin();
		if (tempIt->isEgal(ipToDelete)) {
			++tempIt;
			if (tempIt->isEgal(ipHopToDelete)) {
				mTwoHopNeighborIP.erase(it2);
				clearMpr();
				selectMpr(mTwoHopNeighborIP, mNeighborIP);
				unlock = true;
				mMutexIP.unlock();
				return 0;
			}
		}
	}
	if (!unlock) {
		mMutexIP.unlock();
	}
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
	std::string commande ="ifconfig | grep "+mInterface+"| awk '$0 ~ /HWaddr/ { print $5 }'";
	char * cmd = (char*) commande.c_str();
	// todo : renvoyer erreur si chaine vide
	std::string result = getResCmd(cmd);
	std::string IPv6;
	if (result.empty()) {
		std::cout << "MAC adress not found\n";
		//todo ; retourner erreur
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

	// On n'insère pas le préfixe ici car il sera insérer par le constructeur de la classe IPv6

	return IPv6;
}

