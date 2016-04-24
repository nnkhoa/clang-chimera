////////////////////////////// START MUTANT ////////////////////////////
// MUTATION_LOCATION: 13:17
// MUTATION_TYPE: 0
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int WORD;  // 32-bit word, change to "long" for 16-bit machines

typedef struct {
  BYTE data[64];
  WORD datalen;
  unsigned long long bitlen;
  WORD state[8];
} SHA256_CTX;

WORD test_fun() {
  SHA256_CTX ctx;
  WORD retval = (ctx.datalen & (WORD)0x00) + 1;
  ctx.datalen = 10;

  SHA256_CTX* ctxPtr;
  retval = (ctxPtr->datalen * 10) + ctxPtr->datalen;

  return retval;
}
//SYNTAX_CHECK: PASS
////////////////////////////// END MUTANT //////////////////////////////
////////////////////////////// START MUTANT ////////////////////////////
// MUTATION_LOCATION: 13:17
// MUTATION_TYPE: 1
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int WORD;  // 32-bit word, change to "long" for 16-bit machines

typedef struct {
  BYTE data[64];
  WORD datalen;
  unsigned long long bitlen;
  WORD state[8];
} SHA256_CTX;

WORD test_fun() {
  SHA256_CTX ctx;
  WORD retval = (ctx.datalen | (WORD)0xFFFFFFFFFFFFFFFF) + 1;
  ctx.datalen = 10;

  SHA256_CTX* ctxPtr;
  retval = (ctxPtr->datalen * 10) + ctxPtr->datalen;

  return retval;
}
//SYNTAX_CHECK: PASS
////////////////////////////// END MUTANT //////////////////////////////
////////////////////////////// START MUTANT ////////////////////////////
// MUTATION_LOCATION: 17:13
// MUTATION_TYPE: 0
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int WORD;  // 32-bit word, change to "long" for 16-bit machines

typedef struct {
  BYTE data[64];
  WORD datalen;
  unsigned long long bitlen;
  WORD state[8];
} SHA256_CTX;

WORD test_fun() {
  SHA256_CTX ctx;
  WORD retval = ctx.datalen + 1;
  ctx.datalen = 10;

  SHA256_CTX* ctxPtr;
  retval = ((ctxPtr->datalen & (WORD)0x00) * 10) + ctxPtr->datalen;

  return retval;
}
//SYNTAX_CHECK: PASS
////////////////////////////// END MUTANT //////////////////////////////
////////////////////////////// START MUTANT ////////////////////////////
// MUTATION_LOCATION: 17:13
// MUTATION_TYPE: 1
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int WORD;  // 32-bit word, change to "long" for 16-bit machines

typedef struct {
  BYTE data[64];
  WORD datalen;
  unsigned long long bitlen;
  WORD state[8];
} SHA256_CTX;

WORD test_fun() {
  SHA256_CTX ctx;
  WORD retval = ctx.datalen + 1;
  ctx.datalen = 10;

  SHA256_CTX* ctxPtr;
  retval = ((ctxPtr->datalen | (WORD)0xFFFFFFFFFFFFFFFF) * 10) + ctxPtr->datalen;

  return retval;
}
//SYNTAX_CHECK: PASS
////////////////////////////// END MUTANT //////////////////////////////
////////////////////////////// START MUTANT ////////////////////////////
// MUTATION_LOCATION: 17:37
// MUTATION_TYPE: 0
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int WORD;  // 32-bit word, change to "long" for 16-bit machines

typedef struct {
  BYTE data[64];
  WORD datalen;
  unsigned long long bitlen;
  WORD state[8];
} SHA256_CTX;

WORD test_fun() {
  SHA256_CTX ctx;
  WORD retval = ctx.datalen + 1;
  ctx.datalen = 10;

  SHA256_CTX* ctxPtr;
  retval = (ctxPtr->datalen * 10) + (ctxPtr->datalen & (WORD)0x00);

  return retval;
}
//SYNTAX_CHECK: PASS
////////////////////////////// END MUTANT //////////////////////////////
////////////////////////////// START MUTANT ////////////////////////////
// MUTATION_LOCATION: 17:37
// MUTATION_TYPE: 1
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int WORD;  // 32-bit word, change to "long" for 16-bit machines

typedef struct {
  BYTE data[64];
  WORD datalen;
  unsigned long long bitlen;
  WORD state[8];
} SHA256_CTX;

WORD test_fun() {
  SHA256_CTX ctx;
  WORD retval = ctx.datalen + 1;
  ctx.datalen = 10;

  SHA256_CTX* ctxPtr;
  retval = (ctxPtr->datalen * 10) + (ctxPtr->datalen | (WORD)0xFFFFFFFFFFFFFFFF);

  return retval;
}
//SYNTAX_CHECK: PASS
////////////////////////////// END MUTANT //////////////////////////////
