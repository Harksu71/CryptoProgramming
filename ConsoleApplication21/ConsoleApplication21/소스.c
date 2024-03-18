#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CIR_SHIFT(x,n) ( ((x) << n) | ((x) >> (32-n)) )				// 32비트 왼쪽 순환 이동

// 기약 논리 함수
#define F1(c,d,e) ( ((c)&(e)) | ((~c)&(d)) )

/* 타입 정의 */
typedef unsigned char BYTE;
typedef unsigned int UINT;

////////////////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	int i;

	UINT reg[5] = { 0xaaaabbbb, 0xccccdddd, 0x11112222, 0x33334444, 0x55556666 };				// 초기 레지스터
	UINT W[10] = { 0, }, temp;

	printf("입력 :  ");
	for (i = 0; i<5; i++)
		printf("%X  ", reg[i]);
	printf("\n");

	W[0] = 0x11111111;
	for (i = 1; i <= 5; i++)
		W[i] = W[0] * (i + 1);

	printf("W :  ");
	for (i = 0; i < 5; i++)
		printf("%X  ", W[i]);
	printf("\n");

	for (i = 0; i < 5; i++) {
		temp = reg[0] + F1(reg[2], reg[3], reg[4]) + W[i];
		reg[0] = CIR_SHIFT(reg[1], (i + 1) * 4);
		reg[1] = reg[2];
		reg[2] = F1(reg[2], reg[3], reg[4]);
		reg[3] = reg[4];
		reg[4] = temp;
	}


	printf("5라운드 출력 :  ");
	for (i = 0; i<5; i++)
		printf("%X  ", reg[i]);
	printf("\n");
}
