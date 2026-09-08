# this is an example assuming you have SDL3 installed on the system
all: main.cpp# this is a target that depends on main.cpp
	g++ -o rand main.cpp `pkg-config --cflags --libs sdl3`
# this one specifically works on linux

clean:
	rm -f rand