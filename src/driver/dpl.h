#ifndef DPL_H
#define DPL_H

#include<stdio.h>
#include"../comm/HCPrase.h"
#include"../comm/comm.h"
#include"../comm/barcode.h"
#include"../comm/bitmap.h"
void printDPL(char *p);
pageConfig *getPageConfigDPL(char *p);
void HCreateStartDPL(Queue*DPLqueue,pageConfig *page);
void HCreateUintsDPL(Queue*DPLqueue,pageConfig *page,u_int8_t units);
void HCreateHomeDPL(Queue*DPLqueue,pageConfig *page);
void HCreateSetPageSizeDPL(Queue*DPLqueue,pageConfig *page);
void HCreateObjectDPL(Queue*DPLqueue,pageConfig *page);
void HCreateBarcodeDPL(Queue*DPLqueue,pageConfig *page,BarcodeLayer *barcodeLayer);
void HCreateTextDPL(Queue*DPLqueue,pageConfig *page,TextLayer *textLayer);
void HCreateLineDPL(Queue*DPLqueue,pageConfig *page,LineLayer *lineLayer);
void HCreateRectangleDPL(Queue*DPLqueue,pageConfig *page,RectangleLayer *rectLayer);
void HCreateEllipseDPL(Queue*DPLqueue,pageConfig *page,EllipseLayer *ellipseLayer);
void HCreateImageDPL(Queue*DPLqueue,pageConfig *page,ImageLayer *imageLayer);
void HCreateRfidDPL(Queue*DPLqueue,pageConfig *page,RfidLayer *rfidLayer);
void HCreateEndDPL(Queue*DPLqueue,pageConfig *page);
#endif