#ifndef VDIFILE_H_INCLUDED
#define VDIFILE_H_INCLUDED
//for Inode struct
#define	EXT2_NDIR_BLOCKS		12                      /* number of direct blocks */
#define	EXT2_IND_BLOCK			EXT2_NDIR_BLOCKS        /* single indirect block   */
#define	EXT2_DIND_BLOCK			(EXT2_IND_BLOCK + 1)    /* double indirect block   */
#define	EXT2_TIND_BLOCK			(EXT2_DIND_BLOCK + 1)   /* trible indirect block   */
#define	EXT2_N_BLOCKS			(EXT2_TIND_BLOCK + 1)   /* total number of blocks  */

      struct __attribute__ ((packed)) VDIHeader {
        unsigned int header[16]; //header array
        unsigned int signature; //signature/magic number
        unsigned int version; //vdi header major/minor
        unsigned int hdr_size; //header length
        unsigned int type; //image type
        unsigned int flags; //image flags
        unsigned int description[64]; //header description
        unsigned int OffsetBlock; //block offset
        unsigned int OffsetData;  //data offset
        unsigned int cylinders; //number of cylinders
        unsigned int heads; //number of heads
        unsigned int sectors; //number of sectors
        unsigned int sectorsize; //sector size
        unsigned int unusedPage; //unused page
        unsigned int disksize[2]; //size of disk in bytes
        unsigned int blockSize; //block size
        unsigned int PageXsize;
        unsigned int blocks; //number of blocks
        unsigned int usedBlocks; //number of blocks being used

        unsigned int uuid[4]; //vdi's UUid
        unsigned int uusnap[4]; //last snap
        unsigned int uulink[4]; //uuid link
        unsigned int uuparent[16]; //uuid parent
        unsigned int unusedPages[14];
    };

    struct VDIFile {
        int fd; //file descriptor
        VDIHeader header;
        int fs; //file size
        off_t cursor;

    };



    struct __attribute__ ((packed)) Inode{
        unsigned short inodeMode; //file type and access rights
        unsigned short inodeUid; //low 16 bits of owner user ID
        unsigned int inodeSize; //size of inode
        unsigned int accessTime; //access tie of inode
        unsigned int createTime; //creation time
        unsigned int modTime; //modification time
        unsigned int deleteTime; //deletion time
        unsigned short groupID; //low 16 bits of group ID
        unsigned short linksCount; //links count
        unsigned int blocksCount; //blocks count
        unsigned int flags; //file flags
        unsigned int blockPointers[EXT2_N_BLOCKS]; //pointers to blocks
    };

    struct __attribute__ ((packed)) BlockGroupDescriptor{
        unsigned int bitmapBlock; //block address of block using bitmap
        unsigned int bitmapInode; //block address of inode using bitmap
        unsigned int inodeTableStart; //starting block address of inode table
        unsigned short freeBlockCount; //number of unallocated blocks in group
        unsigned short freeInodesCount; //number of unallocated inodes in group
        unsigned short directoryCount; //number of directories in group
        unsigned short bg_pad;
        unsigned int reserved[3];
    };



#endif // VDIFILE_H_INCLUDED
