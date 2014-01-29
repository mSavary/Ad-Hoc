#include "Node.h"

Node::Node(){
	mMpr=false;
	mInterface = new IPv6(0xAA00, 0xAA00, 0xAA00, 0xAA00, 0xAA00, 0xAA00, 0xAA00, 0xAA00);

	for (int i=0; i<MAX_NEIGHBOR; i++){
		mNeighborTable[i]=new Route();
	}

	for (int i=0; i<MAX_TWOHOPNEIGHBOR; i++){
		mTwoHopNeighborTable[i]=new Route();
	}

	mTimerHello= 2;
	mTimerTc = 5;

}

void Node::sendHello(){
// appelé par le timer
// construire le Hello ( voir si on le fait dans le constructeur vu qu'il change pas ? )
// envoi du Hello
// redéclencher le timer
}

void Node::sendTc(){
// appelé par le timer
// construire le TC ( peut etre != a chaque envoi )
// regarder si on est MPR
// 	alors envoi du TC
// redéclencher le timer
}

int Node::addNeighbor(Route *route){
// maté le tableau dès qu'on tombe sur un NULL on ajoute
// erreur si Plein
// erreur si metric !=1
	if (route->getMetric()!=1)
		return 2;

	int i=0;
	IPv6 ipNull();
	//while(mNeighborTable[i].getIpDest()!=ipNull && i<MAX_NEIGHBOR){
	//@TODO: SUCHARGER OPERATEUR != et == pour comp 2 IPv6
		//i++;
	/*	@TODO SURCHARGER OPERATEUR != et == pour 2  route
	 * if (mNeighborTable[i] == route){
			break;
		}*/
    //}
	if (i==MAX_NEIGHBOR)
		return 1;
	else
		mNeighborTable[i]->setRoute(route);
return 0;
}
int Node::addNeighbor(int ipDest, int ipSource, int metric, int nextHop){
// on créer la route
// addNeighbor(Route route);
return 0;
}

int Node::addTwoHopNeighbor(Route *route){
// MAté le tableau TwoHOP Null on ajoute la route
// erreur si plein
// erruer si metric !=2
return 0;
}
int Node::addTwoHopNeighbor(int ipDest, int ipSource, int metric, int nextHop){
// on créer la route
// addTwoHopNeighbor(Route route);
return 0;
}

int Node::delNeighbor(int ipToDelete){
// maté route.sIpDest du tableau
// Remonter tout les élém en dessous
// ajouter NULL a la derniere ligne
return 0;
}

int Node::delTwoHopNeighbor(int ipToDelete){
// maté route.sIpDest du tableau
// Remonter tout les élém en dessous
// ajouter NULL a la derniere ligne
return 0;}
