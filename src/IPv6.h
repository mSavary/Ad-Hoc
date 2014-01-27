/**
*Class IPv6
*
*
*
*/

#ifndef IPV6_H_
#define IPV6_H_

class IPv6
{
private:
	int mScope[8];

public:
	IPv6();
	IPv6(int scope1,
			int scope2,
			int scope3,
			int scope4,
			int scope5,
			int scope6,
			int scope7,
			int scope8);
	int getScope(int scope);
	virtual ~IPv6();
};

#endif /* IPV6_H_ */
