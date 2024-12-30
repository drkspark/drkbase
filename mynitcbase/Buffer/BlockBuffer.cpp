#include "BlockBuffer.h"

#include <cstdlib>
#include <cstring>

BlockBuffer::BlockBuffer(int blockNum) {
  this->blockNum = blockNum;
}

int BlockBuffer::getBlockNum() {
  return this->blockNum;
}

// calls the parent class constructor
RecBuffer::RecBuffer(int blockNum) : BlockBuffer::BlockBuffer(blockNum) {}

int BlockBuffer::getHeader(struct HeadInfo *head) {
  unsigned char buffer[BLOCK_SIZE];

  // Reading the block into the buffer
  int res = Disk::readBlock(buffer, this->getBlockNum());
  if (res != ReturnTypes::SUCCESS) {
      return static_cast<ReturnTypes>(res);
  }

  // populate the numEntries, numAttrs and numSlots fields in *head 
  memcpy(&head->numSlots, buffer + 24, 4);
  memcpy(&head->numEntries, buffer + 16, 4);
  memcpy(&head->numAttrs, buffer + 20, 4);
  memcpy(&head->rblock, buffer + 12, 4);
  memcpy(&head->lblock, buffer + 8, 4);

  return SUCCESS;
}

int BlockBuffer::setHeader(struct HeadInfo *head) {
  unsigned char buffer[BLOCK_SIZE];

  // Reading the block into the buffer
  int res = Disk::readBlock(buffer, this->getBlockNum());
  if (res != ReturnTypes::SUCCESS) {
      return static_cast<ReturnTypes>(res);
  }

  // Change the old values with the new ones
  memcpy(buffer + 24, &head->numSlots, 4);
  memcpy(buffer + 16, &head->numEntries, 4);
  memcpy(buffer + 20, &head->numAttrs, 4);
  memcpy(buffer + 12, &head->rblock, 4);
  memcpy(buffer + 8,&head->lblock, 4);

  // Writing the block back to the memory
  res = Disk::writeBlock(buffer, this->getBlockNum());
  if (res != ReturnTypes::SUCCESS) {
      return static_cast<ReturnTypes>(res);
  }

  return SUCCESS;
}

// load the record at slotNum into the argument pointer
int RecBuffer::getRecord(union Attribute *rec, int slotNum) {
  struct HeadInfo head;

  // get the header using this.getHeader() function
  this->getHeader(&head);

  int attrCount = head.numAttrs;
  int slotCount = head.numSlots;

  // read the block at this.blockNum into a buffer
  unsigned char buffer[BLOCK_SIZE];

  // Reading the block into the buffer
  int res = Disk::readBlock(buffer, this->getBlockNum());
  if (res != ReturnTypes::SUCCESS) {
      return static_cast<ReturnTypes>(res);
  }

  /* record at slotNum will be at offset HEADER_SIZE + slotMapSize + (recordSize * slotNum)
     - each record will have size attrCount * ATTR_SIZE
     - slotMap will be of size slotCount
  */
  int recordSize = attrCount * ATTR_SIZE;

                              /* calculate buffer + offset */
  unsigned char *slotPointer = buffer + HEADER_SIZE + SLOTMAP_SIZE_RELCAT_ATTRCAT  + (recordSize * slotNum) ;

  // load the record into the rec data structure
  memcpy(rec, slotPointer, recordSize);

  return SUCCESS;
}

// Write the record at slotNum from the argument pointer
int RecBuffer::setRecord(union Attribute *rec, int slotNum) {
  struct HeadInfo head;

  // get the header using this.getHeader() function
  this->getHeader(&head);

  int attrCount = head.numAttrs;
  int slotCount = head.numSlots;

  // read the block at this.blockNum into a buffer
  unsigned char buffer[BLOCK_SIZE];

  // Reading the block into the buffer
  int res = Disk::readBlock(buffer, this->getBlockNum());
  if (res != ReturnTypes::SUCCESS) {
      return static_cast<ReturnTypes>(res);
  }

  /* record at slotNum will be at offset HEADER_SIZE + slotMapSize + (recordSize * slotNum)
     - each record will have size attrCount * ATTR_SIZE
     - slotMap will be of size slotCount
  */
  int recordSize = attrCount * ATTR_SIZE;

                              /* calculate buffer + offset */
  unsigned char *slotPointer = buffer + HEADER_SIZE + SLOTMAP_SIZE_RELCAT_ATTRCAT  + (recordSize * slotNum) ;

  // write rec data structure into the buffer
  memcpy(slotPointer, rec, recordSize);
  
  // Writing the block into the memory
  res = Disk::writeBlock(buffer, this->getBlockNum());
  if (res != ReturnTypes::SUCCESS) {
      return static_cast<ReturnTypes>(res);
  }

  return SUCCESS;
}