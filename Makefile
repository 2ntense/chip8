CC = gcc
CFLAGS = -Werror
LDFLAGS = -lSDL2
OBJFILES = chip8.o
TARGET = chip8
BUILD_PATH = ./build
BIN_PATH = ./bin
SRC_PATH = ./src

all: $(TARGET)

$(TARGET): $(OBJFILES)
	mkdir -p $(BUILD_PATH); \
	$(CC) $(CFLAGS) -o $(BUILD_PATH)/$(TARGET) $(SRC_PATH)/$(OBJFILES) $(LDFLAGS)

chip8.o: $(SRC_PATH)/chip8.c $(SRC_PATH)/chip8.h
	cd $(SRC_PATH); \
	$(CC) $(CFLAGS) -c chip8.c

run:
	./build/chip8

dl_roms:
	mkdir -p roms/ tmp/; \
	cd tmp/; \
	git clone https://github.com/stianeklund/chip8.git; \
	cp chip8/roms/*.ch8 ../roms; \
	cd ..; \
	rm -rf ./tmp

clean:
	rm -rf roms/ build/ src/*.o