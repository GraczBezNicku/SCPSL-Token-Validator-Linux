#pragma once

#include <string>

class QrModule
{
public:
    QrModule();

    auto ScanScreensForCode(std::string& error) -> std::string;
};
