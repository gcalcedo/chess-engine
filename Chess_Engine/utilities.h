#pragma once

#include "types.h"

namespace util {
	namespace {
		template <typename T, T m, int k>
		static inline T swapbits(T p) {
			T q = ((p >> k) ^ p) & m;
			return p ^ q ^ (q << k);
		}
	}

	u64 rev(u64 n)
	{
		static const u64 m1 = ((u64(1) << 63) - 1) / (1 + (1 << 1) + (1 << 2));
		static const u64 m2 = ((u64(1) << 63) - 1) / (1 + (1 << 3) + (1 << 6));
		static const u64 m3 = ((u64(1) << 9) - 1) + (((u64(1) << 9) - 1) << 36);
		static const u64 m4 = (u64(1) << 27) - 1;
		n = swapbits<u64, m1, 2>(n);
		n = swapbits<u64, m2, 6>(n);
		n = swapbits<u64, m3, 18>(n);
		n = swapbits<u64, m4, 36>(n);
		n = (n >> 63) | (n << 1);
		return n;
	}
}