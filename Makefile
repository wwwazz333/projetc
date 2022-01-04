CC = gcc
EXEC = main

SRCDIR = src
SRC = $(wildcard src/*.c)
OBJDIR = obj
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

FLAGS = -Iinclude -g3 -Wall -Wextra 
CFLAGS = 

WINDOWS = no

all : $(EXEC)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(FLAGS) -o $@ -c $<

$(EXEC) : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

ifeq ($(WINDOWS), yes)
clean :
	del .\\$(OBJDIR)\\*.o
	del $(EXEC).exe	
else
clean : 
	rm -rf $(OBJDIR)/*.o 
	rm $(EXEC)
endif