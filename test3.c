#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lwt.h"
void Thread1() {
	int i,j,k;
	for (i = 0; i < 10; i++) {
    		for (j = 0; j < 500; j++){
			for(k = 0; k < 250000; k++);		
		}
		printf("this is thread1:times %d\n",i);
  	}
	
	lwt_exit();

}

void Thread2() {
	int i,j,k;
	for (i = 0; i < 15; i++) {
    		for (j = 0; j < 500; j++){
			for(k = 0; k < 250000; k++);		
		}
		printf("this is thread2:times %d\n",i);
  	}
	printf("thread2 exit\n");
	lwt_exit();

}
void Thread3() {
	int i,j,k;
	for (i = 0; i < 15; i++) {
    		for (j = 0; j < 500; j++){
			for(k = 0; k < 250000; k++);		
		}
		printf("this is thread3:times %d\n",i);
  	}
	printf("thread3 exit\n");
	lwt_exit();
}
void Thread_main() {
	int i,j,k;
	for (i = 0; i < 5; i++) {
    		for (j = 0; j < 500; j++){
			for(k = 0; k < 250000; k++);		
		}
		printf("this is thread_main:times %d\n",i);
  	}
}

int main(){
	lwt_struct *thread1,*thread2,*thread3;

	lwt_init();
	printf("init \n");

	thread1=lwt_create(Thread1);
	
	printf("Thread1 ready_queue size: %d\n",size());

	thread2=lwt_create(Thread2);
	printf("Thread2 ready_queue size: %d\n",size());
	
	thread3=lwt_create(Thread3);
	printf("Thread3 ready_queue size: %d\n",size());

	lwt_sleep(5);
	Thread_main();
	
	lwt_wait(thread1);
	lwt_wait(thread2);
	lwt_wait(thread3);

	printf("complete\n");

	return 0;
}
