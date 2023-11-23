#include <string.h>
#include <iostream>

using namespace std;

#include "btree.h"

#define BTTEST

#define BUFSIZE 50
#define MAXRECLEN ((BUFSIZE / 4) - 4)
#define RootSize(p) (((char *)&(p)->BtFd) - ((char *)&(p)->Root))
#define ORDER(recsize) ((int)(BUFSIZE / (recsize + sizeof(int32_t))))


#include "btree.c"

//         int           Root;
//         int           DelRec;
//         BTREE_BOOL     Dup;
//         unsigned short KeyLen;
//         unsigned short KeyPos;
//         unsigned short RecLen;


void testGetPos()
{
    DbFile dbf;
    dbf.Root = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    int pos = GetPos(&dbf, 1);

    if (pos == 12)
        cout << "Test GetPos -> OK" << endl;
    else
        cout << "Test GetPos -> Failed" << endl;
}

void testGetDataRecFromPage()
{
    DbFile dbf;
    dbf.Root = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    DATAREC *d;

    BTPAGE p1;
    DATAREC d1;
    DATAREC d2;
    DATAREC d3;

    d1.ChildRec = 1;
    strncpy(d1.Data, "0001AAAA", dbf.RecLen);
    d2.ChildRec = 2;
    strncpy(d2.Data, "0002BBBB", dbf.RecLen);
    d3.ChildRec = 3;
    strncpy(d3.Data, "0003CCCC", dbf.RecLen);

    p1.Count = 3;
    p1.BwChild = 0;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d1, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d2, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 2), (void *)&d3, dbf.RecLen + sizeof(int));

    d = GetDataRecFromPage(&dbf, &p1, 1);

    if (d->ChildRec == 2)
        cout << "Test GetDataRecFromPage -> OK" << endl;
    else
        cout << "Test GetDataRecFromPage -> Failed" << endl;
}

void testMove()
{
    DbFile dbf;
    dbf.Root = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    DATAREC d1;
    DATAREC d2;
    DATAREC d3;
    DATAREC d4;
    DATAREC d5;
    DATAREC d6;

    d1.ChildRec = 1;
    strncpy(d1.Data, "0001AAAA", dbf.RecLen);
    d2.ChildRec = 2;
    strncpy(d2.Data, "0002BBBB", dbf.RecLen);
    d3.ChildRec = 3;
    strncpy(d3.Data, "0003CCCC", dbf.RecLen);
    d4.ChildRec = 4;
    strncpy(d4.Data, "0004DDDD", dbf.RecLen);
    d5.ChildRec = 5;
    strncpy(d5.Data, "0005EEEE", dbf.RecLen);
    d6.ChildRec = 6;
    strncpy(d6.Data, "0006FFFF", dbf.RecLen);

    BTPAGE p1;

    p1.Count = 2;
    p1.BwChild = 0;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d1, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d2, dbf.RecLen + sizeof(int));

    BTPAGE p2;

    p2.Count = 4;
    p2.BwChild = 0;
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d3, dbf.RecLen + sizeof(int));
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d4, dbf.RecLen + sizeof(int));
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 2), (void *)&d5, dbf.RecLen + sizeof(int));
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 3), (void *)&d6, dbf.RecLen + sizeof(int));

    DbMove(&dbf, &p1, 2, &p2, 3);

    if (((DATAREC *)(p1.buf + (dbf.RecLen + sizeof(int)) * 2))->ChildRec == 6)
        cout << "Test Move -> OK" << endl;
    else
        cout << "Test Move -> Failed" << endl;
}

void testInsInPage()
{
    DbFile dbf;
    dbf.Root = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    DATAREC d1;
    DATAREC d2;
    DATAREC d3;
    DATAREC d4;
    DATAREC d5;
    DATAREC d6;

    d1.ChildRec = 1;
    strncpy(d1.Data, "0001AAAA", dbf.RecLen);
    d2.ChildRec = 2;
    strncpy(d2.Data, "0002BBBB", dbf.RecLen);
    d3.ChildRec = 3;
    strncpy(d3.Data, "0003CCCC", dbf.RecLen);
    d4.ChildRec = 4;
    strncpy(d4.Data, "0004DDDD", dbf.RecLen);
    d5.ChildRec = 5;
    strncpy(d5.Data, "0005EEEE", dbf.RecLen);
    d6.ChildRec = 6;
    strncpy(d6.Data, "0006FFFF", dbf.RecLen);

    BTPAGE p1;

    p1.Count = 3;
    p1.BwChild = 0;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d1, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d2, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 2), (void *)&d4, dbf.RecLen + sizeof(int));

    InsInPage(&dbf, &d3, &p1, 2);
    if (((DATAREC *)(p1.buf + (dbf.RecLen + sizeof(int)) * 2))->ChildRec == 3)
        cout << "Test 1 InsInPage -> OK" << endl;
    else
        cout << "Test 1 InsInPage -> Failed" << endl;

    if (((DATAREC *)(p1.buf + (dbf.RecLen + sizeof(int)) * 3))->ChildRec == 4)
        cout << "Test 2 InsInPage -> OK" << endl;
    else
        cout << "Test 2 InsInPage -> Failed" << endl;
}

void testSplit()
{
    DbFile dbf;
    dbf.Root   = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    DATAREC d1;
    DATAREC d2;
    DATAREC d3;
    DATAREC d4;
    DATAREC d5;
    DATAREC d6;
    DATAREC promoted;
    int RightChild = 20;

    d1.ChildRec = 1;
    strncpy(d1.Data, "0001AAAA", dbf.RecLen);
    d2.ChildRec = 2;
    strncpy(d2.Data, "0002BBBB", dbf.RecLen);
    d3.ChildRec = 3;
    strncpy(d3.Data, "0003CCCC", dbf.RecLen);
    d4.ChildRec = 4;
    strncpy(d4.Data, "0004DDDD", dbf.RecLen);
    d5.ChildRec = 5;
    strncpy(d5.Data, "0005EEEE", dbf.RecLen);
    d6.ChildRec = 6;
    strncpy(d6.Data, "0006FFFF", dbf.RecLen);

    BTPAGE p1;
    BTPAGE newPage;

    p1.Count = 4;
    p1.BwChild = 0;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d1, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d2, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 2), (void *)&d3, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 3), (void *)&d4, dbf.RecLen + sizeof(int));

    Split(&dbf, &d5, RightChild, &p1, &promoted, &newPage, 4);

    if (promoted.ChildRec == 20)
        cout << "Test 1 Split -> OK" << endl;
    else
        cout << "Test 1 Split -> Failed" << endl;

    if (newPage.Count == 2)
        cout << "Test 2 Split -> OK" << endl;
    else
        cout << "Test 2 Split -> Failed" << endl;

    if (p1.Count == 2)
        cout << "Test 3 Split -> OK" << endl;
    else
        cout << "Test 3 Split -> Failed" << endl;
}

void testSearchNode()
{
    DbFile dbf;
    dbf.Root   = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    DATAREC d1;
    DATAREC d2;
    DATAREC d3;
    DATAREC d4;
    DATAREC d5;
    DATAREC d6;
//    DATAREC promoted;
//    int RightChild = 20;

    d1.ChildRec = 1;
    strncpy(d1.Data, "0001AAAA", dbf.RecLen);
    d2.ChildRec = 2;
    strncpy(d2.Data, "0002BBBB", dbf.RecLen);
    d3.ChildRec = 3;
    strncpy(d3.Data, "0003CCCC", dbf.RecLen);
    d4.ChildRec = 4;
    strncpy(d4.Data, "0004DDDD", dbf.RecLen);
    d5.ChildRec = 5;
    strncpy(d5.Data, "0005EEEE", dbf.RecLen);
    d6.ChildRec = 6;
    strncpy(d6.Data, "0006FFFF", dbf.RecLen);

    BTPAGE p1;

    p1.Count = 4;
    p1.BwChild = 0;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d2, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d3, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 2), (void *)&d4, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 3), (void *)&d5, dbf.RecLen + sizeof(int));

    uint16_t pos;

    bool retval;

    retval = SearchNode(&dbf, &d3, &p1, &pos);

    if (retval == true && pos == 1)
        cout << "Test 1 SearchNode -> OK" << endl;
    else
        cout << "Test 1 SearchNode -> Failed" << endl;

    retval = SearchNode(&dbf, &d1, &p1, &pos);

    if (retval == false && pos == 0)
        cout << "Test 2 SearchNode -> OK" << endl;
    else
        cout << "Test 2 SearchNode -> Failed" << endl;

    retval = SearchNode(&dbf, &d6, &p1, &pos);
    if (retval == false && pos == 4)
        cout << "Test 3 SearchNode -> OK" << endl;
    else
        cout << "Test 3 SearchNode -> Failed" << endl;
}

void test_PushDown()
{
    DbFile dbf;
    dbf.Root   = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    BTREE_BOOL status = 0;

    DATAREC d1;
    DATAREC d2;
    DATAREC d3;
    DATAREC d4;
    DATAREC d5;
    DATAREC d6;
    DATAREC promoted;
//    int RightChild = 20;

    d1.ChildRec = 0;
    strncpy(d1.Data, "0001AAAA", dbf.RecLen);
    d2.ChildRec = 0;
    strncpy(d2.Data, "0002BBBB", dbf.RecLen);
    d3.ChildRec = 0;
    strncpy(d3.Data, "0003CCCC", dbf.RecLen);
    d4.ChildRec = 0;
    strncpy(d4.Data, "0004DDDD", dbf.RecLen);
    d5.ChildRec = 0;
    strncpy(d5.Data, "0005EEEE", dbf.RecLen);
    d6.ChildRec = 0;
    strncpy(d6.Data, "0006FFFF", dbf.RecLen);

    BTPAGE p1;

    p1.Count = 3;
    p1.BwChild = 0;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d2, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d3, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 2), (void *)&d4, dbf.RecLen + sizeof(int));

    pagebuffers.btp[0] = p1;
    pagebuffers.Db[0] = &dbf;
    pagebuffers.addr[0] = 1;

    //short pos;

    BTREE_BOOL retval = 0;

    retval = PushDown(&dbf, 1, &d1, &promoted, &status);

    char *data = ((DATAREC *)(pagebuffers.btp[0].buf + (dbf.RecLen + sizeof(int)) * 0))->Data;

    if (strncmp(data, "0001AAAA", dbf.RecLen) == 0)
        cout << "Test 1 PushDown -> OK" << endl;
    else
        cout << "Test 1 PushDown -> Failed" << endl;

    if (pagebuffers.btp[0].Count == 4)
        cout << "Test 2 PushDown -> OK" << endl;
    else
        cout << "Test 2 PushDown -> Failed" << endl;
    if (retval == 0)
        cout << "Test 3 PushDown -> OK" << endl;
    else
        cout << "Test 3 PushDown -> Failed" << endl;
}

void test_CreateDb()
{
    DbFile dbf;
    const unsigned short reclen = 8;
    const unsigned short keylen = 4;
    const unsigned short keypos = 0;

    remove("test.dat");

    InitDB(NULL);

    CreateDb(&dbf, "test.dat", reclen, keylen, keypos);

    CloseDb(&dbf);

    if (dbf.Root == 0L)
        cout << "Test 1 CreateDb -> OK" << endl;
    else
        cout << "Test 1 CreateDb -> Failed" << endl;

    if (dbf.DelRec == 0L)
        cout << "Test 2 CreateDb -> OK" << endl;
    else
        cout << "Test 2 CreateDb -> Failed" << endl;

    if (dbf.KeyLen == 4)
        cout << "Test 3 CreateDb -> OK" << endl;
    else
        cout << "Test 3 CreateDb -> Failed" << endl;

    if (dbf.KeyPos == 0)
        cout << "Test 4 CreateDb -> OK" << endl;
    else
        cout << "Test 4 CreateDb -> Failed" << endl;

    if (dbf.RecLen == 8)
        cout << "Test 5 CreateDb -> OK" << endl;
    else
        cout << "Test 5 CreateDb -> Failed" << endl;
}

void test_InsertRecord()
{
    DbFile dbf;
    const unsigned short reclen = 8;
    const unsigned short keylen = 4;
    const unsigned short keypos = 0;

    remove("test.dat");

    InitDB(NULL);

    CreateDb(&dbf, "test.dat", reclen, keylen, keypos);

    InsertRecord(&dbf, "0001AAAA");

    CloseDb(&dbf);

    if (dbf.Root != 0L)
        cout << "Test 1 InsertRecord -> OK" << endl;
    else
        cout << "Test 1 InsertRecord -> Failed" << endl;
}

void test_InsertRecords()
{
    DbFile dbf;
    const unsigned short reclen = 8;
    const unsigned short keylen = 4;
    const unsigned short keypos = 0;

    remove("tets1.dat");

    InitDB(NULL);

    CreateDb(&dbf, "test1.dat", reclen, keylen, keypos);

    InsertRecord(&dbf, "0001AAAA");
    InsertRecord(&dbf, "0002BBBB");
    InsertRecord(&dbf, "0003CCCC");
    InsertRecord(&dbf, "0004DDDD");
    InsertRecord(&dbf, "0005EEEE");
    InsertRecord(&dbf, "0006FFFF");
    InsertRecord(&dbf, "0007GGGG");
    InsertRecord(&dbf, "0008HHHH");
    InsertRecord(&dbf, "0009IIII");
    InsertRecord(&dbf, "0010JJJJ");
    InsertRecord(&dbf, "0011KKKK");
    InsertRecord(&dbf, "0012LLLL");
    InsertRecord(&dbf, "0013MMMM");
    InsertRecord(&dbf, "0014NNNN");
    InsertRecord(&dbf, "0015OOOO");
    InsertRecord(&dbf, "0016PPPP");
    InsertRecord(&dbf, "0017QQQQ");
    InsertRecord(&dbf, "0018RRRR");
    InsertRecord(&dbf, "0019SSSS");
    InsertRecord(&dbf, "0020TTTT");
    InsertRecord(&dbf, "0021UUUU");
    InsertRecord(&dbf, "0022VVVV");
    InsertRecord(&dbf, "0023WWWW");
    InsertRecord(&dbf, "0024XXXX");
    InsertRecord(&dbf, "0025YYYY");
    InsertRecord(&dbf, "0026ZZZZ");
    InsertRecord(&dbf, "0027ÆÆÆÆ");
    InsertRecord(&dbf, "0028ØØØØ");
    InsertRecord(&dbf, "0029ÅÅÅÅ");
    InsertRecord(&dbf, "0030aaaa");
    InsertRecord(&dbf, "0031bbbb");
    InsertRecord(&dbf, "0032cccc");
    InsertRecord(&dbf, "0033dddd");
    InsertRecord(&dbf, "0034ffff");
    InsertRecord(&dbf, "0035gggg");
    InsertRecord(&dbf, "0036hhhh");
    InsertRecord(&dbf, "0037iiii");
    InsertRecord(&dbf, "0038jjjj");
    InsertRecord(&dbf, "0039kkkk");

    CloseDb(&dbf);

    if (dbf.Root != 0L)
        cout << "Test 2 InsertRecords -> OK" << endl;
    else
        cout << "Test 2 InsertRecords -> Failed" << endl;
}

void test_GetRecordFound()
{
    DbFile dbf;

    BTREE_BOOL found;

    char myRecord[9];

    InitDB(NULL);

    OpenDb(&dbf, "test1.dat");

    strcpy(myRecord, "0014");

    found = GetRecord(&dbf, myRecord);

    if (found == 1)
        cout << "Test 1 GetRecordFound -> OK" << endl;
    else
        cout << "Test 1 GetRecordFound -> Failed" << endl;
}

void test_GetRecordNotFound()
{
    DbFile dbf;

    BTREE_BOOL found;

    char myRecord[9];

    InitDB(NULL);

    OpenDb(&dbf, "test1.dat");

    strcpy(myRecord, "001A");

    found = GetRecord(&dbf, myRecord);

    if (found == 0)
        cout << "Test 1 GetRecordNotFound -> OK" << endl;
    else
        cout << "Test 1 GetRecordNotFound -> Failed" << endl;

    if (dbf.PP > 0)
        cout << "Test 2 GetRecordNotFound -> OK" << endl;
    else
        cout << "Test 2 GetRecordNotFound -> Failed" << endl;
}

void test_GetAllForward()
{
    DbFile dbf;

    BTREE_BOOL found;

    char myRecord[9];

    InitDB(NULL);

    OpenDb(&dbf, "test1.dat");

    int count = 0;

    while ((found = NextRecord(&dbf, myRecord)) == 1)
        count++;

    if (count == 39)
        cout << "Test 1 NextRecord -> OK" << endl;
    else
        cout << "Test 1 NextRecord -> Failed" << endl;
}

void test_GetAllBackward()
{
    DbFile dbf;

    BTREE_BOOL found;

    char myRecord[9];

    InitDB(NULL);

    OpenDb(&dbf, "test1.dat");

    int count = 0;

    while ((found = PrevRecord(&dbf, myRecord)) == 1)
        count++;

    if (count == 39)
        cout << "Test 1 PrevRecord -> OK" << endl;
    else
        cout << "Test 1 PrevRecord -> Failed" << endl;
}

void test_moveRight()
{
    DbFile dbf;
    dbf.Root = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    DATAREC d1;
    DATAREC d2;
    DATAREC d3;
    DATAREC d4;
    DATAREC d5;
    DATAREC d6;

    d1.ChildRec = 1;
    strncpy(d1.Data, "0001AAAA", dbf.RecLen);
    d2.ChildRec = 2;
    strncpy(d2.Data, "0002BBBB", dbf.RecLen);
    d3.ChildRec = 3;
    strncpy(d3.Data, "0003CCCC", dbf.RecLen);
    d4.ChildRec = 4;
    strncpy(d4.Data, "0004DDDD", dbf.RecLen);
    d5.ChildRec = 5;
    strncpy(d5.Data, "0005EEEE", dbf.RecLen);
    d6.ChildRec = 6;
    strncpy(d6.Data, "0006FFFF", dbf.RecLen);

    BTPAGE p1;

    p1.Count = 2;
    p1.BwChild = 10;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d1, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d2, dbf.RecLen + sizeof(int));

    BTPAGE p2;

    p2.Count = 3;
    p2.BwChild = 20;
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d4, dbf.RecLen + sizeof(int));
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d5, dbf.RecLen + sizeof(int));
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 2), (void *)&d6, dbf.RecLen + sizeof(int));

    BTPAGE parent;

    parent.Count = 1;
    parent.BwChild = 30;
    memcpy((void *)(parent.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d3, dbf.RecLen + sizeof(int));

    MoveRight(&dbf, &parent, &p1, &p2, 0);

    if (GetDataRecFromPage(&dbf, &parent, 0)->ChildRec == 3)
        cout << "Test 1 MoveRight -> OK" << endl;
    else
        cout << "Test 1 MoveRight -> Failed" << endl;

    if (strncmp(GetDataRecFromPage(&dbf, &parent, 0)->Data, "0006FFFF", 8) == 0)
        cout << "Test 2 MoveRight -> OK" << endl;
    else
        cout << "Test 2 MoveRight -> Failed" << endl;

    if (strncmp(GetDataRecFromPage(&dbf, &p1, 0)->Data, "0003CCCC", 8) == 0)
        cout << "Test 3 MoveRight -> OK" << endl;
    else
        cout << "Test 3 MoveRight -> Failed" << endl;

    if (p1.BwChild == 6)
        cout << "Test 4 MoveRight -> OK" << endl;
    else
        cout << "Test 4 MoveRight -> Failed" << endl;

    if (GetDataRecFromPage(&dbf, &p1, 0)->ChildRec == 10)
        cout << "Test 5 MoveRight -> OK" << endl;
    else
        cout << "Test 5 MoveRight -> Failed" << endl;
}

void test_moveLeft()
{
    DbFile dbf;
    dbf.Root = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    DATAREC d1;
    DATAREC d2;
    DATAREC d3;
    DATAREC d4;
    DATAREC d5;
    DATAREC d6;

    d1.ChildRec = 1;
    strncpy(d1.Data, "0001AAAA", dbf.RecLen);
    d2.ChildRec = 2;
    strncpy(d2.Data, "0002BBBB", dbf.RecLen);
    d3.ChildRec = 3;
    strncpy(d3.Data, "0003CCCC", dbf.RecLen);
    d4.ChildRec = 4;
    strncpy(d4.Data, "0004DDDD", dbf.RecLen);
    d5.ChildRec = 5;
    strncpy(d5.Data, "0005EEEE", dbf.RecLen);
    d6.ChildRec = 6;
    strncpy(d6.Data, "0006FFFF", dbf.RecLen);

    BTPAGE p1;

    p1.Count = 2;
    p1.BwChild = 10;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d1, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d2, dbf.RecLen + sizeof(int));

    BTPAGE p2;

    p2.Count = 3;
    p2.BwChild = 20;
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d4, dbf.RecLen + sizeof(int));
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d5, dbf.RecLen + sizeof(int));
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 2), (void *)&d6, dbf.RecLen + sizeof(int));

    BTPAGE parent;

    parent.Count = 1;
    parent.BwChild = 30;
    memcpy((void *)(parent.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d3, dbf.RecLen + sizeof(int));

    MoveLeft(&dbf, &parent, &p1, &p2, 0);

    if (GetDataRecFromPage(&dbf, &parent, 0)->ChildRec == 3)
        cout << "Test 1 MoveLeft -> OK" << endl;
    else
        cout << "Test 1 MoveLeft -> Failed" << endl;

    if (strncmp(GetDataRecFromPage(&dbf, &parent, 0)->Data, "0004DDDD", 8) == 0)
        cout << "Test 2 MoveLeft -> OK" << endl;
    else
        cout << "Test 2 MoveLeft -> Failed" << endl;

    if (strncmp(GetDataRecFromPage(&dbf, &p1, 2)->Data, "0003CCCC", 8) == 0)
        cout << "Test 3 MoveLeft -> OK" << endl;
    else
        cout << "Test 3 MoveLeft -> Failed" << endl;

    if (p2.BwChild == 4)
        cout << "Test 4 MoveLeft -> OK" << endl;
    else
        cout << "Test 4 MoveLeft -> Failed" << endl;

    if (GetDataRecFromPage(&dbf, &p1, 2)->ChildRec == 20)
        cout << "Test 5 MoveLeft -> OK" << endl;
    else
        cout << "Test 5 MoveLeft -> Failed" << endl;
}

void test_Combine()
{
    DbFile dbf;
    dbf.Root = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    DATAREC d1;
    DATAREC d2;
    DATAREC d3;
    DATAREC d4;
    DATAREC d5;
    DATAREC d6;

    d1.ChildRec = 1;
    strncpy(d1.Data, "0001AAAA", dbf.RecLen);
    d2.ChildRec = 2;
    strncpy(d2.Data, "0002BBBB", dbf.RecLen);
    d3.ChildRec = 3;
    strncpy(d3.Data, "0003CCCC", dbf.RecLen);
    d4.ChildRec = 4;
    strncpy(d4.Data, "0004DDDD", dbf.RecLen);
    d5.ChildRec = 5;
    strncpy(d5.Data, "0005EEEE", dbf.RecLen);
    d6.ChildRec = 6;
    strncpy(d6.Data, "0006FFFF", dbf.RecLen);

    BTPAGE p1;

    p1.Count = 1;
    p1.BwChild = 10;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d1, dbf.RecLen + sizeof(int));

    BTPAGE p2;

    p2.Count = 2;
    p2.BwChild = 20;
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d4, dbf.RecLen + sizeof(int));
    memcpy((void *)(p2.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d5, dbf.RecLen + sizeof(int));

    BTPAGE parent;

    parent.Count = 1;
    parent.BwChild = 30;
    memcpy((void *)(parent.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d3, dbf.RecLen + sizeof(int));

    Combine(&dbf, &parent, &p1, &p2, 0);

    if (parent.Count == 0)
        cout << "Test 1 Combine -> OK" << endl;
    else
        cout << "Test 1 Combine -> Failed" << endl;

    if (p1.Count == 4)
        cout << "Test 2 Combine -> OK" << endl;
    else
        cout << "Test 2 Combine -> Failed" << endl;

    if (GetDataRecFromPage(&dbf, &p1, 1)->ChildRec == 20)
        cout << "Test 3 Combine -> OK" << endl;
    else
        cout << "Test 3 Combine -> Failed" << endl;
}

void test_Remove()
{
    DbFile dbf;
    dbf.Root = 0L;
    dbf.DelRec = 0L;
    dbf.KeyLen = 4;
    dbf.KeyPos = 0;
    dbf.RecLen = 8;

    DATAREC d1;
    DATAREC d2;
    DATAREC d3;
    DATAREC d4;
    DATAREC d5;
    DATAREC d6;

    d1.ChildRec = 1;
    strncpy(d1.Data, "0001AAAA", dbf.RecLen);
    d2.ChildRec = 2;
    strncpy(d2.Data, "0002BBBB", dbf.RecLen);
    d3.ChildRec = 3;
    strncpy(d3.Data, "0003CCCC", dbf.RecLen);
    d4.ChildRec = 4;
    strncpy(d4.Data, "0004DDDD", dbf.RecLen);
    d5.ChildRec = 5;
    strncpy(d5.Data, "0005EEEE", dbf.RecLen);
    d6.ChildRec = 6;
    strncpy(d6.Data, "0006FFFF", dbf.RecLen);

    BTPAGE p1;

    p1.Count = 4;
    p1.BwChild = 10;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d1, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d2, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 2), (void *)&d3, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 3), (void *)&d4, dbf.RecLen + sizeof(int));

    Remove(&dbf, &p1, 1);

    if (p1.Count == 3)
        cout << "Test 1 Remove -> OK" << endl;
    else
        cout << "Test 1 Remove -> Failed" << endl;

    if (strncmp(GetDataRecFromPage(&dbf, &p1, 1)->Data, "0003CCCC", 8) == 0)
        cout << "Test 2 Remove -> OK" << endl;
    else
        cout << "Test 2 Remove -> Failed" << endl;

    p1.Count = 4;
    p1.BwChild = 10;
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 0), (void *)&d1, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 1), (void *)&d2, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 2), (void *)&d3, dbf.RecLen + sizeof(int));
    memcpy((void *)(p1.buf + (dbf.RecLen + sizeof(int)) * 3), (void *)&d4, dbf.RecLen + sizeof(int));

    Remove(&dbf, &p1, 3);

    if (p1.Count == 3)
        cout << "Test 3 Remove -> OK" << endl;
    else
        cout << "Test 3 Remove -> Failed" << endl;

    Remove(&dbf, &p1, 0);

    if (p1.Count == 2)
        cout << "Test 4 Remove -> OK" << endl;
    else
        cout << "Test 4 Remove -> Failed" << endl;
}

void test_DeleteRecord()
{
    DbFile dbf;

    BTREE_BOOL found;

    char myRecord[9];

    InitDB(NULL);

    OpenDb(&dbf, "test1.dat");

    int count = 0;

    RemoveRecord(&dbf, "0018RRRR");

    while ((found = NextRecord(&dbf, myRecord)) == 1)
        count++;

    if (count == 38)
        cout << "Test 1 DeteteRecord -> OK" << endl;
    else
        cout << "Test 1 DeleteRecord -> Failed" << endl;
}

void test_DeleteRecord2()
{
    BTREE_BOOL found = 0;
    DbFile dbf;
    const unsigned short reclen = 8;
    const unsigned short keylen = 4;
    const unsigned short keypos = 0;
    char myRecord[9];

    remove("tets3.dat");

    InitDB(NULL);

    CreateDb(&dbf, "test3.dat", reclen, keylen, keypos);

    InsertRecord(&dbf, "0001AAAA");
    InsertRecord(&dbf, "0002BBBB");
    InsertRecord(&dbf, "0003CCCC");
    InsertRecord(&dbf, "0004DDDD");
    InsertRecord(&dbf, "0005EEEE");

    int count = 0;

    RemoveRecord(&dbf, "0003CCCC");

    while ((found = NextRecord(&dbf, myRecord)) == 1)
        count++;

    CloseDb(&dbf);

    if (count == 4)
        cout << "Test 2 DeteteRecord -> OK" << endl;
    else
        cout << "Test 2 DeleteRecord -> Failed" << endl;
}

void test_DeleteRecord3()
{
    BTREE_BOOL found;
    DbFile dbf;
    const unsigned short reclen = 8;
    const unsigned short keylen = 4;
    const unsigned short keypos = 0;
    char myRecord[9];

    remove("tets4.dat");

    InitDB(NULL);

    CreateDb(&dbf, "test4.dat", reclen, keylen, keypos);

    InsertRecord(&dbf, "0001AAAA");
    InsertRecord(&dbf, "0002BBBB");
    InsertRecord(&dbf, "0003CCCC");
    InsertRecord(&dbf, "0004DDDD");
    InsertRecord(&dbf, "0005EEEE");
    InsertRecord(&dbf, "0006FFFF");
    InsertRecord(&dbf, "0007GGGG");
    InsertRecord(&dbf, "0008HHHH");
    InsertRecord(&dbf, "0009IIII");
    InsertRecord(&dbf, "0010JJJJ");
    InsertRecord(&dbf, "0011KKKK");
    InsertRecord(&dbf, "0012LLLL");
    InsertRecord(&dbf, "0013MMMM");
    InsertRecord(&dbf, "0014NNNN");
    InsertRecord(&dbf, "0015OOOO");
    InsertRecord(&dbf, "0016PPPP");
    InsertRecord(&dbf, "0017QQQQ");
    InsertRecord(&dbf, "0018RRRR");
    InsertRecord(&dbf, "0019SSSS");
    InsertRecord(&dbf, "0020TTTT");
    InsertRecord(&dbf, "0021UUUU");
    InsertRecord(&dbf, "0022VVVV");
    InsertRecord(&dbf, "0023WWWW");
    InsertRecord(&dbf, "0024XXXX");
    InsertRecord(&dbf, "0025YYYY");
    InsertRecord(&dbf, "0026ZZZZ");
    InsertRecord(&dbf, "0027ÆÆÆÆ");
    InsertRecord(&dbf, "0028ØØØØ");
    InsertRecord(&dbf, "0029ÅÅÅÅ");
    InsertRecord(&dbf, "0030aaaa");
    InsertRecord(&dbf, "0031bbbb");
    InsertRecord(&dbf, "0032cccc");
    InsertRecord(&dbf, "0033dddd");
    InsertRecord(&dbf, "0034ffff");
    InsertRecord(&dbf, "0035gggg");
    InsertRecord(&dbf, "0036hhhh");
    InsertRecord(&dbf, "0037iiii");
    InsertRecord(&dbf, "0038jjjj");
    InsertRecord(&dbf, "0039kkkk");

    RemoveRecord(&dbf, "0001AAAA");
    RemoveRecord(&dbf, "0002BBBB");
    RemoveRecord(&dbf, "0003CCCC");
    RemoveRecord(&dbf, "0004DDDD");
    RemoveRecord(&dbf, "0005EEEE");
    RemoveRecord(&dbf, "0006FFFF");
    RemoveRecord(&dbf, "0007GGGG");
    RemoveRecord(&dbf, "0008HHHH");
    RemoveRecord(&dbf, "0009IIII");
    RemoveRecord(&dbf, "0010JJJJ");
    RemoveRecord(&dbf, "0011KKKK");
    RemoveRecord(&dbf, "0012LLLL");
    RemoveRecord(&dbf, "0014MMMM");
    RemoveRecord(&dbf, "0015NNNN");
    RemoveRecord(&dbf, "0016OOOO");

    int count = 0;

    while ((found = NextRecord(&dbf, myRecord)) == 1)
        count++;

    CloseDb(&dbf);

    if (count == 24)
        cout << "Test 3 DeleteRecord -> OK" << endl;
    else
        cout << "Test 3 DeleteRecord -> Failed" << endl;
}

void test_DeleteRecord4()
{
    BTREE_BOOL found;
    DbFile dbf;
    const unsigned short reclen = 8;
    const unsigned short keylen = 4;
    const unsigned short keypos = 0;
    char myRecord[9];

    remove("tets5.dat");

    InitDB(NULL);

    CreateDb(&dbf, "test5.dat", reclen, keylen, keypos);

    InsertRecord(&dbf, "0001AAAA");
    InsertRecord(&dbf, "0002BBBB");
    InsertRecord(&dbf, "0003CCCC");
    InsertRecord(&dbf, "0004DDDD");
    InsertRecord(&dbf, "0005EEEE");
    InsertRecord(&dbf, "0006FFFF");
    InsertRecord(&dbf, "0007GGGG");
    InsertRecord(&dbf, "0008HHHH");
    InsertRecord(&dbf, "0009IIII");
    InsertRecord(&dbf, "0010JJJJ");
    InsertRecord(&dbf, "0011KKKK");
    InsertRecord(&dbf, "0012LLLL");
    InsertRecord(&dbf, "0013MMMM");
    InsertRecord(&dbf, "0014NNNN");
    InsertRecord(&dbf, "0015OOOO");
    InsertRecord(&dbf, "0016PPPP");
    InsertRecord(&dbf, "0017QQQQ");
    InsertRecord(&dbf, "0018RRRR");
    InsertRecord(&dbf, "0019SSSS");
    InsertRecord(&dbf, "0020TTTT");
    InsertRecord(&dbf, "0021UUUU");
    InsertRecord(&dbf, "0022VVVV");
    InsertRecord(&dbf, "0023WWWW");
    InsertRecord(&dbf, "0024XXXX");
    InsertRecord(&dbf, "0025YYYY");
    InsertRecord(&dbf, "0026ZZZZ");
    InsertRecord(&dbf, "0027ÆÆÆÆ");
    InsertRecord(&dbf, "0028ØØØØ");
    InsertRecord(&dbf, "0029ÅÅÅÅ");
    InsertRecord(&dbf, "0030aaaa");
    InsertRecord(&dbf, "0031bbbb");
    InsertRecord(&dbf, "0032cccc");
    InsertRecord(&dbf, "0033dddd");
    InsertRecord(&dbf, "0034ffff");
    InsertRecord(&dbf, "0035gggg");
    InsertRecord(&dbf, "0036hhhh");
    InsertRecord(&dbf, "0037iiii");
    InsertRecord(&dbf, "0038jjjj");
    InsertRecord(&dbf, "0039kkkk");

    RemoveRecord(&dbf, "0001AAAA");
    RemoveRecord(&dbf, "0002BBBB");
    RemoveRecord(&dbf, "0003CCCC");
    RemoveRecord(&dbf, "0004DDDD");
    RemoveRecord(&dbf, "0005EEEE");
    RemoveRecord(&dbf, "0006FFFF");
    RemoveRecord(&dbf, "0007GGGG");
    RemoveRecord(&dbf, "0008HHHH");
    RemoveRecord(&dbf, "0009IIII");
    RemoveRecord(&dbf, "0010JJJJ");
    RemoveRecord(&dbf, "0011KKKK");
    RemoveRecord(&dbf, "0012LLLL");
    RemoveRecord(&dbf, "0014MMMM");
    RemoveRecord(&dbf, "0015NNNN");
    RemoveRecord(&dbf, "0016OOOO");

    InsertRecord(&dbf, "0001AAAA");
    InsertRecord(&dbf, "0002BBBB");
    InsertRecord(&dbf, "0003CCCC");
    InsertRecord(&dbf, "0004DDDD");
    InsertRecord(&dbf, "0005EEEE");
    InsertRecord(&dbf, "0006FFFF");
    InsertRecord(&dbf, "0007GGGG");
    InsertRecord(&dbf, "0008HHHH");
    InsertRecord(&dbf, "0009IIII");
    InsertRecord(&dbf, "0010JJJJ");
    InsertRecord(&dbf, "0011KKKK");
    InsertRecord(&dbf, "0012LLLL");
    InsertRecord(&dbf, "0013MMMM");
    InsertRecord(&dbf, "0014NNNN");
    InsertRecord(&dbf, "0015OOOO");
    InsertRecord(&dbf, "0016PPPP");

    int count = 0;

    while ((found = NextRecord(&dbf, myRecord)) == 1)
        count++;

    CloseDb(&dbf);

    if (count == 39)
        cout << "Test 4 DeteteRecord -> OK" << endl;
    else
        cout << "Test 4 DeleteRecord -> Failed" << endl;
}

void test_GetRecordNextRecord()
{
    BTREE_BOOL found;
    DbFile dbf;
    const unsigned short reclen = 8;
    const unsigned short keylen = 4;
    const unsigned short keypos = 0;
    char myRecord[9];

    remove("tets6.dat");

    InitDB(NULL);

    CreateDb(&dbf, "test6.dat", reclen, keylen, keypos);

    InsertRecord(&dbf, "0001AAAA");
    InsertRecord(&dbf, "0002BBBB");
    InsertRecord(&dbf, "0003CCCC");
    InsertRecord(&dbf, "0004DDDD");
    InsertRecord(&dbf, "0005EEEE");
    InsertRecord(&dbf, "0006FFFF");
    InsertRecord(&dbf, "0007GGGG");
    InsertRecord(&dbf, "0008HHHH");
    InsertRecord(&dbf, "0009IIII");
    InsertRecord(&dbf, "0010JJJJ");
    InsertRecord(&dbf, "0011KKKK");
    InsertRecord(&dbf, "0012LLLL");
    InsertRecord(&dbf, "0013MMMM");
    InsertRecord(&dbf, "0015OOOO");
    InsertRecord(&dbf, "0016PPPP");
    InsertRecord(&dbf, "0017QQQQ");
    InsertRecord(&dbf, "0018RRRR");
    InsertRecord(&dbf, "0019SSSS");
    InsertRecord(&dbf, "0020TTTT");
    InsertRecord(&dbf, "0021UUUU");
    InsertRecord(&dbf, "0022VVVV");
    InsertRecord(&dbf, "0023WWWW");
    InsertRecord(&dbf, "0024XXXX");
    InsertRecord(&dbf, "0025YYYY");
    InsertRecord(&dbf, "0026ZZZZ");
    InsertRecord(&dbf, "0027ÆÆÆÆ");
    InsertRecord(&dbf, "0028ØØØØ");
    InsertRecord(&dbf, "0029ÅÅÅÅ");
    InsertRecord(&dbf, "0030aaaa");
    InsertRecord(&dbf, "0031bbbb");
    InsertRecord(&dbf, "0032cccc");
    InsertRecord(&dbf, "0033dddd");
    InsertRecord(&dbf, "0034ffff");
    InsertRecord(&dbf, "0035gggg");
    InsertRecord(&dbf, "0036hhhh");
    InsertRecord(&dbf, "0037iiii");
    InsertRecord(&dbf, "0038jjjj");
    InsertRecord(&dbf, "0039kkkk");


//    int count = 0;

    strncpy(myRecord, "0001    ", 8);

    found = GetRecord(&dbf, myRecord);

    found = NextRecord(&dbf, myRecord);


    if (found)
        cout << "Test 1 GetRecordNextRecord -> OK" << endl;
    else
        cout << "Test 1 GetRecordNextRecord -> Failed" << endl;

    if (strncmp(myRecord, "0002BBBB", 8) == 0)
        cout << "Test 2 GetRecordNextRecord -> OK" << endl;
    else
        cout << "Test 2 GetRecordNextRecord -> Failed " << endl;


    strncpy(myRecord, "0024    ", 8);

    found = GetRecord(&dbf, myRecord);

    found = NextRecord(&dbf, myRecord);

    if (found)
        cout << "Test 3 GetRecordNextRecord -> OK" << endl;
    else
        cout << "Test 3 GetRecordNextRecord -> Failed" << endl;

    if (strncmp(myRecord, "0025YYYY", 8) == 0)
        cout << "Test 4 GetRecordNextRecord -> OK" << endl;
    else
        cout << "Test 4 GetRecordNextRecord -> Failed" << endl;

    CloseDb(&dbf);
}

void test_GetRecordPrevRecord()
{
    BTREE_BOOL found;
    DbFile dbf;
    const unsigned short reclen = 8;
    const unsigned short keylen = 4;
    const unsigned short keypos = 0;
    char myRecord[9];

    remove("tets6.dat");

    InitDB(NULL);

    CreateDb(&dbf, "test6.dat", reclen, keylen, keypos);

    InsertRecord(&dbf, "0001AAAA");
    InsertRecord(&dbf, "0002BBBB");
    InsertRecord(&dbf, "0003CCCC");
    InsertRecord(&dbf, "0004DDDD");
    InsertRecord(&dbf, "0005EEEE");
    InsertRecord(&dbf, "0006FFFF");
    InsertRecord(&dbf, "0007GGGG");
    InsertRecord(&dbf, "0008HHHH");
    InsertRecord(&dbf, "0009IIII");
    InsertRecord(&dbf, "0010JJJJ");
    InsertRecord(&dbf, "0011KKKK");
    InsertRecord(&dbf, "0012LLLL");
    InsertRecord(&dbf, "0013MMMM");
    InsertRecord(&dbf, "0015OOOO");
    InsertRecord(&dbf, "0016PPPP");
    InsertRecord(&dbf, "0017QQQQ");
    InsertRecord(&dbf, "0018RRRR");
    InsertRecord(&dbf, "0019SSSS");
    InsertRecord(&dbf, "0020TTTT");
    InsertRecord(&dbf, "0021UUUU");
    InsertRecord(&dbf, "0022VVVV");
    InsertRecord(&dbf, "0023WWWW");
    InsertRecord(&dbf, "0024XXXX");
    InsertRecord(&dbf, "0025YYYY");
    InsertRecord(&dbf, "0026ZZZZ");
    InsertRecord(&dbf, "0027ÆÆÆÆ");
    InsertRecord(&dbf, "0028ØØØØ");
    InsertRecord(&dbf, "0029ÅÅÅÅ");
    InsertRecord(&dbf, "0030aaaa");
    InsertRecord(&dbf, "0031bbbb");
    InsertRecord(&dbf, "0032cccc");
    InsertRecord(&dbf, "0033dddd");
    InsertRecord(&dbf, "0034ffff");
    InsertRecord(&dbf, "0035gggg");
    InsertRecord(&dbf, "0036hhhh");
    InsertRecord(&dbf, "0037iiii");
    InsertRecord(&dbf, "0038jjjj");
    InsertRecord(&dbf, "0039kkkk");

    strncpy(myRecord, "0014    ", 8);

    found = GetRecord(&dbf, myRecord);

	if (!found)
		found = NextRecord(&dbf, myRecord);

    found = PrevRecord(&dbf, myRecord);

    if (found)
        cout << "Test 1 GetRecordPrevRecord -> OK" << endl;
    else
        cout << "Test 1 GetRecordPrevRecord -> Failed" << endl;

    if (strncmp(myRecord, "0013MMMM", 8) == 0)
        cout << "Test 2 GetRecordPrevRecord -> OK" << endl;
    else
        cout << "Test 2 GetRecordPrevRecord -> Failed" << endl;

    strncpy(myRecord, "0024    ", 8);

    found = GetRecord(&dbf, myRecord);

    found = PrevRecord(&dbf, myRecord);

    if (found)
        cout << "Test 3 GetRecordPrevRecord -> OK" << endl;
    else
        cout << "Test 3 GetRecordPrevRecord -> Failed" << endl;

    if (strncmp(myRecord, "0023WWWW", 8) == 0)
        cout << "Test 4 GetRecordPrevRecord -> OK" << endl;
    else
        cout << "Test 4 GetRecordPrevRecord -> Failed" << endl;

    CloseDb(&dbf);
}

void test_UpdateRecord()
{
    DbFile dbf;
    const unsigned short reclen = 8;
    const unsigned short keylen = 4;
    const unsigned short keypos = 0;

    char buf[9];

    remove("tets6.dat");

    InitDB(NULL);

    CreateDb(&dbf, "test6.dat", reclen, keylen, keypos);

    InsertRecord(&dbf, "0001AAAA");
    InsertRecord(&dbf, "0002BBBB");
    InsertRecord(&dbf, "0003CCCC");
    InsertRecord(&dbf, "0004DDDD");

    const char *newRec = "0001aaaa";
	const char *newRec2 = "1001aaaa";

    if (ReplaceRecord(&dbf, newRec))
    {
        cout << "Test 1 UpdateRecord -> OK" << endl;
    }
    else
    {
        cout << "Test 1 UpdateRecord -> Failed" << endl;
    }

	strncpy(buf, "0001    ", 8);

    if (GetRecord(&dbf, buf))
        cout << "Test 2 UpdateRecord -> OK" << endl;
    else
        cout << "Test 2 UpdateRecord -> Failed" << endl;

    if (memcmp(buf, newRec, 8) == 0)
        cout << "Test 3 UpdateRecord -> OK" << endl;
    else
        cout << "Test 3 UpdateRecord -> Failed" << endl;

    if (!ReplaceRecord(&dbf, newRec2))
    {
        cout << "Test 4 UpdateRecord -> OK" << endl;
    }
    else
    {
        cout << "Test 4 UpdateRecord -> Failed" << endl;
    }

	CloseDb(&dbf);
}

void test_OpenClose()
{
    InitDB(NULL);
    DbFile dbf;
    OpenDb(&dbf, "test6.dat");
    CloseDb(&dbf);
}

void test_RootSize()
{
    InitDB(NULL);
    DbFile dbf;

    OpenDb(&dbf, "test6.dat");
    CloseDb(&dbf);

    if ((long)&dbf.BtFd - (long)&dbf.Root == 16)
        cout << "Test 1 RootSize -> OK" << endl;
    else
        cout << "Test 1 RootSize -> Failed" << endl;
}

int main()
{
    testGetPos();
    testGetDataRecFromPage();
    testMove();
    testInsInPage();
    testSplit();
    testSearchNode();
    test_PushDown();
    test_CreateDb();
    test_InsertRecord();

    test_InsertRecords();

    test_GetRecordFound();
    test_GetRecordNotFound();

    test_GetAllForward();
    test_GetAllBackward();

    test_moveRight();
    test_moveLeft();
    test_Combine();
    test_Remove();

    test_DeleteRecord();
    test_DeleteRecord2();
    test_DeleteRecord3();
    test_DeleteRecord4();

    test_GetRecordNextRecord();

    test_GetRecordPrevRecord();

    test_UpdateRecord();

    test_OpenClose();

    test_RootSize();

    return 0;
}
