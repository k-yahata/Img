#ifndef IMG_HPP
#define IMG_HPP

class Img {
private:
    int height;      // 画像の高さ
    int width;       // 画像の幅
    double* data;    // 画像データの先頭ポインタ
    double** rows;   // 各行の先頭ポインタの配列

    // メモリ管理用の関数
    void releaseMemory();
    void allocateMemory();

public:
    // コンストラクタ（デフォルト値は0）
    Img(int h = 0, int w = 0);
    Img(int h, int w, const std::vector<double>& src_data);
    
    // コピーコンストラクタ
    Img(const Img& other);
    
    // デストラクタ
    ~Img();
    
    // コピー代入演算子
    Img& operator=(const Img& other);
    
    // 行アクセス演算子（インライン実装）
    inline double* operator[](int h) { return rows[h]; }
    inline const double* operator[](int h) const { return rows[h]; }

    // アクセサ
    int getHeight() const { return height; }
    int getWidth() const { return width; }

    // 単項マイナス演算子（負号）
    Img operator-() const;

    // 画像同士の算術演算子
    Img operator+(const Img& other) const;
    Img operator-(const Img& other) const;
    Img operator*(const Img& other) const;
    Img operator/(const Img& other) const;
    
    // スカラー値との算術演算子（メンバ関数）
    Img operator+(double scalar) const;
    Img operator-(double scalar) const;
    Img operator*(double scalar) const;
    Img operator/(double scalar) const;
    
    // Other arithmetic operations
    Img sqrt() const;

    // Image Processing
    // Img trim(unsigned int top, unsigned int right, unsigned int bottom, unsigned int left);
    // Img clip(double lower_th, double upper_th);
    // Img clip_lower(double lower_th);
    // Img clio_higher(double higher_th);

    // statistics
    double min() const;
    double max() const;
    void minmax(double& min, double& max) const;
    double var() const;

};

// スカラー値との算術演算子（フリー関数）
Img operator+(double scalar, const Img& img);
Img operator-(double scalar, const Img& img);
Img operator*(double scalar, const Img& img);
Img operator/(double scalar, const Img& img);

#endif // IMG_HPP