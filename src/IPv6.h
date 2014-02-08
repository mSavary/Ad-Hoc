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
#include <boost/lexical_cast.hpp>
#define EN_TETE_IPv6 0x2011

class IPv6
{
private:
	int mScope[8];

public:
	IPv6();
	IPv6(int scope5,
			int scope6,
			int scope7,
			int scope8);
	IPv6(std::string IPv6Str);
	int getScope(int scope);
	void setIPv6(IPv6* ip);
	std::string toChar();
	bool isEgal(IPv6* ip);
	virtual ~IPv6();
};

#endif /* IPV6_H_ */
