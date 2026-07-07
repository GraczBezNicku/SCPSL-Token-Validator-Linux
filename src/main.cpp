#include "wx/wx.h"

#include "frames/MainFrame.hpp"

class TokenValidaorApp : public wxApp
{
public:
    auto OnInit() -> bool override;
};

auto TokenValidaorApp::OnInit() -> bool
{
    MainFrame *frame = new MainFrame();
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(TokenValidaorApp);
