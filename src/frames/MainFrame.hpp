#pragma once

#include "wx/wx.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();
private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};
