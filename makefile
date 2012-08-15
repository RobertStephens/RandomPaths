BINARY_TARGETS := wave random_paths

all: $(BINARY_TARGETS)

clean:
	rm $(BINARY_TARGETS)

random_paths: random_paths.cpp
	g++ random_paths.cpp -o random_paths `sdl-config --libs`

wave: wave.cpp
	g++ wave.cpp -o wave `sdl-config --libs`





