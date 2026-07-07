#pragma once

#include <string>

class ApiModule
{
public:
    ApiModule();

    auto RefreshToken() -> void;
    auto ReplaceToken(std::string newToken) -> void;

    auto GetAuthenticationStatus() -> std::string;
    auto ValidateAndGetStatusOfAuthToken(std::string authToken) -> std::string;
private:
    std::string apiToken = "";
};
