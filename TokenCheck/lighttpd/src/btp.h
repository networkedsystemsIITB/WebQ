#include <string.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <syslog.h>
#include <string.h>

#include <openssl/engine.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SECRET_KEY "key"

int sockfd;
char buffr[256];
char * url;

int explode(char ***arr_ptr, char *str, char delimiter)
{ 
    // {{{
  char *src = str, *end, *dst;
  char **arr;
  int size = 1, i;

  // Find number of strings
  while ((end = strchr(src, delimiter)) != NULL)
    {
      ++size;
      src = end + 1;
    }

  arr = malloc(size * sizeof(char *) + (strlen(str) + 1) * sizeof(char));

  src = str;
  dst = (char *) arr + size * sizeof(char *);
  for (i = 0; i < size; ++i)
    {
      if ((end = strchr(src, delimiter)) == NULL)
        end = src + strlen(src);
      arr[i] = dst;
      strncpy(dst, src, end - src);
      dst[end - src] = '\0';
      dst += end - src + 1;
      src = end + 1;
    }
  *arr_ptr = arr;

  return size;
//}}}
} 

//{{{  Commented Block
/***  Written to verify token (if user has actually waited for that amount of time or not) **/
// returns 1 if token is correct

/*
int verify_token(char* query)
{
  char * pch;
  unsigned char token[100];
  unsigned char expected[100];
  char temp_query[300];
  strcpy(temp_query, query);
  pch = strtok (temp_query,"&=");
  while (pch != NULL)
  {
    if(strcmp(pch, "token") == 0) 
    {
      pch = strtok (NULL, "&=");
      strcpy(token, pch);
    }
    else if (strcmp(pch, "hash") == 0)
    {
      pch = strtok (NULL, "&=");    
      strcpy(expected, pch);
    }
    pch = strtok (NULL, "&=");    
  }
    
   
  unsigned char* key = (unsigned char*) SECRET_KEY;
  unsigned char* result;
  int result_len = 16;
  int i;
  char res_hexstring[32];

  result = HMAC(EVP_md5(), key, strlen(key), token, strlen(token), NULL, NULL);
  
  for (i = 0; i < result_len; i++) {
    sprintf(&(res_hexstring[i * 2]), "%02x", result[i]);
  }
  
  if (strcmp((char*) res_hexstring, (char*) expected) == 0) {
    return 1;
  } else {
    return 0;
  }
}
*/
//}}}


int verify_token(char* query)
{
    // {{{
  return 1;
  char * pch;
  unsigned char token[100];
  unsigned char expected[100];
  char temp_query[300];
  strcpy(temp_query, query);
  pch = strtok (temp_query,"&=");
  while (pch != NULL)
  {
    if(strcmp(pch, "token") == 0) 
    {
      pch = strtok (NULL, "&=");
      strcpy(token, pch);
    }
    else if (strcmp(pch, "hash") == 0)
    {
      pch = strtok (NULL, "&=");    
      strcpy(expected, pch);
    }
    pch = strtok (NULL, "&=");    
  }
    
   
  unsigned char* key = (unsigned char*) SECRET_KEY;
  unsigned char* result;
  int result_len = 16;
  int i;
  char res_hexstring[32];

  result = HMAC(EVP_md5(), key, strlen(key), token, strlen(token), NULL, NULL);
  
  for (i = 0; i < result_len; i++) {
    sprintf(&(res_hexstring[i * 2]), "%02x", result[i]);
  }
  
  time_t timer;
  time(&timer);
  //long long int current_time = (long long int) timer;
  //char** arr;
  //int size = explode(&arr, token, "%7C");

  //if(size !=3 ) return 0;
  
  //long long int req_time, wait_time;
  //sscanf(arr[1], "%d", &req_time);
  //sscanf(arr[2], "%d", &wait_time);
  //if(req_time + wait_time * 1000 < current_time )
  //  return 0;
  
  if (strcmp((char*) res_hexstring, (char*) expected) == 0) {
    return 1;
  } else {
    return 0;
  }
//}}}
} 


void connect_proxy1()
{
//{{{
    struct sockaddr_in serv_addr;
    struct hostent *servr;
    int portno = 5006;
    char* hostname = "10.129.41.17";

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        //log_error_write(srv, __FILE__, __LINE__,  "s", "chandancode: ERROR opening socket");
        return;
        //exit(1);
    }
    servr = gethostbyname(hostname);
    if (servr == NULL) {
        //log_error_write(srv, __FILE__, __LINE__,  "s", "chandancode: ERROR, no such host1");
        return;
        //exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)servr->h_addr, 
           (char *)&serv_addr.sin_addr.s_addr,
                servr->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
    {
         //log_error_write(srv, __FILE__, __LINE__,  "s", "chandancode: ERROR connecting1");
         return;
         //exit(1);
    }
//}}}
}

void disconnect_proxy1()
{
//{{{
    //close(sockfd);
//}}}
}


void inform_proxy1_arrival(char * query){
//{{{
    bzero (buffr, 256);
	if(strcmp(query,"req1.php")==0)
		strcpy(url,"0");
	else
		strcpy(url,"1");

    //strcpy(buffr, "A\n");//buffr[1]='\0'; 
	
	strcpy(buffr,"A_");
	strcat(buffr,(const char*) url);
	strcat(buffr,"\n");

    /* Send message to the server */
    write(sockfd,buffr,strlen(buffr));    
//}}}
}

void inform_proxy1_departure(unsigned long service_time, int http_status, char* query){
//{{{
    //char* c[50];
    //c = itoa(service_time);
    //strcpy(buffr, c);
    bzero (buffr, 256);
    strcpy(buffr, "" );
	if(strcmp(query,"req1.php")==0)
		strcpy(url,"0");
	else
		strcpy(url,"1");
    if ( http_status != 200 ){
        //strcpy(buffr, "F\n");
		
		strcpy(buffr,"F_");
		strcat(buffr,(const char*) url);
		strcat(buffr,"\n");

    }
    else {
        sprintf(buffr, "%ld\n", service_time);
		strcat(buffr,"_");
		strcat(buffr,(const char*) url);
    }
    //strcat( buffr, "," );
    /* Send message to the server */
    write(sockfd,buffr,strlen(buffr));
    //fflush(sockfd);
//}}}
}

