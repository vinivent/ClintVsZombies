all:
	gcc ./src/*.c -I./include -o ClintVsZombies.out -lm

run:
	./ClintVsZombies.out

clean:
	rm ClintVsZombies.out