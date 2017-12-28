CC = gcc
CC_FLAGS = -g

QDBMP_PATH = qdbmp_1.0.0
INC = $(QDBMP_PATH)
OUT = out

all: dependencies sequential basic_parallel test_sequential test_basic_parallel

sequential: dependencies
	$(CC) $(CC_FLAGS) main.c sequential_operations.c image_model.c -o $(OUT)/sequential -I$(INC) -L$(OUT) -lqdbmp

basic_parallel: dependencies
	$(CC) $(CC_FLAGS) main.c basic_parallel_operations.c image_model.c -o $(OUT)/basic_parallel -I$(INC) -L$(OUT) -lqdbmp

test_sequential: sequential
	$(CC) $(CC_FLAGS) test.c image_model.c sequential_operations.c -o $(OUT)/test_sequential -I$(INC) -L$(OUT) -lqdbmp

test_basic_parallel: basic_parallel
	$(CC) $(CC_FLAGS) test.c image_model.c basic_parallel_operations.c -o $(OUT)/test_basic_parallel -I$(INC) -L$(OUT) -lqdbmp

dependencies: out_dir qdbmp

out_dir:
	@mkdir -p out

qdbmp:
	$(CC) $(QDBMP_PATH)/qdbmp.c -c -o $(OUT)/qdbmp.o
	ar rcs $(OUT)/libqdbmp.a $(OUT)/qdbmp.o

