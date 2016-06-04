#ifndef TIMER_H
#define TIMER_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // to stop compiler cribing on read()
#include <time.h>
#include <event.h>
#include <math.h>
#include "queue.h"
/************* GLOBAL VARIABLES *********/
#define LIMIT 1000 // Keep a track of 1000 seconds only
#define PEERS 5 // max no of peers
#define SIZE 7 // interval size
#define IGNORE 1
//#define FACTOR 1.3//Our ratio (alpha)
#define GRACE 30//Number of seconds provided for recovery from overload
int served;
//extern float prev_ratio = 0;
//extern float prev_ratio2 = 0;
//extern float current_ratio = 0;
//extern float current_response_time = 0.0;
//extern float prev_response_time = 0.0;
//extern float prev_response_time2 = 0.0;
extern int incoming = 0;
extern int outgoing = 0;
extern int failing = 0;

//changes made to enable logging of avg wait time (following two lines are uncommented)
extern int share = 0;
extern float total_waiting_time = 0;
extern float hostIncomingRate = 0;
extern float peer_incomingRate[];
extern float sum_peer_incoming_rate;
//extern float old_service_time = 0;
//extern unsigned long total_service_time = 0;
//extern int proxy2_in = 0;
//extern float initial_alpha = (float) 1 / 15.0; //  Initial factor by which we multiply the number of pending requests to get waiting
//extern float alpha = (float) 1 / 15.0; //  Factor by which we multiply the number of pending requests to get waiting time time
extern int total_in = 0;
extern int total_out = 0;
extern int current_time = 0;
int visitor_count[LIMIT];
int peer_v_count[PEERS][LIMIT];         //peer visitor count : list of arrays storing wait time arrays of peers
int incoming_peers[PEERS];
extern char log_format_string[256];
FILE* log_ptr;
struct queue q = { NULL, NULL, 0, 100, 0, 0, 0 };
struct queue pr = { NULL, NULL, 0, 20, 0, 0, 0 };
struct queue service_times = { NULL, NULL, 0, 20, 0, 0, 0 };
extern int capacity = 10;
//added hardness parameter
extern double hardness[2];
/*****************************************/

/*****************************************
 Call init_logger first
 Then start_timer
 Finally call free_logger in the end
 Don't forget to compile with -levent
 *****************************************/

void start_timer();

void init_logger() {
    if (log_ptr != NULL)
        return;
    char log_file[] = "proxy1.log";     // at DocumentRoot in config file ( /usr/lib/cgi-bin/ )
    log_ptr = fopen(log_file, "a");

    if (log_ptr == NULL) {
        fprintf(stderr, "Can't open output file %s!\n", log_file);
    }
}

void debug_printf(const char *fmt, ...) {
    // one debug function to rule them all !!
    if( log_ptr != NULL ) {
        va_list args;
        va_start(args, fmt);
        vfprintf(log_ptr, fmt, args);
        va_end(args);
    }
}

void free_logger() {
    fclose(log_ptr);
}

//int ctr = 0;
//int flag = 1; // Capacity changes take place only when flag = 1
//int wait_for = 0;
//long long int total_in_interval = 0;
//long long int total_time_interval = 0;
//long long int total_load_interval = 0;
//int capacity2 = -1;
//int interval_flag = 0;
void log_data() {
    int no_log = 0;
    if (log_ptr == NULL) {
        fprintf(stderr, "Error: Can't open log file. Trying again\n");
        init_logger();
        if (log_ptr == NULL) {
            fprintf(stderr, "  Attempt to open file again failed\n");
//          return;
            no_log = 1;
        }
    }

    push_back(&q, incoming, outgoing, failing);
//  unsigned long avg_service_time = 0;


//  if (outgoing > 0)
//      avg_service_time = (unsigned long) total_service_time / outgoing;
//  else
//      avg_service_time = old_service_time;

//  if (outgoing != 0 && avg_service_time != 0) {
        //current_ratio=current_ratio+((float)(outgoing * 1000000.0)/avg_service_time);
//      total_in_interval += outgoing;
//      total_time_interval += ((float) avg_service_time * outgoing / 100000.0);
//      total_load_interval += incoming;
//      ctr = ctr + 1;

//      if (ctr % SIZE == 0) {
//          //current_ratio=total_in_interval/(total_time_interval/total_in_interval);  //  Goodput/Avg_Service_Time
//          current_ratio =
//                  (float) (total_in_interval * total_in_interval * 100)
//                          / total_time_interval;  //  Goodput/Avg_Service_Time
//          current_response_time = (total_time_interval / total_in_interval);
//      }
//  }
//  /*if(avg_service_time>0){
//   current_ratio=(float)(outgoing * 1000000.0)/avg_service_time;
//   //if(prev_ratio == 0) prev_ratio = current_ratio;
//   current_ratio = (1 * current_ratio) + (0 * prev_ratio);
//   }
//   else
//   current_ratio=prev_ratio;*/
//
//  /*fprintf(log_ptr, "%lld %d %d %d %0.5f %ld %d %d %0.2f %d %d %d \n", (long long int) time(NULL),
//   incoming, outgoing, failing, hostIncomingRate, avg_service_time, proxy2_in, capacity, current_ratio, ctr, flag);*/
    time_t rawtime;
    char time_buf[256];
    time(&rawtime);
    strcpy(time_buf,ctime(&rawtime));
    strftime( time_buf, 80, "%H:%M:%S" , localtime(&rawtime) );


    if(!no_log) {
        fprintf(log_ptr,
                "%s "
                "inc %d "
                "hostWt %0.2f "    //logging the hostIncomingRate parameter
                "peerWt %0.2f "    //logging the hostIncomingRate parameter
                "Cap %d "
                "expVisitors %d "
                "share %d "
                "%d "
                "\n",
                time_buf,
                incoming,
                hostIncomingRate,  //logging the hostIncomingRate parameter
                sum_peer_incoming_rate,  //logging the hostIncomingRate parameter
                capacity,
                get_array(&visitor_count[current_time]),
                share,
                current_time//,
                );
        fflush(log_ptr);
    }

//  /*if(avg_service_time>0){
//   //    Impose penalty
//   if(prev_ratio<current_ratio)
//   capacity=capacity+1;
//   else if(prev_ratio>current_ratio)
//   capacity=(capacity<=1)?1:(capacity-1);
//
//   }*/
//  if (ctr % SIZE == 0 && interval_flag == IGNORE) {
//      interval_flag = 0;
//      current_ratio = 0;  // for decreasing case (verify)
//  } else if (flag == 1 && (ctr % SIZE) == 0 && outgoing != 0
//          && wait_for == 0) {
//      if (prev_ratio == 0)
//          prev_ratio = current_ratio;
//      if (prev_ratio > (FACTOR * current_ratio)) {
//          flag = 0; // Capacity reached
//          fprintf(log_ptr, "%lld %0.2f %0.2f %d Capacity Set\n",
//                  (long long int) time(NULL), prev_ratio, current_ratio,
//                  capacity);
//          int iter = 0;
//          for (iter = 0; iter < LIMIT; iter++) {
//              //if(visitor_count[(current_time+iter)%LIMIT]<=(capacity-1)) {
//              if (get_array(&visitor_count[(current_time + iter) % LIMIT])
//                      <= (capacity / 2)) {
//                  //visitor_count[(current_time+iter)%LIMIT]++;
//                  break;
//              }
//          }
//          if (iter == LIMIT) {
//              fprintf(log_ptr, "Array full\n");
//          }
//          wait_for = iter + GRACE; //add grace time
//          capacity -= 2; //Operating 1 unit below actual capacity          FOR_CONSTANT_CAPACITY
//          capacity2 = capacity;
//          capacity /= 2;          //FOR_CONSTANT_CAPACITY
//          prev_ratio2 = prev_ratio;
//          prev_response_time2 = prev_response_time;
//      }
//      /*else if(prev_ratio>=(1.5*current_ratio)){
//       flag=0; // Capacity reached
//       fprintf(log_ptr, "%lld %0.2f %0.2f %d Capacity Set\n", (long long int) time(NULL),
//       prev_ratio, current_ratio, capacity);
//       int iter=0;
//       for(iter=0;iter<LIMIT;iter++) {
//       if(visitor_count[(current_time+iter)%LIMIT]<=(capacity-1)) {
//       //visitor_count[(current_time+iter)%LIMIT]++;
//       break;
//       }
//       }
//       wait_for=2*iter;
//       capacity--;
//       prev_ratio2=prev_ratio;
//       }*/
//      //else capacity=capacity+1;
//      else if (capacity <= (total_load_interval / SIZE)) {
//          capacity++; // Increase capacity only if capacity < offered load       FOR_CONSTANT_CAPACITY
//      }
//      //prev_ratio = current_ratio;
//  } else if ((ctr % SIZE) == 0 && outgoing != 0 && wait_for == 0) { // Don't update capacity
//      if (prev_ratio == 0)
//          prev_ratio = current_ratio;
//      if ((current_ratio * FACTOR) <= prev_ratio2
//              && current_response_time >= (prev_response_time2 * FACTOR)) { // Capacity has decreased
//              // Calculate waiting time and increment corresponding count in visitor_count (Make this more efficient if needed)
//          int iter = 0;
//          for (iter = 0; iter < LIMIT; iter++) {
//              //if(visitor_count[(current_time+iter)%LIMIT]<=(capacity/2)) {
//              if (get_array(&visitor_count[(current_time + iter) % LIMIT])
//                      <= (capacity / 4)) {
//                  //visitor_count[(current_time+iter)%LIMIT]++;
//                  break;
//              }
//          }
//          if (iter == LIMIT) {
//              fprintf(log_ptr, "Array full2\n");
//          }
//          wait_for = iter + GRACE;      //add grace time
//          capacity /= 4;          //FOR_CONSTANT_CAPACITY
//          flag = 1;
//          fprintf(log_ptr,
//                  "%lld %0.2f %0.2f %d %d Capacity Unset(decrease)\n",
//                  (long long int) time(NULL), prev_ratio, current_ratio,
//                  capacity, wait_for);
//      } else if (current_ratio >= (FACTOR * prev_ratio2)) { // Capacity has increased
//          // Start incrementing capacity values
//          flag = 1;
//          fprintf(log_ptr, "%lld %0.2f %0.2f %d Capacity Unset(increase)\n",
//                  (long long int) time(NULL), prev_ratio, current_ratio,
//                  capacity);
//      }
//  } else if (wait_for > 0) {
//      wait_for--;
//  }

//  if (ctr % SIZE == 0) {
//      prev_ratio = current_ratio;
//      prev_response_time = current_response_time;
        //current_ratio=0;
//      ctr = 0;
//      total_in_interval = 0;
//      total_time_interval = 0;
//      total_load_interval = 0;
//  }

//  if (wait_for == 0 && capacity2 != -1) {
//      //ctr=0;
//      capacity = capacity2;         //FOR_CONSTANT_CAPACITY
//      capacity2 = -1;
//      interval_flag = IGNORE;
//      //wait_for+=1;
//  } else if (wait_for == 1 && flag == 1) {
//      interval_flag = IGNORE;
//      prev_ratio = 0;
//  }

    //visitor_count[(current_time]=0;
    update_array(&visitor_count[current_time], 0);
    current_time = (current_time + 1) % LIMIT;

    //if (get_total_failing(&q) == 0) change_float_values(&alpha, -1*penalty, initial_alpha);
    hostIncomingRate = incoming;
    change_values(&incoming, 0);
    change_values(&outgoing, 0);

    //changes made to enable logging of avg wait time (following line is uncommented)
//  old_service_time = avg_service_time;

    //changes made to enable logging of avg wait time (following line is uncommented)
    change_float_values(&total_waiting_time, 0.0, 0);
//  change_long_values(&total_service_time, 0);
//  change_values(&failing, 0);
//  change_values(&proxy2_in, 0);
}
void timed_function(int fd, short event, void *arg) { // Called every second
    struct timespec tim, rem;
    tim.tv_sec = 1;
    tim.tv_nsec = 000000000;
    while( 1 ){
    log_data();
    nanosleep( &tim, &rem );
    }
    start_timer();
}
void start_timer() {
    struct event ev;
    struct timeval tv;

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    event_init();
    evtimer_set(&ev, timed_function, NULL);
    evtimer_add(&ev, &tv);
    event_dispatch();
}

long long int get_pending() {
    return pending_requests(&q);
}

#endif
