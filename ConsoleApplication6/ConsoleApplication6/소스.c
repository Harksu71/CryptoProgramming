#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ��� ���� */
#define BLOCK_SIZE	 8		// DES ��� ������
#define DES_ROUND	16		// DES ���� ��

/* Ÿ�� ���� */
typedef unsigned char BYTE;
typedef unsigned int UINT;

/* �Լ� ���� */
// ġȯ �Լ�
void PC1(BYTE* in, BYTE* out);									// ġȯ ���� - 1 �Լ�
void PC2(UINT c, UINT d, BYTE* out);							// ġȯ ���� - 2 �Լ�
void key_expansion(BYTE *key, BYTE exp_key[16][6]);				// Ű Ȯ�� �Լ�
void makeBit28(UINT* c, UINT *d, BYTE *data);					// 56 bit�� 28 bit�� ������ �Լ�
UINT cir_shift(UINT n, int r);									// 28 bit ��ȯ ����Ʈ �Լ�


																// ġȯ ����-1 ���̺�
BYTE PC_1[56] = { 57, 49, 41, 33, 25, 17,  9,  1,
58, 50, 42, 34, 26, 18, 10,  2,
59, 51, 43, 35, 27, 19, 11,  3,
60, 52, 44, 36, 63, 55, 47, 39,
31, 23, 15,  7, 62, 54, 46, 38,
30, 22, 14,  6, 61, 53, 45, 37,
29, 21, 13,  5, 28, 20, 12,  4 };

// ġȯ ����-2 ���̺�
BYTE PC_2[48] = { 14, 17, 11, 24,  1,  5,  3, 28,
15,  6, 21, 10, 23, 19, 12,  4,
26,  8, 16,  7, 27, 20, 13,  2,
41, 52, 31, 37, 47, 55, 30, 40,
51, 45, 33, 48, 44, 49, 39, 56,
34, 53, 46, 42, 50, 36, 29, 32 };



///////////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	int i, j;

	BYTE key[9] = { 0, };						// ��� Ű
	BYTE round_key[16][6] = { 0, };

	// ���Ű �Է�
	printf("* ���Ű �Է� : ");
	scanf("%s", key);

	key_expansion(key, round_key);		// ���� Ű ����

										// ��ȣ�� ���
	printf("Round key : \n");
	for (i = 0; i<16; i++) {
		for (j = 0; j<6; j++) {
			printf("%x", round_key[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


// Ű Ȯ�� �Լ�
void key_expansion(BYTE *key, BYTE round_key[16][6])
{
	int i;
	BYTE pc1_result[7] = { 0, };
	UINT c = 0, d = 0;

	PC1(key, pc1_result);				// ġȯ ���� - 1

	makeBit28(&c, &d, pc1_result);		// 56bit ����� 28bit ������� ����

	for (i = 0; i<16; i++)
	{
		// 28 bit ��ȯ ����Ʈ
		c = cir_shift(c, i);
		d = cir_shift(d, i);

		PC2(c, d, round_key[i]);		// ġȯ ���� - 2	
	}
}

// ġȯ ���� - 1
void PC1(BYTE* in, BYTE* out)
{
	int i, index, bit;
	UINT mask = 0x00000080;

	for (i = 0; i<56; i++)
	{
		// �ش� bit�� ��ġ�� ���
		index = (PC_1[i] - 1) / 8;
		bit = (PC_1[i] - 1) % 8;

		// �ش� bit�� ���� ���� bit���� ����
		if (in[index] & (BYTE)(mask >> bit))
			out[i / 8] |= (BYTE)(mask >> (i % 8));
	}
}

// ġȯ ���� - 2
void PC2(UINT c, UINT d, BYTE* out)
{
	int i;
	UINT mask = 0x08000000;

	// PC-2 (56 bit -> 48 bit)
	for (i = 0; i<48; i++)
	{
		// �ش� bit�� ���� ���� bit���� ����
		if (PC_2[i] - 1 < 28)
		{
			if (c & (mask >> (PC_2[i] - 1)))
				out[i / 8] |= 0x80 >> (i % 8);
		}
		else
		{
			if (d & (mask >> (PC_2[i] - 1 - 28)))
				out[i / 8] |= 0x80 >> (i % 8);
		}
	}
}

// 56��Ʈ�� 28��Ʈ�� ������ �Լ�
void makeBit28(UINT* c, UINT *d, BYTE *data)
{
	int i;
	BYTE mask = 0x80;

	for (i = 0; i<56; i++)
	{
		if (i < 28)
		{
			if (data[i / 8] & (mask >> (i % 8)))
				*c |= 0x08000000 >> i;
		}
		else
		{
			if (data[i / 8] & (mask >> (i % 8)))
				*d |= 0x08000000 >> (i - 28);
		}
	}
}

// 28bit ���� ��ȯ ����Ʈ
UINT cir_shift(UINT n, int r)
{
	int n_shift[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };	// ���忡 ���� ����Ʈ Ƚ��

	if (n_shift[r] == 1)
	{

		n = ((n << 1) + (n >> 27)) & 0xFFFFFFF;
	}
	else
	{
		n = ((n << 2) + (n >> 26)) & 0xFFFFFFF;
	}

	return n;
}
