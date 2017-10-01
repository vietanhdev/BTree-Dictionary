# Declaration of variables
CC = gcc
CC_FLAGS = -Wall -g
 
# File names
EXEC = Bdict
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

# LIBS
LIBS = -Lbt/lib -lbt -Ibt/inc
 
# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) ${LIBS}
 
# To obtain object files
%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@ ${LIBS}
 
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
