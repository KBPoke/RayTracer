#pragma once
#include <cstdint>
#include <random>
#include <limits>

class PRNG {
private:
	uint32_t state[4];

public:
	PRNG() : state{}  {
		std::random_device rd;
		std::uniform_int_distribution<int> seed(0, 31);
		for (auto& x : state) {
			x = seed(rd);
		}
	}

	uint32_t xorshift() {
		uint32_t t = state[3];

		uint32_t s = state[0];
		state[3] = state[2];
		state[2] = state[1];
		state[1] = state[0];

		t ^= t << 11;
		t ^= t >> 8;

		return state[0] = t ^ s ^ (s >> 19);
	}

	float get_random() {
		return (float)xorshift() / (std::numeric_limits<uint32_t>::max());
	}

	float get_random(float min, float max) {
		return min + get_random() * (max - min);
	}
};