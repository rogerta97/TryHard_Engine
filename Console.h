#pragma once

enum Console_Message_Type { CONSOLE_MLOG, CONSOLE_MDEBUG, CONSOLE_MERROR};

#include <map>
#include "Globals.h"

class Console
{
public:
	Console();
	~Console();

	void AddToConsole(const char* new_message, Console_Message_Type type);
	std::multimap<Console_Message_Type, const char*> GetBuffer() const; 

	std::multimap<Console_Message_Type, const char*> console_buffer;

	uint GetLogCount() const; 
	uint GetDebugCount() const;
	uint GetErrorCount() const;

	void Clear(); 

private: 

	unsigned int log_count, debug_count, error_count; 
};

