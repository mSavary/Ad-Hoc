/**
 *Class IPv6
 *
 *
 *
 */

#ifndef IPV6_H_
#define IPV6_H_
#include <sstream>
#include <list>
#include <iostream>
#include "const.h"

class IPv6 {
private:
	short mScope[8];

public:
	IPv6();
	IPv6(short scope5, short scope6, short scope7, short scope8);
	IPv6(std::string IPv6Str);
	short getScope(int scope);
	void setIPv6(IPv6* ip);
	std::string toChar();
	bool isEgal(IPv6* ip);
	virtual ~IPv6();
};

#endif /* IPV6_H_ */
