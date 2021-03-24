#include <libTS38/headers/TS_38212_RL16_Multiplexing_channelcoding.h>
//#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>
//#include <libTS38/headers/TS_38212_nr_polar_kronecker_power_matrices.h>
//#include <libNFAPI/headers/FAPI_SCF_222.h>


//#include <cstring>
//#include <stdio.h>
//#include <iostream>
//#include <cstdint>
//#include <cstring>
#include <math.h>



/*******************************************************************************************************************************************
TS 38_212
7.1 Broadcast channel
Data arrives to the coding unit in the form of a maximum of one transport block every 80ms. The following coding
steps can be identified:
- Payload generation
- Scrambling
- Transport block CRC attachment
- Channel coding
- Rate matching

********************************************************************************************************************************************/


/*******************************************************************************************************************************************

TS_38212  7.1.1  PBCH payload generation
TS_38212  Table 7.1.1-1: Value of PBCH payload interleaver pattern

********************************************************************************************************************************************/
//TS_38212  Table 7.1.1-1: Value of PBCH payload interleaver pattern
uint8_t nr_pbch_payload_interleaving_pattern[32] = { 16, 23, 18, 17, 8, 30, 10, 6, 24, 7, 0, 5, 3, 2, 1, 4,
                                                     9, 11, 12, 13, 14, 15, 19, 20, 21, 22, 25, 26, 27, 28, 29, 31
                                                   };

// help func: updates interleav table
// input: pointer to interleav table
void nr_init_pbch_interleaver(uint8_t* interleaver)
{
    uint8_t j_sfn = 0, j_hrf = 10, j_ssb = 11, j_other = 14;
    memset((void*)interleaver, 0, NR_POLAR_PBCH_PAYLOAD_BITS);

    for (uint8_t i = 0; i < NR_POLAR_PBCH_PAYLOAD_BITS; i++)
        if (!i) // choice bit:1
        {
            *(interleaver + i) = *(nr_pbch_payload_interleaving_pattern + j_other++);
        }
        else if (i < 7) //Sfn bits:6
        {
            *(interleaver + i) = *(nr_pbch_payload_interleaving_pattern + j_sfn++);
        }

        else if (i < 24) // other:17
            *(interleaver + i) = *(nr_pbch_payload_interleaving_pattern + j_other++);
        else if (i < 28) // Sfn:4
            *(interleaver + i) = *(nr_pbch_payload_interleaving_pattern + j_sfn++);
        else if (i == 28) // Hrf bit:1
            *(interleaver + i) = *(nr_pbch_payload_interleaving_pattern + j_hrf);
        else // Ssb bits:3
            *(interleaver + i) = *(nr_pbch_payload_interleaving_pattern + j_ssb++);
}

// final fun: TS_38212  Table 7.1.1-1: Value of PBCH payload interleaver pattern - generates payload and do interleaving
// input: bchPayload - MIB payload (from L2), where the 24 MSB are used and represent the MIB in [38.331 MIB IE] and represent
//        Lmax - number of ssb in frame
//        ssb_index - index of transmitted ssb
//        n_hf - if hf is used
//        amp
//  output: return pbch_a_interleaved - generate payload? interleaved
uint32_t PBCH_payload_generation(uint32_t bchPayload,
                                 uint8_t Lmax,
                                 uint32_t Kssb,
                                 uint8_t ssb_index,
                                 int sfn,
                                 uint8_t n_hf,
                                 int16_t amp
                                )
{
    uint8_t  pbch_pdu = (uint8_t)bchPayload;
    uint8_t*  pbch_pdu_p = &pbch_pdu;
    uint32_t pbch_a;  // a from 3Gpp
    uint32_t pbch_a_interleaved;
    uint32_t pbch_a_prime;
    uint32_t pbch_e[NR_POLAR_PBCH_E_DWORD];
    uint8_t nr_pbch_interleaver[NR_POLAR_PBCH_PAYLOAD_BITS];


    //инициализируем 24 бита - из pbch_pdu
    for (int i = 0; i < NR_PBCH_PDU_BITS; i++)
        pbch_a |= ((pbch_pdu_p[i >> 3] >> (7 - (i & 7))) & 1) << i;

    //// Extra byte generation
    for (int i = 0; i < 4; i++)
        pbch_a |= ((sfn >> (3 - i)) & 1) << (24 + i); // resp. 4th, 3rd, 2nd ans 1st lsb of sfn

    pbch_a |= n_hf << 28; // half frame index bit


    if (Lmax == 64)
        for (int i = 0; i < 3; i++)
            pbch_a |= ((ssb_index >> (5 - i)) & 1) << (29 + i); // resp. 6th, 5th and 4th bits of ssb_index
    else
        pbch_a |= ((Kssb >> 4) & 1) << 29; //MSB of k_SSB (bit index 4)

    // Payload interleaving
    nr_init_pbch_interleaver(&nr_pbch_interleaver[0]);
    pbch_a_interleaved = 0;
    for (int i = 0; i < NR_POLAR_PBCH_PAYLOAD_BITS; i++)
    {
        pbch_a_interleaved |= ((pbch_a >> i) & 1) << (nr_pbch_interleaver[i]);
    }

    return pbch_a_interleaved;

}


/*******************************************************************************************************************************************

-- TS_38212  7.1.2  Scrambling
-- For PBCH transmission in a frame, the bit sequence:   a 0 , a 1 , a 2 , a 3 ,. . ., a A−1
-- is scrambled into a bit sequence a' 0 ,a' 1 ,a' 2 ,a' 3 ,...,a' A−1 ,
-- where a ' i = ( a i +s i ) mod 2 for i=0,1,..., A−1 and s 0 , s 1 ,s 2 ,s 3 ,...,s A−1 is generated according to the following:
-- c(i) is given by Clause 5.2.1of [4, TS38.211] and initialized with c init = Nid

********************************************************************************************************************************************/
// final fun: TS_38212  7.1.2  Scrambling
// input: bchPayloadInterleaved - a from TS
//        length - number of input bits
//        encoded
//        amp
//  output: a'
uint32_t nr_pbch_scrambling( uint32_t bchPayloadInterleaved, // input
                             uint32_t pci,
                             uint8_t Lmax,
                             uint16_t length,
                             //uint8_t encoded,
                             int sfn
                           )
{
    uint8_t reset, offset, nushift;
    uint32_t x1, x2, s = 0, unscrambling_mask, bchPayloadScrambled, M;
    uint32_t pbch_a_prime; ///after scrampling
    uint32_t pbch_e[NR_POLAR_PBCH_E_DWORD];
    //uint32_t* pbch_e = pbch->pbch_e;
    reset = 1;
    // x1 is set in lte_gold_generic
    x2 = (uint32_t)pci;

    // Scrambling
    unscrambling_mask = (Lmax == 64) ? 0x100006D : 0x1000041;
    M = (Lmax == 64) ? (NR_POLAR_PBCH_PAYLOAD_BITS - 6) : (NR_POLAR_PBCH_PAYLOAD_BITS - 3);
    nushift = (((sfn >> 2) & 1) << 1) ^ ((sfn >> 1) & 1); // from Ts 3812 Table 7.1.2-1: (3rd LSB of SFN, 2nd LSB of SFN)
    bchPayloadScrambled = 0;

    // The Gold sequence is shifted by nushift* M, so we skip (nushift*M /32) double words
    for (int i = 0; i < (uint16_t)ceil(((float)nushift * M) / 32); i++)
    {
        s = lte_gold_generic(&x1, &x2, reset); //5.2.1 Pseudo-random sequence generation
        reset = 0;
    }
    // Scrambling is now done with offset (nushift*M)%32
    offset = (nushift * M) & 0x1f;
    int k = 0;

    if (1)
    {
        for (int i = 0; i < length; ++i)
        {
            if ((unscrambling_mask >> i) & 1)
            {
                pbch_a_prime ^= ((bchPayloadInterleaved >> i) & 1) << i;
            }
            else
            {
                if (((k + offset) & 0x1f) == 0)
                {
                    s = lte_gold_generic(&x1, &x2, reset);
                    reset = 0;
                }

                pbch_a_prime ^= (((bchPayloadInterleaved >> i) & 1) ^ ((s >> ((k + offset) & 0x1f)) & 1)) << i;
                k++;
            }
        }
    }

    return pbch_a_prime;
}


/*******************************************************************************************************************************************************************

--  TS_38212  7.1.3   Transport block CRC attachment
--  Error detection is provided on BCH transport blocks through a Cyclic Redundancy Check (CRC).
--  The entire transport block is used to calculate the CRC parity bits.
--  The input bit sequence is denoted by  a' 0 ,a' 1 ,a' 2 ,a' 3 ,...,a' A−1 ,
--  and the parity bits by p 0 , p 1 , p 2 , p 3 , .. . , p L−1 ,
--  where A is the payload size and L is  the number of parity bits.
--  gCRC24C(D) , L=24 used dor PBCH

*******************************************************************************************************************************************************************/

/// final function TS_38212  7.1.3   Transport block CRC attachment
void nr_PBCH_tb_crc_attach( uint64_t* inputBit,   // pointer to inbut bit sequence
                            //uint64_t crcBit,    //
                            int bitlen,           // original bit len
                            //int crcParityBitsN,   // L=24 for pbch crc calc
                            uint64_t* output      // pointer to output(data+crc)
                          )
{

    int n,K;
    int crcParityBitsN = 24;
    uint64_t crcBit =  crc_calculate( inputBit, //array after scrampling  a'
                                   0,//crcmask, // set 0
                                   bitlen,  //origBitlength,
                                   crcParityBitsN,   // crcParityBitsN - for crc pbch crc24c? L=24
                                   0
                                     );

    K = bitlen + crcParityBitsN;

    //crcBit = crc_calculate()
    // K - 24 Бита CRC + 32 бита данных = 56
    // this is number of quadwords in the bit string
    int quadwlen = (K >> 6);
    if ((K & 63) > 0)
    {
        quadwlen++;
    }
    // Create the B bit string as
    // 0, 0, ..., 0, a'_0, a'_1, ..., a'_A-1, p_0, p_1, ..., p_{N_parity-1}
    //??? b_{N'-1} b_{N'-2} ... b_{N'-A} b_{N'-A-1} ... b_{N'-A-Nparity} = a_{N-1} a_{N-2} ... a_{N-A} p_{N_parity-1} ... p_0
    for (n = 0; n < quadwlen; n++)
    {
        if (n == 0)
        {
            output[n] = (inputBit[n] << crcParityBitsN) | crcBit;
        }
        else
        {
            output[n] = (inputBit[n] << crcParityBitsN) | (inputBit[n - 1] >> (64 - crcParityBitsN));
        }
    }

}

// univercal function, attach crc
void nr_tb_crc_attach( uint64_t* inputBit,   // pointer to inbut bit sequence
                            uint64_t crcBit,    //
                            int bitlen,           // original bit len
                            int crcParityBitsN,   // L=24 for pbch crc calc
                            uint64_t* output      // pointer to output(data+crc)
                          )
{

    int n,K;


    K = bitlen + crcParityBitsN;

    //crcBit = crc_calculate()
    // K - 24 Бита CRC + 32 бита данных = 56
    // this is number of quadwords in the bit string
    int quadwlen = (K >> 6);
    if ((K & 63) > 0)
    {
        quadwlen++;
    }

    // Create the B bit string as
    // 0, 0, ..., 0, a'_0, a'_1, ..., a'_A-1, p_0, p_1, ..., p_{N_parity-1}
    //??? b_{N'-1} b_{N'-2} ... b_{N'-A} b_{N'-A-1} ... b_{N'-A-Nparity} = a_{N-1} a_{N-2} ... a_{N-A} p_{N_parity-1} ... p_0
    for (n = 0; n < quadwlen; n++)
    {
        if (n == 0)
        {
            output[n] = (inputBit[n] << crcParityBitsN) | crcBit;
        }
        else
        {
            output[n] = (inputBit[n] << crcParityBitsN) | (inputBit[n - 1] >> (64 - crcParityBitsN));
        }
    }

}

/********************************************************************************************************************************************************************

TS_38212  5.1  CRC calculation
crc_calculate() - final function

*********************************************************************************************************************************************************************/

unsigned int             poly24a = 0x864cfb00;   // 1000 0110 0100 1100 1111 1011  // D^24 + D^23 + D^18 + D^17 + D^14 + D^11 + D^10 + D^7 + D^6 + D^5 + D^4 + D^3 + D + 1
unsigned int             poly24b = 0x80006300;   // 1000 0000 0000 0000 0110 0011  // D^24 + D^23 + D^6 + D^5 + D + 1
unsigned int             poly24c = 0xb2b11700;   // 1011 0010 1011 0001 0001 0111  // D^24+D^23+D^21+D^20+D^17+D^15+D^13+D^12+D^8+D^4+D^2+D+1
unsigned int             poly16 = 0x10210000;    // 0001 0000 0010 0001            D^16 + D^12 + D^5 + 1
unsigned int             poly12 = 0x80F00000;    // 1000 0000 1111                 D^12 + D^11 + D^3 + D^2 + D + 1
unsigned int             poly8 = 0x9B000000;     // 1001 1011                      D^8  + D^7  + D^4 + D^3 + D + 1
uint32_t                 poly6 = 0x84000000;     // 10000100000... -> D^6+D^5+1
uint32_t                 poly11 = 0xc4200000;    //11000100001000... -> D^11+D^10+D^9+D^5+1

// makes crc for 8bits - help function
unsigned int crcbit (unsigned char * inputptr,
                     int octetlen,
                     unsigned int poly)
{
    unsigned int i, crc = 0, c;

    while (octetlen-- > 0)
    {
        c = (*inputptr++) << 24;

        for (i = 8; i != 0; i--)
        {
            if ((1 << 31) & (c ^ crc))
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;

            c <<= 1;
        }
    }
    return crc;
}

// tables for fast crc
// 256 - число возможных значений;
// ссылк на теорию: https://soltau.ru/index.php/themes/dev/item/461-kak-poschitat-kontrolnuyu-summu-crc32-crc16-crc8
// https://programm.ws/page.php?id=665
static unsigned int        crc24aTable[256];
static unsigned int        crc24bTable[256];
static unsigned int        crc24cTable[256];
static unsigned short      crc16Table[256];
static unsigned short      crc12Table[256];
static unsigned short      crc11Table[256];
static unsigned char       crc8Table[256];
static unsigned char       crc6Table[256];

void crcTableInit(void)
{
    unsigned char c = 0;

    do
    {
        crc24aTable[c] = crcbit (&c, 1, poly24a);
        crc24bTable[c] = crcbit (&c, 1, poly24b);
        crc24cTable[c] = crcbit (&c, 1, poly24c);
        crc16Table[c] = (unsigned short) (crcbit (&c, 1, poly16) >> 16);
        crc12Table[c] = (unsigned short) (crcbit (&c, 1, poly12) >> 16);
        crc11Table[c] = (unsigned short) (crcbit (&c, 1, poly11) >> 16);
        crc8Table[c] = (unsigned char) (crcbit (&c, 1, poly8) >> 24);
        crc6Table[c] = (unsigned char) (crcbit (&c, 1, poly6) >> 24);
    }
    while (++c);
}

//Byte by byte implementations,
//assuming initial byte is 0 padded (in MSB) if necessary
unsigned int crc24a (unsigned char * inptr,
                     int bitlen)
{

    int             octetlen, resbit;
    unsigned int             crc = 0;
    octetlen = bitlen / 8;        /* Change in octets */
    resbit = (bitlen % 8);

    while (octetlen-- > 0)
    {
        //   printf("crc24a: in %x => crc %x\n",crc,*inptr);
        crc = (crc << 8) ^ crc24aTable[(*inptr++) ^ (crc >> 24)];
    }

    if (resbit > 0)
        crc = (crc << resbit) ^ crc24aTable[((*inptr) >> (8 - resbit)) ^ (crc >> (32 - resbit))];

    return crc;
}

unsigned int crc24b (unsigned char * inptr,
                     int bitlen)
{
    int octetlen, resbit;
    unsigned int crc = 0;
    octetlen = bitlen / 8;        /* Change in octets */
    resbit = (bitlen % 8);

    while (octetlen-- > 0)
    {
        //    printf("crc24b: in %x => crc %x (%x)\n",crc,*inptr,crc24bTable[(*inptr) ^ (crc >> 24)]);
        crc = (crc << 8) ^ crc24bTable[(*inptr++) ^ (crc >> 24)];
    }

    if (resbit > 0)
        crc = (crc << resbit) ^ crc24bTable[((*inptr) >> (8 - resbit)) ^ (crc >> (32 - resbit))];

    return crc;
}

unsigned int crc24c (unsigned char * inptr,
                     int bitlen)
{
    int octetlen, resbit;
    unsigned int crc = 0;
    octetlen = bitlen / 8;        /* Change in octets */
    resbit = (bitlen % 8);

    while (octetlen-- > 0)
    {
        crc = (crc << 8) ^ crc24cTable[(*inptr++) ^ (crc >> 24)];
    }

    if (resbit > 0)
    {
        crc = (crc << resbit) ^ crc24cTable[((*inptr) >> (8 - resbit)) ^ (crc >> (32 - resbit))];
    }

    return crc;
}

unsigned int crc16 (unsigned char * inptr, int bitlen)
{
    int             octetlen, resbit;
    unsigned int             crc = 0;
    octetlen = bitlen / 8;        /* Change in octets */
    resbit = (bitlen % 8);

    while (octetlen-- > 0)
    {

        crc = (crc << 8) ^ (crc16Table[(*inptr++) ^ (crc >> 24)] << 16);
    }

    if (resbit > 0)
        crc = (crc << resbit) ^ (crc16Table[((*inptr) >> (8 - resbit)) ^ (crc >> (32 - resbit))] << 16);

    return crc;
}

unsigned int crc12 (unsigned char * inptr, int bitlen)
{
    int             octetlen, resbit;
    unsigned int             crc = 0;
    octetlen = bitlen / 8;        /* Change in octets */
    resbit = (bitlen % 8);

    while (octetlen-- > 0)
    {
        crc = (crc << 8) ^ (crc12Table[(*inptr++) ^ (crc >> 24)] << 16);
    }

    if (resbit > 0)
        crc = (crc << resbit) ^ (crc12Table[((*inptr) >> (8 - resbit)) ^ (crc >> (32 - resbit))] << 16);

    return crc;
}

unsigned int crc11 (unsigned char * inptr, int bitlen)
{
    int             octetlen, resbit;
    unsigned int             crc = 0;
    octetlen = bitlen / 8;        /* Change in octets */
    resbit = (bitlen % 8);

    while (octetlen-- > 0)
    {
        crc = (crc << 8) ^ (crc11Table[(*inptr++) ^ (crc >> 24)] << 16);
    }

    if (resbit > 0)
        crc = (crc << resbit) ^ (crc11Table[((*inptr) >> (8 - resbit)) ^ (crc >> (32 - resbit))] << 16);

    return crc;
}

unsigned int crc8 (unsigned char * inptr, int bitlen)
{
    int             octetlen, resbit;
    unsigned int             crc = 0;
    octetlen = bitlen / 8;        /* Change in octets */
    resbit = (bitlen % 8);

    while (octetlen-- > 0)
    {
        crc = crc8Table[(*inptr++) ^ (crc >> 24)] << 24;
    }

    if (resbit > 0)
        crc = (crc << resbit) ^ (crc8Table[((*inptr) >> (8 - resbit)) ^ (crc >> (32 - resbit))] << 24);

    return crc;
}

unsigned int crc6 (unsigned char * inptr, int bitlen)
{
    int             octetlen, resbit;
    unsigned int             crc = 0;
    octetlen = bitlen / 8;        /* Change in octets */
    resbit = (bitlen % 8);

    while (octetlen-- > 0)
    {
        crc = crc6Table[(*inptr++) ^ (crc >> 24)] << 24;
    }

    if (resbit > 0)
        crc = (crc << resbit) ^ (crc6Table[((*inptr) >> (8 - resbit)) ^ (crc >> (32 - resbit))] << 24);

    return crc;
}

// output is calculated crc, depends on bitlen and polinom, for pbch
uint64_t crc_calculate(     uint64_t *A, //pointer to array after scrampling  a'
                            //void* out,
                            int32_t crcmask, // set 0
                            int bitlen, // input seq bit number
                            int crcParityBitsN, // number of polinom bits
                            //polyType_t polyType
                            uint8_t ones_flag  // appending 24 ones before a0 for DCI as stated in 38.212 7.3.2
                      )
{

//typedef enum
//{
//    POLY24a = 0,
//    POLY24b,
//    POLY24c,
//    POLY16,
//    POLY12,
//    POLY8,
//    POLY6,
//    POLY11
//} polyType_t;

    uint64_t tcrc = 0;
    uint8_t offset = 0;
    // appending 24 ones before a0 for DCI as stated in 38.212 7.3.2
    if (ones_flag)
    {
        offset = 3;
    }

    crcTableInit();

    // A bit string should be stored as 0, 0, ..., 0, a'_0, a'_1, ..., a'_A-1,
    //???a'_{N-1} a'_{N-2} ... a'_{N-A} 0 .... 0, where N=64,128,192,..., N is smallest multiple of 64 greater than or equal to A

    // First flip A bitstring byte endian for CRC routines (optimized for DLSCH/ULSCH, not PBCH/PDCCH)
    // CRC reads in each byte in bit positions 7 down to 0, for PBCH/PDCCH we need to read in a_{A-1} down to a_{0}, A = length of bit string (e.g. 32 for PBCH)
    if (bitlen<=32)
    {
        uint8_t A32_flip[4+offset];
        if (ones_flag)
        {
            A32_flip[0] = 0xff;
            A32_flip[1] = 0xff;
            A32_flip[2] = 0xff;
        }
        uint32_t Aprime= (uint32_t)(((uint32_t)*A)<<(32-bitlen));
        A32_flip[0+offset]=((uint8_t *)&Aprime)[3];
        A32_flip[1+offset]=((uint8_t *)&Aprime)[2];
        A32_flip[2+offset]=((uint8_t *)&Aprime)[1];
        A32_flip[3+offset]=((uint8_t *)&Aprime)[0];
        if      (crcParityBitsN == 24)
        {
            tcrc = (uint64_t)(((crcmask^(crc24c(A32_flip,8*offset+bitlen)>>8)))&0xffffff);
        }
        else if (crcParityBitsN  == 11)
            tcrc = (uint64_t)(((crcmask^(crc11(A32_flip,bitlen)>>21)))&0x7ff);
        else if (crcParityBitsN == 6)
            tcrc = (uint64_t)(((crcmask^(crc6(A32_flip,bitlen)>>26)))&0x3f);
    }
    else if (bitlen<=64)
    {
        uint8_t A64_flip[8+offset];
        if (ones_flag)
        {
            A64_flip[0] = 0xff;
            A64_flip[1] = 0xff;
            A64_flip[2] = 0xff;
        }
        uint64_t Aprime= (uint64_t)(((uint64_t)*A)<<(64-bitlen));
        A64_flip[0+offset]=((uint8_t *)&Aprime)[7];
        A64_flip[1+offset]=((uint8_t *)&Aprime)[6];
        A64_flip[2+offset]=((uint8_t *)&Aprime)[5];
        A64_flip[3+offset]=((uint8_t *)&Aprime)[4];
        A64_flip[4+offset]=((uint8_t *)&Aprime)[3];
        A64_flip[5+offset]=((uint8_t *)&Aprime)[2];
        A64_flip[6+offset]=((uint8_t *)&Aprime)[1];
        A64_flip[7+offset]=((uint8_t *)&Aprime)[0];
        if (crcParityBitsN == 24)
            tcrc = (uint64_t)((crcmask^(crc24c(A64_flip,8*offset+bitlen)>>8)))&0xffffff;
        else if (crcParityBitsN == 11)
            tcrc = (uint64_t)((crcmask^(crc11(A64_flip,bitlen)>>21)))&0x7ff;
    }
    else if (bitlen<=128)
    {
        uint8_t A128_flip[16+offset];
        if (ones_flag)
        {
            A128_flip[0] = 0xff;
            A128_flip[1] = 0xff;
            A128_flip[2] = 0xff;
        }
        __uint128_t Aprime= (__uint128_t)(((__uint128_t)*A)<<(128-bitlen));
        A128_flip[0+offset]=((uint8_t*)&Aprime)[15];
        A128_flip[1+offset]=((uint8_t*)&Aprime)[14];
        A128_flip[2+offset]=((uint8_t*)&Aprime)[13];
        A128_flip[3+offset]=((uint8_t*)&Aprime)[12];
        A128_flip[4+offset]=((uint8_t*)&Aprime)[11];
        A128_flip[5+offset]=((uint8_t*)&Aprime)[10];
        A128_flip[6+offset] =((uint8_t*)&Aprime)[9];
        A128_flip[7+offset] =((uint8_t*)&Aprime)[8];
        A128_flip[8+offset] =((uint8_t*)&Aprime)[7];
        A128_flip[9+offset] =((uint8_t*)&Aprime)[6];
        A128_flip[10+offset]=((uint8_t*)&Aprime)[5];
        A128_flip[11+offset]=((uint8_t*)&Aprime)[4];
        A128_flip[12+offset]=((uint8_t*)&Aprime)[3];
        A128_flip[13+offset]=((uint8_t*)&Aprime)[2];
        A128_flip[14+offset]=((uint8_t*)&Aprime)[1];
        A128_flip[15+offset]=((uint8_t*)&Aprime)[0];
        if (crcParityBitsN  == 24)
            tcrc = (uint64_t)((crcmask^(crc24c(A128_flip,8*offset+bitlen)>>8)))&0xffffff;
        else if (crcParityBitsN  == 11)
            tcrc = (uint64_t)((crcmask^(crc11(A128_flip,bitlen)>>21)))&0x7ff;
    }

    return tcrc;
}


/*******************************************************************************************************************************************************************

--  TS_38212  7.1.4   Channel coding
--  describe that for PBCH coding is done with Polar codes (Ts38212 5.3.1)
--  Parameters for polar codes:
                                 nmax = 9
                                 I_il = 1
                                 Npc = 0
                                 Npc_wm =0
-- Polar codes steps: TS_38212  5.3.1        Polar coding
                      TS_38212  5.3.1.1      Interleaving
                      TS_38212  5.3.1.1.2    Polar encoding


*******************************************************************************************************************************************************************/

/// final function for TS_38212  7.1.4   Channel coding
void polar_encoder_PBCH( uint64_t* in,
                         uint8_t* out,
                         //uint8_t nmax,
                         //uint8_t I_il,
                         //uint8_t Npc,
                         //uint8_t Npc_wm,
                         uint16_t K,
                         uint16_t E
                        )
{

//
   polar_encoder( in,
                  out,
                  9,      //nmax,
                  1,      //I_il,
                  0,      //Npc,
                  0,      //Npc_wm,
                  K,
                  E
                );
}

/********************************************************************************************************************************************************************

TS_38212   5.3.1  Polar coding (N calc)


*********************************************************************************************************************************************************************/

// input:  K - total bits number(data+crc)
//         E - final output after rate match, from TS38212 7.1.5 E=864
//         nmax - for pbch = 9 (TS_38212  7.1.4   Channel coding)
uint32_t nr_polar_output_length(uint16_t K, //(data+crc nits number)
                                uint16_t E, // (the rate matching output sequence length as given in Clause 5.4.1)
                                uint8_t  n_max) //
// вычисляет длину финальной последовательности N - после полярных кодов
{
    uint8_t n_1, n_2, n_min = 5, n;
    double R_min = 1.0 / 8;

    if ((E <= (9.0 / 8) * pow(2, ceil(log2(E)) - 1)) && (K / E < 9.0 / 16))
    {
        n_1 = ceil(log2(E)) - 1;
    }
    else
    {
        n_1 = ceil(log2(E));
    }

    n_2 = ceil(log2(K / R_min));

    n = n_max;
    if (n > n_1)
        n = n_1;
    if (n > n_2)
        n = n_2;
    if (n < n_min)
        n = n_min;

    return ((uint32_t)pow(2.0, n)); //=polar_code_output_length
}

/*******************************************************************************************************************************************

TS_38212  5.3.1.1     Interleaving
input: c
output: c'

********************************************************************************************************************************************/


// part of TS_38212  5.3.1.1  Interleaving - заполняет таблицу интерливера
void nr_polar_interleaving_pattern(uint16_t K, uint8_t I_IL, uint16_t *PI_k_)
{
    uint8_t K_IL_max=164, k=0;
    //Table 5.3.1.1-1: Interleaving pattern
    uint8_t interleaving_pattern_table[164] = {0, 2, 4, 7, 9, 14, 19, 20, 24, 25, 26, 28, 31, 34,
                                               42, 45, 49, 50, 51, 53, 54, 56, 58, 59, 61, 62, 65, 66,
                                               67, 69, 70, 71, 72, 76, 77, 81, 82, 83, 87, 88, 89, 91,
                                               93, 95, 98, 101, 104, 106, 108, 110, 111, 113, 115, 118, 119, 120,
                                               122, 123, 126, 127, 129, 132, 134, 138, 139, 140, 1, 3, 5, 8,
                                               10, 15, 21, 27, 29, 32, 35, 43, 46, 52, 55, 57, 60, 63,
                                               68, 73, 78, 84, 90, 92, 94, 96, 99, 102, 105, 107, 109, 112,
                                               114, 116, 121, 124, 128, 130, 133, 135, 141, 6, 11, 16, 22, 30,
                                               33, 36, 44, 47, 64, 74, 79, 85, 97, 100, 103, 117, 125, 131,
                                               136, 142, 12, 17, 23, 37, 48, 75, 80, 86, 137, 143, 13, 18,
                                               38, 144, 39, 145, 40, 146, 41, 147, 148, 149, 150, 151, 152, 153,
                                               154, 155, 156, 157, 158, 159, 160, 161, 162, 163
                                              };

    if (I_IL == 0)
    {
        for (; k<= K-1; k++)
            PI_k_[k] = k;
    }
    else
    {
        for (int m=0; m<= (K_IL_max-1); m++)
        {
            if (interleaving_pattern_table[m] >= (K_IL_max-K))
            {
                PI_k_[k] = interleaving_pattern_table[m]-(K_IL_max-K);
                k++;
            }
        }
    }
}

// help fun, convert uint32 to uint8_t,
// arraySize - число информационных бит
void nr_bit2byte_uint32_8(uint32_t *in, uint16_t arraySize, uint8_t *out)
{
    uint8_t arrayInd = ceil(arraySize / 32.0);
    for (int i = 0; i < (arrayInd-1); i++)
    {
        for (int j = 0; j < 32; j++)
        {
            out[j+(i*32)] = (in[i] >> j) & 1;
        }
    }

    for (int j = 0; j < arraySize - ((arrayInd-1) * 32); j++)
        out[j + ((arrayInd-1) * 32)] = (in[(arrayInd-1)] >> j) & 1;
}
// help fun, convert uint64_t to uint8_t,
// arraySize - число информационных бит
void nr_bit2byte_uint64_8(uint64_t *in, uint16_t arraySize, uint8_t *out)
{
    uint8_t arrayInd = ceil(arraySize / 64.0);
    for (int i = 0; i < (arrayInd-1); i++)
    {
        for (int j = 0; j < 64; j++)
        {
            out[j+(i*64)] = (in[i] >> j) & 1;
        }
    }

    for (int j = 0; j < arraySize - ((arrayInd-1) * 64); j++)
        out[j + ((arrayInd-1) * 64)] = (in[(arrayInd-1)] >> j) & 1;
}
//input:   pointer to array (data+crc)
//         K - number of information bets (data+crc)
//         i_il - NR_POLAR_PBCH_I_IL 1 - from 5.3.1.1 Interleaving - must be 1 to perform interliving
//output:  updates interlieved array C_prime[K]
void nr_polar_interleaver(uint8_t *input,  //(data+crc)
                          uint8_t *output,
                          uint16_t *pattern,
                          int K
                         )
{

    for (int i=0; i<K; i++)
    {
        output[i]=input[pattern[i]];
    }

}


/*******************************************************************************************************************************************

TS_38212  5.3.1.1.2  Polar encoding
input: c
output: c'

********************************************************************************************************************************************/

//Bit insertion (c' to u) interlived to u
// nr_polar_U - U in 3GPP


static const uint16_t Q_0_Nminus1_1[2] = { 0, 1 };
static const uint16_t Q_0_Nminus1_2[4] = { 0, 1, 2, 3 };
static const uint16_t Q_0_Nminus1_3[8] = { 0, 1, 2, 4, 3, 5, 6, 7 };
static const uint16_t Q_0_Nminus1_4[16] = { 0, 1, 2, 4, 8, 3, 5, 9, 6, 10, 12, 7, 11, 13, 14, 15 };
static const uint16_t Q_0_Nminus1_5[32] = { 0, 1, 2, 4, 8, 16, 3, 5, 9, 6, 17, 10, 18, 12, 20, 24, 7, 11, 19, 13, 14, 21, 26, 25, 22, 28, 15, 23, 27, 29, 30, 31 };
static const uint16_t Q_0_Nminus1_6[64] = { 0, 1, 2, 4, 8, 16, 32, 3, 5, 9, 6, 17, 10, 18, 12, 33, 20, 34, 24, 36, 7, 11, 40, 19, 13, 48, 14, 21, 35, 26, 37, 25, 22, 38, 41, 28, 42, 49, 44, 50, 15, 52, 23, 56, 27, 39, 29, 43, 30, 45, 51, 46, 53, 54, 57, 58, 60, 31, 47, 55, 59, 61, 62, 63 };
static const uint16_t Q_0_Nminus1_7[128] = { 0, 1, 2, 4, 8, 16, 32, 3, 5, 64, 9, 6, 17, 10, 18, 12, 33, 65, 20, 34, 24, 36, 7, 66, 11, 40, 68, 19, 13, 48, 14, 72, 21, 35, 26, 80, 37, 25, 22, 38, 96, 67, 41, 28, 69, 42, 49, 74, 70, 44, 81, 50, 73, 15, 52, 23, 76, 82, 56, 27, 97, 39, 84, 29, 43, 98, 88, 30, 71, 45, 100, 51, 46, 75, 104, 53, 77, 54, 83, 57, 112, 78, 85, 58, 99, 86, 60, 89, 101, 31, 90, 102, 105, 92, 47, 106, 55, 113, 79, 108, 59, 114, 87, 116, 61, 91, 120, 62, 103, 93, 107, 94, 109, 115, 110, 117, 118, 121, 122, 63, 124, 95, 111, 119, 123, 125, 126, 127 };
static const uint16_t Q_0_Nminus1_8[256] = { 0, 1, 2, 4, 8, 16, 32, 3, 5, 64, 9, 6, 17, 10, 18, 128, 12, 33, 65, 20, 34, 24, 36, 7, 129, 66, 11, 40, 68, 130, 19, 13, 48, 14, 72, 21, 132, 35, 26, 80, 37, 25, 22, 136, 38, 96, 67, 41, 144, 28, 69, 42, 49, 74, 160, 192, 70, 44, 131, 81, 50, 73, 15, 133, 52, 23, 134, 76, 137, 82, 56, 27, 97, 39, 84, 138, 145, 29, 43, 98, 88, 140, 30, 146, 71, 161, 45, 100, 51, 148, 46, 75, 104, 162, 53, 193, 152, 77, 164, 54, 83, 57, 112, 135, 78, 194, 85, 58, 168, 139, 99, 86, 60, 89, 196, 141, 101, 147, 176, 142, 31, 200, 90, 149, 102, 105, 163, 92, 47, 208, 150, 153, 165, 106, 55, 113, 154, 79, 108, 224, 166, 195, 59, 169, 114, 156, 87, 197, 116, 170, 61, 177, 91, 198, 172, 120, 201, 62, 143, 103, 178, 93, 202, 107, 180, 151, 209, 94, 204, 155, 210, 109, 184, 115, 167, 225, 157, 110, 117, 212, 171, 226, 216, 158, 118, 173, 121, 199, 179, 228, 174, 122, 203, 63, 181, 232, 124, 205, 182, 211, 185, 240, 206, 95, 213, 186, 227, 111, 214, 188, 217, 229, 159, 119, 218, 230, 233, 175, 123, 220, 183, 234, 125, 241, 207, 187, 236, 126, 242, 244, 189, 215, 219, 231, 248, 190, 221, 235, 222, 237, 243, 238, 245, 127, 191, 246, 249, 250, 252, 223, 239, 251, 247, 253, 254, 255 };
static const uint16_t Q_0_Nminus1_9[512] = { 0, 1, 2, 4, 8, 16, 32, 3, 5, 64, 9, 6, 17, 10, 18, 128, 12, 33, 65, 20, 256, 34, 24, 36, 7, 129, 66, 11, 40, 68, 130, 19, 13, 48, 14, 72, 257, 21, 132, 35, 258, 26, 80, 37, 25, 22, 136, 260, 264, 38, 96, 67, 41, 144, 28, 69, 42, 49, 74, 272, 160, 288, 192, 70, 44, 131, 81, 50, 73, 15, 320, 133, 52, 23, 134, 384, 76, 137, 82, 56, 27, 97, 39, 259, 84, 138, 145, 261, 29, 43, 98, 88, 140, 30, 146, 71, 262, 265, 161, 45, 100, 51, 148, 46, 75, 266, 273, 104, 162, 53, 193, 152, 77, 164, 268, 274, 54, 83, 57, 112, 135, 78, 289, 194, 85, 276, 58, 168, 139, 99, 86, 60, 280, 89, 290, 196, 141, 101, 147, 176, 142, 321, 31, 200, 90, 292, 322, 263, 149, 102, 105, 304, 296, 163, 92, 47, 267, 385, 324, 208, 386, 150, 153, 165, 106, 55, 328, 113, 154, 79, 269, 108, 224, 166, 195, 270, 275, 291, 59, 169, 114, 277, 156, 87, 197, 116, 170, 61, 281, 278, 177, 293, 388, 91, 198, 172, 120, 201, 336, 62, 282, 143, 103, 178, 294, 93, 202, 323, 392, 297, 107, 180, 151, 209, 284, 94, 204, 298, 400, 352, 325, 155, 210, 305, 300, 109, 184, 115, 167, 225, 326, 306, 157, 329, 110, 117, 212, 171, 330, 226, 387, 308, 216, 416, 271, 279, 158, 337, 118, 332, 389, 173, 121, 199, 179, 228, 338, 312, 390, 174, 393, 283, 122, 448, 353, 203, 63, 340, 394, 181, 295, 285, 232, 124, 205, 182, 286, 299, 354, 211, 401, 185, 396, 344, 240, 206, 95, 327, 402, 356, 307, 301, 417, 213, 186, 404, 227, 418, 302, 360, 111, 331, 214, 309, 188, 449, 217, 408, 229, 159, 420, 310, 333, 119, 339, 218, 368, 230, 391, 313, 450, 334, 233, 175, 123, 341, 220, 314, 424, 395, 355, 287, 183, 234, 125, 342, 316, 241, 345, 452, 397, 403, 207, 432, 357, 187, 236, 126, 242, 398, 346, 456, 358, 405, 303, 244, 189, 361, 215, 348, 419, 406, 464, 362, 409, 219, 311, 421, 410, 231, 248, 369, 190, 364, 335, 480, 315, 221, 370, 422, 425, 451, 235, 412, 343, 372, 317, 222, 426, 453, 237, 433, 347, 243, 454, 318, 376, 428, 238, 359, 457, 399, 434, 349, 245, 458, 363, 127, 191, 407, 436, 465, 246, 350, 460, 249, 411, 365, 440, 374, 423, 466, 250, 371, 481, 413, 366, 468, 429, 252, 373, 482, 427, 414, 223, 472, 455, 377, 435, 319, 484, 430, 488, 239, 378, 459, 437, 380, 461, 496, 351, 467, 438, 251, 462, 442, 441, 469, 247, 367, 253, 375, 444, 470, 483, 415, 485, 473, 474, 254, 379, 431, 489, 486, 476, 439, 490, 463, 381, 497, 492, 443, 382, 498, 445, 471, 500, 446, 475, 487, 504, 255, 477, 491, 478, 383, 493, 499, 502, 494, 501, 447, 505, 506, 479, 508, 495, 503, 507, 509, 510, 511 };
static const uint16_t Q_0_Nminus1_10[1024] = { 0, 1, 2, 4, 8, 16, 32, 3, 5, 64, 9, 6, 17, 10, 18, 128, 12, 33, 65, 20, 256, 34, 24, 36, 7, 129, 66, 512, 11, 40, 68, 130, 19, 13, 48, 14, 72, 257, 21, 132, 35, 258, 26, 513, 80, 37, 25, 22, 136, 260, 264, 38, 514, 96, 67, 41, 144, 28, 69, 42, 516, 49, 74, 272, 160, 520, 288, 528, 192, 544, 70, 44, 131, 81, 50, 73, 15, 320, 133, 52, 23, 134, 384, 76, 137, 82, 56, 27, 97, 39, 259, 84, 138, 145, 261, 29, 43, 98, 515, 88, 140, 30, 146, 71, 262, 265, 161, 576, 45, 100, 640, 51, 148, 46, 75, 266, 273, 517, 104, 162, 53, 193, 152, 77, 164, 768, 268, 274, 518, 54, 83, 57, 521, 112, 135, 78, 289, 194, 85, 276, 522, 58, 168, 139, 99, 86, 60, 280, 89, 290, 529, 524, 196, 141, 101, 147, 176, 142, 530, 321, 31, 200, 90, 545, 292, 322, 532, 263, 149, 102, 105, 304, 296, 163, 92, 47, 267, 385, 546, 324, 208, 386, 150, 153, 165, 106, 55, 328, 536, 577, 548, 113, 154, 79, 269, 108, 578, 224, 166, 519, 552, 195, 270, 641, 523, 275, 580, 291, 59, 169, 560, 114, 277, 156, 87, 197, 116, 170, 61, 531, 525, 642, 281, 278, 526, 177, 293, 388, 91, 584, 769, 198, 172, 120, 201, 336, 62, 282, 143, 103, 178, 294, 93, 644, 202, 592, 323, 392, 297, 770, 107, 180, 151, 209, 284, 648, 94, 204, 298, 400, 608, 352, 325, 533, 155, 210, 305, 547, 300, 109, 184, 534, 537, 115, 167, 225, 326, 306, 772, 157, 656, 329, 110, 117, 212, 171, 776, 330, 226, 549, 538, 387, 308, 216, 416, 271, 279, 158, 337, 550, 672, 118, 332, 579, 540, 389, 173, 121, 553, 199, 784, 179, 228, 338, 312, 704, 390, 174, 554, 581, 393, 283, 122, 448, 353, 561, 203, 63, 340, 394, 527, 582, 556, 181, 295, 285, 232, 124, 205, 182, 643, 562, 286, 585, 299, 354, 211, 401, 185, 396, 344, 586, 645, 593, 535, 240, 206, 95, 327, 564, 800, 402, 356, 307, 301, 417, 213, 568, 832, 588, 186, 646, 404, 227, 896, 594, 418, 302, 649, 771, 360, 539, 111, 331, 214, 309, 188, 449, 217, 408, 609, 596, 551, 650, 229, 159, 420, 310, 541, 773, 610, 657, 333, 119, 600, 339, 218, 368, 652, 230, 391, 313, 450, 542, 334, 233, 555, 774, 175, 123, 658, 612, 341, 777, 220, 314, 424, 395, 673, 583, 355, 287, 183, 234, 125, 557, 660, 616, 342, 316, 241, 778, 563, 345, 452, 397, 403, 207, 674, 558, 785, 432, 357, 187, 236, 664, 624, 587, 780, 705, 126, 242, 565, 398, 346, 456, 358, 405, 303, 569, 244, 595, 189, 566, 676, 361, 706, 589, 215, 786, 647, 348, 419, 406, 464, 680, 801, 362, 590, 409, 570, 788, 597, 572, 219, 311, 708, 598, 601, 651, 421, 792, 802, 611, 602, 410, 231, 688, 653, 248, 369, 190, 364, 654, 659, 335, 480, 315, 221, 370, 613, 422, 425, 451, 614, 543, 235, 412, 343, 372, 775, 317, 222, 426, 453, 237, 559, 833, 804, 712, 834, 661, 808, 779, 617, 604, 433, 720, 816, 836, 347, 897, 243, 662, 454, 318, 675, 618, 898, 781, 376, 428, 665, 736, 567, 840, 625, 238, 359, 457, 399, 787, 591, 678, 434, 677, 349, 245, 458, 666, 620, 363, 127, 191, 782, 407, 436, 626, 571, 465, 681, 246, 707, 350, 599, 668, 790, 460, 249, 682, 573, 411, 803, 789, 709, 365, 440, 628, 689, 374, 423, 466, 793, 250, 371, 481, 574, 413, 603, 366, 468, 655, 900, 805, 615, 684, 710, 429, 794, 252, 373, 605, 848, 690, 713, 632, 482, 806, 427, 904, 414, 223, 663, 692, 835, 619, 472, 455, 796, 809, 714, 721, 837, 716, 864, 810, 606, 912, 722, 696, 377, 435, 817, 319, 621, 812, 484, 430, 838, 667, 488, 239, 378, 459, 622, 627, 437, 380, 818, 461, 496, 669, 679, 724, 841, 629, 351, 467, 438, 737, 251, 462, 442, 441, 469, 247, 683, 842, 738, 899, 670, 783, 849, 820, 728, 928, 791, 367, 901, 630, 685, 844, 633, 711, 253, 691, 824, 902, 686, 740, 850, 375, 444, 470, 483, 415, 485, 905, 795, 473, 634, 744, 852, 960, 865, 693, 797, 906, 715, 807, 474, 636, 694, 254, 717, 575, 913, 798, 811, 379, 697, 431, 607, 489, 866, 723, 486, 908, 718, 813, 476, 856, 839, 725, 698, 914, 752, 868, 819, 814, 439, 929, 490, 623, 671, 739, 916, 463, 843, 381, 497, 930, 821, 726, 961, 872, 492, 631, 729, 700, 443, 741, 845, 920, 382, 822, 851, 730, 498, 880, 742, 445, 471, 635, 932, 687, 903, 825, 500, 846, 745, 826, 732, 446, 962, 936, 475, 853, 867, 637, 907, 487, 695, 746, 828, 753, 854, 857, 504, 799, 255, 964, 909, 719, 477, 915, 638, 748, 944, 869, 491, 699, 754, 858, 478, 968, 383, 910, 815, 976, 870, 917, 727, 493, 873, 701, 931, 756, 860, 499, 731, 823, 922, 874, 918, 502, 933, 743, 760, 881, 494, 702, 921, 501, 876, 847, 992, 447, 733, 827, 934, 882, 937, 963, 747, 505, 855, 924, 734, 829, 965, 938, 884, 506, 749, 945, 966, 755, 859, 940, 830, 911, 871, 639, 888, 479, 946, 750, 969, 508, 861, 757, 970, 919, 875, 862, 758, 948, 977, 923, 972, 761, 877, 952, 495, 703, 935, 978, 883, 762, 503, 925, 878, 735, 993, 885, 939, 994, 980, 926, 764, 941, 967, 886, 831, 947, 507, 889, 984, 751, 942, 996, 971, 890, 509, 949, 973, 1000, 892, 950, 863, 759, 1008, 510, 979, 953, 763, 974, 954, 879, 981, 982, 927, 995, 765, 956, 887, 985, 997, 986, 943, 891, 998, 766, 511, 988, 1001, 951, 1002, 893, 975, 894, 1009, 955, 1004, 1010, 957, 983, 958, 987, 1012, 999, 1016, 767, 989, 1003, 990, 1005, 959, 1011, 1013, 895, 1006, 1014, 1017, 1018, 991, 1020, 1007, 1015, 1019, 1021, 1022, 1023 };

// needed to select Q_0_Nminus1
static const uint16_t *Q_0_Nminus1[11] = { NULL, Q_0_Nminus1_1, Q_0_Nminus1_2, Q_0_Nminus1_3, Q_0_Nminus1_4,
                                           Q_0_Nminus1_5, Q_0_Nminus1_6, Q_0_Nminus1_7, Q_0_Nminus1_8, Q_0_Nminus1_9, Q_0_Nminus1_10
                                         };

const uint16_t* nr_polar_sequence_pattern(uint8_t n)
{
    return Q_0_Nminus1[n];
}

//help func to polar codes, generates
// needed to generate Q_I_N, Q_F_N
// ibp - shoe if bit is information or not
void nr_polar_info_bit_pattern(uint8_t* ibp, // out newPolarInitNode->information_bit_pattern, // паттерн  =1 если это информационный бит, обновляется в функции
                               int16_t* Q_I_N, // out
                               int16_t* Q_F_N, // out
                               uint16_t* J,     //out ??????
                               const uint16_t* Q_0_Nminus1, // input, from table TS_38212 Tabl_5.3.1.2-1
                               uint16_t K,
                               uint16_t N,
                               uint16_t E,
                               int8_t n_PC)
{
    int16_t* Q_Ftmp_N = (int16_t*)malloc(sizeof(int16_t) * (N + 1)); // Last element shows the final
    int16_t* Q_Itmp_N = (int16_t*)malloc(sizeof(int16_t) * (N + 1)); // array index assigned a value.

    for (int i = 0; i <= N; i++)
    {
        Q_Ftmp_N[i] = -1; // Empty array.
        Q_Itmp_N[i] = -1;
    }

    uint8_t flag;
    uint16_t limit, ind;

    if (E < N)
    {
        if ((K / (double)E) <= (7.0 / 16))   //puncturing
        {
            for (int n = 0; n <= N - E - 1; n++)
            {
                ind = Q_Ftmp_N[N] + 1;
                Q_Ftmp_N[ind] = J[n];
                Q_Ftmp_N[N] = Q_Ftmp_N[N] + 1;
            }

            if ((E / (double)N) >= (3.0 / 4))
            {
                limit = ceil((double)(3 * N - 2 * E) / 4);
                for (int n = 0; n <= limit - 1; n++)
                {
                    ind = Q_Ftmp_N[N] + 1;
                    Q_Ftmp_N[ind] = n;
                    Q_Ftmp_N[N] = Q_Ftmp_N[N] + 1;
                }
            }
            else
            {
                limit = ceil((double)(9 * N - 4 * E) / 16);
                for (int n = 0; n <= limit - 1; n++)
                {
                    ind = Q_Ftmp_N[N] + 1;
                    Q_Ftmp_N[ind] = n;
                    Q_Ftmp_N[N] = Q_Ftmp_N[N] + 1;
                }
            }
        }
        else   //shortening
        {
            for (int n = E; n <= N - 1; n++)
            {
                ind = Q_Ftmp_N[N] + 1;
                Q_Ftmp_N[ind] = J[n];
                Q_Ftmp_N[N] = Q_Ftmp_N[N] + 1;
            }
        }
    }

    //Q_I,tmp_N = Q_0_N-1 \ Q_F,tmp_N
    for (int n = 0; n <= N - 1; n++)
    {
        flag = 1;
        for (int m = 0; m <= Q_Ftmp_N[N]; m++)
        {
            if (Q_0_Nminus1[n] == Q_Ftmp_N[m])
            {
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            Q_Itmp_N[Q_Itmp_N[N] + 1] = Q_0_Nminus1[n];
            Q_Itmp_N[N]++;
        }
    }

    //Q_I_N comprises (K+n_PC) most reliable bit indices in Q_I,tmp_N
    for (int n = 0; n <= (K + n_PC) - 1; n++)
    {
        ind = Q_Itmp_N[N] + n - ((K + n_PC) - 1);
        Q_I_N[n] = Q_Itmp_N[ind];
    }

    //Q_F_N = Q_0_N-1 \ Q_I_N
    for (int n = 0; n <= N - 1; n++)
    {
        flag = 1;
        for (int m = 0; m <= (K + n_PC) - 1; m++)
        {
            if (Q_0_Nminus1[n] == Q_I_N[m])
            {
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            Q_F_N[Q_F_N[N] + 1] = Q_0_Nminus1[n];
            Q_F_N[N]++;
        }
    }

    //Information Bit Pattern
    for (int n = 0; n <= N - 1; n++)
    {
        ibp[n] = 0;

        for (int m = 0; m <= (K + n_PC) - 1; m++)
        {
            if (n == Q_I_N[m])
            {
                ibp[n] = 1;
                break;
            }
        }
    }

    free(Q_Ftmp_N);
    free(Q_Itmp_N);
}

// genereates u - TS_212 5.3.1.2 Polar encoding
void nr_polar_bit_insertion(uint8_t *input, //c'
                            uint8_t *output, // u
                            uint16_t N,      //   nr_polar_output_length()  TS 5.3.1
                            uint16_t K,      // (data+crc)
                            int16_t *Q_I_N,  //
                            int16_t *Q_PC_N,
                            int8_t n_PC) // parity check, for pbch = 0
{
    uint16_t k=0;
    uint8_t flag;

    if (n_PC>0)
    {
        /*
         *
         */
    }
    else
    {
        for (int n=0; n<=N-1; n++)
        {
            flag=0;
            for (int m=0; m<=(K+n_PC)-1; m++)
            {
                if ( n == Q_I_N[m])
                {
                    flag=1;
                    break;
                }
            }
            if (flag)   // n ϵ Q_I_N
            {
                output[n]=input[k];
                k++;
            }
            else
            {
                output[n] = 0;
            }
        }
    }

}

//Encoding (u to d)
// multiplication to Galua
void nr_matrix_multiplication_uint8_1D_uint8_2D(uint8_t *matrix1,
        uint8_t **matrix2,
        uint8_t *output,
        uint16_t row,
        uint16_t col)
{
    for (uint16_t i = 0; i < col; i++)
    {
        output[i] = 0;
        for (uint16_t j = 0; j < row; j++)
        {
            output[i] += matrix1[j] * matrix2[j][i];
        }
    }

}


void polar_encoder( uint64_t* in,
                    uint8_t* out,
                    uint8_t nmax,
                    uint8_t I_il,
                    uint8_t Npc,
                    uint8_t Npc_wm,
                    uint16_t K,
                    uint16_t E
                  )
{
    /***
            E = 864; // (the rate matching output sequence length as given in Clause 5.4.1)
            n_max = 9;
            n_PC = 0;
            I_il = 0;
    ***/
    // convert input uint64_t to uint8_t
    uint8_t pbch_C[K];
    nr_bit2byte_uint64_8(in, K, &pbch_C[0]);

    // do interleaving
    uint8_t* pbch_C_prime = (uint8_t*)malloc(sizeof(uint8_t) * K); // var for interleaving
    uint16_t* interleaving_pattern  = (uint16_t*)malloc(sizeof(uint16_t) * K);
    nr_polar_interleaving_pattern( K,
                                   I_il, // из глобальных = 1
                                   interleaving_pattern);

    nr_polar_interleaver(&pbch_C[0],  //(data+crc)
                         &pbch_C_prime[0],
                         interleaving_pattern,
                         K
                        );

    // N - output len after codding , from TS, depends on E, nmax
    uint16_t N = nr_polar_output_length( K, //(data+crc nits number)
                                         E, // (the rate matching output sequence length as given in Clause 5.4.1)
                                         nmax);
    uint8_t n = log2(N); // from

    uint8_t* ibp = (uint8_t*)malloc(sizeof(uint8_t) * (N)); // imformation bit pattern
    int16_t* Q_I_N = (int16_t*)malloc(sizeof(int16_t) * (K + Npc)); //
    int16_t* Q_F_N = (int16_t*)malloc(sizeof(int16_t) * (N + 1)); // Last element shows the final array index assigned a value.
    int16_t* Q_PC_N = (int16_t*)malloc(sizeof(int16_t) * (Npc));
    uint16_t* J = (uint16_t*)malloc(sizeof(uint16_t) * N);
    uint16_t* rmp = (uint16_t*)malloc(sizeof(uint16_t) * E);

    for (int i = 0; i <= N; i++)
        Q_F_N[i] = -1; // Empty array.

    // calculate Q_I_N, Q_F_N
    const uint16_t* Q_0_Nminus1 = nr_polar_sequence_pattern(n);
    nr_polar_info_bit_pattern(ibp, // out newPolarInitNode->information_bit_pattern, // паттерн  =1 если это информационный бит, обновляется в функции
                              Q_I_N, // out
                              Q_F_N, // out
                              J,     //out
                              Q_0_Nminus1, // задается статтически:    static const uint16_t Q_0_Nminus1_10[1024] =
                              K,
                              N,
                              E,
                              Npc);
    // calculate U
    uint8_t* nr_polar_U = (uint8_t*)malloc(sizeof(uint8_t) * N);
    nr_polar_bit_insertion( pbch_C_prime,
                            nr_polar_U,
                            N,
                            K,
                            Q_I_N,
                            Q_PC_N,
                            Npc);

    // do multiplication with Galua matrix
    uint8_t **G_N = nr_polar_kronecker_power_matrices(n);
    //uint8_t* nr_polar_D = (uint8_t*)malloc(sizeof(uint8_t) * N);
    nr_matrix_multiplication_uint8_1D_uint8_2D(nr_polar_U,
            G_N,
            out, //nr_polar_D,
            N,
            N);

    for (uint16_t i = 0; i < N; i++)
    {
        out[i] = (out[i] % 2);
    }



}


/*******************************************************************************************************************************************

-- TS_38212  7.1.5 Rate matching
-- based on Clause 5.4.1 rate matching with params:
                                                    E=864
                                                    I BIL =0

input: d[size N]
output: f[size E]


********************************************************************************************************************************************/

void rate_matching_do(uint8_t* in, // input d array
                      uint32_t* out,  // f array
                      uint16_t N,
                      uint16_t K
                     )
{

    uint16_t Ibil = 0;
    uint16_t E = 864;


    uint16_t* rate_matching_pattern = (uint16_t*)malloc(sizeof(uint16_t) * E);
    uint16_t* J = (uint16_t*)malloc(sizeof(uint16_t) * N);
    nr_polar_rate_matching_pattern(rate_matching_pattern, // ее будем обновлять в функции
                                   J,
                                   nr_polar_subblock_interleaver_pattern,  // static const uint8_t nr_polar_subblock_interleaver_pattern[32] =
                                   K,
                                   N,
                                   E);

    uint8_t* nr_polar_E = (uint8_t*)malloc(sizeof(uint8_t) * E);
    uint8_t* nr_polar_F = (uint8_t*)malloc(sizeof(uint8_t) * E);
    //Sub-block interleaving (d to y) and Bit selection (y to e)
    nr_polar_interleaver(in,
                         nr_polar_E,
                         rate_matching_pattern,
                         E);

    //5.4.1.3 The bit sequence  Interleaving of coded bits, if Ibil = 0 -> e=f
    //output [f_31 f_30 ... f_0] [f_63 f_62 ... f_32] ...
    nr_byte2bit_uint8_32(nr_polar_F, E, out);

}



/*******************************************************************************************************************************************

-- 5.4.1 Rate matching for Polar code
-- The rate matching for Polar code is defined per coded block and consists of sub-block interleaving, bit collection, and bit interleaving.
-- The input bit sequence to rate matching is   f 0 ,f 1 , f 2 ,...,f E−1 .
-- The output bit sequence after rate matching is denoted as  5.4.1.1   d 0 ,d 1 ,d 2 ,...,d N −1

********************************************************************************************************************************************/

/*******************************************************************************************************************************************

-- 5.4.1.1  Sub-block interleaving

********************************************************************************************************************************************/
// TS_38212 Table 5.4.1.1-1: Sub-block interleaver pattern Pi
//static const uint8_t nr_polar_subblock_interleaver_pattern[32] = { 0,1,2,4,3,5,6,7,8,16,9,17,10,18,11,19,12,20,13,21,14,22,15,23,24,25,26,28,27,29,30,31 };

//makes rate matching pattern, used for interleaving
void nr_polar_rate_matching_pattern(uint16_t* rmp, // output
                                    uint16_t* J,
                                    const uint8_t* P_i_, //nr_polar_subblock_interleaver_pattern - from TS
                                    uint16_t K,
                                    uint16_t N,
                                    uint16_t E)
{
    //const uint8_t* P_i_ = nr_polar_subblock_interleaver_pattern;
    uint8_t i;
    uint16_t* d, * y, ind;
    d = (uint16_t*)malloc(sizeof(uint16_t) * N);
    y = (uint16_t*)malloc(sizeof(uint16_t) * N);

    for (int m = 0; m <= N - 1; m++)
        d[m] = m;

    for (int m = 0; m <= N - 1; m++)
    {
        i = floor((32 * m) / N);
        J[m] = (P_i_[i] * (N / 32)) + (m % (N / 32));
        y[m] = d[J[m]];
    }

    if (E >= N)   //repetition
    {
        for (int k = 0; k <= E - 1; k++)
        {
            ind = (k % N);
            rmp[k] = y[ind];
        }
    }
    else
    {
        if ((K / (double)E) <= (7.0 / 16))   //puncturing
        {
            for (int k = 0; k <= E - 1; k++)
            {
                rmp[k] = y[k + N - E];
            }
        }
        else   //shortening
        {
            for (int k = 0; k <= E - 1; k++)
            {
                rmp[k] = y[k];
            }
        }
    }

    free(d);
    free(y);
}


void nr_byte2bit_uint8_32(uint8_t *in, uint16_t arraySize, uint32_t *out)
{
    uint8_t arrayInd = ceil(arraySize / 32.0);
    for (int i = 0; i < arrayInd; i++)
    {
        out[i]=0;
        for (int j = 31; j > 0; j--)
        {
            out[i]|=in[(i*32)+j];
            out[i]<<=1;
        }
        out[i]|=in[(i*32)];
    }
}





















