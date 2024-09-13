#include"tpcl.h"
void printTPCL(char *p)
{
  //printf("%s",p);
    Queue* TPCLqueue=createQueue();
    pageConfig *page=getPageConfigTPCL(p);
    HCreateSetPageSizeTPCL(TPCLqueue,page);
    HCreateClearBufferTPCL(TPCLqueue,page);
    HCreateObjectTPCL(TPCLqueue,page);
    HCreatePrintTPCL(TPCLqueue,page);
    //输出打印，并释放内存
    while(!isEmpty(TPCLqueue))
    {
     char *data=dequeue(TPCLqueue);
     printf("%s",data);
     free(data);

    }
    free(TPCLqueue);
    freePage(page);
}
pageConfig *getPageConfigTPCL(char *p)
{
  FILE *file=open(p);
  Queue* queue=  readToQueue(file);
  pageConfig *page=NULL;
  page= prase(queue);
  return page;
}

void HCreateSetPageSizeTPCL(Queue*TPCLqueue,pageConfig *page)
{
  char *cmds=(char *)malloc(sizeof(char)*100);

  float m=pixelToMillimeter(page->dpiX,page->pageWidth);
  float n=pixelToMillimeter(page->dpiY,page->pageHeight);
  float v=pixelToMillimeter(page->dpiY,page->pageVerticalSpacing);
  float h=pixelToMillimeter(page->dpiY,page->pageHorizontalSpacing);
  char pageHeight[10];
  char pageWidth[10];
  char height[10];
  char width[10];
  sprintf(pageHeight, "%04d", (int)((n+v)*10));
  sprintf(width, "%04d", (int)m*10);
  sprintf(height, "%04d", (int)n*10);
  sprintf(pageWidth, "%04d", (int)((m+h)*10));

  strcat(cmds,"{D");
  strcat(cmds,pageHeight);
  strcat(cmds,",");
  strcat(cmds,width);
  strcat(cmds,",");
  strcat(cmds,height);
  strcat(cmds,",");
  strcat(cmds,pageWidth);
  strcat(cmds,"|}");
  strcat(cmds,"\r\n");
  enqueue(TPCLqueue,cmds);
}
void HCreatePrintTPCL(Queue*TPCLqueue,pageConfig *page)
{
  char *cmds=(char *)malloc(sizeof(char)*100);
  strcat(cmds,"{XS:I,0001,0002C3000|}");
  strcat(cmds,"\r\n");
  enqueue(TPCLqueue,cmds);
}
void HCreateClearBufferTPCL(Queue*TPCLqueue,pageConfig *page)
{
 char *cmds=(char *)malloc(sizeof(char)*100);
 strcat(cmds,"{C|}");
 strcat(cmds,"\r\n");
 enqueue(TPCLqueue,cmds);


}
void HCreateObjectTPCL(Queue*TPCLqueue,pageConfig *page)
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
      HCreateTextTPCL(TPCLqueue,page,current->layerData);
      icnt++;
      break;
    case Barcode:
     //BarcodeLayer *barcodeObject=(BarcodeLayer *)current->layerData;
      /* code */
      HCreateBarcodeTPCL(TPCLqueue,page,current->layerData);
      icnt++;
    break;
    case Rfid:
    // RfidLayer *rfidObject=(RfidLayer *)current->layerData;
     HCreateRfidTPCL(TPCLqueue,page,current->layerData);
     icnt++;
      /* code */
    break;
    case Image:
     //ImageLayer *imageObject=(ImageLayer *)current->layerData;
      /* code */
      HCreateImageTPCL(TPCLqueue,page,current->layerData);
      icnt++;
    break;
    case Line:
     //LineLayer *lineObject=(LineLayer *)current->layerData;
      HCreateLineTPCL(TPCLqueue,page,current->layerData);
      icnt++;
      /* code */
    break;
    case Rectangle:
    // RectangleLayer *rectangleObject=(RectangleLayer *)current->layerData;
      /* code */
      HCreateRectangleTPCL(TPCLqueue,page,current->layerData);

      icnt++;
    break;
    case Ellipse:
    //EllipseLayer *ellipseObject=(EllipseLayer *)current->layerData;
     HCreateEllipseTPCL(TPCLqueue,page,current->layerData);
     icnt++;
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
void HCreateBarcodeTPCL(Queue*TPCLqueue,pageConfig *page,BarcodeLayer *barcodeLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char xpos[10];
  int x=pixelToMillimeter(page->dpiX,barcodeLayer->baseLayer->x);
  sprintf(xpos, "%04d", x*10);
  char ypos[10];
  int y=pixelToMillimeter(page->dpiY,barcodeLayer->baseLayer->y);
  sprintf(ypos, "%04d", y*10);

  char d[10];
  char e[10];
  char ff[10];
  char k[10];
  char llll[10];
  int moduleWidth=millimeterToPixel(page->dpiX,barcodeLayer->XDimension);
  sprintf(ff, "%d", moduleWidth%15);
  sprintf(k, "%d", barcodeLayer->baseLayer->Angle);
  sprintf(llll, "%04d", (int)(barcodeLayer->baseLayer->height*10));
  char cnt[10];
  sprintf(cnt, "%02d", icnt);
  switch (barcodeLayer->codeType)
  {
  case 0://ean13
     sprintf(d, "%d", "5");
     sprintf(e, "%d", "2");
     strcat(cmds,"{XB");
     strcat(cmds,cnt);
     strcat(cmds,";");
     strcat(cmds,xpos);
     strcat(cmds,",");
     strcat(cmds,ypos);
     strcat(cmds,",");
     strcat(cmds,d);
     strcat(cmds,",");
     strcat(cmds,e);
     strcat(cmds,",");
     strcat(cmds,ff);
     strcat(cmds,",");
     strcat(cmds,k);
     strcat(cmds,",");
     strcat(cmds,llll);
     strcat(cmds,",");
     strcat(cmds,barcodeLayer->content);
     strcat(cmds,"|}");
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

  enqueue(TPCLqueue,cmds);  

}
void HCreateTextTPCL(Queue*TPCLqueue,pageConfig *page,TextLayer *textLayer)
{
  char *cmds= (char *)malloc(sizeof(char)*5000);
  char *fName =(char *)malloc(sizeof(char)*20);
  sprintf(fName, "\"%s\",", FONTNAME);

  int FontSize;
  FontSize=textLayer->font.FontSize;
  FontSize=FontSize/10;
  int xmul,ymul;
  int rotation=0;
  if(FontSize==0)
  {
    xmul=1;
    ymul=1;
  }else
  {
    xmul=FontSize;
    ymul=FontSize;
  }
  char xMagnification[10];
  char yMagnification[10];
  sprintf(xMagnification, "%d", FontSize);
  sprintf(yMagnification, "%d", FontSize);

 // char fsize[10];
 // sprintf(fsize, "%d*10,", FontSize);

  char xpos[10];
  int x=pixelToMillimeter(page->dpiX,textLayer->baseLayer->x);
  sprintf(xpos, "%04d", x*10);
  char ypos[10];
  int y=pixelToMillimeter(page->dpiY,textLayer->baseLayer->y);
  sprintf(ypos, "%04d", y*10);

  char cnt[10];

  sprintf(cnt, "%03d", icnt);
  


  switch (textLayer->baseLayer->Angle)
  {
    case 0:
    /* code */
    rotation=0;
    break;
    case 1:
     rotation=11;
    break;
    case 2:
      rotation=22;
    break;
    case 3:
        rotation=33;
    break;

  
  default:
    rotation=0;
    break;
  }
  char angle[10];
  sprintf(angle, "%02d,", rotation);
  strcat(cmds,"{PC");
  strcat(cmds,cnt);
  strcat(cmds,";");
  strcat(cmds,xpos);
  strcat(cmds,",");
  strcat(cmds,ypos);
  strcat(cmds,",");
  strcat(cmds,xMagnification);
  strcat(cmds,",");
  strcat(cmds,yMagnification);
  strcat(cmds,",");
  strcat(cmds,fName);
  strcat(cmds,",");
  strcat(cmds,angle);
  strcat(cmds,",");
  if(textLayer->color.r==255&&textLayer->color.g==255&&textLayer->color.b==255)
    strcat(cmds,"W=");
  else
    strcat(cmds,"B=");
  strcat(cmds,textLayer->content);
  strcat(cmds,"|}");
  strcat(cmds,"\r\n");
  free(textLayer->font.Style);
  free(textLayer->font.FontName);
  free(textLayer->content);
  free(textLayer->baseLayer->layerName);
  free(textLayer->baseLayer);
  free(fName);
  free(textLayer);

  enqueue(TPCLqueue,cmds);

}
void HCreateLineTPCL(Queue*TPCLqueue,pageConfig *page,LineLayer *lineLayer)
{

}
void HCreateRectangleTPCL(Queue*TPCLqueue,pageConfig *page,RectangleLayer *rectLayer)
{

}
void HCreateEllipseTPCL(Queue*TPCLqueue,pageConfig *page,EllipseLayer *ellipseLayer)
{

}
void HCreateImageTPCL(Queue*TPCLqueue,pageConfig *page,ImageLayer *imageLayer)
{

}
void HCreateRfidTPCL(Queue*TPCLqueue,pageConfig *page,RfidLayer *rfidLayer)
{

}