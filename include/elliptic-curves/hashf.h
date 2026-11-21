#include <miracl/big.h>
#include <miracl/ecn.h>

#include <string>

using namespace std;

struct hFBig
{
	size_t operator()(const ECn& item) const
	{
		unsigned int prime = 2049982463;
		char number[500];
		int length, n = 0,  i;
		epoint *P;
		big x;

		P = item.get_point();
		x = mirvar(0);
		x = P->X;
		length = otstr(x,number);

		for (i = 0; i < length; i++)
			n = n*8 + number[i];
		return n > 0 ? (n % prime) : (-n % prime);
	}
};
