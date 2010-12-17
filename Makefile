EXE:=pov
SRCS:=pov.c
IMG:=hello.xpm

# PROGRAM:=avrdude
PROGRAM:=hid

OBJS:=$(SRCS:.c=.o)
IMG_c:=$(IMG:.xpm=.c)
LSS:=$(EXE).lss
HEX:=$(EXE).hex
MAP:=$(EXE).map

CC:=avr-gcc
LD:=avr-gcc
OBJDUMP:=avr-objdump
OBJCOPY:=avr-objcopy

CFLAGS:=-mmcu=atmega328p -DIMG=\"$(IMG_c)\" -DF_CPU=16000000 -Os -Wall -Werror -std=gnu99

LDFLAGS:=-mmcu=atmega328p -Wl,-Map=$(MAP),--cref

PROGRAM_avrdude:=avrdude -c avrisp2 -P usb -p m328p -D -U flash:w:$(HEX)
PROGRAM_hid:=bootloadHID -r $(HEX)

.PHONY: all
all: $(EXE) $(LSS)

$(EXE): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY: hex
hex: $(HEX)

# TODO: Add dependency generation and checking.
pov.o: $(IMG_c)

%.c: %.xpm
	$(PWD)/xpmcvt.awk < $< > $@

$(LSS): $(EXE)
	$(OBJDUMP) -l -S -h $< > $@

$(HEX): $(EXE)
	$(OBJCOPY) -O ihex $< $@

.PHONY: flash
flash: $(HEX)
	$(PROGRAM_$(PROGRAM))

.PHONY: clean
clean:
	rm -f $(EXE) $(LSS) $(MAP) $(IMG_c:.xpm=.c) $(OBJS) *~

