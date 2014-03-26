/*!
 * \file RoutingTable.h
 *
 * \date 1 Février 2014
 * \author Chahal Karim & Ngoma Alban
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

#ifndef ROUTINGTABLE_H_
#define ROUTINGTABLE_H_
#include "Route.h"
#include "Node.h"
#include <stdlib.h>
#include <list>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>



/*!
 * \class RoutingTable
 * \brief Class Routing Table handle the management of app and OS kernel routing tables.
 *		It allow you to add, delete, and update routes.
 *		It works with the IPv6 protocol.
 */
class RoutingTable{

private:
	std::list<Route*> mRouteList; /*!< list of all the routes*/

	/*!
	 * \fn void addRoute(Route *r,bool neighb);
	 * \brief System call to add a new route r in the kernel IPv6 Routing Table.
	 *
	 * \param r the route to add
	 * \param neighb to know if r is a neighbor 
	 * \return void
	 */
	void addRoute(Route *r,bool neighb); 

	/*!
	 * \fn void deleteRoute(Route *r,bool neighb);
	 * \brief System call to delete an existing route r in the kernel IPv6 Routing Table.
	 *
	 * \param r the route to delete
	 * \param neighb to know if r is a neighbor 
	 * \return void
	 */
	void deleteRoute(Route *r,bool neighb); 
	
	/*!
	 * \fn void updateRoute(Route *r,bool neighb);
	 * \brief System call to update a route r in the kernel IPv6 Routing Table.
	 *
	 * \param r the route to update
	 * \param neighb to know if r is a neighbor 
	 * \return void
	 */
	void updateRoute(Route *r,bool neighb); 


public:

	/*!
	 * \fn RoutingTable()
	 * \brief Initializes your app routing table
	 */
	RoutingTable(); 


	/*!
	 * \fn void systemTableUpdate(Node* noeud);
	 * \brief It runs through the NeighborTable of noeud 
	 *	  and check the action to do for every route.
	 *
	 * \param noeud containing the NeighborTable field to run through it.
	 * \return void
	 */
	void systemTableUpdate(Node* noeud);

};


#endif 
