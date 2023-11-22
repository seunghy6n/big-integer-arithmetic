#include "bigint.h"

// 1. Karatsuba vs text mul 속도 측정
// 2. modular exponentiation
// 3. test vector
// 4. memory leak
// AES CTR DRBG 심어서 res 파일에 cout=1 : 0, count2 = ff.. count=3 to 100 까지 test number, 답을 넣어서
// Python에서 읽어서 결과랑 일치하면 true

// Main function
int main() {

    bigint *x = NULL;
    bigint *y = NULL;
    bigint *z = NULL;
    bigint *q = NULL;
    bigint *r = NULL;

    word *a = NULL;

    // Test : gen new array and show array
    printf("==================================================================================================== \n");
    printf("Test : gen new array and show array \n");
    printf("**************************************************************************************************** \n");
    array_new_rand(&a, 5);
    printf("a = "); array_show_hex(a, 5);
    printf("==================================================================================================== \n\n\n");

    // Test : bigint <- array
    printf("====================================================================================================\n");
    printf("Test : bigint <- array\n");
    printf("****************************************************************************************************\n");
    bi_set_by_array(&x, NEGATIVE, a, 5);
    printf("x = "); bi_show_hex(x);
    printf("====================================================================================================\n\n\n");

    // Test : gen rand bigint
    printf("====================================================================================================\n");
    printf("Test : gen rand bigint\n");
    printf("****************************************************************************************************\n");
    bi_gen_rand(&y, NEGATIVE, 10);
    printf("y = "); bi_show_hex(y);
    printf("====================================================================================================\n\n\n");

    // Test : bigint <- bigint assign
    printf("====================================================================================================\n");
    printf("Test : bigint <- bigint assign\n");
    printf("****************************************************************************************************\n");
    bi_assign(&z, y);
    printf("z = "); bi_show_hex(z);
    printf("====================================================================================================\n\n\n");

    // Test : print binary
    printf("====================================================================================================\n");
    printf("Test : print binary\n");
    printf("****************************************************************************************************\n");
    printf("z = "); bi_show_bin(z);
    printf("====================================================================================================\n\n\n");

    // Test : isone, iszero
    printf("====================================================================================================\n");
    printf("Test : isone, iszero\n");
    printf("****************************************************************************************************\n");
    bi_set_zero(&x);
    printf("x is zero? %d\n", (bi_is_zero(x) == TRUE));
    bi_set_one(&x);
    printf("x is one? %d\n", (bi_is_one(x) == TRUE));
    printf("====================================================================================================\n\n\n");

    // Test : compare
    printf("====================================================================================================\n");
    printf("Test : compare\n");
    printf("****************************************************************************************************\n");
    y->sign = NON_NEGATIVE;
    z->sign = NON_NEGATIVE;
    
    printf("x = "); bi_show_hex(x);
    printf("y = "); bi_show_hex(y);
    printf("z = "); bi_show_hex(z);
    printf("x VS y = %d\n", bi_compare(x, y)); // x > y (1)
    printf("y VS z = %d\n", bi_compare(y, z)); // x = y (0)
    printf("====================================================================================================\n\n\n");

    // Test : bi_shift
    printf("====================================================================================================\n");
    printf("Test : bi_shift\n");
    printf("****************************************************************************************************\n");
    printf("z = "); bi_show_hex(z);
    bi_shift_right(&z, 64);
    printf(" >>= 64 = "); bi_show_hex(z);


    bi_shift_left(&z, 1);
    printf(" <<= 1 = "); bi_show_hex(z);
    bi_shift_right(&z, 1);
    printf(" >>= 1 = "); bi_show_hex(z);

    bi_shift_right(&z, 500);
    printf(" >>= 500 = "); bi_show_hex(z);

    bi_gen_rand(&z, NEGATIVE, 5);

    printf("z = "); bi_show_hex(z);
    bi_shift_left(&z, 64);
    printf(" <<= 64 = "); bi_show_hex(z);

    bi_shift_left(&z, 8);
    printf(" <<= 8 = "); bi_show_hex(z);

    bi_shift_left(&z, 200);
    printf(" <<= 200 = "); bi_show_hex(z);
    printf("====================================================================================================\n\n\n");

    // Test : bi_reduction
    printf("====================================================================================================\n");
    printf("Test : bi_reduction\n");
    printf("****************************************************************************************************\n");
    printf("z = "); bi_show_hex(z);
    if ( bi_reduction(&z, 400) == 1 ) {
        printf(" mod 2^400 = "); bi_show_hex(z);
    }
    printf("====================================================================================================\n\n\n");

    // Test : bi_add
    printf("====================================================================================================\n");
    printf("Test : bi_add\n");
    printf("****************************************************************************************************\n");
    bi_gen_rand(&y, NON_NEGATIVE, 10);
    bi_gen_rand(&x, NEGATIVE, 10);
    printf("x = "); bi_show_hex(x);
    printf("y = "); bi_show_hex(y);

    bi_add(&z, x, y);
    printf("z = x + y = "); bi_show_hex(z);
    printf("====================================================================================================\n\n\n");


    // Test : bi_sub
    printf("====================================================================================================\n");
    printf("Test : bi_sub\n");
    printf("****************************************************************************************************\n");
    printf("z = "); bi_show_hex(z);
    printf("x = "); bi_show_hex(x);

    bi_sub(&x, z, y);
    printf("x = z - y = "); bi_show_hex(x);
    printf("====================================================================================================\n\n\n");

    // Test : bi_set_by_string
    printf("====================================================================================================\n");
    printf("Test : bi_set_by_string\n");
    printf("****************************************************************************************************\n");
    const char *str_hex = "aabbc0aaca9";
    printf("str_hex = %s\n", str_hex);
    bi_set_by_string(&x, NEGATIVE, str_hex, 16);
    printf("x = "); bi_show_hex(x);
    const char *str_bin = "100111111011101100111111011101100111111011101100111111011101100111111011101100111111011101100111111011101100111111011101100111111011101100111111011101100111111011101100111111011101";
    printf("str_bin = %s\n", str_bin);
    bi_set_by_string(&x, NON_NEGATIVE, str_bin, 2);
    printf("x = "); bi_show_hex(x);
    printf("====================================================================================================\n\n\n");

    // Test : bi_mul
    printf("====================================================================================================\n");
    printf("Test : bi_mul\n");
    printf("****************************************************************************************************\n");
    printf("Test : bi_mul_zxy (Single Mul)\n");
    printf("****************************************************************************************************\n");
    printf("x = "); array_show_hex(x->a, 1);
    printf("y = "); array_show_hex(y->a, 1);
    word t[2];
    bi_mul_zj(t, x->a[0], y->a[0]);
    printf("t = x * y = "); array_show_hex(t, 2);
    printf("****************************************************************************************************\n");

    printf("Test : bi_mul_textbook (Textbook/Improved/Karatsuba Mul)\n");
    printf("****************************************************************************************************\n");
    printf("x = "); bi_show_hex(x);
    printf("y = "); bi_show_hex(y);
    bi_mul(&z, x, y, "Textbook");
    printf("(Textbook Mul)  z = x * y = "); bi_show_hex(z);
    bi_mul(&z, x, y, "Improved");
    printf("(Improved Mul)  z = x * y = "); bi_show_hex(z);
    bi_mul(&z, x, y, "Karatsuba");
    printf("(Karatsuba Mul) z = x * y = "); bi_show_hex(z);
    printf("====================================================================================================\n\n\n");

    // Test : bi_div
    printf("====================================================================================================\n");
    printf("Test : long division\n");
    printf("****************************************************************************************************\n");
    bi_set_by_string(&x, NON_NEGATIVE, "300127af9cd1bcc189910a2c7ee16add2f168c", 16);
    bi_set_by_string(&y, NON_NEGATIVE, "b78a0100cf62be18cc1", 16);
    printf("x = "); bi_show_hex(x);
    printf("y = "); bi_show_hex(y);
    if (bi_div(&q, &r, x, y) == SUCCESS) {
        printf("bi_div: TRUE\n");
    }
    printf("q = "); bi_show_hex(q);
    printf("r = "); bi_show_hex(r);
    printf("====================================================================================================\n\n\n");

    // Test : bi_barrett_reduction
    printf("====================================================================================================\n");
    printf("Test : barrett reduction\n");
    printf("****************************************************************************************************\n");
    printf("x = "); bi_show_hex(x);
    printf("y = "); bi_show_hex(y);
    size_t n = y->wordlen;
    bigint *w = NULL, *tt = NULL;
    bi_set_min_words(&w, NON_NEGATIVE, 2 * n + 1); // w^2n
    bi_div(&tt, &r, w, y);
    printf("w^2n = "); bi_show_hex(w);
    printf("y = "); bi_show_hex(y);
    printf("Pre-computated t = w^2n / y "); bi_show_hex(tt);
    bi_barrett_reduction(&z, x, y, tt);
    printf("z = x mod y = "); bi_show_hex(z);
    printf("====================================================================================================\n\n\n");

    // // Test : bi_modular_exponentiation
    // printf("====================================================================================================\n");
    // printf("Test : Montgomery Ladder \n");
    // printf("****************************************************************************************************\n");

    // printf("====================================================================================================\n\n\n");

    free(a);
    bi_delete(&x);
    bi_delete(&y);
    bi_delete(&z);
    bi_delete(&q);
    bi_delete(&r);
    bi_delete(&w);
    bi_delete(&tt);

    return 0;
} 
// EOF