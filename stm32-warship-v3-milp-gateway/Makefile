PROJECT := warship_v3_milp_gateway
BUILD := build

CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size

CFLAGS := -mcpu=cortex-m3 -mthumb -Os -ffreestanding -fdata-sections -ffunction-sections
CFLAGS += -Wall -Wextra -Werror
LDFLAGS := -T stm32f103ze.ld -nostdlib -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(PROJECT).map

SRCS := startup.c main.c
OBJS := $(SRCS:%.c=$(BUILD)/%.o)

.PHONY: all clean flash

all: $(BUILD)/$(PROJECT).elf $(BUILD)/$(PROJECT).bin
	$(SIZE) $(BUILD)/$(PROJECT).elf

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/$(PROJECT).elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

$(BUILD)/$(PROJECT).bin: $(BUILD)/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

flash: all
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "program $(BUILD)/$(PROJECT).elf verify reset exit"

clean:
	rm -rf $(BUILD)
