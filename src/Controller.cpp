/*
 * Controller.cpp
 *
 *  Created on: 6 mars 2014
 *      Author: maxime
 */

#include "Controller.h"

Controller::Controller() {
	// TODO Auto-generated constructor stub
	mNode= new Node();
	mListener = new Listener ();

}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::run(){
	mListener->listenSocket(); // dans un thread car c'est le prod
	while (1){
		Message *msg = mListener->getMsg();
		int type = msg->getMessageType();
		if(type == HELLO_TYPE){// ajouter le type de HELLO dans const.h
			Hello* helloMsg= (Hello*)msg;
			traitementHello(helloMsg);
		} else if (type == TC_TYPE){// TC_TYPE a ajouter dans const.h
			Tc* tcMsg = (Tc*)msg;
			traitementTc(tcMsg);
		}
	}
}

void Controller::traitementHello (Hello* msg){
 std::list<IPv6> listeNeighbor;
 listeNeighbor=msg->getNeighbors();
 for(std::list<IPv6>::iterator it=listeNeighbor.begin();it!=listeNeighbor.end();it++){
	 //on regarde si deja dans la liste des voisin +1 ou voisin
	 //si pas dans une des 2 liste on ajoute en voisin +1
	 // + verifié les voisin +1 a supprim si ( voir RFC ) autant de temps passé
 }
 IPv6 *ipNeighbor = msg->getOriginatorAddress();
 for(std::list<IPv6>::iterator it= mNode->getNeighborIP().begin();it!=mNode->getNeighborIP().end();it++){
	 IPv6* ipToComp = &*it;
	 if(ipNeighbor->isEgal(ipToComp)){
		 // redemarer le timer
	 } else {
		 // ajout du voisin
	 }
 }
// regarder comment delete les neighbor ! si on a pas recu de hello de ce voisin depuis ...
}

void Controller::traitementTc (Tc* msg){

}
