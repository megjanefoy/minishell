#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Title: Lab 5
 * Semester: COP4338 - Spring 2021
 * Author Megan Jane Thompson
 * Panther ID: 3276301
 *
 * I affirm that the revisions of this program is entirely
 * my own work and none of it is the work of any other person.
 *
 * This program creates two children. One child processes ls -al
 * and pipes that output as input to another child that processes
 * grep minishell.c
 */

/********MAIN********/
int main(){
	pid_t pidC1;
	pid_t pidC2;
	pid_t ppid = getpid();              		//parent pid
//	int waitStat;
	int pipefd[2];

	pipe(pipefd);								//pipe between children

	pidC1 = fork();                 			//first child created (output to pipe)
	if(pidC1 < 0){                 				//an error occurred
		perror("fork failed");
		exit(-1);
	}else if(pidC1 == 0){         				//first child process
		close(pipefd[0]);						//close read end
		dup2(pipefd[1], 1);						//make output go to pipe										//first child process
		execlp("ls", "ls", "-al", NULL);		//execute ls -al
		/* return only when exec fails */
		perror("exec failed");
		exit(-1);
	}
	else{
		pidC2 = fork();   						//second child created
		if(pidC2 < 0) {                 		//an error occurred
			perror("fork failed");
			exit(-1);
		}else if(pidC2 == 0){           		//second child process (input from pipe)
			close(pipefd[1]);					//close write end
			dup2(pipefd[0], 0);					//get input from pipe
			execlp("grep", "grep", "minishell.c", NULL);//grep minishell.c
			/* return only when exec fails */
			perror("exec failed");
			exit(-1);
		}
	}

	if (ppid == getpid()){						//only parent prints pidC1 & pidC2
		sleep(1);									//waits for both children to finish
		printf("Child 1 PID# %d\n", pidC1);
		printf("Child 2 PID# %d\n", pidC2);
	}

	close(pipefd[0]);
	close(pipefd[1]);

	return 0;
}
