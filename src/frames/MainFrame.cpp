#include "MainFrame.hpp"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "SCP:SL Token Validator for Linux")
{
    wxMenu *menuActions = new wxMenu;
    menuActions->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuActions, "&Actions");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Using validator as a guest!");

    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("SCP: SL Token Validator for Linux developed by GBN.\nOriginal version for Windows made by zabszk.\nGithub Repository: https://github.com/GraczBezNicku/SCPSL-Token-Validator Linux", "About");
}
