#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "fcgi_stdio.h"
#include "queue.h"

#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <syslog.h>
#include <string.h>
#include <pthread.h>
#include <openssl/engine.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define SECRET_KEY  "key"
#define RATE 60
#define bool int
#define true 1
#define false 0

const int expiration_time = 1500;

char* encrypt(char* s) {
    return s;
}

char* decrypt(char* s) {
    return s;
}

char* get_token(int time_to_wait) {
    char* s;
    s = (char *) malloc(50 * sizeof(char));
    time_t result;
    extern char **environ;
    const char *addr = FCGX_GetParam("REMOTE_ADDR", environ);

    result = time(NULL);

    sprintf(s, "%s%s%ld%s%d", addr, "%7C", result, "%7C", time_to_wait);
    return s;
}

char* getHash(unsigned char *data) {
    unsigned char* key = (unsigned char *) SECRET_KEY; //"key";
    unsigned char* result;
    unsigned int result_len = 16;
    int i;
    static char res_hexstring[32];

    result = HMAC(EVP_md5(), key, strlen(key), data, strlen(data), NULL, NULL);
    for (i = 0; i < result_len; i++) {
        sprintf(&(res_hexstring[i * 2]), "%02x", result[i]);
    }

    return ((char*) res_hexstring);
}

pthread_mutex_t serialize_values = PTHREAD_MUTEX_INITIALIZER;
void change_values(int* variable, int num) {   // n can be +1 or -1
    pthread_mutex_lock(&serialize_values);
    if (num == 0)
        *variable = 0;
    else
        *variable = *variable + num;
    pthread_mutex_unlock(&serialize_values);
}

pthread_mutex_t serialize_values3 = PTHREAD_MUTEX_INITIALIZER;
void change_long_values(unsigned long* variable, unsigned long num) { // n can be +1 or -1
    pthread_mutex_lock(&serialize_values3);
    if (num == 0)
        *variable = 0;
    else
        *variable = *variable + num;
    pthread_mutex_unlock(&serialize_values3);
}
long get_values(unsigned int* variable) {   // n can be +1 or -1
    pthread_mutex_lock(&serialize_values3);
    long temp = *variable;
    pthread_mutex_unlock(&serialize_values3);
    return temp;
}
pthread_mutex_t serialize_values2 = PTHREAD_MUTEX_INITIALIZER;
void change_float_values(float* variable, float num, float min_val) { // n can be +1 or -1
    pthread_mutex_lock(&serialize_values2);
    if (num == 0.0)
        *variable = 0.0;
    if ((*variable + num) > min_val)
        *variable = (*variable + num);
    else
        *variable = min_val;
    pthread_mutex_unlock(&serialize_values2);
}

pthread_mutex_t serialize_values4 = PTHREAD_MUTEX_INITIALIZER;
void update_array(int* variable, int num) {   // n can be +1 or -1
    pthread_mutex_lock(&serialize_values4);
    if (num == 0)
        *variable = 0;
    else
        *variable = (*variable + num);
    pthread_mutex_unlock(&serialize_values4);
}
void set_array(int* variable, int num) {
    pthread_mutex_lock(&serialize_values4);
    *variable = num;
    pthread_mutex_unlock(&serialize_values4);
}
int get_array(int* variable) {   // n can be +1 or -1
    pthread_mutex_lock(&serialize_values4);
    int temp = *variable;
    pthread_mutex_unlock(&serialize_values4);
    return temp;
}

#endif
