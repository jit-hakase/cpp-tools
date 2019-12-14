#ifndef UTIL_ENDIAN_H
#define UTIL_ENDIAN_H

template <typename T>
inline T change_endian(T value) {
	auto result = value;
	auto value_size = sizeof(T);
	auto src = reinterpret_cast<char*>(&value);
	auto dst = reinterpret_cast<char*>(&result);
	for (int i = 0; i < value_size; ++i) {
		dst[value_size-1-i] = src[i];
	}
	return result;
}

#ifndef _BIG_ENDIAN_
#define CHANGE_ENDIAN(v) v = change_endian(v);
#else
#define CHANGE_ENDIAN(v)
#endif

/*
inline uint16_t change_endian(uint16_t value) {
	return ((value & 0x00ff) << 8) |
		((value & 0xff00) >> 8);
}

inline uint32_t change_endian(uint32_t value) {
	return ((value & 0x000000ff) << 24) |
		((value & 0x0000ff00) << 8) |
		((value & 0x00ff0000) >> 8) |
		((value & 0xff000000) >> 24);
}
*/
#endif
