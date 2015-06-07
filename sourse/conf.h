
#define CONF_DEFAULT_PATH "conf/httpd.conf.default"
#define CONF_PATH "conf/httpd.conf"

struct conf_s{
	char ServName[MAX_STRING_LEN];
	int Port;
	char ServDir[MAX_STRING_LEN];
	char LogsDir[MAX_STRING_LEN];
	char ServDocs[MAX_STRING_LEN];
	char ErrDocs[MAX_STRING_LEN];

	char AdminEmail[MAX_STRING_LEN];
};

typedef struct conf_s conf_t;

void Conf(char filepath[MAX_STRING_LEN]);
void ConfParse();