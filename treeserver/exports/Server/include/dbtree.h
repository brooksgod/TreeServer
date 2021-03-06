/***************************************************************************
*   BTREE.H   2.0
*
*  Copyright: East-Union Computer Service Co., Ltd.
*             1991,92,93
*             Shanghai Institute Of Railway Technology 1994-1996
*             CRSC 1997
*  Author:    Yong Hu,  rewrite for special use by Xilong Pei
****************************************************************************/

#ifndef  __DBTREE_H_
#define  __DBTREE_H_

#include <windows.h>
#include <process.h>
#include <stdio.h>

#ifndef BtreeHeadFileIncluded

#define __d_FIELD_
#define FIELDNAMELEN    32
typedef struct {
	unsigned char field[FIELDNAMELEN];
	unsigned char fieldtype;
	unsigned char fieldlen;
	unsigned char fielddec;
	unsigned char *fieldstart;
} d_FIELD;

#define BTREE_BUFSIZE			(256-24)
// 1000		// data size in a node
#define BTREE_BUFMAXLEN                 256
// 1024		// node size
#define BTREE_RUBBISHBLOCKNO            9

#define bIndexExtention         ".NDX"
/*in VC, this will call error, this H file use the define,but others not, it is defined in stdio.h
#ifndef FILENAME_MAX
#define FILENAME_MAX            256
#endif
*/
typedef enum tagInputDbfType {
	BTREE_FOR_OPENDBF = 0x01,               // Dbf in dFILE
	BTREE_FOR_CLOSEDBF = 0x02,              // Dbf in FileName
	BTREE_FOR_ITSELF = 0x04,                // operate the btree only
	BTREE_FOR_OBJ = 0x11                    // Dbf as object
} InputDbfType;
#define FOROBJMASK      0x10


#define BtreeYES                        '1'     /* in memory */
#define BtreeNO                         '0'     /* not in memory */
#define BtreeDATA                       'D'     /* data */

#define BtreeUdef			'U'
#define MAX_KEYFIELD_NUM                5

#define DEF_BREAKPOS			0.75
#define UPPER_BREAKPOS			0.92
#define CORP_KEYPER			0.5

#define BTREE_MAX_DEGREE                7
#define MAX_KEYEXPR_LEN			128
#define BTREE_HEADBUF_SIZE              1024
#define BTREE_MAX_USETIMES              32700
#define BTREE_MAX_KEYLEN                256
#define BTREE_VER_MESSAGE               "GDC_98\x0D\x0A\x1A"
#define BTREE_BLANKKEY			" "
typedef struct  tagbTREE {
	  char nodeFlag;                        // wether it has son node in memory
	  char fatherPtrFlag;                   // father node is in memory?
	  char nextPtrFlag;                     // next node is in memory?
	  char lastPtrFlag;                     // last node is in memory?
	  long nodeNo;                 		// node no
	  short  keyNum;                        // key number
	  short  prefixLen;                     // keyword prefilx length
	  struct tagbTREE *fatherPtr;           // father pointer
	  struct tagbTREE *nextPtr;             // next pointer
	  struct tagbTREE *lastPtr;             // last pointer
	  unsigned char keyBuf[BTREE_BUFSIZE];  // key contener
} bTREE;
/*
#if sizeof(bTREE) != BTREE_BUFMAXLEN
	#error bTREE define error
#endif
*/

// the following is define of the pointer position in btree
#define BTREE_FATHERNODE_POSITION               12
#define BTREE_LASTNODE_POSITION                 20
#define BTREE_NEXTNODE_POSITION                 16

typedef struct tagbTAB {
	  short   nodeUseTimes;                   // node use time
	  bTREE  *nodeAdress;                   // node address
	  char    nodeWrFlag;                      // node has been writen into file
	  char    nodeNwFlag;                      // node hasn't been writen into file
} bTAB;

typedef short (*OBJINITPFD)(void);
typedef short (*OBJCONPFD)(void *dest, long int *item, short length);
// success return length, fail -1, end 0
typedef short (*OBJDECONPFD)(void *dest, long int *item, short length);
typedef short (*OBJCOMPPFD)(char *s1, char *s2, short maxlen);

typedef struct tagObjFuns {                   // object deal function
		short       ObjLen;           // object length
		OBJINITPFD  ObjInitFun;
		OBJCONPFD   ObjConFun;
		OBJDECONPFD ObjDeconFun;
		OBJCOMPPFD  ObjCompFun;
} bOBJDEALFUN;

typedef union tagObject {
	d_FIELD  keyName[MAX_KEYFIELD_NUM];    // key sequence name
	bOBJDEALFUN  ObjDealFuns;
} bOBJECT;

typedef struct tagbHEAD {
	  char VerMessage[10];                  // "NDX 1.00\0\1A"
	  char nodeMnFlag;                      // next level node manage flag
	  char type;                            // ndx type
	  unsigned char keyDbfName[FILENAME_MAX];         // dbf name
	  bOBJECT bObject;                      // index object
	  unsigned short keyFieldNum;           	// key number
	  unsigned short KeyFldid[MAX_KEYFIELD_NUM];	// key field id
	  short keyLen;                         // key length
	  short keyStoreLen;                    // key length + 5
	  short key4Len;                        // key length + 4
	  short keyMaxCount;                    // max of keys in one node
	  short nodeBreakPos;                   // node break position
	  short upperKeyNum;			// upper key number
	  short nodeCurTab;                     // active(in memory) node number
	  short ndxFp;                          // handle of ndx
	  void  *dbfPtr;                        // dbf dFILE handle
	  bTREE *ptr;                           // btree handle
	  long int nodeMaxNo;                   // node number

	  long int keyCount;                    // record number
	  bTREE *CurNodePtr;                    // current node pointer
	  short nodeCurPos;                     // position in current node

	  bTAB *active_b_tab;                   // active(in memory) node table
	  long  rubbishBlockNum;                 // rubbish block number
	  long  rubbishBlockNo[BTREE_RUBBISHBLOCKNO+1];    // rubbish block recorder

	  short nodeSize;
	  short nodeBufSize;

	  short nodeCorpNum;			 //when the node has less
						 //this num of key, it will
						 //be corporated

#ifdef WSToMT
	  CRITICAL_SECTION 	dCriticalSection;
	  char			inCriticalSection;
	  struct tagbHEAD      *pbh;
	  char    		szKeyBuf[BTREE_MAX_KEYLEN];
	  long			recNo;
	  unsigned long		timeStamp;
#endif

} bHEAD;

/*
#if sizeof(bHEAD) > BTREE_HEADBUF_SIZE
	#error bHEAD typedef error
#endif
*/

typedef struct tagBtreeOpenMan {
	char  szIndexName[FILENAME_MAX];
	bHEAD *bh;
	long  count;
} BTREE_OPEN_MAN;
#ifdef WSToMT
#define _BTREEFILENUM_   60
#else
#define _BTREEFILENUM_   20
#endif

extern int NODESIZE;
#ifdef WSToMT
extern WSToMT short BtreeErr;
extern WSToMT char  _BtreeKeyBuf[BTREE_MAX_KEYLEN];
extern CRITICAL_SECTION  btreeIndexCE;
extern short _BtreeOpenNum_;
#else
extern short BtreeErr;
extern char  _BtreeKeyBuf[BTREE_MAX_KEYLEN];
#endif

#endif	//BtreeHeadFileIncluded


#ifdef __DBTREE_C_
_declspec(dllexport) short BtNodeNumSet( short NodeNum );
    /*
    */
_declspec(dllexport) int writeBtreeData(bHEAD *b_head, char *keyBuf, char *data, int dataLen);
_declspec(dllexport) char * readBtreeData(bHEAD *b_head, char *keyBuf, char *data, int dataLen);
_declspec(dllexport) int freeBtreeData(bHEAD *b_head, char *keyBuf);
_declspec(dllexport) int dbTreeBuild(char *ndxName, short keyLen, short nodeSize);
	/* build the index btree
	*/
_declspec(dllexport) int dbTreeClose( bHEAD *b_head );
	/*
	*/
_declspec(dllexport) short dbTreeDispose(bHEAD *b_head);
	/*
	*/
_declspec(dllexport) char * dbTreeGetKeyContent( bHEAD *b_head );
	/*
	*/
_declspec(dllexport) short  dbTreeRecIns( bHEAD *b_head, char *keyContent, long int recNo);
	/*
	*/
_declspec(dllexport) long int dbTreeRecDel(bHEAD *b_head, char *keyContent, long int recNo);
	/*
	* success: return the number deleted, fail return LONG_MIN
	* to delete all the keyword as keyContent pass recNo with LONG_MIN
	*/
_declspec(dllexport) long int dbTreeSeek( bHEAD *b_head, char *keyContent );

	/*
	*/
_declspec(dllexport) long int dbTreeSkip( bHEAD *b_head, long int count );
	/* Return:
	*      Success: the record no
	*      Fail:    0 (error no in BtreeErr), negative of the record
	*               no(cannot skip so much steps, skip to the limit)
	*/
_declspec(dllexport) long int dbTreeGoTop( bHEAD *b_head );
	/*
	*/
_declspec(dllexport) long int dbTreeGoBottom( bHEAD *b_head );
	/*
	*/
_declspec(dllexport) short IndexEof( bHEAD *b_head );
	/*
	*/
_declspec(dllexport) bHEAD * dbTreeOpen(char *ndxName);
	/*
	*/
_declspec(dllexport) bHEAD * dbTreeAwake(char *ndxName);
_declspec(dllexport) int dbTreeRelease( bHEAD *bh );
_declspec(dllexport) int dbTreeSleep(bHEAD *bh);
_declspec(dllexport) long getBtreeDataLen(bHEAD *b_head, char *keyBuf);
_declspec(dllexport) long getBtreeError( void );
_declspec(dllexport) long getBtreeLastReadSize( void );
_declspec(dllexport) void freeBtreeMem(char *s);
_declspec(dllexport) int dbTreeFlush( bHEAD *b_head );


#else
_declspec(dllimport) short BtNodeNumSet( short NodeNum );
    /*
    */
_declspec(dllimport) int writeBtreeData(bHEAD *b_head, char *keyBuf, char *data, int dataLen);
_declspec(dllimport) char * readBtreeData(bHEAD *b_head, char *keyBuf, char *data, int dataLen);
_declspec(dllimport) int freeBtreeData(bHEAD *b_head, char *keyBuf);
_declspec(dllimport) int dbTreeClose( bHEAD *b_head );
    /*
    */
_declspec(dllimport) short dbTreeDispose(bHEAD *b_head);
    /*
    */
_declspec(dllimport) long getBtreeDataLen(bHEAD *b_head, char *keyBuf);
_declspec(dllimport) long getBtreeError( void );
_declspec(dllimport) long getBtreeLastReadSize( void );
_declspec(dllimport) void freeBtreeMem(char *s);
_declspec(dllimport) int dbTreeBuild(char *ndxName, short keyLen, short nodeSize);
_declspec(dllimport) bHEAD * dbTreeOpen(char *ndxName);
_declspec(dllimport) long int dbTreeSeek( bHEAD *b_head, char *keyContent );
_declspec(dllimport) char * dbTreeGetKeyContent( bHEAD *b_head );
_declspec(dllimport) long int dbTreeSkip( bHEAD *b_head, long int count );
	/* Return:
	*      Success: the record no
	*      Fail:    0 (error no in BtreeErr), negative of the record
	*               no(cannot skip so much steps, skip to the limit)
	*/
_declspec(dllimport) long int dbTreeGoTop( bHEAD *b_head );
	/*
	*/
_declspec(dllimport) long int dbTreeGoBottom( bHEAD *b_head );
	/*
	*/
_declspec(dllimport) short dbTreeEof( bHEAD *b_head );
_declspec(dllimport) int dbTreeFlush( bHEAD *b_head );
#endif

void saveBtreeEnv(bHEAD *bh);
int  restoreBtreeEnv(bHEAD *bh);

#endif
