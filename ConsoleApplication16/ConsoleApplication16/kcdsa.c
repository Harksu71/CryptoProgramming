#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int exp(int X, int E, int N);
int get_integer(void);
int Hash(int a, int q);

int main()
{
	int p = 173, q = 43, g = 106;
	int x = 31, y = 149;
	int k, w, R, M, H, E, s;
	int H_p, E_p, w_1, w_2, w_p, R_p;

	printf("랜덤 수 (<q)\n");
	k = get_integer();
	w = exp(g, k, p);
	R = Hash(w, q);

	printf("메시지 \n");
	M = get_integer();
	H = Hash(M, q);

	E = R^H;
	if ((k - E) < 0) k = k + q;
	s = (x*(k - E)) % q;

	printf("\nM=%d, R=%d, s=%d \n", M, R, s);

	H_p = Hash(M, q);
	E_p = R^H_p;

	w_1 = exp(g, E_p, p);
	w_2 = exp(y, s, p);

	w_p = (w_1 *w_2) % p;
	R_p = Hash(w_p, q);

	printf("\nw_p=%d, R_p=%d, H_p=%d, E_p=%d, \n", w_p, R_p, H_p, E_p);

	if (R_p == R) printf("\n*서명 OK \n");
	else printf("\n*서명 Invalid \n");
}

int Hash(int a, int q)
{
	int n;

	n = (a * 997) % q;

	return n;
}



int get_integer(void)
{
	int n;
	printf("정수를 입력하시오: ");
	scanf(" %d", &n);
	return n;
}



int exp(int X, int E, int N)
{
	int i, bit = 0, B, C, T;

	B = E;
	while (B != 0) {
		B = B / 2;
		bit++;
	}

	C = 1;
	T = X;
	for (i = 0; i <bit; i++) {
		if ((E % 2) == 1) C = (C*T) % N;
		T = (T*T) % N;
		E = E / 2;
	}
	return C;
}
