#include "MyBox.H"
#include "MyDesk.H"

//////////////////////
// test-app/MyBox.C
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

// COPY CTOR
//    We have to handle bookkeeping carefully for copy/paste operations.
//
MyBox::MyBox(const MyBox& o) : Fl_OpBox(o) {
    name = o.name;
    AssignUniqueFullName();
    srccode = o.srccode;
    traversed = 0;
    infodialog = new MyBoxInfoDialog();
}

// Handle copying this box's instances of MyButton's
//
void MyBox::CopyButtons(const MyBox& o) {
    // MAKE COPIES OF INPUT BUTTONS
    for ( int i=0; i<o.GetTotalInputButtons(); i++ ) {
        begin();
            MyButton *but = (MyButton*)o.GetInputButton(i);
            new MyButton(*but);                 // use copy ctor to make copies
        end();
    }
    // MAKE COPIES OF OUTPUT BUTTONS
    for ( int j=0; j<o.GetTotalOutputButtons(); j++ ) {
        begin();
            MyButton *but = (MyButton*)o.GetOutputButton(j);
            new MyButton(*but);                 // use copy ctor to make copies
        end();
    }
}

// STATIC/PRIVATE: EXPAND VARIABLES -- REPLACE ALL OCCURENCES OF 'varname' WITH 'value' IN 'src'
//    IN: src = "${A} = ${B} + ${C}", varname="${B}", value="1234";
//   OUT: src = "${A} = 1234 + ${C}"
//    'src' returns with all instances of 'varname' changed to 'value'
//
void MyBox::_ExpandVariable(std::string& src, const std::string varname, const std::string value) {
    // XXX: Apparently STL has no replace_all()..
    for ( int pos = src.find(varname); pos >= 0; pos = src.find(varname) ) {
        src.replace(pos, varname.size(), value); 
    }
}

// PRIVATE: Create a new button (during layout load) and load definition from layout file
int MyBox::_LayoutMakeButton(const std::string &buttname, Fl_OpButtonType io,
                             FILE *fp, int &line, std::string &errmsg) {
    // Create button with default settings
    begin();
        MyButton *but = new MyButton(buttname.c_str(), io, MyButton::INT);
    end();
    // Load rest of info from file, apply to button
    if ( but->LoadLayout(fp, line, errmsg) < 0 ) {
        // Insert info about this box into errmsg
        std::string msg = std::string("error while loading button ") +
                          std::string(but->GetFullName()) +
                          std::string(": ") +
                          std::string(errmsg);
        errmsg = msg;
        return(-1);
    }
    return(0);
}

// Set source code to a fixed string
void MyBox::SetSourceCode(const char *val) {
    srccode = val;
}

// Return unexpanded source code
std::string MyBox::GetSourceCode() {
    return(srccode);
}

// Return the 'traversed' value
//    This is used by the code generator to prevent generating code more than once.
//
int MyBox::GetTraversed() const {
    return(traversed);
}

// See if this box has been traversed.
int MyBox::IsTraversed() const {
    return(traversed ? 1 : 0);
}

// Set the 'traversed' value
void MyBox::SetTraversed(int val) {
    traversed = val;
}

// Returns a copy of the configured source code with all variables expanded
//    Walks the tree backwards to resolve inputs.
//
std::string MyBox::GetExpandedSourceCode() {
    std::string out = srccode;

    // Expand box title
    std::string varname = "${BoxTitle}";
    std::string value = label();                        // e.g. "add_12"
    _ExpandVariable(out, varname, value);

    // Expand buttons and variable names (or values)
    for ( int t=0; t<children(); t++ ) {
        MyButton *b = (MyButton*)child(t);
        varname = std::string("${") +                   // eg. "${A}"
                  std::string(b->label()) +
                  std::string("}");
        value = b->GetVarValue();                       // walks tree backwards to derive value
        _ExpandVariable(out, varname, value);
    }
    return(out);
}

// How a box saves itself to a file
int MyBox::SaveLayout(FILE *fp) {
    fprintf(fp, "# %s\n", label());
    fprintf(fp, "box \"%s\"\n", GetName().c_str());
    fprintf(fp, "{\n");

    // Save box attributes
    fprintf(fp, "    label \"%s\"\n", label()?label():"???");
    fprintf(fp, "    color %lx\n", (unsigned long)color());
    fprintf(fp, "    labelcolor %lx\n", (unsigned long)labelcolor());
    fprintf(fp, "    xy %d %d\n", x(), y());
    fprintf(fp, "    select %d\n", GetSelected());

    // Save all buttons
    for ( int t=0; t<GetTotalButtons(); t++ ) {
        MyButton *b = (MyButton*)GetButton(t);
        b->SaveLayout(fp);
    }

    // Save source code
    {
        fprintf(fp, "    src begin\n    {\n");
        char *copy = strdup(srccode.c_str());
        char *s = copy, *ss = copy;
        while ( ( ss = strchr(s, '\n') ) != NULL ) {
            *ss = 0;
            fprintf(fp, "\t>%s\n", s);
            s = ss+1;
        }
        if ( *s ) {
            fprintf(fp, "\t>%s\n", s);
        }
        free(copy);
        fprintf(fp, "    }\n");
    }

    fprintf(fp, "}\n");
    return(0);
}

int MyBox::_LoadSrc(FILE *fp, int &line, std::string &errmsg) {
    char s[1024];
    int braces = 0;
    srccode = "";
    while ( fgets(s, sizeof(s)-1, fp) != 0 ) {
        line++;
        char *ss = skipwhite(s);
        if ( *ss == '\n' || *ss == '\r' || *ss == 0 || *s == '#' ) {    // skip blank lines + comments
            continue;
        }
        if ( *ss == '{' ) {                             // count open braces
            ++braces;
            continue;
        }
        if ( *ss == '}' ) {                             // count close braces
            if ( --braces <= 0 ) { break; }
            else { continue; }
        }
        if ( *ss == '>' ) {                             // source code lines start with '>'
            srccode += (ss+1);
            continue;
        }
        // Got this far? Unknown command
        errmsg = std::string("bad line in src code block");
        return(-1);
    }
    return(0);
}

// How a box loads itself from a file
//     Returns 0 if loaded OK, -1 on error, errmsg has reason.
//
int MyBox::LoadLayout(FILE *fp, int &line, std::string &errmsg) {
    int X,Y;
    char s[1024], arg[1024];
    int braces = 0;
    unsigned long ul;
    while ( fgets(s, sizeof(s)-1, fp) != 0 ) {
        line++;
        char *ss = skipwhite(s);
        if ( *ss == '\n' || *ss == '\r' || *ss == 0 || *s == '#' ) {    // skip blank lines + comments
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
        if ( sscanf(ss, "label \"%1023[^\"]\"", arg) == 1 ) {
            copy_label(arg);
            continue;
        }
        if ( sscanf(ss, "color %lx", &ul) == 1 ) {
            color(Fl_Color(ul));
            continue;
        }
        if ( sscanf(ss, "labelcolor %lx", &ul) == 1 ) {
            labelcolor(Fl_Color(ul));
            continue;
        }
        if ( sscanf(ss, "xy %d %d", &X, &Y) == 2 ) {
            position(X,Y);
            continue;
        }
        if ( sscanf(ss, "select %d", &X) == 1 ) {
            SetSelected(X);
            continue;
        }
        if ( sscanf(ss, "in \"%1023[^\"]\"", arg) == 1 ) {      // input button?
            std::string buttname = arg;
            if ( _LayoutMakeButton(buttname, FL_OP_INPUT_BUTTON, fp, line, errmsg) < 0 ) {
                errmsg += std::string("\n") + std::string(s);
                return(-1);
            }
            continue;
        }
        if ( sscanf(ss, "out \"%1023[^\"]\"", arg) == 1 ) {     // output button?
            std::string buttname = arg;
            if ( _LayoutMakeButton(buttname, FL_OP_OUTPUT_BUTTON, fp, line, errmsg) < 0 ) {
                errmsg += std::string("\n") + std::string(s);
                return(-1);
            }
            continue;
        }
        if ( sscanf(ss, "src %20s", arg) == 1 ) {
            if ( _LoadSrc(fp, line, errmsg) < 0 ) {
                errmsg += std::string("\n") + std::string(s);
                return(-1);
            }
            continue;
        }
        // Got this far? Unknown command
        errmsg = std::string("unknown box command") +
                 std::string("\n") + std::string(s);
        return(-1);
    }
    return(0);
}

/// Set the base name of this box, eg. "add"
///    Ensures the box's fullname will be 'unique'.
///
void MyBox::SetName(const char *newname) {
    name = newname;
    AssignUniqueFullName();
}

/// Get the base name of this box. eg. "add"
std::string MyBox::GetName() const {
    return(name);
}

/// Assign ourselves a unique name.
///    Uses the box's index number on the desk.
///    If boxes are created or reordered on the desk, this
///    should be called to ensure box has a unique name.
///
void MyBox::AssignUniqueFullName() {
    char nametmp[256];
    int id = ((MyDesk*)GetOpDesk())->GetNextBoxId();
#ifdef _WIN32
    _snprintf_s(nametmp, sizeof(nametmp), _TRUNCATE, "%s_%d", name.c_str(), id);        // microsoft's _sprintf() doesn't truncate(!) use _sprintf_s() instead
#else
    snprintf(nametmp, sizeof(nametmp), "%s_%d", name.c_str(), id);
#endif
    copy_label(nametmp);
}

/// FLTK Event handler
///     We trap this so we can detect double clicks on the box.
///
int MyBox::handle(int e) {
    int ret = Fl_OpBox::handle(e);
    switch (e) {
        case FL_PUSH:
            // Double click on box? Show 'Info' dialog
            if ( Fl::event_clicks() == 1 ) {
                ShowInfoDialog();
            }
            break;
        case FL_RELEASE:
            break;
    }
    return(ret);
}

void MyBox::ShowInfoDialog() {
    // Update the info dialog with all our buttons
    infodialog->Clear();
    for ( int t=0; t<GetTotalButtons(); t++ ) {
        MyButton *but = (MyButton*)GetButton(t);
        infodialog->AddButton(but);
    }
    // Show the dialog
    infodialog->Show(label());
}
