#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_STRING_LEN 80

#include "conf.h"
#include "logs.h"

struct header_s {
	char method[4];
	char url[64];
	char protocol[4];
	char host[64];
	char userAgent[128];
	char accept[64];
	char connection[64];
	char referer[64];
	int contentLength;
	char contentType[16];
	char cookie[128];
	char cacheControl[32];
};

typedef struct header_s header_t;

void main();