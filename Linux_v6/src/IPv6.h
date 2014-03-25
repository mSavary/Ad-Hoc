/*
 * IPv6.h
 *
 *  Created on: 13 janvier 2014
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

#ifndef IPV6_H_
#define IPV6_H_
#include <sstream>
#include <list>
#include <iostream>
#include "const.h"

class IPv6
{
private:
	short mScope[8];

public:
	IPv6();
	/**
	 * CONSTRUCTOR
	 *
	 * 		Able to create an IPv6 object from 4 last short of the IPv6.
	 */
	IPv6(short scope5,
			short scope6,
			short scope7,
			short scope8);

	/**
	 * CONSTRUCTOR
	 *
	 * 		Able to create an IPv6 object from the string of the IPv6.
	 */
	IPv6(std::string IPv6Str);

	/**
	 * FUNC getScope
	 *
	 * 		return the scope.
	 * 	@param scope (0-7) represent the scope you want
	 */
	short getScope(int scope);

	/**
	 * FUNC setIPv6
	 *
	 * 		Change the IPv6 by an other one.
	 * 	@params ip the new IPv6
	 */
	void setIPv6(IPv6* ip);

	/**
	 * FUNC toChar
	 *
	 * @returns string format of the IPv6
	 */
	std::string toChar();

	/**
	 * FUNC isEgal
	 *
	 * 		Compare 2 IPv6
	 * 	@params ip the ip you want to compare to
	 * 	@returns true if the 2 IPv6 are egal.
	 */
	bool isEgal(IPv6* ip);
	virtual ~IPv6();
};

#endif /* IPV6_H_ */
