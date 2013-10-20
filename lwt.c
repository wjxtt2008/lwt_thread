#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <signal.h>
#include <setjmp.h>
#include "lwt.h"
#include "Queue.h"

#define lwt_quantum 500000
#define lwt_factor 2
#define true 1
#define false 0

//#define _DEBUG_

int wakeup_flag;
lwt_func temp_func;
lwt_struct *temp_thread,*next_thread;
sigset_t blockset;
Queue* ready_queue,*sleep_queue; // ready_queue is a CircleQueue

//lwt_struct thread_list[17];

#ifdef __x86_64__

	#define lwt_store(pthread) \
		do { \
			asm volatile ( \
				"movq %%rsp, %0\n\t" \
				"movq %%rbp, %1" \
				:"=a"(pthread->t_sp),"=b"(pthread->t_bp) \
				: \
				: ); \
		} while(0)

	#define lwt_load() \
		do { \
			asm volatile ( \
				"movq %0, %%rsp\n\t" \
				"movq %1, %%rbp" \
				: \
				:"a"(pthread->t_sp),"b"(pthread->t_bp) \
				:);\
		} while(0)
#else
	#define lwt_store(pthread) \
		do { \
			asm volatile ( \
				"movl %%esp, %0\n\t" \
				"movl %%ebp, %1" \
				:"=a"(pthread->t_sp),"=b"(pthread->t_bp) \
				: \
				: ); \
		} while(0)

	#define lwt_load(pthread) \
		do { \
			asm volatile ( \
				"movl %0, %%esp\n\t" \
				"movl %1, %%ebp" \
				: \
				:"a"(pthread->t_sp),"b"(pthread->t_bp) \
				:);\
		} while(0)

#endif

int size() {
	return ready_queue->size;
}

void ticktock(lwt_struct* pthread){
	pthread->t_slpc--;
	if(pthread->t_slpc<=0)
		wakeup_flag=true;
}

void lwt_init() {
	
	ready_queue = InitQueue();
	sleep_queue = InitQueue();

	temp_thread = (lwt_struct *)malloc(sizeof(lwt_struct));
	temp_thread->t_state=lwt_READY;
	temp_thread->t_father=NULL;

	EnCircleQueue(ready_queue,temp_thread);
	signal(SIGALRM, lwt_scheduler);
  	ualarm(lwt_quantum,lwt_quantum);
	
	sigemptyset(&blockset);
	sigaddset(&blockset, SIGALRM);

}

lwt_struct* lwt_create(lwt_func pfunc) {
	void* addr;
	lwt_struct* new_thread;

	sigprocmask(SIG_BLOCK, &blockset, NULL);

	temp_func = pfunc;

	GetFront(ready_queue,&temp_thread);
	lwt_store(temp_thread);

	new_thread=(lwt_struct *)malloc(sizeof(lwt_struct));	

	if(setjmp(temp_thread->t_env) != 0) {
		sigprocmask(SIG_UNBLOCK, &blockset, NULL);
		return new_thread;
	}
	else{
		//temp_thread = (lwt_struct *)malloc(sizeof(lwt_struct));
		new_thread->t_father=temp_thread;
		new_thread->t_state=lwt_READY;

		EnCircleQueue(ready_queue,new_thread);

		addr = malloc(16384)+16384;
		new_thread->t_sp=new_thread->t_bp=addr;
//		EnCircleQueue(ready_queue,temp_thread); //why put it here make it a bug? fuck you gcc!
#ifdef _DEBUG_		
if(new_thread==NULL)
	printf("create read out temp thread is NULL\n");
#endif
		lwt_load(new_thread);	
		sigprocmask(SIG_UNBLOCK, &blockset, NULL);
		temp_func();		
	}	
}
void lwt_wait(lwt_struct* wait_thread) {
	printf("lwt_wait\n");
	sigprocmask(SIG_BLOCK, &blockset, NULL);

	if(wait_thread->t_state==lwt_EXIT) {
		printf("thread already exit\n");
		free(wait_thread);
		sigprocmask(SIG_UNBLOCK, &blockset, NULL);
		return;
	}

	else{
		DeCircleQueue(ready_queue,&temp_thread);
		temp_thread->t_state=lwt_WAIT;
		lwt_store(temp_thread);

		if(setjmp(temp_thread->t_env) != 0) {
			//void* addr=(wait_thread->t_bp)-16384;
			//free(addr);
			sigprocmask(SIG_BLOCK, &blockset, NULL);
			return;
		}
		else {
			
			GetFront(ready_queue,&temp_thread);
			printf("jump to next\n");
			lwt_load(temp_thread);
			longjmp(temp_thread->t_env,1);		
		}
	}
}
void lwt_exit(){
	sigprocmask(SIG_BLOCK, &blockset, NULL);
	DeCircleQueue(ready_queue,&temp_thread);
	temp_thread->t_state=lwt_EXIT;
	//free((temp_thread->t_bp)-16384);
	

	if(temp_thread->t_father->t_state==lwt_WAIT){
		printf("wake up fathter\n");
		EnCircleQueue(ready_queue,temp_thread->t_father);
	}
	GetFront(ready_queue,&temp_thread);
	lwt_load(temp_thread);
	longjmp(temp_thread->t_env,1);		
	
}
void lwt_sleep(int sec) {
	GetFront(ready_queue,&temp_thread);
	temp_thread->t_state=lwt_SLEEP;
	temp_thread->t_slpc=sec*lwt_factor;
	pause();
}

void lwt_scheduler (int dummy) {
#ifdef _DEBUG_	
printf("scheduler \n");
#endif
	if(ready_queue->size>1) {
		next_thread=NULL;
		GetFront(ready_queue,&temp_thread);
#ifdef _DEBUG_
printf(" Save current thread \n");	
if(temp_thread==NULL)
	printf("temp thread is NULL\n");
#endif
		lwt_store(temp_thread);

		if(setjmp(temp_thread->t_env) != 0) {
			return;
		}
		else {
			if(temp_thread->t_state==lwt_SLEEP){
				EnCircleQueue(sleep_queue,temp_thread);
				DeCircleQueue(ready_queue,&next_thread);
			}
			if(sleep_queue->size>0){
				QueueTraverse(sleep_queue,ticktock);
			}
			if(wakeup_flag==true){
				int i;
				wakeup_flag=false;
				for(i=0;i<sleep_queue->size;i++){
					MoveCircleQueue(sleep_queue,&temp_thread);
					if(temp_thread->t_slpc<=0)
						break;
				}
				temp_thread->t_state=lwt_READY;
				next_thread=temp_thread;
				EnCircleQueue(ready_queue,next_thread);
				DeCircleQueue(sleep_queue,&temp_thread);
			}
			if(next_thread==NULL){
				MoveCircleQueue(ready_queue,&next_thread);
			}
#ifdef _DEBUG_
printf(" Load next thread \n");
#endif 
			
			lwt_load(next_thread);
#ifdef _DEBUG_
printf(" jmp to next thread \n");
#endif
			longjmp(next_thread->t_env,1);				
		}
	}

}


