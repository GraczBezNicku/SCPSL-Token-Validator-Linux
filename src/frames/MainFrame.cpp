#include "MainFrame.hpp"

#include "wx/textdlg.h"
#include "wx/clipbrd.h"

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

    qrModule = new QrModule;
    apiModule = new ApiModule;

    CreateStatusBar();
    SetStatusText(apiModule->GetAuthenticationStatus());

    wxPanel* panel = new wxPanel(this, wxID_ANY);

    clipboardButton = new wxButton(panel, ElementIDs::Button_TokenFromClipboard, "Token from clipboard");
    scanButton = new wxButton(panel, ElementIDs::Button_ScanQRFromScreen, "Scan QR from screen");
    copyButton = new wxButton(panel, ElementIDs::Button_CopyUserId, "Copy UserID");

    tokenInfoText = new wxStaticText(panel, wxID_ANY, "UserID:\nNickname:\nToken issuance date:\nToken expiration date:", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

    wxFont tokenInfoFont = tokenInfoText->GetFont();
    tokenInfoFont.SetPointSize(18);
    tokenInfoText->SetFont(tokenInfoFont);
    tokenInfoText->Refresh();

    userStatusText = new wxStaticText(panel, wxID_ANY, "Ready", wxDefaultPosition, wxDefaultSize);
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

auto MainFrame::ValidateAndDisplay(const std::string& token) -> void
{
    clipboardButton->Disable();
    scanButton->Disable();
    copyButton->Disable();

    tokenInfoText->SetLabelText("UserID:\nNickname:\nToken issuance date:\nToken expiration date:");
    tokenInfoText->Refresh();

    userStatusText->SetLabelText("Token validation in progress...");
    userStatusText->SetForegroundColour(wxColour(255, 255, 255));
    userStatusText->Refresh();

    std::string result = apiModule->ValidateAndGetStatusOfAuthToken(this, token);

    tokenInfoText->SetLabelText(
        "UserID: " + apiModule->lastDecodedData.UserId + "\n" +
        "Nickname: " + apiModule->lastDecodedData.Nickname + "\n" +
        "Token issuance date: " + apiModule->lastDecodedData.IssuanceTime + "\n" +
        "Token expiration date: " + apiModule->lastDecodedData.ExpirationTime + "\n"
    );
    tokenInfoText->Refresh();

    userStatusText->SetLabelText(result);
    userStatusText->SetForegroundColour(apiModule->lastDecodedData.StatusColor);
    userStatusText->Refresh();

    clipboardButton->Enable(true);
    scanButton->Enable(true);
    copyButton->Enable(true);
}

auto MainFrame::OnTokenFromClipboard(wxCommandEvent& event) -> void
{
    if (!wxTheClipboard->Open())
        return;

    if (!wxTheClipboard->IsSupported(wxDF_TEXT))
    {
        wxTheClipboard->Close();
        return;
    }

    wxTextDataObject data;
    wxTheClipboard->GetData(data);

    std::string result = data.GetText().ToStdString();

    wxTheClipboard->Close();

    ValidateAndDisplay(result);
}

auto MainFrame::OnScanQrCodeFromScreen(wxCommandEvent& event) -> void
{
    Show(false);

    std::string result = qrModule->ScanScreensForCode();

    Show(true);

    ValidateAndDisplay(result);
}

auto MainFrame::OnCopyUserId(wxCommandEvent& event) -> void
{
    if (apiModule->lastDecodedData.UserId == "")
        return;

    if (!wxTheClipboard->Open())
        return;

    wxTheClipboard->SetData(new wxTextDataObject(apiModule->lastDecodedData.UserId));
    wxTheClipboard->Close();
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
