#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main()
{
	char  ch, in[4] = { 0 }, temp;
	int i = 0, low = 0, high = 0;
	unsigned long int test = 0;

	printf("���ϴ� 4���ڸ� �Է��Ͻÿ� : ");

	for (int i = 0; i < 4; i++)
	{
		scanf(" %c", &ch);
		in[i] = ch;
	}
	printf("�� : %x %x %x %x\n", in[0], in[1], in[2], in[3]);

	temp = in[3];

	for (int i = 2; i >= 0; i--)
	{
		in[i + 1] = in[i];
	}
	in[0] = temp;

	printf("�� : %x %x %x %x\n", in[0], in[1], in[2], in[3]);

	for (int i = 0; i < 4; i++)
	{
		low = in[i] & 0xF;
		high = (in[i] & 0xF0) >> 4;
		in[i] = (low << 4) | high;
	}
		printf("�� : %x %x %x %x\n", in[0], in[1], in[2], in[3]);

	test = (in[0] << 24) + (in[1] << 16) + (in[2] << 8) + in[3];
	printf("�� : %x\n", test);
}
