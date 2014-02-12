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
#include <string>
#include <string.h>
#include <sstream>

RoutingTable::RoutingTable() {
}

/** METHODS */

void RoutingTable::addRoute(Route *r) {
	/** systeme call to add a new route r in the kernel IPv6 Routing Table. */

	Route *temp = r;
	std::ostringstream syscall;
	syscall << "ip -6 route add " << temp->getIpDest()->toChar() << "/128 via "
			<< temp->getNextHop()->toChar() << " metric "<<temp->getMetric()<< " dev " << temp->getInterface();
	if (system((syscall.str()).c_str()))
		std::cout << "ERROR SysCall add\n" << syscall.str() << std::endl;

}

void RoutingTable::deleteRoute(Route *r) {
	/** syscall to delete a existing route r in the kernel IPv6 Routing Table. */
	Route *temp = r;
	std::ostringstream syscall;
	syscall << "ip -6 route del " << temp->getIpDest()->toChar() << "/128 via "
			<< temp->getNextHop()->toChar() << " metric "<<temp->getMetric()<<" dev " << temp->getInterface();
	if (system((syscall.str()).c_str()))
		std::cout << "ERROR SysCall del\n" << syscall.str() << std::endl;

}

void RoutingTable::updateRoute(Route *r) {
	/** to update a route in the Routing Table */
	Route *temp = r;
	deleteRoute(temp);
	addRoute(temp);

}

void RoutingTable::systemTableUpdate(Node *noeud) {
	//std::cout << tableNeighbor.size() << "test\n";
	//mRouteList.clear();
	noeud->lockSystem();
	//noeud->lockSystem();
	mRouteList = noeud->getNeighborTable();
	for (std::list<Route>::iterator it = mRouteList.begin();
			it != mRouteList.end(); it++) {
		Route route = *it;
		switch (it->getAction()) {
		case ADD: {
			addRoute(&route);
			it->setAction(NONE);
			it--;
			break;
		}
		case DEL: {
			//std::cout << " deleteVoisin \n";
			mRouteList.erase(it);
			it--;
			deleteRoute(&route);
			noeud->delNeighbor(route.getIpDest());

			break;
		}
		case UPD: {
			updateRoute(&route);
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

	/*for (std::list<Route>::iterator it = mRouteList.begin();
	 it != mRouteList.end(); it++) {
	 std::cout << "IP  ! " << it->getIpDest()->toChar() << std::endl;
	 }*/

	for (std::list<Route>::iterator it = mRouteList.begin();
			it != mRouteList.end(); it++) {
		Route route = *it;
		switch (route.getAction()) {
		case ADD: {
			//std::cout << "addroute Voisin+2\n";
			addRoute(&route);
			it->setAction(NONE);

			break;
		}
		case DEL: {
			//std::cout << " delete Voisin+2\n";
			deleteRoute(&route);
			it = mRouteList.erase(it);
			noeud->delTwoHopNeighbor(&route);
			it--;
			break;
		}
		case UPD: {
			//std::cout << " upd Voisin+2\n";
			updateRoute(&route);
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
	noeud->releaseSystem();
}

