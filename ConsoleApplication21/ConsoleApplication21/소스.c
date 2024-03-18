#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CIR_SHIFT(x,n) ( ((x) << n) | ((x) >> (32-n)) )				// 32��Ʈ ���� ��ȯ �̵�

// ��� �� �Լ�
#define F1(c,d,e) ( ((c)&(e)) | ((~c)&(d)) )

/* Ÿ�� ���� */
typedef unsigned char BYTE;
typedef unsigned int UINT;

////////////////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	int i;

	UINT reg[5] = { 0xaaaabbbb, 0xccccdddd, 0x11112222, 0x33334444, 0x55556666 };				// �ʱ� ��������
	UINT W[10] = { 0, }, temp;

	printf("�Է� :  ");
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


	printf("5���� ��� :  ");
	for (i = 0; i<5; i++)
		printf("%X  ", reg[i]);
	printf("\n");
}
