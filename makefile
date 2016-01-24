TARGET   = levitron
MCU      = atmega328p
PROGR    = avrispmkII

CC       = avr-gcc
OBJCOPY  = avr-objcopy
AVRDUDE  = avrdude

CFLAGS   = -Wall -Werror -Os
PFLAGS   =

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
HEADERS  := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(BINDIR)/$(TARGET).hex

$(BINDIR)/$(TARGET).elf: $(OBJECTS) | $(BINDIR)
	$(CC) -mmcu=$(MCU) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) -c -mmcu=$(MCU) $(CFLAGS) -o $@ $<

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)/%.hex: $(BINDIR)/%.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

flash: $(BINDIR)/$(TARGET).hex
	sudo $(AVRDUDE) -c $(PROGR) -p $(MCU) $(PFLAGS) -U flash:w:$<

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/*.elf $(BINDIR)/*.hex
