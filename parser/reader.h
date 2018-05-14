#define IO_ERROR 0
#define IO_SUCCESS 1

int read_char(void);
int unread_char();
int open_input_stream(char *file_name);
void close_input_stream();
