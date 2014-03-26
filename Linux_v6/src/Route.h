/*!
 * Route.h
 *
 *  Created on: 1 Février 2014
 *      Author: Joran LeCallonec & Savary Maxime
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


class Route
{

private:
	IPv6 	*mIpDest,
			*mNextHop;
	int		mMetric;
	std::string 	mInterface;
	int 	mAction;

public:
	Route();
	/*!
	 * CONSTRUCTOR
	 *
	 * 		ROUTE need a destination IP, a nextHop IP, a metric (how many hops), and the interface
	 */
	Route(IPv6 *ipDest,
			IPv6 *nextHop,
			int metric,
			std::string interface);

	/*!
	 * FUNC getIpDest
	 *
	 * @returns IPv6 : destination IP of the route
	 */
	IPv6* getIpDest();

	/*!
	 * FUNC getNextHop
	 *
	 * @returns IPv6 : nextHop IP of the route
	 */
	IPv6* getNextHop();

	/*!
	 *  FUNC getMetric
	 *
	 *  @returns int : metric of the route
	 */
	int getMetric();

	/*!
	 * FUNC getInterface
	 *
	 * @returns string : interface of the route
	 */
	std::string getInterface();

	/*!
	 *  FUNC getAction
	 *
	 *  @returns int : Action of the route ADD / DEL / UPD
	 */
	int getAction();


	/*!
	 *  FUNC setAction
	 *
	 *  @param int : Action to set
	 */
	void setAction(int action);


	/*!
	 *  FUNC setRoute
	 *
	 *  @params Route :route to change
	 */
	void setRoute(Route *route);
	virtual ~Route();
};

#endif /* ROUTE_H_ */
