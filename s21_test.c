#include "s21_test.h"

START_TEST(s21_from_int_to_decimal_test) {
  int src1 = 2000;
  s21_decimal dec1;
  s21_from_int_to_decimal(src1, &dec1);
  int first_test_value = 0, second_test_value = 0, third_test_value = 2000;
  ck_assert_int_eq(s21_get_exp(dec1.bits, INT_DEC_SIZE), 0);
  ck_assert_int_eq(s21_get_sign(dec1.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec1.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec1.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec1.bits, &third_test_value, 4);
  int src2 = -2000;
  s21_decimal dec2;
  s21_from_int_to_decimal(src2, &dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 2000;
  ck_assert_int_eq(s21_get_exp(dec2.bits, INT_DEC_SIZE), 0);
  ck_assert_int_eq(s21_get_sign(dec2.bits, INT_DEC_SIZE), 1);
  ck_assert_mem_eq(dec2.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec2.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec2.bits, &third_test_value, 4);
}
END_TEST

START_TEST(s21_from_decimal_to_int_test) {
  s21_decimal dec1 = {{2000, 0, 0, 0x0000000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{2000, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec3 = {{2000, 0, 0, 0x80000000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{2000, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec5 = {{2000, 20, 20, 0x0000000}, s21_NORMAL_VALUE};
  s21_decimal dec6 = {{2000, 20, 0, 0x0000000}, s21_NORMAL_VALUE};
  s21_decimal dec8 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};
  s21_decimal dec10 = {{0, 0, 0, 0x00000000}, s21_NORMAL_VALUE};
  s21_decimal dec11 = {{0xffffffff, 0, 0, 0x0000000}, s21_NORMAL_VALUE};
  int dst1, dst2, dst3, dst4, dst5;
  ck_assert_int_eq(s21_from_decimal_to_int(dec1, &dst1), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(dec2, &dst2), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(dec3, &dst3), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(dec4, &dst4), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(dec5, &dst1), 1);
  ck_assert_int_eq(s21_from_decimal_to_int(dec6, &dst1), 1);
  ck_assert_int_eq(s21_from_decimal_to_int(dec8, &dst1), 1);
  ck_assert_int_eq(s21_from_decimal_to_int(dec9, &dst1), 1);
  ck_assert_int_eq(s21_from_decimal_to_int(dec10, &dst5), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(dec11, &dst1), 1);
  ck_assert_int_eq(dst1, 2000);
  ck_assert_int_eq(dst2, 20);
  ck_assert_int_eq(dst3, -2000);
  ck_assert_int_eq(dst4, -20);
  ck_assert_int_eq(dst5, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test) {
  float src1 = 2.5e28;
  s21_decimal dec;
  ck_assert_int_eq(s21_from_float_to_decimal(src1, &dec), 0);
  int first_test_value = 0b01010000110001111000010000000000,
      second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_int_eq(s21_get_sign(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec.bits + 2, &first_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits + 1, &second_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits, &third_test_value, sizeof(int));
  float src2 = 1e30;
  ck_assert_int_eq(s21_from_float_to_decimal(src2, &dec), 1);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_int_eq(s21_get_sign(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec.bits + 2, &first_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits + 1, &second_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits, &third_test_value, sizeof(int));
  ck_assert_int_eq(dec.value_type, 1);
  float src3 = INFINITY;
  ck_assert_int_eq(s21_from_float_to_decimal(src3, &dec), 1);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_int_eq(s21_get_sign(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec.bits + 2, &first_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits + 1, &second_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits, &third_test_value, sizeof(int));
  ck_assert_int_eq(dec.value_type, 1);
  float src4 = NAN;
  ck_assert_int_eq(s21_from_float_to_decimal(src4, &dec), 1);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_int_eq(s21_get_sign(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec.bits + 2, &first_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits + 1, &second_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits, &third_test_value, sizeof(int));
  ck_assert_int_eq(dec.value_type, 3);
  float src5 = -0.25;
  ck_assert_int_eq(s21_from_float_to_decimal(src5, &dec), 0);
  first_test_value = 0, second_test_value = 0, third_test_value = 25000000;
  ck_assert_int_eq(s21_get_exp(dec.bits, INT_DEC_SIZE), 8);
  ck_assert_int_eq(s21_get_sign(dec.bits, INT_DEC_SIZE), 1);
  ck_assert_mem_eq(dec.bits + 2, &first_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits + 1, &second_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits, &third_test_value, sizeof(int));
  ck_assert_int_eq(s21_from_float_to_decimal(src5, NULL), 1);
  float src6 = 0.25e-28;
  ck_assert_int_eq(s21_from_float_to_decimal(src6, &dec), 1);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_int_eq(s21_get_sign(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec.bits + 2, &first_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits + 1, &second_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits, &third_test_value, sizeof(int));
  ck_assert_int_eq(dec.value_type, 3);
  float src7 = -7.9e30;
  ck_assert_int_eq(s21_from_float_to_decimal(src7, &dec), 1);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_int_eq(s21_get_sign(dec.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec.bits + 2, &first_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits + 1, &second_test_value, sizeof(int));
  ck_assert_mem_eq(dec.bits, &third_test_value, sizeof(int));
  ck_assert_int_eq(dec.value_type, 2);
}
END_TEST

START_TEST(s21_from_decimal_to_float_test) {
  s21_decimal dec1 = {{3923312640, 58207660, 0, 0x80120000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{3923312640, 58207660, 0, 0x00120000}, s21_NORMAL_VALUE};
  s21_decimal dec7 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec8 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};
  s21_decimal dec10 = {{1874293453, 3448970907, 3486583653, 0x007F0000},
                       s21_NORMAL_VALUE};
  float dst1, dst2, dst5;
  ck_assert_int_eq(s21_from_decimal_to_float(dec1, &dst1), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(dec2, &dst2), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(dec7, &dst1), 1);
  ck_assert_int_eq(s21_from_decimal_to_float(dec8, &dst1), 1);
  ck_assert_int_eq(s21_from_decimal_to_float(dec9, &dst1), 1);
  ck_assert_int_eq(s21_from_decimal_to_float(dec10, &dst5), 0);
  ck_assert_int_eq(dst1, -0.25);
  ck_assert_int_eq(dst2, 0.25);
  ck_assert_int_eq(dst5, 0.0);
}
END_TEST

START_TEST(s21_is_less_test) {
  s21_decimal dec1 = {{212121, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{1, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec3 = {{212121, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{1, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec5 = {{212121, 0, 0, 0x000140000}, s21_NORMAL_VALUE};
  s21_decimal dec6 = {{1, 0, 1, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec7 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec8 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};
  s21_decimal dec10 = {{0, 0, 0, 0x00000000}, s21_NORMAL_VALUE};
  s21_decimal dec11 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0050000},
                       s21_NORMAL_VALUE};
  s21_decimal dec12 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80050000},
                       s21_NORMAL_VALUE};
  ck_assert_int_eq(s21_is_less(dec1, dec2), 1);
  ck_assert_int_eq(s21_is_less(dec3, dec4), 0);
  ck_assert_int_eq(s21_is_less(dec1, dec3), 1);
  ck_assert_int_eq(s21_is_less(dec4, dec2), 0);
  ck_assert_int_eq(s21_is_less(dec1, dec5), 1);
  ck_assert_int_eq(s21_is_less(dec1, dec6), 0);
  ck_assert_int_eq(s21_is_less(dec7, dec10), 1);
  ck_assert_int_eq(s21_is_less(dec10, dec8), 0);
  ck_assert_int_eq(s21_is_less(dec9, dec1), 0);
  ck_assert_int_eq(s21_is_less(dec11, dec10), 1);
  ck_assert_int_eq(s21_is_less(dec12, dec11), 0);
  ck_assert_int_eq(s21_is_less(dec11, dec11), 1);
}
END_TEST

START_TEST(s21_main_tests) {
  s21_decimal dec1 = {{212121, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{1, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec3 = {{212121, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{1, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec5 = {{212121, 0, 0, 0x000140000}, s21_NORMAL_VALUE};
  s21_decimal dec6 = {{1, 0, 1, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec7 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec8 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};
  s21_decimal dec10 = {{0, 0, 0, 0x00000000}, s21_NORMAL_VALUE};
  s21_decimal dec11 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0050000},
                       s21_NORMAL_VALUE};
  s21_decimal dec12 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80050000},
                       s21_NORMAL_VALUE};
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 0);
  ck_assert_int_eq(s21_is_greater(dec3, dec4), 1);
  ck_assert_int_eq(s21_is_greater(dec1, dec3), 0);
  ck_assert_int_eq(s21_is_greater(dec4, dec2), 1);
  ck_assert_int_eq(s21_is_greater(dec1, dec5), 0);
  ck_assert_int_eq(s21_is_greater(dec1, dec6), 1);
  ck_assert_int_eq(s21_is_greater(dec7, dec10), 1);
  ck_assert_int_eq(s21_is_greater(dec8, dec10), 0);
  ck_assert_int_eq(s21_is_greater(dec9, dec1), 1);
  ck_assert_int_eq(s21_is_greater(dec11, dec10), 0);
  ck_assert_int_eq(s21_is_greater(dec12, dec11), 1);
  ck_assert_int_eq(s21_is_greater(dec11, dec11), 1);
}
END_TEST

START_TEST(s21_is_less_or_equal_test) {
  s21_decimal dec1 = {{212121, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{1, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec3 = {{212121, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{1, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec5 = {{212121, 0, 0, 0x000140000}, s21_NORMAL_VALUE};
  s21_decimal dec6 = {{1, 0, 1, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec7 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec8 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};
  s21_decimal dec10 = {{0, 0, 0, 0x00000000}, s21_NORMAL_VALUE};
  s21_decimal dec11 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0050000},
                       s21_NORMAL_VALUE};
  s21_decimal dec12 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80050000},
                       s21_NORMAL_VALUE};
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);
  ck_assert_int_eq(s21_is_less_or_equal(dec3, dec4), 0);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec3), 1);
  ck_assert_int_eq(s21_is_less_or_equal(dec4, dec2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec5), 1);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec6), 0);
  ck_assert_int_eq(s21_is_less_or_equal(dec6, dec6), 0);
  ck_assert_int_eq(s21_is_less_or_equal(dec7, dec10), 1);
  ck_assert_int_eq(s21_is_less_or_equal(dec10, dec8), 0);
  ck_assert_int_eq(s21_is_less_or_equal(dec9, dec1), 0);
  ck_assert_int_eq(s21_is_less_or_equal(dec11, dec10), 1);
  ck_assert_int_eq(s21_is_less_or_equal(dec12, dec11), 0);
  ck_assert_int_eq(s21_is_less_or_equal(dec11, dec11), 0);
}
END_TEST

START_TEST(s21_is_greater_or_equal_test) {
  s21_decimal dec1 = {{212121, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{1, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec3 = {{212121, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{1, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec5 = {{212121, 0, 0, 0x000140000}, s21_NORMAL_VALUE};
  s21_decimal dec6 = {{1, 0, 1, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec7 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec8 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};
  s21_decimal dec10 = {{0, 0, 0, 0x00000000}, s21_NORMAL_VALUE};
  s21_decimal dec11 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0050000},
                       s21_NORMAL_VALUE};
  s21_decimal dec12 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80050000},
                       s21_NORMAL_VALUE};
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(dec3, dec4), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec3), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(dec4, dec2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec5), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec6), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(dec7, dec10), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(dec8, dec10), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(dec9, dec1), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(dec6, dec6), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(dec11, dec10), 0);
  ck_assert_int_eq(s21_is_greater_or_equal(dec12, dec11), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(dec11, dec11), 0);
}
END_TEST

START_TEST(s21_is_equal_test) {
  s21_decimal dec1 = {{212121, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{1, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec3 = {{212121, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{1, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec5 = {{0, 0, 0, 0x000140000}, s21_NORMAL_VALUE};
  s21_decimal dec7 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec8 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};
  s21_decimal dec10 = {{0, 0, 0, 0x00040000}, s21_NORMAL_VALUE};
  s21_decimal dec11 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0050000},
                       s21_NORMAL_VALUE};
  s21_decimal dec12 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80050000},
                       s21_NORMAL_VALUE};
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 1);
  ck_assert_int_eq(s21_is_equal(dec1, dec1), 0);
  ck_assert_int_eq(s21_is_equal(dec3, dec4), 1);
  ck_assert_int_eq(s21_is_equal(dec4, dec4), 0);
  ck_assert_int_eq(s21_is_equal(dec1, dec3), 1);
  ck_assert_int_eq(s21_is_equal(dec10, dec5), 0);
  ck_assert_int_eq(s21_is_equal(dec7, dec10), 1);
  ck_assert_int_eq(s21_is_equal(dec10, dec8), 1);
  ck_assert_int_eq(s21_is_equal(dec10, dec8), 1);
  ck_assert_int_eq(s21_is_equal(dec10, dec10), 0);
  ck_assert_int_eq(s21_is_equal(dec11, dec10), 1);
  ck_assert_int_eq(s21_is_equal(dec12, dec11), 1);
  ck_assert_int_eq(s21_is_equal(dec11, dec11), 0);
  ck_assert_int_eq(s21_is_equal(dec9, dec10), 1);
}
END_TEST

START_TEST(s21_is_not_equal_test) {
  s21_decimal dec1 = {{212121, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{1, 0, 0, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec3 = {{212121, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{1, 0, 0, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec5 = {{0, 0, 0, 0x000140000}, s21_NORMAL_VALUE};
  s21_decimal dec7 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec8 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};
  s21_decimal dec10 = {{0, 0, 0, 0x00040000}, s21_NORMAL_VALUE};
  s21_decimal dec11 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0050000},
                       s21_NORMAL_VALUE};
  s21_decimal dec12 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80050000},
                       s21_NORMAL_VALUE};
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec1), 1);
  ck_assert_int_eq(s21_is_not_equal(dec3, dec4), 0);
  ck_assert_int_eq(s21_is_not_equal(dec4, dec4), 1);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec3), 0);
  ck_assert_int_eq(s21_is_not_equal(dec10, dec5), 1);
  ck_assert_int_eq(s21_is_not_equal(dec7, dec10), 0);
  ck_assert_int_eq(s21_is_not_equal(dec10, dec8), 0);
  ck_assert_int_eq(s21_is_not_equal(dec10, dec8), 0);
  ck_assert_int_eq(s21_is_not_equal(dec10, dec10), 1);
  ck_assert_int_eq(s21_is_not_equal(dec11, dec10), 0);
  ck_assert_int_eq(s21_is_not_equal(dec12, dec11), 0);
  ck_assert_int_eq(s21_is_not_equal(dec11, dec11), 1);
  ck_assert_int_eq(s21_is_not_equal(dec9, dec10), 0);
}
END_TEST

START_TEST(s21_negate_test) {
  s21_decimal dec1 = {{21, 1, 21, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{21, 1, 21, 0x0020000}, s21_NORMAL_VALUE};
  s21_decimal dec3 = {{0, 0, 0, 0}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{0, 0, 0, 0}, s21_INFINITY};
  s21_decimal dec5 = {{0, 0, 0, 0}, s21_NAN};
  s21_decimal dec6 = {{0, 0, 0, 0}, s21_NEGATIVE_INFINITY};
  s21_decimal dec7 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x0050000},
                      s21_NORMAL_VALUE};
  ck_assert_int_eq(s21_get_sign((s21_negate(dec1)).bits, INT_DEC_SIZE), 0);
  ck_assert_int_eq(s21_get_sign((s21_negate(dec2)).bits, INT_DEC_SIZE), 1);
  ck_assert_int_eq(s21_get_sign((s21_negate(dec3)).bits, INT_DEC_SIZE), 1);
  ck_assert_int_eq(s21_negate(dec4).value_type, 2);
  ck_assert_int_eq(s21_get_sign((s21_negate(dec5)).bits, INT_DEC_SIZE), 0);
  ck_assert_int_eq(s21_negate(dec6).value_type, 1);
  ck_assert_int_eq(s21_get_sign((s21_negate(dec7)).bits, INT_DEC_SIZE), 1);
}
END_TEST

START_TEST(s21_truncate_test) {
  s21_decimal dec1 = {{21, 1, 21, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00050000},
                      s21_NORMAL_VALUE};
  s21_decimal dec3 = {{21, 0, 0, 0x00180000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{0, 0, 0, 0}, s21_NORMAL_VALUE};
  s21_decimal dec5 = {{0, 0, 0, 0}, s21_INFINITY};
  s21_decimal dec6 = {{0, 0, 0, 0}, s21_NAN};
  s21_decimal dec7 = {{0, 0, 0, 0}, s21_NEGATIVE_INFINITY};

  int first_test_value = 0, second_test_value = 901943132,
      third_test_value = 730144440;

  dec1 = s21_truncate(dec1);
  ck_assert_int_eq(s21_get_exp(dec1.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec1.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec1.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec1.bits, &third_test_value, 4);

  dec2 = s21_truncate(dec2);
  first_test_value = 42949, second_test_value = 2890341191,
  third_test_value = 2216890319;
  ck_assert_int_eq(s21_get_exp(dec2.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec2.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec2.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec2.bits, &third_test_value, 4);

  dec3 = s21_truncate(dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec3.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec3.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec3.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec3.bits, &third_test_value, 4);

  dec4 = s21_truncate(dec4);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec4.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec4.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec4.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec4.bits, &third_test_value, 4);

  dec5 = s21_truncate(dec5);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec5.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec5.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec5.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec5.bits, &third_test_value, 4);

  dec6 = s21_truncate(dec6);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec6.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec6.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec6.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec6.bits, &third_test_value, 4);

  dec7 = s21_truncate(dec7);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec7.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec7.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec7.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec7.bits, &third_test_value, 4);
}
END_TEST

START_TEST(s21_round_test) {
  s21_decimal dec1 = {{21, 1, 21, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00050000},
                      s21_NORMAL_VALUE};
  s21_decimal dec3 = {{21, 0, 0, 0x00180000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{0, 0, 0, 0}, s21_NORMAL_VALUE};
  s21_decimal dec5 = {{1, 1, 1, 0}, s21_INFINITY};
  s21_decimal dec6 = {{1, 1, 1, 0}, s21_NAN};
  s21_decimal dec7 = {{1, 1, 1, 0}, s21_NEGATIVE_INFINITY};

  int first_test_value = 0, second_test_value = 901943132,
      third_test_value = 730144441;
  dec1 = s21_round(dec1);
  ck_assert_int_eq(s21_get_exp(dec1.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec1.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec1.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec1.bits, &third_test_value, 4);

  dec2 = s21_round(dec2);
  first_test_value = 42949, second_test_value = 2890341191,
  third_test_value = 2216890320;
  ck_assert_int_eq(s21_get_exp(dec2.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec2.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec2.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec2.bits, &third_test_value, 4);

  dec3 = s21_round(dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec3.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec3.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec3.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec3.bits, &third_test_value, 4);

  dec4 = s21_round(dec4);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec4.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec4.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec4.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec4.bits, &third_test_value, 4);

  dec5 = s21_round(dec5);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec5.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec5.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec5.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec5.bits, &third_test_value, 4);

  dec6 = s21_round(dec6);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec6.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec6.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec6.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec6.bits, &third_test_value, 4);

  dec7 = s21_round(dec7);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec7.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec7.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec7.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec7.bits, &third_test_value, 4);
}
END_TEST

START_TEST(s21_floor_test) {
  s21_decimal dec1 = {{21, 1, 21, 0x80020000}, s21_NORMAL_VALUE};
  s21_decimal dec2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00050000},
                      s21_NORMAL_VALUE};
  s21_decimal dec3 = {{21, 0, 0, 0x00180000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{0, 0, 0, 0}, s21_NORMAL_VALUE};
  s21_decimal dec5 = {{1, 1, 1, 0}, s21_INFINITY};
  s21_decimal dec6 = {{1, 1, 1, 0}, s21_NAN};
  s21_decimal dec7 = {{1, 1, 1, 0}, s21_NEGATIVE_INFINITY};
  s21_decimal dec8 = {{21, 0, 0, 0x80180000}, s21_NORMAL_VALUE};

  int first_test_value = 0, second_test_value = 901943132,
      third_test_value = 730144441;
  dec1 = s21_floor(dec1);
  ck_assert_int_eq(s21_get_exp(dec1.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec1.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec1.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec1.bits, &third_test_value, 4);

  dec2 = s21_floor(dec2);
  first_test_value = 42949, second_test_value = 2890341191,
  third_test_value = 2216890319;
  ck_assert_int_eq(s21_get_exp(dec2.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec2.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec2.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec2.bits, &third_test_value, 4);

  dec3 = s21_floor(dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec3.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec3.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec3.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec3.bits, &third_test_value, 4);

  dec4 = s21_floor(dec4);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec4.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec4.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec4.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec4.bits, &third_test_value, 4);

  dec5 = s21_floor(dec5);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec5.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec5.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec5.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec5.bits, &third_test_value, 4);

  dec6 = s21_floor(dec6);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec6.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec6.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec6.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec6.bits, &third_test_value, 4);

  dec7 = s21_floor(dec7);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(dec7.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec7.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec7.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec7.bits, &third_test_value, 4);

  dec3 = s21_floor(dec8);
  first_test_value = 0, second_test_value = 0, third_test_value = 1;
  ck_assert_int_eq(s21_get_exp(dec3.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(dec3.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(dec3.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(dec3.bits, &third_test_value, 4);
}
END_TEST

START_TEST(s21_add_test) {
  s21_decimal res;
  s21_decimal dec1 = {{-1805886985, -617359138, 754860479, 0x80150000},
                      s21_NORMAL_VALUE};
  s21_decimal dec2 = {{-435921560, 1158353539, 342505357, 0x80190000},
                      s21_NORMAL_VALUE};
  s21_decimal dec3 = {{0, 0, 0, 0x00000000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{1647504277, -1358324431, 1951932614, 0x80160000},
                      s21_NORMAL_VALUE};
  s21_decimal dec5 = {{195286955, 1890634887, 1655063024, 0x801C0000},
                      s21_NORMAL_VALUE};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec10 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec11 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};

  res = s21_add(dec1, dec2);
  int first_test_value = 754894730, second_test_value = 1683570678,
      third_test_value = 1721096566;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 21);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_add(dec3, dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 0);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 0);
  ck_assert_mem_eq(res.bits, &third_test_value, 0);

  res = s21_add(dec4, dec5);
  first_test_value = 1951934269, second_test_value = -1087636522,
  third_test_value = -508607765;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 22);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_add(dec9, dec9);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_add(dec9, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_add(dec10, dec11);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_add(dec10, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_INFINITY);

  res = s21_add(dec11, dec11);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NEGATIVE_INFINITY);

  res = s21_add(dec10, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_INFINITY);

  res = s21_add(dec11, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NEGATIVE_INFINITY);
}
END_TEST

START_TEST(s21_sub_test) {
  s21_decimal res;
  s21_decimal dec1 = {{1887484196, 115155085, -419324120, 0x00040000},
                      s21_NORMAL_VALUE};
  s21_decimal dec2 = {{112247158, -1457957590, -1123963046, 0x800B0000},
                      s21_NORMAL_VALUE};
  s21_decimal dec3 = {{0, 0, 0, 0x00000000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{-1268714504, -1459079836, -1702281133, 0x801B0000},
                      s21_NORMAL_VALUE};
  s21_decimal dec5 = {{2091810925, 1952850610, -1865484165, 0x00100000},
                      s21_NORMAL_VALUE};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec10 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec11 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};

  res = s21_sub(dec1, dec2);
  int first_test_value = -419323803, second_test_value = 546477459,
      third_test_value = -681891501;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 4);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_sub(dec3, dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_sub(dec1, dec1);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_sub(dec4, dec5);
  first_test_value = -1865484165, second_test_value = 2064205633,
  third_test_value = 1306373622;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 16);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_sub(dec9, dec9);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, INT_DEC_SIZE);

  res = s21_sub(dec9, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, INT_DEC_SIZE);

  res = s21_sub(dec10, dec11);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 1);

  res = s21_sub(dec10, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, INT_DEC_SIZE);

  res = s21_sub(dec11, dec11);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, INT_DEC_SIZE);

  res = s21_sub(dec10, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 1);

  res = s21_sub(dec11, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 2);
}
END_TEST

START_TEST(s21_div_test) {
  s21_decimal res;
  s21_decimal dec1 = {{286809831, 1882035680, -863347819, 0x80090000},
                      s21_NORMAL_VALUE};
  s21_decimal dec2 = {{1442421674, 2049514631, -1110292214, 0x800B0000},
                      s21_NORMAL_VALUE};
  s21_decimal dec3 = {{0, 0, 0, 0x00000000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{1020144780, -1397221906, -1769300400, 0x001B0000},
                      s21_NORMAL_VALUE};
  s21_decimal dec5 = {{1607036164, -435589727, -725613172, 0x80070000},
                      s21_NORMAL_VALUE};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec10 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec11 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};

  res = s21_div(dec1, dec2);
  int first_test_value = 584136402, second_test_value = -1498229922,
      third_test_value = -1721211881;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 26);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_div(dec2, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 1;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_div(dec3, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_div(dec3, dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_div(dec2, dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NEGATIVE_INFINITY);

  res = s21_div(dec4, dec5);
  first_test_value = 0, second_test_value = 0, third_test_value = 70759774;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 28);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_div(dec9, dec9);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);
  res = s21_div(dec9, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);
  res = s21_div(dec10, dec9);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_div(dec1, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 0);

  res = s21_div(dec3, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 0);

  res = s21_div(dec10, dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_INFINITY);

  res = s21_div(dec10, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_div(dec1, dec11);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 0);
  res = s21_div(dec3, dec11);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 0);
  res = s21_div(dec11, dec1);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_INFINITY);
}
END_TEST

START_TEST(s21_mul_test) {
  s21_decimal res;
  s21_decimal dec1 = {{-1577457547, -482405994, -448239951, 0x00010000},
                      s21_NORMAL_VALUE};
  s21_decimal dec2 = {{122616915, -791317973, -703960709, 0x00180000},
                      s21_NORMAL_VALUE};
  s21_decimal dec3 = {{0, 0, 0, 0x00000000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{1896153200, -1787476504, 810564845, 0x00180000},
                      s21_NORMAL_VALUE};
  s21_decimal dec5 = {{1130434920, -674626482, 91998149, 0x80160000},
                      s21_NORMAL_VALUE};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec10 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec11 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};

  res = s21_mul(dec1, dec2);
  long int first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_INFINITY);

  res = s21_mul(dec3, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_mul(dec3, dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_mul(dec4, dec5);
  first_test_value = 1375582303, second_test_value = -3330741,
  third_test_value = 313562749;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 19);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_mul(dec9, dec9);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mul(dec9, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mul(dec10, dec9);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mul(dec1, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_INFINITY);

  res = s21_mul(dec10, dec1);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 1);

  res = s21_mul(dec10, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 1);

  res = s21_mul(dec1, dec11);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 2);
}
END_TEST

START_TEST(s21_mod_test) {
  s21_decimal res;
  s21_decimal dec1 = {{-2121131716, -226548436, -288333491, 0x80020000},
                      s21_NORMAL_VALUE};
  s21_decimal dec2 = {{1157796390, -110348397, -1898887027, 0x001B0000},
                      s21_NORMAL_VALUE};
  s21_decimal dec3 = {{0, 0, 0, 0x00000000}, s21_NORMAL_VALUE};
  s21_decimal dec4 = {{145525528, 940906139, 380173677, 0x00170000},
                      s21_NORMAL_VALUE};
  s21_decimal dec5 = {{-1230015486, -1691184556, 1705904279, 0x80030000},
                      s21_NORMAL_VALUE};
  s21_decimal dec9 = {{0, 0, 0, 0x00000000}, s21_NAN};
  s21_decimal dec10 = {{0, 0, 0, 0x00000000}, s21_INFINITY};
  s21_decimal dec11 = {{0, 0, 0, 0x00000000}, s21_NEGATIVE_INFINITY};
  s21_decimal dec12 = {{1000, 0, 0, 0x00000000}, s21_NORMAL_VALUE};

  res = s21_mod(dec1, dec2);
  long int first_test_value = 1164936848, second_test_value = -964847787,
           third_test_value = 474459418;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 27);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_mod(dec2, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_mod(dec3, dec2);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_mod(dec3, dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mod(dec2, dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mod(dec4, dec5);
  first_test_value = 380173677, second_test_value = 940906139,
  third_test_value = 145525528;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 23);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);

  res = s21_mod(dec9, dec5);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mod(dec9, dec9);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mod(dec9, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mod(dec10, dec9);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mod(dec1, dec10);
  first_test_value = dec1.bits[2], second_test_value = dec1.bits[1],
  third_test_value = dec1.bits[0];
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE),
                   s21_get_exp(dec1.bits, INT_DEC_SIZE));
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 0);

  res = s21_mod(dec3, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, 0);

  res = s21_mod(dec10, dec3);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mod(dec10, dec1);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);

  res = s21_mod(dec10, dec10);
  first_test_value = 0, second_test_value = 0, third_test_value = 0;
  ck_assert_int_eq(s21_get_exp(res.bits, INT_DEC_SIZE), 0);
  ck_assert_mem_eq(res.bits + 2, &first_test_value, 4);
  ck_assert_mem_eq(res.bits + 1, &second_test_value, 4);
  ck_assert_mem_eq(res.bits, &third_test_value, 4);
  ck_assert_int_eq(res.value_type, s21_NAN);
}
END_TEST

int main() {
  Suite *s1 = suite_create("s21_decimal");
  TCase *s21_is_lessCase = tcase_create("s21_is_less test");
  TCase *s21_is_greaterCase = tcase_create("s21_is_greater test");
  TCase *s21_is_less_or_equalCase = tcase_create("s21_is_less_or_equal test");
  TCase *s21_is_greater_or_equalCase =
      tcase_create("s21_is_greater_or_equal test");
  TCase *s21_is_equalCase = tcase_create("s21_is_equal test");
  TCase *s21_is_not_equalCase = tcase_create("s21_is_not_equal test");
  TCase *s21_negateCase = tcase_create("s21_negate test");
  TCase *s21_truncateCase = tcase_create("s21_truncate test");
  TCase *s21_roundCase = tcase_create("s21_round test");
  TCase *s21_floorCase = tcase_create("s21_floor test");
  TCase *s21_addCase = tcase_create("s21_add test");
  TCase *s21_subCase = tcase_create("s21_sub test");
  TCase *s21_divCase = tcase_create("s21_div test");
  TCase *s21_mulCase = tcase_create("s21_mul test");
  TCase *s21_modCase = tcase_create("s21_mod test");
  TCase *s21_from_int_to_decimal_Case =
      tcase_create("s21_from_int_to_decimal_test");
  TCase *s21_from_decimal_to_int_Case =
      tcase_create("s21_from_decimal_to_int_test");
  TCase *s21_from_float_to_decimal_Case =
      tcase_create("s21_from_float_to_decimal_test");
  TCase *s21_from_decimal_to_float_Case =
      tcase_create("s21_from_decimal_to_float_test");
  SRunner *sr = srunner_create(s1);

  suite_add_tcase(s1, s21_from_int_to_decimal_Case);
  tcase_add_test(s21_from_int_to_decimal_Case, s21_from_int_to_decimal_test);
  suite_add_tcase(s1, s21_from_decimal_to_int_Case);
  tcase_add_test(s21_from_decimal_to_int_Case, s21_from_decimal_to_int_test);
  suite_add_tcase(s1, s21_from_float_to_decimal_Case);
  tcase_add_test(s21_from_float_to_decimal_Case,
                 s21_from_float_to_decimal_test);
  suite_add_tcase(s1, s21_from_decimal_to_float_Case);
  tcase_add_test(s21_from_decimal_to_float_Case,
                 s21_from_decimal_to_float_test);
  suite_add_tcase(s1, s21_is_lessCase);
  tcase_add_test(s21_is_lessCase, s21_is_less_test);
  suite_add_tcase(s1, s21_is_greaterCase);
  tcase_add_test(s21_is_greaterCase, s21_main_tests);
  suite_add_tcase(s1, s21_is_less_or_equalCase);
  tcase_add_test(s21_is_less_or_equalCase, s21_is_less_or_equal_test);
  suite_add_tcase(s1, s21_is_greater_or_equalCase);
  tcase_add_test(s21_is_greater_or_equalCase, s21_is_greater_or_equal_test);
  suite_add_tcase(s1, s21_is_equalCase);
  tcase_add_test(s21_is_equalCase, s21_is_equal_test);
  suite_add_tcase(s1, s21_is_not_equalCase);
  tcase_add_test(s21_is_not_equalCase, s21_is_not_equal_test);

  suite_add_tcase(s1, s21_negateCase);
  tcase_add_test(s21_negateCase, s21_negate_test);
  suite_add_tcase(s1, s21_truncateCase);
  tcase_add_test(s21_truncateCase, s21_truncate_test);
  suite_add_tcase(s1, s21_roundCase);
  tcase_add_test(s21_roundCase, s21_round_test);
  suite_add_tcase(s1, s21_floorCase);
  tcase_add_test(s21_floorCase, s21_floor_test);

  suite_add_tcase(s1, s21_addCase);
  tcase_add_test(s21_addCase, s21_add_test);
  suite_add_tcase(s1, s21_subCase);
  tcase_add_test(s21_subCase, s21_sub_test);
  suite_add_tcase(s1, s21_divCase);
  tcase_add_test(s21_divCase, s21_div_test);
  suite_add_tcase(s1, s21_mulCase);
  tcase_add_test(s21_mulCase, s21_mul_test);
  suite_add_tcase(s1, s21_modCase);
  tcase_add_test(s21_modCase, s21_mod_test);

  srunner_run_all(sr, CK_VERBOSE);
  srunner_free(sr);
  return 0;
}
