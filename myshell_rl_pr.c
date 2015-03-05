#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAX_COMMAND_LEN 1024
#define MAX_PROGS 100
#define MAX_ARGS 200
#define MAX_TOKENS 200
#define MAX_PROG_NAME 64
#define MAX_PROG_LOC 512
#define MAX_ENV_VARS 100
#define ENV_VAR_NMSIZE 256
#define ENV_VAR_SIZE 1024

#define DUMP_CLEAR 42
#define PIPE 36
#define DUMP_APPEND 49
#define TAKE 69

//global variables
char command_line[MAX_COMMAND_LEN];
char* command_tokens[MAX_TOKENS];
int num_tokens;

char* programs[MAX_PROGS];
char* prog_locs[MAX_PROGS];
int num_progs;

char* c_argv[MAX_ARGS];
char* c_envp[MAX_ENV_VARS];

char first_char = '\0';


//environment variables
char* env_variables[MAX_ENV_VARS];
char* env_var_values[MAX_ENV_VARS];
int env_var_count;
char* prompt_string;	//buffer for regular use in prompt subroutine

/** Update the prompt string if set has been used **/
void update_prompt_string()
{
	int i=0;
	for(; i<env_var_count; i++){
		if (strcmp(env_variables[i], "PS") == 0){
			strcpy(prompt_string, env_var_values[i]);
			return;
		}
	}
	perror("PS not found in list of environment variables");
}

/** Trailing whitespace removal **/
void rstrip(char* str)
{
	int len = strlen(str)-1;
	while (str[len] == '\n' || str[len] == '\t' || str[len] == ' ')
		len--;
	str[len+1] = 0;
}

//----------------------------------------------------------------------------------
/** Built-in commands **/
void change_directory(char* wheretogo)
{	
	int i=0;
	if (!wheretogo) {
		for(; i<env_var_count; i++){
			if( env_variables[i] ) {
				if (strcmp(env_variables[i], "HOME") == 0)
					chdir(env_var_values[i]);
			}
		}
	} else {
		int e = chdir(wheretogo);
		if (e != 0)
			perror("chdir() error");

	}
	for (i=0; i<env_var_count; i++) {
		if (strcmp(env_variables[i], "PWD") == 0)
			getcwd(env_var_values[i], ENV_VAR_SIZE);
	}
}

void print_environment()
{
	int i;
	for(i=0; i<env_var_count; i++){
		if ( env_variables[i] && (strlen(env_variables[i]) != 0) ) {
			printf("%s = %s\n", env_variables[i], env_var_values[i]);
		}
	}
}

void set_env_variable(char* key, char* value)
{
	int i;
	for ( i=0; i<env_var_count; i++ ) {
		//printf("ev: %s\n", env_variables[i]);
		if(env_variables[i]) {
			//printf("ev: %s\n", env_variables[i]);
			if ( strcmp(key, env_variables[i]) == 0) {
				free(env_var_values[i]);
				env_var_values[i] = (char*)malloc(ENV_VAR_SIZE);
				strcpy(env_var_values[i], value);
				update_prompt_string();
				return;
			}
		}
	}
	if (env_var_count != MAX_ENV_VARS) {
		i=0;
		while ( (env_variables[i] != NULL) && ( strlen(env_variables[i]) != 0 ) ) i++;
		env_variables[i] = (char*)malloc(ENV_VAR_NMSIZE);
		env_var_values[i] = (char*)malloc(ENV_VAR_SIZE);
		strcpy(env_variables[i], key);
		strcpy(env_var_values[i], value);
		env_var_count++;

	} else 
		printf("Can not set any more environment variables. Sorry.");
}

void unset_env_variable(char* key)
{
	int i;
	for ( i=0; i<env_var_count; i++ ) {
		if ( env_variables[i] ) {
			if ( strcmp(key, env_variables[i]) == 0) {
				free(env_variables[i]);
				free(env_var_values[i]);
				env_variables[i] = malloc(1);
				env_variables[i][0] = 0;
				env_var_values[i] = NULL;
				
				if(strcmp(key, "PS")==0)
					set_env_variable("PS", "");	//do not unset PS
				return;
			}

		}
	}
	printf("Could not find specified variable.");
}

void install(char* progname, char* progloc)
{
	programs[num_progs] = (char*)malloc(MAX_PROG_NAME);
	prog_locs[num_progs] = (char*)malloc(MAX_PROG_LOC);
	strcpy(programs[num_progs], progname);
	strcpy(prog_locs[num_progs], progloc);
	num_progs++;
	FILE* progfile = fopen("locs", "a");
	fprintf(progfile, "%s:%s\n", progname, progloc);
	fclose(progfile);
}

void show_help()
{
	printf("MyShell: A **very** limited shell\n\tCan only run single commands at a time. No piping or any of that fancy stuff.\n\n");
	printf("Install your program by using the install command.\n\tinstall program_name program_location.\n");
	printf("Set your environment variables using the set command.\n\tset variable value.\n");
	printf("Unset your environment variables using the unset command.\n\tunset variable.\n\n");
	printf("***I take no responsibilty if the lack of features of this shell frustrates you into punching your monitor.***\n\t\t\t\tGood luck!\n");
}


//------------------------------------------------------------------------------------
/** Set the default environment variables and their default values **/
void set_default_env_vars()
{
	env_variables[0] = (char*)malloc(ENV_VAR_NMSIZE);
	strcpy(env_variables[0], "PS");
	env_var_values[0] = (char*)malloc(ENV_VAR_SIZE);
	strcpy(env_var_values[0], "myshell>");
	prompt_string = env_var_values[0];
	
	env_variables[1] = (char*)malloc(ENV_VAR_NMSIZE);
	strcpy(env_variables[1], "USER");
	env_var_values[1] = (char*)malloc(ENV_VAR_SIZE);
	getlogin_r(env_var_values[1], ENV_VAR_SIZE);
	
	env_variables[2] = (char*)malloc(ENV_VAR_NMSIZE);
	strcpy(env_variables[2], "PWD");
	env_var_values[2] = (char*)malloc(ENV_VAR_SIZE);
	getcwd(env_var_values[2], ENV_VAR_SIZE);
	
	env_variables[3] = (char*)malloc(ENV_VAR_NMSIZE);
	strcpy(env_variables[3], "TERM");
	env_var_values[3] = (char*)malloc(ENV_VAR_SIZE);
	strcpy(env_var_values[3], "xterm");
	
	env_variables[4] = (char*)malloc(ENV_VAR_NMSIZE);
	strcpy(env_variables[4], "HOME");
	env_var_values[4] = (char*)malloc(ENV_VAR_SIZE);
	strcpy(env_var_values[4], getenv("HOME"));
	
	env_var_count = 5;
}

/** Initialize the environment variables and build installed program list **/
void init()
{
	set_default_env_vars();	
	
	num_progs = 0;
	int i;
	for(i=0; i<10; i++){
		programs[i] = (char*)malloc(MAX_PROG_NAME);
		prog_locs[i] = (char*)malloc(MAX_PROG_LOC);
	}
	
	FILE* progfile = fopen("locs", "r");
	char buf[256];
	while (fgets(buf, sizeof(buf), progfile)) {
		char* split_line = strtok(buf, ":");
		strcpy(programs[num_progs], split_line);
		split_line = strtok(NULL, ":");
		rstrip(split_line);
		strcpy(prog_locs[num_progs], split_line);
		num_progs++;
	}
	fclose(progfile);
}

/** Show Prompt **/
void prompt()
{
	printf("%s", prompt_string);
}

/** Handle signal **/
void handle_signal(int signo)
{
	prompt();
	fflush(stdout);
}

/** Clear command string and tokens **/
void clear_command()
{
	command_line[0] = 0;
	//free(command_line);
	int i;
	for(i=0; i<num_tokens; i++){
		//free(command_tokens[i]);
		command_tokens[i] = NULL;
	}
	num_tokens = 0;
}

/** Tokenize the line read **/
void tokenize_command()
{
	char* tok_buffer = strtok(command_line, " ");
	while(tok_buffer != NULL){
		command_tokens[num_tokens] = tok_buffer;
		num_tokens++;
		tok_buffer = strtok(NULL, " ");
	}
}

/** Read a line of command (NOT USED)**/
void get_line()
{
	clear_command();
	int chars = 1;
	command_line[0] = first_char;
	char input_char = getchar();
	while ( (input_char != '\n') && (chars < MAX_COMMAND_LEN) ){
		command_line[chars] = input_char;
		chars++;
		input_char = getchar();
	}
	command_line[chars] = 0;
	tokenize_command();
}

/** Search command_name against list of installed programs in locs **/
int is_installed(int index)
{
	int i;
	for(i=0; i<num_progs; i++){
		if ( strcmp(programs[i], command_tokens[index]) == 0 ){
			return i;
		}
	}
	return -1;
}

/** Run the installed executable with the rest of the tokens passed as arguments **/
void execute_single_command()
{
	int loc_index = is_installed(0), cstat;
	if ( loc_index != -1 ) {
		char* com = prog_locs[loc_index];
		int i=1, t, j;
		
		//set arguments
		for(; i<num_tokens; i++){
			c_argv[i] = command_tokens[i];
		}
		c_argv[0] = com;
		c_argv[i] = NULL;
		
		//set environment
		for ( i=0, j=0; i<env_var_count; i++ ) {
			if ( env_variables[i] ) {
				c_envp[j] = (char*)malloc(1024);
				sprintf(c_envp[j], "%s=%s", env_variables[i], env_var_values[i]);
				j++;
			}
		}
		c_envp[j] = NULL;
		
		/*printf("com: %s\n", com);
		for(t=0; t<MAX_ARGS; t++){
			printf("c_argv: %s\tc_envp: %s\n", c_argv[t], c_envp[t]);
		}*/
		
		pid_t pid = fork();
		if ( pid == 0 ) {
			execve(com, c_argv, c_envp);
			perror("execve failed");
		} else if ( pid > 0 ) {
			pid_t cpid;
			if ( (cpid = wait(&cstat)) == -1 ) {
				perror("wait() error");
			}
		} else {
			perror("fork() failed");
		}
	}
	else
		printf("Command not found!\n");
}

/** check if a command token is a pipe '|' or arrow '>', '<', '>>' etc. **/
//XXX: this stuff can be optimized
int is_pipe_or_arrow(char* token)
{
	if (!token){
		printf("NULL passed");
		exit(EXIT_FAILURE);
	}
	if (strcmp(token, "|") == 0)
		return PIPE;
	else if (strcmp(token, ">") == 0)
		return DUMP_CLEAR;
	else if (strcmp(token, "<") == 0)
		return TAKE;
	else if (strcmp(token, ">>") == 0)
		return DUMP_APPEND;
	else
		return 0;
}

/** Execute a command string with piping and redirection **/
void execute_command() 
{
	//set environment
	int i, j;
	for ( i=0, j=0; i<env_var_count; i++ ) {
		if ( env_variables[i] ) {
			c_envp[j] = (char*)malloc(1024);
			sprintf(c_envp[j], "%s=%s", env_variables[i], env_var_values[i]);
			j++;
		}
	}
	c_envp[j] = NULL;
	int dump;	//what did the last command do? pipe or redirect?
	
	for (i=0; i<num_tokens; ) {
		j = i;
		printf("Start: i = %d, j = %d\n", i, j);
		//printf("checking...%s\n", command_tokens[j]);
		int decide = is_pipe_or_arrow(command_tokens[j]);
		while (1) {
			//printf("j in loop = %d\n", j);
			if ((decide != 0) || j == num_tokens-1 ) //break if delimiter found or this is the last token
				break;
			++j;
			decide = is_pipe_or_arrow(command_tokens[j]);
			//printf("j in loop after check= %d\n", j);
		}
		int k;
		//printf("updated j = %d\n", j);
		/*if (j == num_tokens)
			--j;*/
		printf("DO HERE: i = %d, j = %d\n", i, j);	//business end: [i, j) is the part which needs to be forked; j decides what to do about i/o;
								//if i==j then it's use last j to decide, pipe implies fork, other implies dump 
		for (k=i; k<=j; k++)
				printf("%s, ", command_tokens[k]);
		printf("\n");
		/* main part */
		
		
		
		/* end of main part */
		i = j+1;
		printf("End: i = %d, j = %d\n\n", i, j);
	}
}

/** Search command_name against list of built ins **/
int is_builtin()
{
	if (strcmp("exit", command_tokens[0]) == 0) {
		exit(EXIT_SUCCESS);
	} else if (strcmp("help", command_tokens[0]) == 0) {
		show_help();
		return 1;
	} else if (strcmp("cd", command_tokens[0]) == 0) {
		if (num_tokens < 2) {
			change_directory(NULL);
		} else  if (num_tokens < 3) {
			change_directory(command_tokens[1]);
		} else {
			printf("Invalid number of arguments passed to built-in cd");
		}
		return 1;
	} else if (strcmp("env", command_tokens[0]) == 0) {
		print_environment();
		return 1;
	} else if (strcmp("set", command_tokens[0]) == 0) {
		if (num_tokens == 3){
			set_env_variable(command_tokens[1], command_tokens[2]);
		} else {
			printf("Invalid number of arguments passed to built-in set\n");
		}
		return 1;
	} else if (strcmp("unset", command_tokens[0]) == 0) {
		if (num_tokens == 2){
			unset_env_variable(command_tokens[1]);
		} else {
			printf("Invalid number of arguments passed to built-in unset\n");
		}
		return 1;
	} else if (strcmp("install", command_tokens[0]) == 0) {
		if (num_tokens == 3){
			install(command_tokens[1], command_tokens[2]);
		} else {
			printf("Invalid number of arguments passed to built-in install\n");
		}
		return 1;
	}
	return 0;
}


//-----------------------------------------------------------------------------------
/** Main **/
int main(void)
{
	init();
	/*
	while (1) {		
		signal(SIGINT, handle_signal);
		prompt();
		strcpy(command_line, readline(""));
		
		if (strcmp(command_line, "")==0)
			continue;
		
		if (*command_line && command_line)
			add_history(command_line);
			
		tokenize_command();
		if ( !is_builtin() ) {
			execute_command();
		}
		clear_command();
	}
	*/
	strcpy(command_line, "cat file");
	
	tokenize_command();
	
	int i;
	printf("Tokens array: \n");
	for (i=0; i< num_tokens; i++) {
		printf("%d = %s\n", i, command_tokens[i]);
	}
	printf("End of tokens array\n\n");
	
	execute_command();	
	
	printf("\n");	
	return 0;
}
