CC = gcc
# CFLAGS = -Wall -Wextra -IInclude -g
CFLAGS = -IInclude -g

SRCDIR = src
INCDIR = Include
OBJDIR = obj

SOURCES = $(SRCDIR)/main.c $(SRCDIR)/module.c $(SRCDIR)/hash_set.c
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = target/release/spreadsheet
REPORT = ./report/report.tex

$(TARGET): $(OBJECTS)
	echo $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -lm

$(OBJDIR):
	@mkdir -p $(OBJDIR)

report:
	pdflatex $(REPORT)

test:
	@gcc -I./Include Test.c src/hash_set.c src/module.c -o test -g && ./test

clean:
	@rm -rf $(OBJDIR) $(TARGET) test
	@rm -rf *.aux *.log *.out *.toc
	@rm -rf *.pdf

.PHONY: clean report test