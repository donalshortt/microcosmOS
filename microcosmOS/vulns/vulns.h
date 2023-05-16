#ifndef __VULNS_H__
#define __VULNS_H__

unsigned long get_average_time_l1();
unsigned long get_average_time_main_mem();


int probe_main_mem(char* addr);
int probe_l1(char* addr);

#endif
