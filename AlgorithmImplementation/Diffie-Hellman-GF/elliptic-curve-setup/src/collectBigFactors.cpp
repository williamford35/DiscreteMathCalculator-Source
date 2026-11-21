#include "miracl/big.h"
#include "collectBigFactors.h"
#include "schoofException.h"

Miracl precision=5000;

#include "zlib.h"

#include <vector>
using namespace std;

vector<Factor> collectBigFactors(const Big& n)
{
	vector<string> factor;
	Factor f;
	Big nn = n, product;
	vector<Factor> ret;
	int i, k;
	char buf[300];
	miracl *mip = get_mip();
	int base = mip->IOBASE;

	if (base == 16)
	{
		// ecfactor assumes the integer is in decimal format
		// convert n from base 16 to base 10
		mip->IOBASE = 10;
		buf << nn;
		nn = Big(buf);
	}

	// ecfactor (nn, factor);

	i = 0;

	while (i < factor.size())
	{
		k = 1;
		product = Big((char *)factor[i].c_str());

		if (i != factor.size()-1)
			while (factor[i] == factor[i+1])
			{
				product *= Big((char *)factor[i].c_str());
				k++;
				i++;
			}

		f.k = k;
		f.number = Big((char *)factor[i].c_str());
		f.product = product;
		i++;
		ret.push_back(f);
	}

	mip->IOBASE = base;

	return ret;
}
