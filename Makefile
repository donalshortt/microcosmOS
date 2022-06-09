
ISO_FILE=kernel.iso

.PHONY: all
.PHONY: kernel
.PHONY: qemu
.PHONY: iso
.PHONY: clean

ifneq (, $(shell which grub2-mkrescue 2> /dev/null))
  GRUB_MKRESCUE = grub2-mkrescue
else ifneq (, $(shell which grub-mkrescue 2> /dev/null))
  GRUB_MKRESCUE = grub-mkrescue
else
    $(error "Cannot find grub-mkrescue or grub2-mkrescue")
endif

all: kernel

kernel:
	make -C microcosmOS all

qemu: $(ISO_FILE)
	qemu-system-x86_64 -cdrom $(ISO_FILE) -serial stdio -m 1024M

debug: $(ISO_FILE)
	qemu-system-x86_64 -s -S -cdrom $(ISO_FILE) -serial stdio -m 8192M

clean:
	make -C microcosmOS clean
	rm -rf iso
	rm -rf $(ISO_FILE)

iso: $(ISO_FILE)

$(ISO_FILE): kernel
	mkdir -p iso/boot/grub
	cp microcosmOS/grub.cfg iso/boot/grub/
	cp microcosmOS/kernel iso/boot/
	$(GRUB_MKRESCUE) -o $(ISO_FILE) iso
