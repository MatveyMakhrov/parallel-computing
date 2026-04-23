#include "../include/sobel_filter.h"
#include <immintrin.h>
#include <cmath>
#include <algorithm>

Image SobelFilter::sobel_scalar(const Image& src) {
    int w = src.getWidth();
    int h = src.getHeight();
    Image dst(w, h);
    
    const float scale = 256.0f / 1140.0f;
    
    for (int y = 1; y < h - 1; ++y) {
        const uint8_t* prev = src.getData() + (y-1) * w;
        const uint8_t* cur  = src.getData() + y * w;
        const uint8_t* next = src.getData() + (y+1) * w;
        uint8_t* out = dst.getData() + y * w;
        
        for (int x = 1; x < w - 1; ++x) {
            int A = prev[x-1], B = prev[x], C = prev[x+1];
            int D = cur[x-1],  F = cur[x+1];
            int G = next[x-1], H = next[x], I = next[x+1];
            
            int t1 = A - I;
            int t2 = C - G;
            
            int Hh = 2 * (D - F) + t1 - t2;
            int Hv = 2 * (B - H) + t1 + t2;
            
            float mag = scale * std::sqrt(float(Hh*Hh + Hv*Hv));
            out[x] = static_cast<uint8_t>((std::min)(255.0f, mag));
        }
    }
    return dst;
}

Image SobelFilter::sobel_avx2(const Image& src) {
    int w = src.getWidth();
    int h = src.getHeight();
    Image dst(w, h);
    
    const float scale = 256.0f / 1140.0f;
    __m256 vscale = _mm256_set1_ps(scale);
    __m256 vzero = _mm256_setzero_ps();
    __m256 v255 = _mm256_set1_ps(255.0f);
    
    for (int y = 1; y < h - 1; ++y) {
        const uint8_t* prev = src.getData() + (y-1) * w;
        const uint8_t* cur  = src.getData() + y * w;
        const uint8_t* next = src.getData() + (y+1) * w;
        uint8_t* out = dst.getData() + y * w;
        
        int x = 1;
        for (; x <= w - 1 - 8; x += 8) {
            __m128i A = _mm_cvtepu8_epi16(_mm_loadl_epi64((__m128i*)(prev + x - 1)));
            __m128i B = _mm_cvtepu8_epi16(_mm_loadl_epi64((__m128i*)(prev + x)));
            __m128i C = _mm_cvtepu8_epi16(_mm_loadl_epi64((__m128i*)(prev + x + 1)));
            
            __m128i D = _mm_cvtepu8_epi16(_mm_loadl_epi64((__m128i*)(cur + x - 1)));
            __m128i F = _mm_cvtepu8_epi16(_mm_loadl_epi64((__m128i*)(cur + x + 1)));
            
            __m128i G = _mm_cvtepu8_epi16(_mm_loadl_epi64((__m128i*)(next + x - 1)));
            __m128i H = _mm_cvtepu8_epi16(_mm_loadl_epi64((__m128i*)(next + x)));
            __m128i I_val = _mm_cvtepu8_epi16(_mm_loadl_epi64((__m128i*)(next + x + 1)));
            
            __m128i t1 = _mm_sub_epi16(A, I_val);
            __m128i t2 = _mm_sub_epi16(C, G);
            
            __m128i D_minus_F = _mm_sub_epi16(D, F);
            __m128i Hh_part1 = _mm_slli_epi16(D_minus_F, 1);
            __m128i Hh = _mm_add_epi16(Hh_part1, _mm_sub_epi16(t1, t2));
            
            __m128i B_minus_H = _mm_sub_epi16(B, H);
            __m128i Hv_part1 = _mm_slli_epi16(B_minus_H, 1);
            __m128i Hv = _mm_add_epi16(Hv_part1, _mm_add_epi16(t1, t2));
            
            __m128i Hh_lo = _mm_cvtepi16_epi32(Hh);
            __m128i Hh_hi = _mm_cvtepi16_epi32(_mm_shuffle_epi32(Hh, 0x4E));
            __m128i Hv_lo = _mm_cvtepi16_epi32(Hv);
            __m128i Hv_hi = _mm_cvtepi16_epi32(_mm_shuffle_epi32(Hv, 0x4E));
            
            __m128 Hh_f_lo = _mm_cvtepi32_ps(Hh_lo);
            __m128 Hh_f_hi = _mm_cvtepi32_ps(Hh_hi);
            __m128 Hv_f_lo = _mm_cvtepi32_ps(Hv_lo);
            __m128 Hv_f_hi = _mm_cvtepi32_ps(Hv_hi);
            
            __m256 Hh_f = _mm256_set_m128(Hh_f_hi, Hh_f_lo);
            __m256 Hv_f = _mm256_set_m128(Hv_f_hi, Hv_f_lo);
            
            __m256 mag_sq = _mm256_add_ps(_mm256_mul_ps(Hh_f, Hh_f), _mm256_mul_ps(Hv_f, Hv_f));
            __m256 mag = _mm256_sqrt_ps(mag_sq);
            __m256 result = _mm256_mul_ps(vscale, mag);
            
            result = _mm256_min_ps(result, v255);
            result = _mm256_max_ps(result, vzero);
            
            __m256i res_int = _mm256_cvtps_epi32(result);
            __m128i res_lo = _mm256_castsi256_si128(res_int);
            __m128i res_hi = _mm256_extracti128_si256(res_int, 1);
            
            __m128i res_u16 = _mm_packs_epi32(res_lo, res_hi);
            __m128i res_u8 = _mm_packus_epi16(res_u16, res_u16);
            
            _mm_storel_epi64((__m128i*)(out + x), res_u8);
        }
        
        for (; x < w - 1; ++x) {
            int A = prev[x-1], B = prev[x], C = prev[x+1];
            int D = cur[x-1],  F = cur[x+1];
            int G = next[x-1], H = next[x], I_val = next[x+1];
            int t1 = A - I_val, t2 = C - G;
            int Hh = 2*(D - F) + t1 - t2;
            int Hv = 2*(B - H) + t1 + t2;
            float mag = scale * std::sqrt(float(Hh*Hh + Hv*Hv));
            out[x] = static_cast<uint8_t>((std::min)(255.0f, mag));
        }
    }
    return dst;
}

bool SobelFilter::validate(const Image& a, const Image& b, double epsilon) {
    if (a.getWidth() != b.getWidth() || a.getHeight() != b.getHeight())
        return false;
    
    int w = a.getWidth(), h = a.getHeight();
    double max_diff = 0.0;
    
    for (int i = 0; i < w * h; ++i) {
        double diff = std::abs(a.getData()[i] - b.getData()[i]);
        if (diff > max_diff) max_diff = diff;
    }
    
    return max_diff <= epsilon;
}