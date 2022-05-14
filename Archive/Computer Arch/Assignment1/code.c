#include <stdio.h>
#define BUF_SIZE 13

int foo(){
	int i;
	int B[BUF_SIZE];
	for(i = 0; i < BUF_SIZE; i++)
		B[i] = 5;
	return i;
}

int main(){

	foo();
	return 0;
}

