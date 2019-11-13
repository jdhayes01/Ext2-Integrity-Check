#pragma once
#ifndef EXT2_H_INCLUDED
#define EXT2_H_INCLUDED

  struct Ext2fs{

        VDIFile *f = new VDIFile;

        
		off_t cursor;
        char sector;
        unsigned char Mystruct = sizeof(SuperBlock_Struct);


        struct SuperBlock_Struct{
            Ext2fs* e;

            off_t Scursor = 1024 + e->cursor;
        };
  };


#endif // EXT2_H_INCLUDED
