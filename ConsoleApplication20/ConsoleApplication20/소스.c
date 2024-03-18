#define _CRT_SECURE_NO_WARNINGS

#include   <stdio.h>
#include   <stdlib.h>
#include  "kcdsa_sub.c"

/* 상수 정의 */
INT64 P[mpb];			// 소수 p
INT64 Q[mqb];			// 소수 q
INT64 G[mb];			// 원시 원소 g
INT64 SEC[mqb];			// 비밀 키 x
INT64 INV_SEC[mqb];		// 비밀 키 x의 역원
INT64 PUB[mpb];			// 공개 키 y
INT64 W[mpb];
INT64 V[mpb];

INT64 K[mqb];			// 랜덤 수 k
INT64 R[mqb];
INT64 H[mqb];
INT64 E[mqb];
INT64 T[mqb];
INT64 S[mqb];

// K, R, S의 이진 형태
short  k[mq], r[mq], s[mq];

// 파일 이름
char source[20], sign[20];

// 파일 포인터
FILE *fptr, *fptw;

void main(void)

{
	int  i;
	INT64 K_Temp[mb] = { 0, }, E_Temp[mb] = { 0, }, S_Temp[mb] = { 0, }, V1[mb] = { 0, }, V2[mb] = { 0, };


	printf("****************************************************");
	printf("\n********  KCDSA Signature and Verification  ********");
	printf("\n****************************************************");

	// p 값 초기화
	P[mpb - 1] = 0x8df2a494;
	P[mpb - 2] = 0x492276aa;
	P[mpb - 3] = 0x3d25759b;
	P[mpb - 4] = 0xb06869cb;
	P[mpb - 5] = 0xeac0d83a;
	P[mpb - 6] = 0xfb8d0cf7;
	P[mpb - 7] = 0xcbb8324f;
	P[mpb - 8] = 0xd7882e5;
	P[mpb - 9] = 0xd0762fc5;
	P[mpb - 10] = 0xb7210eaf;
	P[mpb - 11] = 0xc2e9adac;
	P[mpb - 12] = 0x32ab7aac;
	P[mpb - 13] = 0x49693dfb;
	P[mpb - 14] = 0xf83724c2;
	P[mpb - 15] = 0xec0736ee;
	P[mpb - 16] = 0x31c80291;

	// q 값 초기화
	Q[mqb - 1] = 0xc773218c;
	Q[mqb - 2] = 0x737ec8ee;
	Q[mqb - 3] = 0x993b4f2d;
	Q[mqb - 4] = 0xed30f48e;
	Q[mqb - 5] = 0xdace915f;

	// g 값 초기화
	G[mb - 1] = 0x626d0278;
	G[mb - 2] = 0x39ea0a13;
	G[mb - 3] = 0x413163a5;
	G[mb - 4] = 0x5b4cb500;
	G[mb - 5] = 0x299d5522;
	G[mb - 6] = 0x956cefcb;
	G[mb - 7] = 0x3bff10f3;
	G[mb - 8] = 0x99ce2c2e;
	G[mb - 9] = 0x71cb9de5;
	G[mb - 10] = 0xfa24babf;
	G[mb - 11] = 0x58e5b795;
	G[mb - 12] = 0x21925c9c;
	G[mb - 13] = 0xc42e9f6f;
	G[mb - 14] = 0x464b088c;
	G[mb - 15] = 0xc572af53;
	G[mb - 16] = 0xe6d78802;

	// 비밀 키 x 값 초기화
	SEC[mqb - 1] = 0x6d52b4c6;
	SEC[mqb - 2] = 0x63c3c2f7;
	SEC[mqb - 3] = 0x70e4c470;
	SEC[mqb - 4] = 0x68f3efb9;
	SEC[mqb - 5] = 0x44d0e276;

	// 공개 키 y 값 계산
	Inv(SEC, INV_SEC, Q, mqb);	// 비밀 키 x의 역원 계산

								// y = g^x-1 mod p 계산
	LeftTORight_Pow_2(G, INV_SEC, PUB, P, mpb, mqb);

	/****************************************************************/
	/******   Messages to be signed are read from the file.    ******/
	/****************************************************************/

	// 서명할 파일명 입력
	printf("\n*서명할 파일명을 입력하시오 : ");
	scanf("%s", source);

	// 파일 열기
	if ((fptr = fopen(source, "rb")) == NULL)
	{
		printf("File open error!\n");
		exit(1);
	}

	// 서명 데이터를 저장할 파일명 설정
	// (서명할 파일명.sgn)
	for (i = 0; i < 20; i++)
	{
		if (source[i] == '.')
			break;
		else
			sign[i] = source[i];
	}

	strcat(sign, ".sgn");

	/***************************************************************/
	/*****   Generate a random  k,  where  0 < k < q           *****/
	/***************************************************************/
	while (1)
	{
		rand_g(k, mq);			// 랜덤 수 생성
		CONV_B_to_R(k, K, mqb);	// 이진 형태의 데이터를 Radix 형태로 변환

								// k 값이 q 보다 작은지 비교
		if (Comp16(K, Q, mqb) == -1)
			break;
	}

	K[0] = 0x0123; K[1] = 0x4567; K[2] = 0x89ab; K[3] = 0xcdef;   K[4] = 0x1111;

	printf("\n 랜덤 수 K  : ");
	for (i = 0; i < mqb; i++) {
		printf("%I64x ", K[i]);
	}
	printf("\n");


	/***************************************************************/
	/******       Compute   W =  G^K  mod p                     ****/
	/***************************************************************/

	for (i = 0; i < mqb; i++)
		K_Temp[i] = K[i];

	LeftTORight_Pow_2(G, K_Temp, W, P, mpb, mqb);	// g^k 계산

													// 임시 파일인 temp.dat을 생성하여 W 값을 저장
	fptw = fopen("temp.dat", "wb");
	for (i = 0; i < mpb; i++)
		fprintf(fptw, "%I64x", W[i]);
	fclose(fptw);

	// temp.dat 파일에서 저장된 W 값을 읽는다
	fptr = fopen("temp.dat", "rb");
	HAS_160(fptr, R);
	fclose(fptr);

	// R이 Q보다 작을 때까지 R을 Q로 빼준다
	while (Comp16(R, Q, mqb) == 1)
		Sub16(R, Q, R, mqb);

	/***************************************************************/
	/****       Compute  H = h(message), where  h() is HAS-160  ****/
	/***************************************************************/

	// 메시지를 HAS-160으로 해쉬한다
	fptr = fopen(source, "rb");
	HAS_160(fptr, H);
	fclose(fptr);

	// H가 Q보다 작을 때까지 H를 Q로 빼준다
	while (Comp16(H, Q, mqb) == 1)
		Sub16(H, Q, H, mqb);

	// E = R xor H
	for (i = 0; i < mqb; i++)
		E[i] = R[i] ^ H[i];

	// E가 Q보다 작을 때까지 E를 Q로 빼준다
	while (Comp16(E, Q, mqb) == 1)
		Sub16(E, Q, E, mqb);

	/***************************************************************/
	/****       Compute  T = K - E (mod q)                     *****/
	/***************************************************************/

	Sub_mod16(K, E, T, Q, mqb);

	/***************************************************************/
	/*****       Compute T = X * E  (mod q)                    *****/
	/***************************************************************/

	Conv_mma(SEC, T, S, Q, mqb);

	/***************************************************************/
	/*****     Transmit  the (message, r, s)  to the another   *****/
	/***************************************************************/

	// Radix 형태를 이진 형태로 변환
	CONV_R_to_B(R, r, mqb);
	CONV_R_to_B(S, s, mqb);

	// 서명 데이터를 저장할 파일을 생성
	if ((fptw = fopen(sign, "wb")) == NULL)
	{
		printf("\nFile open errer.\n");
		exit(1);
	}

	// 서명 값을 저장한다
	for (i = mqb - 1; i >= 0; i--)
		fprintf(fptw, "%I64x ", R[i]);
	for (i = mqb - 1; i >= 0; i--)
		fprintf(fptw, "%I64x ", S[i]);
	fclose(fptw);

	// 서명문 출력
	printf("\n서명문 R : ");
	for (i = 0; i < mqb; i++)
		printf("%I64x ", R[i]);
	printf("\n");

	printf("\n서명문 S : ");
	for (i = 0; i < mqb; i++)
		printf("%I64x ", S[i]);
	printf("\n");

	printf("\n\nThe singnature is completed.\n");
	system("del temp.dat");		// 임시 파일인 temp.dat 삭제

	/****************************************************************/
	/********              VERIFICATION  PROCEDURES            ******/
	/****************************************************************/

	// 검증할 파일을 연다
	if ((fptr = fopen(sign, "rb")) == NULL)
	{
		printf("\nFile open errer.\n");
		exit(1);
	}
	/***************************************************************/
	/*****     Receive  the (message, r, s)                    *****/
	/***************************************************************/

	// 서명 값을 읽어온다
	for (i = mqb - 1; i >= 0; i--)
		fscanf(fptr, "%I64x ", &R[i]);
	for (i = mqb - 1; i >= 0; i--)
		fscanf(fptr, "%I64x ", &S[i]);
	fclose(fptr);

	/***************************************************************/
	/****       Compute  H = h(message), where  h() is HAS-160  ****/
	/***************************************************************/

	// 수신하나 메시지를 HAS-160으로 해쉬한다
	fptr = fopen(source, "rb");
	HAS_160(fptr, H);
	fclose(fptr);

	// H가 Q보다 작을 때까지 H를 Q로 빼준다
	while (Comp16(H, Q, mqb) == 1)
		Sub16(H, Q, H, mqb);

	// E = R xor H
	for (i = 0; i<mqb; i++)
		E[i] = R[i] ^ H[i];

	// E가 Q보다 작을 때까지 E를 Q로 빼준다
	while (Comp16(E, Q, mqb) == 1)
		Sub16(E, Q, E, mqb);

	/***************************************************************/
	/*****       Compute T = g^E * yA^s (mod p)                *****/
	/***************************************************************/

	for (i = 0; i<mqb; i++)
		E_Temp[i] = E[i];
	for (i = 0; i<mqb; i++)
		S_Temp[i] = S[i];

	// g^E과 yA^s 계산
	LeftTORight_Pow_2(G, E_Temp, V1, P, mpb, mqb);
	LeftTORight_Pow_2(PUB, S_Temp, V2, P, mpb, mqb);

	Conv_mma(V1, V2, V, P, mpb);	// g^E * yA^s

									// 임시 파일 temp.dat를 생성하여 g^E * yA^s 값 저장
	fptw = fopen("temp.dat", "wb");
	for (i = 0; i<mpb; i++)
		fprintf(fptw, "%I64x", V[i]);
	fclose(fptw);

	// temp.dat 파일에서 저장된 g^E * yA^s 값을 읽어
	// HAS-160으로 해쉬 한다
	fptr = fopen("temp.dat", "rb");
	HAS_160(fptr, T);
	fclose(fptr);

	// T가 Q보다 작을 때까지 T를 Q로 빼준다
	while (Comp16(T, Q, mqb) == 1)
		Sub16(T, Q, T, mqb);

	// 서명이 정당한지 확인한다
	if (Comp16(R, T, mqb) != 0)
	{
		printf("\n\nThe verification is NOT CLEAR.\n");
		exit(0);
	}
	fclose(fptr);

	printf("\n\nThe verification is CLEAR.\n");
	system("del temp.dat");	// 임시 파일인 temp.dat 삭제
}
