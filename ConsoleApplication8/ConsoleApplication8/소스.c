#include <stdio.h>

/* 타입 정의 */
typedef unsigned int WORD;
typedef unsigned char BYTE;

/* 매크로 함수 */
#define HIHEX(x) ( x >> 4 )			// 8bit에서 상위 4bit 값을 구하는 함수
#define LOWHEX(x) ( x & 0x0F )		// 8bit에서 하위 4bit 값을 구하는 함수

/* 함수 선언 */
void SubBytes(BYTE state[][4]);								// SubBytes
void ShiftRows(BYTE state[][4]);							// ShiftRows
void MixColumns(BYTE state[][4]);							// MixColumns
void Inv_SubBytes(BYTE state[][4]);							// Invers SubBytes
void Inv_ShiftRows(BYTE state[][4]);						// Invers ShiftRows
void Inv_MixColumns(BYTE state[][4]);						// Invers MixColumns
void AddRoundKey(BYTE state[][4], WORD*);
void CirShiftRows(BYTE* row);		// state의 한 행을 1회 오른쪽으로 순환 시프트
void Inv_CirShiftRows(BYTE* row);		// state의 한 행을 1회 왼쪽으로 순환 시프트
BYTE x_time_1(BYTE n, BYTE b);

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

void main()
{
	int i, j;
	BYTE p_text[16] = { 's', 'e', 'c', 'u', 'r', 'i', 't', 'y', '1', '2', '3', '4', '5', '6', '7', '8' };
	BYTE c_text[16] = { 0, };
	BYTE inv_c_text[16] = { 0, };
	BYTE state[4][4];

	WORD W[4] = { 0, };
	W[0] = 0x12345678;
	W[1] = 0x23456789;
	W[2] = 0x34567890;
	W[3] = 0x45678901;

	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			state[j][i] = p_text[i * 4 + j];

	SubBytes(state);
	ShiftRows(state);
	MixColumns(state);
	AddRoundKey(state, W);


	// 결과 값 저장
	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			c_text[i * 4 + j] = state[j][i];

	// 암호문 출력
	printf("\n* 암호문 : ");
	for (i = 0; i<16; i++)
		printf("%x ", c_text[i]);
	printf("\n");

	////////////////////////////////////////////
	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			state[j][i] = c_text[i * 4 + j];

	AddRoundKey(state, W);
	Inv_MixColumns(state);
	Inv_ShiftRows(state);
	Inv_SubBytes(state);


	// 결과 값 저장
	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			inv_c_text[i * 4 + j] = state[j][i];

	// 복호문 출력
	printf("\n* 복호문 : ");
	for (i = 0; i<16; i++)
		printf("%c ", inv_c_text[i]);
	printf("\n");
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

// ShiftRows
void ShiftRows(BYTE state[][4])
{
	int i, j;

	// state[4][4]에서 
	// 첫번째 행은 시프트가 없고,
	// 두번째 행은 1번 시프트,
	// 세번째 행은 2번 시프트,
	// 네번째 행은 3번 시프트 함
	for (i = 1; i<4; i++)
		for (j = 0; j<i; j++)
			CirShiftRows(state[i]);
}

// Mixcolumns
void MixColumns(BYTE state[][4])
{
	int i, j, k;
	BYTE a[4][4] = { 0x02, 0x03, 0x01, 0x01,		// a(x) = 03x^3 + 01x^2 + 01x + 02
		0x01, 0x02, 0x03, 0x01,
		0x01, 0x01, 0x02, 0x03,
		0x03, 0x01, 0x01, 0x02 };
	BYTE b[4][4] = { 0, };


	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
			for (k = 0; k<4; k++)
				b[i][j] ^= x_time_1(a[i][k], state[k][j]);	// 곱셈은 x_time 함수를 통해서 수행
	}
	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++) state[i][j] = b[i][j];
}

// AddRoundKey
void AddRoundKey(BYTE state[][4], WORD* rKey)
{
	int i, j;
	WORD mask, shift;

	// state와 라운드 키의 XOR 연산
	for (i = 0; i<4; i++)
	{
		shift = 24;
		mask = 0xFF000000;

		// state는 byte(8bit) 단위이고, 라운드 키는 word(32bit) 단위이므로
		// 라운드 키를 상위비트부터 8bit씩 추출하여 XOR 연산을 함
		for (j = 0; j<4; j++)
		{
			state[j][i] = ((rKey[i] & mask) >> shift) ^ state[j][i];
			mask >>= 8;
			shift -= 8;
		}
	}
}


// Invers SubBytes
void Inv_SubBytes(BYTE state[][4])
{
	int i, j;

	// state의 하나의 값은 1byte 이므로 이 8bit 값을 상위 4bit, 하위 4bit로 나누어
	// 상위 비트는 Inv_S_box의 행 번호로, 하위 비트는 열 번호로 사용함
	// (예: state[i][j] = 10100011(2) -> 상위 : 1010(2) = 10, 하위 : 0011(2) = 3 -> Inv_S_box[10][3])
	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			state[i][j] = Inv_S_box[HIHEX(state[i][j])][LOWHEX(state[i][j])];
}

// Invers ShiftRows
void Inv_ShiftRows(BYTE state[][4])
{
	int i, j;

	// state[4][4]에서 
	// 첫번째 행은 시프트가 없고,
	// 두번째 행은 1번 시프트,
	// 세번째 행은 2번 시프트,
	// 네번째 행은 3번 시프트 함
	for (i = 1; i<4; i++)
		for (j = 0; j<i; j++)
			Inv_CirShiftRows(state[i]);
}


// state의 한 행을 한 바이트씩 오른쪽으로 순환 시프트함
void CirShiftRows(BYTE* row)
{
	BYTE temp = row[0];

	row[0] = row[1];
	row[1] = row[2];
	row[2] = row[3];
	row[3] = temp;
}

// state의 한 행을 한 바이트씩 왼쪽으로 순환 시프트함
void Inv_CirShiftRows(BYTE* row)
{
	BYTE temp = row[3];

	row[3] = row[2];
	row[2] = row[1];
	row[1] = row[0];
	row[0] = temp;
}

// Inverse MixColumns
void Inv_MixColumns(BYTE state[][4])
{
	int i, j, k;
	BYTE a[4][4] = { 0x0E, 0x0B, 0x0D, 0x09,		// a^-1(x) = 0bx^3 + 0dx^2 + 09x + 0e
		0x09, 0x0E, 0x0B, 0x0D,
		0x0D, 0x09, 0x0E, 0x0B,
		0x0B, 0x0D, 0x09, 0x0E };
	BYTE b[4][4] = { 0, };

	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
			for (k = 0; k<4; k++)
				b[i][j] ^= x_time_1(a[i][k], state[k][j]);	// 곱셈은 x_time 함수를 통해서 수행
	}
	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++) state[i][j] = b[i][j];

}


// GF(2^8)에서의 곱셈 연산 
BYTE x_time_1(BYTE n, BYTE b)
{
	int i;
	BYTE temp = 0, mask = 0x01;

	for (i = 0; i<8; i++)
	{
		if (n & mask)
			temp ^= b;

		// 최상위 bit가 1이면 시프트 후 0x1B(x^8 + x^4 + x^3 + x^2 + 1)를 XOR 해줌
		if (b & 0x80)
			b = (b << 1) ^ 0x1B;
		else
			b <<= 1;

		mask <<= 1;
	}

	return temp;
}