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

  return retval;
}
