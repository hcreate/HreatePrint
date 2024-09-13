#ifndef COMM_H
#define COMM_H
#define MILLIMETERTOPIXEL 25.4
#define MILLIMETERTOPOUND 72
#define DPI 203
#define FONTNAME  "TTF.F"
#include<stdio.h>
#include<stdlib.h>
#include "string.h"


void printCOMM(char *p);
int isStringEmpty(const char* str); 
char *findData(char *key,char *data);

float pixelToMillimeter(float dpi,int pixel);
float pixelToInch(float dpi,int pixel);
int  millimeterToPixel(float dpi,float length);
int HCreateDPIChange(int oldDPI,int newDPI,int pos);
char * base64_encode( const unsigned char * originChar, char * base64, int originlength );
int base64_decode( const char * base64, unsigned char * originChar );

#endif