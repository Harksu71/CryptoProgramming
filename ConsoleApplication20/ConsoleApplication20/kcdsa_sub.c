/*******************************************************************/
/********     Subprograms for KCDSA                            *****/
/*******************************************************************/

#include <stdio.h>
#include <time.h>
#include <dos.h>
#include  "has160.c"

/* 상수 정의 */
#define  m    512       	/*** the number of bits of the modulus n      ***/
#define  mp   512       	/*** the number of bits of the modulus p      ***/
#define  mq   160       	/*** the number of bits of the modulus q      ***/
#define  HASH 160       	/*** the number of hash function              ***/
#define  DHEX 32
#define  Char_NUM 8
#define  Leng 512

#define  mb   m/DHEX
#define  mpb  mp/DHEX
#define  mqb  mq/DHEX

#define  rdx  0x100000000 	/*** the base of data used in multiplication  ***/
typedef  unsigned long int ULINT; 	 /*** define the type of variables    ***/
typedef unsigned _int64 INT64;       /*** define the type of variables    ***/
typedef unsigned _int32 INT32;
INT32  LAND=0xFFFFFFFF; 	 /*** for modular rdx                         ***/

#define  false -1
#define  true 1
#define  w_size 5       	/*** the size of window                       ***/
#define  t_size 16      	/*** the size of table                        ***/


/********************************************************************/
/***********   Function name :  Sum16 (A,B,C,N,mn)         **********/
/***********   Description   :  C = A + B mod N            **********/
/********************************************************************/
void Sum16(INT64 *A,INT64 *B,INT64 *C,INT64 *N, short mn)
{
  register  i,j,posit;
  INT64  acumA, acumQ;
  INT64  T[mb+1],arryN[mb+1];
  INT32  acumC;

    for(i=mn-1;i>=0;i--)  C[i]=0x00;
    acumC = 0x00;
    for(i=0;i<=mn-1;i++) {
         acumA = A[i] + B[i] + acumC;
         T[i]  = acumA &LAND;
         acumC = acumA >>DHEX;
	}
    T[i] = acumC;

    if(T[mn]==0x00) {
         if(Comp16(T,N,mn)<0) {
              for(i=mn-1;i>=0;i--) C[i]=T[i];
              return;
         }
    }

    acumQ=(T[mn]*rdx+T[mn-1])/(N[mn-1]+0x01);
    if(acumQ==0) acumQ=1;
    if(acumQ>(rdx-1))  acumQ = rdx-1;

    acumC=0x00;
    for(i=0;i<=mn-1;i++) {
         acumA = N[i]*acumQ + acumC;
         arryN[i]=acumA &LAND;
         acumC = acumA >>DHEX;
    }
    arryN[i] = acumC;

    for(i=0;i<=mn-1;i++) {
         if(T[i]<arryN[i]) {
              T[i]   = T[i]   +  rdx;
              posit=i;
              while(T[i+1]==0 && i<mn){
                   T[i+1]+=rdx-1;
                   i++;
              }
          T[i+1]-=0x01;
          i=posit;
         }
    T[i] = T[i] - arryN[i];
    }
    for(i=mn-1;i>=0;i--)  C[i] = T[i];
}

/********************************************************************/
/***********   Function name :  Sub16 (A,B,C,mn)           **********/
/***********   Description   :  C = A - B                  **********/
/********************************************************************/
void Sub16 (INT64 *A,INT64 *B,INT64 *C,short mn)
{
  register i,k,posit;
  INT64  T[mb];

    for(i=mn-1;i>=0;i--)  T[i] = A[i];
    for(i=0;i<=mn-1;i++) {
         if(T[i]<B[i]) {
              T[i]   = T[i]   +  rdx;
              posit=i;
              while(T[i+1]==0 && i<mn){
                   T[i+1]+=rdx-1;
                   i++;
              }
          T[i+1]-=0x01;
          i=posit;
         }
    C[i] = T[i] - B[i];
    }
}

/********************************************************************/
/***********   Function name :  Sub_mod16 (A,B,C,N)        **********/
/***********   Description   :  C  =  A - B  (mod N)       **********/
/***********      If  A >= B   then   C = A - B            **********/
/***********      else     C = A + (N - B).                **********/
/********************************************************************/
void Sub_mod16 (INT64 *A,INT64 *B,INT64 *C,INT64 *N, short mn)
{
  INT64  T[mb];
  short i;

  /*** If  a >= b then c = a - b
                  else c = a + (q - b)   ***/
    if(Comp16(A, B, mn)>=0) Sub16 (A, B, C, mn);
    else  {
       Sub16 (N, B, T, mn);
       Sum16 (A, T, C, N,mn);
    }
}

/********************************************************************/
/***********   Function name :  Comp16 (A,B,mn)            **********/
/***********   Description   :  If A > B,  return  1       **********/
/***********                       A = B,  return  0       **********/
/***********                       A < B,  return -1       **********/
/********************************************************************/
int Comp16(INT64 *A,INT64 *B,short mn)
{
  register i,j;
  int  diff, sa, sb;

    for(i=mn-1;i>=0;i--)
         if(A[i]>0x00)   break;
	sa = i + 1;
    for(j=mn-1;j>=0;j--)
         if(B[j]>0x00)   break;
	sb = j + 1;
	diff = sa - sb;

	if(diff<0)    return (-1);
	if(diff>0)    return  (1);
	else {
      for(i=mn-1;i>=0;i--) {
		if(A[i] != B[i]) {
			if(A[i]>B[i])     return (1);
			else              return(-1);
		 }
	   }
	  return(0);
	 }
}

/********************************************************************/
/***********   Function name :  CONV_B_to_R (a,B,mn)       **********/
/***********   Description   :  convert bin. into radix    **********/
/********************************************************************/
INT64 mask[DHEX]={0x80000000, 0x40000000, 0x20000000, 0x10000000,0x8000000,
				  0x4000000,0x2000000, 0x1000000, 0x800000,0x400000, 0x200000,
				  0x100000, 0x080000,0x040000, 0x020000, 0x010000,
				  0x8000, 0x4000, 0x2000, 0x1000,0x800,
				  0x400,0x200, 0x100, 0x80,0x40, 0x20,
				  0x10, 0x08,0x04, 0x02, 0x01
				 };

void CONV_B_to_R (short *A,INT64 *B,short mn)
{
	register i, j, k;

	for(i=mn-1;i>=0;i--)  B[i]=0x00;

	i = mn*DHEX-1;
	for(k=0;k<=mn-1;k++) {
		B[k] = 0x00;
		for(j=DHEX-1;j>=0;j--) {
			B[k] += A[i--] * mask[j];
			if(i<0)  break;
		}
		if(i<0) break;
	}
}

/********************************************************************/
/***********   Function name :  CONV_R_to_B (A,b,mn)       **********/
/***********   Description   :  convert radix into bin.    **********/
/********************************************************************/
void CONV_R_to_B (INT64 *A,short *B,short mn)
{
	register i, j, k;

	for(i=0;i<mn*DHEX;i++)  B[i]=0;
	k = mn*DHEX-1;
	for(i=0;i<=mn-1;i++) {
		for(j=0;j<=DHEX-1;j++) {
			B[k--] = (A[i]>>j)&0x01;
			if(k<0)  break;
		}
		if(k<0) break;
	}
}

/********************************************************************/
/***********   Function name :  rand_g (a,n)               **********/
/***********   Description   : n-bits random               **********/
/***********                   number generator.           **********/
/********************************************************************/
void rand_g(short *out,short n)
{
	register  j,k;
	short x;
	long t;

    srand((unsigned)(time(NULL)));
//	srand((unsigned)(time(&t)));
	//delay(100);

	j=0;
	while(1) {
		x = rand();
		for(k=0;k<15;k++) {
			out[n-1-j] = (x>>k) & 0x01;
			j++;
			if(j>=n)   return;
		}
	}
}

/********************************************************************/
/*****     Function name : Modular(C, N mn)                     *****/
/*****     Description   : C = C mod N                          *****/
/********************************************************************/
void Modular (INT64 *X, INT64 *N, short mn)
{
	register  i, j, k;
	short shift,posit;
	INT64  arryA[2*mb+1]={0,},arryN[2*mb+1]={0,};
	INT64  acumA, acumB, acumN, acumQ;
	INT32  acumC;

	acumN = N[mn-1] + 0x01;

	while(1) {
		for(k=2*mn-1;k>=0;k--)
			if(X[k]>0x00)
				break;
		if(k<=mn-1)
			break;

		acumA = X[k]*rdx+X[k-1];
		acumQ = acumA / acumN;

		if(acumQ>(rdx-1))
			acumQ = rdx-1;

		shift = k - mn;   /**  shift number **/

		acumC = 0x00;
		for(k=0;k<=mn-1;k++) {
			acumA = N[k]*acumQ + acumC;
			acumC = acumA >>DHEX;
			acumA = acumA & LAND;
			j=k+shift;
			if(X[j]<acumA) {
				X[j]   += rdx;
				posit=j;
				while((X[j+1])==0 && (j<(mn+shift))){
					X[j+1] += rdx-1;
					j++;
				}
				X[j+1] -= 0x01;
				j=posit;
			}
			X[j] = (X[j]-acumA) & LAND;
		}
		X[mn+shift] = X[mn+shift]-acumC;
	}

	while(1) {
		for(i=mn-1;i>=0;i--) {
			if((X[i]&LAND) != (N[i]&LAND)) {
				if((X[i]&LAND)>(N[i]&LAND))
					break;
				else
					return(0);
			}
		}

		acumA = X[mn-1];
		acumA = acumA / acumN;

		if(acumA==0x00) {
			for(i=0;i<=mn-1;i++) {
				if(X[i]<N[i]) {
					X[i] += rdx;
					posit = i;
					while( (X[i+1]==0) && (i<mn)) {
						X[i+1] += rdx-1;
						i++;
					}
					X[i+1] -= 0x01;
					i = posit;
				}
				X[i] = (X[i]-N[i])&LAND;
			}
		}

		else {
			acumC = 0x00;
			for(i=0;i<=mn-1;i++) {
				acumB = N[i]*acumA + acumC;
				acumC = acumB>>DHEX;
				acumB = acumB & LAND;
				if(X[i]<acumB) {
					X[i] += rdx;
					posit = i;
					while( (X[i+1]==0) && (i<mn)) {
						X[i+1] += rdx-1;
						i++;
					}
					X[i+1] -= 0x01;
					i = posit;
				}
				X[i] = (X[i]-acumB)&LAND;
			}
		}
	}
}

/********************************************************************/
/*****     Function name : Conv_mma(A,B,C,N,mn) (Conventional)  *****/
/*****     Description   : C= A*B mod N                         *****/
/********************************************************************/
void Conv_mma (INT64 *A,INT64 *B,INT64 *C,INT64 *N, short mn)
{
	register  i, j, k;
	INT64 arryC[mb*2],X[mb*2];         /** temporary arrys **/
	INT64 acumA;                     /** temporary acumulators **/
	INT32 acumC;

	for(k=2*mn-1;k>=0;k--)  arryC[k] = 0x00;

	for(i=0;i<=mn-1;i++) {
		if(A[i]>0x00) {
			acumC = 0x00;
			for(j=0;j<=mn-1;j++) {
				acumA = A[i]*B[j] + arryC[i+j] + acumC;
				arryC[i+j] = acumA &LAND;
				acumC = acumA >>DHEX;
			}
			arryC[i+j] = acumC;
		}
	}

	for(i=2*mn-1;i>=0;i--)
		X[i]=arryC[i];

	Modular (X, N, mn);

	for(i=0;i<=mn-1;i++)
		C[i] = X[i];
}


/********************************************************************/
/*****     Function name : LeftTORight_Pow(A,E,C,N,mn)          *****/
/*****     Description   : C= A^E mod N                         *****/
/********************************************************************/
void LeftTORight_Pow(INT64 *A, INT64 *E, INT64 *C, INT64 *N, short mn)
{
	register i;
	INT64 arryC[mb]={0,};
	short e[m]={0,};

	for(i=0;i<mn;i++)
		C[i] = 0x00;

	CONV_R_to_B(E, e, mn);

	arryC[0] = 0x01;

	for(i=0;i<mn*DHEX;i++)
	{
		Conv_mma(arryC, arryC, arryC, N, mn);

		if(e[i] == 1)
			Conv_mma(arryC, A, arryC, N, mn);
	}
	
	for(i=0;i<mn;i++)
		C[i] = arryC[i];
}

/********************************************************************/
/*****     Function name : LeftTORight_Pow_2(A,E,C,N,mn,nn)     *****/
/*****     Description   : C= A^E mod N                         *****/
/********************************************************************/
void LeftTORight_Pow_2(INT64 *A, INT64 *E, INT64 *C, INT64 *N, short mn,short nn)
{
	int i;
	INT64 arryC[mb]={0,};
	short e[m]={0,};

	for(i=0;i<mn;i++)
		C[i] = 0x00;

	CONV_R_to_B(E, e, nn);

	arryC[0] = 0x01;

 	for(i=0;i<nn*DHEX;i++)
	{
		Conv_mma(arryC, arryC, arryC, N, mn);

		if(e[i] == 1)
			Conv_mma(arryC, A, arryC, N, mn);
	}
	
	for(i=0;i<mn;i++)
		C[i] = arryC[i];
}

/********************************************************************/
/***********   Function name :  Inverse (A,X,N,mn)         **********/
/***********   Description   :  X = A -1 mod N             **********/
/***********                    arithmetic operation       **********/
/********************************************************************/
void Inv(INT64 *a, INT64 *x, INT64 *N, short mn)
{
    int i;
	INT64 N_2[mqb]={0,};

    for(i=0 ; i<mn ; i++) N_2[i]=N[i];
	N_2[0]-=0x02;
    
    LeftTORight_Pow(a, N_2, x, N, mn);
}
