#include <FL/Fl.H>
#include "MyButton.H"

//////////////////////
// test-app/MyButton.C
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

// CTOR
MyButton::MyButton(const char *buttname, Fl_OpButtonType io, DataType dt) : Fl_OpButton(buttname, io) {
    // OPTIONAL: This sets a callback that is invoked whenever
    //           the button is pushed.
    //
    const_val    = "";
    dtype        = dt;
    const_dialog = 0;
    callback(MyButtonCallback, (void*)this);
}

// COPY CTOR
MyButton::MyButton(const MyButton &o) : Fl_OpButton(o.label(), o.GetButtonType()) {
    const_val    = o.const_val;
    dtype        = o.dtype;
    const_dialog = 0;
    callback(MyButtonCallback, (void*)this);
}

// Check if OK to connect to another button.
//    Returns 0 if OK, -1 on error with errmsg set to reason.
//
int MyButton::Connecting(Fl_OpButton *opbut, std::string &errmsg) {
    MyButton *but = (MyButton*)opbut;
    if ( ! this->IsDataType(but->GetDataType()) ) {
        errmsg = std::string("Can't connect ");
        errmsg += this->GetDataTypeStr();
        errmsg += std::string(" and ");
        errmsg += but->GetDataTypeStr();
        return(-1);
    }
    return(0);
}

// SET BUTTON TO A CONSTANT VALUE
//    When this is set, GetVarValue() will return this.
//    Handles redraw.
//
void MyButton::SetConstantValue(const char *val) {
    const_val = val;
    GetOpDesk()->redraw();      // redraw desk (const value not on button, but on desk next to it)
}

// RETURN CONSTANT VALUE ("" IF NONE)
std::string MyButton::GetConstantValue() const {
    return(const_val);
}

// RETURN DATA TYPE FOR THIS BUTTON
MyButton::DataType MyButton::GetDataType() const {
    return(dtype);
}

// SET DATA TYPE FOR THIS BUTTON
void MyButton::SetDataType(MyButton::DataType dt) {
    dtype = dt;
}

// RETURN DATA TYPE FOR THIS BUTTON AS A "STRING"
std::string MyButton::GetDataTypeStr() const {
    std::string val;
    val = "";
    if ( dtype & INT    ) { val += (val==""?"":"|"); val += "int"; }
    if ( dtype & FLOAT  ) { val += (val==""?"":"|"); val += "float"; }
    if ( dtype & STRING ) { val += (val==""?"":"|"); val += "string"; }
    if ( val == "" ) val = "???";
    return(val);
}

// SET DATA TYPE FOR THIS BUTTON GIVEN A "STRING"
//     Returns 0 if OK, -1 if unknown data type.
//
int MyButton::SetDataTypeStr(const char *s) {
    dtype = DataType(0);
    if ( strstr(s, "int")    ) dtype = DataType(((int)dtype) | (int)INT);
    if ( strstr(s, "float")  ) dtype = DataType(((int)dtype) | (int)FLOAT);
    if ( strstr(s, "string") ) dtype = DataType(((int)dtype) | (int)STRING);
    return ( dtype == 0 ) ? -1 : 0;
}

// SEE IF DATA TYPE OF THIS BUTTON MATCHES SPECIFIED DATATYPE(S)
//     Returns 1 if there's a match, 0 if no match.
//
int MyButton::IsDataType(DataType val) const {
    return( ((int)dtype & (int)val ) ? 1 : 0);
}

// Return variable name for this button
//    Box's name is included for context.
//
std::string MyButton::GetVarValue() const {
    switch ( GetButtonType() ) {
        case FL_OP_INPUT_BUTTON: {
            // Are we connected to an output button?
            //     If so, return the variable name of the output button we're connected to.
            //
            if ( GetTotalConnectedButtons() ) {
                const MyButton* otherbut = (const MyButton*)GetConnectedButton();       // inputs only have one connection
                // Create "${<BOXNAME>_<BUTTNAME>}"
                std::string val = std::string("${") +                           // "${"
                                  std::string(otherbut->GetOpBox()->label()) +  // "${add_12"
                                  std::string("_") +                            // "${add_12_"
                                  std::string(otherbut->label()) +              // "${add_12_a"
                                  std::string("}");                             // "${add_12_a}"
                return(val);
            }
            // Is there a constant for this button? If so, return that value
            if ( const_val != "" ) {
                return(const_val);
            }
            // Input button with no constant defined and not connected?
            //    This should probably cause an error, or return some kind of default.
            //
            return("NO_VALUE");
        }
        case FL_OP_OUTPUT_BUTTON: {
            // Create "${<BOXNAME>_<BUTTNAME>}"
            std::string val = std::string("${") +                       // "${"
                              std::string(GetOpBox()->label()) +        // "${add_12"
                              std::string("_") +                        // "${add_12_"
                              std::string(label()) +                    // "${add_12_a"
                              std::string("}");                         // "${add_12_a}"
            return(val);
        }
    }
    return("NO_VALUE");
}

// How a button saves itself to a file
int MyButton::SaveLayout(FILE *fp) {
    switch ( GetButtonType() ) {
        case FL_OP_INPUT_BUTTON:
            fprintf(fp, "    in \"%s\"\n", label());
            break;
        case FL_OP_OUTPUT_BUTTON:
            fprintf(fp, "    out \"%s\"\n", label());
            break;
    }
    if ( GetConstantValue() != "" ) {
        std::string dt;
        fprintf(fp, "    {\n");
        fprintf(fp, "        dtype %s\n", GetDataTypeStr().c_str());
        fprintf(fp, "        const %s\n", GetConstantValue().c_str());
        fprintf(fp, "    }\n");
    } else {
        fprintf(fp, "    {\n");
        fprintf(fp, "        dtype %s\n", GetDataTypeStr().c_str());
        fprintf(fp, "    }\n");
    }
    return(0);
}

// How a button loads itself from a file
int MyButton::LoadLayout(FILE *fp, int &line, std::string &errmsg) {
    char s[1024], arg[1024];
    int braces = 0;
    while ( fgets(s, sizeof(s)-1, fp) != 0 ) {
        line++;
        char *ss = skipwhite(s);
        if ( *ss == '\n' || *ss == '\r' || *ss == 0 || *ss == '#' ) {   // skip blank lines + comments
            continue;
        }
        if ( *ss == '{' ) {                                     // count open braces
            ++braces;
            continue;
        }
        if ( *ss == '}' ) {                             // count close braces
            if ( --braces <= 0 ) { break; }
            else { continue; }
        }
        if ( sscanf(ss, "dtype %79s", arg) == 1 ) {
            if ( SetDataTypeStr(arg) < 0 ) {
                errmsg = std::string("unknown data type '") +
                         std::string(arg) +
                         std::string("'") +
                         std::string("\n") + std::string(s);
                return(-1);
            }
            continue;
        }
        if ( sscanf(ss, "const %1023[^\n]\n", arg) == 1 ) {
            SetConstantValue(arg);
            continue;
        }
        // Got this far? Unknown command
        errmsg = std::string("unknown button command") +
                 std::string("\n") + std::string(s);
        return(-1);
    }
    return(0);
}

// POST THE POPUP MENU FOR THIS BUTTON
void MyButton::DoButtonMenu() {
   std::string name = GetFullName();
   Fl_Menu_Item rclick_menu[] = {
       { name.c_str(),     0, 0, 0, FL_MENU_INACTIVE|FL_MENU_DIVIDER },
       { "Help",           0, 0, 0 },
       { "Disconnect All", 0, 0, 0 },
       { 0 }
   };
   const Fl_Menu_Item *m = rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
   if ( m ) {
       // Handle the menu item the user picked
       std::string menuname = m->label();
       if ( menuname == "Help" ) {
           fl_alert("Help about this button goes here.");
       } else if ( menuname == "Disconnect All" ) {
           DisconnectAll();
       }
   }
}

// Fltk event handler
int MyButton::handle(int e) {
     switch ( e ) {
         case FL_PUSH: {
             if ( Fl::event_button() == 3 ) {   // right click?
                 DoButtonMenu();
                 return(1);                             // we've handled this
             }
             break;
         }
     }
     return(Fl_OpButton::handle(e));
}

// Fltk draw
void MyButton::draw() {
    Fl_OpButton::draw();

    // NOT CONNECTED, AND CONSTANT SET?
    //    Show its value to the left of the button.
    //
    if ( !IsConnected() && const_val != "" ) {
        int fontsize = 18;
        int align = FL_ALIGN_RIGHT|FL_ALIGN_INSIDE;                     // right justified
        fl_color(0xf0d0a000);
        fl_font(FL_HELVETICA_BOLD, fontsize);
        int X = x() - 40;
        int Y = y() + 2;
        int W = 40 - 5;
        int H = fontsize;
        fl_draw(const_val.c_str(), X,Y,W,H, Fl_Align(align));   // draw the constant next to the button
    }
}

// HANDLE BUTTON PUSH CALLBACKS
void MyButton::MyButtonCallback2() {
    // OUTPUT BUTTON? DO NOTHING
    //    Only input buttons should have constants assigned to them.
    //
    if ( GetButtonType() == FL_OP_OUTPUT_BUTTON ) {
        return;
    }
    // CREATE CONSTANT DIALOG
    if ( !const_dialog ) {
        const_dialog = new MyConstantDialog(this);
    }
    const_dialog->show();
}

void MyButton::MyButtonCallback(Fl_Widget *w, void *userdata) {
    MyButton *b = (MyButton*)userdata;
    b->MyButtonCallback2();
}
