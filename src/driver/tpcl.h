#ifndef TPCL_H
#define TPCL_H

#include<stdio.h>
#include"../comm/HCPrase.h"
#include"../comm/comm.h"
#include"../comm/barcode.h"
#include"../comm/bitmap.h"
void printTPCL(char *p);
pageConfig *getPageConfigTPCL(char *p);
u_int32_t  icnt=0;
void HCreateSetPageSizeTPCL(Queue*TPCLqueue,pageConfig *page);
void HCreatePrintTPCL(Queue*TPCLqueue,pageConfig *page);
void HCreateClearBufferTPCL(Queue*TPCLqueue,pageConfig *page);
void HCreateObjectTPCL(Queue*TPCLqueue,pageConfig *page);
void HCreateBarcodeTPCL(Queue*TPCLqueue,pageConfig *page,BarcodeLayer *barcodeLayer);
void HCreateTextTPCL(Queue*TPCLqueue,pageConfig *page,TextLayer *textLayer);
void HCreateLineTPCL(Queue*TPCLqueue,pageConfig *page,LineLayer *lineLayer);
void HCreateRectangleTPCL(Queue*TPCLqueue,pageConfig *page,RectangleLayer *rectLayer);
void HCreateEllipseTPCL(Queue*TPCLqueue,pageConfig *page,EllipseLayer *ellipseLayer);
void HCreateImageTPCL(Queue*TPCLqueue,pageConfig *page,ImageLayer *imageLayer);
void HCreateRfidTPCL(Queue*TPCLqueue,pageConfig *page,RfidLayer *rfidLayer);
#endif