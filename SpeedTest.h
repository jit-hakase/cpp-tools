#ifndef UTIL_SPEED_TEST_H
#define UTIL_SPEED_TEST_H

#include <cstdint>

uint64_t rdtscp() {
	uint32_t __a, __d, __c;
	asm volatile("rdtscp" : "=a" (__a), "=d" (__d), "=c" (__c));
	return (uint64_t)(__a) | ((uint64_t)(__d) << 32); 
}

#endif
