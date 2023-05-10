#include "../front/front.h"
#include "../util/util.h"
#include "../vulns/vulns.h"

void get_average_time_main_mem()
{	
	unsigned long average = 0;
	char* addr = "0x400000";

	for (int i = 0; i < 10000; i++) {
		average += probe_main_mem(addr);
	}

	average = average / 10000;
	
	char* output;
	itoa(average, output);

	print_string(output, 20, VGA_LIGHT_GRAY);
}

void get_average_time_l1()
{
	unsigned long average = 0;
	char* addr = "0x400000";

	for (int i = 0; i < 10000; i++) {
		average += probe_l1(addr);
	}

	average = average / 10000;
	
	char* output;
	itoa(average, output);

	print_string(output, 21, VGA_LIGHT_GRAY);
}

//QUESTION: why does the ip get lost when when the "threshold" var is uncommented?
int probe_main_mem(char* addr) {
	//const float threshold = 0.42;

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
		: "c" (addr)
		: "%esi", "%edx");
	
	return time;
}

int probe_l1(char* addr) {
	//const float threshold = 0.42;

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
		: "c" (addr)
		: "%esi", "%edx");
	
	return time;
}
