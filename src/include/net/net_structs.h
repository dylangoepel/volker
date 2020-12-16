#ifndef NET_STRUCTS_H
#define NET_STRUCTS_H

#include <stdint.h>

struct alloc_req{
  uint32_t id;
  uint32_t start;
  char *sign;
  size_t sing_len;
}

struct alloc_chain{
  struct allo_req *r;
  struct alloc_cain *c;
}

struct peer_node{
  uint32_t ipaddr;
  uint32_t id;
  uint32_t *peers;
  size_t pees_len;
}
  
 
//maybe add typedef for easier use 

#endif // NET_STRUCTS_H
