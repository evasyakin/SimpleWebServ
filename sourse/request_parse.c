/*
Request парсер

Заполняет структуру request результатом парсинга входящего запроса
*/

#include "server.h"

void RequestParse(char buf[512]){
	RequestFirstParse(buf);
	printf("Method: %s\n",request.method);
	printf("Url: %s\n",request.url);
	printf("Get: %s\n",request.get);
	printf("HttpVer: %s\n",request.httpVer);
	printf("Host: %s\n",request.host);
	printf("U-A: %s\n",request.userAgent);
	printf("Accept: %s\n",request.accept);
	printf("Accept-Lang: %s\n",request.acceptLanguage);
	printf("Accept-Enc: %s\n",request.acceptEncoding);
	printf("Referer: %s\n",request.referer);
	printf("Connection: %s\n",request.connection);
	printf("Content-Type: %s\n",request.contentType);
	printf("Content-Len: %d\n",request.contentLength);
	printf("CahcheControl: %s\n",request.cacheControl);
	printf("Cookie: %s\n",request.cookie);
	printf("Post: %s\n",request.post);
}

void RequestFirstParse(char buf[512]){
	char* pBegin = buf;
	char* pEnd = buf;
	int count = 0;
	printf("-----\n%s\n-----\n",buf);
	while(count < 20){
		//printf("\n>>>>\nStart%d: %s\n", count, pBegin);
		pEnd = strstr(pBegin,"\r\n");
		if(pEnd)
			*(pEnd) = '\0';
		else{
			int len = strlen(pBegin);
			if(len > 0){
				pEnd = pBegin + len;
				*(pEnd) = '\0';
				//printf("----\nFOUND!: %s\n----\n", pBegin);
				strcpy(request.post, pBegin);
			}
			else
				break;
		}
		//printf("Str%d: %s\n", count, pBegin);
		RequestSecondParse(pBegin);
		pBegin = pEnd+2;
		count++;
	}
}

void RequestSecondParse(char* str){
	if(strstr(str,"HTTP")){
		sscanf(str,"%s %s HTTP/%s",request.method,request.url,request.httpVer);
		char* get = strstr(request.url, "?");
		if(get != NULL){
			strcpy(request.get, get+1);
		}
	}
	if(strstr(str,"Host:")){
		strcpy(request.host, str+6);
	}
	else if(strstr(str,"User-Agent:")){
		strcpy(request.userAgent, str+12);
	}
	else if(strstr(str,"Accept:")){
		strcpy(request.accept, str+8);
	}
	else if(strstr(str,"Accept-Language:")){
		strcpy(request.acceptLanguage, str+17);
	}
	else if(strstr(str,"Accept-Encoding:")){
		strcpy(request.acceptEncoding, str+17);
	}
	else if(strstr(str,"Referer:")){
		strcpy(request.referer, str+9);
	}
	else if(strstr(str,"Connection:")){
		strcpy(request.connection, str+12);
	}
	else if(strstr(str,"Content-Type:")){
		strcpy(request.contentType, str+14);
	}
	else if(strstr(str,"Content-Length:")){
		sscanf(str,"Content-Length: %d",&request.contentLength);
	}
	else if(strstr(str,"Cache-Control:")){
		strcpy(request.cacheControl, str+15);
	}
}