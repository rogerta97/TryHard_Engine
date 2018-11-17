
#include "Functions.h"
#include "PCGRandom\pcg_basic.h"
#include "Globals.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

int GetPCGRandomNumberInt(int min_value, int max_value)
{
	int num_generated = 0;
	int margin = max_value - min_value;
	
	num_generated = pcg32_boundedrand(margin);

	num_generated += min_value;

	return num_generated;
}

float GetPCGRandomNumberFloat(float max_value, float min_value)
{
	float num_generated = 0.0f;
	float margin_flt = max_value - min_value;

	margin_flt *= 100;
	int margin = (int)margin_flt;

	num_generated = pcg32_boundedrand(margin);

	float inc = (float)num_generated / 100;

	num_generated = min_value + inc;

	return num_generated;
}

float GetHigherNumber(float num1, float num2)
{
	if (num1 >= num2)
		return num1;

	else
		return num2; 
}

float GetHigherNumber(float num1, float num2, float num3)
{
	if (GetHigherNumber(num1, num2) == num2)
	{
		if (GetHigherNumber(num2, num3) == num3)
		{
			return num3;
		}

		return num2;
	}
	else if (GetHigherNumber(num1, num3) == num3)
		return num3; 

	else
		return num1; 		
}

bool AskUserIfSave() {
	return true;
}