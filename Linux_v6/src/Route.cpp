/*!
 * Route.cpp
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

#include "Route.h"

Route::Route() {
	mIpDest = new IPv6();
	mNextHop = new IPv6();
	mMetric = 0;
	mInterface = "wlan0";
	mAction = 0;
}

Route::Route(IPv6 *ipDest,
				IPv6 *nextHop,
				int metric,
				std::string interface){
	mIpDest = ipDest;
	mNextHop = nextHop;
	mMetric = metric;
	mInterface = interface;
	mAction = ADD;
}

IPv6* Route::getIpDest(){
	return mIpDest;
}

IPv6* Route::getNextHop(){
	return mNextHop;
}

int Route::getMetric(){
	return mMetric;
}

std::string Route::getInterface(){
	return mInterface;
}

int Route::getAction(){
	return mAction;
}

void Route::setAction(int action){
	mAction=action;
}

void Route::setRoute(Route *route){
	mIpDest->setIPv6(route->getIpDest());
	mNextHop->setIPv6(route->getNextHop());
	mMetric = route->getMetric();
	mInterface = route->getInterface();
	mAction = UPD;
}

Route::~Route() {
	//delete mIpDest, mNextHop, mInterface;
}

