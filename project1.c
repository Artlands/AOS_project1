/*
 * Implement a multithreaded produced-consumer problem with PThreads library
 * Producers produce items and put into a shared buffer, then these items are
 * consumed by consumers.
 * Compile with gcc -Wall project1.c -lpthread
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// #define MAX_ITEMS 512
#define NUM_THREADS	8
#define BUFFER_SIZE 8

pthread_muted_t the_mutex;
pthread_cond_t condc, condp;		//used for signaling
int buffer = 0;		//shared buffer, used between producer and consumer

void *producer(void *pro_id) {		//produce data
	long pro_threadid;
	pro_threadid = (long)pro_id;

	int i;
	int MAX_EACH = 64;

	for(i = 1; i <= MAX_EACH; i++) {
		pthread_mutex_lock(&the_mutex);		//get exclusive access to buffer
		while(buffer == BUFFER_SIZE ) {
			printf("producer %ld found the buffer is full and waiting for a
				consumer to consume", pro_threadid);
			pthread_cond_wait(&condp, &the_mutex);
		}
		buffer = buffer + i; // put item in buffer
		printf("producer %ld put %d item(s)to buffer\n", pro_threadid, i );
		pthread_cond_signal(&condc); // wakeup consumer
		pthread_mutex_unlock(&the_mutex); // release access to buffer
	}
	pthread_exit(0);
}

void *consumer(void *con_id) {		//consume data
	long con_threadid;
	con_threadid = (long)con_id;

	int i;
	int MAX_EACH = 64;

	for(i = 1; i <= MAX_EACH; i++) {
		pthread_mutex_lock(&the_mutex);		//get exclusive access to buffer
		while(buffer == 0) {
			pthread_cond_wait(&condc, &the_mutex);		//wakeup Producer
		}
		printf("consumer %ld take %d out of buffer, ", con_threadid, i);
		buffer = buffer - i;
		printf("and set the buffer to %d\n", buffer);
		pthread_cond_signal(&condp);
		pthread_mutex_unlock(&the_mutex); // release access to buffer
	}
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	pthread_t threads_pro[NUM_THREADS], threads_con[NUM_THREADS];
	int status_pro, status_con;
	long i;

	pthread_mutex_init(&the_mutex, NULL);
	pthread_cond_init(&condc, NULL);
	pthread_cond_init(&condp, NULL);

	for (i = 0; i < NUM_THREADS; i++) {
		status_pro = pthread_create(&threads_pro[i], NULL, producer, (void *)i);
		status_con = pthread_create(&threads_con[i], NULL, consumer, (void *)i);
		if (status_pro != 0 || status_con != 0) {
			printf("ERROR; return code from pthread_create() is %d, %d\n",
				status_pro, status_con);
			exit(-1);
		}
		pthread_join(thread_pro[i], NULL);
		pthread_join(thread_con[j], NULL)
	}

	pthread_cond_destory(&condc);
	pthread_cond_destory(&condp);
	pthread_mutex_destory(&the_mutex);
	pthread_exit(NULL);
}
