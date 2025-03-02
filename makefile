CC = gcc
# CFLAGS = -Wall -Wextra -IInclude -g
CFLAGS = -IInclude -g

SRCDIR = src
INCDIR = Include
OBJDIR = obj

SOURCES = $(SRCDIR)/main.c $(SRCDIR)/module.c $(SRCDIR)/hash_set.c
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = target/release/spreadsheet

$(TARGET): $(OBJECTS)
	echo $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -lm

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: clean
