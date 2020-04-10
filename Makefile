CC = gcc
CFLAGS = -Werror
LDFLAGS = -lSDL2
TARGET = chip8
BIN_PATH = ./bin
SRC_PATH = ./src

all: $(TARGET)

$(TARGET): *.o
	mkdir -p $(BIN_PATH);\
	$(CC) $(CFLAGS) -o $(BIN_PATH)/$(TARGET) $(wildcard $(SRC_PATH)/*.o) $(LDFLAGS)

*.o:
	cd $(SRC_PATH);\
	$(CC) $(CFLAGS) -c *.c
	

debug:
	cd $(SRC_PATH); \
	$(CC) -g $(CFLAGS) -c *.c; \
	cd ..; \
	mkdir -p $(BIN_PATH); \
	$(CC) -g $(CFLAGS) -o $(BIN_PATH)/$(TARGET) $(wildcard $(SRC_PATH)/*.o) $(LDFLAGS)

debug2:
	$(MAKE) debug
	
	
run:
	./bin/chip8 $(PROGRAM)

dl_roms:
	mkdir -p roms/ tmp/; \
	cd tmp/; \
	git clone https://github.com/stianeklund/chip8.git; \
	cp chip8/roms/*.ch8 ../roms; \
	cd ..; \
	rm -rf ./tmp

clean:
	rm -rf roms/ bin/ src/*.o