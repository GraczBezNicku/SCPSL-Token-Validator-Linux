#include "QrModule.hpp"

#include <filesystem>

#include "opencv2/opencv.hpp"

QrModule::QrModule()
{

}

auto QrModule::ScanScreensForCode() -> std::string
{
    std::string tempFile = "codeCapture.png";

    if (std::filesystem::exists(tempFile))
    {
        std::filesystem::remove(tempFile);
    }

    int status = -1;

    status = std::system(("gnome-screenshot -f " + tempFile + " 2>/dev/null").c_str());

    if (status != 0)
    {
        status = std::system(("spectacle -b -n -o " + tempFile + " 2>/dev/null").c_str());
    }

    if (status != 0)
    {
        status = std::system(("grim " + tempFile + " 2>/dev/null").c_str());
    }

    if (!std::filesystem::exists(tempFile))
    {
        return "Could not capture screenshot. Insufficient permissions?";
    }

    cv::Mat image = cv::imread(tempFile);

    std::filesystem::remove(tempFile);

    if (image.empty())
    {
        return "Could not process captured image.";
    }

    cv::QRCodeDetector qrDecoder;
    std::vector<cv::Point> points;
    std::string qrData = qrDecoder.detectAndDecode(image, points);

    if (!qrData.empty())
    {
        return qrData;
    }

    return "No QR code was detected.";
}
