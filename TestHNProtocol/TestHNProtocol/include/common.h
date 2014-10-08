#include <stdio.h>
#include <stdlib.h>

typedef unsigned char _byte_;

#define AGENT_MALLOC(to, size) to = (char*)malloc(size)
#define AGENT_REALLOC(tmp, to, new_size) do { \
	if(to) {\
	tmp = (char*)realloc(to, new_size); \
	if(tmp) \
	to = tmp; \
	} else { \
	tmp = NULL; \
	} \
} while(0)

#define _MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define _MIN(a,b)    (((a) < (b)) ? (a) : (b))

typedef struct {
	int buffer_size;
	int data_len;
	char *data;

	void _char_buffer()
	{
		buffer_size = 0;
		data_len = 0;
		data = NULL;
	}

} _char_buffer;

// type convert
void int_to_byte4(int in, char *out)
{
	int tmp = in;
	int j = 0;
	char tmp_byte = 0x00;

	if (NULL == out) {
		return;
	}
	for (j = 3; 0<= j; j--) {
		out[j] = tmp_byte | tmp;
		tmp >>= 8;
	}
}

int byte4_to_int(char *in)
{
	int ret = 0;
	int j = 0;
	if (NULL == in) {
		return ret;
	}
	for (j = 0; j < 3; j++) {
		ret |= in[j];
		ret <<= 8;
	}
	ret |= in[j];
	return ret;
}


// memory
char *adjust_buffer_size_1(_char_buffer *buf, int need_size)
{
	char *res = NULL;
	int len;
	if(need_size > 0) {
		if(buf->data == NULL) {
			len = _MAX(need_size, 512);
			AGENT_MALLOC(res, len);
		} else if(buf->buffer_size - buf->data_len < need_size) {
			len = buf->data_len + _MAX(need_size, 512);
			AGENT_REALLOC(res, buf->data, len);
		} else {
			return buf->data + buf->data_len;
		}

		if(res == NULL) {
			return NULL;
		} else {
			buf->data = res;
			buf->buffer_size = len;
			res = buf->data + buf->data_len;
		}
	}

	return res;
}

char *adjust_buffer_size(char *buf, int need_size)
{
	char *res = NULL;
	int len = 0;
	if(need_size > 0) {
		if(NULL == buf) {
			len = _MAX(need_size, 512);
			res = (char *)malloc(need_size);
		} else {
			len = _msize(buf);
			if (need_size > len) {
				res = (char*)realloc(buf, need_size);
			}
		}

		if(res == NULL) {
			return NULL;
		} else {
			buf = res;
		}
	}

	return res;
}

// FILE IO
enum {
	MEMORY_STACK = 0,	// Õ»
	MEMORY_DYNAMIC,		// ¶Ñ
};
int save_buffer_to_file(char *buf, const char *file_name, const char *mode, int buf_memory_kind)
{
	FILE *pf = NULL;
	int buf_len = 0;
	int erro = 0;
	char *buf_null = { "NULL" };
	
	if (NULL == buf) {
		buf = buf_null;
	}
	switch (buf_memory_kind) {
	case MEMORY_STACK:
		buf_len = sizeof(buf);
		break;
	case MEMORY_DYNAMIC:
		buf_len = _msize(buf);
		break;
	}	
	if (0 == buf_len) {
		return 0;
	}
	erro = fopen_s(&pf, file_name, mode);
	if (0 != erro || NULL == pf) {
		return 0;
	}
	fseek(pf, 0, SEEK_END);
	char *p_new_line[] = { "\n", {"------------------------------------------------------"} };
	fwrite(p_new_line[0], strlen(p_new_line[0]), 1, pf);
	fwrite(p_new_line[1], strlen(p_new_line[1]), 1, pf);
	fwrite(p_new_line[0], strlen(p_new_line[0]), 1, pf);

	fwrite(buf, buf_len, 1, pf);
	fclose(pf);
	pf = NULL;

	return 1;
}

char* read_file_to_buffer(const char *file_name)
{
	char *buf = NULL;
	FILE *pf = NULL;
	int erro = 0;
	int file_len = 0;

	erro = fopen_s(&pf, file_name, "r+b");
	if (0 != erro || NULL == pf) {
		return buf;
	}
	fseek(pf, 0, SEEK_END);
	file_len = ftell(pf);
	fseek(pf, 0, SEEK_SET);
	buf = (char *)malloc(sizeof(char) * file_len);
	memset(buf, '\0', file_len);
	fread(buf, file_len, 1, pf);
	fclose(pf);
	pf = NULL;
	return buf;
}