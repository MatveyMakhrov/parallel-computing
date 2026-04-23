#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <string>
#include <vector>

class Image {
public:
    Image() = default;
    Image(int width, int height);
    
    bool loadPGM(const std::string& filename);
    bool savePGM(const std::string& filename) const;
    
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    uint8_t* getData() { return data_.data(); }
    const uint8_t* getData() const { return data_.data(); }
    
    uint8_t& at(int x, int y) { return data_[y * width_ + x]; }
    const uint8_t& at(int x, int y) const { return data_[y * width_ + x]; }
    
    bool isEmpty() const { return data_.empty(); }
    
private:
    int width_ = 0;
    int height_ = 0;
    std::vector<uint8_t> data_;
};

#endif // IMAGE_H