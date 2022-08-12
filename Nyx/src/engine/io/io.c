# include <stdio.h>
# include <errno.h>
#include <stdlib.h>

# include "io.h"
# include "../global.h"
# include "../utils.h"

# define IO_READ_CHUNK_SIZE 2097152 // 20 Mib
# define IO_READ_ERROR_GENERAL "Error reading file: %s. error: %d"
# define IO_READ_ERROR_MEMORY "Not enough memory to read file: %s"

bool io_init(char* company_name, char* application_name)
{
	// Getting the prefs path
	char *base_path = SDL_GetPrefPath(company_name, application_name);

	if (base_path)
	{
        global.prefs_directory = SDL_strdup(base_path);
        SDL_free(base_path);
    }
	else
	{
		printf("\nCouldn't create and initialize prefs path");
		return false;
	}

	return true;
}

File io_file_read(const char* path)
{
	File file = { .is_valid = false };

	FILE* fp = fopen(path, "rb");

	if (!fp || ferror(fp))
	{
		ERROR_RETURN(file, IO_READ_ERROR_GENERAL, path, errno);
	}

	char* data = NULL;
	char* tmp;
	size_t used = 0;
	size_t size = 0;
	size_t n;

	while (true)
	{
		if (used + IO_READ_CHUNK_SIZE + 1 > size)
		{
			size = used + IO_READ_CHUNK_SIZE + 1;

			if (size <= used)
			{
				free(data);
				ERROR_RETURN(file, "\nInput file is too large: %s", path);
			}

			tmp = realloc(data, size);
			if (!tmp)
			{
				free(data);
				ERROR_RETURN(file, IO_READ_ERROR_MEMORY, path);
			}
			data = tmp;
		}

		n = fread(data + used, 1, IO_READ_CHUNK_SIZE, fp);
		// End of the file reached
		if (n == 0) break;

		used += n;
	}

	if (ferror(fp))
	{
		free(data);
		ERROR_RETURN(file, IO_READ_ERROR_GENERAL, path, errno);
	}

	tmp = realloc(data, used + 1);
	if (!tmp)
	{
		free(data);
		ERROR_RETURN(file, IO_READ_ERROR_MEMORY, path);
	}
	data = tmp;
	data[used] = 0;

	file.data = data;
	file.len = used;
	file.is_valid = true;

	return file;
}

bool io_file_write(void* buffer, size_t size, const char* path)
{
	FILE* fp = fopen(path, "Wb");
	if (!fp || ferror(fp))
	{
		ERROR_RETURN(false, "\nCannot write file: %s", path);
	}

	size_t chunks_written = fwrite(buffer, size, 1, fp);

	fclose(fp);

	if (chunks_written != 1)
	{
		ERROR_RETURN(false, "\nWriting into file: %s error. %s %zu", path, "\nExpected 1 chunk, got", chunks_written);
	}

	return true;
}

void io_shutdown(void)
{
}
