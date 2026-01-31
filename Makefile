CC=clang
AS=nasm
CFLAGS=-ffreestanding -fno-builtin -fno-stack-protector -O2 -Wall -Wextra -mno-red-zone
ASFLAGS=-f elf64
LDFLAGS=-nostdlib -T linker.ld

BUILD=build

all: iso

$(BUILD):
	mkdir -p $(BUILD)

# assemble boot code
$(BUILD)/boot.o: boot/boot.asm | $(BUILD)
	$(AS) $(ASFLAGS) $< -o $@

# compileaza kernel
$(BUILD)/kernel.o: kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# link kernel ELF
$(BUILD)/kernel.elf: $(BUILD)/boot.o $(BUILD)/kernel.o
	ld $(LDFLAGS) $^ -o $@

# ISO cu GRUB
iso: $(BUILD)/kernel.elf
	mkdir -p iso/boot/grub
	cp $(BUILD)/kernel.elf iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'menuentry "Minux" { multiboot2 /boot/kernel.elf }' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o Minux.iso iso

# run in QEMU
run: iso
	qemu-system-x86_64 -cdrom Minux.iso

clean:
	rm -rf $(BUILD) iso Minux.iso
