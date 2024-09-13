#ifndef ZPL_H
#define ZPL_H

#include<stdio.h>
#include"../comm/HCPrase.h"
#include"../comm/comm.h"
#include"../comm/barcode.h"
#include"../comm/bitmap.h"
void printZPL(char *p);
pageConfig *getPageConfigZPL(char *p);
void HCreateSetPageSizeZPL(Queue*ZPLqueue,pageConfig *page);
void HCreateObjectZPL(Queue*ZPLqueue,pageConfig *page);
void HCreateBarcodeZPL(Queue*ZPLqueue,pageConfig *page,BarcodeLayer *barcodeLayer);
void HCreateTextZPL(Queue*ZPLqueue,pageConfig *page,TextLayer *textLayer);
void HCreateLineZPL(Queue*ZPLqueue,pageConfig *page,LineLayer *lineLayer);
void HCreateRectangleZPL(Queue*ZPLqueue,pageConfig *page,RectangleLayer *rectLayer);
void HCreateEllipseZPL(Queue*ZPLqueue,pageConfig *page,EllipseLayer *ellipseLayer);
void HCreateImageZPL(Queue*ZPLqueue,pageConfig *page,ImageLayer *imageLayer);
void HCreateRfidZPL(Queue*ZPLqueue,pageConfig *page,RfidLayer *rfidLayer);
#endif