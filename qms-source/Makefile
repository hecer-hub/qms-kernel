CC = g++
LD = ld
OBJCOPY = objcopy

CFLAGS = -m32 -ffreestanding -c -fno-exceptions -fno-rtti -fno-stack-protector -fno-toplevel-reorder
LDFLAGS = -m elf_i386 -T linker.ld

KERNEL_DIR = kernel
BUILD_DIR = isodir/boot/kernel

C_SOURCES := $(wildcard $(KERNEL_DIR)/*.c) $(wildcard $(KERNEL_DIR)/func/*.c) $(wildcard $(KERNEL_DIR)/test/*.c) $(wildcard $(KERNEL_DIR)/bin/*.c)
CPP_SOURCES := $(wildcard $(KERNEL_DIR)/*.cpp) $(wildcard $(KERNEL_DIR)/func/*.cpp) $(wildcard $(KERNEL_DIR)/test/*.cpp) $(wildcard $(KERNEL_DIR)/bin/*.cpp)

C_OBJS := $(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
CPP_OBJS := $(patsubst $(KERNEL_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))

ALL_OBJS := $(C_OBJS) $(CPP_OBJS)

INIT_OBJ := $(BUILD_DIR)/init.o
OTHER_OBJS := $(filter-out $(INIT_OBJ),$(ALL_OBJS))
OBJS := $(INIT_OBJ) $(OTHER_OBJS)

TARGET = $(BUILD_DIR)/kernel.bin
QMS_ISO = qms.iso


all: iso run

run:
	@echo "--- Starting QEMU ---"
	qemu-system-i386 -cdrom $(QMS_ISO)

iso: kernel
	@echo "--- Creating ISO image ---"
	grub-mkrescue -o $(QMS_ISO) isodir/
	@echo "--- Cleaning up object files ---"
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/func/*.o $(BUILD_DIR)/test/*.o $(BUILD_DIR)/bin/*.o

kernel: dirs $(OBJS)
	@echo "--- Linking kernel.bin ---"
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "--- Compiling C: $< ---"
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "--- Compiling C++: $< ---"
	$(CC) $(CFLAGS) $< -o $@

dirs:
	@mkdir -p $(BUILD_DIR)/func $(BUILD_DIR)/test $(BUILD_DIR)/bin

clean:
	@echo "--- Full clean ---"
	rm -rf isodir/boot/kernel/*.o $(TARGET) $(QMS_ISO)

.PHONY: all run buildiso buildk clean dirs
