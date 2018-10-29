#pragma once

#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

using namespace std;

int GetPCGRandomNumberInt(int max_value, int min_value);
float GetPCGRandomNumberFloat(float max_value, float min_value);
bool AskUserIfSave();

