#pragma once

#include "../modules/ApiModule.hpp"
#include "../modules/QrModule.hpp"

#include "wx/wx.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();
private:
    ApiModule *apiModule;
    QrModule *qrModule;

    wxStaticText* tokenInfoText;
    wxStaticText* userStatusText;

    wxButton* clipboardButton;
    wxButton* scanButton;
    wxButton* copyButton;

    auto ValidateAndDisplay(const std::string& token) -> void;

    auto OnTokenFromClipboard(wxCommandEvent& event) -> void;
    auto OnScanQrCodeFromScreen(wxCommandEvent& event) -> void;
    auto OnCopyUserId(wxCommandEvent& event) -> void;
    auto OnProvideToken(wxCommandEvent& event) -> void;
    auto OnExit(wxCommandEvent& event) -> void;
    auto OnAbout(wxCommandEvent& event) -> void;
};
