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

#define Header(...) {sprintf(tmp,__VA_ARGS__);strcat(header,tmp);}
#define Content(...) {sprintf(tmp,__VA_ARGS__);strcat(content,tmp);}

#define TRUE 1
#define FALSE 0
typedef unsigned short bool;

// ----- Conf -----

#define CONF_DEFAULT_PATH "../config/httpd.conf.default"
#define CONF_PATH "../config/httpd.conf"

struct conf_s{
	char servName[MAX_STRING_LEN];
	unsigned int port;
	char servDir[MAX_STRING_LEN];
	char logsDir[MAX_STRING_LEN];
	char servDocs[MAX_STRING_LEN];
	char allLogs[MAX_STRING_LEN];
	char allLogsPath[MAX_STRING_LEN];
	char connectsLogs[MAX_STRING_LEN];
	char connectsLogsPath[MAX_STRING_LEN];

	char adminEmail[MAX_STRING_LEN];

	bool rewriteEngine;
	bool comit;
};

typedef struct conf_s conf_t;

void Conf();
void ConfParse(char filepath[MAX_STRING_LEN]);

conf_t conf;

// ----- Logs -----

void AddLog(char type[MAX_STRING_LEN], char msg[MAX_STRING_LEN]);
void ViewLogs();
void DropLogs();

// ----- Server -----

struct request_s {
	char method[8];
	char url[64];
	char get[64];
	char httpVer[8];
	char host[64];
	char userAgent[128];
	char accept[64];
	char acceptLanguage[16];
	char acceptEncoding[16];
	char referer[64];
	char connection[64];
	char contentType[64];
	int contentLength;
	char cacheControl[32];
	char cookie[128];
	char post[64];
};

typedef struct request_s request_t;
request_t request;

int main(void);
void Work(int sock);
void ChildProcess();

// ----- Request Parse -----

void RequestParse(char buf[512]);
void RequestFirstParse(char buf[512]);
void RequestSecondParse(char* str);

// ----- Response -----


//void sendNotFound(int sockfd);