#include "s21_decimal.h"

// ARITHMETIC OPERATORS
s21_decimal s21_add(s21_decimal a, s21_decimal b) {
  s21_decimal result = s21_init_dec(0);
  if (a.value_type == s21_NAN || b.value_type == s21_NAN) {
    result.value_type = s21_NAN;
  } else if (a.value_type != s21_NORMAL_VALUE) {
    if (b.value_type != s21_NORMAL_VALUE && a.value_type != b.value_type) {
      result.value_type = s21_NAN;
    } else {
      result.value_type = a.value_type;
    }
  } else {
    result.value_type = b.value_type;
  }

  if (result.value_type == s21_NORMAL_VALUE) {
    s21_double_decimal na, nb;
    s21_equalize(a, b, &na, &nb);
    int signa = s21_get_sign(a.bits, INT_DEC_SIZE),
        signb = s21_get_sign(b.bits, INT_DEC_SIZE);
    if (!(signa ^ signb)) {
      s21_big_int_sum((unsigned *)na.bits, (unsigned *)nb.bits,
                      (unsigned *)na.bits, INT_DOUBLE_DEC_SIZE);
      s21_set_sign(na.bits, signa, INT_DOUBLE_DEC_SIZE);
    } else if (s21_big_int_sub((unsigned *)na.bits, (unsigned *)nb.bits,
                               (unsigned *)na.bits, INT_DOUBLE_DEC_SIZE)) {
      s21_set_sign(na.bits, signb, INT_DOUBLE_DEC_SIZE);
    } else {
      s21_set_sign(na.bits, signa, INT_DOUBLE_DEC_SIZE);
    }
    result = s21_ddec_to_dec(na);
  }
  return result;
}

s21_decimal s21_sub(s21_decimal a, s21_decimal b) {
  return s21_add(a, s21_negate(b));
}

s21_decimal s21_mul(s21_decimal a, s21_decimal b) {
  s21_decimal result = s21_init_dec(0);
  s21_decimal zero = s21_init_dec(0);
  int needmult = 0,
      signa = s21_get_sign(a.bits, INT_DEC_SIZE) ||
              a.value_type == s21_NEGATIVE_INFINITY,
      signb = s21_get_sign(b.bits, INT_DEC_SIZE) ||
              b.value_type == s21_NEGATIVE_INFINITY;

  if (a.value_type == s21_NAN || b.value_type == s21_NAN) {
    result.value_type = s21_NAN;
  } else if (!s21_is_equal(zero, a) || !s21_is_equal(zero, b)) {
    if (a.value_type != b.value_type) {
      result.value_type = s21_NAN;
    } else {
      result.value_type = s21_NORMAL_VALUE;
      s21_set_sign(result.bits, signa ^ signb, INT_DEC_SIZE);
    }
  } else if (a.value_type != s21_NORMAL_VALUE ||
             b.value_type != s21_NORMAL_VALUE) {
    result.value_type = s21_INFINITY + (signa ^ signb);
  } else {
    result.value_type = s21_NORMAL_VALUE;
    needmult = 1;
  }

  if (needmult) {
    s21_double_decimal na = s21_init_ddec(0);
    s21_big_int_mult((unsigned *)a.bits, (unsigned *)b.bits, NULL,
                     (unsigned *)na.bits, INT_DEC_SIZE);
    s21_set_exp(
        na.bits,
        s21_get_exp(a.bits, INT_DEC_SIZE) + s21_get_exp(b.bits, INT_DEC_SIZE),
        INT_DOUBLE_DEC_SIZE);
    s21_set_sign(na.bits,
                 (s21_get_sign(a.bits, INT_DEC_SIZE) ^
                  s21_get_sign(b.bits, INT_DEC_SIZE)),
                 INT_DOUBLE_DEC_SIZE);
    result = s21_ddec_to_dec(na);
  }

  return result;
}

s21_decimal s21_div(s21_decimal a, s21_decimal b) {
  s21_decimal result = s21_init_dec(0), zero = s21_init_dec(0);
  int need_div = 0,
      signa = (s21_get_sign(a.bits, INT_DEC_SIZE) ||
               a.value_type == s21_NEGATIVE_INFINITY),
      signb = s21_get_sign(b.bits, INT_DEC_SIZE) ||
              (b.value_type == s21_NEGATIVE_INFINITY);

  if (a.value_type == s21_NAN || b.value_type == s21_NAN) {
    result.value_type = s21_NAN;
  } else if (s21_is_equal_mod(b, zero)) {
    if (!s21_is_equal_mod(a, zero)) {
      result.value_type = s21_INFINITY + (signa ^ signb);
    } else {
      result.value_type = s21_NAN;
    }
  } else if (a.value_type != s21_NORMAL_VALUE) {
    if (b.value_type != s21_NORMAL_VALUE) {
      result.value_type = s21_NAN;
    } else {
      result.value_type = a.value_type;
      if (signb) result = s21_negate(result);
    }
  } else if (s21_is_equal_mod(a, zero)) {
    result.value_type = s21_NORMAL_VALUE;
    s21_set_sign(result.bits, signa ^ signb, INT_DEC_SIZE);
  } else if (b.value_type != s21_NORMAL_VALUE) {
    result.value_type = s21_NORMAL_VALUE;
    s21_set_sign(result.bits, signa ^ signb, INT_DEC_SIZE);
  } else if (s21_is_equal_mod(a, b)) {
    result.value_type = s21_NORMAL_VALUE;
    result = s21_init_dec(1);
    s21_set_sign(result.bits, signa ^ signb, INT_DEC_SIZE);
  } else {
    result.value_type = s21_NORMAL_VALUE;
    need_div = 1;
  }

  if (need_div) {
    s21_double_decimal na = s21_init_ddec(0), nb = s21_dec_to_ddec(b),
                       nresult = na, nrem = na, ten = s21_init_ddec(10);
    int aexp = s21_get_exp(a.bits, INT_DEC_SIZE), iszero = 1;
    s21_decimal bten = s21_ten_pow(s21_get_exp(b.bits, INT_DEC_SIZE));
    s21_big_int_mult((unsigned *)a.bits, (unsigned *)bten.bits, NULL,
                     (unsigned *)na.bits, INT_DEC_SIZE);
    s21_big_int_div((unsigned *)na.bits, (unsigned *)nb.bits,
                    (unsigned *)nresult.bits, (unsigned *)nrem.bits,
                    INT_DOUBLE_DEC_SIZE);
    for (int i = 0; i < INT_DEC_SIZE; ++i) {
      if (nrem.bits[i]) iszero = 0;
    }
    while (!iszero && aexp < 28 && !nresult.bits[3]) {
      s21_big_int_mult((unsigned *)nresult.bits, (unsigned *)ten.bits,
                       (unsigned *)nresult.bits, NULL, INT_DOUBLE_DEC_SIZE);
      s21_big_int_mult((unsigned *)nrem.bits, (unsigned *)ten.bits,
                       (unsigned *)nrem.bits, NULL, INT_DOUBLE_DEC_SIZE);
      s21_big_int_div((unsigned *)nrem.bits, (unsigned *)nb.bits,
                      (unsigned *)na.bits, (unsigned *)nrem.bits,
                      INT_DOUBLE_DEC_SIZE);
      s21_big_int_sum((unsigned *)nresult.bits, (unsigned *)na.bits,
                      (unsigned *)nresult.bits, INT_DOUBLE_DEC_SIZE);

      iszero = 1;
      for (int i = 0; i < INT_DEC_SIZE; ++i) {
        if (nrem.bits[i]) iszero = 0;
      }
      ++aexp;
    }

    if (nresult.bits[3]) {
      unsigned int tmp[INT_DOUBLE_DEC_SIZE];
      s21_big_int_div((unsigned *)nresult.bits, (unsigned[]){10, 0, 0, 0, 0, 0},
                      (unsigned *)nresult.bits, tmp, INT_DOUBLE_DEC_SIZE);
      aexp--;
      if (tmp[0] > 5) {
        s21_big_int_sum((unsigned *)nresult.bits,
                        (unsigned[]){1, 0, 0, 0, 0, 0},
                        (unsigned *)nresult.bits, INT_DOUBLE_DEC_SIZE);
      } else if (tmp[0] == 5) {
        if (s21_big_int_cmp((unsigned *)nrem.bits,
                            (unsigned[]){0, 0, 0, 0, 0, 0},
                            INT_DOUBLE_DEC_SIZE)) {
          s21_big_int_sum((unsigned *)nresult.bits,
                          (unsigned[]){1, 0, 0, 0, 0, 0},
                          (unsigned *)nresult.bits, INT_DOUBLE_DEC_SIZE);
        }
      }
    } else {
      unsigned int rem05[INT_DOUBLE_DEC_SIZE] = {5, 0, 0, 0, 0, 0};
      s21_big_int_mult(rem05, (unsigned *)nb.bits, rem05, NULL,
                       INT_DOUBLE_DEC_SIZE);
      s21_big_int_mult((unsigned *)nrem.bits, (unsigned[]){10, 0, 0, 0, 0},
                       (unsigned *)nrem.bits, NULL, INT_DOUBLE_DEC_SIZE);
      int tmp;
      if ((tmp = s21_big_int_cmp((unsigned *)nrem.bits, rem05,
                                 INT_DOUBLE_DEC_SIZE)) > 0 ||
          (tmp == 0 && (nresult.bits[0] & 1u))) {
        s21_big_int_sum((unsigned *)nresult.bits,
                        (unsigned[]){1, 0, 0, 0, 0, 0},
                        (unsigned *)nresult.bits, INT_DOUBLE_DEC_SIZE);
      }
    }
    s21_set_sign(nresult.bits, signa ^ signb, INT_DOUBLE_DEC_SIZE);
    s21_set_exp(nresult.bits, aexp, INT_DOUBLE_DEC_SIZE);
    result = s21_ddec_to_dec(nresult);  // ERROR ???
  }
  return result;
}

s21_decimal s21_mod(s21_decimal a, s21_decimal b) {
  s21_decimal result = s21_init_dec(0), zero = s21_init_dec(0);
  int need_div = 0, signa = s21_get_sign(a.bits, INT_DEC_SIZE);
  if (a.value_type == s21_NAN || b.value_type == s21_NAN ||
      s21_is_equal_mod(b, zero)) {
    result.value_type = s21_NAN;
  } else if (a.value_type != s21_NORMAL_VALUE) {
    result.value_type = s21_NAN;
  } else if (s21_is_equal_mod(a, zero)) {
    result.value_type = s21_NORMAL_VALUE;
    s21_set_sign(result.bits, signa, INT_DEC_SIZE);
  } else if (b.value_type != s21_NORMAL_VALUE) {
    result = a;
  } else if (s21_is_equal_mod(a, b)) {
    result.value_type = s21_NORMAL_VALUE;
    s21_set_sign((unsigned *)result.bits, signa, INT_DEC_SIZE);
  } else {
    result.value_type = s21_NORMAL_VALUE;
    need_div = 1;
  }

  if (need_div) {
    s21_double_decimal na, nb;
    s21_equalize(a, b, &na, &nb);
    if (s21_big_int_cmp((unsigned *)na.bits, (unsigned *)nb.bits,
                        INT_DOUBLE_DEC_SIZE) > 0) {
      s21_big_int_div((unsigned *)na.bits, (unsigned *)nb.bits, NULL,
                      (unsigned *)na.bits, INT_DOUBLE_DEC_SIZE);
    }
    s21_set_sign(na.bits, signa, INT_DOUBLE_DEC_SIZE);
    result = s21_ddec_to_dec(na);
  }
  return result;
}

// COMPARISON OPERATORS
int s21_is_less(s21_decimal a, s21_decimal b) {
  int result = 1, signa = s21_get_sign(a.bits, INT_DEC_SIZE),
      signb = s21_get_sign(b.bits, INT_DEC_SIZE);
  if (a.value_type != s21_NAN && b.value_type != s21_NAN) {
    if (a.value_type == b.value_type && signa == signb) {
      if (a.value_type == s21_NORMAL_VALUE) {
        s21_double_decimal na = s21_init_ddec(0), nb = s21_init_ddec(0);
        s21_equalize(a, b, &na, &nb);
        result = s21_big_int_cmp((unsigned *)na.bits, (unsigned *)nb.bits,
                                 INT_DOUBLE_DEC_SIZE);
        if ((result > 0 && signa) || (result < 0 && !signa)) {
          result = 0;
        } else {
          result = 1;
        }
      } else {
        result = 1;
      }
    } else if ((a.value_type == s21_NEGATIVE_INFINITY &&
                b.value_type != s21_NEGATIVE_INFINITY) ||
               (a.value_type == s21_NORMAL_VALUE &&
                b.value_type == s21_INFINITY) ||
               (signa > signb) && a.value_type == s21_NORMAL_VALUE &&
                   b.value_type == s21_NORMAL_VALUE) {
      result = 0;
    } else {
      result = 1;
    }
  }
  return result;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_equal(a, b) && s21_is_less(a, b);
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  return (!s21_is_less_or_equal(a, b)) || a.value_type == s21_NAN ||
         b.value_type == s21_NAN;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_greater(a, b) && s21_is_equal(a, b);
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  int result = s21_is_equal_mod(a, b) && (s21_get_sign(a.bits, INT_DEC_SIZE) ==
                                          s21_get_sign(b.bits, INT_DEC_SIZE));
  return !result;
}

int s21_is_equal_mod(s21_decimal a, s21_decimal b) {
  s21_double_decimal na = s21_init_ddec(0), nb = s21_init_ddec(0);
  s21_equalize(a, b, &na, &nb);
  int result = 1;
  if ((a.value_type == b.value_type) && (a.value_type != s21_NAN)) {
    if (a.value_type == s21_NORMAL_VALUE) {
      result = !s21_big_int_cmp((unsigned *)na.bits, (unsigned *)nb.bits,
                                INT_DOUBLE_DEC_SIZE);
    }
  } else {
    result = 0;
  }
  return result;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return !(s21_is_equal(a, b));
}

// CONVERTORS
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  *dst = s21_init_dec(0);
  dst->bits[0] |= src;
  if (src & (1u << (INT_BITSIZE - 1))) {
    *dst = s21_negate(*dst);
    dst->bits[0] -= 1;
    dst->bits[0] = ~(dst->bits[0]);
  }
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int flag = 1;
  if (dst && src.value_type != s21_NAN && src.value_type != s21_INFINITY &&
      src.value_type != s21_NEGATIVE_INFINITY) {
    s21_decimal tmp = s21_truncate(src);
    if (tmp.bits[1] == 0 && tmp.bits[2] == 0 &&
        (unsigned)tmp.bits[0] < 0xffffffff) {
      *dst = 0;
      int sign = s21_get_sign(src.bits, INT_DEC_SIZE);
      if (sign) {
        *dst = -tmp.bits[0];
      } else {
        *dst = tmp.bits[0];
      }
      flag = 0;
    }
  }
  return flag;
}

int getBinExp(float src) {
  unsigned int fbits = *((unsigned int *)&src);
  int result = 0, pow_2 = 0b10000000;
  for (int mask = 0x40000000; mask > 0x00400000; mask >>= 1, pow_2 >>= 1) {
    result += pow_2 * !!(mask & fbits);
  }
  return result - 127;
}

int setBit(s21_decimal *dst, int position) {
  int answer = 0;
  if (dst && position < 97) {
    int work_bit = 0, pos_in_bit = 0;
    if (position < 32) {
      work_bit = 0;
      pos_in_bit = position;
    } else if (position < 64) {
      work_bit = 1;
      pos_in_bit = position - 32;
    } else {
      work_bit = 2;
      pos_in_bit = position - 64;
    }
    unsigned int mask = 1;
    for (int i = 0; i < pos_in_bit; i++) mask <<= 1;
    dst->bits[work_bit] |= mask;
  } else {
    answer = 1;
  }
  return answer;
}

void setPower(s21_decimal *dst, int power) {
  unsigned int mask = 0x00800000, pow_mask = 0x00000080, temp = power;
  for (int i = 0; i < 8; i++, mask >>= 1, pow_mask >>= 1) {
    if (!!(pow_mask & temp)) {
      dst->bits[INT_DEC_SIZE] |= mask;
    }
  }
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int answer = 0;
  unsigned int sign = 0;
  if (src < 0.0) {
    src *= -1;
    sign = 1;
  }
  if (dst) {
    dst->bits[1] = dst->bits[2] = dst->bits[3] = dst->bits[0] =
        dst->value_type = 0;
    if (src > 1e-28) {
      int counter = 0, floatexp = getBinExp(src);
      if (floatexp <= 95) {
        for (; src <= 0.1e8; src *= 10, counter++) {
        }
        if (counter <= 28) {
          unsigned int fbits = *((unsigned int *)&src);
          floatexp = getBinExp(src);
          setBit(dst, floatexp--);
          for (unsigned int mask = 0x400000; mask > 0; mask >>= 1, floatexp--) {
            if (!!(mask & fbits)) setBit(dst, floatexp);
          }
          setPower(dst, counter);
          sign <<= 31;
          dst->bits[INT_DEC_SIZE] |= sign;
        } else {
          dst->value_type = s21_INFINITY + sign;
          answer = 1;
        }
      } else {
        dst->value_type = s21_INFINITY + sign;
        answer = 1;
      }
    } else {
      dst->value_type = s21_NAN;
      answer = 1;
    }
  } else {
    answer = 1;
  }
  return answer;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {  // РАБОТАЕТ
  float result = 0.0;
  int answer = 0;
  if (dst && src.value_type == 0) {
    int pow = s21_get_exp(src.bits, INT_DEC_SIZE);
    double temp;
    temp = INT_POW_2_64 * src.bits[2];
    temp += INT_POW_2_32 * src.bits[1];
    temp += src.bits[0];
    for (int i = 0; i < pow; i++) {
      temp /= 10;
    }
    result = temp;
    if ((unsigned int)src.bits[3] >> 31 == 1) {
      result *= -1;
    }
    *dst = result;
  } else {
    answer = 1;
  }
  return answer;
}

// OTHER MATHEMATICAL FUNCTIONS
s21_decimal s21_floor(s21_decimal a) {
  if (a.value_type == s21_NORMAL_VALUE) {
    s21_decimal adenom = s21_ten_pow(s21_get_exp(a.bits, INT_DEC_SIZE));
    s21_big_int_div((unsigned *)a.bits, (unsigned *)adenom.bits,
                    (unsigned *)a.bits, NULL, INT_DEC_SIZE);
    if (s21_get_sign(a.bits, INT_DEC_SIZE)) {
      unsigned int one[3] = {1, 0, 0};
      s21_big_int_sum((unsigned *)a.bits, one, (unsigned *)a.bits,
                      INT_DEC_SIZE);
    }
    s21_set_exp(a.bits, 0, INT_DEC_SIZE);
  } else {
    value_type_t aval = a.value_type;
    a = s21_init_dec(0);
    a.value_type = aval;
  }
  return a;
}

s21_decimal s21_round(s21_decimal a) {
  if (a.value_type == s21_NORMAL_VALUE) {
    s21_decimal adenom = s21_ten_pow(s21_get_exp(a.bits, INT_DEC_SIZE)),
                rem = s21_init_dec(0);
    s21_big_int_div((unsigned *)a.bits, (unsigned *)adenom.bits,
                    (unsigned *)a.bits, (unsigned *)rem.bits, INT_DEC_SIZE);
    s21_set_exp(rem.bits, s21_get_exp(a.bits, INT_DEC_SIZE), INT_DEC_SIZE);
    s21_set_exp(a.bits, 0, INT_DEC_SIZE);
    s21_decimal rem5 = s21_init_dec(5);
    s21_set_exp(rem5.bits, 1, INT_DEC_SIZE);
    if (!s21_is_greater_or_equal(rem, rem5)) {
      s21_decimal one = s21_init_dec(1);
      s21_set_sign(one.bits, s21_get_sign(a.bits, INT_DEC_SIZE), INT_DEC_SIZE);
      a = s21_add(a, one);
    }
  } else {
    value_type_t aval = a.value_type;
    a = s21_init_dec(0);
    a.value_type = aval;
  }
  return a;
}

s21_decimal s21_truncate(s21_decimal src) {
  s21_decimal ans = s21_init_dec(0);
  ans.value_type = src.value_type;
  if (src.value_type == s21_NORMAL_VALUE) {
    s21_decimal tenpowed = s21_ten_pow(s21_get_exp(src.bits, INT_DEC_SIZE));
    s21_big_int_div((unsigned *)src.bits, (unsigned *)tenpowed.bits,
                    (unsigned *)ans.bits, NULL, INT_DEC_SIZE);
    s21_set_sign(ans.bits, s21_get_sign(src.bits, INT_DEC_SIZE), INT_DEC_SIZE);
  }
  return ans;
}

s21_decimal s21_negate(s21_decimal a) {
  if (a.value_type == s21_INFINITY) {
    a.value_type = s21_NEGATIVE_INFINITY;
  } else if (a.value_type == s21_NEGATIVE_INFINITY) {
    a.value_type = s21_INFINITY;
  } else if (a.value_type == s21_NORMAL_VALUE) {
    a.bits[INT_DEC_SIZE] ^= S21_DEC_SIGN;
  }
  return a;
}

// HELP FUNCTIONS
int s21_get_exp(int *bits, int size) {
  return (bits[size] & S21_DEC_EXPONENT) >> 16;
}

int s21_get_sign(int *bits, int size) {
  return (bits[size] & S21_DEC_SIGN) >> 31;
}

int s21_set_exp(int *bits, unsigned int exp, int size) {
  bits[size] &= ~(S21_DEC_EXPONENT);
  bits[size] |= (exp << 16);
  int flag = 0;
  if (exp > 28) flag = 1;
  return flag;
}

int s21_set_sign(int *bits, unsigned int sign, int size) {
  bits[size] &= ~(S21_DEC_SIGN);
  bits[size] |= (sign << 31);
  int flag = 0;
  if (sign != 0 && sign != 1) flag = 1;
  return flag;
}

void s21_equalize(s21_decimal a, s21_decimal b, s21_double_decimal *na,
                  s21_double_decimal *nb) {
  int expa = s21_get_exp(a.bits, INT_DEC_SIZE),
      expb = s21_get_exp(b.bits, INT_DEC_SIZE);
  *na = s21_init_ddec(0);
  *nb = s21_init_ddec(0);
  if (expa == expb) {
    *na = s21_dec_to_ddec(a);
    *nb = s21_dec_to_ddec(b);
  } else if (expa > expb) {
    *na = s21_dec_to_ddec(a);
    s21_decimal ten_exp = s21_ten_pow(expa - expb);
    s21_big_int_mult((unsigned *)b.bits, (unsigned *)ten_exp.bits, NULL,
                     (unsigned *)nb->bits, INT_DEC_SIZE);
    s21_set_exp(nb->bits, expa, INT_DOUBLE_DEC_SIZE);
  } else {
    *nb = s21_dec_to_ddec(b);
    s21_decimal ten_exp = s21_ten_pow(expb - expa);
    s21_big_int_mult((unsigned *)a.bits, (unsigned *)ten_exp.bits, NULL,
                     (unsigned *)na->bits, INT_DEC_SIZE);
    s21_set_exp(na->bits, expb, INT_DOUBLE_DEC_SIZE);
  }
}

int s21_big_int_cmp(unsigned int *a, unsigned int *b, int size) {
  int result = 0;
  for (int i = size - 1; i >= 0 && !result; --i) {
    if (a[i] > b[i])
      result = 1;
    else if (a[i] < b[i])
      result = -1;
  }
  return result;
}

int s21_big_int_shift_left(unsigned int *a, int size, unsigned int shift) {
  int flag = 0;
  for (int i = INT_BITSIZE * size - 1; i >= 0; --i) {
    int arr_i = i / INT_BITSIZE;
    int bit_i = i % INT_BITSIZE;
    unsigned int val = (a[arr_i] & (1u << bit_i)) >> bit_i;
    a[arr_i] &= ~(1u << bit_i);
    int new_i = i + shift;
    if (new_i < INT_BITSIZE * size) {
      int new_arr_i = new_i / INT_BITSIZE;
      int new_bit_i = new_i % INT_BITSIZE;
      a[new_arr_i] |= (val << new_bit_i);
    } else if (val) {
      flag = 1;
    }
  }
  return flag;
}

int s21_big_int_shift_right(unsigned int *a, int size, unsigned int shift) {
  int flag = 0;
  for (int i = 0; i < INT_BITSIZE * size; ++i) {
    int arr_i = i / INT_BITSIZE;
    int bit_i = i % INT_BITSIZE;
    unsigned int val = (a[arr_i] & (1u << bit_i)) >> bit_i;
    a[arr_i] &= ~(1u << bit_i);
    int new_i = i - shift;
    if (new_i >= 0) {
      int new_arr_i = new_i / INT_BITSIZE;
      int new_bit_i = new_i % INT_BITSIZE;
      a[new_arr_i] |= (val << new_bit_i);
    } else if (val) {
      flag = 1;
    }
  }
  return flag;
}

int s21_big_int_sum(unsigned int *a, unsigned int *b, unsigned int *result,
                    int size) {
  int overflow = 0;
  for (int i = 0; i < INT_BITSIZE * size; ++i) {
    int arr_i = i / INT_BITSIZE;
    int bit_i = i % INT_BITSIZE;
    unsigned int val = ((a[arr_i] & (1u << bit_i)) >> bit_i) +
                       ((b[arr_i] & (1u << bit_i)) >> bit_i) + overflow;
    overflow = val / 2;
    if (val > 1) val -= 2;
    if (result) {
      result[arr_i] &= ~(1u << bit_i);
      result[arr_i] |= (val << bit_i);
    }
  }
  return overflow;
}

int s21_big_int_sub(unsigned int *a, unsigned int *b, unsigned int *result,
                    int size) {
  int overflow = 0, flag = 0;
  unsigned int *big = s21_big_int_cmp(a, b, size) > 0 ? a : b;
  unsigned int *small = big == a ? b : a;
  if (big == b) flag = 1;

  for (int i = 0; i < INT_BITSIZE * size; ++i) {
    int arr_i = i / INT_BITSIZE;
    int bit_i = i % INT_BITSIZE;
    int val = ((big[arr_i] & (1u << bit_i)) >> bit_i) -
              ((small[arr_i] & (1u << bit_i)) >> bit_i) + overflow;
    if (val < 0) {
      overflow = 1;
      val += 2;
    } else if (val == 2) {
      overflow = 0;
      val = 0;
    }
    if (result) {
      result[arr_i] &= ~(1u << bit_i);
      result[arr_i] |= ((unsigned int)val) << bit_i;
    }
  }
  return flag;
}

int s21_big_int_mult(unsigned int *a, unsigned int *b, unsigned int *result,
                     unsigned int *fullresult, int size) {
  int flag = 0;
  unsigned int *tempb = (unsigned int *)malloc(sizeof(int) * size * 2);
  unsigned int *tempresult = (unsigned int *)malloc(sizeof(int) * size * 2);
  for (int i = 0; i < size; ++i) {
    tempresult[i] = 0;
    tempb[i] = b[i];
  }
  for (int i = size; i < 2 * size; ++i) {
    tempb[i] = 0;
    tempresult[i] = 0;
  }

  for (int i = 0; i < INT_BITSIZE * size; ++i) {
    int arr_i = i / INT_BITSIZE;
    int bit_i = i % INT_BITSIZE;
    if ((a[arr_i] & (1u << bit_i))) {
      s21_big_int_sum(tempb, tempresult, tempresult, size * 2);
    }
    s21_big_int_shift_left(tempb, size * 2, 1);
  }

  if (fullresult) {
    for (int i = 0; i < size * 2; ++i) {
      fullresult[i] = tempresult[i];
    }
  }
  for (int i = size; i < 2 * size; ++i) {
    if (tempresult[i]) flag = 1;
  }
  if (result) {
    for (int i = 0; i < size; ++i) {
      result[i] = tempresult[i];
    }
  }

  free(tempb);
  free(tempresult);
  return flag;
}

s21_decimal s21_ten_pow(unsigned int exp) {
  s21_decimal ten = s21_init_dec(10);
  s21_decimal result = s21_init_dec(1);
  for (unsigned int i = 0; i < exp; ++i) {
    s21_big_int_mult((unsigned *)ten.bits, (unsigned *)result.bits,
                     (unsigned *)result.bits, NULL, INT_DEC_SIZE);
  }
  if (exp > 28) {
    result.value_type = s21_INFINITY;
  }
  return result;
}

s21_double_decimal s21_ten_double_pow(unsigned int exp) {
  s21_double_decimal ten = s21_init_ddec(10);
  s21_double_decimal result = s21_init_ddec(1);
  for (unsigned int i = 0; i < exp; ++i) {
    s21_big_int_mult((unsigned *)ten.bits, (unsigned *)result.bits,
                     (unsigned *)result.bits, NULL, INT_DOUBLE_DEC_SIZE);
  }
  if (exp > 28 * 2) {
    result.value_type = s21_INFINITY;
  }
  return result;
}

s21_decimal s21_init_dec(unsigned int val) {
  s21_decimal new;
  new.bits[0] = val;
  new.bits[1] = 0;
  new.bits[2] = 0;
  new.bits[INT_DEC_SIZE] = 0;
  new.value_type = s21_NORMAL_VALUE;
  return new;
}

s21_decimal s21_ddec_to_dec(s21_double_decimal num) {
  s21_decimal result = s21_init_dec(0);
  unsigned int flag = 1, exp = s21_get_exp(num.bits, INT_DOUBLE_DEC_SIZE),
               sign = s21_get_sign(num.bits, INT_DOUBLE_DEC_SIZE);
  for (int i = INT_DEC_SIZE; i < INT_DOUBLE_DEC_SIZE && flag; ++i) {
    if (num.bits[i]) {
      flag = 0;
    }
  }
  if (flag && exp <= 28) {  // DDEC CAN BE COPIED
    for (int i = 0; i < INT_DEC_SIZE; ++i) {
      result.bits[i] = num.bits[i];
    }
    s21_set_exp(result.bits, exp, INT_DEC_SIZE);
    s21_set_sign(result.bits, sign, INT_DEC_SIZE);
  } else {  // NEED TO FIND INT PART
    unsigned int intdec[INT_DOUBLE_DEC_SIZE], rem[INT_DOUBLE_DEC_SIZE];
    if (exp && exp <= 28 * 2) {
      s21_double_decimal tenpowed = s21_ten_double_pow(exp);
      s21_big_int_div((unsigned *)num.bits, (unsigned *)tenpowed.bits, intdec,
                      rem, INT_DOUBLE_DEC_SIZE);

      flag = 1;
      for (int i = INT_DEC_SIZE; i < INT_DOUBLE_DEC_SIZE && flag; ++i) {
        if (intdec[i]) {
          flag = 0;
        }
      }
    }
    if (flag) {  // IF INT PART IN RANGE OF DECIMAL
      int tenexp = 0, lastfound = 0;
      s21_double_decimal ten = s21_init_ddec(10);

      while (!lastfound && exp - tenexp > 0 &&
             tenexp < 28) {  // SHIFTING INT PART MAXIMUM TO THE RIGHT
        if (!s21_big_int_mult(intdec, (unsigned *)ten.bits,
                              (unsigned *)result.bits, intdec, INT_DEC_SIZE)) {
          ++tenexp;
        } else {
          s21_big_int_div(intdec, (unsigned *)ten.bits, intdec, NULL,
                          INT_DOUBLE_DEC_SIZE);
          for (int i = 0; i < INT_DEC_SIZE; ++i) {
            result.bits[i] = intdec[i];
          }
          lastfound = 1;
        }
        if (result.bits[INT_DEC_SIZE - 1] & (1u << (INT_BITSIZE - 1)))
          lastfound = 1;
      }

      unsigned int five[6] = {5, 0, 0, 0, 0, 0};
      ten = s21_ten_double_pow(exp - tenexp);

      s21_big_int_div(rem, (unsigned *)ten.bits, intdec, rem,
                      INT_DOUBLE_DEC_SIZE);
      if (s21_big_int_sum((unsigned *)result.bits, intdec, NULL,
                          INT_DEC_SIZE)) {  // ON OVERFLOW
        unsigned int temp[INT_DOUBLE_DEC_SIZE];
        s21_big_int_mult(intdec, (unsigned int *)ten.bits, temp, NULL,
                         INT_DOUBLE_DEC_SIZE);
        s21_big_int_sum(rem, temp, rem, INT_DOUBLE_DEC_SIZE);
        s21_big_int_div((unsigned *)result.bits,
                        (unsigned[]){10, 0, 0, 0, 0, 0},
                        (unsigned *)result.bits, temp, INT_DEC_SIZE);
        tenexp--;
        ten = s21_ten_double_pow(exp - tenexp);
        s21_big_int_div(rem, (unsigned *)ten.bits, intdec, rem,
                        INT_DOUBLE_DEC_SIZE);
      }
      s21_big_int_sum((unsigned *)result.bits, intdec, (unsigned *)result.bits,
                      INT_DEC_SIZE);

      s21_big_int_mult(five, (unsigned *)ten.bits, five, NULL,
                       INT_DOUBLE_DEC_SIZE);
      s21_big_int_mult(rem, (unsigned[]){10, 0, 0, 0, 0, 0}, rem, NULL,
                       INT_DOUBLE_DEC_SIZE);

      {
        int temp;
        if ((temp = s21_big_int_cmp(five, rem, INT_DOUBLE_DEC_SIZE)) < 0 ||
            (temp == 0 &&
             (result.bits[0] & 1u))) {  // ADDING 1 IF REMAINDER MORE OR EQ 0.5
          s21_big_int_sum((unsigned *)result.bits, (unsigned[]){1, 0, 0},
                          (unsigned *)result.bits, INT_DEC_SIZE);
        }
      }
      s21_set_exp(result.bits, tenexp > 28 ? 28 : tenexp, INT_DEC_SIZE);
      s21_set_sign(result.bits, sign, INT_DEC_SIZE);

    } else {
      if (sign) {
        result.value_type = s21_NEGATIVE_INFINITY;
      } else {
        result.value_type = s21_INFINITY;
      }
    }
  }
  return result;
}

s21_double_decimal s21_init_ddec(unsigned int val) {
  s21_double_decimal new;
  for (int i = 0; i <= INT_DOUBLE_DEC_SIZE; ++i) {
    new.bits[i] = 0;
  }
  new.bits[0] = val;
  new.value_type = s21_NORMAL_VALUE;
  return new;
}

s21_double_decimal s21_dec_to_ddec(s21_decimal num) {
  s21_double_decimal result;
  for (int i = 0; i <= INT_DEC_SIZE; ++i) {
    result.bits[i] = num.bits[i];
  }
  for (int i = INT_DEC_SIZE; i < INT_DOUBLE_DEC_SIZE; ++i) {
    result.bits[i] = 0;
  }
  result.bits[INT_DOUBLE_DEC_SIZE] = num.bits[INT_DEC_SIZE];
  result.value_type = num.value_type;
  return result;
}

int s21_big_int_div(unsigned int *a, unsigned int *b, unsigned int *result,
                    unsigned int *remainder, int size) {
  int flag = 0, needshift = 1;
  unsigned int *tempa = (unsigned int *)malloc(sizeof(int) * size);
  unsigned int *tempb = (unsigned int *)malloc(sizeof(int) * size);
  unsigned int *tempzero = (unsigned int *)malloc(sizeof(int) * size);
  unsigned int *tempone = (unsigned int *)malloc(sizeof(int) * size);
  unsigned int *tempresult = (unsigned int *)malloc(sizeof(int) * size);
  for (int i = 0; i < size; ++i) {
    tempa[i] = a[i];
    tempb[i] = b[i];
    tempzero[i] = 0;
    tempone[i] = 0;
    tempresult[i] = 0;
  }
  tempone[0] = 1u;

  if (s21_big_int_cmp(tempzero, b, size) == 0) {
    flag = 1;
  } else {
    while (s21_big_int_cmp(tempa, b, size) >= 0) {
      if (s21_big_int_cmp(tempa, tempb, size) >= 0 && needshift) {
        if (tempb[size - 1] < 0x80000000) {
          s21_big_int_shift_left(tempb, size, 1);
          s21_big_int_shift_left(tempone, size, 1);
          needshift = 1;
        } else {
          needshift = 0;
        }
      } else {
        if (needshift) {
          s21_big_int_shift_right(tempb, size, 1);
          s21_big_int_shift_right(tempone, size, 1);
        }
        needshift = 1;
        s21_big_int_sub(tempa, tempb, tempa, size);
        s21_big_int_sum(tempresult, tempone, tempresult, size);
        for (int i = 0; i < size; ++i) {
          tempb[i] = b[i];
          tempone[i] = 0;
        }
        tempone[0] = 1u;
      }
    }
    if (result != NULL) {
      for (int i = 0; i < size; ++i) {
        result[i] = tempresult[i];
      }
    }
    if (remainder != NULL) {
      for (int i = 0; i < size; ++i) {
        remainder[i] = tempa[i];
      }
    }
  }
  free(tempa);
  free(tempb);
  free(tempzero);
  free(tempone);
  free(tempresult);
  return flag;
}
