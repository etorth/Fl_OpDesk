#ifndef _EVERYTHING_H
#define _EVERYTHING_H

//////////////////////
// test-app/everything.H
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

// OS
#include <string>               // STL string class
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef MICROSOFT
// Grr...
#define snprintf _snprintf
#endif /*MICROSOFT*/

// FLTK
#include <FL/Fl.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/fl_ask.H>          // fl_choice()
#include <FL/Fl_Input.H>

// Fl_OpDesk WIDGETS
#include <Fl_OpDesk.H>
#include <Fl_OpBox.H>
#include <Fl_OpButton.H>

// MISC
#include "Subs.H"

#endif

