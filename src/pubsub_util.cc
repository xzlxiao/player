/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2000  
 *     Brian Gerkey, Kasper Stoy, Richard Vaughan, & Andrew Howard
 *                      
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*
 * internal functions and such
 *
 * $Id$
 */


#include <sys/types.h>     /* for socket(2) */
#include <sys/socket.h>     /* for socket(2) */
#include <fcntl.h>     /* for fcntl(2) */
#include <stdio.h>     /* for sprintf(3) */
#include <string.h>     /* for strncpy(3) */
#include <unistd.h>     /* for getpid(2),close(2) */
#include <time.h>     /* for ctime(3) */
#include <netdb.h>     /* for gethostbyname(3) */
#include <netinet/in.h>   /* for sockaddr_in type */

/*
 * this function creates a socket of the indicated type and binds it to 
 * the indicated port.
 *
 * NOTE: we pick the IP address (and thus network interface) for binding 
 *       by calling gethostname() and then stripping it down to the first
 *       component (i.e. no domain name).  if this process won't
 *       result in the IP address that you want, tough luck.
 *
 * ARGS:
 *  serverp: this is a value-result param that will contain the socket's info
 *  blocking: whether or not it should be blocking
 *  portnum: port to bind() to
 *  socktype: should be SOCK_DGRAM (for UDP) or SOCK_STREAM (for TCP)
 *  backlog: number of waiting connections to be allowed (TCP only)
 *
 * RETURN: 
 *  On success, the fd of the new socket is returned.  Otherwise, -1 
 *  is returned and an explanatory note is dumped to stderr.
 */


int
create_and_bind_socket(struct sockaddr_in* serverp, char blocking,
                int portnum, int socktype, int backlog)
{
  int sock;                   /* socket we're creating */
  int flags;                  /* temp for old socket access flags */
  int address_size;           /* size of server address struct */
  int one = 1;
const int on = 1;

  char* first_dot;
  struct hostent* entp;
  char host[256];
  
  address_size = sizeof(*((struct sockaddr_in*)serverp));

  if(gethostname(host,sizeof(host)) == -1)
  {
    fputs("receive(): couldn't get hostname. probably should quit\n", stderr);
    return(-1);
  }

  /* now, strip down to just the hostname */
  if((first_dot = strchr(host,'.')))
  {
    *first_dot = '\0';
  }

  if((entp = gethostbyname(host)) == NULL)
  {
    fprintf(stderr, "receive(): \"%s\" is unknown host; "
	    "probably should quit\n", host);
    return(-1);
  }

  memcpy(&((*serverp).sin_addr), entp->h_addr_list[0], entp->h_length);


  (*serverp).sin_port = htons(portnum);

  /* 
   * Create the INET socket.  
   * 
   */
  if((sock = socket(PF_INET, socktype, 0)) == -1) 
  {
    perror("create_and_bind_socket:socket() failed; socket not created.");
    return(-1);
  }

  /*
   * let's say that our process should get the SIGIO's when it's
   * readable
   */
  if(fcntl(sock, F_SETOWN, getpid()) == -1)
  {
    perror("create_and_bind_socket():fcntl() failed while setting socket "
           "pid ownership; socket not created.");
    close(sock);
    return(-1);
  }

  if(!blocking)
  {
    /*
     * get the current access flags
     */
    if((flags = fcntl(sock, F_GETFL)) == -1)
    { 
      perror("create_and_bind_socket():fcntl() while getting socket "
                      "access flags; socket not created.");
      close(sock);
      return(-1);
    }
    /*
     * OR the current flags with O_ASYNC (so we'll get SIGIO's) and 
     * O_NONBLOCK (so we won't block), and write them back
     */
    //if(fcntl(sock, F_SETFL, flags | O_ASYNC | O_NONBLOCK) == -1)
    if(fcntl(sock, F_SETFL, flags | O_NONBLOCK ) == -1)
    {
      perror("create_and_bind_socket():fcntl() failed while setting socket "
                      "access flags; socket not created.");
      close(sock);
      return(-1);
    }
  }

  if(socktype == SOCK_STREAM)
  {
    /* make sure we can reuse the port soon after */
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)))
    {
      perror("create_and_bind_socket(): setsockopt(2) failed");
      return(-1);
    }
  }


  /* 
   * Bind it to the port indicated
   * INADDR_ANY indicates that any network interface (IP address)
   * for the local host may be used (presumably the OS will choose the 
   * right * one).
   *
   * Specifying sin_port = 0 would allow the system to choose the port.
   */
  (*serverp).sin_family = PF_INET;
  (*serverp).sin_addr.s_addr = INADDR_ANY;

  setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

  if(bind(sock, (struct sockaddr*)serverp, sizeof(*serverp)) == -1) 
  {
    perror ("create_and_bind_socket():bind() failed; socket not created.");
    close(sock);
    return(-1);
  }

  /* if it's TCP, go ahead with listen() */
  if(socktype == SOCK_STREAM)
  {
    if(listen(sock,backlog))
    {
      perror("create_and_bind_socket(): listen(2) failed:");
      close(sock);
      return(-1);
    }
  }

  /*
   * put the port in the address struct
   */
  //if(getsockname(sock, (struct sockaddr*)serverp, &address_size) == -1)                  
  //{
    //perror("create_and_bind_socket():getsockname() failed; socket "
           //"not created");
    //close(sock);
    //return(-1);
  //}

  /*
   * return the fd for the newly bound socket 
   */
  return(sock);
}
