######################################################################
#  Project Makefile
######################################################################

PREFIX?=arm-none-eabi-
CC=$(PREFIX)gcc
OBJCOPY=$(PREFIX)objcopy
OD=bin

all: realall.really

SFLAGS= --static -nostartfiles -std=c11 -g3 -Os
SFLAGS+= -fno-common -ffunction-sections -fdata-sections
SFLAGS+= -I./freertos -I. -I./libopencm3/include  -L./libopencm3/lib
LFLAGS+=-Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

M0_FLAGS= $(SFLAGS) -mcpu=cortex-m0 -mthumb -msoft-float
M0P_FLAGS= $(SFLAGS) -mcpu=cortex-m0plus -mthumb -msoft-float
M3_FLAGS= $(SFLAGS) -mcpu=cortex-m3 -mthumb -msoft-float
M4FH_FLAGS= $(SFLAGS) -mcpu=cortex-m4 -mthumb -specs=nano.specs -specs=nosys.specs -mfloat-abi=hard -mfpu=fpv4-sp-d16
M7SP_FLAGS= $(SFLAGS) -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16

BINARY		= main
SRCFILES	= main.c freertos/heap_4.c freertos/list.c freertos/port.c freertos/queue.c freertos/tasks.c freertos/opencm3.c
LDSCRIPT	= stm32l476.ld

include boards.stm32.mk

realall.really: outdir $(BOARDS_ELF) $(BOARDS_BIN) $(BOARDS_HEX)

libopencm3/Makefile:
	@echo "Initializing libopencm3 submodule"
	git submodule update --init

libopencm3/lib/libopencm3_%.a: libopencm3/Makefile
	$(MAKE) -C libopencm3

%.bin: %.elf
	@#printf "  OBJCOPY $(*).bin\n"
	$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.hex: %.elf
	@#printf "  OBJCOPY $(*).hex\n"
	$(OBJCOPY) -Oihex $(*).elf $(*).hex

outdir:
	mkdir -p $(OD)

clean:
	$(RM) $(BOARDS_ELF) $(BOARDS_BIN) $(BOARDS_HEX)

.PHONY: realall.really outdir clean all
$(V).SILENT:
