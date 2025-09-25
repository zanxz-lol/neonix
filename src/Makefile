ARCH ?= amd64
KERN_OUT := vm-neonix.elf

CC_PREFIX ?= x86_64-elf-
ASM ?= nasm

CC_FLAGS := \
	-I ./include/ \
	-I ./arch/$(ARCH)/include/ \
	-Werror \
	-ffreestanding \
	-fno-lto \
	-O0 \
	-m64 \
	-march=x86-64 \
	-mno-mmx \
	-mno-red-zone \
	-mcmodel=kernel \
	-mno-80387 \
	-fsanitize=undefined \
	-fstack-protector-all \
	-fstack-check \
	-msse4

ASM_FLAGS := -f elf64 -werror
LD_FLAGS := \
	-Map=$(KERN_OUT).map\
	-m elf_x86_64 \
	-nostdlib \
	-static \
	-z max-page-size=4096 \
	-T arch/$(ARCH)/linker.ld


CC_ALL_SRC := $(shell find -L * -type f -name "*.c")
CC_ARCH_SRC := $(shell find -L * -type f -wholename "arch/$(ARCH)/*.c")
CC_OTHER_ARCH_SRC := $(filter-out $(CC_ARCH_SRC), $(shell find -L * -type f -wholename "arch/*/*.c"))
CC_SRC := $(filter $(CC_ARCH_SRC), $(CC_ALL_SRC)) $(filter-out $(CC_OTHER_ARCH_SRC), $(filter-out $(CC_ARCH_SRC), $(CC_ALL_SRC)))

ASM_SRC := $(shell find -L * -type f -wholename "arch/$(ARCH)/*.s")

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
	@rm -rf $(CC_OBJ) $(ASM_OBJ) $(KERN_OUT)
	
