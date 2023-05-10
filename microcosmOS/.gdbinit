file kernel
target remote localhost:1234
set print pretty on
set confirm off
source /home/donal/repos/gdb-pt-dump/pt.py

br core/kernel.c:25

#br vulns/vulns.c:10

tui enable

c

