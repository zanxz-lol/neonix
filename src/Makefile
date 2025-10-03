KERNEL_MAJOR_VERSION := 0
KERNEL_MINOR_VERSION := 1
KERNEL_PATCH_VERSION := 0

SRC = $(NAHO_ROOT)/src
TOOLS = $(SRC)/tools

CONFIG := $(SRC)/.config

export KERNEL_VERSION = $(KERNEL_MAJOR_VERSION).$(KERNEL_MINOR_VERSION).$(KERNEL_PATCH_VERSION)

CC_PREFIX ?= x86_64-elf-
ARCH ?= amd64
ASM ?= nasm

CC_FLAGS := \
	-I $(SRC)/include/ \
	-I $(SRC)/arch/$(ARCH)/include/ \
	-Werror \
	-ffreestanding \
	-fno-lto \
	-O2 \
	-m64 \
	-march=x86-64 \
	-mno-mmx \
	-mno-red-zone \
	-mcmodel=kernel \
	-mno-80387 \
	-fsanitize=undefined \
	-fstack-protector-all \
	-fstack-check \
	-DKERNEL_MAJOR_VERSION=$(KERNEL_MAJOR_VERSION) \
	-DKERNEL_MINOR_VERSION=$(KERNEL_MINOR_VERSION) \
	-DKERNEL_PATCH_VERSION=$(KERNEL_PATCH_VERSION) \
	-msse4

ASM_FLAGS := \
	-f elf64 \
	-werror

LD_FLAGS := \
	-Map=$(KERN_OUT).map \
	-m elf_x86_64 \
	-nostdlib \
	-static \
	-z max-page-size=4096 \
	-T $(SRC)/arch/$(ARCH)/linker.ld

CC_ALL_SRC := $(shell find -L * -type f -name "*.c")
CC_ARCH_SRC := $(shell find -L * -type f -wholename "./arch/$(ARCH)/*.c")
CC_OTHER_ARCH_SRC := $(filter-out $(CC_ARCH_SRC), $(shell find -L * -type f -wholename "./arch/*/*.c"))
CC_SRC := $(filter $(CC_ARCH_SRC), $(CC_ALL_SRC)) $(filter-out $(CC_OTHER_ARCH_SRC), $(filter-out $(CC_ARCH_SRC), $(CC_ALL_SRC)))

ASM_SRC := $(shell find -L * -type f -name "*.s")

CC_OBJ := $(patsubst %.c, %.o, $(CC_SRC))
ASM_OBJ := $(patsubst %.s, %.o, $(ASM_SRC))

$(KERN_OUT): $(ASM_OBJ) $(CC_OBJ)
	@$(CC_PREFIX)ld $(LD_FLAGS) $^ -o $@
	@echo "[LD] - $@"
	@echo "$(KERN_OUT) compiled successfully! :)"

%.o: %.c
	@$(CC_PREFIX)gcc $(CC_FLAGS) -c $< -o $@
	@echo "[CC] - $<"

%.o: %.s
	@$(ASM) $(ASM_FLAGS) $< -o $@
	@echo "[ASM] - $<"

clean:
	@rm -rf $(CC_OBJ) $(ASM_OBJ) $(KERN_OUT) $(KERN_OUT).map

menuconfig:
	@python3 $(TOOLS)/kconfig/menuconfig.py
	@python3 $(TOOLS)/kconfig/gen_config.py $(CONFIG) $(SRC)/include/config.h
