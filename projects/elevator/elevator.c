#include "elevator.h"
#include <conio.h>

constexpr int max_weight			 = 500;
constexpr int all_height			 = 8;
constexpr int max_wait_time		 = 80;
constexpr int max_waiting_time = 3;


int total_people = 0;
int speed;
int current_people;
int total_people_in_floor;
int timer;
int info_total					 = 1;
int total_passenger_info = 2;
int hour, minute, second;


void show_menu() {
	Cursor.set((COORD){5, 0});
	printf("***************\n");
	Cursor.set((COORD){5, 1});
	printf("Elevator Simulation\n");
	Cursor.set((COORD){5, 2});
	printf("press anykey to start simulation");
}

void init_terminal() {
	Console.clear(&Terminal);
	system("cls");
	Cursor.set((COORD){37, 1});

	Console.set_color(0x0e | FOREGROUND_INTENSITY, 0);
	printf("###########################################");
	for (int i = 1; i <= 8; ++i) {
		Console.set_color(FOREGROUND_BLUE | FOREGROUND_INTENSITY, 0);

		Cursor.set((COORD){37, (i - 1) * 4 + 2});

		printf("|");

		Cursor.set((COORD){37, (i - 1) * 4 + 3});

		printf("|");

		Cursor.set((COORD){37, (i - 1) * 4 + 4});

		printf("|");

		Cursor.set((COORD){37, (i - 1) * 4 + 5});

		Console.set_color(0x0e | FOREGROUND_INTENSITY, 0);
		printf("###########################################");
	}

	Cursor.set((COORD){81, 1});

	Console.set_color(0x06 | FOREGROUND_INTENSITY, 0);
	printf("###################info###################");
	for (int i = 2; i <= 32; ++i) {

		Cursor.set((COORD){81, i});

		printf("|");

		Cursor.set((COORD){129, i});

		printf("|");
	}

	Cursor.set((COORD){81, 32});

	printf("##############################################");
	Console.set_color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 0);

	Cursor.set((COORD){130, 1});

	Console.set_color(0x0b | FOREGROUND_INTENSITY, 0);
	printf("#####################info#####################");

	Cursor.set((COORD){131, 2});

	printf("all records");
	for (int i = 2; i <= 32; ++i) {

		Cursor.set((COORD){130, i});

		printf("|");

		Cursor.set((COORD){170, i});

		printf("|");
	}

	Cursor.set((COORD){130, 32});

	printf("############################################");
	Console.set_color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 0);
}

void draw_elevator(const elevator_t *elevator) {
	for (int i = 1; i <= 33; ++i) {

		Cursor.set((COORD){1, i});

		printf("                                    ");
	}
	Console.set_color(0x0e | FOREGROUND_INTENSITY, 0);

	Cursor.set((COORD){1, (8 - elevator->current_floor) * 4 + 1 - elevator->quarter});

	printf("####################################");

	Cursor.set((COORD){1, (8 - elevator->current_floor) * 4 + 2 - elevator->quarter});

	printf("|");

	Cursor.set((COORD){36, (8 - elevator->current_floor) * 4 + 2 - elevator->quarter});

	printf("|");

	Cursor.set((COORD){1, (8 - elevator->current_floor) * 4 + 3 - elevator->quarter});

	printf("|");

	Cursor.set((COORD){36, (8 - elevator->current_floor) * 4 + 3 - elevator->quarter});

	printf("|");

	Cursor.set((COORD){1, (8 - elevator->current_floor) * 4 + 4 - elevator->quarter});

	printf("|");

	Cursor.set((COORD){36, (8 - elevator->current_floor) * 4 + 4 - elevator->quarter});

	printf("|");

	Cursor.set((COORD){1, (8 - elevator->current_floor) * 4 + 5 - elevator->quarter});

	printf("####################################");
	Console.set_color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 0);
}

void rules(const int index) {

	Cursor.set((COORD){37, 35});

	Console.set_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY, 0);
	printf("msg %s", elevatorMessages[index - 1]);
	Console.set_color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 0);
}

void initialize(elevator_t *elevator, floor_t *floors) {
	println();
	println("   please input the total people(1-2000):");
	do {
		total_people = get_rec(total_people);
	} while (total_people < 1 || total_people > 2000);
	println("   please input the speed(1-10):");
	do {
		speed = get_rec(speed);
	} while (speed < 1 || speed > 10);
	init_terminal();
}

void infomation_panel(elevator_t *elevator, floor_t *floors, const int variants, const int floor_l, const int id) {
	info_total++;
	if (info_total >= 32) {
		for (int i = 2; i <= 31; ++i) {

			Cursor.set((COORD){82, i});

			printf("                                             ");
		}
		info_total = 2;
	}
	second += timer;
	minute += (int)(second / 60);
	hour += (int)(minute / 60);
	hour %= 24;
	minute %= 60;
	second %= 60;

	Cursor.set((COORD){82, info_total});

	if (variants == 1) {
		printf("%02d:%02d:%02d name %s floor%d", hour, minute, second, floors[floor_l].up_passengers[id].name, floor_l);
	} else if (variants == 2) {
		printf("%02d:%02d:%02d name %s floor%d", hour, minute, second, floors[floor_l].down_passengers[id].name, floor_l);
	} else if (variants == 3) {
		printf("%02d:%02d:%02d name %s", hour, minute, second, floors[floor_l].up_passengers[id].name);
	} else if (variants == 4) {
		printf("%02d:%02d:%02d name %s", hour, minute, second, floors[floor_l].down_passengers[id].name);
	} else if (variants == 5) {
		printf("%02d:%02d:%02d name %s", hour, minute, second, floors[floor_l].up_passengers[id].name);
	} else if (variants == 6) {
		printf("%02d:%02d:%02d name %s", hour, minute, second, floors[floor_l].down_passengers[id].name);
	} else if (variants == 7) {
		printf("%02d:%02d:%02d name %s", hour, minute, second, elevator->passenger[id].name);
	} else if (variants == 8) {
		printf("%02d:%02d:%02d", hour, minute, second);
	} else if (variants == 9) {
		Console.set_color(FOREGROUND_RED | FOREGROUND_INTENSITY, 0);
		printf("%02d:%02d:%02d time out!", hour, minute, second);
		Console.set_color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 0);
		if (rand() % 100 > 94) {

			Cursor.set((COORD){82, info_total + 1});

			Console.set_color(FOREGROUND_RED | FOREGROUND_INTENSITY, 0);
			printf("%02d:%02d:%02d.", hour, minute, second);
			exit(0);
		}
	}
}

void passenger_info_1(elevator_t *elevator, floor_t *floors, const int level, const int index, const int times) {
	floors[level].up_passengers[index].weight = rand() % 31 + 50;
	int first																	= rand() % 2 + 1;
	int second																= rand() % 2 + 1;
	int choose1 = 0, choose2 = 0;

	if (first == 1) {
		choose1 = rand() % 1000 + 1;
		if (choose1 <= 600) {
			choose1 += 5;
			choose1 /= 6;
		} else
			choose1 -= 600;

	} else {
		choose1 = rand() % 50 + 1;
	}

	choose2 = rand() % 50 + 1;
	choose1--, choose2--;

	choose1 %= 50;
	choose2 %= 50;
	strcpy(floors[level].up_passengers[index].name, FirstNames[choose1]);
	strcpy(floors[level].up_passengers[index].name, LastNames[choose2]);
	floors[level].up_passengers[index].name[first * 2 + second * 2] = '\0';
	floors[level].up_passengers[index].position										= level;
	floors[level].up_passengers[index].destination								= rand() % (all_height - level) + level + 1;
	floors[level].up_passengers[index].current_time								= times;
	floors[level].up_passengers[index].state											= 1;
	infomation_panel(elevator, floors, 1, level, index);
	floors[level].up_passengers[index].current_time = times;
}

void passenger_info_2(elevator_t *elevator, floor_t *floors, const int level, const int index, const int times) {
	floors[level].down_passengers[index].weight = rand() % 31 + 50;
	int first																= rand() % 2 + 1;
	int sceond																= rand() % 2 + 1;
	int choose1 = 0, choose2 = 0;


	if (first == 1) {
		choose1 = rand() % 1000 + 1;
		if (choose1 <= 600) {
			choose1 /= 6;
		} else {
			choose1 -= 600;
		}
	} else {
		choose1 = rand() % 50 + 1;
	}

	choose1 %= 50;
	choose2 = rand() % 50 + 1;
	choose2 %= 50;

	strcpy(floors[level].down_passengers[index].name, FirstNames[choose1]);
	strcpy(floors[level].down_passengers[index].name, LastNames[choose2]);
	floors[level].down_passengers[index].name[first * 2 + sceond * 2] = '\0';
	floors[level].down_passengers[index].position													 = level;
	floors[level].down_passengers[index].destination											 = rand() % (level - 1) + 1;
	floors[level].down_passengers[index].current_time											 = times;
	floors[level].down_passengers[index].state														 = 1;
	infomation_panel(elevator, floors, 2, level, index);
	floors[level].down_passengers[index].current_time = times;
}

void draw_people(const floor_t *floors) {
	for (int i = 1; i <= all_height; ++i) {

		Cursor.set((COORD){38, (8 - i) * 4 + 2});

		printf("                         ");

		Cursor.set((COORD){38, (8 - i) * 4 + 4});

		printf("                         ");
	}
	for (int i = 1; i <= all_height; ++i) {
		if (floors[i].total_up > 0) {

			Cursor.set((COORD){38, (8 - i) * 4 + 2});

			printf("^");
			for (int j = 1; j <= floors[i].total_up; ++j) {

				Cursor.set((COORD){40 + (j - 1) * 2, (8 - i) * 4 + 2});

				printf("%d", floors[i].up_passengers[j].destination);
			}
		}
		if (floors[i].total_down > 0) {

			Cursor.set((COORD){38, (8 - i) * 4 + 4});

			printf("v");
			for (int j = 1; j <= floors[i].total_down; ++j) {

				Cursor.set((COORD){40 + (j - 1) * 2, (8 - i) * 4 + 4});

				printf("%d", floors[i].down_passengers[j].destination);
			}
		}
	}
}

void gen_passenger(elevator_t *elevator, floor_t *floors, const int times) {
	for (int i = 1; i <= all_height; ++i) {
		int random_num = rand() % ((11 - speed) * 5) + 1;
		if (random_num == 1 && i != all_height && current_people < total_people) {
			floors[i].total_up++;
			floors[i].total_people++;
			current_people++;
			passenger_info_1(elevator, floors, i, floors[i].total_up, times);
		}
		random_num = rand() % ((11 - speed) * 5) + 1;
		if (random_num == 1 && i != 1 && current_people < total_people) {
			floors[i].total_down++;
			floors[i].total_people++;
			current_people++;
			passenger_info_2(elevator, floors, i, floors[i].total_down, times);
		}
	}
}

void passenger_windows(floor_t *floors, const int kinds, const int i, const int j) {
	total_passenger_info++;
	if (total_passenger_info >= 31) {
		for (int idx = 3; idx <= 30; ++idx) {

			Cursor.set((COORD){130, idx});

			printf("                                             ");
		}
		total_passenger_info = 3;
	}

	Cursor.set((COORD){131, total_passenger_info});

	Console.set_color(FOREGROUND_RED | FOREGROUND_INTENSITY, 0);
	if (kinds == 1)
		printf("%-8s %-2dkg %2d %2d %-3ds %-3ds %-3ds", floors[i].up_passengers[j].name,
					 floors[i].up_passengers[j].weight, floors[i].up_passengers[j].position,
					 floors[i].up_passengers[j].destination, floors[i].up_passengers[j].current_time, -1, -1);
	else
		printf("%-8s %-2dkg %2d %2d %-3ds %-3ds %-3ds", floors[i].down_passengers[j].name,
					 floors[i].down_passengers[j].weight, floors[i].down_passengers[j].position,
					 floors[i].down_passengers[j].destination, floors[i].down_passengers[j].current_time, -1, -1);
	Console.set_color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 0);
}

int inspect_people(elevator_t *elevator, floor_t *floors, const int times) {
	for (int i = 1; i <= all_height; ++i) {
		int j = 1;
		while (j <= floors[i].total_up) {
			if (times - floors[i].up_passengers[j].current_time > max_wait_time) {
				int Point = j;
				infomation_panel(elevator, floors, 3, i, j);
				passenger_windows(floors, 1, i, j);
				floors[i].total_people--;
				if (Point != floors[i].total_up)
					while (Point < floors[i].total_up)
						floors[i].up_passengers[Point] = floors[i].up_passengers[Point + 1], Point++;
				floors[i].total_up--;
				j--;
			}
			j++;
		}
		j = 1;
		while (j <= floors[i].total_down) {
			if (times - floors[i].down_passengers[j].current_time > max_wait_time) {
				int Point = j;
				infomation_panel(elevator, floors, 4, i, j);
				passenger_windows(floors, 2, i, j);
				floors[i].total_people--;
				if (Point != floors[i].total_down)
					while (Point < floors[i].total_down)
						floors[i].down_passengers[Point] = floors[i].down_passengers[Point + 1], Point++;
				floors[i].total_down--;
				j--;
			}
			j++;
		}
	}
	int sum = 0;
	for (int i = 1; i <= all_height; ++i) {
		sum += floors[i].total_up + floors[i].total_down;
	}
	return sum;
}

void door_close(const elevator_t *elevator) {
	Console.set_color(FOREGROUND_BLUE | FOREGROUND_INTENSITY, 0);
	Cursor.set((COORD){37, (8 - elevator->current_floor) * 4 + 2 - elevator->quarter});
	printf("|");
	Cursor.set((COORD){37, (8 - elevator->current_floor) * 4 + 3 - elevator->quarter});
	printf("|");
	Cursor.set((COORD){37, (8 - elevator->current_floor) * 4 + 4 - elevator->quarter});
	printf("|");
	Console.set_color(0x0e | FOREGROUND_INTENSITY, 0);
	Cursor.set((COORD){36, (8 - elevator->current_floor) * 4 + 2 - elevator->quarter});
	printf("|");
	Cursor.set((COORD){36, (8 - elevator->current_floor) * 4 + 3 - elevator->quarter});
	printf("|");
	Cursor.set((COORD){36, (8 - elevator->current_floor) * 4 + 4 - elevator->quarter});
	printf("|");
	Console.set_color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 0);
}

void door_open(const elevator_t *elevator) {
	Cursor.set((COORD){37, (8 - elevator->current_floor) * 4 + 2 - elevator->quarter});
	printf(" ");
	Cursor.set((COORD){37, (8 - elevator->current_floor) * 4 + 3 - elevator->quarter});
	printf(" ");
	Cursor.set((COORD){37, (8 - elevator->current_floor) * 4 + 4 - elevator->quarter});
	printf(" ");
	Cursor.set((COORD){36, (8 - elevator->current_floor) * 4 + 2 - elevator->quarter});
	printf(" ");
	Cursor.set((COORD){36, (8 - elevator->current_floor) * 4 + 3 - elevator->quarter});
	printf(" ");
	Cursor.set((COORD){36, (8 - elevator->current_floor) * 4 + 4 - elevator->quarter});
	printf(" ");
}

bool move_elevator_down(const elevator_t *elevator, const int LL) {
	for (int i = 1; i <= elevator->total_passenger; ++i) {
		if (elevator->passenger[i].destination == LL)
			return true;
	}
	return false;
}

bool stay_in_elevator(const elevator_t *elevator, const floor_t *floors, const int level, const int direction) {
	if ((direction == 1 && floors[level].total_up) || move_elevator_down(elevator, level) ||
			(direction == 2 && floors[level].total_down))
		return true;
	return false;
}

void emit_weight_warning(elevator_t *elevator, floor_t *floors) { infomation_panel(elevator, floors, 9, 0, 0); }

void people_in_elevator(elevator_t *elevator, floor_t *floors, const int state, const int level) {
	if (state == 1) {
		int i = 1;
		while (i <= floors[level].total_up) {
			if (elevator->total_weight + floors[level].up_passengers[i].weight <= max_weight) {
				elevator->total_passenger++;
				infomation_panel(elevator, floors, 5, level, i);
				elevator->passenger[elevator->total_passenger]											 = floors[level].up_passengers[i];
				elevator->passenger[elevator->total_passenger].in_elevator_timestamp = timer;
				elevator->total_weight += elevator->passenger[elevator->total_passenger].weight;
				int Point = i;
				floors[level].total_people--;
				if (Point != floors[level].total_up)
					while (Point < floors[level].total_up)
						floors[level].up_passengers[Point] = floors[level].up_passengers[Point + 1], Point++;
				floors[level].total_up--;
			} else {
				rules(6);
				emit_weight_warning(elevator, floors);
			}
			i++;
		}
	} else if (state == 2) {
		int i = 1;
		while (i <= floors[level].total_down) {
			if (elevator->total_weight + floors[level].down_passengers[i].weight <= max_weight) {
				elevator->total_passenger++;
				infomation_panel(elevator, floors, 6, level, i);
				elevator->passenger[elevator->total_passenger]											 = floors[level].down_passengers[i];
				elevator->passenger[elevator->total_passenger].in_elevator_timestamp = timer;
				elevator->total_weight += elevator->passenger[elevator->total_passenger].weight;
				int Point = i;
				floors[level].total_people--;
				if (Point != floors[level].total_down)
					while (Point < floors[level].total_down)
						floors[level].down_passengers[Point] = floors[level].down_passengers[Point + 1], Point++;
				floors[level].total_down--;
			} else {
				rules(6);
				emit_weight_warning(elevator, floors);
			}
			i++;
		}
	}
}

void passenger_panel(elevator_t *elevator, const int j) {
	total_passenger_info++;
	if (total_passenger_info >= 31) {
		for (int i = 3; i <= 30; ++i) {

			Cursor.set((COORD){130, i});

			printf("                                             ");
		}
		total_passenger_info = 3;
	}

	Cursor.set((COORD){131, total_passenger_info});

	printf("%-8s %-2dkg  %2d    %2d    %-3ds  %-3ds  %-3ds", elevator->passenger[j].name, elevator->passenger[j].weight,
				 elevator->passenger[j].position, elevator->passenger[j].destination, elevator->passenger[j].current_time,
				 elevator->passenger[j].in_elevator_timestamp, elevator->passenger[j].out_elevator_timestamp);
}

void people_out_elevator(elevator_t *elevator, floor_t *floors, const int LL) {
	int j = 1;
	while (j <= elevator->total_passenger) {
		if (elevator->passenger[j].destination == LL) {
			elevator->total_weight -= elevator->passenger[j].weight;
			elevator->passenger[j].out_elevator_timestamp = timer;
			int Point																			= j;
			infomation_panel(elevator, floors, 7, 0, j);
			passenger_panel(elevator, j);
			if (Point != elevator->total_passenger)
				while (Point < elevator->total_passenger)
					elevator->passenger[Point] = elevator->passenger[Point + 1], Point++;
			elevator->total_passenger--;
			j--;
		}
		j++;
	}
}

void tar_instruct(elevator_t *elevator, const floor_t *floors) {
	int Flag = 0;
	if (elevator->prev_state == 1) {
		for (int i = elevator->current_floor + 1; i <= all_height; ++i) {
			if (floors[i].total_up > 0 || (floors[i].total_down > 0 && i == all_height)) {
				elevator->prev_state		= 3;
				elevator->stay_time			= 0;
				elevator->current_state = 1;
				Flag										= 1;
				rules(2);
			}
		}
		if (not Flag) {
			for (int i = elevator->current_floor + 1; i <= all_height; ++i) {
				if (floors[i].total_down > 0) {
					elevator->prev_state		= 3;
					elevator->stay_time			= 0;
					elevator->current_state = 1;
					Flag										= 1;
					rules(2);
				}
			}
		}
		if (not Flag) {
			for (int i = elevator->current_floor - 1; i >= 1; --i) {
				if (floors[i].total_up > 0 || floors[i].total_down > 0) {
					elevator->prev_state		= 3;
					elevator->stay_time			= 0;
					elevator->current_state = 2;
					Flag										= 1;
					rules(3);
				}
			}
		}
	} else if (elevator->prev_state == 2) {
		for (int i = elevator->current_floor - 1; i >= 1; --i) {
			if (floors[i].total_down > 0 || (floors[i].total_up > 0 && i == 1)) {
				elevator->prev_state		= 3;
				elevator->stay_time			= 0;
				elevator->current_state = 2;
				Flag										= 1;
				rules(3);
			}
		}
		if (not Flag) {
			for (int i = elevator->current_floor - 1; i >= 1; --i) {
				if (floors[i].total_up > 0) {
					elevator->prev_state		= 3;
					elevator->stay_time			= 0;
					elevator->current_state = 2;
					Flag										= 1;
					rules(3);
				}
			}
		}
		if (not Flag) {
			for (int i = elevator->current_floor + 1; i <= all_height; ++i) {
				if (floors[i].total_up > 0 || floors[i].total_down > 0) {
					elevator->prev_state		= 3;
					elevator->stay_time			= 0;
					elevator->current_state = 1;
					Flag										= 1;
					rules(2);
				}
			}
		}
	}
	if (not Flag) {
		for (int i = 1; i <= elevator->total_passenger; ++i) {
			if (elevator->passenger[i].destination > elevator->current_floor) {
				elevator->prev_state		= 3;
				elevator->stay_time			= 0;
				elevator->current_state = 1;
				Flag										= 1;
				rules(4);
			} else if (elevator->passenger[i].destination < elevator->current_floor) {
				elevator->prev_state		= 3;
				elevator->stay_time			= 0;
				elevator->current_state = 2;
				Flag										= 1;
				rules(5);
			}
		}
	}
	if (not Flag)
		elevator->stay_time = 0;
}

void draw_people_in_elevator(const elevator_t *elevator) {

	Cursor.set((COORD){2, (8 - elevator->current_floor) * 4 + 3 - elevator->quarter});

	printf("                                  ");

	Cursor.set((COORD){2, (8 - elevator->current_floor) * 4 + 4 - elevator->quarter});

	printf("                                  ");
	for (int i = 1; i <= min(15, elevator->total_passenger); ++i) {

		Cursor.set((COORD){i * 2, (8 - elevator->current_floor) * 4 + 2 - elevator->quarter});

		printf("%d ", elevator->passenger[i].destination);
	}
	if (elevator->total_passenger > 15) {
		for (int i = 16; i <= min(20, elevator->total_passenger - 15); ++i) {

			Cursor.set((COORD){(i - 15) * 2, (8 - elevator->current_floor) * 4 + 3 - elevator->quarter});

			printf("%d ", elevator->passenger[i].destination);
		}
	}
	if (elevator->current_state == 1) {

		Cursor.set((COORD){34, (8 - elevator->current_floor) * 4 + 3 - elevator->quarter});

		printf("^");
	}
	if (elevator->current_state == 2) {

		Cursor.set((COORD){34, (8 - elevator->current_floor) * 4 + 3 - elevator->quarter});

		printf("v");
	}

	Cursor.set((COORD){2, (8 - elevator->current_floor) * 4 + 4 - elevator->quarter});

	printf("Count:%d", elevator->total_passenger);

	Cursor.set((COORD){19, (8 - elevator->current_floor) * 4 + 4 - elevator->quarter});

	printf("Weight:%dkg", elevator->total_weight);
}

void instruct_elevator(elevator_t *elevator, floor_t *floors) {
	if (elevator->current_state != 3) {
		elevator->prev_state = elevator->current_state;
	}
	Sleep(300);
	switch (elevator->current_state) {
	case 1:
		if (stay_in_elevator(elevator, floors, elevator->current_floor, 1) && elevator->quarter == 0 &&
				elevator->prev_state != 3) {
			elevator->current_state = 3;
			infomation_panel(elevator, floors, 8, 0, 0);
		} else if (elevator->quarter == 3) {
			elevator->current_floor++;
			elevator->quarter = 0;
			if (elevator->current_floor == all_height)
				elevator->current_state = 3, elevator->prev_state = 2;
		} else {
			elevator->quarter++;
		}
		draw_elevator(elevator);
		break;
	case 2:
		if (stay_in_elevator(elevator, floors, elevator->current_floor, 2) && elevator->quarter == 0 &&
				elevator->prev_state != 3) {
			elevator->current_state = 3;
			infomation_panel(elevator, floors, 8, 0, 0);
		} else if (elevator->quarter == 0 && elevator->current_floor == 1) {
			elevator->current_state = 3, elevator->prev_state = 1;
		} else if (elevator->quarter == 0) {
			elevator->current_floor--;
			elevator->quarter = 3;
		} else {
			elevator->quarter--;
		}
		draw_elevator(elevator);
		break;
	case 3:
		if (elevator->stay_time < max_waiting_time) {
			elevator->stay_time++;
			draw_elevator(elevator);
			door_open(elevator);
			if (elevator->stay_time == 1)
				rules(1);
			people_out_elevator(elevator, floors, elevator->current_floor);
			people_in_elevator(elevator, floors, elevator->prev_state, elevator->current_floor);
		} else {
			draw_elevator(elevator);
			tar_instruct(elevator, floors);
			door_close(elevator);
		}
		break;
	default:
		break;
	}
	draw_people_in_elevator(elevator);
}

void run_game_loop(elevator_t *elevator, floor_t *floors) {
	initialize(elevator, floors);
	current_people				= 0;
	total_people_in_floor = 0;
	timer									= 0;
	Console.get(&Terminal);

	// set font size
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 24;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	wcscpy(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(Terminal.hOutputConsole, FALSE, &cfi);
	memset(floors, 0, sizeof floors);
	while (current_people < total_people || total_people_in_floor > 0 || elevator->total_passenger > 0) {
		timer++;
		draw_people(floors);
		if (current_people < total_people) {
			gen_passenger(elevator, floors, timer);
		}
		total_people_in_floor = inspect_people(elevator, floors, timer);
		instruct_elevator(elevator, floors);
	}
}

status_t elevator_main() {
	ShowWindow(GetForegroundWindow(), SW_MAXIMIZE);
	time_t timep;
	time(&timep);
	val				time_ptr = localtime(&timep);
	smart var floors	 = (floor_t *)calloc(sizeof(floor_t), 10);
	smart var elevator = alloc(elevator_t, 1);
	*elevator					 = (elevator_t){.total_passenger = 0,
																		.total_weight		 = 0,
																		.current_floor	 = 1,
																		.quarter				 = 0,
																		.stay_time			 = 0,
																		.prev_state			 = 1,
																		.current_state	 = 3};
	hour = time_ptr->tm_hour, minute = time_ptr->tm_min, second = time_ptr->tm_sec;
	Console.set_color(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, 0);
	srand(time(nullptr));
	show_menu();
	_getch();
	Console.set_cursor_state(&Terminal, false);
	Console.set_cursor_state(&Terminal, false);
	Console.set_cursor_state(&Terminal, false);
	run_game_loop(elevator, floors);
	Cursor.set((COORD){37, 36});
	println("simulation finished.");
	return 0;
}
