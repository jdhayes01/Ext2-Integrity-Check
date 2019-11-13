
#ifndef _LINUX_EXT2_FS_H
#define _LINUX_EXT2_FS_H

#define EXT2_NAME_LEN 255

/*
 * Maximal count of links to a file
 */
#define EXT2_LINK_MAX		32000

#define EXT2_SB_MAGIC_OFFSET	0x38
#define EXT2_SB_BLOCKS_OFFSET	0x04
#define EXT2_SB_BSIZE_OFFSET	0x18

static inline unsigned long int ext2_image_size(void *ext2_sb)
{
    unsigned short int EXT2_SUPER_MAGIC;

	unsigned char *p = (unsigned char *)ext2_sb;
	if (*(unsigned short int *)(p + EXT2_SB_MAGIC_OFFSET) != EXT2_SUPER_MAGIC){
		return 0;}
	return (*(unsigned int *)(p + EXT2_SB_BLOCKS_OFFSET)) <<
		(*(unsigned int *)(p + EXT2_SB_BSIZE_OFFSET));
}

#endif	// _LINUX_EXT2_FS_H
