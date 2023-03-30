file kernel
target remote localhost:1234
set print pretty on
set confirm off
source /home/donal/repos/gdb-pt-dump/pt.py

#br core/kernel.c:42
#br mm/mm.c:25

br mm/vmm/vmm.c:81
#br mm/vmm/vmm.c:154

#br core/boot.S:29
#br core/boot.S:110
#br core/boot.S:96
#br core/boot.S:53

#br core/kernel.c:28

#br mm/pmm/pmm.c:216
c

