
#include "PCGRandom\pcg_basic.h"
#include <time.h>

float GetPCGRandomNumber(float max_value, float min_value)
{
	int num_generated = 0;
	float margin = max_value - min_value; 

	pcg32_random_t rng; 

	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, 5);
	num_generated = pcg32_boundedrand_r(&rng, margin);

	num_generated += min_value; 

	return num_generated; 
}