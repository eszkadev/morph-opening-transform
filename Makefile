CC = gcc
CC_FLAGS = -g -lrt

QDBMP_PATH = qdbmp_1.0.0
INC = $(QDBMP_PATH)
OUT = out
LIBRARY = -lqdbmp -fopenmp

all: dependencies generate_testcase \
	sequential test_sequential \
	basic_parallel test_basic_parallel

sequential: dependencies
	$(CC) $(CC_FLAGS) main.c sequential_operations.c image_model.c -o $(OUT)/sequential -I$(INC) -L$(OUT) $(LIBRARY)

test_sequential: sequential test_data
	$(CC) $(CC_FLAGS) test.c image_model.c sequential_operations.c -o $(OUT)/test_sequential -I$(INC) -L$(OUT) $(LIBRARY)

basic_parallel: dependencies
	$(CC) $(CC_FLAGS) main.c basic_parallel_operations.c image_model.c -o $(OUT)/basic_parallel -I$(INC) -L$(OUT) $(LIBRARY)

test_basic_parallel: basic_parallel test_data
	$(CC) $(CC_FLAGS) test.c image_model.c basic_parallel_operations.c -o $(OUT)/test_basic_parallel -I$(INC) -L$(OUT) $(LIBRARY)

generate_testcase: dependencies
	$(CC) $(CC_FLAGS) generate_testcase.c image_model.c -o $(OUT)/generate_testcase -I$(INC) -L$(OUT) $(LIBRARY)

test_data: generate_testcase
	test -f `pwd`/test_data/8192.model || ./out/generate_testcase 8192 8192 `pwd`/test_data/8192.model
	test -f `pwd`/test_data/32768.model || ./out/generate_testcase 32768 32768 `pwd`/test_data/32768.model

dependencies: out_dir qdbmp

out_dir:
	@mkdir -p out
	@mkdir -p test_data

qdbmp:
	$(CC) $(QDBMP_PATH)/qdbmp.c -c -o $(OUT)/qdbmp.o
	ar rcs $(OUT)/libqdbmp.a $(OUT)/qdbmp.o

