#include "sgxerror.h"
#include "util.h"
#include <errno.h>

static void * (* const volatile __memset_vp)(void *, int, size_t)
    = (memset);

#undef memset_s /* in case it was defined as a macro */

errno_t
memset_s(void *s, size_t smax, int c, size_t n)
{
    errno_t err = 0;

    if (s == NULL) {
        err = EINVAL;
        goto out;
    }
    if (smax > SIZE_MAX) {
        err = E2BIG;
        goto out;
    }
    if (n > SIZE_MAX) {
        err = E2BIG;
        n = smax;
    }
    if (n > smax) {
        err = EOVERFLOW;
        n = smax;
    }

    /* Calling through a volatile pointer should never be optimised away. */
    (*__memset_vp)(s, c, n);

    out:
    if (err == 0)
        return 0;
    else {
        errno = err;
        /* XXX call runtime-constraint handler */
        return err;
    }
}

int
consttime_memequal(const void *b1, const void *b2, size_t len)
{
    const unsigned char *c1 = b1, *c2 = b2;
    unsigned int res = 0;

    while (len--)
        res |= *c1++ ^ *c2++;

    /*
     * Map 0 to 1 and [1, 256) to 0 using only constant-time
     * arithmetic.
     *
     * This is not simply `!res' because although many CPUs support
     * branchless conditional moves and many compilers will take
     * advantage of them, certain compilers generate branches on
     * certain CPUs for `!res'.
     */
    return (1 & ((res - 1) >> 8));
}

int do_rdrand(uint32_t* eax) {
    return __builtin_ia32_rdrand32_step(eax);
}

static sgx_status_t  __do_get_rand32(uint32_t* rand_num)
{
    if(0 == do_rdrand(rand_num))
        return SGX_ERROR_UNEXPECTED;
    return SGX_SUCCESS;
}

sgx_status_t sgx_read_rand(unsigned char *rand, size_t length_in_bytes)
{
    // check parameters
    //
    // rand can be within or outside the enclave
    if(!rand || !length_in_bytes)
    {
        return SGX_ERROR_INVALID_PARAMETER;
    }
    // loop to rdrand
    uint32_t rand_num = 0;
    while(length_in_bytes > 0)
    {
        sgx_status_t status = __do_get_rand32(&rand_num);
        if(status != SGX_SUCCESS)
        {
            return status;
        }

        size_t size = (length_in_bytes < sizeof(rand_num)) ?                        length_in_bytes : sizeof(rand_num);
        memcpy(rand, &rand_num, size);

        rand += size;
        length_in_bytes -= size;
    }
    memset_s(&rand_num, sizeof(rand_num), 0, sizeof(rand_num));
    return SGX_SUCCESS;
}
