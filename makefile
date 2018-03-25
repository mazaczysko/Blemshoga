DEBUG =
CDEBUG = -g -fno-builtin
CFLAGS = -Wall

ifneq ($(DEBUG),)
CFLAGS += $(CDEBUG)
endif

LIBS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lm
SRC ?= src/*.c
all:
	gcc -o blemshoga $(SRC) $(LIBS) $(CFLAGS)
