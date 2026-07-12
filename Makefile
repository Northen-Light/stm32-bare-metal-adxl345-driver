CC				= arm-none-eabi-gcc
LD				= arm-none-eabi-ld
OBJCOPY		= arm-none-eabi-objcopy

CCFLAGS		= -Iinclude -mthumb -mcpu=cortex-m3 -O0 -ggdb -Wall -Wextra -Wconversion
LDFALGS		= -Tlinker/main.ld -Map=main.map

SRCS			= \
						startup/startup.c \
						src/main.c	\
						src/adxl345.c \
						src/i2c.c	\

OBJS			= $(SRCS:.c=.o)

all:	main.bin

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@ 

main.elf: $(OBJS)
	$(LD) $(LDFALGS) $^ -o $@

main.bin:	main.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(OBJS) main.elf main.map main.bin

flash:
	st-flash write main.bin 0x08000000 

.PHONY:	all, clean