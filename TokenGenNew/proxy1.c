#include "functions.h"
#include "timer.h"
#include "parser.h"
#include <netinet/tcp.h>

char log_format_string[256];
int portno;
char ** ip_array ;

void start_logging() {/*{{{*/
    init_logger();
    start_timer();
}/*}}}*/

int talkToClient( int clientSocketFD ) {/*{{{*/
    // make buffer to store the data from client
    char buffer[256];
    bzero(buffer,256);
    while( read( clientSocketFD, &buffer, 255) > 0)
    {
        capacity = atol(buffer);
        bzero(buffer,256);
    }
    debug_log( " gonna shutdown thread " );
    // 2- shutdown both send and recieve functions
    shutdown(clientSocketFD, 2);
}/*}}}*/

void * ThreadWorker( void * threadArgs) {/*{{{*/
    int clientSocketFD = (int) threadArgs;
    sprintf( log_format_string , "yo yo thread id %d\n", (int) threadArgs );
    debug_log( log_format_string );
    talkToClient( threadArgs);
    pthread_exit(NULL);
}/*}}}*/

void* create_server_socket() { /*{{{*/
    //
    // Creates server socket for communication between Proxy1 and Proxy2
    //
    int sockfd, newsockfd,  clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int bytes_read_count;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    /* portno = 5007; */


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
     * go in sleep mode and will wait for the incoming connection
     */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    int option = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*) &option,
            sizeof(option)) < 0) {
        printf("setsockopt failed\n");
        close(sockfd);
        exit(2);
    }
    //setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &option, sizeof(int));
    /* Accept actual connection from the client */
    pthread_t threadId;
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }
        if(pthread_create(&threadId, NULL, ThreadWorker, (void *)newsockfd) < 0)
        {
            debug_log("Thread creation failed");
            exit(1);
        }
        
        /* If connection is established then start communicating */

        /* bzero(buffer, 256); */
        /* bytes_read_count = read(newsockfd, buffer, 255); */
        /* if (bytes_read_count <= 0) { */
        /*     perror("ERROR reading from socket. Waiting for TokenCheck to reconnect"); */
        /*     debug_log( "bytes_read_count < 0 " ); */
        /*     /1* Accept actual connection from the client *1/ */
        /*     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); */
        /*     continue; */
        /* } */
//      fprintf(log_ptr, "New capacity received from java: %s", buffer);
//      char* pch = strtok(buffer, " ,.-");
//      while (pch != NULL) {
            //printf ("%s\n",pch);
            //fprintf(log_ptr, "%s\n", pch);
            //fflush(log_ptr);
//          if (strcmp(pch, "A") == 0) {
//              proxy2_in++;
//          } else if (strcmp(pch, "F") == 0) {
//              change_values(&failing, 1);
//              change_values(&outgoing, 1);
//              //change_float_values(&alpha, penalty, initial_alpha);
//          } else {
                /* capacity = atol(buffer); */
//              change_values(&outgoing, 1);
                //change_long_values(&total_service_time, wait_time);
//              total_out++;
//          }
//          pch = strtok(NULL, " ,.-");
//      }

    }
}/*}}}*/

#define MAX_INPUT_SIZE 256

void timed_q_function(int fd, short event, void *arg) { // Called every second
    talkToServer();
    start_q_timer();
}

void start_q_timer() {
    struct event ev;
    struct timeval tv;

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    event_init();
    evtimer_set(&ev, timed_q_function, NULL);
    evtimer_add(&ev, &tv);
    event_dispatch();
}

void talkToServer(){/*{{{*/
    int sockfd, portnum, n;
    struct sockaddr_in server_addr;
    char * port = malloc( 5 * sizeof(char) );
    char * ip_str = malloc( 20 * sizeof(char) );
    strcpy( ip_str , ip_array[0] );
    strcpy( port , "10001" );
    char inputbuf[MAX_INPUT_SIZE];
    portnum = atoi(port);
    /* Create client socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        debug_log( "ERROR opening socket\n");
        exit(1);
    }
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(!inet_aton( ip_str  , &server_addr.sin_addr))
    {
        debug_log( "ERROR invalid server IP address\n");
        exit(1);
    }
    server_addr.sin_port = htons(portnum);
    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) >= 0) 
    {
        while( 1)
        {
            bzero(inputbuf,MAX_INPUT_SIZE);
            strcpy(inputbuf, "123");
            n = write(sockfd,inputbuf,strlen(inputbuf));
            if (n < 0) 
            {
                debug_log("ERROR writing to socket\n");
                /* exit(1); */
                break;
            }
            // connect as a client;
            sleep(5);
        }
    }
    else{
        debug_log( "ERROR connecting\n");
        /* sleep(1); */
        /* exit(1); */
    }

}/*}}}*/

void queue_sender( void * args) {/*{{{*/
    sleep(1);
    start_q_timer();
    /* talkToServer(); */
}/*}}}*/

void main(void) {/*{{{*/
//  prev_ratio = 0;
    incoming = 0;
//  outgoing = 0;
    failing = 0;
//  total_waiting_time = 0;
//  total_service_time = 0;
    total_in = 0;
    total_out = 0;
//  proxy2_in = 0;
    capacity = 34;
//  initial_alpha = (float) 1 / 15.0; //  Initial factor by which we multiply the number of pending requests to get waiting
//  alpha = (float) 1 / 15.0; //  Factor by which we multiply the number of pending requests to get waiting time time
    current_time = 0;
    int counter;
    for (counter = 0; counter < LIMIT; counter++)
        visitor_count[counter] = 0;  // Initialize visitor_count


    parse_config_file();


    pthread_t timer_log;
    pthread_create( &timer_log, NULL, start_logging, (void*) NULL);
    pthread_t make_connection;
    pthread_create(&make_connection, NULL, create_server_socket, (void*) NULL);
    pthread_t send_queue;
    pthread_create( &send_queue , NULL , queue_sender, (void *) NULL );

    while (FCGI_Accept() >= 0) {
        change_values(&incoming, 1);
        total_in++;

        // Updated on 17.04
        time_t currtime = time(NULL); 

        //int curr_backlog = get_values(&incoming) - get_values(&outgoing);
        //curr_backlog = curr_backlog > 0 ? curr_backlog : 0;
        //float float_wait_time =  ((float) (curr_backlog + get_pending())/capacity);

        // Calculate waiting time and increment corresponding count in queue (Make this more efficient if needed)
        int iter = 0;
        for (iter = 0; iter < LIMIT; iter++) {
            if (get_array(&visitor_count[(current_time + iter) % LIMIT])
                    < capacity) {
                //visitor_count[(current_time+iter)%LIMIT]++;
                update_array(&visitor_count[(current_time + iter) % LIMIT], 1); // increment by 1
                break;
            }
        }
        if (iter == LIMIT) {
            printf("Content-Type:text/html\n\n");

            printf(
                    "<title>Frontend</title> <body>Server Totally Busy</body></html>");
        } else {
            float float_wait_time = iter;

            int time_to_wait = float_wait_time;

            //changes made to enable logging of avg wait time (following line is uncommented)
            change_float_values(&total_waiting_time, float_wait_time, 0);
            char* gt = get_token(time_to_wait);
            char* env_var = getenv("QUERY_STRING");
            char* request_limit = strchr(env_var, '=') + 1;
            char url_to_visit[100];
            strcpy(url_to_visit, "http://10.129.41.67:8000/test.php?limit=");
            strcat(url_to_visit, (const char*) request_limit);

            printf("Refresh: %d; url=%s&hash=%s&token=%s\n", time_to_wait,
                    url_to_visit,/*"aaa"*/getHash((unsigned char*) gt),
                    encrypt(gt));
            printf("JMeter: %0.3f; url=%s&hash=%s&token=%s\n", float_wait_time,
                    url_to_visit,/*"aaa"*/getHash((unsigned char*) gt),
                    encrypt(gt));
            printf("Content-Type:text/html\n\n");

            printf(
                    "<title>Frontend</title>"
                    "Total incoming this second: <b>%d</b> \n<br />"
                    "Total requests served this second: <b>%d</b> \n<br /> "
                    "Total incoming requests till now: <b>%d</b> \n<br /> "
                    "Total requests served till now: <b>%d</b> \n<br /> "
                    "currtime: <b>%ld</b> \n<br /> "
                    "You will be redirected in %d seconds<br/>"
                    "query limit : %s"
                    "</div></body>",
                    incoming,
                    outgoing,
                    total_in,
                    total_out,
                    currtime,
                    time_to_wait,
                    request_limit);
            printf(
                    "<br>"
                    "<a href='%s&hash=%s&token=%s\n' style='text-decoration:none;'>"
                    "<button style='cursor:pointer'>Click here to go without waiting</button>"
                    "</a></center></body></html>",
                    url_to_visit,
                    /*"aaa"*/getHash((unsigned char*) gt),
                    encrypt(gt));

            free(gt);
        }
    }
}/*}}}*/
