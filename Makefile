# Flags for linking to Mac OS X frameworks only.
CC = g++
CFLAGS = -Wall -ggdb -std=c++0x
LIBS = -framework OpenGL -lpng -lglfw3 -lglew

gldemo: *.cc *.h
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) -o furdemo *.cc

clean:
	rm -f furdemo
	rm -rf furdemo.dSYM
