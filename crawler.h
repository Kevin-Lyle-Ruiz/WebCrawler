#ifndef CRAWLER_H
#define CRAWLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ADDR_LENGTH 1000

typedef struct listNode {

  char addr[MAX_ADDR_LENGTH];
  struct listNode *link;
  
} listNode;

int contains(const struct listNode *pNode, const char *addr);

void insertBack(struct listNode *pNode, const char *addr);

void printAddresses(const struct listNode *pNode);

void destroyList(struct listNode *pNode);

int getLink(const char* srcAddr, char* link, const int maxLinkLength);

#endif
