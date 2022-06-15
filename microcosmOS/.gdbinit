file kernel
target remote localhost:1234
set print pretty on
set confirm off

br core/kernel.c:20
c
