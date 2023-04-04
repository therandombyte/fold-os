GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o gdt.o port.o kernel.o

%.o: %.cpp # create obj files from cpp files
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: %.s # create obj files from as files
	as $(ASPARAMS) -o $@ $<

foldKernel.bin: linker.ld $(objects) # link as and cpp to create kernel binary
	ld $(LDPARAMS) -T  $< -o $@ $(objects)

install: foldKernel.bin
	sudo cp $< /boot/foldKernel.bin

foldKernel.iso: foldKernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Fold OS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/foldKernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: foldKernel.iso
	VirtualBoxVM --startvm "Fold" &

.PHONY: clean
clean:
	rm -f $(objects) foldKernel.bin foldKernel.iso
