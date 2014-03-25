#include "IPv6.h"

IPv6::IPv6() {
	for (int i = 0; i < 8; ++i) {
		mScope[i] = 0x0;
	}
}

IPv6::IPv6(short scope5, short scope6, short scope7, short scope8) {
	mScope[0] = EN_TETE_IPv6;
	mScope[1] = 0x0;
	mScope[2] = 0x0;
	mScope[3] = 0x0;
	mScope[4] = scope5;
	mScope[5] = scope6;
	mScope[6] = scope7;
	mScope[7] = scope8;
}

IPv6::IPv6(std::string IPv6Str) {
	int i = 0;
	std::string temp;

	for (std::string::iterator it = IPv6Str.begin(); it != IPv6Str.end();
			it++) {
		char carac = *it;
		if (i != 7) {
			if (carac != ':') {
				temp.push_back(carac);
			} else {
				std::stringstream scope;
				int array;
				scope << temp;
				scope >> std::hex >> array;
				mScope[i] = (short) array;
				temp = "";
				if (i == 0) { // for the first scope
					mScope[1] = 0x0;
					mScope[2] = 0x0;
					mScope[3] = 0x0;
					it++;
					i = 3;
				}
				i++;
			}
		} else {
			temp.push_back(carac);
			std::stringstream scope;
			int array;
			scope << temp;
			scope >> std::hex >> array;
			mScope[i] = (short) array;
		}
	}
	mScope[0]=EN_TETE_IPv6;
}

short IPv6::getScope(int scope) {
	return mScope[scope];
}

void IPv6::setIPv6(IPv6* ip) {
	for (int i = 0; i < 8; i++) {
		mScope[i] = ip->getScope(i);
	}
}

std::string IPv6::toChar() {
	std::stringstream ip;
	for (int i = 0; i < 8; i++) {
		if (i == 0 || i > 3) {
			ip << std::hex << mScope[i];
			if (i == 0)
				ip << "::";
		}
		if (i > 3 && i < 7)
			ip << ":";
	}
	return ip.str();
}

bool IPv6::isEgal(IPv6* ip) {
	for (int i = 0; i < 8; i++) {
		if (mScope[i] != ip->mScope[i])
			return false;
	}
	return true;
}

IPv6::~IPv6() {
	// TODO Auto-generated destructor stub
}

