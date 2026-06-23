bin/mash-debug: mash.c
	mkdir -p bin
	clang -g -o bin/mash-debug mash.c

bin/mash-release: mash.c
	mkdir -p bin
	clang -O3 -o bin/mash-release mash.c

clean:
	rm -r bin

install: bin/mash-release
	sudo cp bin/mash-release /usr/local/bin/mash

uninstall:
	sudo rm /usr/local/bin/mash
