#include "Img.hpp"
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <filesystem>

// 前方宣言（読み込み関数）
Img read_PNG(const std::string& filename);
Img read_TIFF(const std::string& filename);
Img read_BMP(const std::string& filename);

// 前方宣言（書き込み関数）
void write_PNG(const Img& img, const std::string& filename, int channels, bool use_16bit);
void write_TIFF(const Img& img, const std::string& filename, int channels, bool use_16bit);
void write_BMP(const Img& img, const std::string& filename, int channels);

// ファイル拡張子を小文字で取得する関数
std::string getFileExtension(const std::string& filename) {
    std::string extension = std::filesystem::path(filename).extension().string();
    
    // 拡張子が空の場合
    if (extension.empty()) {
        return "";
    }
    
    // 先頭の'.'を削除
    extension = extension.substr(1);
    
    // 小文字に変換
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    return extension;
}

// 読み込み関数
Img Img::read(const std::string& filename) {
    std::string extension = getFileExtension(filename);
    
    if (extension == "png") {
        return read_PNG(filename);
    } else if (extension == "tif" || extension == "tiff") {
        return read_TIFF(filename);
    } else if (extension == "bmp") {
        return read_BMP(filename);
    } else {
        throw std::runtime_error("Unsupported file format for reading: " + extension);
    }
}

// 書き込み関数
void Img::write(const std::string& filename, int channels, bool use_16bit) const {
    std::string extension = getFileExtension(filename);
    
    if (extension == "png") {
        write_PNG(*this, filename, channels, use_16bit);
    } else if (extension == "tif" || extension == "tiff") {
        write_TIFF(*this, filename, channels, use_16bit);
    } else if (extension == "bmp") {
        // BMPは16ビット形式をサポートしていないため、use_16bitパラメータは無視
        write_BMP(*this, filename, channels);
    } else {
        throw std::runtime_error("Unsupported file format for writing: " + extension);
    }
}
