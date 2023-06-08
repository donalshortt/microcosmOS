file kernel
target remote localhost:1234
set print pretty on
set confirm off
source /home/donal/repos/gdb-pt-dump/pt.py

#br vulns/flush_reloead/flush_reload.c:48
br vulns/spectre/spectre.c:132


#br vulns/vulns.c:10

tui enable

c

