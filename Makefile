CC = gcc
CC_FLAGS = -g

QDBMP_PATH = qdbmp_1.0.0
INC = $(QDBMP_PATH)
OUT = out

all: dependencies sequential test

sequential: dependencies
	$(CC) $(CC_FLAGS) sequential.c sequential_operations.c image_model.c -o $(OUT)/sequential -I$(INC) -L$(OUT) -lqdbmp

test: sequential
	$(CC) $(CC_FLAGS) test.c image_model.c sequential_operations.c -o $(OUT)/test -I$(INC) -L$(OUT) -lqdbmp

dependencies: out_dir qdbmp

out_dir:
	@mkdir -p out

qdbmp:
	$(CC) $(QDBMP_PATH)/qdbmp.c -c -o $(OUT)/qdbmp.o
	ar rcs $(OUT)/libqdbmp.a $(OUT)/qdbmp.o

