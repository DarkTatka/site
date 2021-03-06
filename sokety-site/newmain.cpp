#include <stdio.h>
#include <string.h>
#include <signal.h>
 
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <netinet/ether.h>
#include <iostream>
 
using namespace std;
/*
 * Napište program pro příjem všech linkových rámců vypisující následující informace: 
 * délka rámce
 * linkový protokol
 * adresy odesílatele
 * adresy příjemce (v dvojtečkovém tvaru)
 * protokol síťové vrstvy 
 */
 
#define BUFFLEN 1500
 
int finish = 0;
 
void sig_handler (int sig)
{
  if (sig == SIGINT)
    finish = 1;
}
 
int main (int argc, char *argv[])
{
  struct sigaction act;
  
  int sock, i;
  ssize_t length;
  char buffer[BUFFLEN];
  struct sockaddr_ll address;
  socklen_t address_size;
 
  memset (&act, 0, sizeof(act));
  act.sa_handler = sig_handler;
  sigaction (SIGINT, &act, NULL);
  
  /* vytvorime socket */
  sock = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL));
 
  if (sock == -1) {
    perror ("socket error");
    return -1;
  }
 
  address_size = sizeof(address);
  
  while (!finish) {
    /* prijmeme data */
    length = recvfrom (sock, buffer, BUFFLEN, 0, (struct sockaddr *)&address,
           &address_size);
    
    if (length == -1) {
      if (errno != EINTR) {
  perror ("recvfrom error");
  close (sock);
  return -1;
      }
      else
  continue;
    }
    
    /* vypiseme data */
//    for (i = 0; i < length; i++)
//      printf ("%02hhx ", buffer[i]);
//    printf ("(%i bytes)\n", length);
//  
 /*
 * Napište program pro příjem všech linkových rámců vypisující následující informace: 
 * délka rámce
 * linkový protokol
 * adresy odesílatele
 * adresy příjemce (v dvojtečkovém tvaru)
 * protokol síťové vrstvy 
 */
    
    struct ether_header* header;
    header = (struct ether_header*) buffer; 
    /* 
    * kdo by to byl cekal, pretypovavame buffer abychom dostali ether_header)
    */
   
    struct iphdr* data;
    data = (struct iphdr*) (buffer + sizeof(struct ether_header));
    /*
    * a tady zase pretypovavame buffer, tentokrat na abychom dostali sitovy paket.
    * Pricitame velikost hlavicky, sitovy paket je v ethernetovem ramci zabaleny.
    */
    
    cout << "delka ramce : " << length << endl;
    cout << "linkovy protokol : " << ntohs(address.sll_protocol) << endl;
    cout << "adresa odesilatele : " <<  ether_ntoa((ether_addr*)header->ether_shost) << endl;
    cout << "adresa prijemce : " <<  ether_ntoa((ether_addr*)header->ether_dhost) << endl;
 

    switch (data->protocol){
        case IPPROTO_TCP:
            cout << "protokol sitove vrstvy : TCP" << endl << endl;
            break;
        case IPPROTO_UDP:
            cout << "protokol sitove vrstvy : UDP" << endl << endl;
            break;
        default:
             cout << "protokol sitove vrstvy : " << ntohs(data->protocol) << endl << endl;
            break;
    }

/*
    if(data->protocol == IPPROTO_TCP) { 
        cout << "protokol sitove vrstvy : TCP" << endl;
    } else {
        if(data->protocol == IPPROTO_UDP) {
            cout << "protokol sitove vrstvy : UDP" << endl;
        } else {
            cout << "protokol sitove vrstvy : " << ntohs(data->protocol) << endl << endl;
        }
    }
    */
  }
    
  /* zrusime socket */
  close (sock);
  
  return 0;
}
