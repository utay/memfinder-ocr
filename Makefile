CC=gcc
CPPFLAGS= `pkg-config --cflags sdl`
CFLAGS= -Wall -Wextra -std=c99 -g -D_DEFAULT_SOURCE -D_BSD_SOURCE -pedantic
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image #-Wl,-Bstatic `sdl-config --static-libs` -lSDL_image -Wl,-Bdynamic

SRC_OCR= src/detection.c src/preprocessing.c src/pixel_operations.c src/types.c src/main.c src/classification.c src/stack.c src/neural_network.c
OBJ_OCR= ${SRC_OCR:.c=.o}
DEP_OCR= ${SRC_OCR:.c=.d}

SRC_LEARN= src/pixel_operations.c src/classification.c src/neural_network.c src/learning.c src/stack.c src/preprocessing.c src/types.c src/detection.c
OBJ_LEARN= ${SRC_LEARN:.c=.o}
DEP_LEARN= ${SRC_LEARN:.c=.d}

all: ocr learn

ocr: $(OBJ_OCR)
	$(CC) $(OBJ_OCR) -o ocr $(LDLIBS)

learn: $(OBJ_LEARN)
	$(CC) $(OBJ_LEARN) -o learn $(LDLIBS)

clean:
	find . -name \*~ -exec rm \-fv {} \;
	find . -name \*.o -exec rm \-fv {} \;
	find . -name \*.d -exec rm \-fv {} \;
	rm -f preprocessing ocr learn
