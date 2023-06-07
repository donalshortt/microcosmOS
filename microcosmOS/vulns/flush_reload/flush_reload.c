#include "flush_reload.h"
#include "../../front/front.h"
#include "../vulns.h"
#include "../../util/types.h"
#include "../../util/util.h"
#include "../../mm/mm.h"

void flush_reload()
{
	int cache_threshold = determine_threshold();

	char* secret = kmalloc(100);
	strcpy("my secret", secret);

	fr_victim(secret);
	fr_attacker(cache_threshold, secret);
}

void fr_victim(char* addr) {
    // Read the secret string in a loop
    char* secret = addr;  // Let's assume the secret is 8 characters long
}

void fr_attacker(int threshold, char* addr) {
    uint64_t time;
	
	// get time taken to access the secret
    time = probe_l1(addr);

    if (time <= threshold) {
        print_string("The cache hit time was below the threshold\n", 15, VGA_LIGHT_GRAY);
    } else {
        print_string("The cache hit time was above the threshold\n", 15, VGA_LIGHT_GRAY);
    }
}
