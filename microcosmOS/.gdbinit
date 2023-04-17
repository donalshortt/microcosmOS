file kernel
target remote localhost:1234
set print pretty on
set confirm off
source /home/donal/repos/gdb-pt-dump/pt.py

#br mm/vmm/vmm.c:96
#br mm/vmm/vmm.c:155

#br core/kernel.c:25
#br mm/pmm/pmm.c:163

br test/pmm/pmm_test.c:41

c

