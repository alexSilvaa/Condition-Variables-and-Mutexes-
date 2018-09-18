#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include <string.h> 
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 4
pthread_mutex_t mutex_c, mutex_p; 
pthread_cond_t con_con, con_print; 
static int fastest_thread = -1; 
static int signal = -1;
static int readytoRecieve = -1; 

struct element {
	int id;
	char *string;
};

int element_init(struct element *e)
{	
	e->id = -1; 
	e->string = malloc(100*sizeof(char));
	return 0;  
}

void *Print(void *threadid) {
	//pthread_mutex_lock(&mutex_p);   
	int test = -1; 
	struct element * el = (struct element *)threadid;
	
	while (signal != 1){
		//sleep(0.00001);
		//printf("HERE with thread: %d\n", el->id);
		test = pthread_cond_wait(&con_print, &mutex_c);
		if (signal == 1)
		{	
			pthread_mutex_unlock(&mutex_c);
			break; 
		}
		//printf(":)\n");
    	//printf("HERE NOW\n");
    	//struct element * el = (struct element *)threadid;
    	printf("%d - %s\n", fastest_thread, el->string);
        pthread_mutex_unlock(&mutex_c);
	}

	    
	pthread_exit(NULL); 
} 

void *GetInput(void *input){

	struct element* el = (struct element*)input; 
	int test = -1;  
	while(signal != 1)
	{
		//printf("test before is %d AND i am: %d\n", test, el->id);
        if (readytoRecieve == 1)
        {
            pthread_mutex_lock(&mutex_c);
		    //test = pthread_cond_wait(&con_con,&mutex_c);
		    //printf("test after  is %d and thread is %d\n", test, el->id);
            fastest_thread = el->id;
            pthread_cond_signal(&con_print);
            readytoRecieve = 0; 
            pthread_mutex_unlock(&mutex_c);
        }    

     
        //pthread_t printthread; 
        //int a = pthread_create(&printthread, NULL, Print, (void*)el);
        //pthread_join(printthread, NULL);
        //printf("I AM WAITING\n"); 
	}

	pthread_exit(NULL);  
}

char* readString()
{
    char *input = malloc(50*sizeof(char)); 
    fgets(input,50,stdin); 
    int len = strlen(input) -1;
    if (input[len] = '\n')  { input[len] = '\0'; }
    return input;  
}

int main (int argc, const char * argv[]) { 

	pthread_mutex_init(&mutex_c, NULL); 
	pthread_cond_init(&con_con, NULL);
    pthread_mutex_init(&mutex_p, NULL);
	pthread_cond_init(&con_print, NULL);
	 
	char *input;
	//input = (char*)malloc(50*sizeof(char*)); 
	//printf("Enter text: ");
    //input = readString(); 
	//scanf("%49s", input);
	//fgets(input, 50, stdin);  
	//printf("Test: %s\n", input); 
	//printf("TEST\n");

	struct element el[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++)
	{
		if (element_init(&el[i]))
		{
			printf("ERROR\n");
			return 1; 
		}
		//strcpy(el[i].string, input); 
	} 
	
	pthread_t threads[NUM_THREADS]; 
	int rc,t; 
	for (t=0;t<NUM_THREADS - 1;t++) {
		el[t].id = t;  							//Change as pthread_creates sends the address of
												//el meaning it will always be 2. 
		printf("Creating thread %d\n",t);  
		//if (t == NUM_THREADS - 1) {
			//rc = pthread_create(&threads[t], NULL, Print, (void *) el);  //Printing thread.
		//}
		//else 
		rc = pthread_create(&threads[t],NULL,GetInput,(void*) &el[t]);
		
		if (rc) { 
			printf("ERROR return code from pthread_create(): %d\n",rc); 
			exit(-1); 
		} 
	}
	rc = -1; 
    rc = pthread_create(&threads[t], NULL, Print, (void*)&el[t]); 
    if (rc) {
        printf("ERROR return code from pthread_create(): %d\n",rc); 
	    exit(-1);    
    }


	//for(int i = 0; i < NUM_THREADS; i++)
	//{
	//	printf("Num: %d and Message: %s\n", el[i].id, el[i].string); 
	//}
    //pthread_mutex_lock(&mutex_c);
	//readytoRecieve = 1; 
	//sleep(0.2);
    //pthread_mutex_unlock(&mutex_c);
    //pthread_cond_signal(&con_con); 
    //readytoRecieve = 0; 
/*
    sleep(0.5);
    pthread_cond_signal(&con_con);

    sleep(0.5);
    pthread_cond_signal(&con_con);
*/
    sleep(1);
    printf("Enter text: "); 
	input = readString(); 
    
    while (strcmp(input, "quit") != 0)
    {
        for (int i = 0; i < NUM_THREADS; i++)
        {
            strcpy(el[i].string, input); 
        }
        pthread_mutex_lock(&mutex_c);
        readytoRecieve = 1; 
        //pthread_cond_signal(&con_con);
        //readytoRecieve = 0;
        pthread_mutex_unlock(&mutex_c); 
        sleep(0.5); 
        printf("Enter text: "); 
	    input = readString(); 
    }

    signal = 1;
    pthread_cond_signal(&con_print); 
    for (int i = 0; i < NUM_THREADS - 1; i++)
    {
        pthread_cond_signal(&con_con); 
    } 

	// wait for threads to exit 
	for(t=0;t<NUM_THREADS;t++) { 
		pthread_join( threads[t], NULL); 
	} 
	return 0;
}


//consuming threads
