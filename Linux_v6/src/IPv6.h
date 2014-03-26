/*!
 * \file IPv6.h
 *
 * \date 13 janvier 2014
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

#ifndef IPV6_H_
#define IPV6_H_
#include <sstream>
#include <list>
#include <iostream>
#include "const.h"

/*!
 * \class IPv6
 * \brief Object representing an IPv6.
 *
 */
class IPv6 {
private:
	short mScope[8];/*!< Represent 8 scopes of an IPv6 2014:0:0:0:a:a:a:a*/

public:
	IPv6();
	/*!
	 * \fn IPv6 (short scope5, short scope6, short scope7, short scope8);
	 *
	 * 	\brief Able to create an IPv6 object from 4 last short of the IPv6.
	 *
	 * 	\param scope5 the 5th scope of the IPv6
	 * 	\param scope6 the 6th scope of the IPv6
	 * 	\param scope7 the 7th scope of the IPv6
	 * 	\param scope8 the 8th scope of the IPv6
	 */
	IPv6(short scope5, short scope6, short scope7, short scope8);

	/*!
	 * \fn IPv6 (std::string IPv6Str)
	 *
	 * \brief Able to create an IPv6 object from the string of the IPv6.
	 *
	 * \param IPv6Str string from which the IPv6 Object will be build
	 */
	IPv6(std::string IPv6Str);

	/*!
	 * \fn short getScope (int scope)
	 *
	 * 	\return int the scope of the IPv6
	 * 	\param scope (0-7) represent the scope you want to get.
	 */
	short getScope(int scope);

	/*!
	 * \fn void setIPv6 (IPv6* ip)
	 *
	 * \brief Change the IPv6 by an other one.
	 * \param ip the new IPv6
	 */
	void setIPv6(IPv6* ip);

	/*!
	 * \fn std::string toChar ()
	 *
	 * \return std::string the string format of the IPv6
	 */
	std::string toChar();

	/*!
	 * \fn bool isEgal(IPv6* ip)
	 *
	 * \brief Compare 2 IPv6
	 * \param ip the ip you want to compare to
	 * \return bool true if the 2 IPv6 are egal else false.
	 */
	bool isEgal(IPv6* ip);

	virtual ~IPv6();
};

#endif /* IPV6_H_ */
