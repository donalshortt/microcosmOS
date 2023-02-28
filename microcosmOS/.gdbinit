file kernel
target remote localhost:1234
set print pretty on
set confirm off

br core/kernel.c:42
br mm/mm.c:25
c

