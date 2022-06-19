#ifndef _SUBS_H
#define _SUBS_H

//////////////////////
// test-app/Subs.H
//////////////////////
//
// OpDesk (Version 0.82)
// This file is part of the OpDesk node graph FLTK widget.
// Copyright (C) 2009,2011 by Greg Ercolano.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING.txt" which should have been included with this file.
// If this file is missing or damaged, see the FLTK library license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems to:
//
//     erco (at) seriss.com
//

char *skipwhite(char *s);                       // skip over leading white space
void truncate(char *s, const char *tchars);     // truncate string at specified char(s)
const char *itoa(int i);                        // convert an int to an ascii string

#endif
