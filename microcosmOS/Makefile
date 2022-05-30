CC=gcc
SHARED_FLAGS = -fno-builtin -O0 -nostdinc -nostdlib -ffreestanding -ggdb -Wall -Wextra \
               -Werror -I. -MMD -mno-red-zone -fno-pie
CFLAGS = $(SHARED_FLAGS)
ASFLAGS = $(SHARED_FLAGS) -Wa,--divide

OBJS = boot.o pmm.o

DFILES = $(patsubst %.o,%.d,$(OBJS))

all: kernel

kernel.o: kernel.c
	$(CC) -fno-builtin -O2 -nostdinc -nostdlib -ffreestanding -ggdb -Wall -Wextra \
	-c -O0 -z -no-pie -mcmodel=large kernel.c


kernel: $(OBJS) kernel.o kernel.ld Makefile
	$(CC) -O0 -z max-page-size=0x1000 $(CFLAGS) -no-pie -Wl,--build-id=none -T kernel.ld -o $@ kernel.o $(OBJS)

clean:
	find -name "*~" -delete
	rm -rf $(OBJS) $(DFILES) kernel

$(OBJS): Makefile
kernel.o: Makefile
-include $(DFILES)
