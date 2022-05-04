file kernel
target remote localhost:1234
set print pretty on

br kernel.c:46
br kernel.c:51
c
