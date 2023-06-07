#include "../front/front.h"
#include "../util/util.h"
#include "../vulns/vulns.h"
#include "../util/types.h"

unsigned long get_average_time_main_mem()
{	
	unsigned long average = 0;
	char* addr = "0x400000";

	for (int i = 0; i < 100000; i++) {
		average += probe_main_mem(addr);
	}

	average = average / 100000;
	
	char* output = "";
	itoa(average, output);

	print_string(output, 20, VGA_LIGHT_GRAY);

	return average;
}

unsigned long get_average_time_l1()
{
	unsigned long average = 0;
	char* addr = "0x400000";

	for (int i = 0; i < 100000; i++) {
		average += probe_l1(addr);
	}

	average = average / 100000;
	
	char* output = "";
	itoa(average, output);

	print_string(output, 21, VGA_LIGHT_GRAY);

	return average;
}

int probe_main_mem(char* addr) {
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

void flush(char* addr)
{
	asm __volatile__ (
		"clflush 0(%0)" 
		: 
		: "r"(addr) 
		: "rax"
	);
}

unsigned int read_timestamp()
{
	int time;
	asm __volatile__ (	
		" mfence \n"
		" lfence \n"
		" rdtsc \n"
		: "=a" (time)
		:
		: "%edx");

    /*uint32_t high, low;
    asm __volatile__ ("rdtscp" 
			: "=a"(low), "=d"(high) 
			:
			: "%rcx"
	);
	return ((uint64_t)high << 32) | low;*/
}

int determine_threshold()
{
	int average_l1 = get_average_time_l1();
	int average_main = get_average_time_main_mem();

	return (average_main + average_l1) / 2;
}
