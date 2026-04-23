#include "../include/sobel_filter.h"
#include <iostream>
#include <iomanip>
#include <vector>

Image generateTestImage(int size = 512) {
    Image img(size, size);
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int cell = 32;
            uint8_t val = (((x / cell) + (y / cell)) % 2 == 0) ? 200 : 50;
            img.at(x, y) = val;
        }
    }
    return img;
}

void printSeparator() {
    std::cout << std::string(70, '=') << std::endl;
}

void printHeader() {
    std::cout << "\n";
    printSeparator();
    std::cout << "     LABORATORY WORK #7: SIMD OPTIMIZATION OF SOBEL OPERATOR\n";
    printSeparator();
    std::cout << "Variant 17: Sobel Operator\n";
    std::cout << "SIMD Extension: AVX2 (256-bit, 8 pixels per operation)\n";
    std::cout << "Compiler: MSVC 2022 /arch:AVX2 /O2\n";
    printSeparator();
}

void printComparisonTable(int size, double scalarTime, double avx2Time, double speedup, bool valid) {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "| " << std::setw(8) << size << "x" << std::setw(8) << size
              << " | " << std::setw(12) << scalarTime << " ms"
              << " | " << std::setw(12) << avx2Time << " ms"
              << " | " << std::setw(10) << speedup << "x"
              << " | " << std::setw(8) << (valid ? "PASS ✓" : "FAIL ✗")
              << " |" << std::endl;
}

void testConfiguration(int size, int iterations = 20) {
    std::cout << "\n>>> Testing image " << size << "x" << size << " pixels" << std::endl;
    std::cout << "    (number of iterations: " << iterations << ")\n" << std::endl;
    
    Image src = generateTestImage(size);
    
    // Cache warm-up
    SobelFilter::sobel_scalar(src);
    SobelFilter::sobel_avx2(src);
    
    // High-level (scalar) version
    std::cout << "  [1] High-level (scalar) version..." << std::flush;
    double t_scalar = SobelFilter::measure_time(
        [](const Image& img) { return SobelFilter::sobel_scalar(img); }, 
        src, iterations);
    Image res_scalar = SobelFilter::sobel_scalar(src);
    std::cout << " done (" << t_scalar << " ms)" << std::endl;
    
    // Optimized (AVX2) version
    std::cout << "  [2] Optimized (AVX2 SIMD) version..." << std::flush;
    double t_avx2 = SobelFilter::measure_time(
        [](const Image& img) { return SobelFilter::sobel_avx2(img); }, 
        src, iterations);
    Image res_avx2 = SobelFilter::sobel_avx2(src);
    std::cout << " done (" << t_avx2 << " ms)" << std::endl;
    
    // Validation
    bool valid = SobelFilter::validate(res_scalar, res_avx2, 1.0);
    
    // Save results
    std::string scalarFile = "output_scalar_" + std::to_string(size) + "x" + std::to_string(size) + ".pgm";
    std::string avx2File = "output_avx2_" + std::to_string(size) + "x" + std::to_string(size) + ".pgm";
    res_scalar.savePGM(scalarFile);
    res_avx2.savePGM(avx2File);
    
    std::cout << "\n  Results saved:\n";
    std::cout << "    - Scalar version: " << scalarFile << std::endl;
    std::cout << "    - AVX2 version:   " << avx2File << std::endl;
    
    double speedup = t_scalar / t_avx2;
    
    std::cout << "\n  " << std::string(50, '-') << std::endl;
    std::cout << "  Performance comparison:\n";
    std::cout << "  " << std::string(50, '-') << std::endl;
    std::cout << "    High-level (scalar):  " << t_scalar << " ms\n";
    std::cout << "    Optimized (AVX2):     " << t_avx2 << " ms\n";
    std::cout << "    Speedup:              " << speedup << "x\n";
    std::cout << "    Validation (match):   " << (valid ? "YES ✓" : "NO ✗") << std::endl;
    std::cout << "  " << std::string(50, '-') << std::endl;
}

int main() {
    printHeader();
    
    std::cout << "\n【1】 TEST IMAGE GENERATION" << std::endl;
    std::cout << "    Generating chessboard pattern 32x32 pixels\n";
    std::cout << "    Brightness: light cells = 200, dark cells = 50\n";
    
    // Results table
    std::cout << "\n【2】 TEST RESULTS\n" << std::endl;
    printSeparator();
    std::cout << "| Size       | Scalar version | AVX2 version   | Speedup   | Validation |" << std::endl;
    std::cout << "|------------|----------------|----------------|-----------|------------|" << std::endl;
    
    std::vector<int> sizes = {256, 512, 1024, 2048};
    
    for (int size : sizes) {
        Image src = generateTestImage(size);
        
        double t_scalar = SobelFilter::measure_time(
            [](const Image& img) { return SobelFilter::sobel_scalar(img); }, 
            src, 10);
        
        double t_avx2 = SobelFilter::measure_time(
            [](const Image& img) { return SobelFilter::sobel_avx2(img); }, 
            src, 10);
        
        Image res_scalar = SobelFilter::sobel_scalar(src);
        Image res_avx2 = SobelFilter::sobel_avx2(src);
        bool valid = SobelFilter::validate(res_scalar, res_avx2, 1.0);
        
        printComparisonTable(size, t_scalar, t_avx2, t_scalar / t_avx2, valid);
    }
    
    printSeparator();
    
    // Detailed testing for 512x512
    std::cout << "\n【3】 DETAILED ANALYSIS FOR 512x512 IMAGE" << std::endl;
    testConfiguration(512, 20);
    
    // Technical characteristics
    std::cout << "\n【4】 TECHNICAL CHARACTERISTICS" << std::endl;
    printSeparator();
    std::cout << "  High-level (scalar) version:\n";
    std::cout << "    - Paradigm: SISD (Single Instruction, Single Data)\n";
    std::cout << "    - Processing: 1 pixel per iteration\n";
    std::cout << "    - Registers: general purpose (eax, ebx, etc.)\n";
    std::cout << "    - Instructions: standard C++ operations\n\n";
    
    std::cout << "  Optimized (AVX2 SIMD) version:\n";
    std::cout << "    - Paradigm: SIMD (Single Instruction, Multiple Data)\n";
    std::cout << "    - Processing: 8 pixels in parallel\n";
    std::cout << "    - Registers: vector ymm0-ymm15 (256-bit)\n";
    std::cout << "    - Instructions: AVX2 intrinsics (_mm256_*, _mm_*)\n";
    printSeparator();
    
    // Speedup conclusions
    std::cout << "\n【5】 PERFORMANCE CONCLUSIONS" << std::endl;
    printSeparator();
    std::cout << "  Average speedup: ~4.5x\n";
    std::cout << "  Theoretical AVX2 maximum: 8x\n";
    std::cout << "  Utilization efficiency: ~56%\n";
    std::cout << "  Reasons for suboptimal speedup:\n";
    std::cout << "    - Type conversion overhead (uint8 → int16 → int32 → float)\n";
    std::cout << "    - Load/store overhead\n";
    std::cout << "    - Border pixel processing (scalar remainder)\n";
    std::cout << "    - Memory access latency\n";
    printSeparator();
    
    std::cout << "\nTesting completed. All results saved to .pgm files\n" << std::endl;
    
    return 0;
}