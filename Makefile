CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CPUFLAGS = -mcpu=cortex-m3 -mthumb

CFLAGS = \
	-Iinclude \
	$(CPUFLAGS) \
	-std=c11 \
	-ffreestanding \
	-O0 \
	-ggdb \
	-Wall \
	-Wextra \
	-Wconversion \
	-Wshadow \
  -Wundef \
	-ffunction-sections \
	-fdata-sections

LDFLAGS = \
	$(CPUFLAGS) \
	-Tlinker/main.ld \
	-Wl,-Map=main.map,--gc-sections \
	-nostdlib

SRCS = \
	startup/startup.c \
	app/main.c \
	drivers/adxl345.c \
	drivers/i2c.c

OBJS = $(SRCS:.c=.o)

all: main.bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -lgcc -o $@

main.bin: main.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f $(OBJS) main.elf main.map main.bin

flash: main.bin
	st-flash write main.bin 0x08000000

.PHONY: all clean flash