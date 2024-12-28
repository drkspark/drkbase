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
  char message2[6];
  Disk::readBlock(buffer2, 7000);
  memcpy(message2, buffer2 + 20, 6);
  std::cout << message2 << '\n';

  // std::cout << sizeof(BlockType::BMAP) << " " << sizeof(BlockType::REC) << " " << sizeof(BlockType::UNUSED_BLK) << " "<< sizeof(BlockType::IND_INTERNAL) << " "<< sizeof(BlockType::IND_LEAF) << '\n';
  // Since Block Allocation Map is made up of first 4 
  
  unsigned char block_allocation_mp[BLOCK_SIZE]; // We are reading the 1st block of the Block Allocation Map
  Disk::readBlock(block_allocation_mp, 0);
  unsigned char first_byte[6];
  memcpy(first_byte, block_allocation_mp, 6);
  /**
    Since the Block Allocation Map stores the BlockType we should cast each byte to BlockType to see which type of block is being stored
    It also works when you convert it to int since the BlockType and int are of same size
   */
  std::cout << "1st byte contains: " << static_cast<int>(first_byte[0])<<'\n';
  std::cout << "1st byte contains: " << static_cast<BlockType>(first_byte[0])<<'\n';
 

  std::cout << "2nd byte contains: " << static_cast<BlockType>(first_byte[1])<<'\n';
  std::cout << "3rd byte contains: " << static_cast<BlockType>(first_byte[2])<<'\n';
  std::cout << "4th byte contains: " << static_cast<BlockType>(first_byte[3])<<'\n';
  std::cout << "5th byte contains: " << static_cast<BlockType>(first_byte[4])<<'\n';
  std::cout << "6th byte contains: " << static_cast<BlockType>(first_byte[5])<<'\n';

  return 0; 
}