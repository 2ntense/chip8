make_chip8:
	mkdir -p build/; \
	gcc -o build/chip8 chip8.c -lSDL2

run:
	build/chip8

dl_roms:
	mkdir -p roms/ tmp/; \
	cd tmp/; \
	git clone https://github.com/stianeklund/chip8.git; \
	cp chip8/roms/*.ch8 ../roms

clean:
	rm -rf roms/ build/