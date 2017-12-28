CC = gcc
CC_FLAGS = -g

QDBMP_PATH = qdbmp_1.0.0
INC = $(QDBMP_PATH)
OUT = out

all: dependencies sequential basic_parallel test_sequential test_basic_parallel generate_testcase

sequential: dependencies
	$(CC) $(CC_FLAGS) main.c sequential_operations.c image_model.c -o $(OUT)/sequential -I$(INC) -L$(OUT) -lqdbmp

basic_parallel: dependencies
	$(CC) $(CC_FLAGS) main.c basic_parallel_operations.c image_model.c -o $(OUT)/basic_parallel -I$(INC) -L$(OUT) -lqdbmp -fopenmp

test_sequential: sequential test_data
	$(CC) $(CC_FLAGS) test.c image_model.c sequential_operations.c -o $(OUT)/test_sequential -I$(INC) -L$(OUT) -lqdbmp

test_basic_parallel: basic_parallel test_data
	$(CC) $(CC_FLAGS) test.c image_model.c basic_parallel_operations.c -o $(OUT)/test_basic_parallel -I$(INC) -L$(OUT) -lqdbmp -fopenmp

generate_testcase: dependencies
	$(CC) $(CC_FLAGS) generate_testcase.c image_model.c -o $(OUT)/generate_testcase -I$(INC) -L$(OUT) -lqdbmp

test_data: generate_testcase
	test -f `pwd`/test_data/8096.bmp || ./out/generate_testcase 8096 8096 `pwd`/test_data/8096.bmp

dependencies: out_dir qdbmp

out_dir:
	@mkdir -p out

qdbmp:
	$(CC) $(QDBMP_PATH)/qdbmp.c -c -o $(OUT)/qdbmp.o
	ar rcs $(OUT)/libqdbmp.a $(OUT)/qdbmp.o

