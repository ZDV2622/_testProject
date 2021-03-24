#ifndef TS_38212_RL16_MULTIPLEXING_AND_CHANNEL_CODING_H_INCLUDED
#define TS_38212_RL16_MULTIPLEXING_AND_CHANNEL_CODING_H_INCLUDED


#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>
#include <libTS38/headers/TS_38212_nr_polar_kronecker_power_matrices.h>
#include <cstring> // memset
#include <stdlib.h> // malloc
//#include <stdio.h>
//#include <iostream>
//#include <cstdint>
//#include <cstring>


#define AMP 512


#define NR_PSS_LENGTH 127
#define NR_SSS_LENGTH 127
#define NR_PBCH_DMRS_LENGTH 144
#define NR_PBCH_DMRS_LENGTH_DWORD 10 // ceil(2(QPSK)*NR_PBCH_DMRS_LENGTH/32) // должно быть ближайшим >= к Mnp - длина последовательности голда
#define NR_POLAR_PBCH_PAYLOAD_BITS 32     //uint16_t
#define NR_PBCH_PDU_BITS 24
#define NR_POLAR_PBCH_MESSAGE_TYPE 0      //int8_t
#define NR_POLAR_PBCH_AGGREGATION_LEVEL 0 //uint8_t
#define NR_POLAR_PBCH_CRC_PARITY_BITS 24

#define NR_POLAR_AGGREGATION_LEVEL_1_PRIME 149 //uint16_t
#define NR_POLAR_AGGREGATION_LEVEL_2_PRIME 151 //uint16_t
#define NR_POLAR_AGGREGATION_LEVEL_4_PRIME 157 //uint16_t
#define NR_POLAR_AGGREGATION_LEVEL_8_PRIME 163 //uint16_t
#define NR_POLAR_AGGREGATION_LEVEL_16_PRIME 167 //uint16_t

// TS crc params
#define NR_POLAR_PBCH_AGGREGATION_LEVEL 0 //uint8_t
#define NR_POLAR_PBCH_MESSAGE_TYPE 0      //int8_t
#define NR_POLAR_PBCH_PAYLOAD_BITS 32     //uint16_t
#define NR_POLAR_PBCH_CRC_PARITY_BITS 24
#define NR_POLAR_PBCH_CRC_ERROR_CORRECTION_BITS 3
//Assumed 3 by 3GPP when NR_POLAR_PBCH_L>8 to meet false alarm rate requirements.

//Sec. 7.1.4: Channel Coding
#define NR_POLAR_PBCH_N_MAX 9   //uint8_t
#define NR_POLAR_PBCH_I_IL 1    //uint8_t
#define NR_POLAR_PBCH_I_SEG 0   //uint8_t
#define NR_POLAR_PBCH_N_PC 0    //uint8_t
#define NR_POLAR_PBCH_N_PC_WM 0 //uint8_t
//#define NR_POLAR_PBCH_N 512     //uint16_t

//Sec. 7.1.5: Rate Matching
#define NR_POLAR_PBCH_I_BIL 0 //uint8_t
#define NR_POLAR_PBCH_E 864   //uint16_t
#define NR_POLAR_PBCH_E_DWORD 27 // NR_POLAR_PBCH_E/32



/***********************************************************************************************************************************************

TS_38212  5.1  CRC calculation
crc_calculate - final function

************************************************************************************************************************************************/

void nr_PBCH_tb_crc_attach( uint64_t* inputBit,   // pointer to inbut bit sequence
                            //uint64_t crcBit,    //
                            int bitlen,           // original bit len
                            //int crcParityBitsN,   // L=24 for pbch crc calc
                            uint64_t* output      // pointer to output(data+crc)
                          );



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

// makes crc for 8bits - help function
unsigned int crcbit(unsigned char* inputptr,
                    int octetlen,
                    unsigned int poly);
// 256 - число возможных значений;
// ссылк на теорию: https://soltau.ru/index.php/themes/dev/item/461-kak-poschitat-kontrolnuyu-summu-crc32-crc16-crc8
// https://programm.ws/page.php?id=665
void crcTableInit(void);
/*********************************************************
Byte by byte implementations,
assuming initial byte is 0 padded (in MSB) if necessary
*********************************************************/

unsigned int crc24a (unsigned char * inptr,int bitlen);
unsigned int crc24b (unsigned char * inptr,int bitlen);
unsigned int crc24c (unsigned char * inptr,int bitlen);
unsigned int crc16 (unsigned char * inptr, int bitlen);
unsigned int crc12 (unsigned char * inptr, int bitlen);
unsigned int crc11 (unsigned char * inptr, int bitlen);
unsigned int crc8 (unsigned char * inptr, int bitlen);
unsigned int crc6 (unsigned char * inptr, int bitlen);
// output is calculated crc, depends on bitlen and polinom, for pbch
uint64_t crc_calculate(     uint64_t *A, //array after scrampling  a'
                            //void* out,
                            int32_t crcmask, // set 0
                            int bitlen,
                            int crcParityBitsN,
                            uint8_t ones_flag
                            //t_nrPolar_params* polarParams
                      );


/// TS_38212  7.1.1  PBCH payload generation  -----------------------------------------------------------------------------------------------
/// includes
void nr_init_pbch_interleaver(uint8_t* interleaver);

uint32_t PBCH_payload_generation(uint32_t bchPayload,
                                 uint8_t Lmax,
                                 uint32_t Kssb,
                                 uint8_t ssb_index,
                                 int sfn,
                                 uint8_t n_hf,
                                 int16_t amp
                                );

/// TS_38212  7.1.2  Scrambling     -----------------------------------------------------------------------------------------------
///
uint32_t nr_pbch_scrambling( uint32_t bchPayloadInterleaved,
                             uint32_t pci,
                             uint8_t Lmax,
                             uint16_t length,
                             //uint8_t encoded,
                             int sfn
                           );

void nr_tb_crc_attach( uint64_t* inputBit,
                            uint64_t crcBit,
                            int bitlen,
                            int crcParityBitsN, // for
                            uint64_t* output
                          );

void nr_bit2byte_uint32_8(uint32_t *in, uint16_t arraySize, uint8_t *out);
void nr_bit2byte_uint64_8(uint64_t *in, uint16_t arraySize, uint8_t *out);


void nr_polar_interleaving_pattern(uint16_t K, uint8_t I_IL, uint16_t *PI_k_);

//Interleaving (c to c')
// переставляет биты
typedef struct
{
    uint8_t* nr_polar_A;
    uint8_t* nr_polar_CPrime;
    uint8_t* nr_polar_B;
    uint8_t* nr_polar_U;
} polar_vars_t;

void nr_polar_interleaver(uint8_t *input,  //(data+crc)
                          uint8_t *output,
                          uint16_t *pattern,
                          int K
                         );

/********************************************************************************************************************************************************************
TS_38212   5.3.1  Polar coding
input:
output i
*********************************************************************************************************************************************************************/


uint32_t nr_polar_output_length(uint16_t K, //(data+crc nits number)
                                uint16_t E, // (the rate matching output sequence length as given in Clause 5.4.1)
                                uint8_t n_max);


/*******************************************************************************************************************************************

TS_38212  5.3.1.1.2  Polar encoding
input: c
output: c'

********************************************************************************************************************************************/


const uint16_t* nr_polar_sequence_pattern(uint8_t n);

// TS_38212 Table 5.4.1.1-1: Sub-block interleaver pattern Pi
static const uint8_t nr_polar_subblock_interleaver_pattern[32] = { 0,1,2,4,3,5,6,7,8,16,9,17,10,18,11,19,12,20,13,21,14,22,15,23,24,25,26,28,27,29,30,31 };


void nr_polar_rate_matching_pattern(uint16_t* rmp, // out
                                    uint16_t* J,   // out
                                    const uint8_t* P_i_,
                                    uint16_t K,
                                    uint16_t N,
                                    uint16_t E);
//Bit insertion (c' to u) interlived to u
// nr_polar_U - U in 3GPP

//static const uint8_t nr_polar_subblock_interleaver_pattern[32] = { 0,1,2,4,3,5,6,7,8,16,9,17,10,18,11,19,12,20,13,21,14,22,15,23,24,25,26,28,27,29,30,31 };
//help func to polar codes, generates
void nr_polar_info_bit_pattern(uint8_t* ibp, // out newPolarInitNode->information_bit_pattern, // паттерн  =1 если это информационный бит, обновляется в функции
                               int16_t* Q_I_N, // out
                               int16_t* Q_F_N, // out
                               uint16_t* J,     //out
                               const uint16_t* Q_0_Nminus1, // задается статтически:    static const uint16_t Q_0_Nminus1_10[1024] =
                               uint16_t K,
                               uint16_t N,
                               uint16_t E,
                               int8_t n_PC);

void nr_polar_bit_insertion(uint8_t *input, //c''
                            uint8_t *output, // u
                            uint16_t N,      //   nr_polar_output_length()  TS 5.3.1
                            uint16_t K,      // (data+crc)
                            int16_t *Q_I_N,  //
                            int16_t *Q_PC_N,
                            int8_t n_PC);


void nr_polar_rate_matching_pattern(uint16_t* rmp,
                                    uint16_t* J,
                                    const uint8_t* P_i_,
                                    uint16_t K,
                                    uint16_t N,
                                    uint16_t E);

void nr_matrix_multiplication_uint8_1D_uint8_2D(uint8_t *matrix1,
        uint8_t **matrix2,
        uint8_t *output,
        uint16_t row,
        uint16_t col);

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
                        );


void polar_encoder( uint64_t* in,
                    uint8_t* out,
                    uint8_t nmax,
                    uint8_t I_il,
                    uint8_t Npc,
                    uint8_t Npc_wm,
                    uint16_t K,
                    uint16_t E
                  );

void nr_byte2bit_uint8_32(uint8_t *in, uint16_t arraySize, uint32_t *out);


void rate_matching_do(uint8_t* in, // input d array
                      uint32_t* out,  // f array
                      uint16_t N,
                      uint16_t K
                      );

#endif // TS_38212_RL16_MULTIPLEXING_AND_CHANNEL_CODING_H_INCLUDED
