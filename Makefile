CXX=g++
#CXXFLAGS=-pedantic -g -O3 -march=native
CXXFLAGS=-pedantic -ggdb -O0
OBJS=BridgeCraft.o NeHeGL.o ArcBall.o Bridge.o Edge.o Node.o Vec2.o Level.o Level1.o Level2.o Number.o
LIBS=-L/usr/X11R6/lib -lGL -lGLU -lXxf86vm

all: bridgecraft

bridgecraft: $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS)

clean:
	rm -f $(OBJS)

