#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef S21_DEC_EXPONENT
#define S21_DEC_EXPONENT 0x00ff0000
#endif
#ifndef S21_DEC_SIGN
#define S21_DEC_SIGN 0x80000000
#endif
#ifndef INT_BITSIZE
#define INT_BITSIZE 32
#endif
#ifndef INT_DEC_SIZE
#define INT_DEC_SIZE 3
#endif
#ifndef INT_DOUBLE_DEC_SIZE
#define INT_DOUBLE_DEC_SIZE (INT_DEC_SIZE * 2)
#endif
#ifndef INT_POW_2_64
#define INT_POW_2_64 18446744073709551616.0
#endif
#ifndef INT_POW_2_32
#define INT_POW_2_32 4294967296
#endif

typedef enum {
  s21_NORMAL_VALUE = 0,
  s21_INFINITY = 1,
  s21_NEGATIVE_INFINITY = 2,
  s21_NAN = 3
} value_type_t;

// bits[0] - bits[2] - INTEGER
// bits[0] - least int
// bits[1] - middle int
// bits[2] - biggest int
// bits[3] (16-23 bits) - exponent
// bits[3] (31 bit) - sign

typedef struct {
  int bits[4];
  value_type_t value_type;
} s21_decimal;

typedef struct {
  int bits[7];
  value_type_t value_type;
} s21_double_decimal;

// typedef struct {
//   unsigned long long x;
//   unsigned long long y;
//   unsigned long long z;
// } s21_unsigned_decimal;

// ARITHMETIC OPERATORS
// Returns addition of two decimals
s21_decimal s21_add(s21_decimal a, s21_decimal b);

// Returns substruction of two decimals
s21_decimal s21_sub(s21_decimal a, s21_decimal b);

// Returns multiplication of two decimals
s21_decimal s21_mul(s21_decimal a, s21_decimal b);

// Returns division of two decimals
s21_decimal s21_div(s21_decimal a, s21_decimal b);

// Returns modal division of two decimals
s21_decimal s21_mod(s21_decimal a, s21_decimal b);

// COMPARISON OPERATORS
// Returns 0 if a < b
int s21_is_less(s21_decimal a, s21_decimal b);

// Returns 0 if a <= b
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);

// Returns 0 if a > b
int s21_is_greater(s21_decimal a, s21_decimal b);

// Returns 0 if a >= b
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);

// Returns 0 if a == b
int s21_is_equal(s21_decimal a, s21_decimal b);

// Returns 0 if a != b
int s21_is_not_equal(s21_decimal a, s21_decimal b);

// CONVERTORS
// Returns 1 if error occured else returns 0
int s21_from_int_to_decimal(int src, s21_decimal *dst);

// Returns 1 if error occured else returns 0
int s21_from_float_to_decimal(float src, s21_decimal *dst);

// Returns 1 if error occured else returns 0
int s21_from_decimal_to_int(s21_decimal src, int *dst);

// Returns 1 if error occured else returns 0
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// OTHER MATHEMATICAL FUNCTIONS
// Rounds decimal to lesser integer
s21_decimal s21_floor(s21_decimal a);

// Rounds decimal to closest integer
s21_decimal s21_round(s21_decimal a);

// Truncates all non-integral digits
s21_decimal s21_truncate(s21_decimal a);

// Negates decimal
s21_decimal s21_negate(s21_decimal a);

// HELP FUNCTIONS

// Returns 1 if |a| == |b|
int s21_is_equal_mod(s21_decimal a, s21_decimal b);

// Returns decimal exponent
int s21_get_exp(int *bits, int size);

// Returns 1 if negative else returns 0
int s21_get_sign(int *bits, int size);

int s21_set_exp(int *bits, unsigned int exp, int size);

int s21_set_sign(int *bits, unsigned int sign, int size);

// Initializes decimal, sets last bits part to val
s21_decimal s21_init_dec(unsigned int val);

// Conderts double decimal to decimal
s21_decimal s21_ddec_to_dec(s21_double_decimal num);

// Initializes double decimal, sets last bits part to val
s21_double_decimal s21_init_ddec(unsigned int val);

// Converts decimal to double decimal
s21_double_decimal s21_dec_to_ddec(s21_decimal num);

// return 10^exp in decimal form
s21_decimal s21_ten_pow(unsigned int exp);

s21_double_decimal s21_ten_double_pow(unsigned int exp);

// converts both decimals to one denominator
void s21_equalize(s21_decimal a, s21_decimal b, s21_double_decimal *na,
                  s21_double_decimal *nb);

// Returns decimal value as a string if it's normal value
int s21_bin_dectoa(s21_decimal number, char *buffer, int need_spaces);

// for s21_bin_dectoa
void s21_itoa(int number, char *buffer);

// BIG INT ARITHMETIC

// BIG INT - array of unsigned integers for size*INT_BITSIZE-bit arithmetic
// Example: int bigint[3] - 96-bit unsigned integer

// Returns 1 if a > b, -1 if a < b and 0 if a == b
int s21_big_int_cmp(unsigned int *a, unsigned int *b, int size);

// Shifts a for the "shift" bits to the left. Returns 1 if some bits lost, else
// returns 0
int s21_big_int_shift_left(unsigned int *a, int size, unsigned int shift);

// Shifts a for the "shift" bits to the right. Returns 1 if some bits lost, else
// returns 0
int s21_big_int_shift_right(unsigned int *a, int size, unsigned int shift);

// Sums a and b to result (can be a or b). If overflow happened returns 1, else
// returns 0
int s21_big_int_sum(unsigned int *a, unsigned int *b, unsigned int *result,
                    int size);

// Substracts smaller from bigger bigint to result (can be a or b). If a <
// b returns 1, else returns 0
int s21_big_int_sub(unsigned int *a, unsigned int *b, unsigned int *result,
                    int size);

// Multiplicates a and b to result (can be a or b). If overflow happened returns
// 1, else returns 0. If fullresult isn't NAN (must be size*2), writes full mult
// result to it
int s21_big_int_mult(unsigned int *a, unsigned int *b, unsigned int *result,
                     unsigned int *fullresult, int size);

// Divides a to b (result or remainder can be NULL). Returns 1 if b is zero,
// else returns 0
int s21_big_int_div(unsigned int *a, unsigned int *b, unsigned int *result,
                    unsigned int *remainder, int size);

void s21_big_int_to_a(char *str, unsigned int *bigint, int size,
                      int need_spaces);

// RMARCHEL BRANCH FUNCTIONS

// s21_unsigned_decimal s21_copy_digits(s21_decimal src);

// void s21_unsigned_decimal_div(s21_unsigned_decimal *src, unsigned int del);

int s21_from_float_to_bits(float src);

int s21_from_int_to_bits(unsigned int src);

int getBinExp(float src);

int setBit(s21_decimal *dst, int position);

void setPower(s21_decimal *dst, int power);

#endif