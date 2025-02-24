#include "Img.hpp"
#include <cstring>
#include <algorithm>
#include <iostream>

// メモリ解放関数
void Img::releaseMemory() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
    if (rows != nullptr) {
        delete[] rows;
        rows = nullptr;
    }
}

// メモリ確保関数（行ポインタの設定を含む）
void Img::allocateMemory() {
    if (height > 0 && width > 0) {
        data = new double[height * width];
        rows = new double*[height];
        
        // 行ポインタの設定
        for (int i = 0; i < height; i++) {
            rows[i] = &data[i * width];
        }
    }
}

// コンストラクタ
Img::Img(int h, int w) : height(h), width(w), data(nullptr), rows(nullptr) {
    // 負の値のチェック
    if (h < 0 || w < 0) {
        std::cerr << "Warning: Negative dimensions (h=" << h << ", w=" << w 
                  << "). Both dimensions will be set to 0." << std::endl;
        height = 0;
        width = 0;
        return;
    }
    
    // デフォルト値（0）の場合はメモリ確保をスキップ
    if (height == 0 || width == 0) {
        height = 0;
        width = 0;
        return;
    }

    allocateMemory();
}

Img::Img(int h, int w, const std::vector<double>& src_data)
    : height(h), width(w), data(nullptr), rows(nullptr) {
    
    // パラメータの検証
    if (h <= 0 || w <= 0) {
        std::cerr << "Height and width must be positive" << std::endl;
        exit(1);
    }
    if (src_data.size() < static_cast<size_t>(h * w)) {
        std::cerr << "Source data pointer cannot be nullptr" << std::endl;
        exit(1);
    }

    // メモリの確保
    allocateMemory();
    
    // データのコピー
    std::memcpy(data, src_data.data(), height * width * sizeof(double));
}

// コピーコンストラクタ
Img::Img(const Img& other) : height(other.height), width(other.width), data(nullptr), rows(nullptr) {
    allocateMemory();
    if (data != nullptr) {
        std::memcpy(data, other.data, height * width * sizeof(double));
    }
}

// デストラクタ
Img::~Img() {
    releaseMemory();
}

// コピー代入演算子
Img& Img::operator=(const Img& other) {
    if (this != &other) {  // 自己代入チェック
        // 既存のメモリを解放
        releaseMemory();
        
        // 新しいサイズを設定
        height = other.height;
        width = other.width;
        
        // 新しいメモリを確保してデータをコピー
        allocateMemory();
        if (data != nullptr) {
            std::memcpy(data, other.data, height * width * sizeof(double));
        }
    }
    return *this;
}