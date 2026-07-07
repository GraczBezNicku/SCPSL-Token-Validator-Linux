#include "MainFrame.hpp"

#include "wx/textdlg.h"

#include "../enums/ElementIDs.hpp"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "SCP:SL Token Validator for Linux", wxDefaultPosition, wxSize(1045, 410), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
    wxMenu *menuActions = new wxMenu;
    menuActions->Append(ElementIDs::Action_ProvideToken, "&Provide API Token\tCtrl+T");
    menuActions->AppendSeparator();
    menuActions->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuActions, "&Actions");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    apiModule = new ApiModule;

    CreateStatusBar();
    SetStatusText(apiModule->GetAuthenticationStatus());

    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxButton* clipboardButton = new wxButton(panel, ElementIDs::Button_TokenFromClipboard, "Token from clipboard");
    wxButton* scanButton = new wxButton(panel, ElementIDs::Button_ScanQRFromScreen, "Scan QR from screen");
    wxButton* copyButton = new wxButton(panel, ElementIDs::Button_CopyUserId, "Copy UserID");

    tokenInfoText = new wxStaticText(panel, wxID_ANY, "UserID:\nNickname:\nToken issuance date:\nToken expiration date:", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

    wxFont tokenInfoFont = tokenInfoText->GetFont();
    tokenInfoFont.SetPointSize(18);
    tokenInfoText->SetFont(tokenInfoFont);
    tokenInfoText->Refresh();

    userStatusText = new wxStaticText(panel, wxID_ANY, "Ready", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    userStatusText->SetForegroundColour(wxColour(0, 255, 255));

    wxFont userStatusFont = tokenInfoText->GetFont();
    userStatusFont.SetPointSize(16);
    userStatusText->SetFont(userStatusFont);
    userStatusText->Refresh();

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(clipboardButton, 0, wxALL, 5);
    buttonSizer->Add(scanButton, 0, wxALL, 5);
    buttonSizer->Add(copyButton, 0, wxALL, 5);

    wxBoxSizer* rightSideSizer = new wxBoxSizer(wxVERTICAL);
    rightSideSizer->Add(tokenInfoText, 3, wxEXPAND | wxBOTTOM, 10);
    rightSideSizer->Add(userStatusText, 1, wxEXPAND, 0);

    wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);
    contentSizer->Add(buttonSizer, 0, wxALL, 10);
    contentSizer->Add(rightSideSizer, 1, wxEXPAND | wxALL, 10);

    panel->SetSizer(contentSizer);

    Bind(wxEVT_BUTTON, &MainFrame::OnTokenFromClipboard, this, ElementIDs::Button_TokenFromClipboard);
    Bind(wxEVT_BUTTON, &MainFrame::OnScanQrCodeFromScreen, this, ElementIDs::Button_ScanQRFromScreen);
    Bind(wxEVT_BUTTON, &MainFrame::OnCopyUserId, this, ElementIDs::Button_CopyUserId);

    Bind(wxEVT_MENU, &MainFrame::OnProvideToken, this, ElementIDs::Action_ProvideToken);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
}

auto MainFrame::OnTokenFromClipboard(wxCommandEvent& event) -> void
{

}

auto MainFrame::OnScanQrCodeFromScreen(wxCommandEvent& event) -> void
{

}

auto MainFrame::OnCopyUserId(wxCommandEvent& event) -> void
{

}

auto MainFrame::OnProvideToken(wxCommandEvent& event) -> void
{
    std::string result = wxGetTextFromUser("Input your API token:", "API Token Setup").ToStdString();

    if (result == "")
        return;

    apiModule->ReplaceToken(result);
    apiModule->RefreshToken();

    SetStatusText(apiModule->GetAuthenticationStatus());
}

auto MainFrame::OnExit(wxCommandEvent& event) -> void
{
    Close(true);
}

auto MainFrame::OnAbout(wxCommandEvent& event) -> void
{
    wxMessageBox("SCP: SL Token Validator for Linux developed by GBN.\nOriginal version for Windows made by zabszk.\nGithub Repository: https://github.com/GraczBezNicku/SCPSL-Token-Validator Linux", "About");
}
