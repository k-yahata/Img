#include "Img.hpp"
#include <limits>
#include <iostream>
#include <cstdlib>

double Img::min() const {
    if (height == 0 || width == 0) {
        std::cerr << "Error: Empty image" << std::endl;
        exit(1);
    }

    const int size = height * width;
    double minVal = std::numeric_limits<double>::max();
    
    for (int i = 0; i < size; ++i) {
        if (data[i] < minVal) {
            minVal = data[i];
        }
    }
    
    return minVal;
}

double Img::max() const {
    if (height == 0 || width == 0) {
        std::cerr << "Error: Empty image" << std::endl;
        exit(1);
    }

    const int size = height * width;
    double maxVal = std::numeric_limits<double>::lowest();
    
    for (int i = 0; i < size; ++i) {
        if (data[i] > maxVal) {
            maxVal = data[i];
        }
    }
    
    return maxVal;
}

double Img::var() const {
    if (height == 0 || width == 0) {
        std::cerr << "Error: Empty image" << std::endl;
        exit(1);
    }

    const int size = height * width;
    double sum = 0.0;          // 合計値
    double squareSum = 0.0;    // 二乗の合計値
    
    for (int i = 0; i < size; ++i) {
        sum += data[i];
        squareSum += data[i] * data[i];
    }

    const double mean = sum / size;
    const double squareMean = squareSum / size;
    
    return squareMean - mean * mean;
}

void Img::minmax(double& min, double& max) const {
    if (height == 0 || width == 0) {
        std::cerr << "Error: Empty image" << std::endl;
        exit(1);
    }

    const int size = height * width;
    min = std::numeric_limits<double>::max();
    max = std::numeric_limits<double>::lowest();
    
    for (int i = 0; i < size; ++i) {
        const double val = data[i];
        if (val < min) {
            min = val;
        }
        if (val > max) {
            max = val;
        }
    }

}