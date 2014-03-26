/*!
 * \file RoutingTable.cpp
 *
 *  \date 1 Février 2014
 *      \author Chahal Karim & Ngoma Alban
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
 
#include "RoutingTable.h"

RoutingTable::RoutingTable() {
}



void RoutingTable::addRoute(Route *r, bool neighb) {
	

	std::ostringstream syscall;
	if (neighb) {
		syscall << "ip -6 route add " << r->getIpDest()->toChar()
				<< "/128  metric " << r->getMetric() << " dev "
				<< r->getInterface();
	} else {
		syscall << "ip -6 route add " << r->getIpDest()->toChar()
				<< "/128 via " << r->getNextHop()->toChar() << " metric "
				<< r->getMetric() << " dev " << r->getInterface();
	}
	if (system((syscall.str()).c_str()))
		std::cout << "ERROR SysCall add\n" << syscall.str() << std::endl;
	std::cout << " ADD ROUTE KERNEL \n";

}

void RoutingTable::deleteRoute(Route *r, bool neighb) {

	std::ostringstream syscall;
	if (neighb) {
		syscall << "ip -6 route del " << r->getIpDest()->toChar()
				<< "/128 metric " << r->getMetric() << " dev "
				<< r->getInterface();
	} else {
		syscall << "ip -6 route del " << r->getIpDest()->toChar()
				<< "/128 via " << r->getNextHop()->toChar() << " metric "
				<< r->getMetric() << " dev " << r->getInterface();
	}
	if (system((syscall.str()).c_str()))
		std::cout << "ERROR SysCall del\n" << syscall.str() << std::endl;

}

void RoutingTable::updateRoute(Route *r, bool neighb) {
	
	deleteRoute(r, neighb);
	addRoute(r, neighb);

}

void RoutingTable::systemTableUpdate(Node *noeud) {
	//std::cout << tableNeighbor.size() << "test\n";
	//mRouteList.clear();
	noeud->lockSystem();
	mRouteList = noeud->getNeighborTable();
	for (std::list<Route*>::iterator it = mRouteList.begin();
			it != mRouteList.end(); it++) {
		switch ((*it)->getAction()) {
		case ADD: {
			addRoute((*it), true);
			(*it)->setAction(NONE);
			break;
		}
		case DEL: {
			//std::cout << " deleteVoisin \n";
			deleteRoute((*it), true);
			it = mRouteList.erase(it);
			break;
		}
		case UPD: {
			updateRoute((*it), true);
			(*it)->setAction(NONE);
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

	for (std::list<Route*>::iterator it = mRouteList.begin();
			it != mRouteList.end(); it++) {
		switch ((*it)->getAction()) {
		case ADD: {
			//std::cout << "addroute Voisin+2\n";
			addRoute((*it), false);
			(*it)->setAction(NONE);

			break;
		}
		case DEL: {
			//std::cout << " delete Voisin+2\n";
			deleteRoute((*it), false);
			it = mRouteList.erase(it);

			break;
		}
		case UPD: {
			//std::cout << " upd Voisin+2\n";
			updateRoute((*it), false);
			(*it)->setAction(NONE);
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
	mRouteList = noeud->getDestTable();
	for (std::list<Route*>::iterator it = mRouteList.begin();
			it != mRouteList.end(); it++) {
		switch ((*it)->getAction()) {
		case ADD: {
			addRoute((*it), false);
			(*it)->setAction(NONE);
			break;
		}
		case DEL: {
			//std::cout << " deleteVoisin \n";
			deleteRoute((*it), false);
			it = mRouteList.erase(it);
			break;
		}
		case UPD: {
			updateRoute((*it), false);
			(*it)->setAction(NONE);
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

