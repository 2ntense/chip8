CC = gcc
CFLAGS = -Werror
LDFLAGS = -lSDL2
OBJFILES = chip8.o screen.o input.o main.o
TARGET = chip8
BUILD_PATH = ./build
BIN_PATH = ./bin
SRC_PATH = ./src

all: $(TARGET)

$(TARGET): $(OBJFILES)
	mkdir -p $(BUILD_PATH); \
	$(CC) $(CFLAGS) -o $(BUILD_PATH)/$(TARGET) $(OBJFILES:%=$(SRC_PATH)/%) $(LDFLAGS)

chip8.o: $(SRC_PATH)/chip8.c $(SRC_PATH)/chip8.h
	cd $(SRC_PATH); \
	$(CC) $(CFLAGS) -c chip8.c

input.o: $(SRC_PATH)/input.c $(SRC_PATH)/input.h
	cd $(SRC_PATH); \
	$(CC) $(CFLAGS) -c input.c

screen.o: $(SRC_PATH)/screen.c $(SRC_PATH)/screen.h
	cd $(SRC_PATH); \
	$(CC) $(CFLAGS) -c screen.c

main.o:
	cd $(SRC_PATH); \
	$(CC) $(CFLAGS) -c main.c

debug:
	cd $(SRC_PATH); \
	$(CC) -g $(CFLAGS) -c chip8.c; \
	$(CC) -g $(CFLAGS) -c input.c; \
	$(CC) -g $(CFLAGS) -c screen.c; \
	$(CC) -g $(CFLAGS) -c main.c; \
	cd ..; \
	mkdir -p $(BUILD_PATH); \
	$(CC) -g $(CFLAGS) -o $(BUILD_PATH)/$(TARGET) $(OBJFILES:%=$(SRC_PATH)/%) $(LDFLAGS)
	
run:
	./build/chip8 $(PROGRAM)

dl_roms:
	mkdir -p roms/ tmp/; \
	cd tmp/; \
	git clone https://github.com/stianeklund/chip8.git; \
	cp chip8/roms/*.ch8 ../roms; \
	cd ..; \
	rm -rf ./tmp

clean:
	rm -rf roms/ build/ src/*.o