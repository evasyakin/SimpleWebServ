
#define CONF_DEFAULT_PATH "conf/httpd.conf.default"
#define CONF_PATH "conf/httpd.conf"

struct conf_s{
	char servName[MAX_STRING_LEN];
	int Port;
	char servDir[MAX_STRING_LEN];
	char logsDir[MAX_STRING_LEN];
	char servDocs[MAX_STRING_LEN];
	char errDocs[MAX_STRING_LEN];

	char adminEmail[MAX_STRING_LEN];
};

typedef struct conf_s conf_t;

void Conf(char filepath[MAX_STRING_LEN]);
void ConfParse();