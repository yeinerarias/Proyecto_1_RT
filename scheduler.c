#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>

#include "scheduler.h"


#define ANSI_COLOR_MAGENTA "\x1b[35m"
void scheduler(){ 
	int i;
    system("clear");
    printf("Act  Hilo    Aprox                          Trabajo terminado\n");
    printf("--- ------- --------   ---------------------------------------------------------\n");
    if(r == 0) {printf("->  Hilo 1: %f   ",(1+acc_h1)*2.0);}
	else {printf("    Hilo 1: %f   ",(1+acc_h1)*2.0);}
	for(i = 0; i < act_load_per1;i++)
	{printf("|");}
	printf("\n");
	if(r == 1) {printf("->  Hilo 2: %f   ",(1+acc_h2)*2.0);}
	else {printf("    Hilo 2: %f   ",(1+acc_h2)*2.0);}
	for(i = 0; i < act_load_per2;i++)
	{printf("|");}
	printf("\n");
	if(r == 2) {printf("->  Hilo 3: %f   ",(1+acc_h3)*2.0);}
	else {printf("    Hilo 3: %f   ",(1+acc_h3)*2.0);}
	for(i = 0; i < act_load_per3;i++)
	{printf("|");}
	printf("\n");
	if(r == 3) {printf("->  Hilo 4: %f   ",(1+acc_h4)*2.0);}
	else {printf("    Hilo 4: %f   ",(1+acc_h4)*2.0);}
	for(i = 0; i < act_load_per4;i++)
	{printf("|");}
	printf("\n");
	if(r == 4) {printf("->  Hilo 5: %f   ",(1+acc_h5)*2.0);}
	else {printf("    Hilo 5: %f   ",(1+acc_h5)*2.0);}
	for(i = 0; i < act_load_per5;i++)
	{printf("|");}
	printf("\n");

    if(r != -1)
    {
		if(end_h1 || end_h2 || end_h3 || end_h4 || end_h5){
			switch (r) {
				case 0:
					end_h1 = 0;
				case 1: 
					end_h2 = 0;
				case 2 : 
					end_h3 = 0;
				case 3: 
					end_h4 = 0;
				case 4: 
					end_h5 = 0;
			}
			tickets[r] = 0;
			if(count_deads == 4){exit(0);}
			count_deads += 1;
			//printf("Alguien Murio \n");
		}
	}
	if (sigsetjmp(env_struct[r],1) == 1) {
		return;
	}
		
    r = Lottery_Scheduler(tickets);
	siglongjmp(env_struct[r],1);
    
}

void setuptimer()
{
	struct sigaction sa;
	struct itimerval timer;

	memset (&sa, 0, sizeof (sa));
	sa.sa_handler = &scheduler;
	sigaction (SIGALRM, &sa, NULL);

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = quantum*1000;
	// ... and every 250 msec after that.
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = quantum*1000;
	// Start a real timer. It counts down whenever this process is executing. 
	setitimer (ITIMER_REAL, &timer, NULL);
}


int main()
{	
	srand(time(NULL));
    crear_hilo();	
    if(mode)
    {
		signal(SIGUSR1,&scheduler);
    }
    else
    {
		setuptimer();	
	}
	config();
	sleep(5);
    scheduler();
    return 0;
}
