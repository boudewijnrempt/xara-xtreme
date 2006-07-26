/* @@tag:xara-cn@@ DO NOT MODIFY THIS LINE
================================XARAHEADERSTART===========================

               SVGFilter, XAR <--> SVG plugin filter for XaraLX
                    Copyright (C) 2006 Xara Group Ltd.
       Copyright on certain contributions may be held in joint with their
              respective authors. See AUTHORS file for details.

LICENSE TO USE AND MODIFY SOFTWARE
----------------------------------

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Xara Group Ltd. nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

MARKS
-----

Xara, Xara LX, Xara X, Xara X/Xtreme, Xara Xtreme, the Xtreme and Xara
designs are registered or unregistered trademarks, design-marks, and/or
service marks of Xara Group Ltd. All rights in these marks are reserved.

      Xara Group Ltd, Gaddesden Place, Hemel Hempstead, HP2 6EX, UK.
                        http://www.xara.com/

=================================XARAHEADEREND============================
*/
// utils.cpp: This implements various common utility functions

#include "utils.h"

// trim whitespace from string
wxString TrimWs(const wxString& s)
{
	wxString ret = s;
	return ret.Trim(true).Trim(false);
}

// take (and removes) the first number from the string and return it
double TakeNumber(wxString& data)
{
	wxString s;
	data = TrimWs(data);
	if (data.IsEmpty())
		return 0.0f; // some error
	if (IsNumberChar(data[0])) {
		s += data[0];
		data = data.Mid(1);
		while (data.Length() > 0 && IsNumberDigit(data[0])) {
			s += data[0];
			data = data.Mid(1);
		}
	}
	double f;
	s.ToDouble(&f);
	return f;
}

// take the property named "name"
wxString GetStringProperty(xmlNodePtr cur, const char *name)
{
	wxString s;
	xmlChar *value = xmlGetProp(cur, (const xmlChar *)name);
	if (value != NULL) {
		s = wxString::FromAscii((const char *)value);
		xmlFree(value);
	}
	return s;
}

// take the property named "name" and convert to double
double GetDoubleProperty(xmlNodePtr cur, const char *name)
{
	double x = 0.0f;
	xmlChar *value = xmlGetProp(cur, (const xmlChar *)name);
	if (value != NULL) {
		x = atof((const char *)value);
		xmlFree(value);
	}
	return x;
}

// is the measure without unit?
bool IsUnitless(const wxString& measure)
{
	wxString s = measure;
	// remove whitespace and convert to lowercase
	s = TrimWs(s).MakeLower();
	return (s.IsNumber() || s.Right(2) == _T("px"));
}

// is the string a measure?
bool IsMeasure(const wxString& measure)
{
	wxString s = measure;
	// remove whitespace and convert to lowercase and take last 2 chars
	s = TrimWs(s).MakeLower().Right(2);
	return (s == _T("cm") || s == _T("mm") || s == _T("in") || s == _T("pc"));
}

// convert measure to millipoints
double MeasureToMillipoints(const wxString& measure)
{
	wxString s = measure;
	// remove whitespace and convert to lowercase
	s = TrimWs(s).MakeLower();

	double f;
	if (s.Right(2) == _T("cm")) {
		s = s.RemoveLast(2).Trim();
		s.ToDouble(&f);
		f = CM2PT(f)*1000.0f; // Convert to millipoints
	} else if (s.Right(2) == _T("mm")) {
		s = s.RemoveLast(2).Trim();
		s.ToDouble(&f);
		f = MM2PT(f)*1000.0f; // Convert to millipoints
	} else if (s.Right(2) == _T("in")) {
		s = s.RemoveLast(2).Trim();
		s.ToDouble(&f);
		f = IN2PT(f)*1000.0f; // Convert to millipoints
	} else if (s.Right(2) == _T("pc")) {
		s = s.RemoveLast(2).Trim();
		s.ToDouble(&f);
		f = PC2PT(f)*1000.0f; // Convert to millipoints
	} else if (s.Right(2) == _T("px")) {
		s = s.RemoveLast(2).Trim();
		s.ToDouble(&f);
		f = PX2PT(f)*1000.0f; // Convert to millipoints
	} else { // anything else
		s.ToDouble(&f);
		f *= 1000.0f;
	}

	return f;
}

double MeasureToUserUnits(const wxString& measure)
{
	wxString s = measure;
	// remove whitespace and convert to lowercase
	s = TrimWs(s).MakeLower();

	if (s.Right(2) == _T("px")) {
		// strip units
		s = s.RemoveLast(2).Trim();
	}
	double f;
	s.ToDouble(&f);
	f = PX2PT(f); // Convert to user units

	return f;
}