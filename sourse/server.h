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

// Main

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

// Conf

#define CONF_DEFAULT_PATH "conf/httpd.conf.default"
#define CONF_PATH "conf/httpd.conf"

struct conf_s{
	char servName[MAX_STRING_LEN];
	int port;
	char servDir[MAX_STRING_LEN];
	char logsDir[MAX_STRING_LEN];
	char servDocs[MAX_STRING_LEN];
	char errDocs[MAX_STRING_LEN];

	char adminEmail[MAX_STRING_LEN];
};

typedef struct conf_s conf_t;

void Conf();
void ConfParse(char filepath[MAX_STRING_LEN]);

// Logs


#define ALL_LOGS "logs/all.log"
#define CONNECTS "logs/connects.log"

void AddLog(char type[MAX_STRING_LEN], char msg[MAX_STRING_LEN]);
void ViewLogs();
void DropLogs();