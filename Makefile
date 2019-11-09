# version
VERSION = 0
PATCHLEVEL = 1
SUBLEVEL = 0

# obj name / binary name
PROJECTNAME := STM32F1

OPENCM3_TARGET := stm32/f1
OPENCM3_DIR := libs/libopencm3

TARGET := main
#TARGET += modules os

Q = @+


#编译器设置
# libopencm3 CROSS
PREFIX ?= arm-none-eabi
CC = $(PREFIX)-gcc
LD = $(PREFIX)-gcc
AR = $(PREFIX)-ar
AS = $(PREFIX)-as
OBJCOPY = $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB		:= $(PREFIX)-gdb

OPT		:= -O2
DEBUG		:= -ggdb3
CSTD		?= -std=c99
DEFS		+= -DSTM32F1  #lib 头文件需要

TOP_DIR := $(shell pwd)

# include file
DEFS += -I$(OPENCM3_DIR)/include
DEFS += $(TARGET:%=-I%/inc)
LDFLAGS		+= $(TARGET:%=-L%)
LDFLAGS		+= -L$(OPENCM3_DIR)/lib
LDLIBS		+= $(TARGET:%=-l%)
LDLIBS		+= -lopencm3_$(subst /,,$(OPENCM3_TARGET))
#LDLIBS		+= $(TARGET:%=-l%/%.a)
#LDLIBS		+= -l$(OPENCM3_DIR)/lib/libopencm3_$(subst /,,$(OPENCM3_TARGET)).a
LDSCRIPT	?= cortex-m.ld

FP_FLAGS	?= -msoft-float
ARCH_FLAGS	= -mthumb -mcpu=cortex-m3 $(FP_FLAGS) -mfix-cortex-m3-ldrd

########################################################################
# C flags
TGT_CFLAGS	+= $(OPT) $(CSTD) $(DEBUG)
TGT_CFLAGS	+= $(ARCH_FLAGS)
TGT_CFLAGS	+= -Wextra -Wshadow -Wimplicit-function-declaration
TGT_CFLAGS	+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
TGT_CFLAGS	+= -fno-common -ffunction-sections -fdata-sections

# C++ flags
TGT_CXXFLAGS	+= $(OPT) $(CXXSTD) $(DEBUG)
TGT_CXXFLAGS	+= $(ARCH_FLAGS)
TGT_CXXFLAGS	+= -Wextra -Wshadow -Wredundant-decls  -Weffc++
TGT_CXXFLAGS	+= -fno-common -ffunction-sections -fdata-sections

# C & C++ preprocessor common flags
TGT_CPPFLAGS	+= -MD
TGT_CPPFLAGS	+= -Wall -Wundef
TGT_CPPFLAGS	+= $(DEFS)

# Linker flags
TGT_LDFLAGS		+= --static -nostartfiles
TGT_LDFLAGS		+= -T$(LDSCRIPT)
TGT_LDFLAGS		+= $(ARCH_FLAGS) $(DEBUG)
TGT_LDFLAGS		+= -Wl,-Map=$(*).map -Wl,--cref
TGT_LDFLAGS		+= -Wl,--gc-sections

# Used libraries
LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
#LDLIBS += $(TARGET:%=%/inc)
########################################################################

export

# build objective

all: $(PROJECTNAME).bin $(PROJECTNAME).hex $(PROJECTNAME).elf

LIB_CM := $(OPENCM3_DIR)/lib/libopencm3_$(subst /,,$(OPENCM3_TARGET)).a
$(LIB_CM):
	$(Q)$(MAKE) -C $(OPENCM3_DIR) lib/$(OPENCM3_TARGET)

$(PROJECTNAME).elf: $(LIB_CM) $(TARGET)
	@#printf "  LD      $@\n"
	$(Q)$(LD) $(TGT_LDFLAGS) $(LDFLAGS) main/src/main.o $(LDLIBS) -o $@

%.bin: %.elf
	@#printf "  OBJCOPY $(*).bin\n"
	$(Q)$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.hex: %.elf
	@#printf "  OBJCOPY $(*).hex\n"
	$(Q)$(OBJCOPY) -Oihex $(*).elf $(*).hex

update_stlink:
	openocd -f interface/stlink-v2.cfg  -f target/stm32f1x_stlink.cfg -c init -c halt -c "flash write_image erase $(TOP_DIR)/$(PROJECTNAME).hex" -c reset -c shutdown



$(TARGET):
	@#printf "  BUILD $@\n";
	$(Q)$(MAKE) -C $@ BUILD_TARGET=lib$@.a
	

clean: targetclean
	@#printf "  CLEAN\n;"
	$(Q)$(MAKE) -C $(OPENCM3_DIR) clean
	$(Q)$(RM) *.elf *.bin *.hex

targetclean:$(TARGET:=.clean)

%.clean:
	$(Q)if [ -d $* ]; then \
		printf "  CLEAN   $*\n"; \
		$(MAKE) -C $* clean || exit $?; \
	fi;

.PHONY: all lib $(TARGET) clean

