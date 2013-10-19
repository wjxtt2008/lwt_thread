#ifndef LWT_H
#define LWT_H
#include <setjmp.h>
typedef enum lwt_state{
	lwt_READY,
	thrd_WAIT,
	sem_WAIT,
	lwt_SLEEP,
	lwt_EXIT

} lwt_state;

typedef void (*lwt_func)();

typedef struct lwt_struct {
	int t_id;
	void* t_sp;
	void* t_bp;
	jmp_buf t_env;
	lwt_func t_func;
	lwt_state t_state;
}lwt_struct;

int size();

void lwt_scheduler (int dummy);

void lwt_init();

void lwt_create(lwt_func pfunc);

void lwt_wait();

void lwt_exit();

void lwt_sleep(int secs);

//void lwt_wait();

#endif
