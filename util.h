#include <stdint.h>
#include <string.h>
typedef int errno_t;
errno_t memset_s(void *, size_t, int, size_t);
sgx_status_t sgx_read_rand(unsigned char *rand, size_t length_in_bytes);
int do_rdrand(uint32_t* eax);
int consttime_memequal(const void *b1, const void *b2, size_t len);
