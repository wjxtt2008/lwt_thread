#ifndef LWT_H
#define LWT_H
#include <setjmp.h>
typedef enum lwt_state{
	lwt_READY,
	lwt_WAIT,
	sem_WAIT,
	lwt_SLEEP,
	lwt_EXIT

} lwt_state;

typedef void (*lwt_func)();

typedef struct lwt_struct {
	int t_id;
	int t_slpc;//sleep cycle

	void* t_sp;
	void* t_bp;
	struct lwt_struct* t_father;
	jmp_buf t_env;
	lwt_func t_func;
	lwt_state t_state;
}lwt_struct;

int size();

void lwt_scheduler (int dummy);

void lwt_init();

lwt_struct* lwt_create(lwt_func pfunc);

void lwt_wait(lwt_struct* wait_thread);

void lwt_exit();

void lwt_sleep(int sec);

//void lwt_wait();

#endif
