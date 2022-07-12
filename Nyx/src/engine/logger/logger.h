# pragma once

typedef enum log_level
{
	LOG_LEVEL_NONE,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG

}Log_Level;

void logger_init(Log_Level log_level);
void logger_log_message(Log_Level log_level, char* message, ...);
