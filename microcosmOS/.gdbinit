file kernel
target remote localhost:1234
set print pretty on
set confirm off

br kernel.c:49
c
