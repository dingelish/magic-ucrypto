#include "sgxcrypto.h"
#include "util.h"
#include <stdio.h>

int main() {
	printf("hello world!\n");
	int r;

	sgx_sha256_hash_t result;
	char to_be_hashed[32] = {0};

	sgx_sha256_msg(&to_be_hashed[0], 32, &result);
	
	for(int i = 0; i < SGX_SHA256_HASH_SIZE; i ++) {
		printf("%02x", result[i]);
	}

	printf("\n");

	return 0;
}
