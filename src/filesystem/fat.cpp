#include "filesystem/fat.h"

using namespace cpos;
using namespace cpos::common;
using namespace cpos::filesystem;
using namespace cpos::drivers;

void printf(const char* str);
void printfHex(uint8_t key);

void cpos::filesystem::ReadBiosBlock(AdvancedTechnologyAttachment *hd, uint32_t partitionOffset){

    BiosParameterBlock32 bpb;
    hd->Read28(partitionOffset, (uint8_t*)&bpb, sizeof(BiosParameterBlock32));

    uint32_t fatStart = partitionOffset + bpb.reservedSectors;
    uint32_t fatSize = bpb.tableSize;

    uint32_t dataStart = fatStart + fatSize * bpb.fatCopies;

    uint32_t rootStart = dataStart + bpb.sectorPerCluster * (bpb.rootCluster - 2);

    DirectoryEntryFat32 dirent[16];
    hd->Read28(rootStart, (uint8_t*)&dirent[0], 16 * sizeof(DirectoryEntryFat32));

    for(int i = 0; i < 16; i++){
        if(dirent[i].name[0] == 0x00){
            break;
        }

        if((dirent[i].attributes & 0x0F) == 0x0F){ //非法制
            continue;
        }

        if((dirent[i].attributes & 0x10) == 0x10){ //目录
            continue;
        }

        //打印文件名
        char* foo = "       \n";
        for(int j = 0; j < 8; j++){
            foo[j] = dirent[i].name[j];
        }
        printf(foo);

        uint32_t fileCluster = ((uint32_t)dirent[i].firstClusterHi) << 16 | (uint32_t)(dirent[i].firstClusterLow);
        uint32_t fileSector = dataStart + bpb.sectorPerCluster * (fileCluster - 2);

        uint8_t buffer[512];
        hd->Read28(fileSector, buffer, 512);

        buffer[dirent[i].size] = '\0';
        printf((char*)buffer);
    }

    
}