#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/net/ancillarycat/viva/viva.h"

#if __STDC_VERSION__ >= 202000L
constexpr val MAX_STUDENTS = 1000;
constexpr val MAX_STRING	 = 100;
#else
#define MAX_STUDENTS 1000
#define MAX_STRING 100
#endif

typedef struct _student			 Student;
typedef struct _student_data StudentData;

void load_csv(const char *filename, StudentData *data);
void save_csv(const char *filename, StudentData *data);
void display_students(StudentData *data);
void display_as_tree(StudentData *data);
void add_student(StudentData *data);
int	 is_not_unique(StudentData *data, const char *studentID);
void delete_student(StudentData *data, const char *studentID);
void query_student(StudentData *data, const char *studentID);
void modify_student(StudentData *data, char *student_ID);
void push_history(Student *student);
void pop_history(StudentData *data);

typedef struct _student {
	char id[MAX_STRING];
	char name[MAX_STRING];
	char major[MAX_STRING];
	char department[MAX_STRING];
	char course[MAX_STRING];
} Student;

typedef struct _student_data {
	Student *students;
	size_t	 count;
} StudentData;

struct _history_student {
	struct _history_student *prev;
	struct _student					*change;
};

typedef struct _history {
	struct _history_student *head;
} History;

var history = (History*)nullptr;

int main(int argc, char **argv, char **envp) {
	char *file_path = nullptr;
	if (argc < 2)
		file_path = R"(.\data.csv)";
	else
		file_path = *(argv + 1);
	var _students_ = alloc(Student, MAX_STUDENTS);
	var				data			 = (StudentData){
								.students = _students_,
								.count		= 0,
	};

	char* studentID;

	load_csv(file_path, &data);

	while (true) {
		print("\n--- Student Management System ---\n");
		print("1. Display all students\n");
		print("2. Display students as tree\n");
		print("3. Add a student\n");
		print("4. Delete a student\n");
		print("5. Modify a student\n");
		print("6. Query a student\n");
		print("7. Save and Exit\n");
		print("8. Revert the last change\n");
		print("Enter your choice: ");

		int choice = get_rec(choice);

		switch (choice) {
		case 1:
			display_students(&data);
			break;
		case 2:
			display_as_tree(&data);
			break;
		case 3:
			add_student(&data);
			break;
		case 4:
			printf("Enter Student ID to delete: ");
			studentID = get_rec(Any.char_ptr_type, MAX_STRING);
			delete_student(&data, studentID);
			break;
		case 5:
			printf("Enter the Student ID to modify: ");
			studentID = get_rec(Any.char_ptr_type, MAX_STRING);
			modify_student(&data, studentID);
			break;
		case 6:
			printf("Enter Student ID to query: ");
			studentID = get_rec(Any.char_ptr_type, MAX_STRING);
			query_student(&data, studentID);
			break;
		case 7:
			save_csv(file_path, &data);
			printf("Data saved to %s. Exiting...\n", file_path);
			return 0;
		case 8:
			printf("Revert the last change\n");
			if (history == nullptr) {
				printf("No history found.\n");
			} else {
				pop_history(&data);
				printf("Last change reverted.\n");
			}

		default:
			printf("Invalid choice. Try again.\n");
		}
	}
}


void load_csv(const char *filename, StudentData *data) {
	val file = fopen(filename, "r");
	if (not file) {
		printf("No existing file found. Starting fresh.\n");
		return;
	}

	char line[1024];
	while (fgets(line, sizeof(line), file)) {
		if (strstr(line, "studentid"))
			continue;

		sscanf(line, "%[^,],%[^,],%[^,],%[^,],%s", data->students[data->count].id, data->students[data->count].name,
					 data->students[data->count].major, data->students[data->count].department,
					 data->students[data->count].course);
		data->count++;
	}
	fclose(file);
	printf("Loaded %d students from file.\n", data->count);
}


void save_csv(const char *filename, StudentData *data) {
	val file = fopen(filename, "w");
	if (not file) {
		printf("Error saving file.\n");
		return;
	}


	fprintf(file, "studentid,studentname,major,department,course\n");
	for (size_t i = 0; i < data->count; i++) {
		fprintf(file, "%s,%s,%s,%s,%s\n", data->students[i].id, data->students[i].name, data->students[i].major,
						data->students[i].department, data->students[i].course);
	}
	fclose(file);
}


void display_students(StudentData *data) {
	printf("\n%-15s %-20s %-15s %-15s %-15s\n", "StudentID", "StudentName", "Major", "Department", "Class");
	printf("------------------------------------------------------------------------\n");
	for (size_t i = 0; i < data->count; i++) {
		printf("%-15s %-20s %-15s %-15s %-15s\n", data->students[i].id, data->students[i].name, data->students[i].major,
					 data->students[i].department, data->students[i].course);
	}
}


int is_not_unique(StudentData *data, const char *studentID) {
	for (size_t i = 0; i < data->count; i++) {
		if (strcmp(data->students[i].id, studentID) == 0)
			return 1;

	}
	return 0;
}


void add_student(StudentData *data) {
	if (data->count >= MAX_STUDENTS) {
		printf("Cannot add more students. Storage full!\n");
		return;
	}

	char* studentID;
	printf("Enter Student ID: ");
	studentID = get_rec(Any.char_ptr_type, MAX_STRING);

	if (is_not_unique(data, studentID)) {
		printf("Error: Student ID already exists!\n");
		return;
	}
	strcpy(data->students[data->count].id, studentID);
	printf("Enter Student Name: ");
	scanf(" %[^\n]s", data->students[data->count].name);
	printf("Enter Major: ");
	scanf("%s", data->students[data->count].major);
	printf("Enter Department: ");
	scanf("%s", data->students[data->count].department);
	printf("Enter Class: ");
	scanf("%s", data->students[data->count].course);

	push_history(&data->students[data->count]);

	data->count++;

	printf("Student added successfully.\n");
}


void delete_student(StudentData *data, const char *studentID) {
	for (size_t i = 0; i < data->count; i++) {
		if (strcmp(data->students[i].id, studentID) == 0) {
			push_history(&data->students[i]);
			for (size_t j = i; j < data->count - 1; j++) {
				data->students[j] = data->students[j + 1];
			}
			data->count--;
			printf("Student with ID %s deleted successfully.\n", studentID);
			return;
		}
	}
	printf("Student with ID %s not found.\n", studentID);
}


void query_student(StudentData *data, const char *studentID) {
	for (size_t i = 0; i < data->count; i++) {
		if (strcmp(data->students[i].id, studentID) == 0) {
			printf("\nStudent Found:\n");
			printf("StudentID: %s\n", data->students[i].id);
			printf("StudentName: %s\n", data->students[i].name);
			printf("Major: %s\n", data->students[i].major);
			printf("Department: %s\n", data->students[i].department);
			printf("Class: %s\n", data->students[i].course);
			return;
		}
	}
	printf("Student with ID %s not found.\n", studentID);
}
void modify_student(StudentData *data, char *student_ID) {
	for (size_t i = 0; i < data->count; i++) {
		push_history(&data->students[i]);
		if (strcmp(data->students[i].id, student_ID) == 0) {
			printf("Enter new Student Name: ");
			scanf(" %[^\n]s", data->students[i].name);
			printf("Enter new Major: ");
			scanf("%s", data->students[i].major);
			printf("Enter new Department: ");
			scanf("%s", data->students[i].department);
			printf("Enter new Class: ");
			scanf("%s", data->students[i].course);
			printf("Student with ID %s modified successfully.\n", student_ID);
			return;
		}
	}
}

void push_history(Student *student) {
	if (not history) {
		history								= (History *)malloc(sizeof(History));
		history->head					= (struct _history_student *)malloc(sizeof(struct _history_student));
		history->head->change = student;
		history->head->prev		= nullptr;
	} else {
		struct _history_student *newNode = (struct _history_student *)malloc(sizeof(struct _history_student));
		newNode->change									 = student;
		newNode->prev										 = history->head;
		history->head										 = newNode;
	}
}
void pop_history(StudentData *data) {
	if (not history or not history->head)
		return;
	// if has history, if find the same id but not same data, then replace the data; if data all the same, then delete it
	// if not find the same id, then add the data
	for (size_t i = 0; i < data->count; i++) {
		if (strcmp(data->students[i].id, history->head->change->id) == 0) {
			if (strcmp(data->students[i].name, history->head->change->name) == 0 &&
					strcmp(data->students[i].major, history->head->change->major) == 0 &&
					strcmp(data->students[i].department, history->head->change->department) == 0 &&
					strcmp(data->students[i].course, history->head->change->course) == 0) {
				printf("Reverting the add of student with ID %s...\n", history->head->change->id);
				data->students[i] = data->students[--data->count];
				var temp = history->head;
				history->head = history->head->prev;
				free(temp);

				return;
			} else {
				printf("Reverting the change of student with ID %s...\n", history->head->change->id);
				data->students[i] = *history->head->change;

				var temp = history->head;
				history->head = history->head->prev;
				free(temp);
				return;
			}
		}
	}
	// if not find the same id, then add the data
	printf("Reverting the delete of student with ID %s...\n", history->head->change->id);
	data->students[data->count++] = *history->head->change;
	var temp = history->head;
	history->head = history->head->prev;
	free(temp);
}

void display_as_tree(StudentData *data) {
	printf("\n--- Students Displayed as Tree ---\n");


	char printedDepartments[MAX_STUDENTS][MAX_STRING];
	int	 depCount = 0;

	for (size_t i = 0; i < data->count; i++) {

		int depPrinted = 0;
		for (int d = 0; d < depCount; d++) {
			if (strcmp(data->students[i].department, printedDepartments[d]) == 0) {
				depPrinted = 1;
				break;
			}
		}
		if (depPrinted)
			continue;


		printf("%s\n", data->students[i].department);
		strcpy(printedDepartments[depCount++], data->students[i].department);


		char printedMajors[MAX_STUDENTS][MAX_STRING];
		int	 majCount = 0;

		for (size_t j = 0; j < data->count; j++) {
			if (strcmp(data->students[j].department, data->students[i].department) == 0) {

				int majPrinted = 0;
				for (int m = 0; m < majCount; m++) {
					if (strcmp(data->students[j].major, printedMajors[m]) == 0) {
						majPrinted = 1;
						break;
					}
				}
				if (majPrinted)
					continue;


				printf("|---- %s\n", data->students[j].major);
				strcpy(printedMajors[majCount++], data->students[j].major);


				char printedCourses[MAX_STUDENTS][MAX_STRING];
				int	 courseCount = 0;

				for (size_t k = 0; k < data->count; k++) {
					if (strcmp(data->students[k].department, data->students[i].department) == 0 &&
							strcmp(data->students[k].major, data->students[j].major) == 0) {

						int coursePrinted = 0;
						for (int c = 0; c < courseCount; c++) {
							if (strcmp(data->students[k].course, printedCourses[c]) == 0) {
								coursePrinted = 1;
								break;
							}
						}
						if (coursePrinted)
							continue;


						printf("|     |---- %s\n", data->students[k].course);
						strcpy(printedCourses[courseCount++], data->students[k].course);


						for (size_t l = 0; l < data->count; l++) {
							if (strcmp(data->students[l].department, data->students[i].department) == 0 &&
									strcmp(data->students[l].major, data->students[j].major) == 0 &&
									strcmp(data->students[l].course, data->students[k].course) == 0) {
								printf("|     |    |---- %s (%s)\n", data->students[l].name, data->students[l].id);
							}
						}
					}
				}
			}
		}
	}
}
