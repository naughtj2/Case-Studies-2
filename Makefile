# Compiler
CC = gcc

# Compiler flags
CFLAGS = -O2 -Wall
OMPFLAGS = -fopenmp

# Targets
all: Q2 Q3

Q2: Q2.c
	$(CC) $(CFLAGS) -o Q2 Q2.c -lm

Q3: Q3.c
	$(CC) $(CFLAGS) $(OMPFLAGS) -o Q3 Q3.c -lm

# Clean target to remove compiled binaries
clean:
	rm -f Q2 Q3

