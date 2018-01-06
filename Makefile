CC = gcc
CC_FLAGS = -g

QDBMP_PATH = qdbmp_1.0.0
INC = $(QDBMP_PATH)
OUT = out

all: dependencies generate_testcase \
	sequential test_sequential \
	basic_parallel test_basic_parallel \
	fsa_parallel test_fsa_parallel

sequential: dependencies
	$(CC) $(CC_FLAGS) main.c sequential_operations.c image_model.c -o $(OUT)/sequential -I$(INC) -L$(OUT) -lqdbmp

basic_parallel: dependencies
	$(CC) $(CC_FLAGS) main.c basic_parallel_operations.c image_model.c -o $(OUT)/basic_parallel -I$(INC) -L$(OUT) -lqdbmp -fopenmp

fsa_parallel: dependencies
	$(CC) $(CC_FLAGS) main.c fsa_parallel_operations.c image_model.c -o $(OUT)/fsa_parallel -I$(INC) -L$(OUT) -lqdbmp -fopenmp

test_sequential: sequential test_data
	$(CC) $(CC_FLAGS) test.c image_model.c sequential_operations.c -o $(OUT)/test_sequential -I$(INC) -L$(OUT) -lqdbmp

test_basic_parallel: basic_parallel test_data
	$(CC) $(CC_FLAGS) test.c image_model.c basic_parallel_operations.c -o $(OUT)/test_basic_parallel -I$(INC) -L$(OUT) -lqdbmp -fopenmp

test_fsa_parallel: fsa_parallel test_data
	$(CC) $(CC_FLAGS) test.c image_model.c fsa_parallel_operations.c -o $(OUT)/test_fsa_parallel -I$(INC) -L$(OUT) -lqdbmp -fopenmp

generate_testcase: dependencies
	$(CC) $(CC_FLAGS) generate_testcase.c image_model.c -o $(OUT)/generate_testcase -I$(INC) -L$(OUT) -lqdbmp

test_data: generate_testcase
	test -f `pwd`/test_data/2048.model || ./out/generate_testcase 2048 2048 `pwd`/test_data/2048.model
	test -f `pwd`/test_data/4096.model || ./out/generate_testcase 4096 4096 `pwd`/test_data/4096.model
	test -f `pwd`/test_data/8192.model || ./out/generate_testcase 8192 8192 `pwd`/test_data/8192.model
	test -f `pwd`/test_data/16384.model || ./out/generate_testcase 16384 16384 `pwd`/test_data/16384.model
	test -f `pwd`/test_data/32768.model || ./out/generate_testcase 32768 32768 `pwd`/test_data/32768.model
	test -f `pwd`/test_data/48000.model || ./out/generate_testcase 48000 48000 `pwd`/test_data/48000.model
	test -f `pwd`/test_data/56000.model || ./out/generate_testcase 56000 56000 `pwd`/test_data/56000.model

dependencies: out_dir qdbmp

out_dir:
	@mkdir -p out

qdbmp:
	$(CC) $(QDBMP_PATH)/qdbmp.c -c -o $(OUT)/qdbmp.o
	ar rcs $(OUT)/libqdbmp.a $(OUT)/qdbmp.o

