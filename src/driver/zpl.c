#include"zpl.h"
void printZPL(char *p)
{
  Queue* ZPLqueue=createQueue();
  pageConfig *page=getPageConfigZPL(p);

  HCreateSetPageSizeZPL(ZPLqueue,page);

  HCreateObjectZPL(ZPLqueue,page);

//输出打印，并释放内存
  while(!isEmpty(ZPLqueue))
   {
     char *data=dequeue(ZPLqueue);
     printf("%s",data);
     free(data);

   }
  free(ZPLqueue);
  freePage(page);
  //printf("%s\n",cmds);
}
pageConfig *getPageConfigZPL(char *p)
{

  FILE *file=open(p);
  Queue* queue=  readToQueue(file);
  pageConfig *page=NULL;
  page= prase(queue);
  return page;
}
void HCreateSetPageSizeZPL(Queue*ZPLqueue,pageConfig *page)
{
  int w=HCreateDPIChange(page->dpiX,DPI,page->pageWidth);
  int h=HCreateDPIChange(page->dpiY,DPI,page->pageHeight);
  
  char *cmds=(char *)malloc(sizeof(char)*100);
  char w_s[20];
  char h_s[20];
  sprintf(w_s, "^LL%d", w);
  sprintf(h_s, "^PW%d", h);
  strcat(cmds,w_s);
  strcat(cmds,h_s);
  strcat(cmds,"\n");
  enqueue(ZPLqueue,cmds);
}
void HCreateObjectZPL(Queue*ZPLqueue,pageConfig *page)
{
  layerNode *current=page->layers;
  while (current!=NULL)
  {
    /* code */

    switch (current->layerType)
    {
    case Text:
      /* code */
      //TextLayer *textObject=(TextLayer *)current->layerData;
      HCreateTextZPL(ZPLqueue,page,current->layerData);

      break;
    case Barcode:
     //BarcodeLayer *barcodeObject=(BarcodeLayer *)current->layerData;
      /* code */
      HCreateBarcodeZPL(ZPLqueue,page,current->layerData);
    break;
    case Rfid:
    // RfidLayer *rfidObject=(RfidLayer *)current->layerData;
      HCreateRfidZPL(ZPLqueue,page,current->layerData);
      /* code */
    break;
    case Image:
     //ImageLayer *imageObject=(ImageLayer *)current->layerData;
      /* code */
      HCreateImageZPL(ZPLqueue,page,current->layerData);
    break;
    case Line:
     //LineLayer *lineObject=(LineLayer *)current->layerData;
      HCreateLineZPL(ZPLqueue,page,current->layerData);
      /* code */
    break;
    case Rectangle:
    // RectangleLayer *rectangleObject=(RectangleLayer *)current->layerData;
      /* code */
      HCreateRectangleZPL(ZPLqueue,page,current->layerData);
    break;
    case Ellipse:
    //EllipseLayer *ellipseObject=(EllipseLayer *)current->layerData;
     HCreateEllipseZPL(ZPLqueue,page,current->layerData);
      /* code */
    break;
    default:
      break;
    }
    page->layers=current->next;
    free(current);
    current=page->layers;


  }
}
void HCreateTextZPL(Queue*ZPLqueue,pageConfig *page,TextLayer *textLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char *fName =(char *)malloc(sizeof(char)*20);
  char xpos[10];
  int x=HCreateDPIChange(page->dpiX,DPI,textLayer->baseLayer->x);
  sprintf(xpos, "%d", x);
  char ypos[10];
  int y=HCreateDPIChange(page->dpiY,DPI,textLayer->baseLayer->y);
  sprintf(ypos, "%d", y);
  sprintf(fName, "%s", FONTNAME);
  char fsize[10];
  sprintf(fsize, "%d", (u_int32_t)textLayer->font.FontSize);
  strcat(cmds,"^FO");
  strcat(cmds,xpos);
  strcat(cmds,",");
  strcat(cmds,ypos);
  strcat(cmds,"^A");
  strcat(cmds,fName);
  switch (textLayer->baseLayer->Angle)
  {
    case 0:
    /* code */
    strcat(cmds,"N");
    break;
    case 1:
    strcat(cmds,"R");
    break;
    case 2:
    strcat(cmds,"I");
    break;
    case 3:
    strcat(cmds,"B");
    break;
    default:
    strcat(cmds,"N");
    break;
  }
  strcat(cmds,",");
  strcat(cmds,fsize);
  strcat(cmds,",");
  strcat(cmds,fsize);
  strcat(cmds,"^FD");
  strcat(cmds,textLayer->content);
  strcat(cmds,"^FS");
  strcat(cmds,"\r\n");
  free(textLayer->font.Style);
  free(textLayer->font.FontName);
  free(textLayer->content);
  free(textLayer->baseLayer->layerName);
  free(textLayer->baseLayer);
  free(fName);
  free(textLayer);
  enqueue(ZPLqueue,cmds);
}
void HCreateBarcodeZPL(Queue*ZPLqueue,pageConfig *page,BarcodeLayer *barcodeLayer)
{
  //https://supportcommunity.zebra.cn/s/article/Creating-GS1-Barcodes-with-Zebra-Printers-for-Data-Matrix-and-Code-128-using-ZPL
   char *cmds= (char *)malloc(sizeof(char)*5000);
   int alignment=0;
   char xpos[10];
   int x=HCreateDPIChange(page->dpiX,DPI,barcodeLayer->baseLayer->x);
   sprintf(xpos, "%d", x);
   char ypos[10];
   int y=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->y);
   sprintf(ypos, "%d", y);
   char xdim[10];
   int xdimension=millimeterToPixel(DPI,barcodeLayer->XDimension);
   sprintf(xdim, "%d", xdimension);
   int hh=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->height);
   char height[10];
   sprintf(height, "%d", hh);
   int ww=HCreateDPIChange(page->dpiY,DPI,barcodeLayer->baseLayer->width);
   char width[10];
   sprintf(width,"%d",ww);

   char errorLevel[10];
   sprintf(errorLevel, "%d", barcodeLayer->errorLevel);
  

   unsigned char extAttr[10];
     //QR码字段定义
   char modlparam[10];
   char elevel[10];
   char maskno[10];
   char charactermode[10];
   char inputmodel[10];
   int count=base64_decode(barcodeLayer->ExtAttr,extAttr);

   int readable_aglinment=2;
   int narrow=2;
   int wide=2;
   char o[10];
   char h[10];
   char f[10];
   char g[10];
   char e[10];
   char k[10];
   char l[10];
   char s[10];
   char c[10];
   char r[10];
   char m[10];
   char t[10];
   char d[10];
   char b[10];
   char n[10];


  switch (barcodeLayer->baseLayer->Angle)
   {
    case 0:
    /* code */
    sprintf(o, "%s", "N");
    break;
     case 1:
    /* code */
     sprintf(o, "%s", "R");
    break;
     case 2:
    /* code */
    sprintf(o, "%s", "I");
    break;
     case 3:
    /* code */
    sprintf(o, "%s", "B");
    break;
   
   default:
    sprintf(o, "%s", "N");
    break;
   }
  if(barcodeLayer->isShowText==1)
      sprintf(f, "%s", "N");
  else
      sprintf(f, "%s", "Y");
  if(barcodeLayer->tp==UnderBarcode)
      sprintf(g, "%s", "N");
  else
      sprintf(g, "%s", "Y");
  switch (barcodeLayer->codeType)
  {
  case 0://EAN13
    /* code */
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BE");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
    break;
  case 2://UPCA
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BU");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
 case 3://UPCE
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^B9");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
 break; 
  case 1://EAN8
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^B8");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break; 
  case 11:
  case 53://code39
    if(barcodeLayer->ExtAttr[2]==0x00)
      sprintf(e, "%s", "N");
    else
      sprintf(e, "%s", "Y");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^B3");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 18://codabar
    if(barcodeLayer->ExtAttr[2]==0x00)
      sprintf(e, "%s", "N");
    else
      sprintf(e, "%s", "Y");
    switch (extAttr[3])
    {
      case 0:
      sprintf(k, "%s", "A");
      break;
      case 1:
      sprintf(k, "%s", "B");
      break;
      case 2:
      sprintf(k, "%s", "C");
      break;
      case 3:
      sprintf(k, "%s", "D");
      break;
    
      default:
      sprintf(k, "%s", "A");
      break;
    }
    switch (extAttr[4])
    {
      case 0:
      sprintf(l, "%s", "A");
      break;
      case 1:
      sprintf(l, "%s", "B");
      break;
      case 2:
      sprintf(l, "%s", "C");
      break;
      case 3:
      sprintf(l, "%s", "D");
      break;
    
      default:
      sprintf(l, "%s", "A");
      break;
    }
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BK");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,k);
     strcat(cmds,",");
     strcat(cmds,l);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;

  case 20://dm
      switch (extAttr[1])
      {
      case 0:
         sprintf(s, "%s", "0");
        break;
      case 1:
       sprintf(s, "%s", "50");
      break;
      case 2:
       sprintf(s, "%s", "80");
      break;
      case 3:
       sprintf(s, "%s", "100");
      break;
      case 4:
       sprintf(s, "%s", "140");
       break;
      case 5:
       sprintf(s, "%s", "200");
      break;
      
      default:
       sprintf(s, "%s", "0");
        break;
      }
      switch (extAttr[3])
      {
      case 0:
        sprintf(r, "%s", "0");
        sprintf(c, "%s", "0");
        break;
      case 1:
        sprintf(r, "%s", "12");
        sprintf(c, "%s", "12");
      break;
      case 3:
        sprintf(r, "%s", "14");
        sprintf(c, "%s", "14");
        break;
      case 4:
        sprintf(r, "%s", "16");
        sprintf(c, "%s", "16");
        break;
      case 5:
        sprintf(r, "%s", "18");
        sprintf(c, "%s", "18");
        break;
      case 6:
        sprintf(r, "%s", "20");
        sprintf(c, "%s", "20");
        break;
      case 7:
        sprintf(r, "%s", "22");
        sprintf(c, "%s", "22");
        break;
      case 8:
        sprintf(r, "%s", "24");
        sprintf(c, "%s", "24");
        break;
      case 9:
        sprintf(r, "%s", "26");
        sprintf(c, "%s", "26");
        break;
      case 10:
        sprintf(r, "%s", "32");
        sprintf(c, "%s", "32");
        break;
      case 11:
        sprintf(r, "%s", "36");
        sprintf(c, "%s", "36");
        break;
      case 12:
        sprintf(r, "%s", "40");
        sprintf(c, "%s", "40");
        break;
      case 13:
        sprintf(r, "%s", "44");
        sprintf(c, "%s", "44");
        break;
      case 14:
        sprintf(r, "%s", "48");
        sprintf(c, "%s", "48");
        break;
      case 15:
        sprintf(r, "%s", "52");
        sprintf(c, "%s", "52");
        break;
      case 16:
        sprintf(r, "%s", "64");
        sprintf(c, "%s", "64");
        break;
      case 17:
        sprintf(r, "%s", "72");
        sprintf(c, "%s", "72");
        break;
      case 18:
        sprintf(r, "%s", "80");
        sprintf(c, "%s", "80");
        break;
      case 19:
        sprintf(r, "%s", "88");
        sprintf(c, "%s", "88");
        break;
      case 20:
        sprintf(r, "%s", "96");
        sprintf(c, "%s", "96");
        break;
      case 21:
        sprintf(r, "%s", "104");
        sprintf(c, "%s", "104");
        break;
      case 22:
        sprintf(r, "%s", "120");
        sprintf(c, "%s", "120");
        break;
      case 23:
        sprintf(r, "%s", "132");
        sprintf(c, "%s", "132");
        break;
      case 24:
        sprintf(r, "%s", "144");
        sprintf(c, "%s", "144");
        break;
      case 25:
        sprintf(r, "%s", "8");
        sprintf(c, "%s", "18");
        break;
      case 26:
        sprintf(r, "%s", "8");
        sprintf(c, "%s", "32");
        break;
      case 27:
        sprintf(r, "%s", "12");
        sprintf(c, "%s", "26");
        break;
      case 28:
        sprintf(r, "%s", "12");
        sprintf(c, "%s", "36");
        break;
      case 29:
        sprintf(r, "%s", "16");
        sprintf(c, "%s", "36");
        break;
      case 30:
        sprintf(r, "%s", "16");
        sprintf(c, "%s", "48");
        break;
      
      
      default:
        sprintf(r, "%s", "80");
        sprintf(c, "%s", "80");
        break;
      }
      u_int8_t intf=extAttr[1]+1;
      sprintf(f, "%d", intf);
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BX");
     strcat(cmds,o);
     
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,s);
     strcat(cmds,",");
     strcat(cmds,c);
     strcat(cmds,",");
     strcat(cmds,r);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");


  break;

  case 25://code49
  switch (extAttr[1])
  {
  case 0:
    sprintf(f, "%s", "N");
    break;
  case 1:
     sprintf(f, "%s", "A");
  break;
  case 2:
   sprintf(f, "%s", "B");
  break;
  
  default:
    break;
  }
  if(extAttr[0]==6)
     sprintf(m, "%s", "A");
  else
     sprintf(m, "%d", (int)extAttr[0]);
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^B4");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,m);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 45://logmars
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BL");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 70:
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BM");
     strcat(cmds,o);
     strcat(cmds,",B,");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 34://plessey
     if(extAttr[2]==0x01)
      sprintf(e, "%s", "Y");
     else 
      sprintf(e, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BP");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 9://code93
     if(extAttr[2]==0x01)
      sprintf(e, "%s", "Y");
     else 
      sprintf(e, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BA");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 42:case 10://i2of5
    if(extAttr[2]==0x01)
      sprintf(e, "%s", "Y");
     else 
      sprintf(e, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^B2");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,"^FD");

     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 7:case 86:case 30://sscc18
     if(extAttr[2]==0x01)
      sprintf(e, "%s", "Y");
     else 
      sprintf(e, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BC");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,"D");
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 33://gs128
   if(extAttr[2]==0x01)
      sprintf(e, "%s", "Y");
     else 
      sprintf(e, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BC");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,"D");
     strcat(cmds,"^FD");
     strcat(cmds,">;>8");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 12://code128a
  if(extAttr[2]==0x01)
      sprintf(e, "%s", "Y");
     else 
      sprintf(e, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BC");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,"D");
     strcat(cmds,"^FD");
     strcat(cmds,">9");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 13://code128b
   if(extAttr[2]==0x01)
      sprintf(e, "%s", "Y");
     else 
      sprintf(e, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BC");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,"D");
     strcat(cmds,"^FD");
     strcat(cmds,">:");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");

  break;
  case 14://code128c
   if(extAttr[2]==0x01)
      sprintf(e, "%s", "Y");
     else 
      sprintf(e, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BC");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,"D");
     strcat(cmds,"^FD");
     strcat(cmds,">;");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 80://hibccode128
   if(extAttr[2]==0x01)
      sprintf(e, "%s", "Y");
     else 
      sprintf(e, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BC");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,"D");
     strcat(cmds,"^FD");
     strcat(cmds,">;");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 38://codeblockf

   if(extAttr[1]==0x01)
       sprintf(s, "%s", "N");
     else 
       sprintf(s, "%s", "Y");

     sprintf(r, "%d", (int)extAttr[2]);
     sprintf(c, "%d", (int)extAttr[1]);
     sprintf(m, "%s", "F");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BB");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,s);
     strcat(cmds,",");
     strcat(cmds,c);
     strcat(cmds,",");
     strcat(cmds,r);
     strcat(cmds,",");
     strcat(cmds,m);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");

  break;
  case 39://code11
    if(extAttr[2]==0x01)
       sprintf(e, "%s", "Y");
    else
       sprintf(e, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^B1");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 44://standard2of5

     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BJ");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");

  break;
  case 68://micropdf417
     sprintf(m, "%d", (int)extAttr[4]);
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BF");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,m);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 21:case 71://pdf417
     sprintf(r, "%d", (int)extAttr[1]);
     sprintf(c, "%d", (int)extAttr[0]);
     if(extAttr[3]==0x01)
         sprintf(t, "%s", "Y");
     else
         sprintf(t, "%s", "N");
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BF");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,errorLevel);
     strcat(cmds,",");
     strcat(cmds,c);
     strcat(cmds,",");
     strcat(cmds,r);
     strcat(cmds,",");
     strcat(cmds,t);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 47:case 48: case 49:case 50://postnet
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BZ");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",0");
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 77://postnet
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BZ");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",3");
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 73://postnet
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BZ");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",0");
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");

  break;
  case 75:
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^B1");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",3");
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 81:case 82: case 83: case 89: case 17: case 51://aztec

     if(barcodeLayer->codeSize>150)
       sprintf(m, "%d", (int)(barcodeLayer->codeSize/150));
     else
       sprintf(m, "%d", (int)barcodeLayer->codeSize);
      if(barcodeLayer->ECIMode==1)
        sprintf(c, "%s", "Y");
     else
        sprintf(c, "%s", "N");

      sprintf(d, "%d", barcodeLayer->errorLevel);

      if(barcodeLayer->ExtAttr[0]==0x01)
         sprintf(e, "%s", "Y");
      else
        sprintf(e, "%s", "N");
      
      sprintf(f, "%d", (int)extAttr[3]);
      sprintf(g, "%s", "0");

     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^B0");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,m);
     strcat(cmds,",");
     strcat(cmds,c);
     strcat(cmds,",");
     strcat(cmds,d);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 72:
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^B5");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,height);
     strcat(cmds,",");
     strcat(cmds,f);
     strcat(cmds,",");
     strcat(cmds,g);
     strcat(cmds,",3");
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 16:case 23:case 69: case 74: case 76: case 84: case 78: case 87://QR
     if(extAttr[3]==0x01)
         sprintf(m, "%d", 2);
      else
         sprintf(m, "%d", 1);
     if(barcodeLayer->codeSize>150)
       sprintf(c, "%d", (int)(barcodeLayer->codeSize/150));
     else
       sprintf(c, "%d", (int)barcodeLayer->codeSize);
     switch (barcodeLayer->errorLevel)
     {
      case 0:
        sprintf(d, "%s", "L");
      break;
      case 1:
        sprintf(d, "%s", "M");
      break;
      case 2:
        sprintf(d, "%s", "Q");
      break;
      case 3:
        sprintf(d, "%s", "H");
      break;
     
     default:
        sprintf(d, "%s", "L");
      break;
     }
      if(extAttr[0]!=8)
         sprintf(e, "%d", (int)extAttr[0]);
      else
         sprintf(e, "%d", 7);
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BQ");
     strcat(cmds,o);
     strcat(cmds,",");
     strcat(cmds,m);
     strcat(cmds,",");
     strcat(cmds,c);
     strcat(cmds,",");
     strcat(cmds,d);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");
  break;
  case 29:
   sprintf(m, "%d", (int)extAttr[0]);
   sprintf(n, "%d", 1);
   sprintf(t, "%d", 1);
     strcat(cmds,"^FO");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,"^BY");
     strcat(cmds,xdim);
     strcat(cmds,"\r\n");
     strcat(cmds,"^BD");

     strcat(cmds,m);
     strcat(cmds,",");
     strcat(cmds,n);
     strcat(cmds,",");
     strcat(cmds,t);

     strcat(cmds,"^FD");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"^FS");
     strcat(cmds,"\r\n");

  break;



  default:
    break;
  }
  free(barcodeLayer->EncodingMode);
  free(barcodeLayer->DataMode);
  free(barcodeLayer->ExtAttr);
  free(barcodeLayer->supplenmentText);
  free(barcodeLayer->content);
  free(barcodeLayer->baseLayer->layerName);
  free(barcodeLayer->baseLayer);
  free(barcodeLayer);
  enqueue(ZPLqueue,cmds);
}
void HCreateLineZPL(Queue*ZPLqueue,pageConfig *page,LineLayer *lineLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char xpos[10];
  int x=HCreateDPIChange(page->dpiX,DPI,lineLayer->baseLayer->x);
  sprintf(xpos, "%d", x);
  char ypos[10];
  int y=HCreateDPIChange(page->dpiY,DPI,lineLayer->baseLayer->y);
  sprintf(ypos, "%d", y);

  char width[10];
  char height[10];

  char linewidth[10];
  int w=HCreateDPIChange(page->dpiY,DPI,lineLayer->lineWidth);
  sprintf(linewidth, "%d", w);
  int endx;
  int endy;

  if(lineLayer->linDirect==1)//横线
  {
      endx=HCreateDPIChange(page->dpiX,DPI,lineLayer->baseLayer->x+lineLayer->lineLength);
      endy=y;
    
     
  }else//竖线
  {
      endx=x;
      endy=HCreateDPIChange(page->dpiX,DPI,lineLayer->baseLayer->y+lineLayer->lineLength);
    
   
  }

  if(x==endx)
  {
      sprintf(width, "%d", 0);
      sprintf(height, "%d", endy-x);
  }
  if(y==endy)
  {
      sprintf(width, "%d", endx-x);
      sprintf(height, "%d", 0);
  }

  strcat(cmds,"^FO");
  strcat(cmds,xpos);
  strcat(cmds,",");
  strcat(cmds,ypos);
  strcat(cmds,"^GB");
  strcat(cmds,width);
  strcat(cmds,",");
  strcat(cmds,height);
  strcat(cmds,",");
  strcat(cmds,linewidth);
  strcat(cmds,",");
  strcat(cmds,"B");
  strcat(cmds,",");
  strcat(cmds,"0");
  strcat(cmds,"^FS");
  strcat(cmds,"\r\n");


  free(lineLayer->baseLayer->layerName);
  
  free(lineLayer->baseLayer);

  free(lineLayer);
  enqueue(ZPLqueue,cmds);

}
void HCreateRectangleZPL(Queue*ZPLqueue,pageConfig *page,RectangleLayer *rectLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char xpos[10];
  int x=HCreateDPIChange(page->dpiX,DPI,rectLayer->baseLayer->x);
  sprintf(xpos, "%d", x);
  char ypos[10];
  int y=HCreateDPIChange(page->dpiY,DPI,rectLayer->baseLayer->y);
  sprintf(ypos, "%d", y);

  char width[10];
  char height[10];
  char radius[10];
  int w=HCreateDPIChange(page->dpiX,DPI,rectLayer->RectangWidth);
  int h=HCreateDPIChange(page->dpiX,DPI,rectLayer->RectangHeight);
  sprintf(width, "%d", w);
  sprintf(height, "%d", h);
  sprintf(radius, "%d", rectLayer->RectanRadius%8);

  char linewidth[10];
  int lw=HCreateDPIChange(page->dpiY,DPI,rectLayer->lineWidth);
  sprintf(linewidth, "%d", lw);

  strcat(cmds,"^FO");
  strcat(cmds,xpos);
  strcat(cmds,",");
  strcat(cmds,ypos);
  strcat(cmds,"^GB");
  strcat(cmds,width);
  strcat(cmds,",");
  strcat(cmds,height);
  strcat(cmds,",");
  strcat(cmds,linewidth);
  strcat(cmds,",");
  strcat(cmds,"B");
  strcat(cmds,",");
  strcat(cmds,radius);
  strcat(cmds,"^FS");
  strcat(cmds,"\r\n");
  free(rectLayer->baseLayer->layerName);
  free(rectLayer->baseLayer);
  free(rectLayer);

  enqueue(ZPLqueue,cmds);

}
void HCreateEllipseZPL(Queue*ZPLqueue,pageConfig *page,EllipseLayer *ellipseLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char xpos[10];
  int x=HCreateDPIChange(page->dpiX,DPI,ellipseLayer->baseLayer->x);
  sprintf(xpos, "%d", x);
  char ypos[10];
  int y=HCreateDPIChange(page->dpiY,DPI,ellipseLayer->baseLayer->y);
  sprintf(ypos, "%d", y);
  char width[10];
  char height[10];
  int w=HCreateDPIChange(page->dpiX,DPI,ellipseLayer->EllipseWidth);
  int h=HCreateDPIChange(page->dpiX,DPI,ellipseLayer->EllipseHeight);
  sprintf(width, "%d", w);
  sprintf(height, "%d", h);
  char linewidth[10];
  int lw=HCreateDPIChange(page->dpiY,DPI,ellipseLayer->lineWidth);
  sprintf(linewidth, "%d", lw);

  if(ellipseLayer->EllipseHeight==ellipseLayer->EllipseWidth)//圆形
  {
  strcat(cmds,"^FO");
  strcat(cmds,xpos);
  strcat(cmds,",");
  strcat(cmds,ypos);
  strcat(cmds,"^GC");
  strcat(cmds,width);
  strcat(cmds,",");
  strcat(cmds,linewidth);
  strcat(cmds,",");
  strcat(cmds,"B");
  strcat(cmds,"^FS");
  strcat(cmds,"\r\n");
  }else//椭圆
  {
  strcat(cmds,"^FO");
  strcat(cmds,xpos);
  strcat(cmds,",");
  strcat(cmds,ypos);
  strcat(cmds,"^GE");
  strcat(cmds,width);
  strcat(cmds,",");
  strcat(cmds,height);
  strcat(cmds,",");
  strcat(cmds,linewidth);
  strcat(cmds,",");
  strcat(cmds,"B");
  strcat(cmds,"^FS");
  strcat(cmds,"\r\n");
  }
  free(ellipseLayer->baseLayer->layerName);
  free(ellipseLayer->baseLayer);
  free(ellipseLayer);

  enqueue(ZPLqueue,cmds);

}
void HCreateImageZPL(Queue*ZPLqueue,pageConfig *page,ImageLayer *imageLayer)
{
  free(imageLayer->img);
  free(imageLayer->baseLayer->layerName);
  
  free(imageLayer->baseLayer);

  free(imageLayer);
  //enqueue(ZPLqueue,cmds);
}
void HCreateRfidZPL(Queue*ZPLqueue,pageConfig *page,RfidLayer *rfidLayer)
{
    free(rfidLayer->baseLayer->layerName);
    free(rfidLayer->RfidContent);
    free(rfidLayer->baseLayer);
    free(rfidLayer);
}
