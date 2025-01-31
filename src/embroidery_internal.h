#ifndef LIBEMBROIDERY_INTERNAL_HEADER__
#define LIBEMBROIDERY_INTERNAL_HEADER__

#include "embroidery.h"

/**
 * \file embroidery_internal.h
 */

/* For FILE * */
#include <stdio.h>

/**
 * Type of sector
 */
#define CompoundFileSector_MaxRegSector 0xFFFFFFFA
#define CompoundFileSector_DIFAT_Sector 0xFFFFFFFC
#define CompoundFileSector_FAT_Sector   0xFFFFFFFD
#define CompoundFileSector_EndOfChain   0xFFFFFFFE
#define CompoundFileSector_FreeSector   0xFFFFFFFF

/**
 * Type of directory object
 */
#define ObjectTypeUnknown   0x00 /*!< Probably unallocated    */
#define ObjectTypeStorage   0x01 /*!< a directory type object */
#define ObjectTypeStream    0x02 /*!< a file type object      */
#define ObjectTypeRootEntry 0x05 /*!< the root entry          */

/**
 * Special values for Stream Identifiers
 */
#define CompoundFileStreamId_MaxRegularStreamId 0xFFFFFFFA /*!< All real stream Ids are less than this */
#define CompoundFileStreamId_NoStream           0xFFFFFFFF /*!< There is no valid stream Id            */

#define ELEMENT_XML              0
#define ELEMENT_A                1
#define ELEMENT_ANIMATE          2
#define ELEMENT_ANIMATECOLOR     3
#define ELEMENT_ANIMATEMOTION    4
#define ELEMENT_ANIMATETRANSFORM 5
#define ELEMENT_ANIMATION        6
#define ELEMENT_AUDIO            7
#define ELEMENT_CIRCLE           8
#define ELEMENT_DEFS             9
#define ELEMENT_DESC            10
#define ELEMENT_DISCARD         11
#define ELEMENT_ELLIPSE         12
#define ELEMENT_FONT            13
#define ELEMENT_FONT_FACE       14
#define ELEMENT_FONT_FACE_SRC   15
#define ELEMENT_FONT_FACE_URI   16
#define ELEMENT_FOREIGN_OBJECT  17
#define ELEMENT_G               18
#define ELEMENT_GLYPH           19
#define ELEMENT_HANDLER         20
#define ELEMENT_HKERN           21
#define ELEMENT_IMAGE           22
#define ELEMENT_LINE            23
#define ELEMENT_LINEAR_GRADIENT 24
#define ELEMENT_LISTENER        25
#define ELEMENT_METADATA        26
#define ELEMENT_MISSING_GLYPH   27
#define ELEMENT_MPATH           28
#define ELEMENT_PATH            29
#define ELEMENT_POLYGON         30
#define ELEMENT_POLYLINE        31
#define ELEMENT_PREFETCH        32
#define ELEMENT_RADIAL_GRADIENT 33
#define ELEMENT_RECT            34
#define ELEMENT_SCRIPT          35
#define ELEMENT_SET             36
#define ELEMENT_SOLID_COLOR     37
#define ELEMENT_STOP            38
#define ELEMENT_SVG             39
#define ELEMENT_SWITCH          40
#define ELEMENT_TBREAK          41
#define ELEMENT_TEXT            42
#define ELEMENT_TEXT_AREA       43
#define ELEMENT_TITLE           44
#define ELEMENT_TSPAN           45
#define ELEMENT_USE             46
#define ELEMENT_VIDEO           47

/* INTERNAL DEFINES */
#define RED_TERM_COLOR      "\x1B[0;31m"
#define GREEN_TERM_COLOR    "\x1B[0;32m"
#define YELLOW_TERM_COLOR   "\x1B[1;33m"
#define RESET_TERM_COLOR       "\033[0m"

#define HOOP_126X110                  0
#define HOOP_110X110                  1
#define HOOP_50X50                    2
#define HOOP_140X200                  3
#define HOOP_230X200                  4

#define EMB_MIN(A, B) (((A) < (B)) ? (A) : (B))
#define EMB_MAX(A, B) (((A) > (B)) ? (A) : (B))

/* Libembroidery's handling of integer types.
 */
#define EMB_BIG_ENDIAN                          0
#define EMB_LITTLE_ENDIAN                       1

#define ENDIAN_HOST                             EMB_LITTLE_ENDIAN

#define EMB_INT16_BIG                           2
#define EMB_INT16_LITTLE                        3
#define EMB_INT32_BIG                           4
#define EMB_INT32_LITTLE                        5

#define PES0001         0
#define PES0020         1
#define PES0022         2
#define PES0030         3
#define PES0040         4
#define PES0050         5
#define PES0055         6
#define PES0056         7
#define PES0060         8
#define PES0070         9
#define PES0080        10
#define PES0090        11
#define PES0100        12
#define N_PES_VERSIONS 13

/* DXF Version Identifiers */
#define DXF_VERSION_R10 "AC1006"
#define DXF_VERSION_R11 "AC1009"
#define DXF_VERSION_R12 "AC1009"
#define DXF_VERSION_R13 "AC1012"
#define DXF_VERSION_R14 "AC1014"
#define DXF_VERSION_R15 "AC1015"
#define DXF_VERSION_R18 "AC1018"
#define DXF_VERSION_R21 "AC1021"
#define DXF_VERSION_R24 "AC1024"
#define DXF_VERSION_R27 "AC1027"

#define DXF_VERSION_2000 "AC1015"
#define DXF_VERSION_2002 "AC1015"
#define DXF_VERSION_2004 "AC1018"
#define DXF_VERSION_2006 "AC1018"
#define DXF_VERSION_2007 "AC1021"
#define DXF_VERSION_2009 "AC1021"
#define DXF_VERSION_2010 "AC1024"
#define DXF_VERSION_2013 "AC1027"

#define SVG_CREATOR_NULL              0
#define SVG_CREATOR_EMBROIDERMODDER   1
#define SVG_CREATOR_ILLUSTRATOR       2
#define SVG_CREATOR_INKSCAPE          3

#define SVG_EXPECT_NULL               0
#define SVG_EXPECT_ELEMENT            1
#define SVG_EXPECT_ATTRIBUTE          2
#define SVG_EXPECT_VALUE              3

/*  SVG_TYPES
 *  ---------
 */
#define SVG_NULL                      0
#define SVG_ELEMENT                   1
#define SVG_PROPERTY                  2
#define SVG_MEDIA_PROPERTY            3
#define SVG_ATTRIBUTE                 4
#define SVG_CATCH_ALL                 5

/* path flag codes */
#define LINETO             0
#define MOVETO             1
#define BULGETOCONTROL     2
#define BULGETOEND         4
#define ELLIPSETORAD       8
#define ELLIPSETOEND      16
#define CUBICTOCONTROL1   32
#define CUBICTOCONTROL2   64
#define CUBICTOEND       128
#define QUADTOCONTROL    256
#define QUADTOEND        512

/* STRUCTS
 *******************************************************************/

/* double-indirection file allocation table references */

/**
 * @brief 
 * 
 */
typedef struct _bcf_file_difat
{
    unsigned int fatSectorCount;
    unsigned int fatSectorEntries[109];
    unsigned int sectorSize;
} bcf_file_difat;

/**
 * @brief 
 * 
 */
typedef struct _bcf_file_fat
{
    int          fatEntryCount;
    unsigned int fatEntries[255]; /* maybe make this dynamic */
    unsigned int numberOfEntriesInFatSector;
} bcf_file_fat;

/**
 * @brief 
 * 
 */
typedef struct _bcf_directory_entry
{
    char                         directoryEntryName[32];
    unsigned short               directoryEntryNameLength;
    unsigned char                objectType;
    unsigned char                colorFlag;
    unsigned int                 leftSiblingId;
    unsigned int                 rightSiblingId;
    unsigned int                 childId;
    unsigned char                CLSID[16];
    unsigned int                 stateBits;
    EmbTime                      creationTime;
    EmbTime                      modifiedTime;
    unsigned int                 startingSectorLocation;
    unsigned long                streamSize; /* should be long long but in our case we shouldn't need it, and hard to support on c89 cross platform */
    unsigned int                 streamSizeHigh; /* store the high int of streamsize */
    struct _bcf_directory_entry* next;
} bcf_directory_entry;

/**
 * @brief 
 * 
 * \todo possibly add a directory tree in the future.
 */
typedef struct _bcf_directory
{
    bcf_directory_entry* dirEntries;
    unsigned int         maxNumberOfDirectoryEntries;
} bcf_directory;

/**
 * @brief 
 * \todo CLSID should be a separate type.
 */
typedef struct _bcf_file_header
{
    unsigned char  signature[8];
    unsigned char  CLSID[16]; 
    unsigned short minorVersion;
    unsigned short majorVersion;
    unsigned short byteOrder;
    unsigned short sectorShift;
    unsigned short miniSectorShift;
    unsigned short reserved1;
    unsigned int   reserved2;
    unsigned int   numberOfDirectorySectors;
    unsigned int   numberOfFATSectors;
    unsigned int   firstDirectorySectorLocation;
    unsigned int   transactionSignatureNumber;
    unsigned int   miniStreamCutoffSize;
    unsigned int   firstMiniFATSectorLocation;
    unsigned int   numberOfMiniFatSectors;
    unsigned int   firstDifatSectorLocation;
    unsigned int   numberOfDifatSectors;
} bcf_file_header;

/**
 * @brief 
 * 
 */
typedef struct _bcf_file
{
    bcf_file_header header;   /*! The header for the CompoundFile */
    bcf_file_difat* difat;    /*! The "Double Indirect FAT" for the CompoundFile */
    bcf_file_fat* fat;        /*! The File Allocation Table for the Compound File */
    bcf_directory* directory; /*! The directory for the CompoundFile */
} bcf_file;

/**
 * @brief 
 * 
 */
typedef struct _vp3Hoop
{
    int right;
    int bottom;
    int left;
    int top;
    int threadLength;
    char unknown2;
    unsigned char numberOfColors;
    unsigned short unknown3;
    int unknown4;
    int numberOfBytesRemaining;

    int xOffset;
    int yOffset;

    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;

    /* Centered hoop dimensions */
    int right2;
    int left2;
    int bottom2;
    int top2;

    int width;
    int height;
} vp3Hoop;

/**
 * @brief 
 * 
 */
typedef struct ThredHeader_     /* thred file header */
{
    unsigned int sigVersion;    /* signature and version */
    unsigned int length;        /* length of ThredHeader + length of stitch data */
    unsigned short numStiches;  /* number of stitches */
    unsigned short hoopSize;    /* size of hoop */
    unsigned short reserved[7]; /* reserved for expansion */
} ThredHeader;

/**
 * @brief 
 * 
 */
typedef struct ThredExtension_  /* thred v1.0 file header extension */
{
    float hoopX;                /* hoop size x dimension in 1/6 mm units */
    float hoopY;                /* hoop size y dimension in 1/6 mm units */
    float stitchGranularity;    /* stitches per millimeter--not implemented */
    char creatorName[50];       /* name of the file creator */
    char modifierName[50];      /* name of last file modifier */
    char auxFormat;             /* auxiliary file format, 0=PCS,1=DST,2=PES */
    char reserved[31];          /* reserved for expansion */
} ThredExtension;

/**
 * @brief 
 * 
 */
typedef struct SubDescriptor_
{
    int someNum;      /*! \todo better variable naming */
    int someInt;      /*! \todo better variable naming */
    int someOtherInt; /*! \todo better variable naming */
    char* colorCode;
    char* colorName;
} SubDescriptor;

/**
 * @brief 
 * 
 */
typedef struct StxThread_
{
    char* colorCode;
    char* colorName;
    char* sectionName;
    SubDescriptor* subDescriptors;
    EmbColor stxColor;
} StxThread;

/**
 * @brief 
 * 
 */
typedef struct VipHeader_ {
    int magicCode;
    int numberOfStitches;
    int numberOfColors;
    short postitiveXHoopSize;
    short postitiveYHoopSize;
    short negativeXHoopSize;
    short negativeYHoopSize;
    int attributeOffset;
    int xOffset;
    int yOffset;
    unsigned char stringVal[8];
    short unknown;
    int colorLength;
} VipHeader;

/**
 * @brief 
 * 
 */
typedef enum
{
    CSV_EXPECT_NULL,
    CSV_EXPECT_QUOTE1,
    CSV_EXPECT_QUOTE2,
    CSV_EXPECT_COMMA
} CSV_EXPECT;

/**
 * @brief 
 * 
 */
typedef enum
{
    CSV_MODE_NULL,
    CSV_MODE_COMMENT,
    CSV_MODE_VARIABLE,
    CSV_MODE_THREAD,
    CSV_MODE_STITCH
} CSV_MODE;

/**
 * @brief 
 * 
 */
typedef struct SvgAttribute_
{
    char* name;
    char* value;
} SvgAttribute;

/**
 * @brief 
 * 
 */
typedef struct Huffman {
    int default_value;
    int lengths[1000];
    int nlengths;
    int table[1000];
    int table_width;
    int ntable;
} huffman;

/**
 * @brief 
 * 
 */
typedef struct Compress {
    int bit_position;
    char *input_data;
    int input_length;
    int bits_total;
    int block_elements;
    huffman character_length_huffman;
    huffman character_huffman;
    huffman distance_huffman;
} compress;

/* Function Declarations
*****************************************************************************/
void huffman_build_table(huffman *h);
int *huffman_table_lookup(huffman *h, int byte_lookup, int *lengths);

int compress_get_bits(compress *c, int length);
int compress_pop(compress *c, int bit_count);
int compress_read_variable_length(compress *c);
void compress_load_character_length_huffman(compress *c);
void compress_load_character_huffman(compress *c);
void compress_load_distance_huffman(compress *c);
void compress_load_block(compress *c);
int compress_get_token(compress *c);
int compress_get_position(compress *c);

void readPecStitches(EmbPattern* pattern, FILE* file);
void writePecStitches(EmbPattern* pattern, FILE* file, const char* filename);

int decodeNewStitch(unsigned char value);

void pfaffEncode(FILE* file, int x, int y, int flags);
EmbReal pfaffDecode(unsigned char a1, unsigned char a2, unsigned char a3);

unsigned char mitEncodeStitch(EmbReal value);
int mitDecodeStitch(unsigned char value);

int encode_tajima_ternary(unsigned char b[3], int x, int y);
void decode_tajima_ternary(unsigned char b[3], int *x, int *y);

void encode_t01_record(unsigned char b[3], int x, int y, int flags);
int decode_t01_record(unsigned char b[3], int *x, int *y, int *flags);
void readPESHeaderV5(FILE* file, EmbPattern* pattern);
void readPESHeaderV6(FILE* file, EmbPattern* pattern);
void readPESHeaderV7(FILE* file, EmbPattern* pattern);
void readPESHeaderV8(FILE* file, EmbPattern* pattern);
void readPESHeaderV9(FILE* file, EmbPattern* pattern);
void readPESHeaderV10(FILE* file, EmbPattern* pattern);

void readDescriptions(FILE* file, EmbPattern* pattern);
void readHoopName(FILE* file, EmbPattern* pattern);
void readImageString(FILE* file, EmbPattern* pattern);
void readProgrammableFills(FILE* file, EmbPattern* pattern);
void readMotifPatterns(FILE* file, EmbPattern* pattern);
void readFeatherPatterns(FILE* file, EmbPattern* pattern);
void readThreads(FILE* file, EmbPattern* pattern);

void embInt_read(FILE* f, char *label, void *b, int mode);
void embInt_write(FILE* f, char *label, void *b, int mode);
int emb_readline(FILE* file, char *line, int maxLength);

int bcfFile_read(FILE* file, bcf_file* bcfFile);
FILE* GetFile(bcf_file* bcfFile, FILE* file, char* fileToFind);
void bcf_file_free(bcf_file* bcfFile);

void binaryReadString(FILE* file, char *buffer, int maxLength);
void binaryReadUnicodeString(FILE* file, char *buffer, const int stringLength);

int stringInArray(const char *s, const char **array);
void fpad(FILE *f, char c, int n);
char *copy_trim(char const *s);
char* emb_optOut(EmbReal num, char* str);

void write_24bit(FILE* file, int);
int check_header_present(FILE* file, int minimum_header_length);

unsigned short fread_uint16(FILE *file);
short fread_int16(FILE* f);
int fread_int32_be(FILE* f);
void safe_free(void *data);
void embInt_read(FILE* f, char *label, void *b, int mode);
 
void binaryWriteUIntBE(FILE* f, unsigned int data);
void binaryWriteUInt(FILE* f, unsigned int data);
void binaryWriteIntBE(FILE* f, int data);
void binaryWriteInt(FILE* f, int data);
void binaryWriteUShort(FILE* f, unsigned short data);
void binaryWriteUShortBE(FILE* f, unsigned short data);
void binaryWriteShort(FILE* f, short data);

bcf_file_difat* bcf_difat_create(FILE* file, unsigned int fatSectors, const unsigned int sectorSize);
unsigned int readFullSector(FILE* file, bcf_file_difat* bcfFile, unsigned int* numberOfDifatEntriesStillToRead);
unsigned int numberOfEntriesInDifatSector(bcf_file_difat* fat);
void bcf_file_difat_free(bcf_file_difat* difat);

unsigned int entriesInDifatSector(bcf_file_difat* fat);
bcf_file_fat* bcfFileFat_create(const unsigned int sectorSize);
void loadFatFromSector(bcf_file_fat* fat, FILE* file);
void bcf_file_fat_free(bcf_file_fat** fat);

bcf_directory_entry* CompoundFileDirectoryEntry(FILE* file);
bcf_directory* CompoundFileDirectory(const unsigned int maxNumberOfDirectoryEntries);
void readNextSector(FILE* file, bcf_directory* dir);
void bcf_directory_free(bcf_directory** dir);

bcf_file_header bcfFileHeader_read(FILE* file);
int bcfFileHeader_isValid(bcf_file_header header);

int hus_compress(char* input, int size, char* output, int *out_size);
int hus_decompress(char* input, int size, char* output, int *out_size);

int encode_tajima_ternary(unsigned char b[3], int x, int y);
void decode_tajima_ternary(unsigned char b[3], int *x, int *y);
void testTangentPoints(EmbCircle c, EmbVector p, EmbVector *t0, EmbVector *t1);
void printArcResults(EmbReal bulge, EmbArc arc,
                     EmbReal centerX,   EmbReal centerY,
                     EmbReal radius,    EmbReal diameter,
                     EmbReal chord,
                     EmbReal chordMidX, EmbReal chordMidY,
                     EmbReal sagitta,   EmbReal apothem,
                     EmbReal incAngle,  char   clockwise);
int create_test_file_1(const char* outf);
int create_test_file_2(const char* outf);
int create_test_file_3(const char* outf);
int testEmbCircle(void);
int testEmbCircle_2(void);
int testGeomArc(void);
int testThreadColor(void);
int testEmbFormat(void);

void embColor_read(FILE *f, EmbColor *c, int toRead);
void embColor_write(FILE *f, EmbColor c, int toWrite);

char read100(EmbPattern *pattern, FILE* file);
char write100(EmbPattern *pattern, FILE* file);
char read10o(EmbPattern *pattern, FILE* file);
char write10o(EmbPattern *pattern, FILE* file);
char readArt(EmbPattern *pattern, FILE* file);
char writeArt(EmbPattern *pattern, FILE* file);
char readBmc(EmbPattern *pattern, FILE* file);
char writeBmc(EmbPattern *pattern, FILE* file);
char readBro(EmbPattern *pattern, FILE* file);
char writeBro(EmbPattern *pattern, FILE* file);
char readCnd(EmbPattern *pattern, FILE* file);
char writeCnd(EmbPattern *pattern, FILE* file);
char readCol(EmbPattern *pattern, FILE* file);
char writeCol(EmbPattern *pattern, FILE* file);
char readCsd(EmbPattern *pattern, FILE* file);
char writeCsd(EmbPattern *pattern, FILE* file);
char readCsv(EmbPattern *pattern, FILE* file);
char writeCsv(EmbPattern *pattern, FILE* file);
char readDat(EmbPattern *pattern, FILE* file);
char writeDat(EmbPattern *pattern, FILE* file);
char readDem(EmbPattern *pattern, FILE* file);
char writeDem(EmbPattern *pattern, FILE* file);
char readDsb(EmbPattern *pattern, FILE* file);
char writeDsb(EmbPattern *pattern, FILE* file);
char readDst(EmbPattern *pattern, FILE* file);
char writeDst(EmbPattern *pattern, FILE* file);
char readDsz(EmbPattern *pattern, FILE* file);
char writeDsz(EmbPattern *pattern, FILE* file);
char readDxf(EmbPattern *pattern, FILE* file);
char writeDxf(EmbPattern *pattern, FILE* file);
char readEdr(EmbPattern *pattern, FILE* file);
char writeEdr(EmbPattern *pattern, FILE* file);
char readEmd(EmbPattern *pattern, FILE* file);
char writeEmd(EmbPattern *pattern, FILE* file);
char readExp(EmbPattern *pattern, FILE* file);
char writeExp(EmbPattern *pattern, FILE* file);
char readExy(EmbPattern *pattern, FILE* file);
char writeExy(EmbPattern *pattern, FILE* file);
char readEys(EmbPattern *pattern, FILE* file);
char writeEys(EmbPattern *pattern, FILE* file);
char readFxy(EmbPattern *pattern, FILE* file);
char writeFxy(EmbPattern *pattern, FILE* file);
char readGc(EmbPattern *pattern, FILE* file);
char writeGc(EmbPattern *pattern, FILE* file);
char readGnc(EmbPattern *pattern, FILE* file);
char writeGnc(EmbPattern *pattern, FILE* file);
char readGt(EmbPattern *pattern, FILE* file);
char writeGt(EmbPattern *pattern, FILE* file);
char readHus(EmbPattern *pattern, FILE* file);
char writeHus(EmbPattern *pattern, FILE* file);
char readInb(EmbPattern *pattern, FILE* file);
char writeInb(EmbPattern *pattern, FILE* file);
char readInf(EmbPattern *pattern, FILE* file);
char writeInf(EmbPattern *pattern, FILE* file);
char readJef(EmbPattern *pattern, FILE* file);
char writeJef(EmbPattern *pattern, FILE* file);
char readKsm(EmbPattern *pattern, FILE* file);
char writeKsm(EmbPattern *pattern, FILE* file);
char readMax(EmbPattern *pattern, FILE* file);
char writeMax(EmbPattern *pattern, FILE* file);
char readMit(EmbPattern *pattern, FILE* file);
char writeMit(EmbPattern *pattern, FILE* file);
char readNew(EmbPattern *pattern, FILE* file);
char writeNew(EmbPattern *pattern, FILE* file);
char readOfm(EmbPattern *pattern, FILE* file);
char writeOfm(EmbPattern *pattern, FILE* file);
char readPcd(EmbPattern *pattern, const char *fileName, FILE* file);
char writePcd(EmbPattern *pattern, FILE* file);
char readPcm(EmbPattern *pattern, FILE* file);
char writePcm(EmbPattern *pattern, FILE* file);
char readPcq(EmbPattern *pattern, const char *fileName, FILE* file);
char writePcq(EmbPattern *pattern, FILE* file);
char readPcs(EmbPattern *pattern, const char *fileName, FILE* file);
char writePcs(EmbPattern *pattern, FILE* file);
char readPec(EmbPattern *pattern, const char *fileName, FILE* file);
char writePec(EmbPattern *pattern, const char *fileName,  FILE* file);
char readPel(EmbPattern *pattern, FILE *file);
char writePel(EmbPattern *pattern, FILE *file);
char readPem(EmbPattern *pattern, FILE *file);
char writePem(EmbPattern *pattern, FILE *file);
char readPes(EmbPattern *pattern, const char *fileName, FILE* file);
char writePes(EmbPattern *pattern, const char *fileName, FILE* file);
char readPhb(EmbPattern *pattern, FILE* file);
char writePhb(EmbPattern *pattern, FILE *file);
char readPhc(EmbPattern *pattern, FILE* file);
char writePhc(EmbPattern *pattern, FILE *file);
char readPlt(EmbPattern *pattern, FILE* file);
char writePlt(EmbPattern *pattern, FILE* file);
char readRgb(EmbPattern *pattern, FILE* file);
char writeRgb(EmbPattern *pattern, FILE* file);
char readSew(EmbPattern *pattern, FILE* file);
char writeSew(EmbPattern *pattern, FILE* file);
char readShv(EmbPattern *pattern, FILE* file);
char writeShv(EmbPattern *pattern, FILE *file);
char readSst(EmbPattern *pattern, FILE* file);
char writeSst(EmbPattern *pattern, FILE *file);
char readStx(EmbPattern *pattern, FILE* file);
char writeStx(EmbPattern *pattern, FILE *file);
char readSvg(EmbPattern *pattern, FILE* file);
char writeSvg(EmbPattern *pattern, FILE* file);
char readT01(EmbPattern *pattern, FILE* file);
char writeT01(EmbPattern *pattern, FILE* file);
char readT09(EmbPattern *pattern, FILE* file);
char writeT09(EmbPattern *pattern, FILE* file);
char readTap(EmbPattern *pattern, FILE* file);
char writeTap(EmbPattern *pattern, FILE* file);
char readThr(EmbPattern *pattern, FILE* file);
char writeThr(EmbPattern *pattern, FILE* file);
char readTxt(EmbPattern *pattern, FILE* file);
char writeTxt(EmbPattern *pattern, FILE* file);
char readU00(EmbPattern *pattern, FILE* file);
char writeU00(EmbPattern *pattern, FILE *file);
char readU01(EmbPattern *pattern, FILE* file);
char writeU01(EmbPattern *pattern, FILE *file);
char readVip(EmbPattern *pattern, FILE* file);
char writeVip(EmbPattern *pattern, FILE* file);
char readVp3(EmbPattern *pattern, FILE* file);
char writeVp3(EmbPattern *pattern, FILE* file);
char readXxx(EmbPattern *pattern, FILE* file);
char writeXxx(EmbPattern *pattern, FILE* file);
char readZsk(EmbPattern *pattern, FILE* file);
char writeZsk(EmbPattern *pattern, FILE* file);

extern const char imageWithFrame[38][48];

#endif
