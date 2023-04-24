file kernel
target remote localhost:1234
set print pretty on
set confirm off
source /home/donal/repos/gdb-pt-dump/pt.py

#br core/kernel.c:22

br test/vmm/vmm_test.c:29

tui enable

c

