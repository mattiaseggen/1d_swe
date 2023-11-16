CPPFLAGS=-Wall -g
LDFLAGS=-lm
TARGETS=shallow_water
all: $(TARGETS)

run: all
	./$(TARGETS)

plot:
	gnuplot plot.gp
