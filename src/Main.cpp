#include <iostream>

#include "../include/aes.h"
//#include "aes/aes_test.h"
//#include <Windows.h>

int main()
{
	//uint8_t key[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	uint8_t out[EXP_KEY_SIZE];

	uint8_t buf[BLOCK_SIZE] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a };
	uint8_t key[KEY_SIZE] = { 0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c };
	
	int i = 10;
	int r = 1;

	int xoff = i + (r * Nk);
	int yoff = ((i / Nk) * (32 / 4));

	int row_count = i / Nk;
	int row = row_count * 8;
	int res = row + (i - (row_count * Nk));
	int idx = res + (r * Nk);

	aes_key_ctx ctx;
	aes::set_key(&ctx, key);
	aes::encrypt(buf, &ctx);

	for (int i = 0; i < 16; i++)
		std::cout << (int)buf[i] << " ";

	//8e 4d a1 bc     4   3  2  1

}

