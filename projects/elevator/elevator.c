#include "elevator.h"

#include <net/ancillarycat/viva/viva.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>
#include "thread.h"

typedef struct {
	viva_mutex_t *mutex;
	rectangle		 *rect;
	int						current_floor;
	int						max_floor;
	int						min_floor;
	int						direction;
	size_t				duration;
} elevator_t;

void draw_building(const viva_mutex_t *mutex) {
	Threading.Mutex.lock(mutex);
	for (int i = 1; i <= 5; ++i) {
		rectangle *floor =
			viva_rectangle.init((COORD){20, 3}, (COORD){0, i * 4}, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		viva_rectangle.set_outline(floor, L'#');
		viva_rectangle.draw(floor);
		// viva_rectangle.free(floor);
	}
	Threading.Mutex.unlock(mutex);
}

void elevator_shuttle(void *args) {
	elevator_t *elevator = args;
	while (1) {
		Threading.Mutex.lock(elevator->mutex);
		viva_rectangle.draw(elevator->rect);
		Threading.Mutex.unlock(elevator->mutex);
		Sleep(elevator->duration);

		Threading.Mutex.lock(elevator->mutex);
		viva_rectangle.fill_all(elevator->rect, L' ').draw(elevator->rect);
		Threading.Mutex.unlock(elevator->mutex);

		elevator->current_floor += elevator->direction;
		if (elevator->current_floor == elevator->max_floor || elevator->current_floor == elevator->min_floor) {
			elevator->direction = -elevator->direction;
		}

		elevator->rect->upper_left.Y = elevator->current_floor * 4;
	}
}

status_t make_building_and_elevator() {
	viva_mutex_t *mutex = Threading.Mutex.create();
	draw_building(mutex);

	elevator_t elevator = {.mutex					= mutex,
												 .rect					= viva_rectangle.init((COORD){10, 3}, (COORD){5, 4},
																															FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY),
												 .current_floor = 1,
												 .max_floor			= 5,
												 .min_floor			= 1,
												 .direction			= 1,
												 .duration			= 1000};

	viva_rectangle.set_outline(elevator.rect, L'#');
	viva_rectangle.draw(elevator.rect);

	viva_thread_t *elevator_thread = Threading.Thread.create(elevator_shuttle, &elevator);
	Threading.Thread.join(elevator_thread);

	Threading.Mutex.destroy(mutex);
	return kOkStatus;
}
status_t elevator_parse_args(const int argc, char ***argv, enum choice *choice, void **data) { return kOkStatus; }

status_t make_floor() { return kOkStatus; }
status_t elevator_main(enum choice choice, void *data) {
	make_building_and_elevator();
	return kOkStatus;
}

/*
						############## ##############
						#            # #            #
						#						 # #            #
						# floor n	   # #            # <- elevator
						############## ##############
						// floor n-1 ...
						...
						floor 1
*/
