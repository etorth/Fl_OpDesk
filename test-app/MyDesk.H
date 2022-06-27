#ifndef _MY_DESK_H
#define _MY_DESK_H

//////////////////////
// test-app/MyDesk.H
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

#include <FL/fl_ask.H>
#include "everything.H"
#include "MyButton.H"
#include "FSM_Node.H"

// CUSTOM Fl_OpDesk
//    Our app derives from the Fl_OpDesk widget to augment it with our app-specific code.
//
class MyDesk : public Fl_OpDesk {
    std::vector<FSM_Node*> box_clipboard;  // copy/paste clipboard for FSM_Node's
    int next_boxid;                     // next box id#

    // Fl_OpDesk calls this virtual when connection errors occur.
    //    Let's post a dialog instead of the default stderr behavior.
    //
    void ConnectionError(Fl_OpButton *src, Fl_OpButton *dst, std::string &errmsg);

public:
    MyDesk(int X,int Y,int W,int H,const char *L=0):Fl_OpDesk(X,Y,W,H,L) {
        next_boxid = 0;
    }

    // CLEAR THE DESK
    //    Resets our box id counter to zero
    //
    void Clear() {
        Fl_OpDesk::Clear();
        next_boxid = 0;
    }

    int GetNextBoxId() {
        return(++next_boxid);
    }

    int SaveLayout(FILE *fp);
    int LoadLayout(FILE *fp, int &line, std::string &errmsg);
    int CopySelected();
    int PasteSelected();
};

#endif /*_MY_DESK_H*/
