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
	std::stringstream str0;
	str0 << IPv6Str.substr(0, 4);
	int array0;
	str0 >> array0;


	std::stringstream str4;
	str4 << IPv6Str.substr(6, 4);
	int array4;
	str4 >> std::hex >> array4;

	std::stringstream str5;
	str5 << IPv6Str.substr(11, 4);
	int array5;
	str5 >> std::hex >> array5;

	std::stringstream str6;
	str6 << IPv6Str.substr(16, 4);
	int array6;
	str6 >> std::hex >> array6;

	std::stringstream str7;
	str7 << IPv6Str.substr(21, 4);
	int array7;
	str7 >> std::hex >> array7;

	mScope[0] = (short)array0;
	mScope[1] = 0x0;
	mScope[2] = 0x0;
	mScope[3] = 0x0;
	mScope[4] = (short)array4;
	mScope[5] = (short)array5;
	mScope[6] = (short)array6;
	mScope[7] = (short)array7;
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

