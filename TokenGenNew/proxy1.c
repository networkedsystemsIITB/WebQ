#include "functions.h"
#include "timer.h"
#include "parser.h"
#include <netinet/tcp.h>

int listening_portno;
char ** ip_array ;
char * sending_port;

void start_logging() {/*{{{*/
    init_logger();
    start_timer();
}/*}}}*/

struct clientDetails{
    int sockfd;
    int ip1;
    int ip2;
    int ip3;
    int ip4;
    int port;
};

int readFromClient( struct clientDetails * cd ) {/*{{{*/
    // make buffer to store the data from client
    // Two types of clients
    //  1 Capacity Estimator
    //  2 other peer proxy
    int clientSocketFD = cd->sockfd;
    debug_printf( "yo yo thread id %d\n", clientSocketFD );
    int bytes = LIMIT * sizeof(int);
    int*  buffer = malloc( bytes );
    int bytesRead;
    while( ( bytesRead = read( clientSocketFD, buffer, bytes ) ) > 0)
    {
        /* debug_lognum( "bytesRead", bytesRead ); */
        if( bytesRead > 5 ) {
            /* debug_log( "gonna write memory" ); */
            int j;
            memcpy( peer_v_count[cd->ip4%PEERS] , buffer , bytesRead );
            for( j =0 ; j < LIMIT ; j++ ){
                if (peer_v_count[cd->ip4%PEERS][j] != 0 ||  visitor_count[j]!= 0 )
                    debug_printf( "(%d)%d,%d", j, peer_v_count[cd->ip4%PEERS][j], visitor_count[j] );
            }
            //calculate peer_avg_waiting_time here with locks
        }
        else{
            debug_lognum( "data*", *buffer );
            capacity = *buffer;
        }
    }
    debug_log( " gonna shutdown thread " );
    // 2- shutdown both send and recieve functions
    return shutdown(clientSocketFD, 2);
}/*}}}*/

void * ThreadWorker( void * threadArgs) {/*{{{*/
    readFromClient( (struct clientDetails *) threadArgs);
    pthread_exit(NULL);
}/*}}}*/

void* create_server_socket() { /*{{{*/
    //
    // Creates server socket for communication between Proxy1 and Proxy2
    //
    int sockfd, newsockfd,  clilen;
    struct sockaddr_in serv_addr, cli_addr;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    /* listening_portno = 5007; */


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(listening_portno);

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
        // print the ip
        debug_printf("%d.%d.%d.%d\n",
                (int)(cli_addr.sin_addr.s_addr&0xFF),
                (int)((cli_addr.sin_addr.s_addr&0xFF00)>>8),
                    (int)((cli_addr.sin_addr.s_addr&0xFF0000)>>16),
                    (int)((cli_addr.sin_addr.s_addr&0xFF000000)>>24));
        struct clientDetails * cd;
        cd = malloc( sizeof( struct clientDetails ) );
        cd->sockfd = newsockfd;
        cd->ip1 = (int)(cli_addr.sin_addr.s_addr&0xFF);
        cd->ip2 = (int)((cli_addr.sin_addr.s_addr&0xFF00)>>8);
        cd->ip3 = (int)((cli_addr.sin_addr.s_addr&0xFF0000)>>16);
        cd->ip4 = (int)((cli_addr.sin_addr.s_addr&0xFF000000)>>24);
        if(pthread_create(&threadId, NULL, ThreadWorker, (void *)cd) < 0)
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

void writeToServer(){/*{{{*/
    int sockfd, portnum, n;
    struct sockaddr_in server_addr;
    // sending_port is already filled by the parser
    portnum = atoi(sending_port);
    /* Create client socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        debug_log( "ERROR opening socket\n");
        exit(1);
    }
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(!inet_aton( ip_array[0]  , &server_addr.sin_addr))
    {
        debug_log( "ERROR invalid server IP address\n");
        debug_log( ip_array[0]);
        exit(1);
    }
    server_addr.sin_port = htons(portnum);
    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) >= 0)
    {
        /* while( 1) */
        /* { */
            n = write(sockfd, visitor_count , 1000 * sizeof(int) );
            if (n < 0)
            {
                debug_log("ERROR writing to socket\n");
                /* exit(1); */
                /* break; */
            }
            // connect as a client;
            /* sleep(1); */
        /* } */
    }
    else{
        debug_log( "ERROR connecting to a peer");
        /* sleep(1); */
        /* exit(1); */
    }
    debug_printf( "Closing socket\n");
    shutdown(sockfd, 2);

}/*}}}*/

void start_q_timer();

void timed_q_function(int fd, short event, void *arg) { // Called every second/*{{{*/
    writeToServer();
    start_q_timer();
}/*}}}*/

void start_q_timer() {/*{{{*/
    struct event ev;
    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 500000;

    event_init();
    evtimer_set(&ev, timed_q_function, NULL);
    evtimer_add(&ev, &tv);
    event_dispatch();
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
    capacity = 15;
//  initial_alpha = (float) 1 / 15.0; //  Initial factor by which we multiply the number of pending requests to get waiting
//  alpha = (float) 1 / 15.0; //  Factor by which we multiply the number of pending requests to get waiting time time
    current_time = 0;
    int counter;
    for (counter = 0; counter < LIMIT; counter++)
    {
        visitor_count[counter] = 0;  // Initialize visitor_count
        int i ;
        for( i = 0 ; i < PEERS; i++ )
        peer_v_count[i][counter] = 0; // replace with memset
    }


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
        int j;
        peer_avg_waiting_time = 0;
        avg_waiting_time = 0;
        int reqInHost =0;
        int reqInPeers=0;
        for (iter = 0; iter < LIMIT; iter++) {
            reqInHost += visitor_count[iter];
            if( iter - current_time > 0 )
            {
                // sum the times .. actual avg found later outside the loop
                avg_waiting_time += ( iter - current_time ) * visitor_count[ iter ];
                for( j=0; j<PEERS; j++)
                {
                    peer_avg_waiting_time += (iter-current_time) * peer_v_count[j][iter];
                    reqInPeers += peer_v_count[j][iter];
                    if( peer_v_count[j][iter] != 0 )
                    {
                        debug_printf( "%d|%d||%d|%d \n",
                                (iter -current_time), visitor_count[ iter ],
                                (iter -current_time),peer_v_count[j][iter] );
                    }
                }
            }
        }
        if( reqInHost != 0 )
            avg_waiting_time /= reqInHost;
        if( reqInPeers != 0 )
            peer_avg_waiting_time /= reqInPeers;
        /* debug_printf( "%.0f %d, %.0f %d \n" , */
        /*         avg_waiting_time , */
        /*         reqInHost, */
        /*         peer_avg_waiting_time , */
        /*         reqInPeers ); */
        int usedCapacity = 0;
        for (iter = 0; iter < LIMIT; iter++) {
            // find the current used capacity for this "iter"
            usedCapacity = 0;
            usedCapacity += get_array(&visitor_count[(current_time + iter) % LIMIT]);
            for( j=0; j<PEERS; j++)
            {
                usedCapacity += get_array( &peer_v_count[j][(current_time + iter) % LIMIT] );
            }
            // find share of the current proxy
            // share = total_capacity /2 if visitor_waiting_time == peer_wt; else
            // share = total_capacity * visitor_waiting_time/(sum peer_wt and avg) if peer_wt != 0;
            // share = total_capacity  if peer_wt == 0;
            int share;
            int total_usable_capacity = (capacity - usedCapacity) ; // use a buffer here to compensate n/w delay!!!
            if( avg_waiting_time == peer_avg_waiting_time )
            {
                share = total_usable_capacity/2;
            }
            else if( peer_avg_waiting_time != 0 )
            {
                share = total_usable_capacity * avg_waiting_time/(avg_waiting_time + peer_avg_waiting_time);
            }
            else if( peer_avg_waiting_time == 0 )
            {
                share = total_usable_capacity;
            }
            if ( share > 0 )
            {
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
