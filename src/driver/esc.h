#ifndef ESC_H
#define ESC_H
#include<stdio.h>
#include"../comm/HCPrase.h"
#include"../comm/comm.h"
#include"../comm/barcode.h"
#include"../comm/bitmap.h"
 void printESC(char *p);
pageConfig *getPageConfigESC(char *p);
void HCreateDirectionESC(Queue*ESCqueue,pageConfig *page,u_int8_t n);
void HCreatePrintESC(Queue*ESCqueue,pageConfig *page,u_int8_t m);
void HCreateClearBufferESC(Queue*ESCqueue,pageConfig *page);
void HCreateSizeESC(Queue*ESCqueue,pageConfig *page,u_int8_t type);
void HCreateGapESC(Queue*ESCqueue,pageConfig *page,u_int8_t type);
void HCreateObjectESC(Queue*ESCqueue,pageConfig *page);
void HCreateBarcodeESC(Queue*ESCqueue,pageConfig *page,BarcodeLayer *barcodeLayer);
void HCreateTextESC(Queue*ESCqueue,pageConfig *page,TextLayer *textLayer);
void HCreateLineESC(Queue*ESCqueue,pageConfig *page,LineLayer *lineLayer);
void HCreateRectangleESC(Queue*ESCqueue,pageConfig *page,RectangleLayer *rectLayer);
void HCreateEllipseESC(Queue*ESCqueue,pageConfig *page,EllipseLayer *ellipseLayer);
void HCreateImageESC(Queue*ESCqueue,pageConfig *page,ImageLayer *imageLayer);
void HCreateRfidESC(Queue*ESCqueue,pageConfig *page,RfidLayer *rfidLayer);
#endif

