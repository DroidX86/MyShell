myshell:myshell.c
	gcc -g myshell.c -o myshell
	
myshell_rl:myshell_rl.c
	gcc -g myshell_rl.c -o myshell_rl -lreadline

myshell_rl_pr:myshell_rl_pr.c
	gcc -g myshell_rl_pr.c -o myshell_rl_pr -lreadline
	
ls:ls.c
	gcc -g ls.c -o ls

pwd:pwd.c
	gcc -g pwd.c -o pwd

echo:echo.c
	gcc -g echo.c -o echo
	
envtest:envtest.c
	gcc -g envtest.c -o envtest
