#pragma once

#include "../modules/ApiModule.hpp"

#include "wx/wx.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();
private:
    ApiModule *apiModule;

    auto OnProvideToken(wxCommandEvent& event) -> void;
    auto OnExit(wxCommandEvent& event) -> void;
    auto OnAbout(wxCommandEvent& event) -> void;
};
