#include "ApiModule.hpp"

#include <fstream>

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

auto ApiModule::ValidateAndGetStatusOfAuthToken(const std::string& authToken) -> std::string
{
    return "Auth Token validation not implemented.";
}
