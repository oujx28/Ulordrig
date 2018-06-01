/* Copyright 2016-2018 The Ulord Core Foundation */

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define INPUT_LEN	32
#define OUTPUT_LEN	32

extern const size_t sizeof_1_uint8_t;
extern const size_t sizeof_6_uint8_t;
extern const size_t sizeof_32_uint8_t;

// Least common multiple
inline uint32_t lcm(uint32_t num1, uint32_t num2) {
	uint32_t m = num1, n = num2;
	while(num2) {
		uint32_t r = num1 % num2;
		num1 = num2;
		num2 = r;
	}
	uint32_t lcm = m * n / num1;
	return lcm;
}

inline void reduce_bit_2(uint8_t *input, uint32_t inputLen, 
		uint8_t *output, uint32_t bits) {
	uint32_t i, outputLen = (bits) >> 3;
	uint32_t lcmBytes = lcm(inputLen, outputLen);
	memcpy(output, input, outputLen * sizeof(uint8_t));
	for (i = outputLen; i < lcmBytes; ++i) {
		output[i % outputLen] ^= input[i % inputLen];
	}
}

inline void reduce_bit(uint8_t *input, uint32_t inputLen, 
		uint8_t *output, uint32_t bits) {
	uint32_t i, outputLen = (bits) >> 3, j;
	size_t len = sizeof_1_uint8_t;
	//memcpy(output, input, outputLen * sizeof(uint8_t));
	//for (i = outputLen; i < inputLen; ++i) {
	//	output[i % outputLen] ^= input[i % inputLen];
	//}
	switch (outputLen) {
		case 1:
			len = sizeof_1_uint8_t;
			break;
		case 6:
			len = sizeof_6_uint8_t;
			break;
		case 32:
			len = sizeof_32_uint8_t;
			break;
		default:
			len = outputLen * sizeof(uint8_t);
			break;
	}
	memcpy(output, input, len);
	
	for (i = outputLen; i < inputLen; ++i) {
		for (j = 0; i < inputLen; ++i) {
			output[j] ^= input[i];
			if (++j == outputLen) break;
		}
	}
}

inline void rrs(uint8_t *input, uint32_t inputLen, 
		uint8_t *output, uint32_t bits) {
	uint32_t shiftBytes = (bits) >> 3, shiftBits = (bits) & 0x7;
	uint32_t rIndex = (inputLen) - shiftBytes;
	//uint32_t lIndex = (rIndex + (inputLen) - 1) % (inputLen);
	uint32_t lIndex = (rIndex + (inputLen) - 1) & 0x1F;   // inputLen = 32 -> 0x20;
	for (uint32_t i = 0; i < inputLen; ++i) {
		//output[i] = (input[(rIndex++) % (inputLen)] >> shiftBits) |
		//	(input[(lIndex++) % (inputLen)] << (8 - shiftBits));
		output[i] = (input[(rIndex++) & (0x1F)] >> shiftBits) |
			(input[(lIndex++) & (0x1F)] << (8 - shiftBits));
	}
}

#ifdef __cplusplus
extern "C" {
#endif

	void view_data_u8(const char *mess, uint8_t *data, uint32_t len);
	void view_data_u32(const char *mess, uint32_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif	


#endif
