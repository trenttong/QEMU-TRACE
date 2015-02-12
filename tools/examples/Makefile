include ../../config-host.mak

# INSTRUMENT_INCLUDE += $(GLIB_CFLAGS) $(QEMU_INCLUDES) -I/home/xtong/qemu-trace/x86_64-softmmu -I/home/xtong/qemu-trace/target-i386
CC_X86 = /usr/bin/gcc
CCFLAGS += -O3 -g -Wattributes -shared -fPIC  -I../../ -I../../include/ -I../../target-i386/ 

INSTRUMENT_MODULES= userkern.so branchtrace.so memaccesstrace.so memvaluetrace.so regtrace.so pctrace.so 
all:    $(INSTRUMENT_MODULES)

%.so:   %.c
	$(CC_X86) $(CCFLAGS) -o $@ $^
clean:  
	rm -rf *.so
