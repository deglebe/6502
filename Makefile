include config.mk

SRCDIR	= src
INCDIR	= include

SOURCES	= $(SRCDIR)/main.c \
          $(SRCDIR)/system.c \
          $(SRCDIR)/hardware/hardware.c \
          $(SRCDIR)/hardware/cpu.c

OBJECTS	= $(SOURCES:.c=.o)

TARGET	= 6502

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -I$(INCDIR) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
