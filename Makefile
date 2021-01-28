DEST = /usr/local/bin

all:
	@gcc -o $(DEST)/wt-move wt-move.c
	@gcc -o $(DEST)/morph-controller morph-controller.c
	@gcc -o $(DEST)/syx2raw syx2raw.c
	@gcc -o $(DEST)/wav2pro3 wav2pro3.c
	@gcc -o $(DEST)/serum2pro3 serum2pro3.c
	@gcc -o $(DEST)/split2pro3 split2pro3.c
	@echo "Done!"
