/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <sys/types.h>

#include <stdio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
#include <wx/dcgraph.h>

#include "biginteger_calculator/bsd.h"
#include "biginteger_calculator/spellNumberException.h"
#include "biginteger_calculator/spellNumber.h"

#define	MAXNUM		65		/* Biggest number we handle. */
#define	LINELEN		256

#ifndef WX_PRECOMP
//(*HeadersPCH(SpellNumberDialog)
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#endif
//(*Headers(SpellNumberDialog)
//*)

char *name1[] =
{
	(char *) "", (char *) "one", (char *) "two", (char *) "three",
	(char *) "four",	(char *) "five",	(char *) "six", (char *) "seven",
	(char *) "eight",	(char *) "nine",	(char *) "ten", (char *) "eleven",
	(char *) "twelve", (char *) "thirteen",	(char *) "fourteen",	(char *) "fifteen",
	(char *) "sixteen",	(char *) "seventeen", (char *) "eighteen",	(char *) "nineteen"
},

*name2[] =
{
	(char *) "", (char *) "ten", (char *) "twenty", (char *) "thirty",
	(char *) "forty",	(char *) "fifty",	(char *) "sixty",	(char *) "seventy",
	(char *) "eighty", (char *) "ninety"
},

*name3[] =
{
	(char *) "hundred",	(char *) "thousand",	(char *) "million",	(char *) "billion",
	(char *) "trillion",	(char *) "quadrillion",	(char *) "quintillion",	(char *) "sextillion",
	(char *) "septillion",	(char *) "octillion", (char *) "nonillion", (char *) "decillion",
	(char *) "undecillion",	(char *) "duodecillion", (char *) "tredecillion", (char *) "quattuordecillion",
	(char *) "quindecillion",	(char *) "sexdecillion",
	(char *) "septendecillion", (char *) "octodecillion",
	(char *) "novemdecillion",	(char *) "vigintillion"
};

void	convert (wxTextCtrl*, char *);
void	convertexp (wxTextCtrl *, char *);
int	number (wxTextCtrl*, const char *, int);
void	pfract (wxTextCtrl*, int);
void	toobig (void);
int	unit (wxTextCtrl*, int, const char *);
void	usage (void);
void  printfToTextCtrl (wxTextCtrl* spellNumberTextCtrl, char *formatStr, ...);

int lflag;
char errormsg[50];

void spellNumber (char *numberToSpell, wxTextCtrl* spellNumberTextCtrl)
{
	lflag = 0;
	convert (spellNumberTextCtrl, numberToSpell);
}

void convert (wxTextCtrl* spellNumberTextCtrl, char *numberToSpell)
{
	int flen, len, rval;
	char *p, *fraction;

	/* strip trailing and leading whitespace */
	len = strlen (numberToSpell) - 1;

	while ( (isblank (numberToSpell[len])) || (numberToSpell[len] == '\n'))
		numberToSpell[len--] = '\0';

	while ( (isblank (numberToSpell[0])) || (numberToSpell[0] == '\n'))
		numberToSpell++;

	if (strchr (numberToSpell, 'e') || strchr (numberToSpell, 'E'))
		convertexp (spellNumberTextCtrl, numberToSpell);
	else
	{
		fraction = NULL;

		for (p = numberToSpell; *p != '\0' && *p != '\n'; ++p)
		{
			if (isblank (*p))
				goto badnum;

			if (isdigit (*p))
				continue;

			switch (*p)
			{
			case '.':
				if (fraction != NULL)
					goto badnum;

				fraction = p + 1;
				*p = '\0';
				break;

			case '-':
			case '+':
				if (p == numberToSpell)
					break;

			/* FALLTHROUGH */
			default:
badnum:
				sprintf (errormsg, "illegal number: %s", numberToSpell);
				throw spellNumberException (errormsg);
				break;
			}
		}

		*p = '\0';

		if ( (len = strlen (numberToSpell)) > MAXNUM ||
		      ( (fraction != NULL) && (flen = strlen (fraction)) > MAXNUM))
		{
			sprintf (errormsg, "number too long (max %d digits).", MAXNUM);
			throw spellNumberException (errormsg);
		}

		if (*numberToSpell == '-')
		{
			printfToTextCtrl (spellNumberTextCtrl, (char *) "minus%s", lflag ? (char *) " " : (char *) "\n");
			++numberToSpell;
			--len;
		}

		if (*numberToSpell == '+')
		{
			printfToTextCtrl (spellNumberTextCtrl, (char *) "plus%s", lflag ? (char *) " " : (char *) "\n");
			++numberToSpell;
			--len;
		}

		rval = len > 0 ? unit (spellNumberTextCtrl, len, numberToSpell) : 0;

		if (fraction != NULL && flen != 0)
			for (p = fraction; *p != '\0'; ++p)
				if (*p != '0')
				{
					if (rval)
						printfToTextCtrl (spellNumberTextCtrl, (char *) "%sand%s",
						                  lflag ? (char *) " " : (char *) "",
						                  lflag ? (char *) " " : (char *) "\n");

					if (unit (spellNumberTextCtrl, flen, fraction))
					{
						if (lflag)
							printfToTextCtrl (spellNumberTextCtrl, (char *) " ");

						pfract (spellNumberTextCtrl, flen);
						rval = 1;
					}

					break;
				}

		if (!rval)
			printfToTextCtrl (spellNumberTextCtrl, (char *) "zero%s", lflag ? (char *) "" : (char *) ".\n");
	}
}

void convertexp (wxTextCtrl* spellNumberTextCtrl, char *numberToSpell)
{
	char locline[LINELEN];
	char *part1, *part2, *part3, *part4;
	char tmp[2];
	int  i, j;

	strlcpy (locline,numberToSpell,LINELEN);
	part3 = locline;
	part2 = strsep (&part3, (char *) "eE");	/* part3 is the exponent */
	part4 = part3;
	strsep (&part4, (char *) ".");	/* no decimal allowed in the exponent */

	if (part4)
	{
		sprintf (errormsg, (char *) "illegal number: %s", numberToSpell);
		throw spellNumberException (errormsg);
	}

	part1 = strsep (&part2, (char *) ".");	/* we can have one in the mantissa */
	/* At this point everything should be null or a digit.  Check for
	 * that before starting to convert.  Two characters may be + or -.
	 */
	j = strlen (numberToSpell);

	for (i = 0; i < j; i++)
		if ( (!isdigit (locline[i])) && (locline[i]))
			if ( ( (locline[i] != '+') && (locline[i] != '-')) ||
			      ( (i != 0) && (i != part3 - locline)))
			{
				sprintf (errormsg, (char *) "illegal number: %s", numberToSpell);
				throw spellNumberException (errormsg);
			}

	convert (spellNumberTextCtrl, part1);
	printfToTextCtrl (spellNumberTextCtrl, (char *) "%s", lflag ? (char *) " " : (char *) "");

	if (part2 && part2[0])  	/* do individual digits separately */
	{
		printfToTextCtrl (spellNumberTextCtrl, (char *) "point%s", lflag ? (char *) " " : (char *) "\n");
		j = strlen (part2);
		tmp[1] = '\0';

		for (i = 0 ; i < j; i++)
		{
			tmp[0] = part2[i];
			convert (spellNumberTextCtrl, tmp);
			printfToTextCtrl (spellNumberTextCtrl, (char *) "%s", lflag ? (char *) " " : (char *) "");
		}
	}

	printfToTextCtrl (spellNumberTextCtrl, (char *) "times ten to the%s", lflag ? (char *) " " : (char *) "\n");

	if (part3 && part3[0])
		convert (spellNumberTextCtrl, part3);
	else
		printfToTextCtrl (spellNumberTextCtrl, (char *) "zero%s", lflag ? (char *) " " : (char *) ".\n");
}

int unit (wxTextCtrl* spellNumberTextCtrl, int len, const char *p)
{
	int off, rval;

	rval = 0;

	if (len > 3)
	{
		if (len % 3)
		{
			off = len % 3;
			len -= off;

			if (number (spellNumberTextCtrl, p, off))
			{
				rval = 1;
				printfToTextCtrl (spellNumberTextCtrl, (char *) " %s%s",
				                  name3[len / 3], lflag ? (char *) " " : (char *) ".\n");
			}

			p += off;
		}

		for (; len > 3; p += 3)
		{
			len -= 3;

			if (number (spellNumberTextCtrl, p, 3))
			{
				rval = 1;
				printfToTextCtrl (spellNumberTextCtrl, (char *) " %s%s",
				                  name3[len / 3], lflag ? (char *) " " : (char *) ".\n");
			}
		}
	}

	if (number (spellNumberTextCtrl, p, len))
	{
		if (!lflag)
			printfToTextCtrl (spellNumberTextCtrl, (char *) ".\n");

		rval = 1;
	}

	return (rval);
}

int number (wxTextCtrl* spellNumberTextCtrl, const char *p, int len)
{
	int val, rval;

	rval = 0;

	switch (len)
	{
	case 3:
		if (*p != '0')
		{
			rval = 1;
			printfToTextCtrl (spellNumberTextCtrl, (char *) "%s hundred", name1[*p - '0']);
		}

		++p;

	/* FALLTHROUGH */
	case 2:
		val = (p[1] - '0') + (p[0] - '0') * 10;

		if (val)
		{
			if (rval)
				printfToTextCtrl (spellNumberTextCtrl, (char *) " ");

			if (val < 20)
				printfToTextCtrl (spellNumberTextCtrl, (char *) "%s", name1[val]);
			else
			{
				printfToTextCtrl (spellNumberTextCtrl, (char *) "%s", name2[val / 10]);

				if (val % 10)
					printfToTextCtrl (spellNumberTextCtrl, (char *) "-%s", name1[val % 10]);
			}

			rval = 1;
		}

		break;

	case 1:
		if (*p != '0')
		{
			rval = 1;
			printfToTextCtrl (spellNumberTextCtrl, (char *) "%s", name1[*p - '0']);
		}
	}

	return (rval);
}

void pfract (wxTextCtrl* spellNumberTextCtrl, int len)
{
	static const char *const pref[] = { (char *) "", (char *) "ten-", (char *) "hundred-" };

	switch (len)
	{
	case 1:
		printfToTextCtrl (spellNumberTextCtrl, (char *) "tenths%s", lflag ? (char *) "" : (char *) ".\n");
		break;

	case 2:
		printfToTextCtrl (spellNumberTextCtrl, (char *) "hundredths%s", lflag ? (char *) "" : (char *) ".\n");
		break;

	default:
		printfToTextCtrl (spellNumberTextCtrl, (char *) "%s%sths%s", pref[len % 3], name3[len / 3],
		                  lflag ? (char *) "" : (char *) ".\n");
		break;
	}
}

void usage (void)
{
	sprintf (errormsg, (char *) "usage: number [-l] [--] [# ...]\n");
	throw spellNumberException (errormsg);
}

void printfToTextCtrl (wxTextCtrl* spellNumberTextCtrl, char *formatStr, ...)
{
	va_list data;
	char *p = formatStr, *value[3], output[50];
	int numArgs = 0;

	va_start (data,formatStr);

	while (*p)
	{
		if (*p=='s')
		{
			value[numArgs] = va_arg (data,char *);
			numArgs++;
		}

		p++;
	}

	va_end (data);

	switch (numArgs)
	{
	case 0:
		sprintf (output, formatStr);
		break;

	case 1:
		sprintf (output, formatStr, value[0]);
		break;

	case 2:
		sprintf (output, formatStr, value[0], value[1]);
		break;

	case 3:
		sprintf (output, formatStr, value[0], value[1], value[2]);
		break;
	}

	wxString text (output,wxConvUTF8);
	spellNumberTextCtrl->AppendText (text);
}
