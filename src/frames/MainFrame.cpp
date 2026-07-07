#include "MainFrame.hpp"

#include "wx/textdlg.h"

#include "../enums/ElementIDs.hpp"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "SCP:SL Token Validator for Linux")
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

    Bind(wxEVT_MENU, &MainFrame::OnProvideToken, this, ElementIDs::Action_ProvideToken);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
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
