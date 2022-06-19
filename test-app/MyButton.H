#ifndef _MY_BUTTON_H
#define _MY_BUTTON_H

//////////////////////
// test-app/MyButton.H
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

// APP'S INCLUDE FILES
#include "MyButton.H"
#include "MyConstantDialog.H"

// CUSTOM FL_OPBUTTON
//    Our app derives from the Fl_OpButton widget to augment it with our app-specific code.
//
class MyButton : public Fl_OpButton {
public:
    enum DataType { INT=1, FLOAT=2, STRING=4, ANY=7 };
private:
    std::string const_val;
    DataType dtype;
    MyConstantDialog *const_dialog;

    // Virtual: Called by Fl_OpDesk when a connection is being made.
    int Connecting(Fl_OpButton *but, std::string &errmsg);

    // Virtual: Called by Fl_OpDesk when a connection is being dropped.
    void Disconnecting(Fl_OpButton *but) {
        if ( getenv("MYAPP_DEBUG") ) {
            fprintf(stderr, "**DISCONNECTING*** '%s' <-> '%s'\n",
                this->GetFullName().c_str(),
                 but->GetFullName().c_str());
        }
    }

    void MyButtonCallback2();
    static void MyButtonCallback(Fl_Widget *w, void *userdata);

public:
    // CTORS
    MyButton(const char *buttname, Fl_OpButtonType io, DataType dtype);
    MyButton(const MyButton &o);

    // DATA TYPES/CONSTANTS
    void         SetConstantValue(const char *val);
    std::string  GetConstantValue() const;
    DataType     GetDataType() const;
    void         SetDataType(DataType dt);
    std::string  GetDataTypeStr() const;
    int          SetDataTypeStr(const char *s);
    std::string  GetVarValue() const;
    int          IsDataType(DataType val) const;

    // LAYOUT LOAD/SAVE
    int SaveLayout(FILE *fp);
    int LoadLayout(FILE *fp, int &line, std::string &errmsg);

    // MISC
    void DoButtonMenu();

    // FLTK
    int handle(int e);
    void draw();

};

#endif
