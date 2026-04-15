CC     = gcc
CFLAGS = -Wall -O2
LIBS   = -lGL -lGLU -lglut -lm
SRC    = $(wildcard src/*.c)
TARGET = ninja_shadow

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
