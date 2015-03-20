myshell:myshell.c
	gcc -g myshell.c -o myshell
	
myshell_rl:myshell_rl.c
	gcc -g myshell_rl.c -o myshell_rl -lreadline

final_myshell:myshell_rl_pr.c
	gcc -g myshell_rl_pr.c -o final_myshell -lreadline

backup_myshell:myshell_rl_pr2.c
	gcc -g myshell_rl_pr2.c -o backup_myshell -lreadline

pro_myshell:myshell_rl_pr_con.c
	gcc -g myshell_rl_pr_con.c -o pro_myshell -lreadline
	
ls:ls.c
	gcc -g ls.c -o ls

pwd:pwd.c
	gcc -g pwd.c -o pwd

echo:echo.c
	gcc -g echo.c -o echo
	
envtest:envtest.c
	gcc -g envtest.c -o envtest
