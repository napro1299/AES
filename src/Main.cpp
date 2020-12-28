#include <iostream>

//#include "../include/aes.h"
#include "aes/tinyaes.h"
//#include "aes/aes_test.h"

static uint32_t mul_by_x(uint32_t w) // 2 4 6 8 a c e 10 12
{
	uint32_t x = w & 0x7f7f7f7f;
	uint32_t y = w & 0x80808080;

	/* multiply by polynomial 'x' (0b10) in GF(2^8) */
	return (x << 1) ^ (y >> 7) * 0x1b;
}

int main()
{
	uint8_t key[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	uint8_t out[16];

	

	uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	AES_ctx ctx;
	ctx.RoundKey = 
	AES_ECB_encrypt(&ctx, data);
	//key_expand(key, out);

	for (int i = 0; i < 16; i++)
		std::cout << (int)out[i] << " ";

	////u8 key[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	//u8 d[16];
	//MakeRoundKey(key, d, FIRST);
	//for (int i = 0; i < 16; i++)
	//	std::cout << (int)d[i] << " ";

	//std::cout << std::endl;

	//u8 f[4] = { 0x10, 0x04, 0x08, 0x0c };
	//SubBytes<4>(f);
	//for (int i = 0; i < 4; i++)
	//	std::cout << (int)f[i] << " ";

}

