#ifndef _MY_BOX_INFO_DIALOG_H
#define _MY_BOX_INFO_DIALOG_H

//////////////////////
// test-app/MyBoxInfoDialog.H
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
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Pack.H>
#include "MyButton.H"
#include <vector>

// BUTTON INFO CLASS
//    This manages presenting a single button's bookkeeping info
//    to the user in a GUI format.
//
class ButtonInfo : public Fl_Group {
    MyButton *but;
    std::string name;
    Fl_Input *input;

    // Handle user changing input field
    void InputChanged() {
        but->SetConstantValue(input->value());
        redraw();
    }
    // CALLBACK INVOKED WHEN USER CHANGES 'input'
    static void InputChanged_CB(Fl_Widget *w, void *data) {
        ((ButtonInfo*)data)->InputChanged();
    }
public:
    // Ctor
    ButtonInfo(int X,int Y,int W,int H, MyButton *but);
    // Set the name of the button
    void SetName(const char *val) {
        name = val;
        input->label(name.c_str());
    } 
    // Set the constant value for the button
    void SetConstantValue(const char *val) {
        input->value(val);
    }
    // Set the button's type
    void SetButtonType(Fl_OpButtonType type) {
        if ( type == FL_OP_OUTPUT_BUTTON ) {
           input->deactivate();
        } else {
           input->activate();
        }
    }
};

class MyBoxInfoDialog : public Fl_Window {
    std::vector<ButtonInfo*> binfos;
    Fl_Scroll *scroll;
    Fl_Pack   *pack;
public:
    MyBoxInfoDialog() : Fl_Window(300,200) {
        // Create scroller
        scroll = new Fl_Scroll(10,10,w()-20,h()-20);
        scroll->begin();
        {
            // Create a pack to handle the array of ButtonInfo's
            pack = new Fl_Pack(scroll->x(), scroll->y(), scroll->w(), scroll->h());
            pack->box(FL_NO_BOX);
            pack->color(46);
            pack->type(Fl_Pack::VERTICAL);
            pack->spacing(3);
        }
        scroll->end();
        resizable(this);
    }
    void AddButton(MyButton *but) {
        pack->begin();
        {
            int X = x();
            int Y = y();        // Fl_Pack will override this
            int W = scroll->w() - scroll->scrollbar.w();
            int H = 20;
            ButtonInfo *binfo = new ButtonInfo(X,Y,W,H,but);
            binfos.push_back(binfo);
        }
        pack->end();
    }
    void Clear() {
        pack->clear();          // dump all the button info instances
        binfos.clear();
    }
    void Show(const char *title) {
        copy_label(title);
        show();
    }
};

#endif
