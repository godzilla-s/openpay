# 编译 
CC=gcc
CFLAG=
PHOME=$(shell cd ..; pwd)
TARGET=libcommon.a 

LINK_OBJ=datapool.o log.o tries.o 

.PHONY:$(TARGET)

all:$(TARGET)
%.o:%.c
	$(CC) -c $< -I$(INC_PATH)

libcommon.a:$(LINK_OBJ)
	@echo "cpmplie libcommon.a"
	ar rv $@ $(LINK_OBJ)

clean:
	$(RM) $(LINK_OBJ) $(TARGET)

