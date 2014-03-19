/*
 * Controller.cpp
 *
 *  Created on: 6 mars 2014
 *      Author: maxime
 */

#include "Controller.h"

Controller::Controller() {
	// TODO Auto-generated constructor stub
	mNode = new Node();
	mListener = new Listener();
	mRoutingTable = new RoutingTable();

}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
	delete mNode;
	delete mListener;
	delete mRoutingTable;
}

void Controller::run() {
	if (mListener->run()) {
		int i=0;
		while (1) {
			i = (i+1)%5;
			Message tmp = mListener->getMsg();
			if (!(tmp.getTimeToLive() < 0)) { // TODO verifier le TTL
				Message *msg = &tmp;
				int type = msg->getMessageType();
				if (type == HELLO_TYPE) { // ajouter le type de HELLO dans const.h
					Hello *helloMsg = (Hello *) msg;
					traitementHello(helloMsg);
				} else if (type == TC_TYPE) { // TC_TYPE a ajouter dans const.h
					Tc *tcMsg = (Tc*) msg;
					traitementTc(tcMsg);
				}
			}
			if(i==4){
				mRoutingTable->systemTableUpdate(mNode);
			}
		}
	}
}
void Controller::traitementHello(Hello* msg) {
	IPv6* origIp = msg->getOriginatorAddress();
	std::list<IPv6> listNghbNode = mNode->getNeighborIP();
	bool find = false;
	for (std::list<IPv6>::iterator itListNghbNode = listNghbNode.begin();
			itListNghbNode != listNghbNode.end(); itListNghbNode++) {
		if ((*itListNghbNode).isEgal(origIp)) {
			find = true;
			for (std::list<Destination>::iterator itDest = mDestination.begin();
					itDest != mDestination.end(); itDest++) {
				if (origIp->isEgal(itDest->getIp())) {
					itDest->resetTimer();
					break;
				}
			}
			break;
		}
	}
	if (!find) {
		mNode->addNeighbor(origIp, origIp, 1, mNode->getInterface());
		Destination *dest = new Destination(origIp, 1);
		mDestination.push_back(*dest);
	}
//fin traitement originator ADDRESSE

	std::list<HelloNeighborList> listNeighbor = msg->getNeighbors();

	for (std::list<HelloNeighborList>::iterator itListNeighbor =
			listNeighbor.begin(); itListNeighbor != listNeighbor.end();
			itListNeighbor++) {
		if ((*itListNeighbor).getLinkCode() == LINK_CODE_MPR) {
			std::list<IPv6> listIp = (*itListNeighbor).getNeighborsAddrList();
			bool advertise = false;
			for (std::list<IPv6>::iterator itIp = listIp.begin();
					itIp != listIp.end(); itIp++) {
				if (itIp->isEgal(mNode->getMyIp())) {
					advertise = true;
					mNode->addAdvertisedNeighbor(origIp);
				} else {
					mNode->addTwoHopNeighbor(&(*itIp), origIp, 2,
							mNode->getInterface());
				}
			}
			if (!advertise) {
				mNode->delAdvertisedNeighbor(origIp);
			}
		} else if ((*itListNeighbor).getLinkCode() == LINK_CODE_NGHB) {
			std::list<IPv6> listIp = (*itListNeighbor).getNeighborsAddrList();
			for (std::list<IPv6>::iterator itIp = listIp.begin();
					itIp != listIp.end(); itIp++) {
				mNode->addTwoHopNeighbor(&(*itIp), origIp, 2,
						mNode->getInterface());
			}
		} else {
			std::cout << " ERREUR LINK CODE\n";
		}
	}
}

void Controller::traitementTc(Tc* msg) {
	std::list<IPv6> listAdvertised = msg->getAdvertisedNeighborMainAddress();
	for (std::list<IPv6>::iterator itMsg = listAdvertised.begin();
			itMsg != listAdvertised.end(); itMsg++) {
		bool find = false;
		for (std::list<IPv6>::iterator itNode = mNode->getDestIP().begin();
				itNode != mNode->getDestIP().end(); itNode++) {
			IPv6 * ipToComp = &(*itNode);

			if (itMsg->isEgal(ipToComp)) {
				find = true;
				for (std::list<Destination>::iterator itDest =
						mDestination.begin(); itDest != mDestination.end();
						itDest++) {
					if (itMsg->isEgal(itDest->getIp())) {
						itDest->resetTimer();
						break;
					}
				}
				break;
			}
		}
		if (!find) {
			Route *route = new Route(&(*itMsg), NULL/*TODO IP DERNIER FORWARD*/,
					msg->getHopCount(), mNode->getInterface());
			mNode->addDestTable(route);
			Destination *dest = new Destination(&(*itMsg), msg->getHopCount());
			mDestination.push_back(*dest);
		}
	}
}

