#include <iostream>

#include "../include/aes.h"
//#include "aes/aes_test.h"
#include <Windows.h>

int main()
{
	//uint8_t key[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	uint8_t out[EXP_KEY_SIZE];

	uint8_t buf[BLOCK_SIZE] = { 0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a };
	uint8_t key[KEY_SIZE] = { 0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c };

	
	int i = 10;
	int r = 0;
	
	//int xoff = i + (r * Nk);
	//int yoff = (i / Nk) * (EXP_KEY_SIZE / 4);
	
	int xoff = i

	std::cout << "X: " << xoff << std::endl;
	std::cout << "Y: " << yoff << std::endl;
	std::cout << "R: " << xoff + yoff;


	//aes_key_ctx ctx;
	//AES::set_key(&ctx, key);
	//AES::encrypt(buf, &ctx);

	//8e 4d a1 bc     4   3  2  1

}

