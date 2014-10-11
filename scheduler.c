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


void scheduler(){ 
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
			if(count_deads == 4){draw(); exit(0);}
			count_deads += 1;
		}
	}
	if (sigsetjmp(env_struct[r],1) == 1) {
		return;
	}
    r = Lottery_Scheduler(tickets);
    draw();
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
    config();	
    if(mode)
    {
		setuptimer();	
    }
    else
    {
		signal(SIGUSR1,&scheduler);
	}
    scheduler();
    return 0;
}
