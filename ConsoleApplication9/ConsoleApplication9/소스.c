#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 상수 정의 */
#define Nb	4				// AES 블록 크기(word)
#define Nk	4				// AES 키 길이(word)

/* 타입 정의 */
typedef unsigned int WORD;
typedef unsigned char BYTE;

/* 매크로 함수 */
#define HIHEX(x) ( x >> 4 )			// 8bit에서 상위 4bit 값을 구하는 함수
#define LOWHEX(x) ( x & 0x0F )		// 8bit에서 하위 4bit 값을 구하는 함수
#define BTOW(b0, b1, b2, b3) ( ((WORD)b0 << 24) | ((WORD)b1 << 16) | ((WORD)b2 << 8) | (WORD)b3 )	// BYTE를 WORD로 변환하는 함수

/* 함수 선언 */
void AES_Cipher(BYTE* in, BYTE* out, BYTE* key);			// AES 암호화
void AES_Inverse_Cipher(BYTE* in, BYTE* out, BYTE* key);	// AES 복호화
void SubBytes(BYTE state[][4]);								// SubBytes
void ShiftRows(BYTE state[][4]);							// ShiftRows
void MixColumns(BYTE state[][4]);							// MixColumns
void Inv_SubBytes(BYTE state[][4]);							// Invers SubBytes
void Inv_ShiftRows(BYTE state[][4]);						// Invers ShiftRows
void Inv_MixColumns(BYTE state[][4]);						// Invers MixColumns
void AddRoundKey(BYTE state[][4], WORD*);					// AddRoundKey
void KeyExpansion(BYTE* key, WORD* W);						// AES 키 확장 함수
void CirShiftRows(BYTE* row);								// state의 한 행을 1회 오른쪽으로 순환 시프트
void Inv_CirShiftRows(BYTE* row);							// state의 한 행을 1회 왼쪽으로 순환 시프트
WORD SubWord(WORD W);										// SubWord
WORD RotWord(WORD W);										// RotWord
BYTE x_time(BYTE n, BYTE b);								// GF(2^8) 상에서 곱셈 연산 함수
BYTE x_time_1(BYTE n, BYTE b);								// GF(2^8) 상에서 곱셈 연산 함수


															/* 전역 변수 */
															// 암호화 S-box
BYTE S_box[16][16] = {
	99, 124, 119, 123, 242, 107, 111, 197,  48,   1, 103,  43, 254, 215, 171, 118,
	202, 130, 201, 125, 250,  89,  71, 240, 173, 212, 162, 175, 156, 164, 114, 192,
	183, 253, 147,  38,  54,  63, 247, 204,  52, 165, 229, 241, 113, 216,  49,  21,
	4, 199,  35, 195,  24, 150,   5, 154,   7,  18, 128, 226, 235,  39, 178, 117,
	9, 131,  44,  26,  27, 110,  90, 160,  82,  59, 214, 179,  41, 227,  47, 132,
	83, 209,   0, 237,  32, 252, 177,  91, 106, 203, 190,  57,  74,  76,  88, 207,
	208, 239, 170, 251,  67,  77,  51, 133,  69, 249,   2, 127,  80,  60, 159, 168,
	81, 163,  64, 143, 146, 157,  56, 245, 188, 182, 218,  33,  16, 255, 243, 210,
	205,  12,  19, 236,  95, 151,  68,  23, 196, 167, 126,  61, 100,  93,  25, 115,
	96, 129,  79, 220,  34,  42, 144, 136,  70, 238, 184,  20, 222,  94,  11, 219,
	224,  50,  58,  10,  73,   6,  36,  92, 194, 211, 172,  98, 145, 149, 228, 121,
	231, 200,  55, 109, 141, 213,  78, 169, 108,  86, 244, 234, 101, 122, 174,   8,
	186, 120,  37,  46,  28, 166, 180, 198, 232, 221, 116,  31,  75, 189, 139, 138,
	112,  62, 181, 102,  72,   3, 246,  14,  97,  53,  87, 185, 134, 193,  29, 158,
	225, 248, 152,  17, 105, 217, 142, 148, 155,  30, 135, 233, 206,  85,  40, 223,
	140, 161, 137,  13, 191, 230,  66, 104,  65, 153,  45,  15, 176,  84, 187,  22,
};

// 복호화 S-box
BYTE Inv_S_box[16][16] = {
	82,   9, 106, 213,  48,  54, 165,  56, 191,  64, 163, 158, 129, 243, 215, 251,
	124, 227,  57, 130, 155,  47, 255, 135,  52, 142,  67,  68, 196, 222, 233, 203,
	84, 123, 148,  50, 166, 194,  35,  61, 238,  76, 149,  11,  66, 250, 195,  78,
	8,  46, 161, 102,  40, 217,  36, 178, 118,  91, 162,  73, 109, 139, 209,  37,
	114, 248, 246, 100, 134, 104, 152,  22, 212, 164,  92, 204,  93, 101, 182, 146,
	108, 112,  72,  80, 253, 237, 185, 218,  94,  21,  70,  87, 167, 141, 157, 132,
	144, 216, 171,   0, 140, 188, 211,  10, 247, 228,  88,   5, 184, 179,  69,   6,
	208,  44,  30, 143, 202,  63,  15,   2, 193, 175, 189,   3,   1,  19, 138, 107,
	58, 145,  17,  65,  79, 103, 220, 234, 151, 242, 207, 206, 240, 180, 230, 115,
	150, 172, 116,  34, 231, 173,  53, 133, 226, 249,  55, 232,  28, 117, 223, 110,
	71, 241,  26, 113,  29,  41, 197, 137, 111, 183,  98,  14, 170,  24, 190,  27,
	252,  86,  62,  75, 198, 210, 121,  32, 154, 219, 192, 254, 120, 205,  90, 244,
	31, 221, 168,  51, 136,   7, 199,  49, 177,  18,  16,  89,  39, 128, 236,  95,
	96,  81, 127, 169,  25, 181,  74,  13,  45, 229, 122, 159, 147, 201, 156, 239,
	160, 224,  59,  77, 174,  42, 245, 176, 200, 235, 187,  60, 131,  83, 153,  97,
	23,  43,   4, 126, 186, 119, 214,  38, 225, 105,  20,  99,  85,  33,  12, 125,
};

// Rcon 상수
static WORD Rcon[11] = { 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000 };

static int Nr = 10;	// 라운드 수

					/////////////////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	int i;

	BYTE key[Nk * 4 + 1] = { 0, };
	WORD W[44] = { 0, };


	// 비밀키 입력
	printf("* 비밀키 입력 : ");
	scanf("%s", key);

	KeyExpansion(key, W);	// 키 확장

							// 복호문 출력
	printf("\n* 확장 키  : ");
	for (i = 0; i<Nb * (Nr + 1); i++)
		printf("\n%d = %x", i, W[i]);
	printf("\n");
}

// AES 키 확장 함수
void KeyExpansion(BYTE* key, WORD* W)
{
	WORD temp;
	int i = 0;

	// 첫번째 word에는 입력된 키 값이 들어감
	while (i < Nk)
	{
		W[i] = BTOW(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
		i = i + 1;
	}

	i = Nk;

	// 두번째 word부터는 이전 word 값을 이용해 SubWord와 RotWord 함수, Rcon 상수, XOR 연산을 적용시켜 구함
	while (i < (Nb * (Nr + 1)))
	{
		temp = W[i - 1];
		if (i % Nk == 0)
			temp = SubWord(RotWord(temp)) ^ Rcon[i / Nk - 1];
		else if ((Nk > 6) && (i % Nk == 4))
			temp = SubWord(temp);

		W[i] = W[i - Nk] ^ temp;
		i += 1;
	}
}

// SubWord
WORD SubWord(WORD W)
{
	int i;
	WORD out = 0, mask = 0xFF000000;
	BYTE shift = 24;

	// 인자로 들어온 32bit word 값을 상위비트부터 8bit씩 추출하고,
	// 추출한 값을 상위 4bit, 하위 4bit로 나누어 S_box의 행과 열의 값으로 사용해 얻은 결과 값(8bit)을
	// 저장할 32bit word에 상위비트부터 8bit씩 채움
	for (i = 0; i<4; i++)
	{
		out += (WORD)S_box[HIHEX((W & mask) >> shift)][LOWHEX((W & mask) >> shift)] << shift;
		mask >>= 8;
		shift -= 8;
	}

	return out;
}

// RotWord (32bit word를 8bit 오른쪽 순환 시프트 수행)
WORD RotWord(WORD W)
{
	return ((W & 0xFF000000) >> 24) | (W << 8);
}

// SubBytes
void SubBytes(BYTE state[][4])
{
	int i, j;

	// state의 하나의 값은 1byte 이므로 이 8bit 값을 상위 4bit, 하위 4bit로 나누어
	// 상위 비트는 S_box의 행 번호로, 하위 비트는 열 번호로 사용함
	// (예: state[i][j] = 10100011(2) -> 상위 : 1010(2) = 10, 하위 : 0011(2) = 3 -> S_box[10][3])
	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			state[i][j] = S_box[HIHEX(state[i][j])][LOWHEX(state[i][j])];
}
