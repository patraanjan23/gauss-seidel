CC = gcc

gauss-seidal : main.c
	gcc main.c -o gauss-seidel -lm --std=c89