////////////////////////////

float acc_h1,acc_h2,acc_h3,acc_h4,acc_h5;
int r = -1;
int count_deads = 0;

int end_h1,end_h2,end_h3,end_h4,end_h5;
pid_t pid1;
int load_per1 = 10, load_per2 = 20, load_per3 = 11, load_per4 = 10, load_per5 = 21;
int n_1=30000, n_2=10000, n_3=10000, n_4=10000, n_5=10000; 
int quantum = 100;
int mode = 1;
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

#ifdef __x86_64__
// code for 64 bit Intel arch

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

#else
// code for 32 bit Intel arch
#define JB_SP 4
#define JB_PC 5
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%gs:0x18,%0\n"
                "rol    $0x9,%0\n"
                 : "=g" (ret)
                 : "0" (addr));
    return ret;
}

#endif


////////////////////////////////////////////////////////////////////////

int Lottery_Scheduler(int tickets[5])
{
	int winner;
	int all_tickets = 0;
	int i;
	for(i=0;i<5;i++)
	{
		all_tickets += tickets[i];
	}
	winner = rand() % all_tickets;
	all_tickets = 0;
	for(i=0;i<5;i++)
	{
		all_tickets += tickets[i];
		if(all_tickets > winner)
		{
			return i;
		}
	}
	return 0;
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


void config()
{
	tickets[0] = 8000;
    tickets[1] = 300;
    tickets[2] = 10;
    tickets[3] = 300;
    tickets[4] = 2000;
    
    FILE *ifp;
	char *mode = "a+";
	char username[9];  /* One extra for nul char. */
	int score;


	ifp = fopen("config.txt", mode);

	if (ifp == NULL) {
	  fprintf(stderr, "Can't open input file in.list!\n");
	  exit(1);
	}
	else {
		while (fscanf(ifp, "%s %d\n", username, &score) != EOF) {
			fprintf(ifp, "%s %d\n", username, score+10);
		}
	}
/*
	ofp = fopen(outputFilename, "w");

	if (ofp == NULL) {
	  fprintf(stderr, "Can't open output file %s!\n",
			  outputFilename);
	  exit(1);
	}*/
}

