#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define CYCLES 100000


int numberOfPhilosophers;
int numberOfForks;
int leftFork;
int rightFork;

pthread_t *philosophers;
pthread_mutex_t *forks;

void eating (int philosopherID){
	printf("Philosopher n°%d is eating\n",philosopherID);

}

void* philosopher(void* arg){

	int *id = (int*)arg;
	leftFork = *id;
	rightFork = (leftFork + 1) % numberOfPhilosophers; 

	printf("this is my id : %d\n this is my leftFork :%d\n right fork : %d\n",*id, leftFork, rightFork);


	for (int i = 0; i < CYCLES; i++){
		if(leftFork<rightFork) {
      		pthread_mutex_lock(&forks[leftFork]);
	  		printf("Philosopher n°%d took his left fork [%d]\n",*id,leftFork);
      		pthread_mutex_lock(&forks[rightFork]);
      		printf("Philosopher n°%d took his right fork [%d]\n",*id,rightFork);
    		} else {
	  		pthread_mutex_lock(&forks[rightFork]);
	  		printf("Philosopher n°%d took his right fork [%d]\n",*id,rightFork);
      		pthread_mutex_lock(&forks[leftFork]);
      		printf("Philosopher n°%d took his left fork [%d]\n",*id,leftFork);
    		}

    		eating(*id);

    		pthread_mutex_unlock(&forks[leftFork]);
		printf("Philosopher n°%d has freed his left fork [%d]\n",*id,leftFork);
    		pthread_mutex_unlock(&forks[rightFork]);
    		printf("Philosopher n°%d has freed his right fork [%d]\n",*id,rightFork);
	}

	return (NULL);
}



int main(int argc, char const *argv[])
{
	// HANDLING THE INPUT ARGUMENTS
	if(argc != 2){
		printf("This program uses 1 argument.\n");
		exit(-1);
	}

	numberOfPhilosophers = atoi(argv[1]);
	numberOfForks = numberOfPhilosophers;


	 //CREATING THE PHILOSOPHERS AND FORKS ARRAYS

	philosophers = (pthread_t*)malloc(numberOfPhilosophers*sizeof(pthread_t));
	if(philosophers==NULL){
		printf("Philosophers memory was not allocated\n");
		exit(0);
	}else{
		printf("Philosophers : %d \n",numberOfPhilosophers);
	}

	forks = (pthread_mutex_t*)malloc(numberOfForks*sizeof(pthread_mutex_t));

	if(forks==NULL){
		printf("Forks memory was not allocated\n");
		exit(0);
	}else{
		printf("Forks : %d\n",numberOfForks);
	}

	// INITIALIZING THE MUTEXES IN THE FORK ARRAY

	int err;

	for (int i = 0; i < numberOfForks; i++)
	{
		err = pthread_mutex_init(&(forks[i]), NULL);
		if(err!=0){
			error(err,"MUTEX NOT INITIALIZED");
		}else{
			printf("FORK N°%d CREATED\n", i);
		}
	}

	//CREATING THE PHILOSOPHER THREADS

	for (int i = 0; i < numberOfPhilosophers; i++)
	{
		int* j = (int*)malloc(sizeof(int));
		*j = i;
		err = pthread_create(&(philosophers[i]),NULL,&philosopher,(void *) j);
		if(err!=0){
			error(err,"pthread_create");
		}else{
			printf("THREAD N° %d CREATED\n", i);
		}
	}

	//JOINING THE PHILOSOPHER THREADS

	for (int i = 0; i < numberOfPhilosophers; i++)
	{
		err = pthread_join(philosophers[i],NULL);
		 if(err!=0){
		 	error(err,"pthread_join");
		 }else{
			printf("THREAD N° %d JOINED\n", i);
		}
	}


	//DESTROYING THE FORK MUTEXES

	for (int i = 0; i < numberOfForks; ++i)
	{
		err=pthread_mutex_destroy(&(forks[i]));
  		if(err!=0){
  	    	error(err,"pthread_mutex_destroy");
  		}
	}

	//FREEING THE ARRAYS OF FORKS AND PHILOSOPHERS

	free(philosophers);
	free(forks);

	return 0;
}