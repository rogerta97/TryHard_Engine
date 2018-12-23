#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

using namespace std;

int GetPCGRandomNumberInt(int max_value, int min_value);
float GetPCGRandomNumberFloat(float max_value, float min_value);
float GetHigherNumber(float num1, float num2);
float GetHigherNumber(float num1, float num2, float num3);
bool AskUserIfSave();

#endif