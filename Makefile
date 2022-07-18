TARGET = nrf52-rtt-demo
DEVICE = NRF52832_XXAA
CROSS = arm-none-eabi-
CC = $(CROSS)gcc
LD = $(CROSS)ld
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
RM = rm
FIND = find
PYOCD = pyocd

SRC = ./src
BUILD=./build
LD_SCRIPT = ./linker/nrf52_xxaa.ld
NRF5_SDK = ./nRF5_SDK

CFLAGS = \
	-D$(DEVICE) \
	-I$(NRF5_SDK)/components/toolchain/cmsis/include \
	-I$(NRF5_SDK)/modules/nrfx \
	-I$(NRF5_SDK)/modules/nrfx/mdk \
	-I$(NRF5_SDK)/modules/nrfx/hal \
	-I./inc \
	-mthumb \
	-mcpu=cortex-m4 \
	-mfloat-abi=hard \
	-Os \
	-fno-common \
	-fomit-frame-pointer \
	-ffunction-sections \
	-fdata-sections \
	-Wstrict-prototypes  \
	-Wno-strict-aliasing

LDFLAGS = -T$(LD_SCRIPT) -L $(NRF5_SDK)/modules/nrfx/mdk -lgcc -lm -lnosys -lc -Xlinker --gc-sections

SRCS = \
	$(NRF5_SDK)/modules/nrfx/mdk/gcc_startup_nrf52.S \
	$(NRF5_SDK)/modules/nrfx/mdk/system_nrf52.c \
	$(SRC)/timer.c \
	$(SRC)/SEGGER_RTT.c \
	$(SRC)/printf.c \
	$(SRC)/rtt.c \
	$(SRC)/main.c
	
SRCS_C = $(filter %.c, $(SRCS))
SRCS_S = $(filter %.S, $(SRCS))
OBJS = $(SRCS_S:.S=.o) $(SRCS_C:.c=.o)

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $^ $@

$(BUILD)/$(TARGET).hex: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -Oihex $^ $@

$(BUILD)/$(TARGET).elf: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $^

flash: $(BUILD)/$(TARGET).bin
	$(PYOCD) flash $(BUILD)/$(TARGET).bin

clean:
	$(FIND) . -name *.o -exec $(RM) \{\} \;
	$(RM) -f $(BUILD)/$(TARGET).*
