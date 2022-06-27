DIR_INC = ./include
DIR_SRC = ./src

SRC = $(wildcard ${DIR_SRC}/*.c)
OBJ = $(patsubst %.c, ./%.o, ${notdir ${SRC}})

TARGET = ipmi-fru-parser

CFLAGS = -g -Wall -I${DIR_INC} -static

${TARGET}:${OBJ}
	${CC} ${OBJ} -o $@

%.o: ${DIR_SRC}/%.c
	${CC} ${CFLAGS} -c $< -o $@

.PHONY: clean
clean:
	rm -rf ./*.o ${TARGET}
