# pragma once
# include <stdlib.h>
# include <stdbool.h>

typedef struct file
{
	char* data;
	size_t len;
	bool is_valid;

}File;

bool io_init(char* comany_name, char* application_name);
File io_file_read(const char* path);
bool io_file_write(void* buffer, size_t size, const char* path);
void io_shutdown(void);
