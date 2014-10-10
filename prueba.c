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


////////////////////////////

float acc_h1,acc_h2,acc_h3,acc_h4,acc_h5;
int r = -1;
int count_deads = 0;

int end_h1,end_h2,end_h3,end_h4,end_h5;
pid_t pid1;
int load_per1 = 10, load_per2 = 20, load_per3 = 11, load_per4 = 10, load_per5 = 11;
int n_1=20000, n_2=10000, n_3=10000, n_4=10000, n_5=10000; 
int quantum = 10;
int mode = 0;
int act_load_per1, act_load_per2, act_load_per3, act_load_per4 = 0, act_load_per5;
int tickets[5];

////////////////////////////////////////////////////////////////////////
#define SECOND 800000
#define STACK_SIZE 40096

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];
char stack3[STACK_SIZE];
char stack4[STACK_SIZE];
char stack5[STACK_SIZE];

////////////////////////////////////////////////////////////////////////

sigjmp_buf env_struct[5];

////////////////////////////////////////////////////////////////////////

typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7

address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
		"rol    $0x11,%0\n"
                 : "=g" (ret)
                 : "0" (addr));
    return ret;
}

////////////////////////////////////////////////////////////////////////
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

void hilo1( void )
{
    int i=0;
	float j=0;
	long double acc=1.0;
	acc_h1=0.0;

	if (end_h1==0){
		for(i=1;i<n_1+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=pow(j,3)/(pow(j,2)*4*(j-i));
			}
			act_load_per1 = i*57/n_1;
			acc=acc/(2*i+1);
			acc_h1=acc_h1+acc;
			if (((i % (int)(n_1*load_per1/(100)) == 0) || (n_1 == i)) && (mode)){
				if(i == n_1){end_h1 = 1;}
				raise(SIGUSR1);
            }
		}
		end_h1=1;  
	}    
	while(1);
}

void hilo2( void )
{
    int  i=0;
	float j=0;
	long double acc=1.0;
	acc_h2=0.0;

	if (end_h2==0){
		for(i=1;i<n_2+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=pow(j,3)/(pow(j,2)*4*(j-i));
			}
			act_load_per2 = i*57/n_2;
			acc=acc/(2*i+1);
			acc_h2=acc_h2+acc;
			if (((i % (int)(n_2*load_per2/(100)) == 0) || (n_2 == i)) && (mode)){
				if(i == n_2){end_h2 = 1;}
				raise(SIGUSR1);
            }
		}
		end_h2=1;  
	}    
	while(1);
}

void hilo3( void )
{
    int  i=0;
	float j=0;
	long double acc=1.0;
	acc_h3=0.0;

	if (end_h3==0){
		for(i=1;i<n_3+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=pow(j,3)/(pow(j,2)*4*(j-i));
			}
			act_load_per3 = i*57/n_3;
			acc=acc/(2*i+1);
			acc_h3=acc_h3+acc;
			if (((i % (int)(n_3*load_per3/(100)) == 0) || (n_3 == i)) && (mode)){
				if(i == n_3){end_h1 = 3;}
				raise(SIGUSR1);
            }
		}
		end_h3=1;  
	}   
	while(1);
}

void hilo4( void )
{
    int  i=0;
	float j=0;
	long double acc=1.0;
	acc_h4=0.0;

	if (end_h4==0){
		for(i=1;i<n_4+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=pow(j,3)/(pow(j,2)*4*(j-i));
			}
			act_load_per4 = i*57/n_4;
			acc=acc/(2*i+1);
			acc_h4=acc_h4+acc;
			if (((i % (int)(n_4*load_per4/(100)) == 0) || (n_4 == i)) && (mode)){
				if(i == n_4){end_h4 = 1;}
				raise(SIGUSR1);
            }
		}
		end_h4=1;  
	}   
	while(1);
}

void hilo5( void )
{
    int  i=0;
	float j=0;
	long double acc=1.0;
	acc_h5=0.0;

	if (end_h5==0){
		for(i=1;i<n_5+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=pow(j,3)/(pow(j,2)*4*(j-i));
			}
			act_load_per5 = i*57/n_5;
			acc=acc/(2*i+1);
			acc_h5=acc_h5+acc;
			if (((i % (int)(n_5*load_per5/(100)) == 0) || (n_5 == i)) && (mode)){
				if(i == n_5){end_h5 = 1;}
				raise(SIGUSR1);
            }
		}
		end_h5=1;  
	}   
	while(1);
}
 
void crear_hilo()
{
    address_t sp, pc;
    
    sp = (address_t)stack1 + STACK_SIZE;
    //sp = malloc(STACK_SIZE) + STACK_SIZE;
    pc = (address_t)hilo1; 
    
    sigsetjmp(env_struct[0],1);
    (env_struct[0]-> __jmpbuf)[JB_SP] = translate_address(sp);
    (env_struct[0]-> __jmpbuf)[JB_PC] = translate_address(pc);
    //(env_struct[0]-> __jmpbuf)[8] = translate_address(pc);
    //(env_struct[0]-> __jmpbuf)[JB_PC] = translate_address(pc);
    //sigemptyset(&env_struct[0]-> __saved_mask);     

    sp = (address_t)stack2 + STACK_SIZE ;
    //sp = malloc(STACK_SIZE) + STACK_SIZE;
    pc = (address_t)hilo2;
 
    sigsetjmp(env_struct[1],1);
    (env_struct[1]-> __jmpbuf)[JB_SP] = translate_address(sp);
    (env_struct[1]-> __jmpbuf)[JB_PC] = translate_address(pc);
    //sigemptyset(&env_struct[1]-> __saved_mask);    
    
    sp = (address_t)stack3 + STACK_SIZE ;
    pc = (address_t)hilo3;
 
    sigsetjmp(env_struct[2],1);
    (env_struct[2]-> __jmpbuf)[JB_SP] = translate_address(sp);
    (env_struct[2]-> __jmpbuf)[JB_PC] = translate_address(pc);   
    
    sp = (address_t)stack4 + STACK_SIZE ;
    pc = (address_t)hilo4;
 
    sigsetjmp(env_struct[3],1);
    (env_struct[3]-> __jmpbuf)[JB_SP] = translate_address(sp);
    (env_struct[3]-> __jmpbuf)[JB_PC] = translate_address(pc);   
    
    sp = (address_t)stack5 + STACK_SIZE ;
    pc = (address_t)hilo5;
 
    sigsetjmp(env_struct[4],1);
    (env_struct[4]-> __jmpbuf)[JB_SP] = translate_address(sp);
    (env_struct[4]-> __jmpbuf)[JB_PC] = translate_address(pc);     
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
    setuptimer();	
    //signal(SIGUSR1,&scheduler);
    tickets[0] = 8000;
    tickets[1] = 300;
    tickets[2] = 10;
    tickets[3] = 300;
    tickets[4] = 2000;
    scheduler();
    return 0;
}






