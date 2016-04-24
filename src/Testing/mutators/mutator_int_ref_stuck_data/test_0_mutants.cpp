////////////////////////////// START MUTANT ////////////////////////////
// MUTATION_LOCATION: 21:10
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
  retval = (ctxPtr->datalen * 10) + ctxPtr->datalen;

  retval = ctx.state[0] + 10;

  return (retval & (WORD)0x00);
}
//SYNTAX_CHECK: PASS
////////////////////////////// END MUTANT //////////////////////////////
////////////////////////////// START MUTANT ////////////////////////////
// MUTATION_LOCATION: 21:10
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
  retval = (ctxPtr->datalen * 10) + ctxPtr->datalen;

  retval = ctx.state[0] + 10;

  return (retval | (WORD)0xFFFFFFFFFFFFFFFF);
}
//SYNTAX_CHECK: PASS
////////////////////////////// END MUTANT //////////////////////////////
