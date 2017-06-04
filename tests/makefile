CC=gcc
CFLAGS=-g -Wall -O0
LIBRARY_PATH=../src/
FILES= memoryBuffersExample.c ../src/bufferMemory.c
TARGET = memoryBuffersExample

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(FILES)

clean:
	rm -f *.o core
	rm $(TARGET)
