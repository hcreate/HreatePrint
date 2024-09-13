#include"esc.h"
void printESC(char *p)
{
    printf("%s",p);
}
void HCreateObjectESC(Queue*ESCqueue,pageConfig *page){}
void HCreateBarcodeESC(Queue*ESCqueue,pageConfig *page,BarcodeLayer *barcodeLayer){}
void HCreateTextESC(Queue*ESCqueue,pageConfig *page,TextLayer *textLayer){}
void HCreateLineESC(Queue*ESCqueue,pageConfig *page,LineLayer *lineLayer){}
void HCreateRectangleESC(Queue*ESCqueue,pageConfig *page,RectangleLayer *rectLayer){}
void HCreateEllipseESC(Queue*ESCqueue,pageConfig *page,EllipseLayer *ellipseLayer){}
void HCreateImageESC(Queue*ESCqueue,pageConfig *page,ImageLayer *imageLayer){}
void HCreateRfidESC(Queue*ESCqueue,pageConfig *page,RfidLayer *rfidLayer){}