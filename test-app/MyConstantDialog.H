#ifndef _MY_CONSTANT_DIALOG_H
#define _MY_CONSTANT_DIALOG_H

//////////////////////
// test-app/MyConstantDialog.H
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

#include "everything.H"

class MyButton;                         // forward declare MyButton

//
// MyConstantDialog -- a popup dialog for prompting the user for a constant value
//
class MyConstantDialog : public Fl_Double_Window {
    MyButton *butt;                     // parent button
    Fl_Input *input;                    // input widget

    // Callback to handle user input
    void InputCallback2();
    static void InputCallback(Fl_Widget*, void *userdata) {
        MyConstantDialog *o = (MyConstantDialog*)userdata;
        o->InputCallback2();
    }

public:
    MyConstantDialog(MyButton *butt);   // CTOR
    void show();                        // show the window, get current value from parent
};

#endif
