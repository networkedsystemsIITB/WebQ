#include <unordered_map>
#include <string>
#include "functions.h"
#include "timer.h"
#include "parser.h"
#include <netinet/tcp.h>
#include <arpa/inet.h>   //for inet_aton
using namespace std;

int listening_portno;
char ** ip_array ;
char * sending_port;
int no_of_proxy;
char * tokenCheckIp;
float peer_avg_waiting_time[PEERS];
float sum_peer_avg_waiting_time;
double hardness[2];
struct clientDetails{
    int sockfd;
    int ip1;
    int ip2;
    int ip3;
    int ip4;
    int port;
};
unordered_map<clientDetails *,int> ipToid;
int connectedClients; // for asigining array indices to ipToid

void* start_logging( void* ) {
    init_logger();
    start_timer();
}

int readFromClient( struct clientDetails * cd ) {
    // make buffer to store the data from client
    // Two types of clients
    //  1 Capacity Estimator
    //  2 other peer proxy
    int clientSocketFD = cd->sockfd;
    /* debug_printf( "yo yo thread id %d\n", clientSocketFD ); */
    int bytes = LIMIT * sizeof(int);
    int*  buffer = (int *) malloc( bytes );
    int bytesRead;
    int prev_bytesRead=0;
    while( ( bytesRead = read( clientSocketFD, buffer, bytes ) ) > 0)
    {
        /* debug_printf( "bytesRead %d \n", bytesRead ); */
        if( *(char*)buffer == 'c' ){ // if content of buffer == c
            debug_printf( "data from capacity estimator in c %s", ((char*)buffer)+1 );
            capacity = stoi((char*)buffer+1 );
        }
        else if( *(char*)buffer == 'h' ){
            debug_printf( "data from capacity estimator in h %s", ((char*)buffer)+1 );
            // error here due to not handling buffer which is not decimal
            hardness[0] = stod( ((char*)buffer)+1 );
            hardness[1] = stod( strchr( ((char*)buffer)+2 , ' ')  );
            debug_printf( "hardness %f %f n", hardness[0], hardness[1] );
        }
        else if( bytesRead > 20 ) {
            /* debug_printf( "read from %d.%d.%d.%d arrayIdx %d\n", cd->ip1, cd->ip2, cd->ip3, cd->ip4 , ipToid[cd] ); */
            int j;
            memcpy( peer_v_count[ ipToid[cd] ]+(prev_bytesRead/4) , buffer , bytesRead );
            prev_bytesRead += bytesRead;
            prev_bytesRead = (prev_bytesRead >= 4000 ) ? 0:prev_bytesRead;
            if( prev_bytesRead == 0 ) {
                /* debug_printf( "once cycle done \n"); */
                // debug print the array being read
                /* for( j =0 ; j < LIMIT ; j++ ){ */
                /*     if (peer_v_count[ ipToid[cd] ][j] != 0 ||  visitor_count[j]!= 0 ) */
                /*         debug_printf( "(%d)%d,%d\n", j, peer_v_count[ ipToid[cd] ][j], visitor_count[j] ); */
                /* } */
            }
            //calculate peer_avg_waiting_time here with locks
        }
    }
    debug_printf( "gonna shutdown read thread \n");
    // 2- shutdown both send and recieve functions
    return close( clientSocketFD );
}

void * ThreadWorker( void * threadArgs) {
    readFromClient( (struct clientDetails *) threadArgs);
    pthread_exit(NULL);
}

void* create_server_socket(void*) {
    //
    // Creates server socket for communication between Proxy1 and Proxy2
    //
    int sockfd, newsockfd;
    socklen_t clilen;
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
        /* debug_printf("peer connected: %d.%d.%d.%d\n", */
        /*         (int)(cli_addr.sin_addr.s_addr&0xFF), */
        /*         (int)((cli_addr.sin_addr.s_addr&0xFF00)>>8), */
        /*             (int)((cli_addr.sin_addr.s_addr&0xFF0000)>>16), */
        /*             (int)((cli_addr.sin_addr.s_addr&0xFF000000)>>24)); */
        struct clientDetails * cd;
        cd = (clientDetails * ) malloc( sizeof( struct clientDetails ) );
        cd->sockfd = newsockfd;
        cd->ip1 = (int)(cli_addr.sin_addr.s_addr&0xFF);
        cd->ip2 = (int)((cli_addr.sin_addr.s_addr&0xFF00)>>8);
        cd->ip3 = (int)((cli_addr.sin_addr.s_addr&0xFF0000)>>16);
        cd->ip4 = (int)((cli_addr.sin_addr.s_addr&0xFF000000)>>24);
        // add cd to a hash
        ipToid[ cd ] = connectedClients;
        connectedClients++;
        if(pthread_create(&threadId, NULL, ThreadWorker, (void *)cd) < 0)
        {
            debug_printf("Thread creation failed");
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
}

void writeToServer(char *ip_array_n){
    // FUNCTION DESC:
    // write visitor array to peer proxy 
    // ( which is a server as far as 'this' proxy is concerned )
    int sockfd, portnum, n;
    struct sockaddr_in server_addr;
    // sending_port is already filled by the parser
    portnum = atoi(sending_port);
    /* Create client socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        debug_printf( "ERROR opening socket while writing to server\n");
        exit(1);
    }
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(!inet_aton( ip_array_n  , &server_addr.sin_addr))
    {
        debug_printf( "ERROR invalid server IP address\n");
        debug_printf( ip_array_n);
        exit(1);
    }
    server_addr.sin_port = htons(portnum);
    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) >= 0)
    {
        float sec= 0;       // time frequency in which to communicate
        float sec_frac = 0.5;
        debug_printf( "connected %s \n" , ip_array_n);
        struct timespec tim, rem;
        tim.tv_sec = sec;
        tim.tv_nsec = sec_frac*1000000000;
        while( 1)
        {
            /* debug_printf( "writing to %s \n" , ip_array_n); */
            // debug print the visitor_count being written
            /* for(int j =0 ; j < LIMIT ; j++ ){ */
            /*     if ( visitor_count[j]!= 0 ) */
            /*         debug_printf( "(%d) vc %d\n", j, visitor_count[j] ); */
            /* } */
            n = write(sockfd, visitor_count , 1000 * sizeof(int) );
            if (n < 0)
            {
                debug_printf("ERROR writing to peer socket\n");
                /* exit(1); */
                /* break; */
            }
            // connect as a client;
            nanosleep( &tim, &rem );
        }
    }else{
        debug_printf( "ERROR connecting to a peer");
        /* sleep(1); */
        /* exit(1); */
    }
    debug_printf( "disconnected %s \n" , ip_array_n);
    close( sockfd );
}

void* queue_sender( void * args) {/*{{{*/
    sleep(1);
    writeToServer( (char *) args  );
}/*}}}*/

int main(void) {/*{{{*/
//  prev_ratio = 0;
    incoming = 0;
//  outgoing = 0;
    failing = 0;
//  total_waiting_time = 0;
//  total_service_time = 0;
    total_in = 0;
    total_out = 0;
//  proxy2_in = 0;
    capacity = 100000;
    connectedClients = 0;
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
    pthread_t send_queue[PEERS];
    for (counter = 0; counter < no_of_proxy ; counter++)
    {
        pthread_create( &send_queue[ counter ] , NULL , queue_sender, (void *) ip_array[ counter ] );
    }

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
        /* peer_avg_waiting_time = 0; */
        avg_waiting_time = 0;
        int reqInHost =0;
        int reqInPeers[PEERS];
        for( j=0; j<PEERS; j++)
        {
            peer_avg_waiting_time[j] = 0; // TODO use memset
            reqInPeers[j] = 0;
        }
        for (iter = 0; iter < LIMIT; iter++) {
            reqInHost += visitor_count[iter];
            if( iter - current_time > 0 )
            {
                // sum the times .. actual avg found later outside the loop
                avg_waiting_time += ( iter - current_time ) * visitor_count[ iter ];
                for( j=0; j<PEERS; j++)
                {
                    peer_avg_waiting_time[j] += (iter-current_time) * peer_v_count[j][iter];
                    reqInPeers[j] += peer_v_count[j][iter];
                    /* if( visitor_count[iter] != 0 && peer_v_count[j][iter] != 0) */
                    /* { */
                    /*     debug_printf( "%d|%d||%d|%d \n", */
                    /*             (iter -current_time), visitor_count[ iter ], */
                    /*             (iter -current_time), peer_v_count[j][iter] ); */
                    /* } */
                }
            }
        }
        if( reqInHost != 0 )
            avg_waiting_time /= reqInHost;
        sum_peer_avg_waiting_time = 0;
        for( j=0; j<PEERS; j++)
        {
            if( reqInPeers[j] != 0 )
                peer_avg_waiting_time[j] /= reqInPeers[j];
            sum_peer_avg_waiting_time += peer_avg_waiting_time[j];
        }
        /* debug_printf( "%.2f %d, %.2f %d \n" , */
        /*         avg_waiting_time , */
        /*         reqInHost, */
        /*         peer_avg_waiting_time , */
        /*         reqInPeers ); */
        int usedCapacity = 0;
        int peerUsedCapacity = 0;
        // calculate the share :
        // reserve a min value of capacity (0.1) for each servers
        int percent = 10;
        if( avg_waiting_time == 0 && sum_peer_avg_waiting_time == 0 )
        {
            avg_waiting_time = 1;
            sum_peer_avg_waiting_time = 1;
        }
        else if ( avg_waiting_time == 0 ){
            //      awt/( awt + p_awt ) * 100 = percent;
            avg_waiting_time = percent * sum_peer_avg_waiting_time / ( 100 - percent );
        }
        else if ( sum_peer_avg_waiting_time == 0 ){
            sum_peer_avg_waiting_time = percent * avg_waiting_time / ( 100 - percent );
        }
        // we need to multiply by no_of_proxy so as to normalize .
        share = capacity * avg_waiting_time/(avg_waiting_time + sum_peer_avg_waiting_time);
        // share found
        if ( share == 0 ) {
            // share can never be 0
            share = 1;
        }

        int excess_used;
        for (iter = 0; iter < LIMIT; iter++) {
            // find the current used capacity for THIS "iter"
            // for subsequent reqests at same time
            peerUsedCapacity = 0;
            usedCapacity = get_array(&visitor_count[(current_time + iter) % LIMIT]);
            for( j=0; j<PEERS; j++)
            {
                peerUsedCapacity += get_array( &peer_v_count[j][(current_time + iter) % LIMIT] );
            }
            debug_printf( "uc-%d puc-%d share-%d iter-%d \n", usedCapacity, peerUsedCapacity, share , iter);
            int total_usable_capacity = (share  - usedCapacity) ; // use a buffer here to compensate n/w delay!!!
            if( peerUsedCapacity > 0 ){
                excess_used = (capacity - share)-peerUsedCapacity;
                total_usable_capacity += excess_used < 0 ? excess_used : 0;
            }
            char* req_url = getenv("QUERY_STRING");
            int url;
            if(strcmp(req_url,"req1.php")==0)
                url=0;
            else
                url=1;
            if ( total_usable_capacity > 0 )
            {
                //visitor_count[(current_time+iter)%LIMIT]++;
                debug_printf( "time %d %d \n" , current_time , iter) ;
                update_array(&visitor_count[(current_time + iter) % LIMIT], hardness[url]); // increment by hardness
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
            if( strchr(env_var, 'M') > 0 || strchr(env_var, 'S') > 0 ){
                strcpy(url_to_visit, "http://10.129.41.67:9000/");
            }
            else{
                /* fired url http://10.129.28.32:8000/proxy1?limit=moodleS/moodle/ */
                /* http://10.129.49.76/moodleS/moodle/ */
                strcpy(url_to_visit, "http://10.129.41.67:9000/test.php?limit=");
            }
            strcat(url_to_visit, (const char*) request_limit);
            /* debug_printf("%d %d\n", time_to_wait, (currtime+iter)%LIMIT ) ; */
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
