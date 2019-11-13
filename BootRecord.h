#ifndef BOOTRECORD_H_INCLUDED
#define BOOTRECORD_H_INCLUDED

    struct __attribute__ ((packed)) PartitionTable{ //for partition table used in BootRecord
        unsigned char unused[8];
        unsigned int absSector; //absolute sector
        unsigned int numSector; //number sector

    };

    struct __attribute__ ((packed)) BootRecord{//struct to represent master boot record
        unsigned char unused[446];
        PartitionTable part[4]; //array of 4 partition tables. 16 bytes long each
        unsigned short buff;
    };

#endif // BOOTRECORD_H_INCLUDED
