////////////////////////////////////////////////////////////////////////////////
//                                                                             /
// 2012-2017 (c) Baical                                                        /
//                                                                             /
// This library is free software; you can redistribute it and/or               /
// modify it under the terms of the GNU Lesser General Public                  /
// License as published by the Free Software Foundation; either                /
// version 3.0 of the License, or (at your option) any later version.          /
//                                                                             /
// This library is distributed in the hope that it will be useful,             /
// but WITHOUT ANY WARRANTY; without even the implied warranty of              /
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU           /
// Lesser General Public License for more details.                             /
//                                                                             /
// You should have received a copy of the GNU Lesser General Public            /
// License along with this library.                                            /
//                                                                             /
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// This header file describe all packets types which are used in communication /
// N.B.: In this file I remove all checks for valid pointers and packets       /
//       because this job is made outside by packet pool.                      /
////////////////////////////////////////////////////////////////////////////////
#ifndef TPACKETS_H
#define TPACKETS_H

#define CLIENT_PROTOCOL_VERSION                                         (0x0007)

//this value can store 32 bits but to avoid overflow we reduce it
#define TPACKET_MAX_ID                                               (0xFFFFFFF) 
#define TPACKET_PROCESS_NAME_MAX_LEN                                        (96) 


////////////////////////////////////////////////////////////////////////////////
//Transport Packet types, we have only 4 bits for them (16 types max)
enum eTPacket_Type
{
    ETPT_CLIENT_HELLO             = 0x0,    //tH_Common + tH_Client_Initial
    ETPT_CLIENT_PING              = 0x1,    //tH_Common
    ETPT_CLIENT_DATA              = 0x2,
    ETPT_CLIENT_DATA_REPORT       = 0x3,
    ETPT_CLIENT_BYE               = 0x4,
    
    ETPT_ACKNOWLEDGMENT           = 0x9,
    ETPT_SERVER_DATA_REPORT       = 0xA,
    ETPT_SERVER_CHAINED           = 0xB,

    ETPACKET_TYPE_UNASSIGNED      = 0xF
};

enum eTPacket_Endian
{
    ETP_LITTLE_ENDIAN   = 0,
    ETP_BIG_ENDIAN         ,
    ETP_MAX_ENDIAN
};

#define TPACKET_MIN_SIZE                                                 (0x200) 
#define TPACKET_MAX_SIZE                                                (0xFF00) 

////////////////////////////////////////////////////////////////////////////////
//Transport Packet flags
#define TPACKET_FLAG_EXTRA_DATA                                         (0x001u) // 1 << 0
#define TPACKET_FLAG_ARCH_64                                            (0x002u) // 1 << 1
#define TPACKET_FLAG_RESERVED_02                                        (0x004u) // 1 << 2
#define TPACKET_FLAG_BIG_ENDIAN_SRV                                     (0x008u) // 1 << 3
#define TPACKET_FLAG_RESERVED_04                                        (0x010u) // 1 << 4
#define TPACKET_FLAG_RESERVED_05                                        (0x020u) // 1 << 5
#define TPACKET_FLAG_RESERVED_06                                        (0x040u) // 1 << 6
#define TPACKET_FLAG_RESERVED_07                                        (0x080u) // 1 << 7
#define TPACKET_FLAG_RESERVED_08                                        (0x100u) // 1 << 8
#define TPACKET_FLAG_RESERVED_09                                        (0x200u) // 1 << 9
#define TPACKET_FLAG_RESERVED_10                                        (0x400u) // 1 << 10
#define TPACKET_FLAG_BIG_ENDIAN_CLN                                     (0x800u) // 1 << 11

#define TPACKET_TYPE_MASK                                                 (0xFu)
#define TPACKET_FLAGS_MASK                                             (0xFFF0u)
#define TPACKET_FLAGS_OFFSET                                                (4u)


////////////////////////////////////////////////////////////////////////////////
//Transport Packet structures


PRAGMA_PACK_ENTER(2) //alignment is now 2, MS only !

//This is common header for every P7 transport packet, it situated at the 
//beginning of the packet, then, depending on Type can be located other headers
struct sH_Common
{
    tUINT32  dwCRC32;
    tUINT32  dwID;
    tUINT16  wBits; //[0..3] - Type, [4..15] - flags
    //tUINT16  wType:4;
    //tUINT16  wFlags:12;
    //We are working through UDP, so the packet size is limited to 65k. 
    //The minimum guaranteed size of the packet 576 bytes, user size is 512 bytes.
    tUINT16  wSize; 
    tUINT16  wClient_ID;
} ATTR_PACK(2);

struct sH_Client_Hello
{
    tUINT16  wProtocol_Version;
    tUINT16  wData_Max_Size;
    //Process ID and process time are dedicated to matching processes on server
    //side
    tUINT32  dwProcess_ID;
    tUINT32  dwProcess_Start_Time_Hi;
    tUINT32  dwProcess_Start_Time_Lo;
    tWCHAR   pProcess_Name[TPACKET_PROCESS_NAME_MAX_LEN];
} ATTR_PACK(2);


struct sH_Data_Window
{
    tUINT32 dwFirst_ID;
    tUINT32 dwLast_ID;
} ATTR_PACK(2);


struct sH_Packet_Ack
{
    tUINT32  dwSource_ID;
    tUINT16  wResult; // 0 - NOK, 1 - OK
} ATTR_PACK(2);


struct sH_Data_Window_Report
{
    tUINT32 dwSource_ID;
    tUINT8  pID[1]; //Array of the ID, each byte contains 8 ID markers ... bitfield
} ATTR_PACK(2);


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                        User packet  definitions                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// This defines has been moved to P7_Client.h to be visible from outside the 
// library
// #define USER_PACKET_CHANNEL_ID_BITS_COUNT                                    (5)
// #define USER_PACKET_SIZE_BITS_COUNT       (32-USER_PACKET_CHANNEL_ID_BITS_COUNT)
// #define USER_PACKET_MAX_SIZE                  (1 << USER_PACKET_SIZE_BITS_COUNT)
// #define USER_PACKET_CHANNEL_ID_MAX_SIZE (1 << USER_PACKET_CHANNEL_ID_BITS_COUNT)

#define INIT_USER_HEADER(iSize, iChannel_Id) (iSize | (iChannel_Id << USER_PACKET_SIZE_BITS_COUNT))
#define GET_USER_HEADER_SIZE(iBits) (iBits & ((1 << USER_PACKET_SIZE_BITS_COUNT) - 1))
#define GET_USER_HEADER_CHANNEL_ID(iBits) ((iBits >> USER_PACKET_SIZE_BITS_COUNT) & ((1 << USER_PACKET_CHANNEL_ID_BITS_COUNT) - 1))

struct sH_User_Raw //user data header
{
    tUINT32 dwBits;
} ATTR_PACK(2);

struct sH_User_Data //user data header, map for sH_User_Raw
{
    tUINT32 dwSize       :USER_PACKET_SIZE_BITS_COUNT;       //<< 28 bits for Size
    tUINT32 dwChannel_ID :USER_PACKET_CHANNEL_ID_BITS_COUNT; //<< 5  bits for ChannelId
} ATTR_PACK(2);

GASSERT(sizeof(sH_User_Data) == sizeof(sH_User_Raw));

PRAGMA_PACK_EXIT()

#endif //TPACKETS_H
