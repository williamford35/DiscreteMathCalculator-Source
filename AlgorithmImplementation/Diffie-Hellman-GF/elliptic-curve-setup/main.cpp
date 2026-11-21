#include "miracl/big.h"
#include "miracl/ecn.h"
#include "miracl/p1363.h"
#include "ellipticCurveFunctions.h"
#include "schoof.h"
#include <NTL/ZZ.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#include <unistd.h>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#define PATH_MAX 256

bool exe_name (char *str);

NTL_CLIENT

int main(int argc, char *argv[])
{
	additionalCurveParameters addv;
	p_and_seed v;
	Big p, seed, A, B, order, basePointOrder, cofactor, xcoor, ycoor;
	Big a, b;
	ECn basePoint, aP, bP, abP;
	long numBits;
	char numBitsStr[15];
	char pstr[512], seedStr[512], Astr[512], Bstr[512], Xcoor[512], Ycoor[512];
	char orderStr[512], basePointOrderStr[512], cofactorStr[512];
	char astr[512], bstr[512], aPstrX[512], aPstrY[512], bPstrX[512], bPstrY[512];
	char abPstrX[512], abPstrY[512];
	ofstream ellipticCurveParams;

#ifdef _WIN64
	string exeDirectory;
	wchar_t path[2048];
	string exePath;

	GetModuleFileName (NULL, path, 2048);
	exePath = string (path);

	exeDirectory = exePath.substr (0, exePath.rfind ("\\") ) );

	if (!SetCurrentDirectory (exeDirectory.c_str() ) )
		exit (1);
#elif __APPLE__
#include "CFBundle.h"

	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyBundleURL (mainBundle);
	CFStringRef str = CFURLCopyFileSystemPath (resourcesURL, kCFURLPOSIXPathStyle);
	CFRelease (resourcesURL);
	char path[PATH_MAX];

	CFStringGetCString (str, path, FILENAME_MAX, kCFStringEncodingASCII);
	CFRelease (str);

	cout << "path = " << path << endl;
	if (chdir (path) != 0)
	{
		cout << "Failed to change directory to " << path << endl;
		return 1;
	}
#else
	char path[PATH_MAX+1], *pp;

	if (!exe_name (path) )
		return 1;

	if ( (pp = strrchr (path, '/') ) )
		* (pp+1) = '\0';

	if (chdir (path) != 0)
		return 1;
#endif

		strcpy(numBitsStr, argv[1]);
		numBits = atoi(numBitsStr);

		v = generate_p_and_seed(numBits);

		p = v.p;
		pstr << p;
		// cout << "p = " << p << endl;

		seed = v.seed;
		seedStr << seed;
		// cout << "seed = " << seed << endl;

		addv = CreateTheCurve(p, seed);

		A = addv.A;
		Astr << A;
		// cout << "A = " << A << endl;

		B = addv.B;
		Bstr << B;
		// cout << "B = " << B << endl;

		order = addv.order;
		orderStr << order;
		// cout << "order = " << order << endl;

		basePoint = addv.basePoint;
		basePoint.get(xcoor,ycoor);

		Xcoor << xcoor;
		// cout << "P.x = " << xcoor << endl;

		Ycoor << ycoor;
		// cout << "P.y = " << ycoor << endl;
		// cout << ECn(xcoor,ycoor) << endl;

		basePointOrder = addv.basepointOrder;
		basePointOrderStr << basePointOrder;
		// cout << "Basepoint order = " << basePointOrder << endl;

		cofactor = addv.cofactor;
		cofactorStr << cofactor;
		// cout << "cofactor = " << cofactor << endl;

		bool tstCurve = verify_curve (p, seed, A, B);
		if (tstCurve == false)
	{
		// cout << "The curve did not verify." << endl;
		return 1;
	}

a = generateBig(numBits);
// cout << "a = " << a << endl;
b = generateBig(numBits);
// cout << "b = " << b << endl;
aP = a*basePoint;
// cout << "a*P = " << aP << endl;
bP = b*basePoint;
// cout << "b*P = " << bP << endl;
abP = a*b*basePoint;
// cout << "a*b*P = " << abP << endl;

#ifdef __APPLE__
if (argc < 3)
{
cerr << "Missing output directory." << endl;
return 1;
}
string gfparamsPath = string(argv[2]) + "/gfparams.txt";
ellipticCurveParams.open(gfparamsPath.c_str());
#else
ellipticCurveParams.open("./gfparams.txt");
#endif
if(!ellipticCurveParams.is_open())
	{
		// cout << "Failed to create output file." << endl;
		return 1;
	}

	ellipticCurveParams << pstr << ',';
	ellipticCurveParams << seedStr << ',';
	ellipticCurveParams << Astr << ',';
	ellipticCurveParams << Bstr << ',';
	ellipticCurveParams << orderStr << ',';
	ellipticCurveParams << Xcoor << ',';
	ellipticCurveParams << Ycoor << ',';
	ellipticCurveParams << basePointOrderStr << ',';
	ellipticCurveParams << cofactorStr << ',';
	astr << a;
	ellipticCurveParams << astr << ',';
	bstr << b;
	ellipticCurveParams << bstr << ',';
	aP.get(xcoor,ycoor);
	aPstrX << xcoor;
	ellipticCurveParams << aPstrX << ',';
	aPstrY << ycoor;
	ellipticCurveParams << aPstrY << ',';
	bP.get(xcoor,ycoor);
	bPstrX << xcoor;
	ellipticCurveParams << bPstrX << ',';
	bPstrY << ycoor;
	ellipticCurveParams << bPstrY << ',';
	abP.get(xcoor,ycoor);
	abPstrX << xcoor;
	ellipticCurveParams << abPstrX << ',';
	abPstrY << ycoor;
	ellipticCurveParams << abPstrY << endl;
	ellipticCurveParams.close();

	// cout << "The curve verifies." << endl;
	return 0;
}

bool exe_name (char *str)
{
	FILE *fp;
	char buf[4096], *p;

	*str = '\0';

	if (! (fp = fopen ("/proc/self/maps", "r") ) )
		return false;

	fgets (buf, sizeof (buf), fp);
	fclose (fp);

	* (p = strchr (buf, '\n') ) = '\0';

	while (*p != ' ')
		p--;

	strncpy (str, p+1, PATH_MAX);
	str[PATH_MAX] = '\0';

	return true;
}

