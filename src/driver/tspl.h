#ifndef TSPL_H
#define TSPL_H

#include<stdio.h>
#include"../comm/HCPrase.h"
#include"../comm/comm.h"
#include"../comm/barcode.h"
#include"../comm/bitmap.h"

void printTSPL(char *p);
pageConfig *getPageConfigTSPL(char *p);
void HCreateDirectionTSPL(Queue*TSPLqueue,pageConfig *page,u_int8_t n);
void HCreatePrintTSPL(Queue*TSPLqueue,pageConfig *page,u_int8_t m);
void HCreateClearBufferTSPL(Queue*TSPLqueue,pageConfig *page);
void HCreateSizeTSPL(Queue*TSPLqueue,pageConfig *page,u_int8_t type);
void HCreateGapTSPL(Queue*TSPLqueue,pageConfig *page,u_int8_t type);
void HCreateObjectTSPL(Queue*TSPLqueue,pageConfig *page);
void HCreateBarcodeTSPL(Queue*TSPLqueue,pageConfig *page,BarcodeLayer *barcodeLayer);
void HCreateTextTSPL(Queue*TSPLqueue,pageConfig *page,TextLayer *textLayer);
void HCreateLineTSPL(Queue*TSPLqueue,pageConfig *page,LineLayer *lineLayer);
void HCreateRectangleTSPL(Queue*TSPLqueue,pageConfig *page,RectangleLayer *rectLayer);
void HCreateEllipseTSPL(Queue*TSPLqueue,pageConfig *page,EllipseLayer *ellipseLayer);
void HCreateImageTSPL(Queue*TSPLqueue,pageConfig *page,ImageLayer *imageLayer);
void HCreateRfidTSPL(Queue*TSPLqueue,pageConfig *page,RfidLayer *rfidLayer);

#endif