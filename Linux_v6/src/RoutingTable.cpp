/** ADHOC OLSR PROJECT
 *
 * File : RoutingTable.cpp
 *
 * Created on: 27 janv. 2014
 *     Author: CHAHAL Karim & NGOMA Alban (ADHOC TEAM)
 *
 * Description : This file is concerning the application (and system) routing table management,
 * 				 with all his methods.
 */
#include "RoutingTable.h"

RoutingTable::RoutingTable() {
}

/** METHODS */

void RoutingTable::addRoute(Route *r, bool neighb) {
	/** systeme call to add a new route r in the kernel IPv6 Routing Table. */

	Route *temp = r;
	std::ostringstream syscall;
	if (neighb) {
		syscall << "ip -6 route add " << temp->getIpDest()->toChar()
				<< "/128  metric " << temp->getMetric() << " dev "
				<< temp->getInterface();
	} else {
		syscall << "ip -6 route add " << temp->getIpDest()->toChar()
				<< "/128 via " << temp->getNextHop()->toChar() << " metric "
				<< temp->getMetric() << " dev " << temp->getInterface();
	}
	if (system((syscall.str()).c_str()))
		std::cout << "ERROR SysCall add\n" << syscall.str() << std::endl;

}

void RoutingTable::deleteRoute(Route *r, bool neighb) {
	/** syscall to delete a existing route r in the kernel IPv6 Routing Table. */
	Route *temp = r;
	std::ostringstream syscall;
	if (neighb) {
		syscall << "ip -6 route del " << temp->getIpDest()->toChar()
				<< "/128 metric " << temp->getMetric() << " dev "
				<< temp->getInterface();
	} else {
		syscall << "ip -6 route del " << temp->getIpDest()->toChar()
				<< "/128 via " << temp->getNextHop()->toChar() << " metric "
				<< temp->getMetric() << " dev " << temp->getInterface();
	}
	if (system((syscall.str()).c_str()))
		std::cout << "ERROR SysCall del\n" << syscall.str() << std::endl;

}

void RoutingTable::updateRoute(Route *r, bool neighb) {
	/** to update a route in the Routing Table */
	Route *temp = r;
	deleteRoute(temp, neighb);
	addRoute(temp, neighb);

}

void RoutingTable::systemTableUpdate(Node *noeud) {
	//std::cout << tableNeighbor.size() << "test\n";
	//mRouteList.clear();
	noeud->lockSystem();
	mRouteList = noeud->getNeighborTable();
	for (std::list<Route>::iterator it = mRouteList.begin();
			it != mRouteList.end(); it++) {
		Route route = *it;
		switch (it->getAction()) {
		case ADD: {
			addRoute(&route,true);
			it->setAction(NONE);
			it--;
			break;
		}
		case DEL: {
			//std::cout << " deleteVoisin \n";
			deleteRoute(&route,true);
			mRouteList.erase(it);
			it--;


			break;
		}
		case UPD: {
			updateRoute(&route,true);
			it->setAction(NONE);
			//std::cout << " updVoisin \n";
			break;
		}

		case NONE: {
			//std::cout << " noneVoisin\n";
			break;
		}

		default: {
			std::cout << "Erreur Voisin : Action non définie!" << std::endl;
			break;
		}

		}

	}

	noeud->setNeighborTable(mRouteList);

	mRouteList.clear();
	mRouteList = noeud->getTwoHopNeighborTable();


	for (std::list<Route>::iterator it = mRouteList.begin();
			it != mRouteList.end(); it++) {
		Route route = *it;
		switch (route.getAction()) {
		case ADD: {
			//std::cout << "addroute Voisin+2\n";
			addRoute(&route,false);
			it->setAction(NONE);

			break;
		}
		case DEL: {
			//std::cout << " delete Voisin+2\n";
			deleteRoute(&route,false);
			it = mRouteList.erase(it);
			it--;
			break;
		}
		case UPD: {
			//std::cout << " upd Voisin+2\n";
			updateRoute(&route,false);
			it->setAction(NONE);
			break;
		}

		case NONE: {

			//std::cout << " no Voisin+2\n";
			break;
		}

		default: {
			std::cout << "Erreur Voisin+2: Action non définie!" << std::endl;
			break;
		}

		} // end of switch

	} //end of for
	noeud->setTwoHopNeighborTable(mRouteList);
	mRouteList.clear();
	noeud->lockSystem();
		mRouteList = noeud->getDestTable();
		for (std::list<Route>::iterator it = mRouteList.begin();
				it != mRouteList.end(); it++) {
			Route route = *it;
			switch (it->getAction()) {
			case ADD: {
				addRoute(&route,true);
				it->setAction(NONE);
				it--;
				break;
			}
			case DEL: {
				//std::cout << " deleteVoisin \n";
				deleteRoute(&route,true);
				mRouteList.erase(it);
				it--;


				break;
			}
			case UPD: {
				updateRoute(&route,true);
				it->setAction(NONE);
				//std::cout << " updVoisin \n";
				break;
			}

			case NONE: {
				//std::cout << " noneVoisin\n";
				break;
			}

			default: {
				std::cout << "Erreur Voisin : Action non définie!" << std::endl;
				break;
			}

			}

		}

		noeud->setDestTable(mRouteList);
	noeud->releaseSystem();
}

