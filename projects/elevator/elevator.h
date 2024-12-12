#pragma once
#include "net/ancillarycat/viva/utils/console.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "string.h"

static const char *FirstNames[] = {
	"Amy",			"Mike",		 "Chris",	 "Tina",			"Sam",			"Laura",	"Tom",		"Jack",			 "Jill",		 "Mark",
	"Paul",			"Anna",		 "John",	 "Ella",			"Nina",			"Luke",		"Ryan",		"Sophie",		 "Zoe",			 "Liam",
	"Noah",			"Emma",		 "Olivia", "Ava",				"Isabella", "Mia",		"Sophia", "Charlotte", "Amelia",	 "Harper",
	"Evelyn",		"Abigail", "Emily",	 "Elizabeth", "Mila",			"Ella",		"Avery",	"Scarlett",	 "Grace",		 "Chloe",
	"Victoria", "Riley",	 "Aria",	 "Lily",			"Nora",			"Camila", "Hannah", "Elena",		 "Penelope", "Layla"};

static const char *LastNames[] = {
	"Smith", "Johnson",	 "Williams", "Brown",		 "Jones",		"Garcia",		"Miller",		"Davis",	 "Martinez", "Hernandez",
	"Lopez", "Gonzalez", "Wilson",	 "Anderson", "Thomas",	"Taylor",		"Moore",		"Jackson", "Martin",	 "Lee",
	"Perez", "Thompson", "White",		 "Harris",	 "Sanchez", "Clark",		"Ramirez",	"Lewis",	 "Robinson", "Walker",
	"Young", "Allen",		 "King",		 "Wright",	 "Scott",		"Torres",		"Nguyen",		"Hill",		 "Flores",	 "Green",
	"Adams", "Nelson",	 "Baker",		 "Hall",		 "Rivera",	"Campbell", "Mitchell", "Carter",	 "Roberts", "Gomez"};
static const char *elevatorMessages[] = {"Elevator is stationary, a call is on the current floor ==> Open the door",
																	"Elevator is stationary, a call is on an upper floor ==> Move up",
																	"Elevator is stationary, a call is on a lower floor ==> Move down",
																	"Elevator is stationary, passengers want to go up ==> Move up",
																	"Elevator is stationary, passengers want to go down ==> Move down",
																	"Elevator is stationary, overloaded ==> Close the door and warn"};
struct _passenger_t {
	char name[50];
	int	 weight;
	int	 position;
	int	 destination;
	int	 current_time;
	int	 in_elevator_timestamp;
	int	 out_elevator_timestamp;
	int	 waiting_time;
	int	 state;
} ;

struct _elevator_t {
	int					total_passenger;
	int					total_weight;
	int					current_floor;
	int					quarter;
	int					stay_time;
	int					prev_state;
	int					current_state;
	struct _passenger_t passenger[100];
};

struct _floor_t {
	int					total_people;
	struct _passenger_t up_passengers[100], down_passengers[100];
	int					total_up, total_down;
};

typedef struct _passenger_t passenger_t;
typedef struct _elevator_t	elevator_t;
typedef struct _floor_t			floor_t;

void show_menu();
void init_terminal();
void draw_elevator(const elevator_t *);
void rules(int);
void initialize(elevator_t *, floor_t *);
void infomation_panel(elevator_t *, floor_t *, int, int, int);
void passenger_info_1(elevator_t *, floor_t *, int, int, int);
void passenger_info_2(elevator_t *, floor_t *, int, int, int);


struct viva_cstd_terminal_rectangle;
struct VIVA_CSTD_TERMINAL_RECT;

status_t elevator_parse_args(int, char ***, enum choice *, void **);
status_t elevator_main();

static inline struct viva_cstd_terminal_rectangle *viva_cstd_terminal_rect_init(COORD, COORD, WORD);
static inline struct VIVA_CSTD_TERMINAL_RECT
viva_cstd_terminal_rect_set_outline(const struct viva_cstd_terminal_rectangle *, wchar_t);
static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_draw(const struct viva_cstd_terminal_rectangle *);
static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_free(void *);
static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_fill(const struct viva_cstd_terminal_rectangle *,
																																					wchar_t);
static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_fill_all(const struct viva_cstd_terminal_rectangle *,
wchar_t);

struct VIVA_CSTD_TERMINAL_RECT {
	typeof(viva_cstd_terminal_rect_init)				*init;
	typeof(viva_cstd_terminal_rect_set_outline) *set_outline;
	typeof(viva_cstd_terminal_rect_draw)				*draw;
	typeof(viva_cstd_terminal_rect_free)				*free;
	typeof(viva_cstd_terminal_rect_fill)				*fill;
	typeof(viva_cstd_terminal_rect_fill_all)	*fill_all;
} static const viva_rectangle = {
	.init				 = viva_cstd_terminal_rect_init,
	.set_outline = viva_cstd_terminal_rect_set_outline,
	.draw				 = viva_cstd_terminal_rect_draw,
	.free				 = viva_cstd_terminal_rect_free,
	.fill				 = viva_cstd_terminal_rect_fill,
	.fill_all		 = viva_cstd_terminal_rect_fill_all,
};
struct viva_cstd_terminal_rectangle {
	COORD			 size;
	COORD			 upper_left;
	PCHAR_INFO buffer;
	WORD			 attributes;
	HANDLE		 console;
};
static inline struct viva_cstd_terminal_rectangle *
viva_cstd_terminal_rect_init(const COORD size, const COORD upper_left, const WORD attributes) {
	val rect = alloc(struct viva_cstd_terminal_rectangle, 1);
	*rect = (struct viva_cstd_terminal_rectangle){
		.size				= size,
		.upper_left = upper_left,
		.attributes = attributes,
		.buffer			= malloc(size.X * size.Y * sizeof(CHAR_INFO)),
		.console		= GetStdHandle(STD_OUTPUT_HANDLE),
	};
	return rect;
}
static inline struct VIVA_CSTD_TERMINAL_RECT
viva_cstd_terminal_rect_set_outline(const struct viva_cstd_terminal_rectangle *rect, const wchar_t outline_char) {
	for (int y = 0; y < rect->size.Y; y++)
		for (int x = 0; x < rect->size.X; x++) {
			val cell				 = rect->buffer + y * rect->size.X + x;
			cell->Attributes = rect->attributes;
			if (y == 0 or y == rect->size.Y - 1 or x == 0 or x == rect->size.X - 1)
				cell->Char.UnicodeChar = outline_char; // Set outline
			else
				cell->Char.UnicodeChar = L' '; // Interior empty space
		}
	return viva_rectangle;
}
static inline struct VIVA_CSTD_TERMINAL_RECT
viva_cstd_terminal_rect_draw(const struct viva_cstd_terminal_rectangle *rect) {
	SMALL_RECT writeRegion = {
		.Left		= rect->upper_left.X,
		.Top		= rect->upper_left.Y,
		.Right	= rect->upper_left.X + rect->size.X - 1,
		.Bottom = rect->upper_left.Y + rect->size.Y - 1,
	};
	WriteConsoleOutput(rect->console, rect->buffer, rect->size, (COORD){0, 0}, &writeRegion);
	return viva_rectangle;
}
static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_free(void *rect) {
	free(((struct viva_cstd_terminal_rectangle *)(void**)rect)->buffer);
	free(*((void **)rect));
	return viva_rectangle;
}

static inline struct VIVA_CSTD_TERMINAL_RECT
viva_cstd_terminal_rect_fill(const struct viva_cstd_terminal_rectangle *rect, const wchar_t fill_char) {
	for (int y = 1; y < rect->size.Y - 1; y++)
		for (int x = 1; x < rect->size.X - 1; x++) {
			val cell							 = rect->buffer + y * rect->size.X + x;
			cell->Attributes			 = rect->attributes;
			cell->Char.UnicodeChar = fill_char;
		}
	return viva_rectangle;
}

static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_fill_all(const struct viva_cstd_terminal_rectangle *rect,
																																						const wchar_t fill_char) {
	for (int y = 0; y < rect->size.Y; y++)
		for (int x = 0; x < rect->size.X; x++) {
			val cell							 = rect->buffer + y * rect->size.X + x;
			cell->Attributes			 = rect->attributes;
			cell->Char.UnicodeChar = fill_char;
		}
	return viva_rectangle;
}


#undef VIVA_CSTD_TERMINAL_RECT
#define VIVA_CSTD_TERMINAL_RECT // nothing

typedef struct viva_cstd_terminal_rectangle rectangle;

#define auto_ptr_rect __attribute__((cleanup(viva_cstd_terminal_rect_free)))
