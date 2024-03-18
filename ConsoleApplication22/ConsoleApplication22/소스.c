#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int UINT;
typedef unsigned char BYTE;

#define CIR_SHIFT(x,n) ( ((x) <<n) | ((x) >> 32-n) )
#define F1(c,d,e) ( ((c)&(e)) | ((~c)&(d)) )

void SHA_1_digest(BYTE* in);

void main() {
	int i;
	UINT A = 0xAAAABBBB;
	UINT B = 0xCCCCDDDD;
	UINT C = 11112222;
	UINT D = 0x33334444;
	UINT E = 0x55556666;
	int S1[5] = { 4, 8, 12, 16, 20 };
	UINT W[5] = { 0, };

	W[0] = 0x11111111;
W[1] = 0x22222222;
 W[2] = 0x33333333; W[3] = 0x44444444;
W[4] = 0x55555555;

	UINT reg[5];

	reg[0] = A; reg[1] = B; reg[2] = C; reg[3] = D; reg[4] = E;

	printf("입력 : %x %x %x %x %x", A, B, C, D, E);
	printf("\n");

	for (i = 1; i <= 5; i++)
		printf("W : ", W[i]);
	printf("\n");

	for (i = 0; i < 6; i++)
	{
		UINT temp;
		temp = CIR_SHIFT(reg[1], S1[i]);

		reg[4] = reg[0] + F1(reg[2], reg[3], reg[4]) + W[i];
		reg[3] = reg[4];
		reg[2] = F1(reg[2], reg[3], reg[4]);
		reg[1] = reg[2];
		reg[0] = temp;
	}

	for (i = 0; i < 5; i++)
		printf("5라운드 출력 : ", reg[i]);
	printf("\n");

}
