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
