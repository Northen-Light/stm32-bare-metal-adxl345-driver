CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

BUILD_DIR = build

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
	-Wl,-Map=$(BUILD_DIR)/main.map,--gc-sections \
	-nostdlib

SRCS = \
	startup/startup.c \
	src/main.c \
	src/i2c.c

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

ELF = $(BUILD_DIR)/main.elf
BIN = $(BUILD_DIR)/main.bin

all: $(BIN)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -lgcc -o $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD_DIR)

flash: $(BIN)
	st-flash write $(BIN) 0x08000000

.PHONY: all clean flash