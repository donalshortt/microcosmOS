#include "../front/front.h"

void get_average_time_main_mem()
{
	
}

void get_average_time_l1()
{
	unsigned long average;

	for (int i = 0; i < 10000; i++) {
		average += probe_l1();
	}

	print_string("AVERAGE L1 TIME:" + average.itoa(), 20, VGA_LIGHT_GRAY);
}

int probe_main_mem(char *adrs) {
	const float threshold = 0.42;

	volatile unsigned long time;

	asm __volatile__ (
		" mfence \n"
		" lfence \n"
		" rdtsc \n"
		" lfence \n"
		" movl %%eax, %%esi \n"
		" movl (%1), %%eax \n"
		" lfence \n"
		" rdtsc \n"
		" subl %%esi, %%eax \n"
		" clflush 0(%1) \n"
		: "=a" (time)
		: "c" (adrs)
		: "%esi", "%edx");
	
	return time;
}

int probe_l1(char *adrs) {
	const float threshold = 0.42;

	volatile unsigned long time;

	asm __volatile__ (
		" mfence \n"
		" lfence \n"
		" rdtsc \n"
		" lfence \n"
		" movl %%eax, %%esi \n"
		" movl (%1), %%eax \n"
		" lfence \n"
		" rdtsc \n"
		" subl %%esi, %%eax \n"
		: "=a" (time)
		: "c" (adrs)
		: "%esi", "%edx");
	
	return time;
}
