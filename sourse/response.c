/*
Response

Формирует и отправляет ответ клиенту
*/

#include "server.h"

void sendNotFound(int sockfd) {

	char *responseString = 
		"HTTP/1.1 404 Not Found\n"
		"Content-type: text/html\n"
		"\n"
		"<html>\n"
		" <body>\n"
		"  <h1>Not Found</h1>\n"
		"  <p>The requested URL was not found on this server.</p>\n"
		" </body>\n"
		"</html>\n";

	write(sockfd, responseString, strlen(responseString));
}