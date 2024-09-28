#include "include/net/ancillarycat/viva/viva.h"
#include "include/net/ancillarycat/viva/generic/forward_list.h"


void test();

instantiate_forward_list_of(int)

int main() {
	var i = 0;
	println("input the number of node in the forward list:");
	fflush(stdout);
	while (true) {
		i = get_rec(Any.int_type);
		if (i <= 0) {
			println("The number of node should be greater than 0.");
			fflush(stdout);
		}
		break;
	}
	var my_forward_list = Forward_list_of_int.new_header();

	while (i--)
		Forward_list_of_int.push_back(my_forward_list, get_rec(Any.int_type));

	Forward_list_of_int.unique(my_forward_list);

	var it = Forward_list_of_int.begin(my_forward_list);
	println("Unique forward list:");
	while (it) {
		printf("%d ", *(int*)it->data_ptr);
		it = it->next;
	}

	println("\nThen reverse the forward list:");
	Forward_list_of_int.reverse(my_forward_list);
	it = Forward_list_of_int.begin(my_forward_list);
	while (it) {
		printf("%d ", *(int*)it->data_ptr);
		it = it->next;
	}
}

void test() {
	printf("Enter an integer: ");
	int int_value = get_rec(int_value);
	printf("You entered: %d\n", int_value);

	printf("Enter a character: ");
	char char_value = get_rec(char_value);
	printf("You entered: %c\n", char_value);

	printf("Enter a string: ");
	const char* string_value = get_rec(string_value);
	printf("You entered: %s\n", string_value);
}
