#ifndef EPL_H
#define EPL_H

#include<stdio.h>
#include"../comm/HCPrase.h"
#include"../comm/comm.h"
#include"../comm/barcode.h"
#include"../comm/bitmap.h"
void printEPL(char *p);
pageConfig *getPageConfigEPL(char *p);
void HCreateClearBufferEPL(Queue*EPLqueue,pageConfig *page);
void HCreateSetPageWidthEPL(Queue*EPLqueue,pageConfig *page);
void HCreateSetPageHeightAndGapAndOffsetEPL(Queue*EPLqueue,pageConfig *page);
void HCreateObjectEPL(Queue*EPLqueue,pageConfig *page);
void HCreateBarcodeEPL(Queue*EPLqueue,pageConfig *page,BarcodeLayer *barcodeLayer);
void HCreateTextEPL(Queue*EPLqueue,pageConfig *page,TextLayer *textLayer);
void HCreateLineEPL(Queue*EPLqueue,pageConfig *page,LineLayer *lineLayer);
void HCreateRectangleEPL(Queue*EPLqueue,pageConfig *page,RectangleLayer *rectLayer);
void HCreateEllipseEPL(Queue*EPLqueue,pageConfig *page,EllipseLayer *ellipseLayer);
void HCreateImageEPL(Queue*EPLqueue,pageConfig *page,ImageLayer *imageLayer);
void HCreateRfidEPL(Queue*EPLqueue,pageConfig *page,RfidLayer *rfidLayer);
void HCreatePrintEPL(Queue*EPLqueue,pageConfig *page,u_int32_t n);
#endif