/*
 * main.cpp
 *
 *  Created on: 18 mars 2014
 *      Author: maxime
 */


#include "Controller.h"


int main(){
	std::ostringstream syscall;
	syscall << "./BootScript.pl";
		if (!system((syscall.str()).c_str())){
			std::cout<<" Starting Ad-Hoc Networks ...\n";
		}
	Controller *ctrl = new Controller();

	ctrl->run();
	return 0;
}
