#include "biginteger_calculator/SpellNumberDialog.h"

#include <wx/event.h>

/*
   Copyright (c) 1988, 1993, 1994
	The Regents of the University of California.  All rights reserved.
*/

#include "biginteger_calculator/bsd.h"

#define	MAXNUM		65		/* Biggest number we handle. */
#define	LINELEN		256

char *SpellNumberDialog::name1[] =
{
	(char *) "", (char *) "one", (char *) "two", (char *) "three",
	(char *) "four",	(char *) "five",	(char *) "six", (char *) "seven",
	(char *) "eight",	(char *) "nine",	(char *) "ten", (char *) "eleven",
	(char *) "twelve", (char *) "thirteen",	(char *) "fourteen",	(char *) "fifteen",
	(char *) "sixteen",	(char *) "seventeen", (char *) "eighteen",	(char *) "nineteen"
};

char *SpellNumberDialog::name2[] =
{
	(char *) "", (char *) "ten", (char *) "twenty",	(char *) "thirty",
	(char *) "forty",	(char *) "fifty",	(char *) "sixty",	(char *) "seventy",
	(char *) "eighty", (char *) "ninety"
};

char *SpellNumberDialog::name3[] =
{
	(char *) "hundred",	(char *) "thousand",	(char *) "million",	(char *) "billion",
	(char *) "trillion",	(char *) "quadrillion",	(char *) "quintillion",	(char *) "sextillion",
	(char *) "septillion",	(char *) "octillion", (char *) "nonillion", (char *) "decillion",
	(char *) "undecillion",	(char *) "duodecillion", (char *) "tredecillion", (char *) "quattuordecillion",
	(char *) "quindecillion",	(char *) "sexdecillion",
	(char *) "septendecillion", (char *) "octodecillion",
	(char *) "novemdecillion",	(char *) "vigintillion"
};

SpellNumberDialog::SpellNumberDialog (wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog (parent, id, title, pos, size, style)
{
	this->SetSizeHints (wxDefaultSize, wxDefaultSize);

	wxFlexGridSizer* spellNumberDialogFlexGridSizer;
	spellNumberDialogFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	spellNumberDialogFlexGridSizer->AddGrowableCol (0);
	spellNumberDialogFlexGridSizer->AddGrowableRow (1);
	spellNumberDialogFlexGridSizer->SetFlexibleDirection (wxBOTH);
	spellNumberDialogFlexGridSizer->SetNonFlexibleGrowMode (wxFLEX_GROWMODE_SPECIFIED);

	ID_SPELL_NUMBER_TEXTCTRL = new wxTextCtrl (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize (400,450), wxTE_MULTILINE|wxHSCROLL);
	spellNumberDialogFlexGridSizer->Add (ID_SPELL_NUMBER_TEXTCTRL, 1, wxALL|wxEXPAND, 1);

	ID_SPELL_NUMBER_CLOSE_BUTTON = new wxButton (this, wxID_ANY, wxT ("Close"), wxDefaultPosition, wxDefaultSize, 0);
	ID_SPELL_NUMBER_CLOSE_BUTTON->SetDefault();
	spellNumberDialogFlexGridSizer->Add (ID_SPELL_NUMBER_CLOSE_BUTTON, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

	this->SetSizer (spellNumberDialogFlexGridSizer);
	this->Layout();

	this->Centre (1);

	// Connect Events
	ID_SPELL_NUMBER_CLOSE_BUTTON->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (SpellNumberDialog::OnSpellNumberCloseButtonClick), NULL, this);
}

SpellNumberDialog::~SpellNumberDialog()
{
	// Disconnect Events
	ID_SPELL_NUMBER_CLOSE_BUTTON->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (SpellNumberDialog::OnSpellNumberCloseButtonClick), NULL, this);
}

void SpellNumberDialog::writeSpelledValue (char *numberToBeSpelled)
{
	spellNumber (numberToBeSpelled);
	ID_SPELL_NUMBER_TEXTCTRL->SetEditable (false);
}

/*	$OpenBSD: number.c,v 1.13 2005/11/05 22:55:37 jmc Exp $	*/

/*
 * Copyright (c) 1988, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

void SpellNumberDialog::spellNumber (char *numberToSpell)
{
	lflag = 0;
	convert (numberToSpell);
}

void SpellNumberDialog::convert (char *numberToSpell)
{
	int flen = 0, len, rval;
	char *p, *fraction;

	/* strip trailing and leading whitespace */
	len = strlen (numberToSpell) - 1;

	while ( (isblank (numberToSpell[len])) || (numberToSpell[len] == '\n'))
		numberToSpell[len--] = '\0';

	while ( (isblank (numberToSpell[0])) || (numberToSpell[0] == '\n'))
		numberToSpell++;

	if (strchr (numberToSpell, 'e') || strchr (numberToSpell, 'E'))
		convertexp (numberToSpell);
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
badnum:	/* cannot happen */
				break;
			}
		}

		*p = '\0';

		if ( (len = strlen (numberToSpell)) > MAXNUM ||
		      ( (fraction != NULL) && (flen = strlen (fraction)) > MAXNUM))
		{
			/* cannot happen */
			//sprintf(errormsg, "number too long (max %d digits).", MAXNUM);
			//throw spellNumberException(errormsg);
		}

		if (*numberToSpell == '-')
		{
			printfToTextCtrl ( (char *) "minus%s", lflag ? " " : "\n");
			++numberToSpell;
			--len;
		}

		if (*numberToSpell == '+')
		{
			printfToTextCtrl ( (char *) "plus%s", lflag ? " " : "\n");
			++numberToSpell;
			--len;
		}

		rval = len > 0 ? unit (len, numberToSpell) : 0;

		if (fraction != NULL && flen != 0)
			for (p = fraction; *p != '\0'; ++p)
				if (*p != '0')
				{
					if (rval)
						printfToTextCtrl ( (char *) "%sand%s", lflag ? (char *) " " : (char *) "",
						                   lflag ? (char *) " " : (char *) "\n");

					if (unit (flen, fraction))
					{
						if (lflag)
							printfToTextCtrl ( (char *) " ");

						pfract (flen);
						rval = 1;
					}

					break;
				}

		if (!rval)
			printfToTextCtrl ( (char *) "zero%s", lflag ? (char *) "" : (char *) ".\n");
	}
}

void SpellNumberDialog::convertexp (char *numberToSpell)
{
	char locline[LINELEN];
	char *part1, *part2, *part3, *part4;
	char tmp[2];
	int  i, j;

	strlcpy (locline,numberToSpell,LINELEN);
	part3 = locline;
	part2 = strsep (&part3, "eE");	/* part3 is the exponent */
	part4 = part3;
	strsep (&part4, ".");	/* no decimal allowed in the exponent */

	if (part4)
	{
		/* cannot happen */
		//sprintf(errormsg, "illegal number: %s", numberToSpell);
		//throw spellNumberException(errormsg);
	}

	part1 = strsep (&part2, ".");	/* we can have one in the mantissa */
	/* At this point everything should be null or a digit.  Check for
	 * that before starting to convert.  Two characters may be + or -.
	 */
	j = strlen (numberToSpell);

	for (i = 0; i < j; i++)
		if ( (!isdigit (locline[i])) && (locline[i]))
			if ( ( (locline[i] != '+') && (locline[i] != '-')) ||
			      ( (i != 0) && (i != part3 - locline)))
			{
				/* cannot happen */
				//sprintf(errormsg, "illegal number: %s", numberToSpell);
				//throw spellNumberException(errormsg);
			}

	convert (part1);
	printfToTextCtrl ( (char *) "%s", lflag ? (char *) " " : (char *) "");

	if (part2 && part2[0])  	/* do individual digits separately */
	{
		printfToTextCtrl ( (char *) "point%s", lflag ? (char *) " " : (char *) "\n");
		j = strlen (part2);
		tmp[1] = '\0';

		for (i = 0 ; i < j; i++)
		{
			tmp[0] = part2[i];
			convert (tmp);
			printfToTextCtrl ( (char *) "%s", lflag ? (char *) " " : (char *) "");
		}
	}

	printfToTextCtrl ( (char *) "times ten to the%s", lflag ? (char *) " " : (char *) "\n");

	if (part3 && part3[0])
		convert (part3);
	else
		printfToTextCtrl ( (char *) "zero%s", lflag ? (char *) " " : (char *) ".\n");
}

int SpellNumberDialog::unit (int len, const char *p)
{
	int off, rval;

	rval = 0;

	if (len > 3)
	{
		if (len % 3)
		{
			off = len % 3;
			len -= off;

			if (number (p, off))
			{
				rval = 1;
				printfToTextCtrl ( (char *) " %s%s", name3[len / 3], lflag ? (char *) " " : (char *) ".\n");
			}

			p += off;
		}

		for (; len > 3; p += 3)
		{
			len -= 3;

			if (number (p, 3))
			{
				rval = 1;
				printfToTextCtrl ( (char *) " %s%s", name3[len / 3], lflag ? (char *) " " : (char *) ".\n");
			}
		}
	}

	if (number (p, len))
	{
		if (!lflag)
			printfToTextCtrl ( (char *) ".\n");

		rval = 1;
	}

	return (rval);
}

int SpellNumberDialog::number (const char *p, int len)
{
	int val, rval;

	rval = 0;

	switch (len)
	{
	case 3:
		if (*p != '0')
		{
			rval = 1;
			printfToTextCtrl ( (char *) "%s hundred", name1[*p - '0']);
		}

		++p;

	/* FALLTHROUGH */
	case 2:
		val = (p[1] - '0') + (p[0] - '0') * 10;

		if (val)
		{
			if (rval)
				printfToTextCtrl ( (char *) " ");

			if (val < 20)
				printfToTextCtrl ( (char *) "%s", name1[val]);
			else
			{
				printfToTextCtrl ( (char *) "%s", name2[val / 10]);

				if (val % 10)
					printfToTextCtrl ( (char *) "-%s", name1[val % 10]);
			}

			rval = 1;
		}

		break;

	case 1:
		if (*p != '0')
		{
			rval = 1;
			printfToTextCtrl ( (char *) "%s", name1[*p - '0']);
		}
	}

	return (rval);
}

void SpellNumberDialog::pfract (int len)
{
	static const char *const pref[] = { "", "ten-", "hundred-" };

	switch (len)
	{
	case 1:
		printfToTextCtrl ( (char *) "tenths%s", lflag ? (char *) "" : (char *) ".\n");
		break;

	case 2:
		printfToTextCtrl ( (char *) "hundredths%s", lflag ? (char *) "" : (char *) ".\n");
		break;

	default:
		printfToTextCtrl ( (char *) "%s%sths%s", pref[len % 3], name3[len / 3],
		                   lflag ? (char *) "" : (char *) ".\n");
		break;
	}
}

void SpellNumberDialog::printfToTextCtrl (char *formatStr, ...)
{
	va_list data;
	char *p = formatStr, *value[3], output[50];
	int numArgs = 0;
	wxString text;

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

	text = wxString (output, wxConvUTF8);

	ID_SPELL_NUMBER_TEXTCTRL->AppendText (text);
}
