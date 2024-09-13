#include<stdio.h>
#include"driver/esc.h"
#include"driver/tspl.h"
#include"driver/cpcl.h"
#include"driver/zpl.h"
#include"driver/epl.h"
#include"driver/dpl.h"
#include"comm/comm.h"
#include"comm/hcreate.h"
#include"comm/algorithm.h"
#include"comm/HCPrase.h"

int main(int argc, char const *argv[])
{

    char *p="test.tag";
    // printHC(p);//解析tag文件
    // printESC(p);
    // printTSPL(p);
    // printCOMM(p);
    // printCPCL(p);
   // printZPL(p);
   // printEPL(p);
      printDPL(p);
    return 0;
}
