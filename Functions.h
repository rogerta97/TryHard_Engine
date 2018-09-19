
#include "PCGRandom\pcg_basic.h"
#include <time.h>
#include <math.h>

int GetPCGRandomNumberInt(int max_value, int min_value)
{
	int num_generated = 0;
	int margin = max_value - min_value; 

	pcg32_random_t rng; 

	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, 5);
	num_generated = pcg32_boundedrand_r(&rng, margin);

	num_generated += min_value; 

	return num_generated; 
}

float GetPCGRandomNumberFloat(float max_value, float min_value)
{
	float num_generated = 0.0f;
	float margin_flt = max_value - min_value;

	margin_flt *= 100;
	int margin = (int)margin_flt; 

	pcg32_random_t rng;
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, 5);
	num_generated = pcg32_boundedrand_r(&rng, margin);

	float inc = (float)num_generated / 100;

	num_generated = min_value + inc;

	return num_generated;
}