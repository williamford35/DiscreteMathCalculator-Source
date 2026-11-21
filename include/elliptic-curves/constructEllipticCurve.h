#ifndef CONSTRUCTELLIPTICCURVE_H
#define CONSTRUCTELLIPTICCURVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void constructCurve (char *eq, double a, double b)
{
	char buf[50], acoef[20], bcoef[20];
	bool aIsNeg = false, bIsNeg = false;

	if (a < 0)
		aIsNeg = true;
	if (b < 0)
		bIsNeg = true;

	// format the A coefficient
	if (a != 0.0)
		if (a == 1.0)
			strcpy(acoef, "1");
		else
			sprintf(acoef, "%g", a);
	else
		strcpy(acoef," ");

	// format the B coefficient
	if (b != 0.0)
		if (b == 1.0)
			strcpy(bcoef, "1");
		else
			sprintf(bcoef, "%g", b);
	else
		strcpy(acoef," ");

	sprintf(eq, "y^2 = x^3 ");
	if (aIsNeg)
	{
		a = -a;
		sprintf(buf, "- %gx ", a);
		strcat(eq, buf);
	}
	else
	{
		sprintf(buf, "+ %gx ", a);
		strcat(eq, buf);
	}
	if (bIsNeg)
	{
		b = -b;
		sprintf(buf, "- %g", b);
		strcat(eq, buf);
	}
	else
	{
		sprintf(buf, "+ %g ", b);
		strcat(eq, buf);
	}
}

#endif // CONSTRUCTELLIPTICCURVE_H
