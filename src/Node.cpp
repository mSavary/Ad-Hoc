#include "Node.h"

Node::Node(){

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

int Node::addNeighbor(Route route){
// maté le tableau dès qu'on tombe sur un NULL on ajoute
// erreur si Plein
// erreur si metric !=1
return 0;
}
int Node::addNeighbor(int ipDest, int ipSource, int metric, int nextHop){
// on créer la route
// addNeighbor(Route route);
return 0;
}

int Node::addTwoHopNeighbor(Route route){
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
