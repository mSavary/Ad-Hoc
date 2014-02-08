#include "Node.h"

Node::Node() {

	mMpr = false;
	mInterface = new IPv6(macToIpv6());
	mTimerHello = boost::thread(&Node::sendHello, this);
	mTimerTc = boost::thread(&Node::sendTc, this);

}

Node::~Node() {
	// TODO Auto-generated destructor stub
	mTimerHello.join();
	mTimerTc.join();
}

void Node::sendHello() {
// appelé par le timer
// construire le Hello ( voir si on le fait dans le constructeur vu qu'il change pas ? )
// envoi du Hello
// redéclencher le timer

	while (1) {
		sleep(HELLO_INTERVAL);
		std::cout << "\n***** ENVOI HELLO *****\n";
	}
	/* appeler fonctions de construction et d'envoi de message Hello */
}

void Node::sendTc() {
// appelé par le timer
// construire le TC ( peut etre != a chaque envoi )
// regarder si on est MPR
// 	alors envoi du TC
// redéclencher le timer

	while (1) {
		sleep(10);
		std::cout << "\n***** ENVOI TC *****\n";
	}
	/* appeler fonctions de construction et d'envoi de message Tc */
}
/*
 void Node::selectMpr(){
 // 1ere étape
 // définir le noeud avec le + de voisin+1
 mMyMprList.clear();
 IPv6 mpr; // IP du MPR élu
 std::list<IPv6> mprNeighbor; // voisin du MPR
 std::list<std::list<IPv6> > updatelist;
 updatelist=mTwoHopNeighborIP;
 bool ajoutMpr=false;
 std::cout<<"\nSelect MPR TOTO AVANT FOR 1\n";
 for(std::list<IPv6>::iterator voisin=mNeighborIP.begin();voisin!=mNeighborIP.end();++voisin){
 ajoutMpr=false;
 int cptTwoHop =0;
 //std::cout<<"\n select MPR TOTO AVANT FOR 2\n";
 for(std::list<std::list<IPv6> >::iterator twoHop=updatelist.begin();twoHop!=updatelist.end();++twoHop){
 int tmpTwoHop=0;
 std::list<IPv6> tmp; // liste voisin+1, voisin
 tmp = *twoHop;
 //std::cout<<"\n select MPR TOTO AVANT FOR 3\n";
 bool test=false;
 for(std::list<IPv6>::iterator it=tmp.begin();it != tmp.end();++it){
 //std::cout<<"\n ********************select MPR TOTO TEST\n";
 if(test){
 std::list<IPv6>::iterator voisinNeighbor = --it;
 std::cout << voisinNeighbor->toChar() << std::endl;
 std::cout << "Voisin : " << voisin->toChar() << std::endl;
 IPv6 ipToComp=voisin;
 std::cout << "ipTO COMP  : " << ipToComp.toChar() << std::endl;

 //std::cout<<"\n *****DEBUG ******************\n";
 if(it->isEgal(&ipToComp)){
 std::cout<<"\n select MPR TOTO ON TROUVE\n";
 tmpTwoHop++;
 tmp.push_back(*voisinNeighbor);
 std::cout<<"\n select MPR TOTO AJOUT tmp\n";
 }
 } else {
 test=true;
 //std::cout<<"\n ********************BARKDZF?KSDFK?QSDK? QSDC \n";
 }

 }
 if(tmpTwoHop>cptTwoHop){
 ajoutMpr=true;
 std::cout<<"\n prout ooooooooooooooooooooo\n";

 cptTwoHop=tmpTwoHop;
 mpr=voisin;
 mprNeighbor=tmp;
 }
 }
 }
 ;
 if(ajoutMpr){
 mMyMprList.push_back(mpr);// on ajoute le MPR élu a notre liste
 //2em etape
 //retirer lles voisin +1 du voisin choisi au dessus et le voisin
 for(std::list<std::list<IPv6> >::iterator update=updatelist.begin(); update!=updatelist.end();++update){
 for(std::list<IPv6>::iterator it2=update->begin();it2 != update->end();++it2){
 if(it2->isEgal(&mpr))
 updatelist.erase(update);
 }
 }
 // 3em etape
 //std::cout<<"APPEL A RECUR\n";
 //recursivSelectMpr(updatelist);
 }
 }
 */
/*void Node::recursivSelectMpr(std::list<std::list<IPv6> > liste){
 if(liste.size()<1)
 return;
 IPv6* mpr; // IP du MPR élu
 std::list<IPv6> mprNeighbor; // voisin+1 du MPR
 std::list<std::list<IPv6> > updatelist;
 updatelist=liste;
 bool ajoutMpr=false;
 for(std::list<IPv6>::iterator voisin=mNeighborIP.begin();voisin!=mNeighborIP.end();++voisin){
 int cptTwoHop =0;
 IPv6 *ipToComp=new IPv6();
 ipToComp->setIPv6(voisin);
 ajoutMpr=false;
 for(std::list<std::list<IPv6> >::iterator twoHop=updatelist.begin();twoHop!=updatelist.end();++twoHop){
 int tmpTwoHop=0;
 std::list<IPv6> tmp;
 bool test=false;
 for(std::list<IPv6>::iterator it=twoHop->begin();it != twoHop->end();++it){
 if(test){
 std::list<IPv6>::iterator voisinNeighbor = --it;
 if(it->isEgal(ipToComp)){
 tmpTwoHop++;
 tmp.push_back(*voisinNeighbor);
 }
 }else{
 test=true;
 }
 }
 if(tmpTwoHop>cptTwoHop){
 ajoutMpr=true;
 cptTwoHop=tmpTwoHop;
 mpr=ipToComp;
 mprNeighbor=tmp;
 }
 }
 }
 if(ajoutMpr){
 mMyMprList.push_back(*mpr);// on ajoute le MPR élu a notre liste
 //2em etape
 //retirer lles voisin +1 du voisin choisi au dessus et le voisin
 for(std::list<std::list<IPv6> >::iterator update=updatelist.begin(); update!=updatelist.end();++update){
 for(std::list<IPv6>::iterator it2=update->begin();it2 != update->end();++it2){
 if(it2->isEgal(mpr))
 updatelist.erase(update);
 }
 }
 // 3em etape
 recursivSelectMpr(updatelist);
 }
 }
 */
int Node::addNeighborTable(Route *route) {
// maté le tableau dès qu'on tombe sur un NULL on ajoute
// erreur si Plein
// erreur si metric !=1
	if (route->getMetric() != 1) {
		std::cout << "ERROR : Add Neighbor with Metric > 1" << std::endl;
		return 2;
	} else {
		for (std::list<Route>::iterator it = mNeighborTable.begin();
				it != mNeighborTable.end(); ++it) {
			if (it->getIpDest()->isEgal(route->getIpDest())) {
				std::cout << "ERROR : Neighbor Already Exist" << std::endl;
				return 3;
			}
		}
		std::cout << "AJOUT Voisin :  \n" << route->getIpDest()->toChar()
				<< std::endl;
		mNeighborTable.push_back(*route);
	}
	return 1;
}

int Node::addNeighbor(Route* route) {
	if (addNeighborTable(route)) {
		mNeighborIP.push_back(*route->getIpDest());
		//selectMpr();
		std::cout << "MAJ MPR\n";
		return 0;
	}
	std::cout << "Erreur ajout route voisin\n";
	return 1;

}
int Node::addNeighbor(IPv6* ipDest, IPv6* ipSource, int metric, IPv6* nextHop,
		int action) {
	Route *route = new Route(ipDest, ipSource, metric, nextHop, action);
	if (addNeighborTable(route)) {
		mNeighborIP.push_back(*ipDest);
		// selectMpr();
		std::cout << "MAJ MPR\n";
		return 0;
	}
	std::cout << "Erreur ajout route voisin\n";
	return 1;
}

int Node::addTwoHopNeighborTable(Route *route) {
// MAté le tableau TwoHOP Null on ajoute la route
// erreur si plein
// erruer si metric !=2
	if (route->getMetric() != 2) {
		std::cout << "ERROR : Add Two Hop Neighbor with Metric != 2"
				<< std::endl;
		return 2;
	} else {
		for (std::list<Route>::iterator it = mTwoHopNeighborTable.begin();
				it != mTwoHopNeighborTable.end(); ++it) {
			if (it->getIpDest()->isEgal(route->getIpDest())) {
				std::cout << "ERROR : Two Hop Neighbor Already Exist"
						<< std::endl;
				return 3;
			}
		}
		for (std::list<Route>::iterator it = mNeighborTable.begin();
				it != mNeighborTable.end(); ++it) {
			if (it->getIpDest()->isEgal(route->getIpDest())) {
				std::cout << "ERROR : Dest is a Neighbor" << std::endl;
				return 4;
			}
		}
		std::cout << "AJOUT Voisin +1 :  \n" << route->getIpDest()->toChar()
				<< std::endl;
		mTwoHopNeighborTable.push_back(*route);
	}
	return 1;
}

int Node::addTwoHopNeighbor(Route* route) {
	std::list<IPv6> TwoHop;
	if (addTwoHopNeighborTable(route)) {
		TwoHop.push_back(*route->getIpDest());
		TwoHop.push_back(*route->getNextHop());
		mTwoHopNeighborIP.push_back(TwoHop);
		//selectMpr();
		std::cout << "MAJ MPR\n";
		return 0;
	}
	std::cout << " Erreur d'ajout de route two hop" << std::endl;
	return 1;
}
int Node::addTwoHopNeighbor(IPv6* ipDest, IPv6* ipSource, int metric,
		IPv6* nextHop, int action) {
	Route *route = new Route(ipDest, ipSource, metric, nextHop, action);
	std::list<IPv6> TwoHop;
	if (addTwoHopNeighborTable(route)) {
		TwoHop.push_back(*ipDest);
		TwoHop.push_back(*nextHop);
		mTwoHopNeighborIP.push_back(TwoHop);
		// selectMpr();
		std::cout << "MAJ MPR\n";
		return 0;
	}
	std::cout << " Erreur d'ajout de route" << std::endl;
	return 1;
}

int Node::delNeighbor(IPv6* ipToDelete) {
// maté route.sIpDest du tableau
// Remonter tout les élém en dessous
// ajouter NULL a la derniere ligne

	/*
	 * On place action de la route sur mNeighborTable a DEL
	 */
	for (std::list<Route>::iterator it = mNeighborTable.begin();
			it != mNeighborTable.end(); ++it) {
		if (it->getIpDest()->isEgal(ipToDelete)) {
			std::cout << "Suppression OK : Neighbor" << std::endl;
			it->setAction(DEL);

		}
	}

	/*
	 * Supprime le neighbor de la liste des HopNgihborIP
	 */
	for (std::list<IPv6>::iterator it = mNeighborIP.begin();
			it != mNeighborIP.end(); ++it) {
		if (it->isEgal(ipToDelete)) {
			mNeighborIP.erase(it);
			std::cout << "Suppression de NeighborIP" << std::endl;
			//selectMpr();
			std::cout << "MAJ MPR\n";
			return 0;
		}
	}
	std::cout << "ERROR : Neighbor not Found \n";
	return 1;
}
int Node::delNeighbor(Route* route) {
	IPv6* ip = route->getIpDest();
	return delNeighbor(ip);
}

int Node::delTwoHopNeighbor(IPv6* ipToDelete) {
// maté route.sIpDest du tableau
// Remonter tout les élém en dessous
// ajouter NULL a la derniere ligne
	/*
	 * Place l'action de la route sur mTwoHopNeighborTable a DEL
	 */
	for (std::list<Route>::iterator it = mTwoHopNeighborTable.begin();
			it != mTwoHopNeighborTable.end(); ++it) {
		if (it->getIpDest()->isEgal(ipToDelete)) {
			std::cout << "Suppression OK : Neighbor" << std::endl;
			it->setAction(DEL);
		}
	}
	/*
	 * Supprime le 2 hop neighbor de la liste des TwoHopNgihborIP
	 */
	for (std::list<std::list<IPv6> >::iterator it2 = mTwoHopNeighborIP.begin();
			it2 != mTwoHopNeighborIP.end(); ++it2) {
		std::list<IPv6> temp = *it2;
		std::list<IPv6>::iterator tempIt = temp.begin();
		if (tempIt->isEgal(ipToDelete)) {
			mTwoHopNeighborIP.erase(it2);
			//  selectMpr();
			std::cout << "Suppression de 2HopNeighborIP" << std::endl;
			return 0;
		}
	}

	std::cout << "ERROR : Two Hop Neighbor not Found \n";
	return 1;
}
int Node::delTwoHopNeighbor(Route* route) {
	IPv6* ip = route->getIpDest();
	return delTwoHopNeighbor(ip);
}

std::string Node::getMacAdress(char * cmd) {
	FILE* pipe = popen(cmd, "r");
	if (!pipe)
		return "ERROR";
	char buffer[128];
	std::string result = "";
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return result;
}

std::string Node::hex2Bin(const std::string& s) {
	std::stringstream ss;
	ss << std::hex << s;
	unsigned n;
	ss >> n;
	std::bitset<32> b(n);

	unsigned x = 0;
	if (boost::starts_with(s, "0x") || boost::starts_with(s, "0X"))
		x = 2;
	return b.to_string().substr(32 - 4 * (s.length() - x));
}

std::string Node::bin2Hex(const std::string& s) {
	std::bitset<32> bs(s);
	unsigned n = bs.to_ulong();
	std::stringstream ss;
	ss << std::hex << n;
	return "0x" + boost::to_upper_copy(ss.str());
}

std::string Node::macToIpv6() {
	char * commande = "ifconfig | grep wlan | awk '$0 ~ /HWaddr/ { print $5 }'";
	// todo : renvoyer erreur si chaine vide
	std::string result = getMacAdress(commande);
	std::string IPv6;

	if (result.empty()) {
		std::cout << "MAC adress not found\n";
		//todo ; retourner erreur
	}

	// reformat to IPv6 notation
	IPv6 = result.substr(0, 2) + result.substr(3, 5) + "ff:fe"
			+ result.substr(9, 5) + result.substr(15, 2);

	// convertion of first octet
	std::string hexaStr = IPv6.substr(0, 2);
	std::string binaryStr = hex2Bin(hexaStr);

	if (binaryStr[6] == '0') {
		binaryStr[6] = '1';
	} else {
		binaryStr[6] = '0';
	}

	hexaStr = bin2Hex(binaryStr);


	// insert of first octet modified
	IPv6.replace(0, 2, hexaStr.substr(2, 4));

	// On n'insère pas le préfixe ici car il sera insérer par le constructeur de la classe IPv6

	return IPv6;
}

