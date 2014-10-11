////////////////////////////

float acc_h1,acc_h2,acc_h3,acc_h4,acc_h5;
int r = -1;
int count_deads = 0;

int end_h1,end_h2,end_h3,end_h4,end_h5;
pid_t pid1;
int load_per[5];
int n[5]; 
int quantum;
int mode;
int act_load_per[5];
int tickets[5];

#define C_R "\x1b[31m"
#define C_Y "\x1b[33m"
#define C_W "\x1b[37m"
#define C_G "\x1b[32m"

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
		for(i=1;i<n[0]+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=pow(j,3)/(pow(j,2)*4*(j-i));
			}
			act_load_per[0] = i*100/n[0];
			acc=acc/(2*i+1);
			acc_h1=acc_h1+acc;
			if (((i % (int)(n[0]*load_per[0]/(100)) == 0) || (n[0] == i)) && (!mode)){
				if(i == n[0]){end_h1 = 1;}
				raise(SIGUSR1);
            }
		}
	}  
	end_h1 = 1;  
	while(1);
}

void hilo2( void )
{
    int  i=0;
	float j=0;
	long double acc=1.0;
	acc_h2=0.0;

	if (end_h2==0){
		for(i=1;i<n[1]+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=pow(j,3)/(pow(j,2)*4*(j-i));
			}
			act_load_per[1] = i*100/n[1];
			acc=acc/(2*i+1);
			acc_h2=acc_h2+acc;
			if (((i % (int)(n[1]*load_per[1]/(100)) == 0) || (n[1] == i)) && (!mode)){
				if(i == n[1]){end_h2 = 1;}
				raise(SIGUSR1);
            }
		}
	}   
	end_h2 = 1; 
	while(1);
}

void hilo3( void )
{
    int  i=0;
	float j=0;
	long double acc=1.0;
	acc_h3=0.0;

	if (end_h3==0){
		for(i=1;i<n[2]+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=pow(j,3)/(pow(j,2)*4*(j-i));
			}
			act_load_per[2] = i*100/n[2];
			acc=acc/(2*i+1);
			acc_h3=acc_h3+acc;
			if (((i % (int)(n[2]*load_per[2]/(100)) == 0) || (n[2] == i)) && (!mode)){
				if(i == n[2]){end_h3 = 1;}
				raise(SIGUSR1);
            }
		} 
	} 
	end_h3 = 1;  
	while(1);
}

void hilo4( void )
{
    int  i=0;
	float j=0;
	long double acc=1.0;
	acc_h4=0.0;

	if (end_h4==0){
		for(i=1;i<n[3]+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=pow(j,3)/(pow(j,2)*4*(j-i));
			}
			act_load_per[3] = i*100/n[3];
			acc=acc/(2*i+1);
			acc_h4=acc_h4+acc;
			if (((i % (int)(n[3]*load_per[3]/(100)) == 0) || (n[3] == i)) && (!mode)){
				if(i == n[3]){end_h4 = 1;}
				raise(SIGUSR1);
            }
		}
	}   
	end_h4 = 1;
	while(1);
}

void hilo5( void )
{
    int  i=0;
	float j=0;
	long double acc=1.0;
	acc_h5=0.0;

	if (end_h5==0){
		for(i=1;i<n[4]+1;i++){
			acc=1.0;
			for (j=2*i;j>i;j--){
				acc*=pow(j,3)/(pow(j,2)*4*(j-i));
			}
			act_load_per[4] = i*100/n[4];
			acc=acc/(2*i+1);
			acc_h5=acc_h5+acc;
			if (((i % (int)(n[4]*load_per[4]/(100)) == 0) || (n[4] == i)) && (!mode)){
				if(i == n[4]){end_h5 = 1;}
				raise(SIGUSR1);
            }
		}
	}   
	end_h5 = 1;
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
    FILE *ifp;
	char username[2];
	int data;
	int i = 0;
	int selec;
	printf("¿Leer de archivo 1, Insertar valores 0?\n");
	scanf("%d",&selec);
	if(selec) {
		ifp = fopen("config.txt", "r");

		if (ifp == NULL) {
		  fprintf(stderr, "Can't open input file in.list!\n");
		  exit(1);
		}
		else {
			while (fscanf(ifp,"%s %d\n",username,&data) != EOF) {
				if(i == 0){mode = data;}
				else if(i < 6) {tickets[i-1] = data;}
				else if(i < 11){n[i-6] = data*50;}
				else if(i == 11){quantum = data;}
				else {load_per[i-12] = data;}
				i++;
			}
		}
	}
	else{
		ifp = fopen("config.txt", "w");
		printf("¿Modo de trabajo 1 (expropiativo) 0 (no expropiativo)?\n");
		scanf("%d",&mode);
		fprintf(ifp,"modo %d\n",mode);
		printf("¿Numero de tickets hilo 1?\n");
		scanf("%d",&tickets[0]);
		fprintf(ifp,"tickets_hilo_1 %d\n",tickets[0]);
		printf("¿Numero de tickets hilo 2?\n");
		scanf("%d",&tickets[1]);
		fprintf(ifp,"tickets_hilo_2 %d\n",tickets[1]);
		printf("¿Numero de tickets hilo 3?\n");
		scanf("%d",&tickets[2]);
		fprintf(ifp,"tickets_hilo_3 %d\n",tickets[2]);
		printf("¿Numero de tickets hilo 4?\n");
		scanf("%d",&tickets[3]);
		fprintf(ifp,"tickets_hilo_4 %d\n",tickets[3]);
		printf("¿Numero de tickets hilo 5?\n");
		scanf("%d",&tickets[4]);
		fprintf(ifp,"tickets_hilo_5 %d\n",tickets[4]);
		printf("¿Unidades de trabajo hilo 1?\n");
		scanf("%d",&n[0]);
		fprintf(ifp,"unidades_trabajo1 %d\n",n[0]);
		n[0] *= 50;
		printf("¿Unidades de trabajo hilo 2?\n");
		scanf("%d",&n[1]);
		fprintf(ifp,"unidades_trabajo2 %d\n",n[1]);
		n[1] *= 50;
		printf("¿Unidades de trabajo hilo 3?\n");
		scanf("%d",&n[2]);
		fprintf(ifp,"unidades_trabajo3 %d\n",n[2]);
		n[2] *= 50;
		printf("¿Unidades de trabajo hilo 4?\n");
		scanf("%d",&n[3]);
		fprintf(ifp,"unidades_trabajo4 %d\n",n[3]);
		n[3] *= 50;
		printf("¿Unidades de trabajo hilo 5?\n");
		scanf("%d",&n[4]);
		fprintf(ifp,"unidades_trabajo5 %d\n",n[4]);
		n[4] *= 50;
		if(mode)
		{
			printf("¿Quantum en millisegundos?\n");
			scanf("%d",&quantum);
			fprintf(ifp,"quantum %d\n",quantum);
			fprintf(ifp,"porcentaje_hilo1 10\n");
			fprintf(ifp,"porcentaje_hilo2 10\n");
			fprintf(ifp,"porcentaje_hilo3 10\n");
			fprintf(ifp,"porcentaje_hilo4 10\n");
			fprintf(ifp,"porcentaje_hilo5 10\n");
			load_per[0] = 1; load_per[1] = 1; load_per[2] = 1;
			load_per[3] = 1; load_per[4] = 1;
		}
		else
		{
			fprintf(ifp,"quantum 100\n");
			printf("Porcentaje de trabajo hilo 1?\n");
			scanf("%d",&load_per[0]);
			fprintf(ifp,"porcentaje_hilo1 %d\n",load_per[0]);
			printf("Porcentaje de trabajo hilo 2?\n");
			scanf("%d",&load_per[1]);
			fprintf(ifp,"porcentaje_hilo2 %d\n",load_per[1]);
			printf("Porcentaje de trabajo hilo 3?\n");
			scanf("%d",&load_per[2]);
			fprintf(ifp,"porcentaje_hilo3 %d\n",load_per[2]);
			printf("Porcentaje de trabajo hilo 4?\n");
			scanf("%d",&load_per[3]);
			fprintf(ifp,"porcentaje_hilo4 %d\n",load_per[3]);
			printf("Porcentaje de trabajo hilo 5?\n");
			scanf("%d",&load_per[4]);
			fprintf(ifp,"porcentaje_hilo5 %d\n",load_per[4]);
		}
	}
	fclose(ifp);
}

void draw()
{
	int i;
	system("clear");
    printf(C_W"Act  Hilo    Aprox                          Trabajo terminado\n");
    printf("--- ------- --------  ----------------------------------------------------------\n");
    if(r == 0) {printf("->  Hilo 1: %f  ",(1+acc_h1)*2.0);}
	else {printf("    Hilo 1: %f  ",(1+acc_h1)*2.0);}
	for(i = 0; i < act_load_per[0]*54/100;i++)
	{if(i<17){printf(C_R "|");}else if(i<35){printf(C_Y "|");}else{printf(C_G "|");}}
	printf(C_W"%d%c\n",act_load_per[0],37);
	if(r == 1) {printf("->  Hilo 2: %f  ",(1+acc_h2)*2.0);}
	else {printf("    Hilo 2: %f  ",(1+acc_h2)*2.0);}
	for(i = 0; i < act_load_per[1]*54/100;i++)
	{if(i<17){printf(C_R "|");}else if(i<35){printf(C_Y "|");}else{printf(C_G "|");}}
	printf(C_W"%d%c\n",act_load_per[1],37);
	if(r == 2) {printf("->  Hilo 3: %f  ",(1+acc_h3)*2.0);}
	else {printf("    Hilo 3: %f  ",(1+acc_h3)*2.0);}
	for(i = 0; i < act_load_per[2]*54/100;i++)
	{if(i<17){printf(C_R "|");}else if(i<35){printf(C_Y "|");}else{printf(C_G "|");}}
	printf(C_W"%d%c\n",act_load_per[2],37);
	if(r == 3) {printf("->  Hilo 4: %f  ",(1+acc_h4)*2.0);}
	else {printf("    Hilo 4: %f  ",(1+acc_h4)*2.0);}
	for(i = 0; i < act_load_per[3]*54/100;i++)
	{if(i<17){printf(C_R "|");}else if(i<35){printf(C_Y "|");}else{printf(C_G "|");}}
	printf(C_W"%d%c\n",act_load_per[3],37);
	if(r == 4) {printf("->  Hilo 5: %f  ",(1+acc_h5)*2.0);}
	else {printf("    Hilo 5: %f  ",(1+acc_h5)*2.0);}
	for(i = 0; i < act_load_per[4]*54/100;i++)
	{if(i<17){printf(C_R "|");}else if(i<35){printf(C_Y "|");}else{printf(C_G "|");}}
	printf(C_W"%d%c\n",act_load_per[4],37);
}

