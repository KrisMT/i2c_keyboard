MCU=atmega32a
AVRDUDEMCU=m32
F_CPU=16000000UL

CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=$(MCU) -DF_CPU=$(F_CPU)
OBJ2HEX=avr-objcopy
AVRDUDE=avrdude
TARGET=firmware

all:
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)
	$(OBJ2HEX) -R .eeprom -O ihex $(TARGET) $(TARGET).hex
	rm -f $(TARGET)

install:
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -c linuxgpio -U flash:w:$(TARGET).hex

clean:
	rm -fr *.hex *.obj *.o

