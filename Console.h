#pragma once

enum Console_Message_Type { CONSOLE_MLOG, CONSOLE_MDEBUG, CONSOLE_MERROR};

#include <map>
#include "Globals.h"

class Console
{
public:
	Console();
	~Console();

	void AddToConsole(std::string new_message, Console_Message_Type type);
	std::multimap<Console_Message_Type, std::string> GetBuffer() const;

	std::multimap<Console_Message_Type, std::string> console_buffer;

	uint GetLogCount() const; 
	uint GetDebugCount() const;
	uint GetErrorCount() const;

	void Clear(); 

private: 

	unsigned int log_count, debug_count, error_count; 
};

