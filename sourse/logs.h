
#define ALL_LOGS "logs/all.log"
#define CONNECTS "logs/connects.log"

void AddLog(char type[MAX_STRING_LEN], char msg[MAX_STRING_LEN]);
void ViewLogs();
void DropLogs();