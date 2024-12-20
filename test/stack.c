#include <net/ancillarycat/viva/utils/console.h>
#include <net/ancillarycat/viva/utils/cursor.h>
#include <net/ancillarycat/viva/viva.h>
#include <net/ancillarycat/viva/vtest.h>

#include "../projects/maze/maze.h"
#include "../projects/maze/maze_stack.h"


TEST_SETUP()

TEST(create, stack) {
	smart_stack struct maze_stack *stack = create_maze_stack(10);
	EXPECT_TRUE(stack);
}

TEST(push, stack) {
	smart_stack struct maze_stack *stack = create_maze_stack(10);
	push_maze_stack(stack, (COORD){1, 2});
	EXPECT_EQ(top_maze_stack(stack).X, 1);
	EXPECT_EQ(top_maze_stack(stack).Y, 2);
}

TEST(pop, stack) {
	smart_stack struct maze_stack *stack = create_maze_stack(10);
	push_maze_stack(stack, (COORD){1, 2});
	push_maze_stack(stack, (COORD){3, 4});
	EXPECT_EQ(pop_maze_stack(stack).X, 3);
	EXPECT_EQ(pop_maze_stack(stack).Y, 4);
}

TEST(top, stack) {
	smart_stack struct maze_stack *stack = create_maze_stack(10);
	push_maze_stack(stack, (COORD){1, 2});
	push_maze_stack(stack, (COORD){3, 4});
	EXPECT_EQ(top_maze_stack(stack).X, 3);
	EXPECT_EQ(top_maze_stack(stack).Y, 4);
}
