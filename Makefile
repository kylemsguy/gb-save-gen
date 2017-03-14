all: GBSaveGen

GBSaveGen: GBSaveGen.c GBSaveGen.h
	gcc -o GBSaveGen --std=c99 GBSaveGen.c

clean:
	rm -f GBSaveGen
