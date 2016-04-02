#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


int main()
{
	int fd = open("/home/rounak/Dropbox/Current Sem/Operating Systems/Lab/MyShell/echoes.txt", O_RDONLY);
	printf("fd=%d\n", fd);
	
	pid_t cpid = fork();
	if (cpid == 0){
		dup2(fd, STDIN_FILENO);
		close(fd);
		execve("/bin/cat", (char *[]){"/bin/cat",  NULL}, (char *[]){NULL});
		perror("fuck");
	} else {
		waitpid(-1, NULL, 0);
	}
	return 0;
}
