#include "wx/wx.h"

#include "frames/MainFrame.hpp"

class TokenValidaorApp : public wxApp
{
public:
    bool OnInit() override;
};

bool TokenValidaorApp::OnInit()
{
    MainFrame *frame = new MainFrame();
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(TokenValidaorApp);
