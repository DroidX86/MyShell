#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

//print info about a single file from stat call
void print_file_info(struct dirent* file, struct stat filestat, int longlist, int all)
{
	if (!all && (file->d_name[0] == '.') ) {
		return;
	}

	char firstchar;
	if (S_ISREG(filestat.st_mode)) {
		firstchar = '-';
	} else if (S_ISDIR(filestat.st_mode)) {
		firstchar = 'd';
	} else if (S_ISLNK(filestat.st_mode)) {
		firstchar = 'l';
	} else {
		firstchar = 'u';
	}

	int mode = filestat.st_mode & 0777;

	int size = filestat.st_size;

	if (longlist) {
		printf("%c\t%o\t%d\t%s\n", firstchar, mode, size, file->d_name);
	} else {
		printf("%s\t", file->d_name);
	}
}

int main(int argc, char* argv[], char* envp[])
{
	if (argv[1] && (strcmp(argv[1], "--help")) == 0) {
		printf("Usage:\n\t-a: Do not ignore hidden entries\n\t-l: use longlisting\n");
		exit(EXIT_SUCCESS);
	}

	//process options
	int all=0, longlist=0, opt;
	while ((opt = getopt(argc, argv, "al")) != -1) {
		switch (opt) {
			case 'a':
				all = 1;
				break;
			case 'l':
				longlist = 1;
				break;
			default:
				printf("Unrecognised option. Please see help.\n");
				exit(EXIT_FAILURE);
		}
	}

	DIR *argdir;

	//no arguments given
	if (optind >= argc) {
		argdir = opendir(getenv("PWD"));
	} else {
		argdir = opendir(argv[1]);
	}

	if (!argdir) {
		printf("Could not open directory stream\n");
		exit(EXIT_FAILURE);
	}

	struct dirent *files;
	while ((files = readdir(argdir)) != NULL){
		struct stat filestat;
		stat(files->d_name, &filestat);
		print_file_info(files, filestat, longlist, all);
	}
	printf("\n");
	return 0;
}
