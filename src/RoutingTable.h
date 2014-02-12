/* ADHOC OLSR PROJECT
 *
 * File : RoutingTable.h
 *
 * Created on: 27 janv. 2014
 *     Author: CHAHAL Karim & NGOMA Alban (ADHOC TEAM)
 *
 * Description : Header of RoutingTable class.
 */

#ifndef ROUTINGTABLE_H_
#define ROUTINGTABLE_H_
#include "Route.h"
#include "Node.h"
#include <stdlib.h>
#include <list>
#include <stdio.h>

class RoutingTable{

public:
	RoutingTable(); // Initializes the Application Routing Table.

	void systemTableUpdate(Node *noeud);

	inline std::list<Route> getRouteList(){ // returns the private attribute routeList containing all the routes
			return mRouteList;
		}

private:
	std::list<Route> mRouteList;

	/**
	 *  Function addRoute
	 *
	 *  	add a new route in the Routing Table of the application
	 *  @param Route r
	 */
	void addRoute(Route *r); /** add a new route in the Routing Table of the application. */

	/**
	 *  Funcion delete route
	 *  	delete an existing route from the Routing table of the application
	 */
	void deleteRoute(Route *r); /** delete an existing route from the Routing Table of the application. */
	void updateRoute(Route *r); /** replace an existing by r if route r is better than the existing one. */
};


#endif /* ROUTINGTABLE_H_ */
