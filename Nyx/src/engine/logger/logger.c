# include <stddef.h>
# include <time.h>
# include <string.h>
# include <stdio.h>
# include "logger.h"
# include "../global.h"
# include "../io/io.h"


void logger_init(Log_Level log_level)
{
	global.log_level = log_level;
}
void logger_log_message(Log_Level log_level, char* message, ...)
{
	if (global.log_level < log_level) return;

	size_t message_len = strlen(message);
	if (message_len < 1 || message_len > 4096)
	{
		return;
	}

	char path[200];
	snprintf(path, sizeof(path), "%s\\log.log", global.prefs_directory);

	char time_string[32];
	time_t result = time(NULL);
	struct tm *tm = localtime(&result);
	snprintf(time_string, sizeof(time_string), "[%02u/%02u/%u - %02u:%02u:%02u] : ", tm->tm_mon, tm->tm_mday,
			tm->tm_year + 1900, tm->tm_hour, tm->tm_min, tm->tm_sec);

	char level_string[16];
	switch (log_level)
	{
		case LOG_LEVEL_NONE:
			return;
		case LOG_LEVEL_ERROR:
			strcpy(level_string, "\n[ERROR]");
			break;
		case LOG_LEVEL_WARNING:
			strcpy(level_string, "\n[WARN]");
			break;
		case LOG_LEVEL_INFO:
			strcpy(level_string, "\n[INFO]");
			break;
		case LOG_LEVEL_DEBUG:
			strcpy(level_string, "\n[DEBUG]");
			break;
		default:
			return;
	}

	char log_message[4096];
	va_list ap;
    va_start(ap, message);
	vsnprintf(log_message, sizeof(log_message), message, ap);
    va_end(ap);

	FILE* fp = fopen(path, "a+");
	if (!fp || ferror(fp))
	{
		return;
	}

	fputs(level_string, fp);
	fputs(time_string, fp);
	fputs(log_message, fp);

	fclose(fp);
}
