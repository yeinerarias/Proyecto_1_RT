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


////////////////////////////

long double acc_h1,acc_h2;
int r = -1;

int end_h1,end_h2,end_h3,end_h4,end_h5;

int porcentajehilo1 = 1, porcentajehilo2 = 2, porcentajehilo3 = 4, porcentajehilo4 = 6, porcentajehilo5 = 11;
int n_1=10000, n_2=35000, n_3, n_4, n_5=9000; 
int quantum = 0;
int porcentajehilo1actual = 0, porcentajehilo2actual = 0, porcentajehilo3actual = 0, porcentajehilo4actual = 0, porcentajehilo5actual = 0;


////////////////////////////
#define SECOND 800000
#define STACK_SIZE 40096

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];


sigjmp_buf env_struct[2];

////////////////////////////////////////////////////////////////////////

void scheduler();

////////////////////////////////////////////////////////////////////////

typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7

//A translation required when using an address of a variable
//Use this as a black box in your code.
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
			porcentajehilo1actual = i*57/n_1;
			acc=acc/(2*i+1);
			acc_h1=acc_h1+acc;
		}
		end_h1=1;  
	}    
	printf("Termine Hilo 1 \n"); 
}

void hilo2( void )
{
    static int  i=0;
	static float j=0;
	static long double acc=1.0;
	acc_h2=0.0;

	if (end_h2==0){
		for(i=1;i<n_2+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=j*j*j/(j*j*4*(j-i));
			}
			porcentajehilo2actual = i*57/n_1;
			acc=acc/(2*i+1);
			acc_h2=acc_h2+acc;
		}
		end_h2=1;  
	}   
	printf("Termine Hilo 2 \n"); 
}
 
void exxit(){scheduler();}
void crear_hilo()
{
    address_t sp, pc;
    
    sp = (address_t)stack1 + STACK_SIZE;
    //sp = malloc(STACK_SIZE) + STACK_SIZE;
    pc = (address_t)hilo1; 
    
    sigsetjmp(env_struct[0],1);
    (env_struct[0]->__jmpbuf)[JB_SP] = translate_address(sp);
    (env_struct[0]->__jmpbuf)[JB_PC] = translate_address(pc);
    //(env_struct[0]->__jmpbuf)[8] = translate_address(pc);
    //(env_struct[0]->__jmpbuf)[JB_PC] = translate_address(pc);
    //sigemptyset(&env_struct[0]->__saved_mask);     

    sp = (address_t)stack2 + STACK_SIZE ;
    //sp = malloc(STACK_SIZE) + STACK_SIZE;
    pc = (address_t)hilo2;
 
    sigsetjmp(env_struct[1],1);
    (env_struct[1]->__jmpbuf)[JB_SP] = translate_address(sp);
    (env_struct[1]->__jmpbuf)[JB_PC] = translate_address(pc);
    //sigemptyset(&env_struct[1]->__saved_mask);         
}


void scheduler(){ 
    
    printf("acc_h1 =  %Le acc_h2 =  %Le \n",(1+acc_h1)*2, (1+acc_h2)*2);
    if(r != -1)
    {
		if(end_h1 || end_h2){
			printf("Alguien Murio \n");
		}
		if (sigsetjmp(env_struct[r],1) == 1) {
			return;
		}
		
	}
    r = 0;//rand() % 2;
	siglongjmp(env_struct[r],1);
    
	/*if(sigsetjmp(env_struct[r],1)==0){
		printf("no vengo de un salto, pi1 es %Le \n",(1+acc_h1)*2.0);
		printf("no vengo de un salto, pi2 es %Le \n",(1+acc_h2)*2.0);
		r=rand() % 2;
		siglongjmp(env_struct[r],1);	   
	}
	else{
	r=rand() % 2;
	printf(" va el hilo %d \n",1+ r);
	siglongjmp(env_struct[r],1);}*/
}


void setuptimer()
{
	srand(time(NULL));

	struct sigaction sa;
	struct itimerval timer;

	memset (&sa, 0, sizeof (sa));
	sa.sa_handler = &scheduler;
	sigaction (SIGALRM, &sa, NULL);

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 100000;
	// ... and every 250 msec after that.
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 100000;
	// Start a real timer. It counts down whenever this process is executing. 
	setitimer (ITIMER_REAL, &timer, NULL);
}

int main()
{	
	srand(time(NULL));
    crear_hilo();	
    setuptimer();	
    scheduler();
    while(1);
    return 0;
}






