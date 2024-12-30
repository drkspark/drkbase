#include "Buffer/BlockBuffer.h"
#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
// #include "FrontendInterface/FrontendInterface.h"
#include "define/constants.h"
#include <cstring>
#include <iostream>


void printHeaderInfo(const HeadInfo& header) {
  std::cout << "Block Type: " << header.blockType << "\n"
            << "#Attributes : " << header.numAttrs << "\n"
            << "#Entries: " << header.numEntries << "\n"
            << "#Slots : " << header.numSlots << "\n"
            << "Right Block: " << header.rblock << '\n'
            << "Left Block: " << header.lblock << '\n';
}



int main(int argc, char *argv[]) {
  /* Initialize the Run Copy of Disk */
  Disk disk_run;
  
  // creating objects for relation and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // Loading the headers of both the blocks into relCatHeader and attrCatHeader
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);
  
  // std::cout << "Printing Relation Catalog Header:: \n";
  // printHeaderInfo(relCatHeader);
  
  // std::cout << "Printing Attribute Catalog Header:: \n";
  // printHeaderInfo(attrCatHeader);

  /* i = 0 to total relation count */
  for (int i = 0; i < relCatHeader.numEntries; ++i) {

    Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

    /* j = 0 to number of entries in the attribute catalog */
    for (int j = 0; j < attrCatHeader.numEntries; ++j) {

      // declare attrCatRecord and load the attribute catalog entry into it
      Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
      attrCatBuffer.getRecord(attrCatRecord, j);
      
      // std::cout << "Relation name from Attribute CAT:: " << attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal << '\n';
      // std::cout << "Relation name form Rel CAT:: " << relCatRecord[RELCAT_REL_NAME_INDEX].sVal << '\n';

      /* attribute catalog entry corresponds to the current relation */
      if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal, relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0) {
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        printf("  %s: %s\n", /* get the attribute name */ attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
      }
    }
    printf("\n");
  }

  return 0; 
}