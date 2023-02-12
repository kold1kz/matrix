CC=gcc#-g -fsanitize=address 
CFLAGS= -Wall -Werror -Wextra -std=c11#-pedantic -g -fsanitize=address 
GCOV_FLAGS=-fprofile-arcs -ftest-coverage -lgcov
CHECK_FLAGS=-lcheck -lm -lpthread
ALL_FLAGS=$(CFLAGS) $(GCOV_FLAGS) $(CHECK_FLAGS)

SRCS = s21_matrix.c

OBJS = $(SRCS:.c=.o)
OBJST = $(OBJS) s21_matrix_test.c
NAME = test
REPORT_NAME = report



all: objs s21_matrix.a gcov_report

leaks:
	CK_FORK=no leaks --atExit -- ./test

test: $(OBJST)
	$(CC) $(CHECK_FLAGS) $(OBJS) s21_matrix_test.c -o $(NAME)
	./test


clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf *.dSYM
	rm -rf *.gcno
	rm -rf *.gcda
	rm -rf *.info
	rm -rf *.a
	rm -rf main
	rm -rf $(REPORT_NAME)
	rm -rf *.log
	rm -rf test
	rm -rf CPPLINT.cfg
	rm -rf cpplint.py


objs: $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS) 

s21_matrix.a: $(OBJS)
	ar rc s21_matrix.a $(OBJS)
	ranlib s21_matrix.a
	
check:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.c
	clang-format -n *.h
	rm -rf *.clang-format
	make test
	make clean
	

gcov_report:
	$(CC) -o $(NAME) $(SRCS) s21_matrix_test.c $(GCOV_FLAGS) $(CHECK_FLAGS)
	./$(NAME)
	lcov -t "gcov_report" -o tests.info -c -d .
	genhtml -o $(REPORT_NAME) tests.info
	open report/index.html
	rm $(NAME)
	rm -rf *.gcno
	rm -rf *.gcda

