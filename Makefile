CC=g++
STD=-std=c++20
W=-Wall -Werror -Wextra

all: clean matrixOperations matrixOperationsLib matrixProgram main

matrixOperations:
	@$(CC) --coverage -g -O0 $(STD) $(W) matrixOperations.cpp -o test_matrixOperations
	./test_matrixOperations
	@echo "---------------------"
	@gcov test_matrixOperations-matrixOperations.cpp | grep -A 1 "File 'matrixOperations.hpp'"
	@echo "---------------------"
	@rm ./test_matrixOperations
	@rm *.gcov *.gcda *.gcno
	@rm -rf *.dSYM

matrixOperationsLib:
	@$(CC) -fPIC -shared -O0 $(STD) $(W) -o libmatrixOperations.so matrixOperations.cpp

matrixProgram:
	@$(CC) -L. -lmatrixOperations --coverage -g -O0 $(STD) $(W) matrixProgram.cpp -o test_matrixProgram
	./test_matrixProgram
	@echo "---------------------"
	@gcov test_matrixProgram-matrixProgram.cpp | grep -A 1 "File 'matrixProgram.hpp'"
	@echo "---------------------"
	@rm ./test_matrixProgram
	@rm *.gcov *.gcda *.gcno
	@rm -rf *.dSYM

main:
	$(CC) -L. -lmatrixOperations -O2 $(STD) $(W) main.cpp -o matrixProgram
	$(TARGET)

clean:
	@rm -rf ./matrixProgram
