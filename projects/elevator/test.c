#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elevator.h"
#include "thread.h"

typedef struct {
	viva_mutex_t *mutex;
	rectangle		 *rect;
	size_t				duration;
} shuttle_rect_t;

void rect_shuttle(void *args) {
	shuttle_rect_t *s_rect = args;
	Threading.Mutex.lock(s_rect->mutex);
	viva_rectangle.draw(s_rect->rect);
	Threading.Mutex.unlock(s_rect->mutex);
	Sleep(s_rect->duration);
	Threading.Mutex.lock(s_rect->mutex);
	viva_rectangle.fill_all(s_rect->rect, L'x').draw(s_rect->rect);
	Threading.Mutex.unlock(s_rect->mutex);
}

void test_main(void) {
	val mutex = Threading.Mutex.create();

	var rect1 = (shuttle_rect_t){
		mutex,
		viva_rectangle.init((COORD){10, 5}, (COORD){5, 5}, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY),
		1000};

	viva_rectangle.set_outline(rect1.rect, L'#');
	val thread1 = Threading.Thread.create(rect_shuttle, &rect1);

	var rect2 = (shuttle_rect_t){
		mutex,
		viva_rectangle.init((COORD){10, 5}, (COORD){5, 10}, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY), 500};

	viva_rectangle.set_outline(rect2.rect, L'#');
	val thread2 = Threading.Thread.create(rect_shuttle, &rect2);

	Threading.Thread.join(thread1);
	Threading.Thread.join(thread2);
}


int main(const int argc, char **argv, const char **envp) {
	val init_callback = console_init();
	if (init_callback != kOkStatus)
		return fprintf(stderr, "Elevator: Console initialization failed with error code %d\n", init_callback),
					 init_callback;
	test_main();
	val restore_callback = console_restore();
	if (restore_callback != kOkStatus)
		return fprintf(stderr, "Elevator: Console restore failed with error code %d\n", restore_callback), restore_callback;

	return 0;
}
