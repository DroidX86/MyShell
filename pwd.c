#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	char cwd[512];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		printf("Could not get CWD\n");
	return 0;
}
