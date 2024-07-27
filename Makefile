all:
	gcc -o main  main.c disamb.c emulator.c font.c -lraylib

clean:
	rm main