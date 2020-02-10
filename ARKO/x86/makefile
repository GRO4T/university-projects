CC = gcc
CFLAGS = -Wall -m64 -fPIC -Wformat-security -no-pie
NASM_FLAGS = -f elf64
LIBS = -lallegro -lallegro_memfile -lallegro_image -lpthread

# May be useless
# fatal error: GL/glew.h: No such file or directory
# fix: sudo apt-get install libglew-dev

# fatal error: GLFW/glfw3.h: No such file or directory
# fix: sudo apt install libglfw3-dev

# fatal error: glm/glm.hpp: No such file or directory
# fix: sudo apt-get install libglm-dev

# This is for linux programming
# https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/Linux


all: executable


debug: CFLAGS += -g
debug: NASM_FLAGS += -F dwarf -g
debug: executable

executable: main.o quadratic_func.o
	$(CC) $(CFLAGS)  -o program main.o quadratic_func.o $(LIBS)

quadratic_func.o: quadratic_func.s
	nasm $(NASM_FLAGS) -o quadratic_func.o quadratic_func.s

main.o: main.c
	$(CC) $(CFLAGS) -c  -o main.o main.c

clean:
	rm -f *.o program
