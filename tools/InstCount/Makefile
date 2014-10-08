include ../../config-host.mak

# INSTRUMENT_INCLUDE += $(GLIB_CFLAGS) $(QEMU_INCLUDES) -I/home/xtong/qemu-trace/x86_64-softmmu -I/home/xtong/qemu-trace/target-i386
CC_X86 = gcc
CCFLAGS += -O3 -g -Wattributes -shared -fPIC  -I../../ -I../../include/ -I../../target-i386/ 

INSTRUMENT_MODULES= memsim.so regsim.so 
all:    $(INSTRUMENT_MODULES)

%.so:   %.c
	$(CC_X86) $(CCFLAGS) -o $@ $^
clean:  
	rm -rf *.so
