all:
	gcc -o /usr/local/bin/wt-move wt-move.c
	gcc -o /usr/local/bin/morph-controller morph-controller.c
	gcc -o /usr/local/bin/syx2raw syx2raw.c
	gcc -o /usr/local/bin/wav2pro3 wav2pro3.c

