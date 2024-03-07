#compiling in 32 bit mode
GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
#assembling in 32 bit mode  
ASPARAMS = --32
#linker produce ELF files for 32 bit intel architecture
LDPARAMS = -melf_i386

objects = loader.o kernel.o

#target ($@) : prerequisite ($<)

#-c compiling without linking
%.o : %.cpp
	g++ $(GCCPARAMS) -o $@ -c $<  

%.o : %.s
	as $(ASPARAMS) -o $@ $<

#-T use the linker script $< to output $@ from input $(objects)
mykernel.bin : linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

#copy on boot directory
install : mykernel.bin
	sudo cp $< /boot/mykernel.bin

#create a directory iso/boot/grub 
#copy mykernel.bin on iso/boot
#create menuentry in grub.config on iso/boot/grub
#create an iso image of the kernel
mykernel.iso : mykernel.bin
	mkdir -p iso/boot/grub/
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "os-blue" {' >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '    boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

#run OS in VirtualBox, if already running, kill and run
run : mykernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm "os-blue" &

