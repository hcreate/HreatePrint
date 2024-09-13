#include"dpl.h"
void printDPL(char *p)
{
  Queue* DPLqueue=createQueue();
  pageConfig *page=getPageConfigDPL(p);
  HCreateStartDPL(DPLqueue,page);
  HCreateUintsDPL(DPLqueue,page,0);
  HCreateSetPageSizeDPL(DPLqueue,page);
  HCreateHomeDPL(DPLqueue,page);
  HCreateObjectDPL(DPLqueue,page);
  HCreateEndDPL(DPLqueue,page);
//输出打印，并释放内存
  while(!isEmpty(DPLqueue))
   {
     char *data=dequeue(DPLqueue);
     printf("%s",data);
     free(data);

   }
  free(DPLqueue);
  freePage(page);
  //printf("%s\n",cmds);
}
pageConfig *getPageConfigDPL(char *p)
{
  FILE *file=open(p);
  Queue* queue=  readToQueue(file);
  pageConfig *page=NULL;
  page= prase(queue);
  return page;
}

void HCreateStartDPL(Queue*DPLqueue,pageConfig *page)
{
    char *cmds=(char *)malloc(sizeof(char)*100);
    strcat(cmds,"<STX>L\n");
    enqueue(DPLqueue,cmds);
}
void HCreateUintsDPL(Queue*DPLqueue,pageConfig *page,u_int8_t units)
{
    char *cmds=(char *)malloc(sizeof(char)*100);
    if(units==0)
      strcat(cmds,"m\n");
    else 
      strcat(cmds,"n\n");

}
void HCreateHomeDPL(Queue*DPLqueue,pageConfig *page)
{
     char *cmds=(char *)malloc(sizeof(char)*100);
     int offset=pixelToMillimeter(page->dpiX,page->pagePadding);
     char ori[10];
     sprintf(ori, "%04d", offset); 
     strcat(cmds,"R");
     strcat(cmds,ori);
     strcat(cmds,"<CR>\n");
     strcat(cmds,"C");
     strcat(cmds,ori);
     strcat(cmds,"<CR>\n");
     enqueue(DPLqueue,cmds);
}
void HCreateSetPageSizeDPL(Queue*DPLqueue,pageConfig *page)
{  
     char *cmds=(char *)malloc(sizeof(char)*100);
     int width=pixelToMillimeter(page->dpiX,page->pageWidth);
     int height=pixelToMillimeter(page->dpiX,page->pageHeight);
     char pageWidth[10];
     char pageHeight[10];
     sprintf(pageWidth, "%04d", width); 
     sprintf(pageHeight, "%04d", height); 
     strcat(cmds,"<STX>KW");
     strcat(cmds,pageWidth);
     strcat(cmds,"<CR>\n");
     strcat(cmds,"<STX>Kc");
     strcat(cmds,pageHeight);
     strcat(cmds,"<CR>\n");
     enqueue(DPLqueue,cmds);
}
void HCreateObjectDPL(Queue*DPLqueue,pageConfig *page)
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
      HCreateTextDPL(DPLqueue,page,current->layerData);

      break;
    case Barcode:
     //BarcodeLayer *barcodeObject=(BarcodeLayer *)current->layerData;
      /* code */
      HCreateBarcodeDPL(DPLqueue,page,current->layerData);
    break;
    case Rfid:
    // RfidLayer *rfidObject=(RfidLayer *)current->layerData;
      HCreateRfidDPL(DPLqueue,page,current->layerData);
      /* code */
    break;
    case Image:
     //ImageLayer *imageObject=(ImageLayer *)current->layerData;
      /* code */
      HCreateImageDPL(DPLqueue,page,current->layerData);
    break;
    case Line:
     //LineLayer *lineObject=(LineLayer *)current->layerData;
      HCreateLineDPL(DPLqueue,page,current->layerData);
      /* code */
    break;
    case Rectangle:
    // RectangleLayer *rectangleObject=(RectangleLayer *)current->layerData;
      /* code */
      HCreateRectangleDPL(DPLqueue,page,current->layerData);
    break;
    case Ellipse:
    //EllipseLayer *ellipseObject=(EllipseLayer *)current->layerData;
     HCreateEllipseDPL(DPLqueue,page,current->layerData);
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
void HCreateBarcodeDPL(Queue*DPLqueue,pageConfig *page,BarcodeLayer *barcodeLayer)
{
     char *cmds= (char *)malloc(sizeof(char)*5000);
     unsigned char extAttr[10];
     int count=base64_decode(barcodeLayer->ExtAttr,extAttr);
     char xpos[10];
     int x=pixelToMillimeter(page->dpiX,barcodeLayer->baseLayer->x)*10;
     sprintf(xpos, "%04d", x);
     char ypos[10];
     int y=pixelToMillimeter(page->dpiY,barcodeLayer->baseLayer->y)*10;
     int pageHeight=pixelToMillimeter(page->dpiY,page->pageHeight);
     y=(int)(pageHeight*10-y);
     sprintf(ypos, "%04d", y);
     char angle[10];
     sprintf(angle, "%d", barcodeLayer->baseLayer->Angle);
     int w=0;
     int h=0;
     char width[10];
     char height[10];
     h=barcodeLayer->baseLayer->height*10;
     w=barcodeLayer->baseLayer->width*10;
     sprintf(width, "%03d", w);
     sprintf(height, "%03d", h);

     char xdim[10];
     int xdimension=barcodeLayer->XDimension*10;
     sprintf(xdim, "%d", xdimension);
     char errorLevel[10];
     sprintf(errorLevel, "%d", barcodeLayer->errorLevel);

     //PDF ROW  COL  RC
     char row[10];
     char col[10];
     char rc[10];
     sprintf(rc, "%02d,",(int)extAttr[2]);
     sprintf(row, "%02d,",(int)extAttr[1]);
     sprintf(col, "%02d,",(int)extAttr[0]);

     //START
     strcat(cmds,angle); 
     switch (barcodeLayer->codeType)
     {
     case 0://ean13
      /* code */
      if(barcodeLayer->isShowText==1)
        strcat(cmds,"F");
      else
        strcat(cmds,"f");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");

      break;
     case 1://ean8
       if(barcodeLayer->isShowText==1)
        strcat(cmds,"G");
      else
        strcat(cmds,"g");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 2://ucpa
     if(barcodeLayer->isShowText==1)
     {
      strcat(cmds,"B");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
      if(isStringEmpty(barcodeLayer->supplenmentText)==0)//无supplenmentText
      {
        strcat(cmds,angle); 
         if(strlen(barcodeLayer->supplenmentText)==2)
         {
           strcat(cmds,"M");
         }
         if(strlen(barcodeLayer->supplenmentText)==5)
         {
            strcat(cmds,"N");
         }
           strcat(cmds,"0");
           strcat(cmds,"0");
           strcat(cmds,height);
           strcat(cmds,xpos);
           strcat(cmds,ypos);
           strcat(cmds,barcodeLayer->content);
           strcat(cmds,"<CR>\n");
      }
  
     }
     else
     {
      strcat(cmds,"b");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
      if(isStringEmpty(barcodeLayer->supplenmentText)==0)//有supplenmentText
      {
        strcat(cmds,angle); 
         if(strlen(barcodeLayer->supplenmentText)==2)
         {
           strcat(cmds,"m");
         }
         if(strlen(barcodeLayer->supplenmentText)==5)
         {
            strcat(cmds,"n");
         }
           strcat(cmds,"0");
           strcat(cmds,"0");
           strcat(cmds,height);
           strcat(cmds,xpos);
           strcat(cmds,ypos);
           strcat(cmds,barcodeLayer->content);
           strcat(cmds,"<CR>\n");
      }
  
     }

     break;
     case 3://upce
     if(barcodeLayer->isShowText==1)
        strcat(cmds,"C");
      else
        strcat(cmds,"c");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 7://gs128
     if(barcodeLayer->isShowText==1)
        strcat(cmds,"Q");
      else
        strcat(cmds,"q");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 8://gs1dm

      strcat(cmds,"W1c");
      strcat(cmds,"4");
      strcat(cmds,"4");
      strcat(cmds,"000");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,"000");
      strcat(cmds,"0");
      strcat(cmds,"000");
      strcat(cmds,"000");
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
       
     break;
     case 9://code93
      if(barcodeLayer->isShowText==1)
        strcat(cmds,"O");
      else
        strcat(cmds,"o");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 11: case 12: case 13: case 14://code128abc
     if(barcodeLayer->isShowText==1)
        strcat(cmds,"E");
      else
        strcat(cmds,"e");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 16:case 23://qr
      strcat(cmds,"W1d");
      strcat(cmds,"1");
      strcat(cmds,"1");
      strcat(cmds,"000");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 17://aztec
      strcat(cmds,"W1f");
      strcat(cmds,"1");
      strcat(cmds,"1");
      strcat(cmds,"000");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,"0");
      strcat(cmds,"050");
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 18://codabar
     if(barcodeLayer->isShowText==1)
        strcat(cmds,"I");
      else
        strcat(cmds,"i");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 20://pdf
      strcat(cmds,"z");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,"F");
      strcat(cmds,errorLevel);
      strcat(cmds,rc);
      strcat(cmds,row);
      strcat(cmds,col);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 24://code16k
       if(barcodeLayer->isShowText==1)
        strcat(cmds,"W1Y");
      else
        strcat(cmds,"W1y");
      strcat(cmds,"3");
      strcat(cmds,"3");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 29://maxicode
      strcat(cmds,"u");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 41://iata2of5
     if(barcodeLayer->isShowText==1)
        strcat(cmds,"L");
      else
        strcat(cmds,"l");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 42://interleaved2of5
      if(barcodeLayer->isShowText==1)
        strcat(cmds,"D");
      else
        strcat(cmds,"d");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
      break;
      case 44://standard2of5
      if(barcodeLayer->isShowText==1)
        strcat(cmds,"W1S");
      else
        strcat(cmds,"W1s");
      strcat(cmds,"8");
      strcat(cmds,"3");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
      break;
      case 51://aztecrunes
      strcat(cmds,"W1f");
      strcat(cmds,"1");
      strcat(cmds,"1");
      strcat(cmds,"000");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,"0");
      strcat(cmds,"050");
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
      break;
      case 55://databarexpanded
      case 56:
      strcat(cmds,"W1k");
      strcat(cmds,"2");
      strcat(cmds,"2");
      strcat(cmds,"000");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,"E");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,"00");
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
      break;

      case 57://databarlimited/omni/stacked
      case 58:
      case 59:
      case 60:
      case 61:
      strcat(cmds,"W1k");
      strcat(cmds,"2");
      strcat(cmds,"2");
      strcat(cmds,"000");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      if(barcodeLayer->codeType==57)
       strcat(cmds,"L");
      if(barcodeLayer->codeType==58)
       strcat(cmds,"D");
      if(barcodeLayer->codeType==59)
       strcat(cmds,"D");
      if(barcodeLayer->codeType==60)
       strcat(cmds,"S");
      if(barcodeLayer->codeType==61)
       strcat(cmds,"T");

      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,"00");
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
      break;
     case 68://mircropdf417
      strcat(cmds,"W1z");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,"1");
      strcat(cmds,"4");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 69://microqrcode
      strcat(cmds,"W1d");
      strcat(cmds,"1");
      strcat(cmds,"1");
      strcat(cmds,"000");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 70://msiplessey
      strcat(cmds,"K");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 71://pdf417truncated
      strcat(cmds,"z");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,"T");
      strcat(cmds,errorLevel);
      strcat(cmds,rc);
      strcat(cmds,row);
      strcat(cmds,col);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 72://PLANET
      strcat(cmds,"W1L");
      strcat(cmds,"4");
      strcat(cmds,"4");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 73://postnet
      strcat(cmds,"p");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 74:case 78: case 84: case 87:
      strcat(cmds,"W1d");
      strcat(cmds,"1");
      strcat(cmds,"1");
      strcat(cmds,"000");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 79: case 85: case 88://dm
      strcat(cmds,"W1c");
      strcat(cmds,"4");
      strcat(cmds,"4");
      strcat(cmds,"000");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,"000");
      strcat(cmds,"0");
      strcat(cmds,"000");
      strcat(cmds,"000");
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
     break;
     case 80:
      if(barcodeLayer->isShowText==1)
        strcat(cmds,"H");
      else
        strcat(cmds,"h");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
      break;
    case 81:case 82:case 83: case 89://aztec
      strcat(cmds,"W1f");
      strcat(cmds,"1");
      strcat(cmds,"1");
      strcat(cmds,"000");
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,"0");
      strcat(cmds,"050");
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
    break;
    
    default:
     if(barcodeLayer->isShowText==1)
        strcat(cmds,"E");
      else
        strcat(cmds,"e");
      strcat(cmds,"0");
      strcat(cmds,"0");
      strcat(cmds,height);
      strcat(cmds,xpos);
      strcat(cmds,ypos);
      strcat(cmds,barcodeLayer->content);
      strcat(cmds,"<CR>\n");
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

  enqueue(DPLqueue,cmds);  

     


}
void HCreateTextDPL(Queue*DPLqueue,pageConfig *page,TextLayer *textLayer)
{
     char *cmds= (char *)malloc(sizeof(char)*5000);
     char xpos[10];
     int x=pixelToMillimeter(page->dpiX,textLayer->baseLayer->x)*10;
     sprintf(xpos, "%04d", x);
     char ypos[10];
     int y=pixelToMillimeter(page->dpiY,textLayer->baseLayer->y)*10;
     int fontSize=(int)(textLayer->font.FontSize/10);
     if(fontSize>8)
        fontSize=8;
     char fsize[10];
     sprintf(fsize, "%d", fontSize/10);
     int pageHeight=pixelToMillimeter(page->dpiY,page->pageHeight);
     y=(int)(pageHeight*10+(fontSize/2)*10-y);
     sprintf(ypos, "%04d", y);
     char angle[10];
     sprintf(angle, "%d", textLayer->baseLayer->Angle);
     strcat(cmds,angle);
     strcat(cmds,fsize);
     strcat(cmds,"1");
     strcat(cmds,"1");
     strcat(cmds,"000");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     strcat(cmds,textLayer->content);
     strcat(cmds,"<CR>\n");
     
     enqueue(DPLqueue,cmds);
    
     
}
void HCreateLineDPL(Queue*DPLqueue,pageConfig *page,LineLayer *lineLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char xpos[10];
  int x=pixelToMillimeter(page->dpiX,lineLayer->baseLayer->x)*10;
  sprintf(xpos, "%04d", x);
  char ypos[10];
  int y=pixelToMillimeter(page->dpiY,lineLayer->baseLayer->y)*10;
  int pageHeight=pixelToMillimeter(page->dpiY,page->pageHeight);
   y=(int)(pageHeight*10-y);
  sprintf(ypos, "%04d", y);
   char lineLength[10];
   int length=pixelToMillimeter(page->dpiX,lineLayer->lineLength)*10;
   if(length>999)
    sprintf(lineLength, "%04d", length);
    else
    sprintf(lineLength, "%03d", length);
   char lineWidth[10];
   int   width=pixelToMillimeter(page->dpiY,lineLayer->lineWidth)*10;
   if(width>999)
    sprintf(lineWidth, "%04d", width);
    else
    sprintf(lineWidth, "%03d", width);
   char end_x[10];
   char end_y[10];
   if(lineLayer->linDirect==1)
   {
     sprintf(end_x, "%04d", x+length);
     sprintf(end_y, "%04d", y);
     strcat(cmds,"X");
     strcat(cmds,"1");
     strcat(cmds,"1");
     strcat(cmds,"000");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     strcat(cmds,"l");
     strcat(cmds,lineLength);
     strcat(cmds,lineWidth);
     strcat(cmds,"<CR>\n");

   }else
   {
     sprintf(end_x, "%04d", x);
     sprintf(end_y, "%04d", y+length);
     strcat(cmds,"X");
     strcat(cmds,"1");
     strcat(cmds,"1");
     strcat(cmds,"000");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     strcat(cmds,"L");
     strcat(cmds,lineWidth);
     strcat(cmds,lineLength);
     strcat(cmds,"<CR>\n");
   }
   free(lineLayer->baseLayer->layerName);
   free(lineLayer->baseLayer);
   free(lineLayer);
   enqueue(DPLqueue,cmds);



}
void HCreateRectangleDPL(Queue*DPLqueue,pageConfig *page,RectangleLayer *rectLayer)
{

  char *cmds= (char *)malloc(sizeof(char)*5000);
  char xpos[10];
  int x=pixelToMillimeter(page->dpiX,rectLayer->baseLayer->x)*10;
  sprintf(xpos, "%04d", x);
  char ypos[10];
  int y=pixelToMillimeter(page->dpiY,rectLayer->baseLayer->y)*10;
  int pageHeight=pixelToMillimeter(page->dpiY,page->pageHeight);
   y=(int)(pageHeight*10-y);
  sprintf(ypos, "%04d", y);

   char lineWidth[10];
   int   lwidth=pixelToMillimeter(page->dpiY,rectLayer->lineWidth)*10;
 
    
  
  char width[10];
   int   rwidth=pixelToMillimeter(page->dpiX,rectLayer->RectangWidth)*10;

    
  char height[10];
  int   rheight=pixelToMillimeter(page->dpiY,rectLayer->RectangHeight)*10;

    

   if(lwidth>999||rwidth>999||rheight>999)
   {
     sprintf(lineWidth, "%04d", lwidth);
     sprintf(width, "%04d", rwidth);
     sprintf(height, "%04d", rheight);
   }else
   {
       sprintf(lineWidth, "%03d", lwidth);
       sprintf(width, "%03d", rwidth);
       sprintf(height, "%03d", rheight);
   } 

     strcat(cmds,"X");
     strcat(cmds,"1");
     strcat(cmds,"1");
     strcat(cmds,"000");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     if(lwidth>999||rwidth>999||rheight>999)
      strcat(cmds,"b");
     else
      strcat(cmds,"B");
     strcat(cmds,"");
     strcat(cmds,width);
     strcat(cmds,height);
     strcat(cmds,lineWidth);
     strcat(cmds,lineWidth);
     strcat(cmds,"<CR>\n");

   free(rectLayer->baseLayer->layerName);
   free(rectLayer->baseLayer);
   free(rectLayer);

   enqueue(DPLqueue,cmds); 
}
void HCreateEllipseDPL(Queue*DPLqueue,pageConfig *page,EllipseLayer *ellipseLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char xpos[10];
  int x=pixelToMillimeter(page->dpiX,ellipseLayer->baseLayer->x)*10;
  sprintf(xpos, "%04d", x);
  char ypos[10];
  int y=pixelToMillimeter(page->dpiY,ellipseLayer->baseLayer->y)*10;
  int pageHeight=pixelToMillimeter(page->dpiY,page->pageHeight);
   y=(int)(pageHeight*10-y);
  sprintf(ypos, "%04d", y);

   char lineWidth[10];
   int   lwidth=pixelToMillimeter(page->dpiY,ellipseLayer->lineWidth)*10;
 
    
  
  char width[10];
   int   rwidth=pixelToMillimeter(page->dpiX,ellipseLayer->EllipseWidth)*10;

    
  char height[10];
  int   rheight=pixelToMillimeter(page->dpiY,ellipseLayer->EllipseHeight)*10;

    

   if(lwidth>999||rwidth>999||rheight>999)
   {
     sprintf(lineWidth, "%04d", lwidth);
     sprintf(width, "%04d", rwidth);
     sprintf(height, "%04d", rheight);
   }else
   {
       sprintf(lineWidth, "%03d", lwidth);
       sprintf(width, "%03d", rwidth);
       sprintf(height, "%03d", rheight);
   } 
   if(rwidth==rheight)
   {
     strcat(cmds,"X");
     strcat(cmds,"1");
     strcat(cmds,"1");
     strcat(cmds,"000");
     strcat(cmds,xpos);
     strcat(cmds,ypos);
     strcat(cmds,"C");
     strcat(cmds,"0010001");
     strcat(cmds,width);
     strcat(cmds,"<CR>\n");
   }else
   {

     int r0=x+rwidth/2;
     int r1=y+rheight/2;
     char x0[10];
     char y0[10];
     sprintf(x0, "%04d", r0);
     sprintf(y0, "%04d", r1);
     strcat(cmds,"X");
     strcat(cmds,"1");
     strcat(cmds,"1");
     strcat(cmds,"000");
     strcat(cmds,x0);
     strcat(cmds,y0);
     strcat(cmds,"C");
     strcat(cmds,"0010001");
     strcat(cmds,width);
     strcat(cmds,"<CR>\n");

   }

  free(ellipseLayer->baseLayer->layerName);
  free(ellipseLayer->baseLayer);
  free(ellipseLayer);
  enqueue(DPLqueue,cmds);
}
void HCreateImageDPL(Queue*DPLqueue,pageConfig *page,ImageLayer *imageLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*(100+strlen(imageLayer->img)));

  free(imageLayer->img);
  free(imageLayer->baseLayer->layerName);
  
  free(imageLayer->baseLayer);

  free(imageLayer);
  enqueue(DPLqueue,cmds);
}
void HCreateRfidDPL(Queue*DPLqueue,pageConfig *page,RfidLayer *rfidLayer)
{
    free(rfidLayer->baseLayer->layerName);
    free(rfidLayer->RfidContent);
    free(rfidLayer->baseLayer);
    free(rfidLayer);
}
void HCreateEndDPL(Queue*DPLqueue,pageConfig *page)
{
    char *cmds=(char *)malloc(sizeof(char)*100);
    strcat(cmds,"E\n");
    enqueue(DPLqueue,cmds);
}