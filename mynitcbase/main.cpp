#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
// #include "FrontendInterface/FrontendInterface.h"
#include "define/constants.h"
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
  /* Initialize the Run Copy of Disk */
  Disk disk_run;
  // StaticBuffer buffer;
  // OpenRelTable cache;

  // return FrontendInterface::handleFrontend(argc, argv);

  
  unsigned char buffer[BLOCK_SIZE];
  Disk::readBlock(buffer, 7000);
  char message[] = "hello";

  memcpy(buffer + 20, message, 6);
  Disk::writeBlock(buffer, 7000);

  unsigned char buffer2[BLOCK_SIZE];
  char messasge2[6];
  Disk::readBlock(buffer2,  7000);
  memcpy(messasge2, buffer2 + 20, 6);
  std::cout << messasge2 << "\n";



  return 0; 
}