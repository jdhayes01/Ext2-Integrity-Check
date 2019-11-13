#include "ext2.fs.h"
#include <iostream>
#include "unistd.h"
#include "vdifile.h"
#include "ext2.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "SuperBlock.h"
#include "BootRecord.h"
#include <typeinfo>

#define BASE_OFFSET 1024

/*
OPERATING SYSTEMS PROJECT
Checking the integrity of ext2 filesystem/vdi file
Authors: James Hayes, Firaus Odeh, and assisted/guided by Dr. Kramer
References:
https://wiki.osdev.org/Ext2
https://wiki.osdev.org/MBR_(x86)
https://wiki.osdev.org/Partition_Table
http://cs.smith.edu/~nhowe/Teaching/csc262/oldlabs/ext2.html
https://www.nongnu.org/ext2-doc/ext2.html#SUPERBLOCK
https://forums.virtualbox.org/viewtopic.php?t=8046
*/

using namespace std;

    int vdiOpen(VDIFile *f, char* fn) {
        //cout<<"vdi open"<<endl;
        f->fd = open(fn, O_RDWR);
        return f->fd;
    }

    void vdiClose(VDIFile *f){
        //cout<<"vdi close started"<<endl;
        int i = close(f->fd);
        if(i == 0){
            cout<<"successfully closed"<<endl;
        }
        else if(i==-1) cout<<"error"<<endl;
    }

    off_t vdiSeek(VDIFile* f,off_t offset,int whence){

        //cout<<"vdi seek started"<<endl;

        off_t location;
        location = lseek(f->fd, offset, SEEK_SET); //seek header. returns -1 if error
        //cout<<"location: "<<location<<endl;
        if(location < 0){ //error during header seek
            cout<<"error in lseek()"<<endl;
            return -1;
        }

        //determine whence and set cursor for vdi file
        if (whence == SEEK_SET){
            //cout<<"whence == SEEK SET" << endl;
            f->cursor = location;
        }
        else if (whence == SEEK_CUR){
            //cout<<"whence == SEEK CUR" << endl;
            location = lseek(f->fd, offset, whence); //seek header. returns -1 if error
            f->cursor += offset;
        }
        else if (whence == SEEK_END){
            //cout<<"whence == SEEK END" << endl;
            f->cursor = f->fs + offset;
        }
        return f->cursor;
    }


    ssize_t vdiRead(VDIFile *f,void *buf,ssize_t n){
        //cout<<"vdi read started"<<endl;
        //cout << '[' << f->fd << ']' << endl;
        ssize_t bytes = read(f->fd, buf, n);
        //cout<<"Bytes requested: "<<n<<endl<<"Bytes returned: "<<bytes<<endl;
        return bytes;
    }

    ssize_t vdiWrite(VDIFile *f,void *buf,ssize_t n){

        if (n == 0) {
            return 0;
        }
        else if (n > INT_MAX) {
            return -1;
        }
        else {
            return n;
        }
}

    struct Ext2fs *ext2Open(char *fn){
        //cout<<"ext2open started"<<endl;
        char buf;
        Ext2fs* e;

       // vdiOpen(e->f,fn);
        //cout<<"vdi seek called"<<endl;
        vdiSeek(e->f, 446, SEEK_SET);
        vdiRead(e->f, &buf, 16);

        //cout<<"ext2open here"<<endl;

        if (buf == 0x83) {
            e->cursor = buf *512;

        }
        else {
            vdiSeek(e->f, 462, SEEK_SET);
            vdiRead(e->f, &buf, 16);}
        if (buf == 0x83){
            e->cursor = buf *512;

        }
        else {
            vdiSeek(e->f, 478, SEEK_SET);
            vdiRead(e->f, &buf, 16);}
        if (buf == 0x83){
            e->cursor = buf *512;

        }
        else {
            vdiSeek(e->f, 494, SEEK_SET);
            vdiRead(e->f, &buf, 16);}
        if (buf == 0x83){
            e->cursor = buf *512;

        }
        vdiSeek(e->f, 1024+e->cursor, SEEK_SET);
        vdiRead(e->f, &buf, e->Mystruct);
        e->sector = buf;

        //cout<<"ext2open completed"<<endl;


        return e;
    }

    int ReadSuperBlock(VDIFile *f, int location, SuperBlock& sb){
        cout<<"read super block started."<<endl;
        off64_t offset = vdiSeek(f,location, SEEK_SET);
        if(offset<0) cout<<"sb offset error"<<endl;
        cout<<"size of sb: "<<sizeof(sb)<<endl;
        int superbloc = vdiRead(f,&sb, sizeof(sb));
        return superbloc;
    }

    int ReadBootRecord(VDIFile *f, BootRecord& br){
        cout<<"read boot record started"<<endl;
        off64_t offset = vdiSeek(f, f->header.OffsetData, SEEK_SET);
        if(offset<0) cout<<"boot record offset error"<<endl;
        cout<<"br size: "<<sizeof(br)<<endl;
        int bootrecord = vdiRead(f,&br, sizeof(br));
        return bootrecord;
    }

    int ReadGroupDescriptor(VDIFile *f, unsigned int bs, unsigned int gcount, BlockGroupDescriptor bgd[]){
        cout<<"read group descriptor started"<<endl;
        off64_t offset = vdiSeek(f, f->header.OffsetData + bs, SEEK_SET);
        if(offset<0) cout<<"Boot Record Offset error"<<endl;
        int blockGroupDescriptor = vdiRead(f,&bgd,sizeof(bgd)*gcount);
        return blockGroupDescriptor;
    }

   uint8_t* FetchBlock(VDIFile *f, unsigned int bnum, uint8_t* buff, int blocation, unsigned int bsize){
        cout<<"fetching block"<<endl;
        int seekNum, currentBlock;
        seekNum = (bnum * bsize) + blocation;
        off64_t offset = vdiSeek(f, seekNum, SEEK_SET);
        currentBlock = vdiRead(f, buff, bsize);
        return buff;
    }

    Inode FetchInode(VDIFile *f, int inodeNum, BlockGroupDescriptor bgd[], SuperBlock sb, unsigned int bsize, int fsystemstart){
        cout<<"Fetching Inode"<<endl;
        Inode inode;
        inodeNum--;
        Inode* inodebuff = new Inode[bsize];
        //blockgroup=i/inodes per block group
        unsigned int blockgroup = inodeNum/sb.inodes_per_bgroup;
        //blockGroupNum=i%inodes per block group
        unsigned int blockgroupNum = inodeNum%sb.inodes_per_bgroup;
        //b= i/inodes per block(calculate this)(bsize/inodessize)
        unsigned int inodesPerBlock = bsize/sizeof(Inode);
        unsigned int blockNum = blockgroupNum/inodesPerBlock;
        //i=bg num%inodes perblock
        inodeNum = blockgroupNum%inodesPerBlock;
        int bnum = bgd[blockgroup].inodeTableStart+blockNum;
        FetchBlock(f, bnum, (uint8_t*) inodebuff, fsystemstart, bsize);
        //return inodearray[i]
        return inodebuff[inodeNum];
   }

    void ext2Close(){
        Ext2fs *e;
        close(e->f->fd);
    }

    void FetchFromFile( unsigned int k){
  /*      ipb <- blocksize/4

        if k < 12 {
            base ← inode.i_block
            goto direct}
        else if k < 12 + ipb {
            base ← inode.i_block + 12
            k ← k − 12
            goto single}
        else if k < 12 + ipb + ipb^2 {
            base ← inode.i_block + 13
            k ← k − 12 − ipb
            goto double}
        else{
            base ← inode.i_block + 14
            k ← k − 12 − ipb − ipb^2}
        end if
*/
    /*
        process triple stuff
        b=base[k/ipb^3]
        fetchBlock(b,buff)
        base = buff;
        k%=ipb^3
        double:
        b=base[k/ipb^2]
        FetchBlock(b,buff)
        base=buff
        k%=ipb^2
        single:

        b=base[k/ipb]
        FetchBlock(b)
        base=buff
        k%=ipb
        direct:
        fetchBlock(base[k])
        done
    */

   }

    void PrintSuperBlock(SuperBlock &sb){
        cout<<"-----SuperBlock Specs-----"<<endl;
        printf("Inode Count: %-u\n"
               "Block Count: %-u\n"
               "Reserved Block Count: %-u\n"
               "Unused Blocks: %-u\n"
               "Unused Inodes: %-u\n"
               "Block Number: %-u\n"
               "Block Size: %-u\n"
               "Blocks Per Group: %-u\n"
               "Inodes Per Group: %-u\n"
               "System State: %-u\n"
               "Magic Number: %-x\n",
               sb.inode_count, sb.block_count, sb.res_block_count, sb.unused_block_count,
               sb.unused_inode_count, sb.block_number, sb.block_size, sb.blocks_per_bgroup,
               sb.inodes_per_bgroup, sb.filesys_state, sb.ext2_magic);
    }

    void PrintBlockGroupDescriptor(BlockGroupDescriptor &bgd){
        cout<<"-----Block Group Descriptor Specs-----"<<endl;
        printf("BitMap Block: %-u\n"
               "BitMap Inode: %-u\n"
               "Inode Table Start: %-u\n"
               "Free Block Count: %-u\n"
               "Free Inode Count: %-u\n"
               "Directory Count: %-u\n"
               "BG Pad: %-u\n",
               bgd.bitmapBlock, bgd.bitmapInode, bgd.inodeTableStart, bgd.freeBlockCount,
               bgd.freeInodesCount, bgd.directoryCount, bgd.bg_pad);
    }

    /*void PrintBootRecord(BootRecord &br){
        cout<<"-----Boot Record Specs-----"<<endl;
        printf("BitMap Block: %-u\n"
               "BitMap Inode: %-u\n"
               "Inode Table Start: %-u\n"
               "Free Block Count: %-u\n"
               "Free Inode Count: %-u\n"
               "Directory Count: %-u\n"
               "BG Pad: %-u\n",
               );
    }*/

    /*
        free bitmap
        allocate chunk of mem 1 block per clok croup
        calculate num of bg not sotred in sb
        nsize=bs*bg
        intialize all to clear (mae sure all 0s)
        to mark an item wether inode
        take itemnum (if inode -1) / items per bgroup
        find quotient
        find remainder - where in bg you are marking
        bit you want = remainder/8
        remainder % 8 is byte you want
        left shift bit you want
        byte you want array[group*bs]
    */

    int main() {
    char* fn = "C:\\Users\\James\\Documents\\YSU\\Operating Systems\\OS_Project\\VDI Test Files\\Good\\Test-fixed-1k.vdi";
    //open vdi file
    VDIFile *f = new VDIFile();
    int vdifile = vdiOpen(f, fn);

    //read vdi header
    off_t offset = vdiSeek(f,0,SEEK_SET);
    ssize_t vdiHead = vdiRead(f, &(f->header), sizeof(f->header));
    cout<<"Header Signature: "<<f->header.signature<<endl<<"Header Size: "<<sizeof(f->header)<<endl;

    //Read Master Boot Record
    BootRecord bootrec;
    int br = ReadBootRecord(f, bootrec);

    SuperBlock supblock;
    //Superblock is always located at byte 1024 from the beginning of the volume and is exactly 1024 bytes in length
    int sb_location = f->header.OffsetData+bootrec.part[0].absSector * 512 + 1024;
    //sb is 1024 from file start
    int filestart = sb_location - 1024;
    int sb_read = ReadSuperBlock(f, sb_location, supblock);

    //output sector numbers for debugging
    cout<<hex<<"Absolute Sector: "<<bootrec.part[0].absSector<< endl << "Number Sector: "<<bootrec.part[0].numSector<<endl;
    //check file system state store in SB
    PrintSuperBlock(supblock);
    cout<<"supblock filesys state: "<<supblock.filesys_state<<endl;
    if(supblock.filesys_state==1) cout<<"File system state is safe."<<endl;
    else if (supblock.filesys_state==2)cout<<"File system state is not safe."<<endl;
    else {cout<<"SB incorrect or error."<<endl;}

    //Read Block Group Descriptor Table
    unsigned int groupTotal = (supblock.block_count - supblock.block_number) / supblock.blocks_per_bgroup; //formula from class
    unsigned int remain = (supblock.block_count - supblock.block_number) % supblock.blocks_per_bgroup; //remainder
    //check value of remainder
    if(remain > 0){
        //if there is a remainder then round up
        groupTotal++;
    }
    cout<<"group total: " <<groupTotal<<endl;
    //create Block Group Descriptor
    BlockGroupDescriptor bgdescriptor[groupTotal];
    //calculate block size
    unsigned int bsize = 1024 << supblock.block_size;
    unsigned int bgd_location = sb_location+bsize;
    int bgd_read = ReadGroupDescriptor(f, bgd_location, groupTotal, bgdescriptor);

    //IN PROGRESS
    cout<<"-----in progress code-----"<<endl;
    //fetch inode
    for(int i = 1; i<supblock.first_inode;i++){
        Inode iNode= FetchInode(f, i, bgdescriptor, supblock, bsize, filestart);
    }

    //check group total value

    //freeblock bitmap
    /*
        g=entity#/epg
        e%=epg

        setbit(mcp[g*block size+e/8],e%8)
        #define setbit(a,b){(a)|=(a<<(b));}
    */

    /*cout<<"ext2open called"<<endl;
    ext2Open(fn);
    ext2Close();*/

    vdiClose(f);
    cout<<"vdi close complete"<<endl;

    return 0;
}
