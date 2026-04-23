#include "../include/image.h"
#include <fstream>
#include <iostream>
#include <sstream>

Image::Image(int width, int height) 
    : width_(width), height_(height), data_(width * height, 0) {}

bool Image::loadPGM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    
    std::string magic;
    file >> magic;
    
    if (magic != "P5") {
        std::cerr << "Not a binary PGM file (P5 expected)" << std::endl;
        return false;
    }
    
    int ch;
    while ((ch = file.get()) == '#') {
        file.ignore(1000, '\n');
    }
    file.unget();
    
    int maxVal;
    file >> width_ >> height_ >> maxVal;
    file.get();
    
    if (maxVal != 255) {
        std::cerr << "Only 8-bit PGM supported (maxVal=255)" << std::endl;
        return false;
    }
    
    data_.resize(width_ * height_);
    file.read(reinterpret_cast<char*>(data_.data()), data_.size());
    
    return file.good();
}

bool Image::savePGM(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to create file: " << filename << std::endl;
        return false;
    }
    
    file << "P5\n" << width_ << " " << height_ << "\n255\n";
    file.write(reinterpret_cast<const char*>(data_.data()), data_.size());
    
    return file.good();
}