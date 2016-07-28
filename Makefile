DEVICE=atmega32a
MCU=m32
F_CPU=16000000UL
AVRDUDE=sudo avrdude -c linuxgpio -p $(MCU)

CFLAGS=-I. -DDEBUG_LEVEL=0
OBJECTS = firmware.o keymap.o matrix.o
COMPILE=avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build main.hex"
	@echo "make program ... to flash fuses and firmware"
	@echo "make flash ..... to flash the firmware (use this on metaboard)"
	@echo "make clean ..... to delete objects and hex file"

hex: firmware.hex

program: flash

flash: firmware.hex
	$(AVRDUDE) -U flash:w:firmware.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

firmware.elf: $(OBJECTS)
	$(COMPILE) -o firmware.elf $(OBJECTS)

firmware.hex: firmware.elf
	rm -fr firmware.hex firmware.eep.hex
	avr-objcopy -j .text -j .data -O ihex firmware.elf firmware.hex
	avr-size firmware.hex

disasm: firmware.elf
	avr-objdump -d firmware.elf

clean:
	rm -fr *.hex *.obj *.o *.elf

