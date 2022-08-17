CC              = gcc -std=c11
CFLAGS          = 
#-Wextra -Werror -fsanitize=address
EXTRA_CFLAGS    = 
#-ffast-math -fdata-sections -ffunction-sections -Wconversion -Wpointer-arith -Wstrict-prototypes -Wundef -pedantic
TEST_CFLAGS     = -I check/include -L check/Cellar/check/0.15.2/lib -lcheck
OUTPUT_NAME     = s21_decimal
C_FILES         = s21_decimal.c

all: full_clean s21_decimal.a test gcov_report

clean:
	rm -f *.o *.info *.gcno *.gcda gcov_report all

test:
	$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $(TEST_CFLAGS) s21_decimal.c
	ar r s21_decimal.a s21_decimal.o
	$(CC) -L. -I. $(TEST_CFLAGS) -o $(OUTPUT_NAME) s21_decimal.a *.c
	$(CC) -o $(OUTPUT_NAME) --coverage $(CFLAGS) $(EXTRA_CFLAGS) $(TEST_CFLAGS) *.c -lcheck
	./$(OUTPUT_NAME)
	rm -f *.o all

s21_decimal.a:
	$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $(TEST_CFLAGS) *.c
	ar rcs s21_decimal.a s21*.o
	ar rcs libs21_decimal.a s21*.o
	make clean

gcov_report:
	make full_clean
	$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $(TEST_CFLAGS) s21_decimal.c
	ar r s21_decimal.a s21_decimal.o
	$(CC) -o $(OUTPUT_NAME) --coverage $(CFLAGS) $(EXTRA_CFLAGS) $(TEST_CFLAGS) *.c -lcheck   
	./$(OUTPUT_NAME)
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	open ./report/index.html
	make clean

rebuild: 
	rm -f $(OUTPUT_NAME)
	make clean
	make all

full_clean:
	rm -f $(OUTPUT_NAME)
	rm -f *.o *.a *.info *.gcno *.gcda all
	rm -rf report

linters:
	make full_clean
	$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $(TEST_CFLAGS) s21_decimal.c
	ar r s21_decimal.a s21_decimal.o
	$(CC) -o $(OUTPUT_NAME) $(CFLAGS) $(EXTRA_CFLAGS) $(TEST_CFLAGS) *.c -lcheck   

	./$(OUTPUT_NAME)
	``leaks -atExit -- ./$(OUTPUT_NAME)``
	``cp ../materials/linters/CPPLINT.cfg ./``
	``python3 ../materials/linters/cpplint.py --extensions=c *.c``
