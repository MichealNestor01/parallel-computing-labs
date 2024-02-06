#
# A simple makefile that compiles GLFW on Linux or Macs.
#
EXE = Mandelbrot
CC = gcc
CCFLAGS = -Wall -fopenmp -DGL_SILENCE_DEPRECATION

OS = $(shell uname)

ifeq ($(OS), Linux)
	MSG = On school RedHat machines, first try 'module load dependent-modules' followed by 'module load glfw', otherwise you will
	MSG += get errors about not being able to find the glfw libraries needed for the graphical display.
	CCFLAGS += -lm -lX11 -ldl -lGL -lglfw
endif

ifeq ($(OS), Darwin)
	MSG = Requires GLFW\; current include/lib dirs work for GLFW installed via homebrew but may need to be altered for other distributions.
	CCFLAGS += -lglfw -framework OpenGL -L /opt/homebrew/lib -I /opt/homebrew/include
endif

all:
	@echo $(MSG)
	@echo
	$(CC) -o $(EXE) Mandelbrot.c $(CCFLAGS) 
