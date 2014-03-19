/**
*Class IPv4
*
*
*
*/

#ifndef IPV4_H_
#define IPV4_H_
#include <sstream>
#include <list>
#include "const.h"

class IPv4
{
private:
	short mScope[4];

public:
	IPv4();
	IPv4(short scope1,
			short scope2,
			short scope3);
	IPv4(std::string IPv4Str);
	short getScope(int scope);
	void setIPv4(IPv4* ip);
	std::string toChar();
	bool isEgal(IPv4* ip);
	virtual ~IPv4();
};

#endif /* IPV4_H_ */
