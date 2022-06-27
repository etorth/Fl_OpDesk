// Test Application
//
//    This shows a 'starting point' for how to approach writing
//    a working application that uses the Fl_OpDesk widget.
//

//////////////////////
// test-app/test-app.C
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
#include <stdio.h>              // fopen, popen..
#include <string.h>             // strerror
#include <errno.h>              // errno

#include "MyButton.H"
#include "FSM_Node.H"
#include "MyDesk.H"

#include <FL/Fl_Text_Display.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>          // fl_message()

using namespace std;

// APPLICATION CLASS
//    Creates the main window, menubar, Fl_OpDesk.
//    
class App : public Fl_Double_Window {
    Fl_Menu_Bar *menubar;               // main menubar
    MyDesk      *mydesk;                // my operation desk

    // Layout files
    Fl_File_Chooser *layout_fc;         // layout load/save file chooser
    std::string      layout_filename;   // layout filename

    // CREATE AN 'ADD' BOX
    FSM_Node* MakeAddBox(int X, int Y) {
        mydesk->begin();
            // Create box
            FSM_Node *mybox = new FSM_Node(X,Y,150,80,"add");
            mybox->SetSourceCode("# Box: '${BoxTitle}'\n"
                                 "${OUT} = ${A} + ${B} + ${C};\n"
                                 "printf(\"\\${OUT}: %d\\n\",${OUT});   # print result\n"
                                 "\n");
            mybox->color(0xc0c0ff00);
            mybox->labelcolor(0x40404000);
            mybox->begin();
                // Create Input/Output buttons for the box
                MyButton *a   = new MyButton("A",   FL_OP_INPUT_BUTTON,  MyButton::INT);
                MyButton *b   = new MyButton("B",   FL_OP_INPUT_BUTTON,  MyButton::INT);
                MyButton *c   = new MyButton("C",   FL_OP_INPUT_BUTTON,  MyButton::INT);
                MyButton *out = new MyButton("OUT", FL_OP_OUTPUT_BUTTON, MyButton::INT);
                c = c;          // silence compiler warnings
                out = out;      // silence compiler warnings
            mybox->end();
        mydesk->end();
        // DEBUG: Assign incrementing static values to each button
        {
            static int bvalue = 0;
            char s[80]; 
            sprintf(s, "%d", ++bvalue); a->SetConstantValue(s);
            sprintf(s, "%d", ++bvalue); b->SetConstantValue(s);
            sprintf(s, "%d", ++bvalue); c->SetConstantValue(s);
        }
        return(mybox);
    }

    // CREATE AN 'MULT' BOX
    FSM_Node* MakeMultBox(int X, int Y) {
        mydesk->begin();
            // Create box
            FSM_Node *mybox = new FSM_Node(X,Y,150,80,"mult");
            mybox->SetSourceCode("# Box: '${BoxTitle}'\n"
                                 "${OUT} = ${A} * ${B};\n"
                                 "printf(\"\\${OUT}: %d\\n\",${OUT});   # print result\n"
                                 "\n");
            mybox->color(0xc0c0ff00);
            mybox->labelcolor(0x40404000);
            mybox->begin();
                // Create Input/Output buttons for the box
                MyButton *a = new MyButton("A",   FL_OP_INPUT_BUTTON,  MyButton::INT);
                MyButton *b = new MyButton("B",   FL_OP_INPUT_BUTTON,  MyButton::INT);
                MyButton *c = new MyButton("OUT", FL_OP_OUTPUT_BUTTON, MyButton::INT);
                c = c;          // silence compiler warnings
            mybox->end();
        mydesk->end();
        // DEBUG: Assign incrementing static values to each button
        {
            static int bvalue = 0;
            char s[80]; 
            sprintf(s, "%d", ++bvalue); a->SetConstantValue(s);
            sprintf(s, "%d", ++bvalue); b->SetConstantValue(s);
        }
        return(mybox);
    }

    // CREATE A 'StrCat' BOX
    FSM_Node* MakeStrcatBox(int X, int Y) {
        mydesk->begin();
            // Create box
            FSM_Node *mybox = new FSM_Node(X,Y,150,80,"strcat");
            mybox->SetSourceCode("# Box: '${BoxTitle}'\n"
                                 "${OUT} = ${A} . ${B};\n"
                                 "printf(\"\\${OUT}: '%s'\\n\",${OUT});   # print result\n"
                                 "\n");
            mybox->color(0xc0c0c000);
            mybox->labelcolor(0x40404000);
            mybox->begin();
                // Create Input/Output buttons for the box
                MyButton *a = new MyButton("A",   FL_OP_INPUT_BUTTON,  MyButton::ANY);
                MyButton *b = new MyButton("B",   FL_OP_INPUT_BUTTON,  MyButton::ANY);
                MyButton *c = new MyButton("OUT", FL_OP_OUTPUT_BUTTON, MyButton::STRING);
                c = c;          // silence compiler warnings
            mybox->end();
        mydesk->end();
        {
            a->SetConstantValue("\"Aaa\"");
            b->SetConstantValue("\"Bbb\"");
        }
        return(mybox);
    }

    // CREATE A 'Print' BOX
    FSM_Node* MakePrintBox(int X, int Y) {
        mydesk->begin();
            // Create box
            FSM_Node *mybox = new FSM_Node(X,Y,150,80,"print");
            mybox->SetSourceCode("# Box: '${BoxTitle}'\n"
                                 "print \"${IN}\\n\";\n"
                                 "\n");
            mybox->color(0xc0c0c000);
            mybox->labelcolor(0x40404000);
            mybox->begin();
                // Create Input button for the box
                //     Allow float/int/string to be connected.
                //
                MyButton *in = new MyButton("IN", FL_OP_INPUT_BUTTON, MyButton::ANY);
                in->SetConstantValue("\"This is a test\"");
            mybox->end();
        mydesk->end();
        return(mybox);
    }

    // CREATE A 'SIN' BOX
    FSM_Node* MakeSinBox(int X, int Y) {
        mydesk->begin();
            // Create box
            FSM_Node *mybox = new FSM_Node(X,Y,150,80,"sin");
            mybox->SetSourceCode("# Box: '${BoxTitle}'\n"
                                 "${OUT} = sin(${A});\n"
                                 "\n");
            mybox->color(0xa0e0a000);
            mybox->labelcolor(0x40404000);
            mybox->begin();
                // Create Input/Output buttons for the box
                MyButton *a   = new MyButton("A",   FL_OP_INPUT_BUTTON,  MyButton::FLOAT);
                MyButton *out = new MyButton("OUT", FL_OP_OUTPUT_BUTTON, MyButton::FLOAT);
                a->SetConstantValue("0.00");
                out = out;      // silence compiler warnings
            mybox->end();
        mydesk->end();
        return(mybox);
    }

    // CREATE A 'COS' BOX
    FSM_Node* MakeCosBox(int X, int Y) {
        mydesk->begin();
            // Create box
            FSM_Node *mybox = new FSM_Node(X,Y,150,80,"cos");
            mybox->SetSourceCode("# Box: '${BoxTitle}'\n"
                                 "${OUT} = cos(${A});\n"
                                 "\n");
            mybox->color(0xa0e0a000);
            mybox->labelcolor(0x40404000);
            mybox->begin();
                // Create Input/Output buttons for the box
                MyButton *a   = new MyButton("A",   FL_OP_INPUT_BUTTON,  MyButton::FLOAT);
                a->SetConstantValue("0.00");
                MyButton *out = new MyButton("OUT", FL_OP_OUTPUT_BUTTON, MyButton::FLOAT);
                a = a;          // silence compiler warnings
                out = out;      // silence compiler warnings
            mybox->end();
        mydesk->end();
        return(mybox);
    }

    void RunCommandInWindow(std::string cmd) {
        // CREATE THE COMMAND WINDOW
        static Fl_Window       *win = 0;
        static Fl_Text_Buffer  *buf = 0;
        static Fl_Text_Display *out = 0;
        if ( win == 0 ) {
            // Make window for first time
            win = new Fl_Window(720,400);
            win->begin();
                buf = new Fl_Text_Buffer();
                out = new Fl_Text_Display(10, 10, win->w()-20, win->h()-20);
                out->buffer(buf);
                out->textfont(FL_COURIER);
            win->end();
            win->resizable(win);
        }

        // TITLE THE WINDOW WITH THE COMMAND WE'RE RUNNING
        std::string title;
        title = "Executing: ";
        title += cmd;
        win->copy_label(title.c_str());
        win->show();
        Fl::wait(.10);

        // RUN THE COMMAND, APPEND OUTPUT TO WINDOW
        FILE *fp = popen(cmd.c_str(), "r");
        char linebuf[4096];
        while ( fgets(linebuf, sizeof(linebuf)-1, fp) != NULL ) {
            buf->text(linebuf);
            Fl::wait(.10);
        }
        pclose(fp);
        return;
    }

    // COMPILE AND RUN THE GENERATED CODE
    //     Save the code to a temp file, and run it with perl
    //     with perl's output sent to a window.
    //
    void CompileAndRun() {
        // GENERATE CODE
        std::string code;
        GenerateCode(code);

        // WRITE CODE TO A TEMP FILE
        std::string tmpfile;
#ifdef MICROSOFT
        tmpfile = "c:/temp/test-app.pl";
#define popen _popen                    // *eyeroll*
#else
        tmpfile = "/tmp/test-app.pl";
#endif
        FILE *fp;
        if ( (fp = fopen(tmpfile.c_str(), "w")) == NULL) {
            fl_message("%s: %s", tmpfile.c_str(), strerror(errno));
            return;
        }
        fwrite(code.c_str(), 1, code.size(), fp);
        fclose(fp);

        // RUN COMMAND WITH OUTPUT IN WINDOW
        std::string cmd;
        cmd = "perl ";
        cmd += tmpfile;
        RunCommandInWindow(cmd);
    }

    // MAIN MENU CALLBACK
    //    Handle the main menubar item the user picked.
    //
    void Menu_CB2() {
        char picked[1024];
        menubar->item_pathname(picked, sizeof(picked)-1);
        if ( getenv("MYAPP_DEBUG") ) {
            fprintf(stderr, "MENU CALLBACK: ITEM='%s'\n", picked);
        }
        // Handle the menu item picked
             if ( strcmp(picked, "&File/&New"                      ) == 0 ) { LayoutNew(); }
        else if ( strcmp(picked, "&File/&Open"                     ) == 0 ) { LayoutOpen(); }
        else if ( strcmp(picked, "&File/&Save"                     ) == 0 ) { LayoutSave(); }
        else if ( strcmp(picked, "&File/Save &As"                  ) == 0 ) { LayoutSaveAs(); }
        else if ( strcmp(picked, "&File/Save &Code"                ) == 0 ) { SaveCode(); }
        else if ( strcmp(picked, "&File/&Quit"                     ) == 0 ) { exit(0); }
        else if ( strcmp(picked, "&Edit/Copy"                      ) == 0 ) { mydesk->CopySelected(); }
        else if ( strcmp(picked, "&Edit/Cut"                       ) == 0 ) { mydesk->CutSelected(); }
        else if ( strcmp(picked, "&Edit/Paste"                     ) == 0 ) { mydesk->PasteSelected(); }
        else if ( strcmp(picked, "&Edit/Select All"                ) == 0 ) { mydesk->SelectAll(); }
        else if ( strcmp(picked, "&Edit/Preferences/Lines/Trace"   ) == 0 ) { mydesk->SetConnectStyle(FL_OPCONNECT_STYLE_TRACE); }
        else if ( strcmp(picked, "&Edit/Preferences/Lines/Curved"  ) == 0 ) { mydesk->SetConnectStyle(FL_OPCONNECT_STYLE_CURVE); }
        else if ( strcmp(picked, "&Edit/Preferences/Lines/Straight") == 0 ) { mydesk->SetConnectStyle(FL_OPCONNECT_STYLE_STRAIGHT); }
        else if ( strcmp(picked, "&Edit/Preferences/Debug"         ) == 0 ) {
            Fl_Menu_Item *item = (Fl_Menu_Item*)menubar->mvalue();
            if ( item->value() ) {
                putenv((char*)"MYAPP_DEBUG=1");
            } else {
                unsetenv((char*)"MYAPP_DEBUG");
            }
        }
        else if ( strcmp(picked, "&Operators/&Add"                 ) == 0 ) { MakeAddBox(300,300); }
        else if ( strcmp(picked, "&Operators/&Mult"                ) == 0 ) { MakeMultBox(300,300); }
        else if ( strcmp(picked, "&Operators/&Strcat"              ) == 0 ) { MakeStrcatBox(300,300); }
        else if ( strcmp(picked, "&Operators/&Print"               ) == 0 ) { MakePrintBox(300,300); }
        else if ( strcmp(picked, "Functions/&Sin"                  ) == 0 ) { MakeSinBox(300,300); }
        else if ( strcmp(picked, "Functions/&Cos"                  ) == 0 ) { MakeCosBox(300,300); }
        else if ( strcmp(picked, "&Test/Make Test Boxes"           ) == 0 ) { MakeTestBoxes(); }
        else if ( strcmp(picked, "&Run/Compile + Run"              ) == 0 ) { CompileAndRun(); }
    }
    static void Menu_CB(Fl_Widget*, void *userdata) {
        App *app = (App*)userdata;
        app->Menu_CB2();
    }

    // Enable/disable debug preference
    void DebugMode(int val) {
        Fl_Menu_Item *item = (Fl_Menu_Item*)menubar->find_item("&Edit/Preferences/Debug");
        if ( item ) {
            if ( val ) {
                item->flags = (item->flags | FL_MENU_VALUE);
                putenv((char*)"MYAPP_DEBUG=1");
            } else {
                item->flags = (item->flags & ~FL_MENU_VALUE);
                unsetenv((char*)"MYAPP_DEBUG");
            }
        }
    }

public:
    App(int W, int H, const char *L=0) : Fl_Double_Window(W,H,L) {
        // Create app's menubar
        menubar = new Fl_Menu_Bar(0,0,w(),20);
        menubar->box(FL_FLAT_BOX);
        menubar->add("&File/&New",                       0,         Menu_CB, (void*)this);
        menubar->add("&File/&Open",                      0,         Menu_CB, (void*)this, FL_MENU_DIVIDER);
        menubar->add("&File/&Save",                      "^s",      Menu_CB, (void*)this);
        menubar->add("&File/Save &As",                   0,         Menu_CB, (void*)this, FL_MENU_DIVIDER);
        menubar->add("&File/Save &Code",                 "+^S",     Menu_CB, (void*)this);
        menubar->add("&File/&Quit",                      "^q",      Menu_CB, (void*)this);
        menubar->add("&Edit/Copy",                       "^c",      Menu_CB, (void*)this);
        menubar->add("&Edit/Cut",                        "^x",      Menu_CB, (void*)this);
        menubar->add("&Edit/Paste",                      "^v",      Menu_CB, (void*)this);
        menubar->add("&Edit/Select All",                 "^a",      Menu_CB, (void*)this);
        menubar->add("&Edit/Preferences/Lines/Trace",    0,         Menu_CB, (void*)this);
        menubar->add("&Edit/Preferences/Lines/Curved",   0,         Menu_CB, (void*)this);
        menubar->add("&Edit/Preferences/Lines/Straight", 0,         Menu_CB, (void*)this);
        menubar->add("&Edit/Preferences/Debug",          0,         Menu_CB, (void*)this, FL_MENU_TOGGLE);  // off by default
        menubar->add("&Operators/&Add",                  0,         Menu_CB, (void*)this);
        menubar->add("&Operators/&Mult",                 0,         Menu_CB, (void*)this);
        menubar->add("&Operators/&Strcat",               0,         Menu_CB, (void*)this);
        menubar->add("&Operators/&Print",                0,         Menu_CB, (void*)this);
        menubar->add("Functions/&Ramp",                  0,         Menu_CB, (void*)this, FL_MENU_INACTIVE);
        menubar->add("Functions/&Sin",                   0,         Menu_CB, (void*)this);
        menubar->add("Functions/&Cos",                   0,         Menu_CB, (void*)this);
        menubar->add("Display/Graph",                    0,         Menu_CB, (void*)this, FL_MENU_INACTIVE); 
        menubar->add("&Test/Make Test Boxes",            0,         Menu_CB, (void*)this); 
        menubar->add("&Run/Compile + Run",               FL_F+4,    Menu_CB, (void*)this); 

        // Create operations desk
        mydesk = new MyDesk(10,20,w()-20,h()-30);
        mydesk->end(); 
        resizable(mydesk);
        layout_fc = NULL;
        layout_filename = "";
        show();
    }

    // Make some test boxes
    void MakeTestBoxes() {
        std::string errmsg; 
        // MAKE TWO BOXES
        FSM_Node *a = MakeAddBox(200,200);
        FSM_Node *b = MakeAddBox(400,200); 
        // VERIFY WE CAN SET CONSTANTS FOR INPUTS AFTER BUTTONS CREATED
        //    "Constant values" are a concept we implement in our MyButton class,
        //    allowing unconnected input buttons to be assigned a fixed value.
        {
            MyButton *but;
            if ( ( but = (MyButton*)a->FindButtonByLabel("A", errmsg) ) ) but->SetConstantValue("1.0");
            if ( ( but = (MyButton*)a->FindButtonByLabel("B", errmsg) ) ) but->SetConstantValue("2.0");
            if ( ( but = (MyButton*)a->FindButtonByLabel("C", errmsg) ) ) but->SetConstantValue("3.0");
        } 
        // MAKE CONNECTIONS BETWEEN THE TWO BOXES
        fprintf(stderr, "THIS CONNECT SHOULD WORK: %s(OUT) -> %s(A)\n", a->label(), b->label());
        if ( mydesk->Connect(a, "OUT",  // from
                             b, "A",    // to
                             errmsg) < 0 ) {
            fprintf(stderr, "ERROR: FAILED TO CONNECT: %s\n", errmsg.c_str());
            exit(1);
        } 
        fprintf(stderr, "THIS CONNECT SHOULD WORK: %s(OUT) -> %s(B)\n", a->label(), b->label());
        mydesk->SetOpConnectWidth(4);
        mydesk->SetOpConnectColor(FL_RED);
        if ( mydesk->Connect(a, "OUT",  // from
                             b, "B",    // to
                             errmsg) < 0 ) {
            fprintf(stderr, "ERROR: FAILED TO CONNECT: %s\n", errmsg.c_str());
            exit(1);
        } 
        fprintf(stderr, "THIS CONNECT SHOULD WORK: %s(OUT) -> %s(C)\n", a->label(), b->label());
        if ( mydesk->Connect(a, "OUT",  // from
                             b, "C",    // to
                             errmsg) < 0 ) {
            fprintf(stderr, "ERROR: FAILED TO CONNECT: %s\n", errmsg.c_str());
            exit(1);
        }
        Fl_OpConnect *con = mydesk->GetLastOpConnect();
        con->SetColor(FL_BLUE);
        con->SetWidth(6);
        fprintf(stderr, "VERIFY NON-EXISTING BUTTON CONNECTS SHOULD FAIL:\n");
        if ( mydesk->Connect(a, "OUT",  // from
                             b, "NOPE", // to
                             errmsg) < 0 ) {
            fprintf(stderr, "OK -- as expected, error: %s\n", errmsg.c_str());
            errmsg = "";
        }
        fprintf(stderr, "THIS CONNECT BETWEEN TWO OUTPUTS SHOULD FAIL:\n");
        if ( mydesk->Connect(a, "OUT",  // from
                             b, "OUT",  // to
                             errmsg) < 0 ) {
            fprintf(stderr, "OK -- as expected, error: %s\n", errmsg.c_str());
            errmsg = "";
        }
    }

    // Save layout given a filename
    //    Returns: 0 if OK
    //            -1 on error errmsg has reason.
    //
    int LayoutSaveFilename(std::string filename, std::string &errmsg) {
        // Open file for writing, save
        FILE *fp = fopen(filename.c_str(), "w");
        if ( fp == NULL ) {
            // Error? Post dialog
            errmsg = std::string("Cannot open '") +
                     std::string(filename) +
                     std::string("' for writing: ") +
                     std::string(strerror(errno));
            return(-1);
        }
        if ( mydesk->SaveLayout(fp) != 0 ) {
            errmsg = std::string("Failed while writing '") +
                     std::string(layout_filename) +
                     std::string(": ") + 
                     std::string(strerror(errno));
            fclose(fp);
            return(-1);
        }
        fclose(fp);
        return(0);
    }

    // Open file chooser, let user specify filename to save layout
    int LayoutSaveAs() {
        // Create chooser if none already
        if ( layout_fc == NULL ) {
            layout_fc = new Fl_File_Chooser(".", "*", Fl_File_Chooser::SINGLE, "Save Layout");
            layout_fc->filter("Layout Files(*.lay)");
        }
        layout_fc->label("Save Layout");
        layout_fc->type(Fl_File_Chooser::SINGLE|Fl_File_Chooser::CREATE);
        // Post file chooser, wait for input
        layout_fc->show();
        while (layout_fc->visible()) {
            Fl::wait();
        }
        // User didn't hit 'cancel'? Keep the filename they specified, save file
        if ( layout_fc->count() > 0 && layout_fc->value() ) {
            // Get filename user supplied
            layout_filename = layout_fc->value();
            // No filename extension? Add one
            if ( !strchr(layout_filename.c_str(), '.') ) {
                layout_filename += ".lay";
                layout_fc->value(layout_filename.c_str());
            }
            // Save file
            std::string errmsg;
            if ( LayoutSaveFilename(layout_filename, errmsg) < 0 ) {
                fl_alert("%s", errmsg.c_str());
            }
        }
        return(0);
    }

    // Save box layout
    void LayoutSave() {
        if ( layout_filename == "" ) {          // no filename already? Get one!
            if ( LayoutSaveAs() < 0 ) return;   // user cancel?
        }
        std::string errmsg;
        if ( LayoutSaveFilename(layout_filename, errmsg) < 0 ) {
            fl_alert("%s", errmsg.c_str());
        }
    }

    // Load layout from a filename
    //    Returns: 0 if OK
    //            -1 on error errmsg has reason.
    //
    int LayoutLoadFilename(std::string filename, std::string &errmsg) {
        // Open file for reading, load
        FILE *fp = fopen(filename.c_str(), "r");
        if ( fp == NULL ) {
            // Error? Post dialog
            errmsg = std::string("Cannot open '") +
                     std::string(filename) +
                     std::string("' for reading: ") +
                     std::string(strerror(errno));
            return(-1);
        }
        int line = 0;
        if ( mydesk->LoadLayout(fp, line, errmsg) != 0 ) {
            std::string msg = std::string(filename) +
                              std::string(": Error (Line ") +
                              std::string(itoa(line)) +
                              std::string("): ") +
                              std::string(errmsg);
            errmsg = msg;
            fclose(fp);
            return(-1);
        }
        fclose(fp);
        return(0);
    }

    // Open file chooser, let user specify filename to load layout
    int LayoutOpen() {
        // Create chooser if none already
        if ( layout_fc == NULL ) {
            layout_fc = new Fl_File_Chooser(".", "*", Fl_File_Chooser::SINGLE, "Load Layout");
            layout_fc->filter("Layout Files(*.lay)");
        }
        layout_fc->label("Open Layout");
        layout_fc->type(Fl_File_Chooser::SINGLE);
        // Post file chooser, wait for input
        layout_fc->show();
        while (layout_fc->visible()) {
            Fl::wait();
        }
        // User didn't hit 'cancel'? Keep the filename they specified, save file
        if ( layout_fc->count() > 0 && layout_fc->value() ) {
            // Get filename user supplied
            layout_filename = layout_fc->value();
            // No filename extension? Add one
            if ( !strchr(layout_filename.c_str(), '.') ) {
                layout_filename += ".lay";
                layout_fc->value(layout_filename.c_str());
            }
            // Clear desk, load file
            mydesk->Clear();
            std::string errmsg;
            if ( LayoutLoadFilename(layout_filename, errmsg) < 0 ) {
                fl_alert("%s", errmsg.c_str());
            }
        }
        return(0);
    }

    // New layout -- start with a clear desk
    //     Returns 0 if done, -1 if user chose to cancel.
    //
    int LayoutNew() {
        if ( mydesk->GetOpBoxTotal() != 0 ) {           // any boxes? ask first
            if ( fl_choice("Clear the desk first?", "Cancel", "New", 0) == 0 ) {
                return(-1);
            }
        }
        mydesk->Clear();
        return(0);
    }

    // Recurse the tree backwards, accumulating boxes forwards
    void GetBoxesSorted(std::vector<FSM_Node*> &boxes, FSM_Node *box) {
        if ( box->IsTraversed() ) return;
        box->SetTraversed(1);
        // Loop thru input buttons, walking up the tree one level
        for ( int t=0; t<box->GetTotalInputButtons(); t++ ) { 
            // For each input button, find all boxes connected to it
            Fl_OpButton *inbut = box->GetInputButton(t);
            std::vector<Fl_OpBox*> outboxes;
            inbut->GetOutputBoxes(outboxes); 
            // Loop through all the boxes we collected
            for ( size_t j=0; j<outboxes.size(); j++ ) {
                FSM_Node *mybox = (FSM_Node*)outboxes[j];
                GetBoxesSorted(boxes, mybox);
            }
        }
        boxes.push_back(box);           // do this last, so trunks of tree at end of list
    }

    // Return an array of boxes sorted 'appropriately' for code generation
    void GetBoxesSortedLeftToRight(std::vector<FSM_Node*> &boxes) {
        int i;
        // Mark all boxes 'traversed'
        for ( i=0; i<mydesk->GetOpBoxTotal(); i++ ) {
            FSM_Node *box = (FSM_Node*)mydesk->GetOpBox(i);
            box->SetTraversed(0);
        }
        // Find all boxes that have no outputs
        for ( i=0; i<mydesk->GetOpBoxTotal(); i++ ) {
            FSM_Node *box = (FSM_Node*)mydesk->GetOpBox(i);
            int tconnects = 0;
            int outbuts = box->GetTotalOutputButtons();
            for ( int j=0; j<outbuts; j++ ) {
                MyButton *obut = (MyButton*)box->GetOutputButton(j);
                tconnects += obut->GetTotalConnectedButtons();
            }
            if ( tconnects == 0 ) {
                GetBoxesSorted(boxes, box);
            }
        }
    }

    // Show text in a popup window
    void ShowText(const char* text, const char* title)
    {
        static Fl_Window       *win = 0;
        static Fl_Text_Buffer  *buf = 0;
        static Fl_Text_Display *out = 0;
        if ( win == 0 ) {
            // Make window for first time
            win = new Fl_Window(720,400);
            win->begin();
                buf = new Fl_Text_Buffer();
                out = new Fl_Text_Display(10, 10, win->w()-20, win->h()-20);
                out->buffer(buf);
                out->textfont(FL_COURIER);
            win->end();
            win->resizable(win);
        }
        buf->text(text);
        win->copy_label(title);
        win->show();
    }

    // Generate code into string
    void GenerateCode(std::string &code) {
        // C STYLE HEADER
        code += "#!/usr/bin/perl\n"
                "### START\n\n";
        // Get a list of the boxes sorted in left-to-right order
        std::vector<FSM_Node*> boxes;
        GetBoxesSortedLeftToRight(boxes); 
        // Walk the box list, generating code as we go
        for ( size_t t=0; t<boxes.size(); t++ ) {
            FSM_Node *box = boxes[t];
            code += box->GetExpandedSourceCode();
        }
        // C STYLE FOOTER
        code += "### END\n"
                "exit(0);\n";
    }

    // Save all box's source code
    int SaveCode() {
        std::string code;
        GenerateCode(code);
#ifdef PRINTF_SOURCE
        printf("%s", code.c_str());
#else /*PRINTF_SOURCE*/
        // Make a window, show source in text viewer
        ShowText(code.c_str(), "Source Code Viewer");
#endif /*PRINTF_SOURCE*/
        return(0);
    }
};

int main() {
    App app(720,486,"test-App");
    return(Fl::run());
}
