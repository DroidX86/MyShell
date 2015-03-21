myshell:./Other/myshell.c
	gcc -g ./Other/myshell.c -o ./Other/myshell
	
myshell_rl:./Other/myshell_rl.c
	gcc -g ./Other/myshell_rl.c -o ./Other/myshell_rl -lreadline

dead_myshell:./Other/myshell_rl_pr.c
	gcc -g ./Other/myshell_rl_pr.c -o ./Other/dead_myshell -lreadline

backup_myshell:./Other/myshell_rl_pr2.c
	gcc -g ./Other/myshell_rl_pr2.c -o ./Other/backup_myshell -lreadline

final_myshell:myshell_final.c
	gcc -g myshell_final.c -o final_myshell -lreadline
	
ls:ls.c
	gcc -g ls.c -o ls

pwd:pwd.c
	gcc -g pwd.c -o pwd

echo:echo.c
	gcc -g echo.c -o echo
	
envtest:envtest.c
	gcc -g envtest.c -o envtest
