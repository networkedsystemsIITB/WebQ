extern int portno;
extern char * ip;
void parse_config_file(){
    int size = 1024, pos;
    int nl =0;
    int c;
    char *buffer = (char *)malloc(size);

    FILE *f = fopen("/home/murali/webq/TokenGenNew/proxy.conf", "r");
    if(f) {
        do { // read all lines in file
            pos = 0;
            do{ // read till the first space in each line
                c = fgetc(f);
                if(c != EOF) buffer[pos++] = (char)c;
                if(pos >= size - 1) {
                    // increase buffer length - leave room for 0
                    size *=2;
                    buffer = (char*)realloc(buffer, size);
                }
            }while(c != EOF && c != ' ');
            // proceed till the end of the line is reached
            while( c!= EOF && c != '\n') c=fgetc(f);
            buffer[pos] = 0;
            nl ++;
            ip = (char*) malloc( 20 * sizeof(char) );
            strcpy( ip ,  "10.129.41.17" );
            // line is now in buffer
            switch( nl ){
                case 1:
                    portno = atoi( buffer );
                    break;
                default:
                    break;
            }
            /* handle_line(buffer); */
        } while(c != EOF);
        fclose(f);
    }
    free(buffer);
}
