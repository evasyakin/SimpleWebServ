/*
Парсер файла конфигурации ../conf/http.conf

В результате его работы формируется объект conf, который содержит все важнейшие данные конфигурации сервера
*/

#include "conf.h"

conf_t conf;

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
		// printf("%s",str);
		if(strstr(str, "ServName")){
			sscanf(str,"ServName = %s", conf.ServName);
		}
		if(strstr(str,"Port")){
			sscanf(str,"Port = %d", &conf.Port);
		}
		if(strstr(str, "ServDir")){
			sscanf(str,"ServDir = %s", conf.ServDir);
		}
		if(strstr(str, "LogsDir")){
			sscanf(str,"LogsDir = %s", conf.LogsDir);
		}
		if(strstr(str, "ServDocs")){
			sscanf(str,"ServDocs = %s", conf.ServDocs);
		}
		if(strstr(str, "ErrDocs")){
			sscanf(str,"ErrDocs = %s", conf.ErrDocs);
		}
		if(strstr(str, "AdminEmail")){
			sscanf(str,"AdminEmail = %s", conf.AdminEmail);
		}

	}
	fclose(file);
}