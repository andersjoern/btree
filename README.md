# B-Tree Implementation (`btree.c`)

This project provides a packed B-tree implementation in C for managing fixed-length records in a file. The B-tree supports efficient insertion, deletion, and search operations, making it suitable for database indexing and similar use cases.

## Overview

The core logic is implemented in [`btree.c`](btree.c), which manages B-tree pages and records stored in a file. The B-tree is parameterized by record length, key length, and key position, allowing flexible indexing of binary data.

### Key Features

- **Create/Open/Close Database**: Functions to create a new B-tree file or open an existing one.
- **Insert/Delete/Replace/Search Records**: Efficient operations for managing records by key.
- **Forward/Backward Traversal**: Functions to iterate through records in key order.
- **Packed Structures**: Uses packed structs for space efficiency.
- **Page Buffering**: Internal buffering for B-tree pages to minimize disk I/O.

## Main Data Structures

- **DbFile**: Represents the database file and its metadata.
- **DATAREC**: Represents a single record, including its child pointer and data.
- **BTPAGE**: Represents a B-tree page containing multiple records and child pointers.

## API Functions

The main API functions are declared in [`btree.h`](btree.h):

- `void InitDB(void (*ErrorHandler)(const char *));`
- `BTREE_BOOL OpenDb(DbFPtr db, const char *fname);`
- `void CloseDb(DbFPtr db);`
- `void CreateDb(DbFPtr db, const char *fname, uint16_t reclen, uint16_t keylen, uint16_t keypos);`
- `BTREE_BOOL InsertRecord(DbFPtr db, const char *data);`
- `BTREE_BOOL RemoveRecord(DbFPtr db, const char *record);`
- `BTREE_BOOL ReplaceRecord(DbFPtr db, const char *Data);`
- `BTREE_BOOL GetRecord(DbFPtr db, char *data);`
- `BTREE_BOOL NextRecord(DbFPtr db, char *data);`
- `BTREE_BOOL PrevRecord(DbFPtr db, char *data);`
- `void DBFlush(DbFPtr db);`

## Usage Example

1. **Initialize the library** (optional error handler):

    ```c
    InitDB(NULL);
    ```

2. **Create a new database**:

    ```c
    DbFile db;
    CreateDb(&db, "test.dat", reclen, keylen, keypos);
    ```

3. **Insert a record**:

    ```c
    InsertRecord(&db, record_data);
    ```

4. **Search for a record**:

    ```c
    if (GetRecord(&db, search_key)) {
        // Record found
    }
    ```

5. **Delete a record**:

    ```c
    RemoveRecord(&db, key_data);
    ```

6. **Close the database**:

    ```c
    CloseDb(&db);
    ```

## File Structure

- [`btree.c`](btree.c): Main implementation of B-tree logic.
- [`btree.h`](btree.h): API declarations and data structures.
- [`testbtree.cpp`](testbtree.cpp): Example/test code for the B-tree.

## Notes

- The B-tree is designed for fixed-length records.
- All file I/O is performed using POSIX system calls.
- Error handling is customizable via `InitDB`.

## License

This code is provided as-is