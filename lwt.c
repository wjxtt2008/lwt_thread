#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <signal.h>
#include <setjmp.h>
#include "lwt.h"
#include "Queue.h"

#define lwt_quantum 500000
//#define _DEBUG_
lwt_func temp_func;
lwt_struct *temp_thread;
sigset_t blockset;
Queue* ready_queue; // ready_queue is a CircleQueue

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

void lwt_init() {
	
	ready_queue = InitQueue();
	temp_thread = (lwt_struct *)malloc(sizeof(lwt_struct));
	EnCircleQueue(ready_queue,temp_thread);
	signal(SIGALRM, lwt_scheduler);
  	ualarm(lwt_quantum,lwt_quantum);
	
	sigemptyset(&blockset);
	sigaddset(&blockset, SIGALRM);

}

void lwt_create(lwt_func pfunc) {
	void* addr;

	sigprocmask(SIG_BLOCK, &blockset, NULL);

	temp_func = pfunc;

	GetFront(ready_queue,&temp_thread);
	lwt_store(temp_thread);

	if(setjmp(temp_thread->t_env) != 0) {
		sigprocmask(SIG_UNBLOCK, &blockset, NULL);
		return;
	}
	else{
		temp_thread = (lwt_struct *)malloc(sizeof(lwt_struct));
		EnCircleQueue(ready_queue,temp_thread);

		addr = malloc(16384)+16384;
		temp_thread->t_sp=temp_thread->t_bp=addr;
//		EnCircleQueue(ready_queue,temp_thread); //why put it here make it a bug? fuck you gcc!
#ifdef _DEBUG_		
if(temp_thread==NULL)
	printf("create read out temp thread is NULL\n");
#endif
		lwt_load(temp_thread);	
		sigprocmask(SIG_UNBLOCK, &blockset, NULL);
		temp_func();		
	}	
}
void lwt_scheduler (int dummy) {
#ifdef _DEBUG_	
printf("scheduler \n");
#endif
	if(ready_queue->size>1) {
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
#ifdef _DEBUG_
printf(" Load next thread \n");
#endif 
			MoveCircleQueue(ready_queue,&temp_thread);
			lwt_load(temp_thread);
#ifdef _DEBUG_
printf(" jmp to next thread \n");
#endif
			longjmp(temp_thread->t_env,1);				
		}
	}

}


