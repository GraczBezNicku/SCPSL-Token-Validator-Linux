#include "ApiModule.hpp"
#include "../frames/MainFrame.hpp"

#include <fstream>
#include <string>

#include "wx/wx.h"
#include "wx/protocol/http.h"
#include "wx/sstream.h"
#include "wx/uri.h"
#include "wx/base64.h"

#include "nlohmann/json.hpp"

ApiModule::ApiModule()
{
    RefreshToken();
}

auto ApiModule::RefreshToken() -> void
{
    std::ifstream tokenFile ("token.txt");

    if (tokenFile.is_open())
    {
        std::string readLine;

        if (getline(tokenFile, readLine))
        {
            apiToken = readLine;
        }

        tokenFile.close();
    }
}

auto ApiModule::ReplaceToken(const std::string& newToken) -> void
{
    std::remove("token.txt");

    std::ofstream tokenFile ("token.txt");

    tokenFile << newToken;

    tokenFile.close();
}

auto ApiModule::GetAuthenticationStatus() -> std::string
{
    if (apiToken == "")
        return "Not authenticated, using API as guest.";

    return "Authenticated using staff API token.";
}

auto ApiModule::ValidateAndGetStatusOfAuthToken(MainFrame* frame, const std::string& authToken) -> std::string
{
    lastDecodedData = DecodedData();

    wxString escapedAuth = FormUrlEncode(authToken);
    wxString escapedToken = FormUrlEncode(apiToken);

    wxString postData = wxString::Format("auth=%s", escapedAuth);
    if (apiToken != "")
    {
        postData += "&token=" + escapedToken;
    }

    wxString command = wxString::Format(
        "curl --silent --fail-with-body -L -X POST -d '%s' 'https://api.scpslgame.com/v5/tools/validatetoken.php'",
        postData
    );

    wxArrayString output;
    wxArrayString errors;

    long exitCode = wxExecute(command, output, errors, wxEXEC_SYNC | wxEXEC_NODISABLE);

    if (exitCode != 0)
    {
        lastDecodedData.StatusColor = wxColour(140, 0, 0);
        return "Web request failed.";
    }

    wxString responseText;
    for (size_t i = 0; i < output.GetCount(); ++i)
    {
        responseText += output[i];
        if (i < output.GetCount() - 1)
        {
            responseText += "\n";
        }
    }

    try
    {
        nlohmann::json data = nlohmann::json::parse(responseText.ToStdString());

        if (data["success"] == false)
        {
            lastDecodedData.StatusColor = wxColour(140, 0, 0);
            return "Error: " + to_string(data["error"]);
        }

        if (data["verified"] == false)
        {
            lastDecodedData.StatusColor = wxColour(140, 0, 0);
            return "Digital signature invalid.";
        }

        lastDecodedData.UserId = data["UserID"];
        std::string rawNickname = data["Nickname"];
        lastDecodedData.Nickname = wxBase64Decode(wxString(rawNickname));
        lastDecodedData.IssuanceTime = data["Issuance time"];
        lastDecodedData.ExpirationTime = data["Expiration time"];

        if (!data.contains("clean") || !data.contains("GlobalBan"))
        {
            if (data["newToken"] == false)
            {
                lastDecodedData.StatusColor = wxColour(102, 205, 170);
                return "Signature verification successful, token is old.";
            }

            lastDecodedData.StatusColor = wxColour(0, 191, 255);
            return "Signature verification successful.";
        }

        if (data["clean"] == true)
        {
            if (data["newToken"] == false)
            {
                lastDecodedData.StatusColor = wxColour(102, 205, 170);
                return "Signature verification successful, not banned in any game, token is old.";
            }

            lastDecodedData.StatusColor = wxColour(0, 128, 128);
            return "Signature verification successful, not banned in any game.";
        }

        if (data["GlobalBan"] == true)
        {
            if (data["newToken"] == false)
            {
                lastDecodedData.StatusColor = wxColour(255, 69, 0);
                return "Signature verification successful, banned in SCP:SL, token is old.";
            }

            lastDecodedData.StatusColor = wxColour(255, 69, 0);
            return "Signature verification successful, banned in SCP:SL.";
        }

        if (data["newToken"] == false)
        {
            lastDecodedData.StatusColor = wxColour(255, 140, 0);
            return "Signature verification successful, banned in other games, token is old.";
        }

        lastDecodedData.StatusColor = wxColour(255, 165, 0);
        return "Signature verification successful, banned in other games.";
    }
    catch (std::exception ex)
    {
        lastDecodedData.StatusColor = wxColour(140, 0, 0);
        return "Token validation failed (exception).";
    }
}

auto ApiModule::FormUrlEncode(const wxString& value) -> wxString
{
    std::ostringstream escaped;
    escaped << std::hex;

    std::string stdStr = value.ToStdString(wxConvUTF8);

    for (char c : stdStr)
    {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
        }
        else if (c == ' ')
        {
            escaped << '+';
        }
        else
        {
            escaped << '%' << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(c));
        }
    }

    return wxString::FromUTF8(escaped.str());
}
