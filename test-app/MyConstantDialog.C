#include <string>
#include "MyButton.H"           // our own button class derived from Fl_OpButton
#include "MyConstantDialog.H"

//////////////////////
// test-app/MyConstantDialog.C
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

// Callback to handle when user hits Enter.
//     Save current input value to button, and hide the dialog window.
//
void MyConstantDialog::InputCallback2() {
    butt->SetConstantValue(input->value());
    hide();
}

// CONSTRUCTOR
MyConstantDialog::MyConstantDialog(MyButton *butt) : Fl_Double_Window(250,50) {
    // Save parent button for later use
    this->butt = butt;

    // Set title of window based on button's full name
    std::string title = butt->GetFullName();
    copy_label(title.c_str());

    // Create input widget, assign it the button's current input value.
    begin();
        input = new Fl_Input(60,10,180,25,"Value:");    // create input widget
        input->callback(InputCallback, (void*)this);    // callback for input widget
        input->when(FL_WHEN_ENTER_KEY_ALWAYS);          // do callback when user hits enter
    end();

    // Make this dialog window modal (above all others)
    set_modal();
    show();
}

// Show the dialog, update constant value with button's current value
void MyConstantDialog::show() {
    std::string const_value = butt->GetConstantValue();
    input->value(const_value.c_str());
    input->position(0,999);                                     // select all text
    position(Fl::event_x_root()-10, Fl::event_y_root()-20);     // position window near mouse
    Fl_Double_Window::show();
}

