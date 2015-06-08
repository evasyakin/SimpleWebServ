/*
Парсер файла конфигурации ../conf/http.conf

В результате его работы формируется объект conf, который содержит все важнейшие данные конфигурации сервера
*/

#include "server.h"

void Conf(){
	ConfParse(CONF_DEFAULT_PATH);
	ConfParse(CONF_PATH);
}

void ConfParse(char filepath[MAX_STRING_LEN]){
	char str[MAX_STRING_LEN];
	char* comment_pos;
	FILE *file = fopen(filepath,"r");
	if(file == NULL){
		AddLog("Error","Не могу открыть файл конфигурации");
	}
	while(fgets(str,sizeof(str),file)){
		if((comment_pos = strstr(str,"#")) != NULL)
			*comment_pos = '\0';
		// Server Data
		if(strstr(str, "ServName")){
			sscanf(str,"ServName = %s", conf.servName);
		}
		if(strstr(str, "Port")){
			sscanf(str,"Port = %d", &conf.port);
		}
		if(strstr(str, "ServDir")){
			sscanf(str,"ServDir = %s", conf.servDir);
		}
		if(strstr(str, "LogsDir")){
			sscanf(str,"LogsDir = %s", conf.logsDir);
		}
		if(strstr(str, "ServDocs")){
			sscanf(str,"ServDocs = %s", conf.servDocs);
		}
		if(strstr(str, "AllLogs")){
			sscanf(str,"AllLogs = %s", conf.allLogs);
		}
		if(strstr(str, "ConnectsLogs")){
			sscanf(str,"ConnectsLogs = %s", conf.connectsLogs);
		}
		// Admin Data
		if(strstr(str, "AdminEmail")){
			sscanf(str,"AdminEmail = %s", conf.adminEmail);
		}
		// Load Modules
		if(strstr(str, "LoadModule")){
			//sscanf(str,"LoadModule %s \"%s\""); <---
		}
		// Launch Modules
		if(strstr(str, "RewriteEngine")){
			sscanf(str,"RewriteEngine = %d", &conf.rewriteEngine);
		}
		if(strstr(str, "Comit")){
			sscanf(str,"Comit = %d", &conf.rewriteEngine);
		}
		strcpy(conf.allLogsPath, "../");
		strcat(conf.allLogsPath, conf.logsDir);
		strcat(conf.allLogsPath, conf.allLogs);
		strcpy(conf.connectsLogsPath, "../");
		strcat(conf.connectsLogsPath, conf.logsDir);
		strcat(conf.connectsLogsPath, conf.connectsLogs);
	}
	fclose(file);
}