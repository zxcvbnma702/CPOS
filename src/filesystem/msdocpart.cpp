#include "filesystem/msdocpart.h"
#include "filesystem/fat.h"

using namespace cpos;
using namespace cpos::common;
using namespace cpos::filesystem;
using namespace cpos::drivers;


void printf(const char* str);
void printfHex(uint8_t key);

void MSDOSPartitionTable::ReadPartitions(AdvancedTechnologyAttachment *hd){
    MasterBootRecord mbr;
    hd->Read28(0, (uint8_t*)&mbr, sizeof(MasterBootRecord));

    if(mbr.magicnumber != 0xAA55){
        printf("illegal MBR");
        return;
    }

    for(int i = 0; i < 4; i++){

        if(mbr.primaryPartition[i].partition_id == 0){
            return;
        }

        printf(" Partition: ");
        printfHex(i);
        
        if(mbr.primaryPartition[i].bootable == 0x80){
            printf(" bootable ");
        }else{
            printf(" not bootable. Type");
        }

        printfHex(mbr.primaryPartition[i].partition_id);
        printf("\n");

        ReadBiosBlock(hd, mbr.primaryPartition[i].start_lba);
    
    }
}