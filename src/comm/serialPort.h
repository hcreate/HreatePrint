#ifndef SERIALPORT_H
#define SERIALPORT_H

#include<stdio.h>
void openPort();
void writePort(char *data);
void *readPort();
void closePort();
#endif