file kernel
target remote localhost:1234
set print pretty on
set confirm off

br test/pmm_test.c:96
c

