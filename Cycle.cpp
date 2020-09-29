#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstring>
#include <time.h>

using namespace std;

#define number_of_producers 5
#define arr_size 5
//test auto stage
//g++ -D_REENTRANT Cycle.cpp -o Cycle -lpthread
//./Cycle
int ind=1;
pthread_mutex_t mutex;
sem_t full_array, empty_array;

void* produce_item(void* arg)
{
	//printf("I'm producer %s\n",(char*)arg);
	//pthread_exit((void*)strcat((char*)arg," is finishing\n"));
	while(true)
	{
		sem_wait(&empty_array); //down
		pthread_mutex_lock(&mutex);
		sleep(1);
		printf("producer %s has produced %d th item\n",(char*)arg,ind);
		ind++;
		pthread_mutex_unlock(&mutex);
		sem_post(&full_array); //up
	}
}

void* consume_item(void* arg)
{
	//printf("I'm producer %s\n",(char*)arg);
	//pthread_exit((void*)strcat((char*)arg," is finishing\n"));
	while(true)
	{
		sem_wait(&full_array); //down
		pthread_mutex_lock(&mutex);
		sleep(1);
		printf("consumer %s has consumed %d th item\n",(char*)arg,ind);
		pthread_mutex_unlock(&mutex);
		sem_post(&empty_array); //up
	}
}

int main()
{
	int res;

	res= sem_init(&full_array,0,0);
	res= sem_init(&empty_array,0,arr_size);


	pthread_mutex_init(&mutex, NULL);


	pthread_t producers[number_of_producers];

	for (int i = 0; i < number_of_producers; ++i)
	{
		char* id = new char[3];
		strcpy(id , to_string(i+1).c_str());
		res = pthread_create(&producers[i],NULL,produce_item,id);
		pthread_create(&producers[i],NULL,consume_item,id);
		if (res != 0)
		{
			printf("Failed!\n");
		}
	}

	for (int i = 0; i < number_of_producers; ++i)
	{
		void* result;
		pthread_join(producers[i],&result);
		printf("%s", (char*)result);
	}
	//cout<<"hello"<<endl;
	sem_destroy(&full_array);
	sem_destroy(&empty_array);
	pthread_mutex_destroy(&mutex);

	return 0;
}