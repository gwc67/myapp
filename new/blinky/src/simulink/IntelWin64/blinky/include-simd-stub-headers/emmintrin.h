#include <stdint.h>
typedef void* __m128d;
__m128d _mm_add_pd(__m128d u1, __m128d u2);
__m128d _mm_div_pd(__m128d u1, __m128d u2);
__m128d _mm_loadu_pd(const real_T *u1);
__m128d _mm_mul_pd(__m128d u1, __m128d u2);
__m128d _mm_set1_pd(real_T u1);
__m128d _mm_set_pd(real_T u1, real_T u2);
__m128d _mm_sub_pd(__m128d u1, __m128d u2);
void _mm_storeu_pd(real_T *u1, __m128d u2);
