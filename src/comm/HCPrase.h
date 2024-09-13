#ifndef HCPRASE_H

#include<stdio.h>
#include<stdlib.h>
//#include"xml.h"
#include"algorithm.h"
#include "string.h"
#include"comm.h"
void printHC(char *p);

FILE* open(char *filename);
void read(FILE* file);
Queue* readToQueue(FILE *file);
pageConfig * queueCoventToPage(Queue* queue);
void travel(layerTreeNode *root,Queue* queue);
pageConfig * prase(Queue* queue);
void addTextLayer(pageConfig *page);

#endif