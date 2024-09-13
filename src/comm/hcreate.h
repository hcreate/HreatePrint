#ifndef HCREATE_H
#define HCREATE_H

#include<stdio.h>
#include"barcode.h"
typedef struct Color
{
     /* data */
    u_int8_t r;
    u_int8_t g;
    u_int8_t b;
   
}Color;
typedef struct Font
{
    /* data */
    char *FontName;
    float FontSize;
    char *Style;


}Font;
typedef enum ProgrammingLanguage
    {
        ZPL = 0,
        EPL = 1,
        CPCL = 2,
        TSPL = 3,
        POSTSCRIPT=4,
        ESC=5,
        HCP=6,
        DPL=7,
        IPL=8,
        FPL=9,
        TPCL=10,
        EPS=11
    }ProgrammingLanguage;
typedef enum LayerType
{
   Text=0,
   Barcode=1,
   Line=2,
   Rectangle=3,
   Ellipse=4,
   Rfid=5,
   Image=6,
   Others=7
}LayerType;
typedef struct  BaseImageLayer
{
char *layerName;
int x;
int y;
int Angle;
int width;
int height;


}BaseImageLayer;

typedef struct TextLayer
{
    BaseImageLayer *baseLayer;
    char *content;
    Color color;
    Font  font;
    int8_t isReverse;

    
    /* data */
}TextLayer;


typedef struct BarcodeLayer
{
    /* data */
 BaseImageLayer *baseLayer;
char *content;
char *supplenmentText;
char *ExtAttr;
u_int8_t codeType;
u_int32_t codeSize;
u_int8_t isShowText;
u_int8_t ECIMode;
int version;
char *EncodingMode;
char *DataMode;
Color tm_t;
Color tm_k;
Color tm_zt;
Font zt_font;
TextAlignment ta;
TextPosition tp;

float XDimension;
float bwr;
u_int8_t errorLevel;



}BarcodeLayer;

typedef struct ImageLayer
{
 BaseImageLayer *baseLayer;
 char *img;

}ImageLayer;

typedef struct LineLayer
{
    /* data */
     BaseImageLayer *baseLayer;
    int lineWidth;
    int lineLength;
    int linDirect;
    u_int8_t isDash;
    int dashLength;

}LineLayer;

typedef struct RectangleLayer
{
    /* data */
     BaseImageLayer *baseLayer;
     int lineWidth;
     int RectangWidth;
     int RectangHeight;
     int RectanRadius;
}RectangleLayer;

typedef struct EllipseLayer
{
  BaseImageLayer *baseLayer;
  int EllipseWidth;
  int EllipseHeight;
  int lineWidth;

}EllipseLayer;


typedef struct RfidLayer
{
    /* data */
    BaseImageLayer *baseLayer;
    char *RfidContent;

}RfidLayer;


void printHCREATE(char *p);
#endif