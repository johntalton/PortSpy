#include <stdlib.h>
#include <socket.h>
#include <netdb.h>
#include <stdio.h>


#define PING_MINSIZE 32
#define PING_MAXSIZE (65536 - 100)

typedef struct echo_packet {
	unsigned seq;
	unsigned now[2];	/* unaligned long long */
	char data[PING_MAXSIZE - (3 * sizeof(int))]; /* actual variable-length */
} echo_packet_t;


bool PingHost(const char *hostname){
   echo_packet_t *data = NULL;
   int size = PING_MINSIZE;
   data = (echo_packet_t*)malloc(size);
   
   struct hostent *h = NULL;
   h = gethostbyname(hostname);
   if(h == NULL){
      // Not a real host! DNS catches this
      // in our scanner but we do it here 
      // for safty
      return false;
   }
   // Make a destanation address out of our
   // newly created host entry
   struct sockaddr_in dst;
   memcpy(&dst.sin_addr, h->h_addr, h->h_length);
   
   // Now actually create a socket!!!
   int s;
   s = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
   if(s < 0){
      // Cound not create a socket ... thats odd
      return false;
   }
   
   // Crate a time packet
   union{
      bigtime_t now;
      unsigned nowi[2];
   }u;
   u.now = system_time();
   // Fill in the time for our packet
   data->now[0] = u.nowi[0];
   data->now[1] = u.nowi[1];
   
   // Set the sequesce number for this packet. 
   // We are only doing ONE ping. So we can just
   // make this the 1st packet we send, thus its
   // hardcoded to 1.
   data->seq = 1;
   
   // !! Actaully send the packet on its way !!
   // (Socket, packet, packet size, ?, where to, size of des addrest)
   if(sendto(s,(char *)data, size, 0, (struct sockaddr *)&dst, sizeof(dst)) < 0){
      // Faild to send the packet .. this is BAD
      // close the socket we have open and run
      closesocket(s);
      return false;
   }
   // Now we must wait for our reply
   // it should be comeing along any time now
   
   // Create a time out struct
   // and set it to about 2 seconds that should be
   // enough for one ping from a host
   struct timeval tv;
   tv.tv_sec = 5;
   tv.tv_usec = 0;
   
   // This is a socket mask.
   // It helps select figure out which socket we want
   struct fd_set fds;
   FD_ZERO(&fds);  //FD_ZERO(set) clears the mask given by set
   FD_SET(s, &fds);  // FD_SET(socket, set) adds a socket to the mask
   
   
   // This is where we actually read the socket and see
   // if the ping has come back to us yet
   // -1 is a fail, 0 is a time out and 1 is a good to go.
   switch(select(s + 1, &fds, NULL, NULL, &tv)){
   //switch(select(32, &fds, NULL, NULL, &tv)){
   case -1: // Fail
      printf("Failed %s\n",hostname);
      closesocket(s);
      return false;
      break;
   case 0:  // TimeOut
      printf("Time out %s\n",hostname);
      closesocket(s);
      return false;
      break;
   }   
   
   printf("After select\n");
   
   // we got a 1 from select .. thus we have data ready
   // on the socket... this is good
   struct sockaddr_in from;
   int fromlen = sizeof(from);
   int datalen;
   datalen = recvfrom(s, (char *)data, size,0, (struct sockaddr *)&from, &fromlen);
   if(datalen < 0){
      //what does this mean?
      // we have not data from the host
      // that sent us a packet back?
      return false;
   }
   
   closesocket(s);
   
   return true;
}


















