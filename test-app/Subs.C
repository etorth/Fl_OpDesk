#include <stdio.h>
#include <string.h>

//////////////////////
// test-app/Subs.C
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

// Skip over leading white space; return pointer to first non-white char
char *skipwhite(char *s) {
    while ( *s == ' ' || *s == 9 ) { ++s; }
    return(s);
}

// Truncate string 's' at any matching character in 'tchars'
void truncate(char *s, const char *tchars) {
    while (*s) {
        if ( strchr(tchars, *s) ) {     // char in set?
            *s = 0;                     // truncate
            break;
        }
        ++s;
    }
}

// Convert integer to an ascii string
//     Opposite of atoi(3)
//
const char *itoa(int i) {
    static char s[80];
    sprintf(s, "%d", i);
    return(s);
}
