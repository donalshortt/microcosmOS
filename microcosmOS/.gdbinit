file kernel
target remote localhost:1234
set print pretty on
set confirm off

br mm/pmm.c:55
c
