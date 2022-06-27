#include "MyDesk.H"

//////////////////////
// test-app/MyDesk.C
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

void MyDesk::ConnectionError(Fl_OpButton *src, Fl_OpButton *dst, std::string &errmsg) {
    std::string msg;
    msg = std::string("    INPUT: ") + src->GetFullName() + std::string("\n") +
          std::string(   "OUTPUT: ") + dst->GetFullName() + std::string("\n") +
          errmsg;
    fl_alert("%s", msg.c_str());              // post a dialog
}

// Save the entire desk layout (all boxes/buttons/connections)
int MyDesk::SaveLayout(FILE *fp) {
    fprintf(fp, "next_boxid %d\n", next_boxid);
    // Save connection linestyle
    fprintf(fp, "linestyle %d\n", int(GetConnectStyle()));
    // Tell all the boxes to 'save'
    for ( int t=0; t<GetOpBoxTotal(); t++ ) {
        FSM_Node *mybox = (FSM_Node*)GetOpBox(t);
        mybox->SaveLayout(fp);
    }
    fprintf(fp, "# %d CONNECTIONS\n", (int)GetConnectionsTotal());
    for ( int t=0; t<GetConnectionsTotal(); t++ ) {
        std::string src = GetConnection(t)->GetSrcButton()->GetFullName();
        std::string dst = GetConnection(t)->GetDstButton()->GetFullName();
        Fl_OpConnect *conn = GetConnection(t);
        fprintf(fp, "connect 0x%08lx %s\t%s\n", (unsigned long)conn->GetColor(),
                                                src.c_str(),
                                                dst.c_str());
    }
    return(0);
}

// Load an entire desk layout (all boxes/buttons/connections)
//     Returns 0 if loaded OK, -1 on error, errmsg has reason.
//
int MyDesk::LoadLayout(FILE *fp, int &line, std::string &errmsg) {
    char s[1024], arg[1024], from[512], to[512];
    int new_next_boxid = 0;
    while ( fgets(s, sizeof(s)-1, fp) != 0 ) {
        line++;
        char *ss = skipwhite(s);
        if ( *ss == '\n' || *ss == '\r' || *ss == 0 || *ss == '#' ) {   // skip blank lines + comments
            continue;
        }
        // Next boxid?
        if ( sscanf(ss, "next_boxid %d", &new_next_boxid) == 1 ) {
            continue;
        }
        // Box?
        if ( sscanf(ss, "box \"%1023[^\"]\"", arg) == 1 ) {
            begin();
                FSM_Node *box = new FSM_Node(0,0,150,80,arg);
                if ( box->LoadLayout(fp, line, errmsg) < 0 ) {
                    return(-1);
                }
            end();
            continue;
        }
        // Connect?
        unsigned long lcol;
        if ( sscanf(ss, "connect 0x%lx %512s %512s", &lcol, from, to) == 3 ) {
            if ( Connect(from, to, errmsg) < 0 ) {
                errmsg += std::string("\n") + std::string(s);
                return(-1);
            }
            // Set the connection 'color'
            Fl_OpConnect *conn = GetLastOpConnect();
            conn->SetColor(Fl_Color(lcol));
            continue;
        }
        // linestyle?
        unsigned int ls;
        if ( sscanf(ss, "linestyle %d", &ls) == 1 ) {
            SetConnectStyle(Fl_OpConnectStyle(ls));
            continue;
        }
        // Got this far? Unknown command
        errmsg = std::string("unknown desk command") +
                 std::string("\n") + std::string(s);
        return(-1);
    }
    next_boxid = new_next_boxid;
    return(0);
}

/// Copy the selected boxes to the paste buffer.
/// Returns how many boxes were copied.
///
int MyDesk::CopySelected() {
    box_clipboard.clear();
    int count = 0;
    for ( int t=0; t<GetOpBoxTotal(); t++ ) { 
        FSM_Node *b = (FSM_Node*)GetOpBox(t);
        if ( b->GetSelected() ) {
            box_clipboard.push_back(b);
            ++count;
        }
    }
    return(count);
}

/// Handle 'paste' operations.
/// Pastes the boxes from the clipboard.
/// Returns how many new boxes were pasted.
///
int MyDesk::PasteSelected() {
    int count = 0;
    std::vector<Fl_OpBox*> origboxes;
    std::vector<Fl_OpBox*> copyboxes;

    // Deselect all boxes first.
    //    This way if we do two pastes in a row (with no intermediate copy)
    //    only the last paste remains selected.
    //
    DeselectAll();

    // Copy the boxes
    //    Keep track of original vs. copy, so when we copy
    //    the connections, we know which to make for which.
    //
    for ( size_t t=0; t<box_clipboard.size(); t++ ) {
        FSM_Node *orig = box_clipboard[t];
        begin();
            FSM_Node *copy = new FSM_Node(*orig);     // create new instance of FSM_Node using copy ctor
            copy->position(copy->x()+100, copy->y()+100);
            copy->CopyButtons(*orig);           // copy the buttons
            copy->SetSelected(1);               // leave copy selected
            origboxes.push_back((Fl_OpBox*)orig);
            copyboxes.push_back((Fl_OpBox*)copy);
        end();
        ++count;
    }

    // Copy the connections
    CopyConnections(origboxes, copyboxes);

    // Update fltk
    redraw();
    return(count);
}
