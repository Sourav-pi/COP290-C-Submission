CC = gcc
CFLAGS = -Wall -Wextra -IInclude

SRCDIR = src
INCDIR = Include
OBJDIR = obj
BINDIR = bin

SOURCES = $(SRCDIR)/main.c $(SRCDIR)/module.c
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/sheet

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: clean