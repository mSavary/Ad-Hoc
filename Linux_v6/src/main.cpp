/*
 * main.cpp
 *
 *  Created on: 18 mars 2014
 *      Author: maxime
 */

#include "Controller.h"

int main() {

	Controller *ctrl = new Controller();
	std::ostringstream syscall, syscall2;
	syscall << "./BootScript.pl " + ctrl->getNode()->getInterface();
	if (!system((syscall.str()).c_str())) {
		std::cout << " Starting Ad-Hoc Networks ...\n";
	}

	syscall2
			<< "ip -6 route add 2014::/64 dev "
					+ ctrl->getNode()->getInterface() + "\nip -6 addr add "
					+ ctrl->getNode()->getMyIp()->toChar() + " dev "
					+ ctrl->getNode()->getInterface();
	if (!system((syscall2.str()).c_str())) {
		std::cout << " Starting Ad-Hoc Networks ...\n";
	}
	ctrl->run();
	return 0;
}
