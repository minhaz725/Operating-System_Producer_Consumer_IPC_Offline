#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstring>
#include <time.h>

using namespace std;

#define number_of_serviceman 1
#define number_of_cycles 1
#define arr_size 3 //service room size 
//test auto stage
//g++ -D_REENTRANT Cycle.cpp -o Cycle -lpthread
//./Cycle
int ind=1;

pthread_mutex_t mutex;
sem_t full_array, empty_array;
char* sharedmemory[arr_size]; 
int value;

void* produce_item(void* arg)
{
	//printf("I'm producer %s\n",(char*)arg);
	//pthread_exit((void*)strcat((char*)arg," is finishing\n"));
	
	while(true)
	{
		sem_getvalue(&empty_array,&value);
		sem_wait(&empty_array); //down

		pthread_mutex_lock(&mutex);
		
		sharedmemory[0]=(char*)arg;
		sleep(1);
		printf("producer %s has produced %d th item\n",(char*)arg,value);
		ind++;
		pthread_mutex_unlock(&mutex);
		sem_post(&full_array); //up
		//if(ind >3 ) ind=1;


	}
}

void* consume_item(void* arg)
{
	//printf("I'm producer %s\n",(char*)arg);
	//pthread_exit((void*)strcat((char*)arg," is finishing\n"));
	//char* priority[arr_size];
	//priority[0]=(char*)1;
	while(true)
	{
		//reseter=ind;
		//if(priority[0]==sharedmemory[0]) continue;
		sem_getvalue(&full_array,&value);
		sem_wait(&full_array); //down
		
		pthread_mutex_lock(&mutex);
		printf("cyclist %s started taking service from serviceman %d\n",(char*)arg,value);
	
		sleep(5);
		printf("cyclist %s finished taking service from serviceman %d\n",(char*)arg,value);
		pthread_mutex_unlock(&mutex);
		sem_post(&empty_array); //up
	}
}

int main()
{
	int res;

	res= sem_init(&full_array,0,0);
	if (res != 0)
		{
			printf("Failed!\n");
		}
	res= sem_init(&empty_array,0,arr_size);
	if (res != 0)
		{
			printf("Failed!\n");
		}


	pthread_mutex_init(&mutex, NULL);


	pthread_t serviceman[number_of_serviceman];

	for (int i = 0; i < number_of_serviceman; ++i)
	{
		char* id = new char[3];
		strcpy(id , to_string(i+1).c_str());
		res = pthread_create(&serviceman[i],NULL,produce_item,id);
		
		if (res != 0)
		{
			printf("Failed!\n");
		}
	}

	pthread_t cycles[number_of_cycles];

	for (int i = 0; i < number_of_cycles; ++i)
	{
		char* id = new char[3];
		strcpy(id , to_string(i+1).c_str());
		res = pthread_create(&cycles[i],NULL,consume_item,id);
		if (res != 0)
		{
			printf("Failed!\n");
		}
	}



	



	for (int i = 0; i < number_of_cycles; ++i)
	{
		void* result;
		pthread_join(cycles[i],&result);
		printf("%s", (char*)result);
	}
	
	for (int i = 0; i < number_of_serviceman; ++i)
	{
		void* result;
		pthread_join(serviceman[i],&result);
		printf("%s", (char*)result);
	}
	//cout<<"hello"<<endl;
	sem_destroy(&full_array);
	sem_destroy(&empty_array);
	pthread_mutex_destroy(&mutex);

	return 0;
}