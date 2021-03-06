/*!
 * \file const.h
 *
 * \date 15 mars 2014
 * \author Le Callonec Joran & Savary Maxime
 *
 * \brief regroup all global values
 */

/*
 * This file is part of Ad-Hoc Networks an app base on OLSR to handle Ad-Hoc
 *  network.
 *
 * Copyright (c) 2014-2014 Gauthier Quesnel <quesnel@users.sourceforge.net>
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

/// MESSAGE TYPE
#define HELLO_MESSAGE 1
#define TC_MESSAGE 2
#define MID_MESSAGE 3
#define HNA_MESSAGE 4

/// LINK TYPE for linkcode
#define UNSPEC_LINK 0
#define ASYM_LINK 1
#define SYM_LINK 2
#define LOST_LINK 3

/// NEIGHBOR TYPE
#define NOT_NEIGH 0
#define SYM_NEIGH 1
#define MPR_NEIGH 2

/// LINK CODE VALUES //todo mettre les valeurs exacte
#define LINK_CODE_MPR 2
#define LINK_CODE_NGHB 1

/// LINK Hysteresis
#define HYST_THRESHOLD_HIGH 0.8
#define HYST_THRESHOLD_LOW 0.3
#define HYST_THRESHOLD_SCALING 0.5

/// WILLINGNESS
#define WILL_NEVER 0
#define WILL_LOW 1
#define WILL_DEFAULT 3
#define WILL_HIGH 6
#define WILL_ALWAYS 7

/// EMISSION INTERVALS
#define HELLO_INTERVAL 2
#define REFRESH_INTERVAL 2
#define TC_INTERVAL 5
#define MID_INTERVAL 5
#define HNA_INTERVAL 5

/// HOLDING TIME
#define NEIGHB_HOLD_TIME 3*REFRESH_INTERVAL
#define TOP_HOLD_TIME 3*TC_INTERVAL
#define DUP_HOLD_TIME 30
#define MID_HOLD_TIME 3*MID_INTERVAL
#define HNA_HOLD_TIME 3*HNA_INTERVAL

/// Miscellaneous Constants
#define TC_REDUNDANCY 0
#define MPR_COVERAGE 1
#define MAX_JITTER HELLO_INTERVAL/4

/// IPv6 prefix
#define EN_TETE_IPv6 0x2014

/// OLSR PORT
#define PORT 698

/// Size for Listener Buffer
#define BUFF_SIZE 1280 // size in bytes
/// Max numbers of message store
#define MAX_LENGTH 50
#define MIN_LENGTH 0

/// Controller
#define HELLO_TYPE 1
#define TC_TYPE 2

/// Route Action
#define ADD 1
#define DEL 2
#define UPD 3
#define NONE 0
