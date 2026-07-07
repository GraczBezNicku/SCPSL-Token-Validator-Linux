#pragma once

#include "wx/wx.h"

#include <string>

struct DecodedData
{
    wxColour StatusColor = wxColour(255, 255, 255);
    std::string UserId = "";
    std::string Nickname = "";
    std::string IssuanceTime = "";
    std::string ExpirationTime = "";
};

class MainFrame;

class ApiModule
{
public:
    ApiModule();

    DecodedData lastDecodedData = DecodedData();

    auto RefreshToken() -> void;
    auto ReplaceToken(const std::string& newToken) -> void;

    auto GetAuthenticationStatus() -> std::string;
    auto ValidateAndGetStatusOfAuthToken(MainFrame* frame, const std::string& authToken) -> std::string;

    auto FormUrlEncode(const wxString& value) -> wxString;
private:
    std::string apiToken = "";
};
