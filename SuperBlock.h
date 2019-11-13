#ifndef SUPERBLOCK_H_INCLUDED
#define SUPERBLOCK_H_INCLUDED



#endif // SUPERBLOCK_H_INCLUDED

struct __attribute__ ((packed)) SuperBlock {
        //unsigned 4 byte fields
        unsigned int inode_count;
        unsigned int block_count;
        unsigned int res_block_count; //# reserved blocks for super user
        unsigned int unused_block_count; //# un allocated blocks
        unsigned int unused_inode_count; //# un allocated inodes
        unsigned int block_number; //location of sb
        unsigned int block_size; //shift by 1024 left
        unsigned int frag_size; //shift by 1024 left
        unsigned int blocks_per_bgroup;
        unsigned int frags_per_bgroup;
        unsigned int inodes_per_bgroup;
        unsigned int last_mount_time; //POSIX time
        unsigned int last_write_time; //POSIX time
        //2 byte short fields
        unsigned short vol_mount_count;
        unsigned short mounts_permt_count;
        unsigned short ext2_magic; //ext2 signature for confirmation of existance
        unsigned short filesys_state;
        unsigned short error_resp; //who to do when an error is detected
        unsigned short minor_ver; //minor portion of version. combine with major for full version field
        //4 byte fields
        unsigned int last_check; //last consistency check in POSIX
        unsigned int checkinterval;
        unsigned int osSYS_id; //OS id of file sys
        unsigned int major_ver; //major portion of version. combine with minor
        //2 byte fields
        unsigned short userid; //user id that can use reserved blocks
        unsigned short groupid; //group id that can use reserved blocks
        //extended sb fields
        unsigned int first_inode; //first free inode in file sys
        unsigned short inode_size; //size of each inode structure
        unsigned short block_group; //block group of sb
        unsigned int opt_features; //optional features present
        unsigned int req_features; //required features present
        unsigned int ro_features; //if not supported features mounted read only

        unsigned char filesys_id[16]; //filesystem ID
        char volume_name[16]; //volume name in c-style string
        char last_mpath[64]; //the last mounted path in c-style string

        unsigned int compress_algos; //compression algorithms used
        unsigned char preallocateBlocks; //number of blocks to preallocate
        unsigned char preallocateDirect; //number of directories to preallocate
        unsigned short sb_padding; //unused space

        unsigned char journalID[16]; //journal id
        unsigned int jour_inode; //journal inode
        unsigned int jour_device; //journal device
        unsigned int orphan_head; //head of orphan inode list

        unsigned int hash_seed[4]; //seeds used for hash algorithm
        unsigned char hash_version; //hash version used for directory indexing
        unsigned char char_padding; //reserved char padding
        unsigned short short_padding; //reserved short padding
        unsigned int mount_options; //default mount options
        unsigned int first_meta_bg; //bg ID of first meta bg
        unsigned int sb_reserved[190]; //reserved mem
    };
