#include "MyBoxInfoDialog.H"

ButtonInfo::ButtonInfo(int X,int Y,int W,int H,MyButton *b) : Fl_Group(X,Y,W,H) {
    but = b;
    box(FL_FLAT_BOX);
    color(46);
    begin();                            // begin group
    {
        input = new Fl_Input(X+100,Y,W-100,H,"");
        input->align(FL_ALIGN_LEFT);
        input->callback(InputChanged_CB, (void*)this);
        input->when(FL_WHEN_ENTER_KEY_CHANGED);
    }
    end();                              // end group

    SetName(but->label());
    SetConstantValue(but->GetConstantValue().c_str());
    if ( but->IsConnected() ) {
        input->color(50);               // indicate button 'connected'
        input->tooltip("This button is connected");
    } else {
        input->color(FL_WHITE);
        input->tooltip(0);
    }
    SetButtonType(but->GetButtonType());
}
