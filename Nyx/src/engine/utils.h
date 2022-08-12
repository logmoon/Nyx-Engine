# pragma once
# include <stdio.h>
# include "logger/logger.h"

# define ERROR_EXIT(...) { logger_log_message(LOG_LEVEL_ERROR, __VA_ARGS__); exit(1); }
# define ERROR_RETURN(R, ...) { logger_log_message(LOG_LEVEL_ERROR, __VA_ARGS__); return R; }
# define WARN_RETURN(R, ...) { logger_log_message(LOG_LEVEL_WARNING, __VA_ARGS__); return R; }
# define LOG_DEBUG(...) { logger_log_message(LOG_LEVEL_DEBUG, __VA_ARGS__); }
# define LOG_INFO(...) { logger_log_message(LOG_LEVEL_INFO, __VA_ARGS__); }
# define LOG_WARNING(...) { logger_log_message(LOG_LEVEL_WARNING, __VA_ARGS__); }
