#include "IPv4.h"

IPv4::IPv4() {
	for (int i = 0; i < 4; ++i) {
		mScope[i] = 0x0;
	}
}	

IPv4::IPv4(short scope1, short scope2, short scope3) {
	mScope[0] = 0x10;
	mScope[1] = scope1;
	mScope[2] = scope2;
	mScope[3] = scope3;
}

IPv4::IPv4(std::string IPv4Str) {
	std::stringstream str0;
	str0 << IPv4Str.substr(0, 2);
	int array0;
	str0 >> array0;

	std::stringstream str1;
	str1 << IPv4Str.substr(2, 2);
	int array1;
	str1 >> array1;

	std::stringstream str2;
	str2 << IPv4Str.substr(4, 2);
	int array2;
	str2 >> array2;

	std::stringstream str3;
	str3 << IPv4Str.substr(6, 2);
	int array3;
	str3 >> array3;

	mScope[0] = (short)array0;
	mScope[1] = (short)array1;
	mScope[2] = (short)array2;
	mScope[3] = (short)array3;
}

short IPv4::getScope(int scope) {
	return mScope[scope];
}

void IPv4::setIPv4(IPv4* ip) {
	for (int i = 0; i < 4; i++) {
		mScope[i] = ip->getScope(i);
	}
}

std::string IPv4::toChar() {
	std::stringstream ip;
	for (int i = 0; i < 4; i++) {
		ip << mScope[i];
		ip << ".";
	}
	return ip.str();
}

bool IPv4::isEgal(IPv4* ip) {
	for (int i = 0; i < 4; i++) {
		if (mScope[i] != ip->mScope[i])
			return false;
	}
	return true;
}

IPv4::~IPv4() {
	// TODO Auto-generated destructor stub
}