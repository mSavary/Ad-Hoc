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
	/*mListener->listenSocket(); // dans un thread car c'est le prod
	 while (1){
	 //Message *msg = mListener->getMsg();
	 int type = msg->getMessageType();
	 if(type == HELLO_TYPE){// ajouter le type de HELLO dans const.h
	 Hello* helloMsg= (Hello*)msg;
	 traitementHello(helloMsg);
	 } else if (type == TC_TYPE){// TC_TYPE a ajouter dans const.h
	 Tc* tcMsg = (Tc*)msg;
	 traitementTc(tcMsg);
	 }
	 }*/
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
				} else{
					mNode->addTwoHopNeighbor(&(*itIp),origIp,2,mNode->getInterface());
				}
			}
			if (!advertise) {
				mNode->delAdvertisedNeighbor(origIp);
			}
		} else if ((*itListNeighbor).getLinkCode() == LINK_CODE_NGHB) {
			std::list<IPv6> listIp = (*itListNeighbor).getNeighborsAddrList();
			for (std::list<IPv6>::iterator itIp = listIp.begin();
					itIp != listIp.end(); itIp++) {
				mNode->addTwoHopNeighbor(&(*itIp),origIp,2,mNode->getInterface());
			}
		} else{
			std::cout<<" ERREUR LINK CODE\n";
		}
	}
}

void Controller::traitementTc(Tc* msg) {

}

