#ifndef LWT_H
#define LWT_H
#include <setjmp.h>
//#include "Queue.h"
//extern Queue;

typedef enum lwt_state{
	lwt_READY,
	lwt_WAIT,
	sem_WAIT,
	lwt_SLEEP,
	lwt_EXIT

} lwt_state;

//typedef void (*lwt_func)(int i);

typedef struct lwt_struct {
	int t_id;
	int t_slpc;//sleep cycle

	void* t_sp;
	void* t_bp;
	struct lwt_struct* t_father;
	struct lwt_struct* t_wthread;
	jmp_buf t_env;
	//lwt_func t_func;
	lwt_state t_state;
}lwt_struct;
/**********************/
typedef lwt_struct* Item;
//typedef int Item;
typedef struct node * PNode;
typedef struct node
{
	Item data;
	PNode next;
}Node;

typedef struct
{
	PNode front;
	PNode rear;
	int size;
}Queue;
/**********************/
typedef struct lwt_semaphore{
	int sem_value;
	//void* sem_queue;
	Queue *sem_queue;
}lwt_semaphore;

int size();

void lwt_scheduler (int dummy);

void lwt_init();

//lwt_struct* lwt_create(void (*pfunc)(),char *p);
lwt_struct* lwt_create(void (*pfunc)(),int argc,char **argv);
//lwt_struct* lwt_create(lwt_func pfunc);

void lwt_wait(lwt_struct* wait_thread);

void lwt_exit();

void lwt_sleep(int sec);

void lwt_sem_init(lwt_semaphore* sem,int value);
/* sem_up*/
void lwt_semV(lwt_semaphore* sem);
/* sem_down*/
void lwt_semP(lwt_semaphore* sem);


//void lwt_wait();

#endif
