/*
Файл работы с логами сервера

Добавление лога, просмотр списка логов. Удаление списка логов
*/

#include "logs.h"

char msg[MAX_STRING_LEN];

void AddLog(char type[MAX_STRING_LEN], char msg[MAX_STRING_LEN]){
	char str[MAX_STRING_LEN];
	sprintf(str, "%s: %s\n", type, msg);
	FILE *file = fopen(ALL_LOGS,"a");
	if(file == NULL){
		printf("Не могу открыть файл ALL_LOG :(\n");
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
	FILE *file = fopen(ALL_LOGS,"r");
	if(file == NULL){
		AddLog("Error","Не могу открыть файл ALL_LOG");
	}
	else{
		while(fgets(str,sizeof(str),file)){
			printf(str);
		}
		fclose(file);
	}
}

void DropLogs(){
	FILE *file = fopen(ALL_LOGS,"wb");
	if(file == NULL){
		AddLog("Error","Не могу открыть файл ALL_LOG", );
	}
	else{
		fclose(file);
		printf("Логи очищены\n");
	}
}