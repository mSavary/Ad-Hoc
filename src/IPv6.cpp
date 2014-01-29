#include "IPv6.h"

IPv6::IPv6() {
	for (int i = 0; i < 8; ++i){
		mScope[i] = 0x0;
	}
}

IPv6::IPv6(int scope1,
			int scope2,
			int scope3,
			int scope4,
			int scope5,
			int scope6,
			int scope7,
			int scope8) {
	mScope[0] = scope1;
	mScope[1] = scope2;
	mScope[2] = scope3;
	mScope[3] = scope4;
	mScope[4] = scope5;
	mScope[5] = scope6;
	mScope[6] = scope7;
	mScope[7] = scope8;
}

int IPv6::getScope(int scope){
	return mScope[scope];
}

void IPv6::setIPv6(IPv6 ip){
	for (int i=0; i<8; i++){
		mScope[i]=ip.getScope(i);
	}
}

std::string IPv6::toChar(){
	std::stringstream ip;
	for (int i=0;i<8;i++){
		ip<<std::hex<<mScope[i];
		if(i<7)
			ip<<":";
	}
	return ip.str();
}

IPv6::~IPv6() {
	// TODO Auto-generated destructor stub
}

