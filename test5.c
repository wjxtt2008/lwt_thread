#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>

#include "lwt.h"

#define buf_size 500

#define producer_num 2
#define consumer_num 2

#define produce_times 1000
#define consume_times 1000

int buffer[buf_size];
int in=0;
int out=0;
lwt_semaphore mutex,empty,full;

void producer(int p_num,char **argv) {
	int i;
	p_num--;
	printf("producer %c create\n",'A'+p_num);
	for(i = 0; i < produce_times; i++ ){
		lwt_semP(&empty); //get a empty slot 
		lwt_semP(&mutex);
		/*critical section begin*/
		buffer[in]=1;
		printf("producer %c: produced %d \n",'A'+p_num,in);
		in = (in+1) % buf_size;
		/*critical section end*/
		lwt_semV(&mutex);
		lwt_semV(&full);
	}
	printf("producer %c exit\n",'A'+p_num);
	lwt_exit();
}
void consumer(int c_num,char **argv) {
	int i;
	c_num--;
	printf("consumer %c create\n",'A'+c_num);
	for(i = 0; i < consume_times; i++ ){
		lwt_semP(&full); //get a full slot 
		lwt_semP(&mutex);
		/*critical section begin*/
		buffer[out]=0;
		printf("consumer %c: consumed %d \n",'A'+c_num,out);
		out = (out+1) % buf_size;
		/*critical section end*/
		lwt_semV(&mutex);
		lwt_semV(&empty);
	}
	printf("consumer %c exit\n",'A'+c_num);	
	lwt_exit();
}

int main(){
	int i;
	lwt_struct * producer_thread[producer_num];
	lwt_struct * consumer_thread[consumer_num];

	lwt_sem_init(&mutex,1);
	lwt_sem_init(&empty,buf_size);
	lwt_sem_init(&full,0);
	
	lwt_init();


	for(i = 0; i < producer_num; i++){
		producer_thread[i]=lwt_create(producer,i+1,NULL);
		printf("producer %d created\n",i);
	}
	printf("producers created\n");

	for(i = 0; i < consumer_num; i++){
		consumer_thread[i]=lwt_create(consumer,i+1,NULL);
		printf("consumer %d created\n",i);
	}
	printf("consumers created\n");

	for(i = 0; i < producer_num; i++){
		lwt_wait(producer_thread[i]);
	}	
	for(i = 0; i < consumer_num; i++){
		lwt_wait(consumer_thread[i]);
	}
	
	printf("producer comsuer complete\n");
	return 0;
}
