#include "Img.hpp"
#include <string>
#include <vector>
#include <stdexcept>
#include <png.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <limits>

// ifstreamを使ったPNGデータ読み込み用のコールバック関数
void read_png_from_ifstream(png_structp png_ptr, png_bytep data, png_size_t length) {
    std::ifstream* stream = static_cast<std::ifstream*>(png_get_io_ptr(png_ptr));
    if (!stream->read(reinterpret_cast<char*>(data), length)) {
        png_error(png_ptr, "Read error");
    }
}

// ofstreamを使ったPNGデータ書き込み用のコールバック関数
void write_png_to_ofstream(png_structp png_ptr, png_bytep data, png_size_t length) {
    std::ofstream* stream = static_cast<std::ofstream*>(png_get_io_ptr(png_ptr));
    if (!stream->write(reinterpret_cast<char*>(data), length)) {
        png_error(png_ptr, "Write error");
    }
}

// ofstreamのフラッシュ用コールバック関数
void flush_png_to_ofstream(png_structp png_ptr) {
    std::ofstream* stream = static_cast<std::ofstream*>(png_get_io_ptr(png_ptr));
    stream->flush();
}

Img read_PNG(const std::string& filename) {
    // PNGファイルを開く
    std::ifstream stream(filename, std::ios::binary);
    if (!stream) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    // PNGファイルのシグネチャを確認
    png_byte header[8];
    if (!stream.read(reinterpret_cast<char*>(header), 8) || png_sig_cmp(header, 0, 8)) {
        throw std::runtime_error("Invalid PNG signature in file: " + filename);
    }
    
    // png_structとpng_infoの初期化
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        throw std::runtime_error("Failed to create PNG read struct");
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        throw std::runtime_error("Failed to create PNG info struct");
    }
    
    // エラーハンドリングのためのセットアップ
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        throw std::runtime_error("Error during PNG reading process");
    }
    
    // PNGファイルの読み込み開始（ifstreamを使用）
    png_set_read_fn(png_ptr, &stream, read_png_from_ifstream);
    png_set_sig_bytes(png_ptr, 8);
    
    // PNGファイルの情報を読み込む
    png_read_info(png_ptr, info_ptr);
    
    // 画像情報を取得
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);
    
    // 16ビット深度の処理 (16ビットをそのまま保持する)
    bool is_16bit = (bit_depth == 16);
    
    // パレットカラー -> RGB
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    
    // グレースケール（ビット深度 < 8） -> 8ビット
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    
    // アルファチャンネル -> フル不透明度
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }
    
    // チャンネル数を取得
    int channels = png_get_channels(png_ptr, info_ptr);
    
    // アルファチャンネルがあるかどうか確認
    bool has_alpha = (color_type & PNG_COLOR_MASK_ALPHA) || png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    
    // グレースケールと色情報があるかどうか確認
    bool is_rgb = (color_type & PNG_COLOR_MASK_COLOR);
    
    // 実際のチャンネル数を計算
    if (is_rgb) {
        channels = has_alpha ? 4 : 3; // RGBA or RGB
    } else {
        channels = has_alpha ? 2 : 1; // Grayscale with alpha or just grayscale
    }
    
    // 変換を適用
    png_read_update_info(png_ptr, info_ptr);
    
    // 行ポインタの配列を確保
    std::vector<std::unique_ptr<png_byte[]>> row_data(height);
    std::vector<png_bytep> row_pointers(height);
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_data[y] = std::make_unique<png_byte[]>(png_get_rowbytes(png_ptr, info_ptr));
        row_pointers[y] = row_data[y].get();
    }
    
    // 画像データを読み込む
    png_read_image(png_ptr, row_pointers.data());
    
    // 各チャンネルを縦方向に連結した画像データを作成
    int total_height = height * channels;
    
    // Imgオブジェクトを作成
    Img result(total_height, width);
    
    // データを直接書き込む
    size_t idx = 0;
    for (int ch = 0; ch < channels; ch++) {
        for (png_uint_32 y = 0; y < height; y++) {
            for (png_uint_32 x = 0; x < width; x++) {
                if (is_16bit) {
                    // 16ビットデータの場合 (2バイトで1ピクセル)
                    png_uint_16 pixel_value = 0;
                    // バイトオーダーを考慮して16ビット値を取得
                    size_t pos = x * channels * 2 + ch * 2; // 16ビットなので各チャンネルは2バイト
                    pixel_value = (row_pointers[y][pos] << 8) | row_pointers[y][pos + 1];
                    result.data[idx++] = static_cast<double>(pixel_value);
                } else {
                    // 8ビットデータの場合
                    result.data[idx++] = static_cast<double>(row_pointers[y][x * channels + ch]);
                }
            }
        }
    }
    
    // PNGリソースの解放
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return result;
}

void write_PNG(const Img& img, const std::string& filename, int channels, bool use_16bit) {
    // 画像の高さがチャンネル数で割り切れるか確認
    if (img.getHeight() % channels != 0) {
        throw std::runtime_error("Image height is not divisible by the number of channels");
    }
    
    // 実際の画像の高さを計算
    int actual_height = img.getHeight() / channels;
    int width = img.getWidth();
    
    // ファイル開く
    std::ofstream stream(filename, std::ios::binary);
    if (!stream) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    
    // PNGライブラリの初期化
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        throw std::runtime_error("Failed to create PNG write struct");
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        throw std::runtime_error("Failed to create PNG info struct");
    }
    
    // エラーハンドリングのためのセットアップ
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        throw std::runtime_error("Error during PNG writing process");
    }
    
    // PNGファイルの書き込み開始（ofstreamを使用）
    png_set_write_fn(png_ptr, &stream, write_png_to_ofstream, flush_png_to_ofstream);
    
    // 画像の種類を設定
    int color_type;
    switch (channels) {
        case 1:
            color_type = PNG_COLOR_TYPE_GRAY;
            break;
        case 2:
            color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
            break;
        case 3:
            color_type = PNG_COLOR_TYPE_RGB;
            break;
        case 4:
            color_type = PNG_COLOR_TYPE_RGB_ALPHA;
            break;
        default:
            png_destroy_write_struct(&png_ptr, &info_ptr);
            throw std::runtime_error("Unsupported number of channels: " + std::to_string(channels));
    }
    
    // ビット深度の設定
    int bit_depth = use_16bit ? 16 : 8;
    
    // PNGヘッダ情報を設定
    png_set_IHDR(png_ptr, info_ptr, width, actual_height, bit_depth, color_type,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    // 行ポインタの配列を確保
    size_t row_bytes = width * channels * (use_16bit ? 2 : 1);
    std::vector<std::unique_ptr<png_byte[]>> row_data(actual_height);
    std::vector<png_bytep> row_pointers(actual_height);
    
    for (int y = 0; y < actual_height; y++) {
        row_data[y] = std::make_unique<png_byte[]>(row_bytes);
        row_pointers[y] = row_data[y].get();
    }
    
    // Imgオブジェクトからデータをコピー
    double min_val = std::numeric_limits<double>::max();
    double max_val = std::numeric_limits<double>::lowest();
    
    // 最小値と最大値を見つける（必要に応じて正規化するため）
    if (!use_16bit) {
        // 8ビットモードの場合、データ範囲が8ビット(0-255)を超えているかチェック
        for (int i = 0; i < img.getHeight() * img.getWidth(); i++) {
            min_val = std::min(min_val, img.data[i]);
            max_val = std::max(max_val, img.data[i]);
        }
    }
    
    // データ範囲が0-255を超えているかどうか
    bool need_scaling = (min_val < 0 || max_val > 255) && !use_16bit;
    
    // 画像データをrow_pointersにコピー
    for (int ch = 0; ch < channels; ch++) {
        for (int y = 0; y < actual_height; y++) {
            for (int x = 0; x < width; x++) {
                // 元画像のインデックス
                size_t src_idx = (ch * actual_height + y) * width + x;
                
                double value = img.data[src_idx];
                
                if (use_16bit) {
                    // 16ビットデータの場合
                    png_uint_16 pix_val;
                    
                    // 値の範囲を確認
                    if (value < 0) value = 0;
                    if (value > 65535) value = 65535;
                    
                    pix_val = static_cast<png_uint_16>(std::round(value));
                    
                    // ビッグエンディアンで書き込み
                    size_t dst_pos = x * channels * 2 + ch * 2;
                    row_pointers[y][dst_pos] = (pix_val >> 8) & 0xFF; // 上位バイト
                    row_pointers[y][dst_pos + 1] = pix_val & 0xFF;    // 下位バイト
                } else {
                    // 8ビットデータの場合
                    png_byte pix_val;
                    
                    // 必要に応じてスケーリング
                    if (need_scaling) {
                        // 0-255の範囲に正規化
                        value = 255.0 * (value - min_val) / (max_val - min_val);
                    }
                    
                    // 値の範囲を確認
                    if (value < 0) value = 0;
                    if (value > 255) value = 255;
                    
                    pix_val = static_cast<png_byte>(std::round(value));
                    row_pointers[y][x * channels + ch] = pix_val;
                }
            }
        }
    }
    
    // PNGヘッダを書き込み
    png_write_info(png_ptr, info_ptr);
    
    // 16ビットデータの場合、ネットワークバイトオーダー（ビッグエンディアン）に変換
    if (use_16bit) {
        png_set_swap(png_ptr);
    }
    
    // 画像データを書き込み
    png_write_image(png_ptr, row_pointers.data());
    
    // PNGフッターを書き込み
    png_write_end(png_ptr, nullptr);
    
    // リソースの解放
    png_destroy_write_struct(&png_ptr, &info_ptr);
}