#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char* argv[], char* envp[] )
{
	if ((strcmp(argv[1], "--help")) == 0) {
		printf("Usage: \n\t-e: enables escape characters (\\a, \\b, \\t, \\n, \\r, \\v)\n\t-n: removes the trailing newline\n");
		exit(EXIT_SUCCESS);
	}

	//get rid of this later
	if ((strcmp(argv[1], "es"))== 0) {
		system("cat echoes.txt");
		exit(EXIT_SUCCESS);
	}

	//process the options
	int escape = 0, newline = 1, opt;
	while ((opt = getopt(argc, argv, "ne")) != -1) {
		switch (opt){
			case 'n':
				newline = 0;
				break;
			case 'e':
				escape = 1;
				break;
			default:
				printf("Unrecognised option. Please see help.\n");
				exit(EXIT_FAILURE);
		}
	}

	//handle case of no arguments
	if (optind >= argc) {
		if (newline) {
			printf("\n");
		}
		exit(EXIT_SUCCESS);
	}

	//handle each argument
	int i;
	for (i=optind; i<argc; i++) {
		char dest[1024];
		if (!escape) {
			strcpy(dest, argv[i]);
		} else {
			int si=0, di=0, eflag=0;
			for (; si<strlen(argv[i]); si++) {
				if (eflag == 0) {
					if (argv[i][si] != '\\')
						dest[di++] = argv[i][si];
					else
						eflag = 1;
				} else if (eflag == 1) {
					switch (argv[i][si]) {
						case 'b': dest[di++] = '\b'; break;
						case 't': dest[di++] = '\t'; break;
						case 'n': dest[di++] = '\n'; break;
						case 'a': dest[di++] = '\a'; break;
						case 'r': dest[di++] = '\r'; break;
						case 'v': dest[di++] = '\v'; break;
						case '\\': dest[di++] = '\\'; break;
						default: dest[di++] = '\\'; dest[di++] = argv[i][si];
					}
					eflag = 0;
				}
			dest[di] = 0;
			}
		}
		printf("%s ", dest);
	}
	 if (newline) {
	 	printf("\n");
	 }

	return 0;
}
