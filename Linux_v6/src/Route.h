/*!
 * \file Route.h
 *
 * \date 1 Février 2014
 * \author Joran LeCallonec & Savary Maxime
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

#ifndef ROUTE_H_
#define ROUTE_H_

#include "IPv6.h"

/*!
 * \class Route
 * \brief Class route is made to represent a routing table entry.
 */
class Route
{

private:
	IPv6 	*mIpDest,
			*mNextHop;/*!< ipDestination and ipNexthop to represent the dest and the next hop*/
	int		mMetric;/*!< represent the hop counts of the */
	std::string 	mInterface;/*!< the interface for the route */
	int 	mAction;/*!< Action update add delete or none of the route */

public:
	Route();
	/*!
	 * \fn Route (IPv6 *ipDest,	IPv6 *nextHop, int metric, std::string interface)
	 *
	 * \param ipDest IPv6 of the destination
	 * \param nextHop IPv6 of the next hop to reach the destination
	 * \param metric number of hops to reach the Dest
	 * \param interface the interface used by the route
	 */
	Route(IPv6 *ipDest,
			IPv6 *nextHop,
			int metric,
			std::string interface);

	/*!
	 * \fn IPv6* getIpDest ()
	 *
	 * \return IPv6* Destination IP of the route
	 */
	IPv6* getIpDest();

	/*!
	 * \fn IPv6* getNextHop ()
	 *
	 * \return IPv6*  nextHop IP of the route
	 */
	IPv6* getNextHop();

	/*!
	 *  \fn int getMetric ()
	 *
	 *  \return int Metric of the route
	 */
	int getMetric();

	/*!
	 * \fn std::string getInterface()
	 *
	 * \return string Interface of the route
	 */
	std::string getInterface();

	/*!
	 *  \fn int getAction ()
	 *
	 *  \return int  Action of the route ADD / DEL / UPD
	 */
	int getAction();


	/*!
	 *  \fn void setAction(int action)
	 *
	 *  \param int Action to set
	 */
	void setAction(int action);


	/*!
	 *  \fn void setRoute(Route *route)
	 *
	 *  \param route route to change
	 */
	void setRoute(Route *route);
	virtual ~Route();
};

#endif /* ROUTE_H_ */
