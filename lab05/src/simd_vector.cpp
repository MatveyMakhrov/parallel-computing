#include "../include/simd_vector.h"
#include <immintrin.h>

void normalize_simd_avx2(
    const std::vector<int8_t>& a,
    std::vector<int8_t>& out,
    int8_t min_val,
    int8_t max_val,
    int V
) {
    int n = a.size();
    float range = static_cast<float>(max_val - min_val);

    __m256 v_min = _mm256_set1_ps((float)min_val);
    __m256 v_range = _mm256_set1_ps(range);
    __m256 v_V = _mm256_set1_ps((float)V);

    for (int i = 0; i < n; i += 8) {
        __m128i v8 = _mm_loadl_epi64((__m128i*)&a[i]);
        __m256i v32 = _mm256_cvtepi8_epi32(v8);
        __m256 v = _mm256_cvtepi32_ps(v32);
        v = _mm256_sub_ps(v, v_min);
        v = _mm256_div_ps(v, v_range);
        v = _mm256_mul_ps(v, v_V);
        __m256i res32 = _mm256_cvtps_epi32(v);

        __m128i lo = _mm256_castsi256_si128(res32);
        __m128i hi = _mm256_extracti128_si256(res32, 1);

        __m128i packed16 = _mm_packs_epi32(lo, hi);
        __m128i packed8 = _mm_packs_epi16(packed16, packed16);

        _mm_storel_epi64((__m128i*)&out[i], packed8);
    }
}