#pragma once

#ifndef _AES_
#define _AES_

#include <stdint.h>
#include <string.h>
#include <limits.h>

#ifdef AES192
#	define Nr           (12)
#	define Nk           (6)
#	define KEY_SIZE     (24)	
#	define EXP_KEY_SIZE (208)
#elif defined(AES256)
#	define Nr           (14)
#	define Nk           (8)
#	define KEY_SIZE     (32)
#	define EXP_KEY_SIZE (240)
#else						 // Default is AES128
#	define Nr           (10) // Number of rounds
#	define Nk           (4)  // Size of round key (number of 32-bit words)
#	define KEY_SIZE	    (16)	
#	define EXP_KEY_SIZE (176)
#endif

#define Nb (4) // number of columns in a state 
#define BLOCK_SIZE (Nb << 2)

#define SCOPE(x) {x}

#define PRINT_STATE NULL

#if defined PRINT_STATE && (PRINT_STATE == NULL)
#include <stdio.h>
#define PRINT_STATE(msg, state) \
printf("%s : ", msg);           \
for (int i = 0; i < 16; i++)    \
	printf("%x ", state[i]);     \
printf("\n");                   
#endif

////////////////////////////////////////////////////////////////
/// 
///        AES (128, 192, 156) Header Library
///	   	   Mode: EBC
/// 
////////////////////////////////////////////////////////////////

/*
* AES context struct
*/
typedef struct aes_key_ctx {
	uint8_t exp_key[EXP_KEY_SIZE];
} aes_key_ctx;

// Forward S-box
static const uint8_t sbox[] =
{ 
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 
};

// Inverse S-box
static const uint8_t inv_sbox[] =
{ 
	0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
	0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
	0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
	0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
	0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
	0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
	0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
	0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
	0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
	0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
	0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
	0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
	0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
	0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
	0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D 
};

// Round constant
static const uint8_t rcon[] =
{
	 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

static inline void subb(uint8_t state[Nb * Nb])
{
	for (int i = 0; i < Nb * Nb; i++)
	{	
		state[i] = sbox[state[i]];
	}
}

static inline void inv_subb(uint8_t state[Nb * Nb])
{
	for (int i = 0; i < Nb * Nb; i++)
	{
		state[i] = inv_sbox[state[i]];
	}
}

static inline void subw(uint8_t word[4])
{
	word[0] = sbox[word[0]];
	word[1] = sbox[word[1]];
	word[2] = sbox[word[2]];
	word[3] = sbox[word[3]];
}

static inline void rotword(uint8_t word[4])
{
	uint8_t temp[4]; 
	temp[0] = word[1];
	temp[1] = word[2];
	temp[2] = word[3];
	temp[3] = word[0];
	
	memcpy(word, temp, 4);
}

static void key_expand(const uint8_t* key_in, uint8_t* key_out) 
{
	uint8_t word[4];

	memcpy(key_out, key_in, Nk * 4);
	
	for (unsigned int i = Nk; i < Nb * (Nr + 1); i++) // 44
	{
		word[0] = key_out[((i - 1) * 4) + 0];
		word[1] = key_out[((i - 1) * 4) + 1];
		word[2] = key_out[((i - 1) * 4) + 2];
		word[3] = key_out[((i - 1) * 4) + 3]; 

		if (i % Nk == 0)
		{
			rotword(word);
			subw(word);
			word[0] = word[0] ^ rcon[i / Nk];
		}
#ifdef AES256
		if (i % Nk == 4)
			subw(word);
#endif
		key_out[(i * 4) + 0] = key_out[((i - Nk) * 4) + 0] ^ word[0];
		key_out[(i * 4) + 1] = key_out[((i - Nk) * 4) + 1] ^ word[1]; 
		key_out[(i * 4) + 2] = key_out[((i - Nk) * 4) + 2] ^ word[2];
		key_out[(i * 4) + 3] = key_out[((i - Nk) * 4) + 3] ^ word[3];
	}
}

static void add_rk(uint8_t r, uint8_t* state, const uint8_t* rkey)
{
	unsigned nrows, row, idx;
	for (int i = 0; i < Nb * 4; i++)
	{
		nrows = i / Nk;
		row = nrows * (EXP_KEY_SIZE / 4);
		idx = row + (i - (nrows * 4));
		state[i] ^= rkey[idx + (r * Nk)]; 
	}
}

// rotate left
static void shift_rows(uint8_t* state)
{
	uint8_t temp;
	
	// row 1
	temp                = state[Nb * 1];
	state[Nb * 1]       = state[(Nb * 1) + 1];
	state[(Nb * 1) + 1] = state[(Nb * 1) + 2];
	state[(Nb * 1) + 2] = state[(Nb * 1) + 3];
	state[(Nb * 1) + 3] = temp;

	// row 2
	temp                = state[Nb * 2];
	state[Nb * 2]       = state[(Nb * 2) + 2];
	state[(Nb * 2) + 2] = temp;
	temp                = state[(Nb * 2) + 1];
	state[(Nb * 2) + 1] = state[(Nb * 2) + 3];
	state[(Nb * 2) + 3] = temp;
	
	// row 3
	temp                = state[Nb * 3];
	state[Nb * 3]       = state[(Nb * 3) + 3];
	state[(Nb * 3) + 3] = state[(Nb * 3) + 2];
	state[(Nb * 3) + 2] = state[(Nb * 3) + 1];
	state[(Nb * 3) + 1] = temp;
}

/* Generic mathematical helper functions mul_by_x and mul_by_x2 are from the official */
/* Linux Kernel Generic AES library module */
/* https://www.github.com/torvalds/linux */

static uint32_t mul_by_x(uint32_t w)
{
	uint32_t x = w & 0x7f7f7f7f;
	uint32_t y = w & 0x80808080;

	/* multiply by polynomial 'x' (0b10) in GF(2^8) */
	return (x << 1) ^ (y >> 7) * 0x1b;
}

static uint32_t mul_by_x2(uint32_t w)
{
	uint32_t x = w & 0x3f3f3f3f;
	uint32_t y = w & 0x80808080;
	uint32_t z = w & 0x40404040;

	/* multiply by polynomial 'x^2' (0b100) in GF(2^8) */
	return (x << 2) ^ (y >> 7) * 0x36 ^ (z >> 6) * 0x1b;
}

// rotate right
static uint32_t ror32(uint32_t x, unsigned int count)
{
	return (x >> count) | (x << (32 - count));
}

static uint32_t mix_col(uint32_t w) 
{
	/*
	 * Perform the following matrix multiplication in GF(2^8)
	 *
	 * | 0x2 0x3 0x1 0x1 |   | x[0] |
	 * | 0x1 0x2 0x3 0x1 |   | x[1] |
	 * | 0x1 0x1 0x2 0x3 | x | x[2] |
	 * | 0x3 0x1 0x1 0x2 |   | x[3] |
	 */
	uint32_t y = mul_by_x(w) ^ ror32(w, 16);

	return y ^ ror32(w ^ y, 8);
};

#define COL_TO_WORD(arr, x, y, z, w) (arr[x]) ^	\
						  (arr[y] << 8) ^	\
					      (arr[z] << 16) ^	\
						  (arr[w] << 24) 	\

static void mix_columns(uint8_t* state)
{
	for (int i = 0; i < 4; i++)
	{
		uint32_t w = COL_TO_WORD(state, i, i + Nb, i + (Nb * 2), i + (Nb * 3));

		w = mix_col(w);
		state[i]            = w & 0xff;
		state[i + Nb]       = (w >> 8) & 0xff;
		state[i + (Nb * 2)] = (w >> 16) & 0xff;
		state[i + (Nb * 3)] = (w >> 24) & 0xff;
	}
}

// rotate right
static void inv_shift_rows(uint8_t* state)
{
	uint8_t temp;

	// row 1
	temp                = state[(Nb * 1) + 3];
	state[(Nb * 1) + 3] = state[(Nb * 1) + 2];
	state[(Nb * 1) + 2] = state[(Nb * 1) + 1];
	state[(Nb * 1) + 1] = state[Nb * 1];
	state[Nb * 1]       = temp;

	// row 2
	temp                = state[Nb * 2];
	state[Nb * 2]       = state[(Nb * 2) + 2];
	state[(Nb * 2) + 2] = temp;
	temp                = state[(Nb * 2) + 1];
	state[(Nb * 2) + 1] = state[(Nb * 2) + 3];
	state[(Nb * 2) + 3] = temp;

	// row 3
	temp                = state[Nb * 3];
	state[Nb * 3]       = state[(Nb * 3) + 1];
	state[(Nb * 3) + 1] = state[(Nb * 3) + 2];
	state[(Nb * 3) + 2] = state[(Nb * 3) + 3];
	state[(Nb * 3) + 3] = temp;
}

static uint32_t inv_mix_col(uint32_t w)
{
	uint32_t y = mul_by_x2(w);

	return mix_col(w ^ y ^ ror32(y, 16));
}

static uint32_t inv_mix_columns(uint8_t* state)
{
	for (int i = 0; i < 4; i++)
	{
		uint32_t w = COL_TO_WORD(state, i, i + Nb, i + (Nb * 2), i + (Nb * 3));

		w = inv_mix_col(w);
		state[i]            = w & 0xff;
		state[i + Nb]       = (w >> 8) & 0xff;
		state[i + (Nb * 2)] = (w >> 16) & 0xff;
		state[i + (Nb * 3)] = (w >> 24) & 0xff;
	}
}

static void cipher(uint8_t* state, const uint8_t* key)
{
	add_rk(0, state, key);

	for (int r = 0;; r++)
	{
		subb(state);
		shift_rows(state);
		if (r == Nr)
			break;
		mix_columns(state);
		add_rk(r, state, key);
	}
	add_rk(Nr, state, key);
}

static void inv_cipher(uint8_t* state, const uint8_t* key)
{
	add_rk(Nr, state, key);

	for (int r = Nr - 1;; r--)
	{
		inv_shift_rows(state);
		inv_subb(state);
		add_rk(r, state, key);
		if (r == 0)
			break;
		inv_mix_columns(state);
	}
}

void aes_set_key(aes_key_ctx* ctx, unsigned char* key)
{
	key_expand(key, ctx->exp_key);
}

void aes_encrypt(unsigned char* buf, aes_key_ctx* ctx)
{
	cipher(buf, ctx->exp_key);
}

void aes_decrypt(unsigned char* buf, aes_key_ctx* ctx)
{
	inv_cipher(buf, ctx->exp_key);
}

#endif // _AES_