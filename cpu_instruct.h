#ifndef CPU_INSTRUCT_H
#define CPU_INSTRUCT_H

#include <cstdint>

#define CPU_PAUSE() __asm__ __volatile__ (".byte 0xf3, 0x90")

#define RDTSCP(val) do { \
	uint32_t __a, __d, __c; \
	__asm__ __volatile__ ("rdtscp" : "=a" (__a), "=d" (__d), "=c" (__c)); \
	val = (uint64_t)(__a) | ((uint64_t)(__d) << 32); \
} while (0)

/*
uint64_t rdtscp() {
	uint32_t __a, __d, __c;
	asm volatile("rdtscp" : "=a" (__a), "=d" (__d), "=c" (__c));
	return (uint64_t)(__a) | ((uint64_t)(__d) << 32); 
}
*/
#endif
