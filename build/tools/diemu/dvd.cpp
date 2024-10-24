#include "dvd.h"

u16 keyTbl[16] = {
    3, 48, 32512, 28673, 6,
    69, 32256, 24579, 12, 192,
    31744, 16391, 24, 384, 30720,
    15
};

struct DiskId
{
    char gamecode[4];
    char makercode[2];
    u8 audioBuffer;
    u8 audioBufferSize;
    u8 padding1[18];
    u32 magic;
};

struct DiscBca
{
    u8 optInfo[52];
    u8 manufacturer[2];
    u8 recorderDevice[2];
    u8 bcaSerial;
    u8 discDate[2];
    u8 discTime[2];
    u8 discNumber[3];
    u8 key[8];
    u8 id[4];
    PsnRegion psn[6];
};

struct PsnRegion
{
    u32 start;
    u32 end;
};

struct DataFrame
{
    u32 id;
    u16 ied;
    u8 userdata[2048];
    u8 cpr_mai[6];
    u32 edc;
};

struct PhysicalFormatInfo
{
    u8 reversed[6];
    u8 discMagic;
    u8 discSizeMinTransferRate;
    u8 discStructure;
    u8 recordedDensity;
    DataAreaAllocation m_dataAreaAllocation;
    u8 reversed2[2000];
    u8 reversed3[6];
};

struct DiscManufacturingInfo
{
    u8 reversed[6];
    u8 secret1[6];
    u8 randomNumber2[6];
    u8 secret2[6];
    u8 randomNumber3[6];
    char mediaId[19];
    u8 randomNumber4[6];
    u8 bookTypePartVersion;
    u8 discSizeMinReadoutRate;
    u8 discStructure;
    u8 recordedDensity;
    DataAreaAllocation m_dataAreaAllocation;
    u8 bcaDescriptor;
    u8 reversed2[1967];
    u8 reversed3[6];
};

struct DataAreaAllocation
{
    u8 reversed;
    u16 startSector;
    u8 reversed2;
    u16 endSector;
    u8 reversed3[3];
};
