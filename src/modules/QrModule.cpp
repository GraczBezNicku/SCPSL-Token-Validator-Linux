#include "QrModule.hpp"

#include "wx/image.h"
#include "ZXing/ReadBarcode.h"

#include <filesystem>

QrModule::QrModule()
{

}

auto QrModule::ScanScreensForCode(std::string& error) -> std::string
{
    std::string tempFile = "codeCapture.bmp";

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
        error = "Could not capture screenshot. Insufficient permissions?";
        return "";
    }

    wxImage originalImage;

    if (!originalImage.LoadFile(tempFile, wxBITMAP_TYPE_BMP))
    {
        error = "Could not open screenshot as wxImage.";
        return "";
    }

    wxImage grayscaleImage = originalImage.ConvertToGreyscale();

    int width = grayscaleImage.GetWidth();
    int height = grayscaleImage.GetHeight();
    std::vector<uint8_t> luminanceBuffer(width * height);

    unsigned char* rgbData = grayscaleImage.GetData();

    for (int i = 0; i < width * height; ++i)
    {
        // Covnert wxImage's grayscale into 1 byte per pixel for ZXing
        uint8_t grayValue = rgbData[i * 3];

        // Cutting out artifacts
        luminanceBuffer[i] = (grayValue > 128) ? 255 : 0;
    }

    std::filesystem::remove(tempFile);

    ZXing::ImageView zxImage = ZXing::ImageView(luminanceBuffer.data(), width, height, ZXing::ImageFormat::Lum);
    ZXing::Results codes = ZXing::ReadBarcodes(zxImage, {});

    if (!codes.empty())
    {
        return codes[0].text();
    }

    error = "No QR code was detected.";
    return "";
}
