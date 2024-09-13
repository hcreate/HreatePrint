#ifndef CPCL_H
#define CPCL_H

#include<stdio.h>
#include"../comm/HCPrase.h"
#include"../comm/comm.h"


void printCPCL(char *p);
pageConfig *getPageConfig(char *p);
void HCreateSetPageSize(Queue*CPCLqueue,pageConfig *page);
void HCreateUnitSet(int unitType,Queue*CPCLqueue,pageConfig *page);
void HCreatePageWidth(Queue*CPCLqueue,pageConfig *page);
void HCreateOpen(Queue*CPCLqueue,pageConfig *page);
void HCreateObject(Queue*CPCLqueue,pageConfig *page);
void HCreateClose(Queue*CPCLqueue,pageConfig *page);
void HCreateFormFeed(Queue*CPCLqueue,pageConfig *page);
void HCreateText(Queue*CPCLqueue,pageConfig *page,TextLayer *textLayer);
void HCreateBarcode(Queue*CPCLqueue,pageConfig *page,BarcodeLayer *barcodeLayer);
void HCreateLine(Queue*CPCLqueue,pageConfig *page,LineLayer *lineLayer);
void HCreateRectangle(Queue*CPCLqueue,pageConfig *page,RectangleLayer *rectLayer);
void HCreateEllipse(Queue*CPCLqueue,pageConfig *page,EllipseLayer *ellipseLayer);
void HCreateImage(Queue*CPCLqueue,pageConfig *page,ImageLayer *imageLayer);
void HCreateRfid(Queue*CPCLqueue,pageConfig *page,RfidLayer *rfidLayer);
#endif