
# CC = gcc

CFLAGS = -O
LDFLAGS = -lm


OBJECTS = fluid.o bndinit.o dimalloc.o parse.o particle.o pressure.o tscells.o velocity.o visual.o view.o

all: fluid

fluid: $(OBJECTS)
	$(CC) -o $(@) $(OBJECTS) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) fluid
