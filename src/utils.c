#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

char *slurp_file(const char *path)
{
	FILE *f = fopen(path, "rt");
	if (!f) return NULL;

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *buf = calloc(size + 1, sizeof(char));
	if (!buf) return NULL;

	fread(buf, sizeof(char), size, f);

	fclose(f);	
	return buf;
}
