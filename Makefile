# Flags for linking to Mac OS X frameworks only.
CC = g++
CFLAGS = -Wall -ggdb -std=c++0x
RELEASE_CFLAGS = -Wall -std=c++0x -O3
LIBS = -framework OpenGL -lpng -lglfw3 -lglew

gldemo: *.cc *.h
	$(CC) $(CFLAGS) $(LIBS) -o furdemo *.cc

gldemo-release: *.cc *.h
	$(CC) $(RELEASE_CFLAGS) $(LIBS) -o furdemo *.cc
	

clean:
	rm -f furdemo
	rm -rf furdemo.dSYM
