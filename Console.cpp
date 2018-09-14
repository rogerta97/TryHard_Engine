#include "Console.h"



Console::Console()
{
	log_count = 0; 
	error_count = 0;
	debug_count = 0;
}


Console::~Console()
{
}

void Console::AddToConsole(const char* new_message, Console_Message_Type type)
{
	if (type == CONSOLE_MLOG) log_count++; 
	else if (type == CONSOLE_MDEBUG) debug_count++;
	else if (type == CONSOLE_MERROR) error_count++;

	console_buffer.insert(std::pair<Console_Message_Type, const char*>(type, new_message)); 
}

std::multimap<Console_Message_Type, const char*> Console::GetBuffer() const
{
	return console_buffer;
}

uint Console::GetLogCount() const
{
	return log_count;
}

uint Console::GetDebugCount() const
{
	return debug_count;
}

uint Console::GetErrorCount() const
{
	return error_count;
}

void Console::Clear()
{
	console_buffer.clear();
	log_count = debug_count = error_count = 0; 
}