

void inform_proxy1_100(server* srv){
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *servr;
    char buffr[256];
    portno = 5001;
    char* hostname = "localhost";
    
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        log_error_write(srv, __FILE__, __LINE__,  "s", "chandancode: ERROR opening socket");
        return;
        //exit(1);
    }
    servr = gethostbyname(hostname);
    if (servr == NULL) {
        log_error_write(srv, __FILE__, __LINE__,  "s", "chandancode: ERROR, no such host1");
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
         log_error_write(srv, __FILE__, __LINE__,  "s", "chandancode: ERROR connecting1");
         return;
         //exit(1);
    }	
    strcpy(buffr, "100"); buffr[3]='\0';
    /* Send message to the server */
    write(sockfd,buffr,strlen(buffr));
    close(sockfd);
}

void inform_proxy1_200(server* srv){
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *servr;
    char buffr[256];
    portno = 5001;
    char* hostname = "localhost";
    
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        log_error_write(srv, __FILE__, __LINE__,  "s", "chandancode: ERROR opening socket");
        return;
        //exit(1);
    }
    servr = gethostbyname(hostname);
    if (servr == NULL) {
        log_error_write(srv, __FILE__, __LINE__,  "s", "chandancode: ERROR, no such host1");
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
         log_error_write(srv, __FILE__, __LINE__,  "s", "chandancode: ERROR connecting1");
         return;
         //exit(1);
    }	
    strcpy(buffr, "200"); buffr[3]='\0';
    /* Send message to the server */
    write(sockfd,buffr,strlen(buffr));
    close(sockfd);
}

