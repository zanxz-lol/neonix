export NAHO_ROOT = $(CURDIR)
export KERN_OUT = $(CURDIR)/src/vmnaho.elf

ISO_OUT := naho-bootable.iso

BOOTLOADER_DIR := $(NAHO_ROOT)/bootloader
ISO_STRUCT_DIR := $(NAHO_ROOT)/iso

.PHONY: iso
.PHONY: bootloader

bootloader: $(BOOTLOADER_DIR)/limine

$(BOOTLOADER_DIR)/limine:
	@make -C $(BOOTLOADER_DIR)

$(KERN_OUT):
	@make -C $(NAHO_ROOT)/src

run: $(KERN_OUT) $(ISO_OUT)
	@qemu-system-x86_64 \
		-cdrom $(ISO_OUT) \
		-debugcon stdio \
		-m 8G \
		-no-reboot \
		-bios /usr/share/edk2/x64/OVMF.4m.fd \
		-cpu qemu64,+sse4.1 \
		-enable-kvm \
		-audiodev pa,id=speaker \
		-machine pcspk-audiodev=speaker

menuconfig:
	@make menuconfig --directory=$(NAHO_ROOT)/src


# this assumes that you have already compiled the kernel
$(ISO_OUT):
	@mkdir -p $(ISO_STRUCT_DIR)/boot/limine/
	@mkdir -p $(ISO_STRUCT_DIR)/EFI/BOOT/
	@cp -v $(KERN_OUT) $(ISO_STRUCT_DIR)/boot/
	@cp -v $(NAHO_ROOT)/config/limine.conf $(BOOTLOADER_DIR)/limine-bios.sys $(BOOTLOADER_DIR)/limine-bios-cd.bin \
		$(BOOTLOADER_DIR)/limine-uefi-cd.bin $(ISO_STRUCT_DIR)/boot/limine/
	@cp -v $(BOOTLOADER_DIR)/BOOTX64.EFI $(ISO_STRUCT_DIR)/EFI/BOOT/
	@xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_STRUCT_DIR) -o $(ISO_OUT)
	@$(BOOTLOADER_DIR)/limine bios-install $(ISO_OUT)

clean:
	@rm -rf $(ISO_OUT) $(ISO_STRUCT_DIR)/EFI/BOOT/* $(ISO_STRUCT_DIR)/boot/vm-neonix.elf $(ISO_STRUCT_DIR)/boot/limine/*
	@make clean --directory=$(NAHO_ROOT)/src
