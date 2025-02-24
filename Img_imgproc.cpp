#include "Img.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>

Img Img::trim(unsigned int top, unsigned int right, unsigned int bottom, unsigned int left) {
    // 境界チェック
    if (top + bottom >= static_cast<unsigned int>(height) || 
        left + right >= static_cast<unsigned int>(width)) {
        std::cerr << "Error: Trim parameters exceed image dimensions" << std::endl;
        exit(1);
    }

    // 新しい画像サイズを計算
    int new_height = height - (top + bottom);
    int new_width = width - (left + right);

    // 新しい画像を作成
    Img result(new_height, new_width);

    // データをコピー
    for (int h = 0; h < new_height; ++h) {
        for (int w = 0; w < new_width; ++w) {
            result[h][w] = rows[h + top][w + left];
        }
    }

    return result;
}

Img Img::clip(double lower_th, double upper_th) {
    if (lower_th > upper_th) {
        std::cerr << "Error: Lower threshold must be less than or equal to upper threshold" << std::endl;
        exit(1);
    }

    Img result(height, width);

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            double val = rows[h][w];
            if (val < lower_th) {
                result[h][w] = lower_th;
            } else if (val > upper_th) {
                result[h][w] = upper_th;
            } else {
                result[h][w] = val;
            }
        }
    }

    return result;
}

Img Img::clip_lower(double lower_th) {
    Img result(height, width);

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            result[h][w] = std::max(rows[h][w], lower_th);
        }
    }

    return result;
}

Img Img::clip_higher(double higher_th) {
    Img result(height, width);

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            result[h][w] = std::min(rows[h][w], higher_th);
        }
    }

    return result;
}