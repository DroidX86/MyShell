myshell:./Other/myshell.c
	gcc -g ./Other/myshell.c -o ./Other/myshell -Wall
	
myshell_rl:./Other/myshell_rl.c
	gcc -g ./Other/myshell_rl.c -o ./Other/myshell_rl -lreadline -Wall

dead_myshell:./Other/myshell_rl_pr.c
	gcc -g ./Other/myshell_rl_pr.c -o ./Other/dead_myshell -lreadline -Wall

backup_myshell:./Other/myshell_rl_pr2.c
	gcc -g ./Other/myshell_rl_pr2.c -o ./Other/backup_myshell -lreadline -Wall

final_myshell:myshell_final.c
	gcc -g myshell_final.c -o final_myshell -lreadline -Wall
	
ls:ls.c
	gcc -g ls.c -o ls -Wall

pwd:pwd.c
	gcc -g pwd.c -o pwd -Wall

echo:echo.c
	gcc -g echo.c -o echo -Wall
	
envtest:envtest.c
	gcc -g envtest.c -o envtest -Wall
