#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main()
{
	int base = 0x9A;
	int result, result1, result2;
	int temp = 0;
	int left = 0;
	int right = 0;


	printf("�� : ");
	for (int i = 7; i >= 0; i--)
	{
		result = (base >> i) & 0x01;
		printf("%d", result);
	}
	printf("\n");

	result = (base >> 3) & 0x01;
	printf("�� : %d\n",result);

	result1 = (base&0xF0) >>4 ;
	printf("�� -1 : %x\n", result1);

	result2 = (base & 0x0F);
	printf("�� -2 : %x\n", result2);
	

	left = (result1 ^ 0x0B);
	printf("�� -1 : %d\n", left);
	right = (result2 ^ 0x0F);
	printf("�� -2 : %d\n",right);

	temp = right;
	right = left;
	left = temp;
	printf("�� -1 : %d\n", left);
	printf("�� -2 : %d\n", right);

	result = (left << 4) | right;
	printf("�� : %x\n", result);

}
