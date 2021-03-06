#pragma once

#include <ctime>
#include "txbase/sys/memory.h"

namespace TX{

	class RNG {
	public:
		RNG(uint32_t seed = (uint32_t)time(NULL)){
#ifdef _MSC_VER

			cur_seed_ = AllocAligned<__m128i>(sizeof __m128i, 16);
			if (!cur_seed_) throw std::runtime_error("out of memory");
			*cur_seed_ = _mm_set_epi32(seed, seed + 1, seed, seed + 1);
#else
			cur_seed_ = seed;
#endif
		}

		~RNG(){
#ifdef _MSC_VER
			if (cur_seed_){
				FreeAligned(cur_seed_);
			}
#endif
		}

		inline uint32_t UInt(){
#ifdef _MSC_VER
			__declspec(align(16)) uint32_t result[4];
			__declspec(align(16)) __m128i cur_seed_split;
			__declspec(align(16)) __m128i multiplier;
			__declspec(align(16)) __m128i adder;
			__declspec(align(16)) __m128i mod_mask;
			__declspec(align(16)) __m128i sra_mask;
#ifdef COMPATABILITY
			__declspec(align(16)) __m128i sseresult;
#endif
			__declspec(align(16)) static const unsigned int mult[4] = { 214013, 17405, 214013, 69069 };
			__declspec(align(16)) static const unsigned int gadd[4] = { 2531011, 10395331, 13737667, 1 };
			__declspec(align(16)) static const unsigned int mask[4] = { 0xFFFFFFFF, 0, 0xFFFFFFFF, 0 };
			__declspec(align(16)) static const unsigned int masklo[4] = { 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF };
			adder = _mm_load_si128((__m128i*) gadd);
			multiplier = _mm_load_si128((__m128i*) mult);
			mod_mask = _mm_load_si128((__m128i*) mask);
			sra_mask = _mm_load_si128((__m128i*) masklo);
			cur_seed_split = _mm_shuffle_epi32(*cur_seed_, _MM_SHUFFLE(2, 3, 0, 1));
			*cur_seed_ = _mm_mul_epu32(*cur_seed_, multiplier);
			multiplier = _mm_shuffle_epi32(multiplier, _MM_SHUFFLE(2, 3, 0, 1));
			cur_seed_split = _mm_mul_epu32(cur_seed_split, multiplier);
			*cur_seed_ = _mm_and_si128(*cur_seed_, mod_mask);
			cur_seed_split = _mm_and_si128(cur_seed_split, mod_mask);
			cur_seed_split = _mm_shuffle_epi32(cur_seed_split, _MM_SHUFFLE(2, 3, 0, 1));
			*cur_seed_ = _mm_or_si128(*cur_seed_, cur_seed_split);
			*cur_seed_ = _mm_add_epi32(*cur_seed_, adder);
#ifdef COMPATABILITY
			// Add the lines below if you wish to reduce your results to 16-bit vals...
			sseresult = _mm_srai_epi32(cur_seed_, 16);
			sseresult = _mm_and_si128(sseresult, sra_mask);
			_mm_storeu_si128((__m128i*) result, sseresult);
			return;
#else
			_mm_storeu_si128((__m128i*) result, *cur_seed_);
			return result[0];
#endif COMPATABILITY

#else // _MSC_VER
			cur_seed_ = (214013*cur_seed_+2531011);
			return (cur_seed_>>16)&0x7FFF;
#endif // _MSC_VER
		}

		/// <summary>
		/// [0, 1]
		/// </summary>
		inline float Float(){
#ifdef COMPATABILITY
			return UInt() / (float)(0xFFFF);
#else
			return UInt() / (float)(0xFFFFFFFF);
#endif
		}
	private:
#ifdef _MSC_VER
		__m128i* cur_seed_;
#else
		uint cur_seed_;
#endif
	};
}
