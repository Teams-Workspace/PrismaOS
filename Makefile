# Compiler and assembler settings
CC=gcc                       # C compiler
AS=as                         # GNU assembler
GCCPARAMS = -m32 -nostdlib -fno-builtin -fno-exceptions -ffreestanding -fno-leading-underscore -Wall -Wextra -Wpedantic
                              # Flags for GCC:
                              # -m32: generate 32-bit code
                              # -nostdlib: do not link standard libraries
                              # -fno-builtin: do not use compiler built-ins
                              # -fno-exceptions: disable C++ exceptions (mostly for freestanding)
                              # -ffreestanding: for OS development (no standard library assumed)
                              # -fno-leading-underscore: prevent underscore prefixing symbols
                              # -Wall -Wextra -Wpedantic: enable warnings

ASPARAMS = --32                # Assemble as 32-bit code
LDPARAMS = -melf_i386 -nostdlib # Link as 32-bit ELF, without standard library

# Directory variables
SRC_DIR=src
HDR_DIR=include/
OBJ_DIR=obj
ISO_DIR=iso

# Source file collections
SRC_FILES1=$(wildcard $(SRC_DIR)/*.c)       # All C source files
OBJ_FILES1=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES1))  # Corresponding object files
SRC_FILES2=$(wildcard $(SRC_DIR)/*.s)       # All assembly files (.s)
OBJ_FILES2=$(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(SRC_FILES2))
SRC_FILES3=$(wildcard $(SRC_DIR)/*.asm)     # NASM assembly files (.asm)
OBJ_FILES3=$(patsubst $(SRC_DIR)/%.asm, $(OBJ_DIR)/%.o, $(SRC_FILES3))

# Default target
all: primus-os.iso

# Create object directory if it doesn't exist
check_dir:
	mkdir -p $(OBJ_DIR)

# Compile C files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | check_dir
	$(CC) $(GCCPARAMS) $^ -I$(HDR_DIR) -c -o $@

# Assemble .s files with GNU assembler
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | check_dir
	$(AS) $(ASPARAMS) -o $@ $<

# Assemble .asm files with NASM
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm | check_dir
	nasm -f elf32 -o $@ $<

# Link all object files into a single OS binary
primus-os.bin: $(SRC_DIR)/linker.ld $(OBJ_FILES1) $(OBJ_FILES2) $(OBJ_FILES3)
	ld $(LDPARAMS) -T $< -o $@ $(OBJ_DIR)/*.o

# Create bootable ISO
primus-os.iso: primus-os.bin
	./update_version                         # Update version header and file
	mkdir -p iso/boot/grub                    # Create ISO directory structure
	cp primus-os.bin iso/boot/primus-os.bin   # Copy kernel binary
	# Create grub configuration
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "PrimusOS" {'            >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/primus-os.bin'   >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=primus-os.iso iso  # Build bootable ISO with GRUB
	rm -rf iso                                # Clean temporary ISO directory

# Clean all build artifacts
clean:
	rm -rf *.o primus-os primus-os.iso primus-os.bin $(OBJ_DIR) iso


# Summary
# This Makefile automates the build process for a simple operating system called PrimusOS. Here’s a summary of its key features:
# 1. **Compiler/Assembler Setup:** Uses GCC for C, `as` for `.s`, and NASM for `.asm` files in 32-bit mode.
# 2. **Source Management:** Automatically finds C, GNU assembly, and NASM files in `src/`.
# 3. **Object Compilation:** Builds `.o` files into `obj/` directory.
# 4. **Linking:** Links all object files with a custom linker script into `primus-os.bin`.
# 5. **ISO Creation:**
#    * Updates version info
#    * Prepares GRUB bootloader directory
#    * Copies kernel binary
#    * Writes `grub.cfg` with a menu entry
#    * Runs `grub-mkrescue` to produce `primus-os.iso`
# 6. **Clean Target:** Removes all object files, binaries, ISO, and temporary directories.

# **In short:** This Makefile fully automates building a bootable OS ISO from C/assembly source code with version management.

