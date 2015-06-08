/*
Файл работы с логами сервера

Добавление лога, просмотр списка логов. Удаление списка логов
*/

#include "server.h"

void AddLog(char type[MAX_STRING_LEN], char msg[MAX_STRING_LEN]){
	char str[MAX_STRING_LEN];
	sprintf(str, "%s: %s\n", type, msg);
	FILE *file = fopen(conf.allLogs,"a");
	if(file == NULL){
		printf("Не могу открыть файл %s :(\n", conf.allLogsPath);
	}
	else{
		fprintf(file, str);
		fclose(file);
	}
	printf(str);
	if(strstr(str, "Err")) {
		exit(1);
	}
}

void ViewLogs(){
	char str[MAX_STRING_LEN];
	FILE *file = fopen(conf.allLogs,"r");
	if(file == NULL){
		AddLog("Error","Не могу открыть файл логов");
	}
	else{
		while(fgets(str,sizeof(str),file)){
			printf(str);
		}
		fclose(file);
	}
}

void DropLogs(){
	FILE *file = fopen(conf.allLogs,"wb");
	if(file == NULL){
		AddLog("Error","Не могу открыть файл логов");
	}
	else{
		fclose(file);
		printf("Логи очищены\n");
	}
}