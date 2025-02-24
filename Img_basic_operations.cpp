#include "Img.hpp"
#include <iostream>
#include <cstdlib>

// 単項-
Img Img::operator-() const {
    Img result(height, width);
    for (int h = 0; h < height; h++) {
        double* result_row = result.rows[h];
        const double* this_row = rows[h];
        for (int w = 0; w < width; w++) {
            result_row[w] = -this_row[w];
        }
    }
    return result;
}

// 加算演算子
Img Img::operator+(const Img& other) const {
    // サイズが完全に一致する場合
    if (height == other.height && width == other.width) {
        Img result(height, width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            const double* other_row = other.rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] + other_row[w];
            }
        }
        return result;
    }
    // 高さが1で幅が一致する場合（ブロードキャスト）
    if (height == 1 && width == other.width) {
        Img result(other.height, width);
        const double* this_row = rows[0];
        for (int h = 0; h < other.height; h++) {
            double* result_row = result.rows[h];
            const double* other_row = other.rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] + other_row[w];
            }
        }
        return result;
    }
    if (other.height == 1 && width == other.width) {
        Img result(height, width);
        const double* other_row = other.rows[0];
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] + other_row[w];
            }
        }
        return result;
    }
    // 幅が1で高さが一致する場合（ブロードキャスト）
    if (width == 1 && height == other.height) {
        Img result(height, other.width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double this_val = rows[h][0];
            const double* other_row = other.rows[h];
            for (int w = 0; w < other.width; w++) {
                result_row[w] = this_val + other_row[w];
            }
        }
        return result;
    }
    if (other.width == 1 && height == other.height) {
        Img result(height, width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            const double other_val = other.rows[h][0];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] + other_val;
            }
        }
        return result;
    }
    
    // サイズが一致しない場合はエラーメッセージを出力して終了
    std::cerr << "Error: Incompatible dimensions for addition operation." << std::endl;
    std::cerr << "First image: " << height << "x" << width << std::endl;
    std::cerr << "Second image: " << other.height << "x" << other.width << std::endl;
    std::exit(1);
}

// 減算演算子
Img Img::operator-(const Img& other) const {
    // サイズが完全に一致する場合
    if (height == other.height && width == other.width) {
        Img result(height, width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            const double* other_row = other.rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] - other_row[w];
            }
        }
        return result;
    }
    // 高さが1で幅が一致する場合（ブロードキャスト）
    if (height == 1 && width == other.width) {
        Img result(other.height, width);
        const double* this_row = rows[0];
        for (int h = 0; h < other.height; h++) {
            double* result_row = result.rows[h];
            const double* other_row = other.rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] - other_row[w];
            }
        }
        return result;
    }
    if (other.height == 1 && width == other.width) {
        Img result(height, width);
        const double* other_row = other.rows[0];
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] - other_row[w];
            }
        }
        return result;
    }
    // 幅が1で高さが一致する場合（ブロードキャスト）
    if (width == 1 && height == other.height) {
        Img result(height, other.width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double this_val = rows[h][0];
            const double* other_row = other.rows[h];
            for (int w = 0; w < other.width; w++) {
                result_row[w] = this_val - other_row[w];
            }
        }
        return result;
    }
    if (other.width == 1 && height == other.height) {
        Img result(height, width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            const double other_val = other.rows[h][0];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] - other_val;
            }
        }
        return result;
    }
    
    // サイズが一致しない場合はエラーメッセージを出力して終了
    std::cerr << "Error: Incompatible dimensions for addition operation." << std::endl;
    std::cerr << "First image: " << height << "x" << width << std::endl;
    std::cerr << "Second image: " << other.height << "x" << other.width << std::endl;
    std::exit(1);
}

Img Img::operator*(const Img& other) const {
    // サイズが完全に一致する場合
    if (height == other.height && width == other.width) {
        Img result(height, width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            const double* other_row = other.rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] * other_row[w];
            }
        }
        return result;
    }
    // 高さが1で幅が一致する場合（ブロードキャスト）
    if (height == 1 && width == other.width) {
        Img result(other.height, width);
        const double* this_row = rows[0];
        for (int h = 0; h < other.height; h++) {
            double* result_row = result.rows[h];
            const double* other_row = other.rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] * other_row[w];
            }
        }
        return result;
    }
    if (other.height == 1 && width == other.width) {
        Img result(height, width);
        const double* other_row = other.rows[0];
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] * other_row[w];
            }
        }
        return result;
    }
    // 幅が1で高さが一致する場合（ブロードキャスト）
    if (width == 1 && height == other.height) {
        Img result(height, other.width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double this_val = rows[h][0];
            const double* other_row = other.rows[h];
            for (int w = 0; w < other.width; w++) {
                result_row[w] = this_val * other_row[w];
            }
        }
        return result;
    }
    if (other.width == 1 && height == other.height) {
        Img result(height, width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            const double other_val = other.rows[h][0];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] * other_val;
            }
        }
        return result;
    }
    
    // サイズが一致しない場合はエラーメッセージを出力して終了
    std::cerr << "Error: Incompatible dimensions for addition operation." << std::endl;
    std::cerr << "First image: " << height << "x" << width << std::endl;
    std::cerr << "Second image: " << other.height << "x" << other.width << std::endl;
    std::exit(1);
}



Img Img::operator/(const Img& other) const {
    // サイズが完全に一致する場合
    if (height == other.height && width == other.width) {
        Img result(height, width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            const double* other_row = other.rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] / other_row[w];
            }
        }
        return result;
    }
    // 高さが1で幅が一致する場合（ブロードキャスト）
    if (height == 1 && width == other.width) {
        Img result(other.height, width);
        const double* this_row = rows[0];
        for (int h = 0; h < other.height; h++) {
            double* result_row = result.rows[h];
            const double* other_row = other.rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] / other_row[w];
            }
        }
        return result;
    }
    if (other.height == 1 && width == other.width) {
        Img result(height, width);
        const double* other_row = other.rows[0];
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] / other_row[w];
            }
        }
        return result;
    }
    // 幅が1で高さが一致する場合（ブロードキャスト）
    if (width == 1 && height == other.height) {
        Img result(height, other.width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double this_val = rows[h][0];
            const double* other_row = other.rows[h];
            for (int w = 0; w < other.width; w++) {
                result_row[w] = this_val / other_row[w];
            }
        }
        return result;
    }
    if (other.width == 1 && height == other.height) {
        Img result(height, width);
        for (int h = 0; h < height; h++) {
            double* result_row = result.rows[h];
            const double* this_row = rows[h];
            const double other_val = other.rows[h][0];
            for (int w = 0; w < width; w++) {
                result_row[w] = this_row[w] / other_val;
            }
        }
        return result;
    }
    
    // サイズが一致しない場合はエラーメッセージを出力して終了
    std::cerr << "Error: Incompatible dimensions for addition operation." << std::endl;
    std::cerr << "First image: " << height << "x" << width << std::endl;
    std::cerr << "Second image: " << other.height << "x" << other.width << std::endl;
    std::exit(1);
}


Img Img::operator+(double scalar) const {
    Img result(height, width);
    for (int h = 0; h < height; h++) {
        double* result_row = result.rows[h];
        const double* this_row = rows[h];
        for (int w = 0; w < width; w++) {
            result_row[w] = this_row[w] + scalar;
        }
    }
    return result;
}
Img operator+(double scalar, const Img& img) {
    return img + scalar;
}

Img Img::operator-(double scalar) const {
    Img result(height, width);
    for (int h = 0; h < height; h++) {
        double* result_row = result.rows[h];
        const double* this_row = rows[h];
        for (int w = 0; w < width; w++) {
            result_row[w] = this_row[w] - scalar;
        }
    }
    return result;
}

// スカラー値との減算演算子（スカラー - 画像）
Img operator-(double scalar, const Img& img) {
    Img result(img.getHeight(), img.getWidth());
    for (int h = 0; h < img.getHeight(); h++) {
        double* result_row = result[h];
        const double* img_row = img[h];
        for (int w = 0; w < img.getWidth(); w++) {
            result_row[w] = scalar - img_row[w];
        }
    }
    return result;
}

// スカラー値との乗算演算子（画像 * スカラー）
Img Img::operator*(double scalar) const {
    Img result(height, width);
    for (int h = 0; h < height; h++) {
        double* result_row = result.rows[h];
        const double* this_row = rows[h];
        for (int w = 0; w < width; w++) {
            result_row[w] = this_row[w] * scalar;
        }
    }
    return result;
}

// スカラー値との乗算演算子（スカラー * 画像）
Img operator*(double scalar, const Img& img) {
    return img * scalar;  // 既存の演算子を再利用
}

// スカラー値との除算演算子（画像 / スカラー）
Img Img::operator/(double scalar) const {
    if (scalar == 0.0) {
        std::cerr << "Error: Division by zero scalar." << std::endl;
        std::exit(1);
    }
    Img result(height, width);
    for (int h = 0; h < height; h++) {
        double* result_row = result.rows[h];
        const double* this_row = rows[h];
        for (int w = 0; w < width; w++) {
            result_row[w] = this_row[w] / scalar;
        }
    }
    return result;
}

// スカラー値との除算演算子（スカラー / 画像）
Img operator/(double scalar, const Img& img) {
    Img result(img.getHeight(), img.getWidth());
    bool zero_division = false;
    for (int h = 0; h < img.getHeight(); h++) {
        double* result_row = result[h];
        const double* img_row = img[h];
        for (int w = 0; w < img.getWidth(); w++) {
            if (img_row[w] == 0.0) {
                std::cerr << "Error: Division by zero at position (" << h << ", " << w << ")." << std::endl;
                std::exit(1);
            }
            result_row[w] = scalar / img_row[w];
        }
    }
    return result;
}

// 要素ごとの平方根を計算
Img Img::sqrt() const {
    Img result(height, width);    
    for (int h = 0; h < height; h++) {
        double* result_row = result.rows[h];
        const double* this_row = rows[h];
        for (int w = 0; w < width; w++) {
            if (this_row[w] < 0) {
                std::cerr << "Error: Negative value found during square root operation (" << h << ", " << w << ")." << std::endl;
                std::exit(1);
            } else {
                result_row[w] = std::sqrt(this_row[w]);
            }
        }
    }
    return result;
}