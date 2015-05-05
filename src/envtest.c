#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
  
int main(int argc, char* argv[], char* envp[])
{
	printf("HOME : %s\n", getenv("HOME"));
	printf("PS : %s\n", getenv("PS"));
	printf("foo : %s\n", getenv("foo"));
	
	return 0;
}
