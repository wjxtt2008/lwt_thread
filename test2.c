#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lwt.h"
void Thread1() {
	int i,j,k;
	for (i = 0; i < 15; i++) {
    		for (j = 0; j < 500; j++){
			for(k = 0; k < 250000; k++);		
		}
	printf("this is thread1:times %d\n",i);
  	}

}

void Thread2() {
	int i,j,k;
	for (i = 0; i < 15; i++) {
    		for (j = 0; j < 500; j++){
			for(k = 0; k < 250000; k++);		
		}
	printf("this is thread2:times %d\n",i);
  	}

}
void Thread3() {
	int i,j,k;
	for (i = 0; i < 15; i++) {
    		for (j = 0; j < 500; j++){
			for(k = 0; k < 250000; k++);		
		}
	printf("this is thread3:times %d\n",i);
  	}

}
void Thread_main() {
	int i,j,k;
	for (i = 0; i < 20; i++) {
    		for (j = 0; j < 500; j++){
			for(k = 0; k < 250000; k++);		
		}
	printf("this is thread_main:times %d\n",i);
  	}

}

int main(){
	lwt_init();
	printf("init ready_queue size: %d\n",size());
	lwt_create(Thread1);
	printf("Thread1 ready_queue size: %d\n",size());
	lwt_create(Thread2);
	printf("Thread2 ready_queue size: %d\n",size());
	lwt_create(Thread3);
	printf("Thread3 ready_queue size: %d\n",size());
	Thread_main();
	printf("complete\n");
	return 0;
}
