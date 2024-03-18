//////////////////////////////////////////////
//	��� ���̰� 6�� 2���� Feistel ��ȣ    //
//////////////////////////////////////////////

#include <stdio.h>

/* ��� ���� */
#define BLOCK_SIZE		8	// ��� ũ��(bit)
#define ROUND_NUM		3	// ���� ��

/* �Լ� ���� */
char F1(char input);		// F �Լ� 1
char F2(char input);		// F �Լ� 2
char F3(char input);		// F �Լ� 2
char Feistel_Enc(char in);	// Feistel ��ȣȭ �Լ�
char Feistel_Dec(char in);	// Feistel ��ȣȭ �Լ�

							////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	char p_bit = 0xA7;
	char c_bit, d_bit;
	int temp = 0, i = 0;

	// �� ���
	printf("* �� : ");
	for (i = BLOCK_SIZE - 1; i >= 0; i--)	// ������ �� ��Ʈ�� ����
	{
		temp = (p_bit >> i) & 0x01;
		printf("%d ", temp);
	}
	printf("\n");

	c_bit = Feistel_Enc(p_bit);		// Feistel ��ȣȭ

									// ��ȣ�� ���
	printf("* ��ȣ�� : ");
	for (i = BLOCK_SIZE - 1; i >= 0; i--)	// ������ �� ��Ʈ�� ����
	{
		temp = (c_bit >> i) & 0x01;
		printf("%d ", temp);
	}
	printf("\n");

	d_bit = Feistel_Dec(c_bit);		// Feistel ��ȣȭ

									// ��ȣ�� ���
	printf("* ��ȣ�� : ");
	for (i = BLOCK_SIZE - 1; i >= 0; i--)	// ������ �� ��Ʈ�� ����
	{
		temp = (d_bit >> i) & 0x01;
		printf("%d ", temp);
	}
	printf("\n");
}

// Feistel ��ȣȭ
char Feistel_Enc(char in)
{
	int i;
	char temp, left, right;

	left = (in >> 4) & 0x0F;	// ���� 3bit
	right = in & 0x0F;			// ������ 3bit

	for (i = 0; i<ROUND_NUM; i++)
	{
		if (i == 0)
			left = left ^ F1(right);	// 1����
		else if (i == 1)
			left = left ^ F2(right);	// 2����
		else if (i == 2)
			left = left ^ F3(right);	// 2����

										// ������ ����� ������ ���� ����
		if (i != ROUND_NUM - 1)
		{
			temp = left;
			left = right;
			right = temp;
		}
	}

	return (left << 4) | right;		// 3bit left�� right�� ���� 6bit�� ����
}

// Feistel ��ȣȭ
char Feistel_Dec(char in)
{
	int i;
	char temp, left, right;

	left = (in >> 4) & 0x0F;	// ���� 3bit
	right = in & 0x0F;			// ������ 3bit

	for (i = 0; i<ROUND_NUM; i++)
	{
		// ��ȣȭ������ Ű�� �������� �����
		if (i == 0)
			left = left ^ F3(right);	// 1����
		else if (i == 1)
			left = left ^ F2(right);	// 2����
		else if (i == 2)
			left = left ^ F1(right);	// 2����

										// ������ ����� ������ ���� ����
		if (i != ROUND_NUM - 1)
		{
			temp = left;
			left = right;
			right = temp;
		}
	}

	return (left << 4) | right;		// 3bit left�� right�� ���� 6bit�� ����
}
// F �Լ� 1
char F1(char input)
{
	char temp;

	temp = ((~(input) & 0xF) | 0x01);
	return temp;
}
// F �Լ� 2
char F2(char input)
{
	char temp;

	temp = ((~(input) & 0xF) | 0x02);
	return temp;
}
// F �Լ� 3
char F3(char input)
{
	char temp;

	temp = ((~(input) & 0xF) | 0x03);
	return temp;
}