//#include <stdio.h>
//#include <iostream>
//#include <cstdint>
//#include <cstring>
#include <cmath>
#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>

//#include <libINIT/headers/_struct_zdv_cfg.h>
//#include <libTS38/headers/TS_38213_PHY_Layer_Procedure.h>


/*******************************************************************************************************************************************

TS_38211  5.2.1 Pseudo-random sequence generation(Gold)

********************************************************************************************************************************************/


extern uint32_t lte_gold_generic(uint32_t* x1, uint32_t* x2, uint8_t reset)
{
    int32_t n;

    // 3GPP 3x.211
    // Nc = 1600
    // c(n)     = [x1(n+Nc) + x2(n+Nc)]mod2
    // x1(n+31) = [x1(n+3)                     + x1(n)]mod2
    // x2(n+31) = [x2(n+3) + x2(n+2) + x2(n+1) + x2(n)]mod2
    if (reset)
    {
        // Init value for x1: x1(0) = 1, x1(n) = 0, n=1,2,...,30
        // x1(31) = [x1(3) + x1(0)]mod2 = 1
        *x1 = 1 + (1 << 31);
        // Init value for x2: cinit = sum_{i=0}^30 x2*2^i
        // x2(31) = [x2(3)    + x2(2)    + x2(1)    + x2(0)]mod2
        //        =  (*x2>>3) ^ (*x2>>2) + (*x2>>1) + *x2
        *x2 = *x2 ^ ((*x2 ^ (*x2 >> 1) ^ (*x2 >> 2) ^ (*x2 >> 3)) << 31);
        // x1 and x2 contain bits n = 0,1,...,31
        // Nc = 1600 bits are skipped at the beginning
        // i.e., 1600 / 32 = 50 32bit words

        for (n = 1; n < 50; n++)
        {
            // Compute x1(0),...,x1(27)
            *x1 = (*x1 >> 1) ^ (*x1 >> 4);
            // Compute x1(28),..,x1(31) and xor
            *x1 = *x1 ^ (*x1 << 31) ^ (*x1 << 28);
            // Compute x2(0),...,x2(27)
            *x2 = (*x2 >> 1) ^ (*x2 >> 2) ^ (*x2 >> 3) ^ (*x2 >> 4);
            // Compute x2(28),..,x2(31) and xor
            *x2 = *x2 ^ (*x2 << 31) ^ (*x2 << 30) ^ (*x2 << 29) ^ (*x2 << 28);
        }
    }
    *x1 = (*x1 >> 1) ^ (*x1 >> 4);
    *x1 = *x1 ^ (*x1 << 31) ^ (*x1 << 28);
    *x2 = (*x2 >> 1) ^ (*x2 >> 2) ^ (*x2 >> 3) ^ (*x2 >> 4);
    *x2 = *x2 ^ (*x2 << 31) ^ (*x2 << 30) ^ (*x2 << 29) ^ (*x2 << 28);
    //printf("x2 %p", *x2);
    // c(n) = [x1(n+Nc) + x2(n+Nc)]mod2
    return(*x1 ^ *x2);
}


/*******************************************************************************************************************************************

-- TS_38211 5.3.1 OFDM baseband signal generation for all channels except PRACH and RIM-RS
-- return: CP lenght in  samples number

********************************************************************************************************************************************/


uint16_t ncp_calculate_no_prach(uint8_t mu,
                                uint8_t l, // l - it is ofdm symbol number in SFN
                                lte_prefix_type_t lte_prefix_type
                               )
{
    uint16_t ncp = 0;

    if (lte_prefix_type == 1)
    {
        ncp = 512*K_TS_TC*(pow(2,-mu));
        //printf("\ntest");
    }
    else
    {
        if (mu==0)
        {
            //printf("\ntest1");
            if ( l%14==0 || l%7==0 )
            {
                //printf("\ntest2");
                ncp = 144 * K_TS_TC * (pow(2,-mu)) + 16 * K_TS_TC;
            }
            else
                //printf("\ntest3");
            {
                ncp = 144*K_TS_TC*(pow(2,-mu));
            }
        }
        else
        {
            //printf("\ntest4");
            if ( l%14==0 )
            {
                //printf("\ntest5");
                ncp = 144 * K_TS_TC * (pow(2,-mu)) + 16 * K_TS_TC;
            }
            else
            {
                //printf("\ntest6");
                ncp = 144*K_TS_TC*(pow(2,-mu));
            }
        }
    }


    return ncp/K_TS_TC;

}


/// to do: subfunction - generate, mapping
///обновляет данные в txdataF
///input:  ssb_start_symbol - индекс ofdm символа внутри слота
/// обновляет d_pss
///-- TS_38211 7.4.2.2.1 Primary synchronization signal Sequence generation
///-- using Table 7.4.3.1-1: Resources within an SS/PBCH block for PSS, SSS, PBCH, and DM-RS for PBCH.

void nr_generate_pss(nr_config_full_t *cfg,//input params
                     int *txdataF, // output
                     int amp
                     //int fft_size,
                     //int subc_test,// for test
                     //int ofdm_symb_test//for test
                    )
{
    int i, n, k, l, m;
    int fft_size;
    int16_t a;
    int x[NR_PSS_LENGTH];
    const int x_initial[7] = { 0, 1, 1, 0, 1, 1, 1 };
    int d_pss[NR_PSS_LENGTH];

    fft_size = cfg->fft_size;

    uint8_t Nid2 = cfg->nfapi_nr_config_tlv.cell_config.phy_cell_id.value % 3;

    /// Sequence generation
    for (i = 0; i < 7; i++)
        x[i] = x_initial[i];

    for (i = 0; i < (NR_PSS_LENGTH - 7); i++)
    {
        x[i + 7] = (x[i + 4] + x[i]) % 2;
    }

    for (n = 0; n < NR_PSS_LENGTH; n++)
    {
        m = (n + 43 * Nid2) % (NR_PSS_LENGTH);
        d_pss[n] = (1 - 2 * x[m]) * 23170;//??????????????????????????????????????
    }
    /// Resource mapping
    a = amp;

    ///PSS occupies a predefined position (subcarriers 56-182, symbol 0) Table 7.4.3.1-1: Resources within an SS/PBCH block for PSS, SSS, PBCH, and DM-RS for PBCH.
    k = cfg->fft_shift_zdv +
        cfg->nfapi_nr_config_tlv.ssb_table.ssb_offset_point_a.value*12 +
        cfg->nfapi_nr_config_tlv.ssb_table.ssb_subcarrier_offset.value + 56; //and
    if (k >= fft_size)
        k = k - fft_size;  //?????????
    // test
    //k = 0;
    //k = subc_test;
    //l = ssb_start_symbol;
    for(i = 0; i<64; i++)
    {
        l = cfg->ssb_start_ofdm_symb[i];
        //l = l-2;
        if(l>0)
        {
#ifdef DEBUG_PSS
            printf("\nofdm symbol for pss l = %d",l);
#endif // DEBUG
            //l = ofdm_symb_test; // zdv test
            for (int m = 0; m < NR_PSS_LENGTH; m++)
            {
#ifdef DEBUG_PSS
                printf("\npss: txdataF[%d] ||  pss_n = %d || pss_v = %d  || l = %d ", 2 * (l * fft_size + k),m, (a * d_pss[m]) >> 15,l);
#endif
                txdataF[2 * (l * fft_size + k)] = (a * d_pss[m]) >> 15;
                k++;

                if (k >= fft_size)
                    k = k - fft_size;
            }
        }
    }

}


/// SSS ------------------------------------------------------------------------------------------------------------------------------------------------
/// 7.4.2.3.1 Secondarysynchronization signal Sequence generation
/// using Table 7.4.3.1-1: Resources within an SS/PBCH block for PSS, SSS, PBCH, and DM-RS for PBCH.
void nr_generate_sss(nr_config_full_t *cfg,//input params
                     int *txdataF, // output
                     int amp
                     //int fft_size,
                     //int subc_test,// for test
                     //int ofdm_symb_test//for test
                    )
{
    int i, k, l;
    int m0, m1;
    int Nid, Nid1, Nid2;
    int16_t a;
    int16_t x0[NR_SSS_LENGTH], x1[NR_SSS_LENGTH];
    const int x0_initial[7] = { 1, 0, 0, 0, 0, 0, 0 };
    const int x1_initial[7] = { 1, 0, 0, 0, 0, 0, 0 };
    int d_sss[NR_SSS_LENGTH];
    int fft_size;

    fft_size = cfg->fft_size;

    /// Sequence generation
    Nid = cfg->nfapi_nr_config_tlv.cell_config.phy_cell_id.value;
    Nid2 = Nid % 3;
    Nid1 = (Nid - Nid2) / 3;

    for (i = 0; i < 7; i++)
    {
        x0[i] = x0_initial[i];
        x1[i] = x1_initial[i];
    }

    for (i = 0; i < NR_SSS_LENGTH - 7; i++)
    {
        x0[i + 7] = (x0[i + 4] + x0[i]) % 2;
        x1[i + 7] = (x1[i + 1] + x1[i]) % 2;
    }

    m0 = 15 * (Nid1 / 112) + (5 * Nid2);
    m1 = Nid1 % 112;

    for (i = 0; i < NR_SSS_LENGTH; i++)
    {
        d_sss[i] = (1 - 2 * x0[(i + m0) % NR_SSS_LENGTH]) * (1 - 2 * x1[(i + m1) % NR_SSS_LENGTH]) * 23170;
    }

    //PSS occupies a predefined position (subcarriers 56-182, symbol 0) Table 7.4.3.1-1: Resources within an SS/PBCH block for PSS, SSS, PBCH, and DM-RS for PBCH.
    k = cfg->fft_shift_zdv +
        cfg->nfapi_nr_config_tlv.ssb_table.ssb_offset_point_a.value*12 +
        cfg->nfapi_nr_config_tlv.ssb_table.ssb_subcarrier_offset.value + 56; //and
    if (k >= fft_size)
        k = k - fft_size;  //?????????
    a = amp;

    for(i = 0; i<64; i++)
    {
        l = cfg->ssb_start_ofdm_symb[i]+2;
        if((l-2)>0)
        {
#ifdef DEBUG_SSS
            //printf("\nofdm symbol for sss l = %d",l);
#endif // DEBUG
            //l = ofdm_symb_test; // zdv test
            for (int m = 0; m < NR_SSS_LENGTH; m++)
            {
#ifdef DEBUG_SSS
                printf("\nsss: txdataF[%d] ||  sss_n = %d || sss_v = %d  || l = %d ", 2 * (l * fft_size + k),m, (a * d_sss[m]) >> 15,l);
#endif
                txdataF[2 * (l * fft_size + k)] = (a * d_sss[m]) >> 15;
                k++;

                if (k >= fft_size)
                    k = k - fft_size;
            }
        }
    }

}

/*******************************************************************************************************************************************

TS_38211 7.4.1.4  DMRS Sequence Generation and scrambling


********************************************************************************************************************************************/
//template <size_t N, size_t M, size_t M>
// it generates dmrs sequence for all Lmax ssb
// not all of them can be transmitted, but generation is done for all of them
void DMRS_Seq_Generation_pbch ( uint16_t pci,   // used for scrambling
                                uint16_t Lmax,   //
                                uint16_t *ssb_index_mask,
                                uint32_t (&out)[2][64][10]//uint32_t ***out
                                )
{
    unsigned int x1, x2;
    uint16_t Nid, i_ssb, i_ssb2;
    unsigned char l, n_hf, N_hf;
    uint8_t idx = 0;
    uint8_t nushift = pci & 3;

    //uint32_t nr_gold_pbch_dmrs[2][64][10];
    //nfapi_nr_config_request_scf_t* cfg = &gNB->gNB_config;
    //NR_DL_FRAME_PARMS* fp = &gNB->frame_parms;
    uint8_t reset;
    int k;

    Nid = pci;
    N_hf = (Lmax == 4) ? 2 : 1;


    for (n_hf = 0; n_hf < N_hf; n_hf++)
    {
        for (l = 0; l < Lmax; l++)
        {
            if ( ssb_index_mask[l] == 1 )
            {
                i_ssb = l & (Lmax - 1); // i_ssb is the two least significant bits of the candidate SS/PBCH block index as defined in [5, TS 38.213]
                i_ssb2 = i_ssb + (n_hf << 2);  // i_ssb' = i_ssb + 4 nhf - from TS

                reset = 1;
                x2 = (1 << 11) * (i_ssb2 + 1) * ((Nid >> 2) + 1) + (1 << 6) * (i_ssb2 + 1) + (Nid & 3);
                for (uint8_t n = 0; n < NR_PBCH_DMRS_LENGTH_DWORD; n++)
                {
                    out[n_hf][l][n] = lte_gold_generic(&x1, &x2, reset);
                    reset = 0;
                }
            }

        }
    }

}


/*******************************************************************************************************************************************

-- TS_38211 7.3.3.2  Modulation PBCH DMRS
-- QPSK modulated as described in clause 5.1.3,

done:
void pbch_data_modulation( uint32_t *in, // input data after scrambling
                           int16_t *out // modulated data
                         )


********************************************************************************************************************************************/




/*******************************************************************************************************************************************

7.4.3.1.3  Mapping of PBCH and DM-RS within an SS/PBCH block
Table 7.4.3.1-1: Resources within an SS/PBCH block for PSS, SSS, PBCH, and DM-RS for PBCH.


********************************************************************************************************************************************/


void pbch_dmrs_resource_mapper( int16_t *mod_dmrs, // input, modulated data pbch dmrs
                                int16_t *txdataF,  // output
                                uint16_t ssb_subcarrier_offset, // offset in freq, kssb
                                uint16_t fft_size,  // estimated: 2*(fftsize - NRb*12 / 2)
                                uint16_t fft_shift,
                                uint16_t ssb_offset_point_a, // in Nrb
                                uint16_t* ssb_start_symb_mask,  // taken from ssb_index_mask
                                //uint16_t Kssb,
                                uint16_t pci,
                                int amp
                              )
{

//int16_t mod_dmrs[NR_PBCH_DMRS_LENGTH << 1];  input
    unsigned char l;
    uint8_t idx = 0;
    uint8_t nushift = pci & 3;

    uint8_t reset;
    int k;


    for(int i = 0; i<64; i++)
    {
        l = ssb_start_symb_mask[i];
        if(l>0)
        {
            /// Resource mapping
            // PBCH DMRS are mapped  within the SSB block on every fourth subcarrier starting from nushift of symbols 1, 2, 3
            ///symbol 1  [0+nushift:4:236+nushift] -- 60 mod symbols

            k = fft_shift +
                ssb_offset_point_a * 12 +
                ssb_subcarrier_offset +
                nushift;
            if (k >= fft_size)
                k = k - fft_size;  //?????????
            l = l + 1;


            for (int m = 0; m < 60; m++)
            {
                txdataF[(l * fft_size + k) << 1] = (amp * mod_dmrs[m << 1]) >> 15;
                txdataF[((l * fft_size + k) << 1) + 1] = (amp * mod_dmrs[(m << 1) + 1]) >> 15;
                k += 4;

                if (k >= fft_size)
                    k -= fft_size;

            }
            ///symbol 2  [0+u:4:44+nushift ; 192+nu:4:236+nushift] -- 24 mod symbols
            k = fft_shift +
                ssb_offset_point_a*12 +
                ssb_subcarrier_offset +
                nushift;
            if (k >= fft_size)
                k = k - fft_size;  //?????????
            l++;

            for (int m = 60; m < 84; m++)
            {
                txdataF[(l * fft_size + k) << 1] = (amp * mod_dmrs[m << 1]) >> 15;
                txdataF[((l * fft_size + k) << 1) + 1] = (amp * mod_dmrs[(m << 1) + 1]) >> 15;
                k += (m == 71) ? 148 : 4; // Jump from 44+nu to 192+nu
                if (k >= fft_size)
                    k -= fft_size;
            }
            ///symbol 3  [0+nushift:4:236+nushift] -- 60 mod symbols
            k = fft_shift +
                ssb_offset_point_a *12 +
                ssb_subcarrier_offset +
                nushift;
            if (k >= fft_size)
                k = k - fft_size;  //?????????
            l++;
            for (int m = 84; m < NR_PBCH_DMRS_LENGTH; m++)
            {
                txdataF[(l * fft_size + k) << 1] = (amp * mod_dmrs[m << 1]) >> 15;
                txdataF[((l * fft_size + k) << 1) + 1] = (amp * mod_dmrs[(m << 1) + 1]) >> 15;
                k += 4;
                if (k >= fft_size)
                    k -= fft_size;
            }


        }
    }

}



/// 7.4.1.4.1 Demodulation reference signals for PBCH Sequence generation
/// using Table 7.4.3.1-1: Resources within an SS/PBCH block for PSS, SSS, PBCH, and DM-RS for PBCH.
// генерим pbch
void nr_generate_pbch_dmrs(nr_config_full_t *cfg,//input params
                           int *txdataF, // output
                           int16_t amp
                           //int fft_size,
                           //int subc_test,// for test
                           //int ofdm_symb_test//for test
                          )
{
//    unsigned int x1, x2;
//    uint16_t Nid, i_ssb, i_ssb2;
//    unsigned char l, n_hf, N_hf;
//    int16_t mod_dmrs[NR_PBCH_DMRS_LENGTH << 1];
//    uint8_t idx = 0;
//    uint8_t nushift = pci & 3;
//    //nfapi_nr_config_request_scf_t* cfg = &gNB->gNB_config;
//    //NR_DL_FRAME_PARMS* fp = &gNB->frame_parms;
//    uint8_t reset;
//    int k;
//    uint32_t nr_gold_pbch_dmrs[2][64][10];
//
//    Nid = pci;
//    N_hf = (Lmax == 4) ? 2 : 1;
//    cfg->n_hf = N_hf;
//
//    for (n_hf = 0; n_hf < N_hf; n_hf++)
//    {
//        for (l = 0; l < Lmax; l++)
//        {
//            i_ssb = l & (Lmax - 1);
//            i_ssb2 = i_ssb + (n_hf << 2);
//
//            reset = 1;
//            x2 = (1 << 11) * (i_ssb2 + 1) * ((Nid >> 2) + 1) + (1 << 6) * (i_ssb2 + 1) + (Nid & 3);
//            for (uint8_t n = 0; n < NR_PBCH_DMRS_LENGTH_DWORD; n++)
//            {
//                cfg->nr_gold_pbch_dmrs[n_hf][l][n] = lte_gold_generic(&x1, &x2, reset);
//                reset = 0;
//            }
//
//        }
//    }
//
//    nr_generate_modulation_table();
//
//    // QPSK modulation
//    for (int m = 0; m < NR_PBCH_DMRS_LENGTH; m++)
//    {
//        //idx = (((gold_pbch_dmrs[(m << 1) >> 5]) >> ((m << 1) & 0x1f)) & 3);
//        uint32_t *gold_pbch_dmrs;
//        gold_pbch_dmrs = &cfg->nr_gold_pbch_dmrs[0][0][0];
//        idx = (((gold_pbch_dmrs[(m << 1) >> 5]) >> ((m << 1) & 0x1f)) & 3);
//        mod_dmrs[m << 1] = nr_qpsk_mod_table[idx << 1];
//        mod_dmrs[(m << 1) + 1] = nr_qpsk_mod_table[(idx << 1) + 1];
//    }
//
//
//    for(int i = 0; i<64; i++)
//    {
//        l = ssb_start_symb_mask[i];
//        if(l>0)
//        {
//            /// Resource mapping
//            // PBCH DMRS are mapped  within the SSB block on every fourth subcarrier starting from nushift of symbols 1, 2, 3
//            ///symbol 1  [0+nushift:4:236+nushift] -- 60 mod symbols
//
//            k = fft_shift +
//                ssb_offset_point_a * 12 +
//                ssb_subcarrier_offset +
//                nushift;
//            if (k >= fft_size)
//                k = k - fft_size;  //?????????
//            l = l + 1;
//
//
//            for (int m = 0; m < 60; m++)
//            {
//                txdataF[(l * cfg->fft_size + k) << 1] = (amp * mod_dmrs[m << 1]) >> 15;
//#ifdef DEBUG_PBCH_DMRS
//                printf("\nsss: txdataF[%d] ||  pbch_dmrs_n = %d || pbch_dmrs_v = %d  || l = %d ", (l * cfg->fft_size + k) << 1,m, (amp * mod_dmrs[m << 1]) >> 15,l);
//#endif
//                txdataF[((l * cfg->fft_size + k) << 1) + 1] = (amp * mod_dmrs[(m << 1) + 1]) >> 15;
//#ifdef DEBUG_PBCH_DMRS
//                printf("\nsss: txdataF[%d] ||  pbch_dmrs_n = %d || pbch_dmrs_v = %d  || l = %d ", ((l * cfg->fft_size + k) << 1) + 1,m,(amp * mod_dmrs[(m << 1) + 1]) >> 15,l);
//#endif
//                k += 4;
//
//                if (k >= cfg->fft_size)
//                    k -= cfg->fft_size;
//
//
//            }
//
//            ///symbol 2  [0+u:4:44+nushift ; 192+nu:4:236+nushift] -- 24 mod symbols
//            k = fft_shift +
//                ssb_offset_point_a*12 +
//                ssb_subcarrier_offset +
//                nushift;
//            if (k >= cfg->fft_size)
//                k = k - cfg->fft_size;  //?????????
//            l++;
//
//            for (int m = 60; m < 84; m++)
//            {
//                txdataF[(l * fft_size + k) << 1] = (amp * mod_dmrs[m << 1]) >> 15;
//#ifdef DEBUG_PBCH_DMRS
//                printf("\nsss: txdataF[%d] ||  pbch_dmrs_n = %d || pbch_dmrs_v = %d  || l = %d ", (l * cfg->fft_size + k) << 1,m, (amp * mod_dmrs[m << 1]) >> 15,l);
//#endif
//                txdataF[((l * fft_size + k) << 1) + 1] = (amp * mod_dmrs[(m << 1) + 1]) >> 15;
//#ifdef DEBUG_PBCH_DMRS
//                printf("\nsss: txdataF[%d] ||  pbch_dmrs_n = %d || pbch_dmrs_v = %d  || l = %d ", ((l * cfg->fft_size + k) << 1) + 1,m,(amp * mod_dmrs[(m << 1) + 1]) >> 15,l);
//#endif
//                k += (m == 71) ? 148 : 4; // Jump from 44+nu to 192+nu
//                if (k >= cfg->fft_size)
//                    k -= cfg->fft_size;
//            }
//
//            ///symbol 3  [0+nushift:4:236+nushift] -- 60 mod symbols
//            k = cfg->fft_shift_zdv +
//                cfg->nfapi_nr_config_tlv.ssb_table.ssb_offset_point_a.value*12 +
//                cfg->nfapi_nr_config_tlv.ssb_table.ssb_subcarrier_offset.value +
//                nushift;
//            if (k >= cfg->fft_size)
//                k = k - cfg->fft_size;  //?????????
//            l++;
//            for (int m = 84; m < NR_PBCH_DMRS_LENGTH; m++)
//            {
//                txdataF[(l * cfg->fft_size + k) << 1] = (amp * mod_dmrs[m << 1]) >> 15;
//#ifdef DEBUG_PBCH_DMRS
//                printf("\nsss: txdataF[%d] ||  pbch_dmrs_n = %d || pbch_dmrs_v = %d  || l = %d ", (l * cfg->fft_size + k) << 1,m, (amp * mod_dmrs[m << 1]) >> 15,l);
//#endif
//                txdataF[((l * cfg->fft_size + k) << 1) + 1] = (amp * mod_dmrs[(m << 1) + 1]) >> 15;
//#ifdef DEBUG_PBCH_DMRS
//                printf("\nsss: txdataF[%d] ||  pbch_dmrs_n = %d || pbch_dmrs_v = %d  || l = %d ", ((l * cfg->fft_size + k) << 1) + 1,m,(amp * mod_dmrs[(m << 1) + 1]) >> 15,l);
//#endif
//                k += 4;
//                if (k >= cfg->fft_size)
//                    k -= cfg->fft_size;
//            }
//
//
//        }
//    }



}


/*******************************************************************************************************************************************

-- TS_38211  7.3.3.1  Data Scrambling


********************************************************************************************************************************************/

void pbch_data_scrambling( uint32_t *in, // input pbch_f, after rate_match, 864 bits, will be scrapbeled
                           uint32_t pci,
                           uint8_t Lmax,
                           uint16_t length, // NR_POLAR_PBCH_E = 864 - number of bits for scrample
                           int sfn,
                           int ssb_index
                         )
{
    uint8_t reset, offset, nushift;
    uint32_t x1, x2, s = 0, unscrambling_mask, bchPayloadScrambled, M;

    //uint32_t* pbch_e = pbch->pbch_e;
    reset = 1;
    // x1 is set in lte_gold_generic
    x2 = (uint32_t)pci;

    M = length;
    nushift = (Lmax==4)? ssb_index&3 : ssb_index&7;

    // The Gold sequence is shifted by nushift* M, so we skip (nushift*M /32) double words
    for (int i = 0; i < (uint16_t)ceil(((float)nushift * M) / 32); i++)
    {
        s = lte_gold_generic(&x1, &x2, reset); //5.2.1 Pseudo-random sequence generation
        reset = 0;
    }

    // Scrambling is now done with offset (nushift*M)%32
    offset = (nushift * M) & 0x1f;
    //fp->pbch.pbch_a_prime = 0;
    //if (!encoded)

    if(1)
    {
        for (int i = 0; i < length; ++i)
        {
            //printf("second scrambl");
            if (((i + offset) & 0x1f) == 0)
            {
                s = lte_gold_generic(&x1, &x2, reset);
                reset = 0;
            }
            in[i >> 5] ^= (((s >> ((i + offset) & 0x1f)) & 1) << (i & 0x1f)); // всего 32 бита - сдвиг на 5 заполняет только fp->pbch.pbch_e[0]
        }
    }

}


/*******************************************************************************************************************************************

-- TS_38211  7.3.3.2 PBCH Data Modulation
-- QPSK modulated as described in clause 5.1.3,
resulting in a block of complex-valued modulation symbols d PBCH ( 0 ) , ... , d PBCH ( M symb −1)

********************************************************************************************************************************************/

void pbch_data_modulation( uint32_t *in, // input data after scrambling
                           int16_t *out // modulated data
                         )
{

    nr_generate_modulation_table();
    uint8_t idx;

    /// QPSK modulation
    for (int i=0; i<NR_POLAR_PBCH_E>>1; i++)
    {
        idx = ((in[(i<<1)>>5]>>((i<<1)&0x1f))&3);
        out[i<<1] = nr_qpsk_mod_table[idx<<1];
        out[(i<<1)+1] = nr_qpsk_mod_table[(idx<<1)+1];
#ifdef DEBUG
        //printf("i %d idx %d  mod_pbch %d %d\n", i, idx, mod_pbch_e[2*i], mod_pbch_e[2*i+1]);
#endif
    }


}


/*******************************************************************************************************************************************

-- TS_38211  7.3.3.3 Mapping to physical resources (pbch)
-- need position in time and freq domain

********************************************************************************************************************************************/

void pbch_data_resource_mapper( int16_t *in, // modulated data
                                int16_t *txdataF,
                                uint16_t ssb_index,
                                SSB_pattern_t SSB_pattern,
                                uint16_t ssb_subcarrier_offset,
                                uint16_t fft_shift,
                                uint16_t ssb_offset_point_a,
                                uint16_t fft_size,
                                uint16_t pci,
                                int amp
                              )
{
    uint16_t ssb_start_symbol  = nr_get_ssb_start_symbol_fromInd( ssb_index,
                                 SSB_pattern
                                                                );
    /// Resource mapping
    int nushift,k,l,m;
    nushift = pci &3;
    // PBCH modulated symbols are mapped  within the SSB block on symbols 1, 2, 3 excluding the subcarriers used for the PBCH DMRS
    ///symbol 1  [0:239] -- 180 mod symbols
    //uint16_t first_carrier_offset;
    //uint16_t ssb_start_subcarrier
    k = fft_shift +
        ssb_offset_point_a *12 +
        ssb_subcarrier_offset +
        nushift;
    //k = first_carrier_offset + ssb_start_subcarrier;

    l = ssb_start_symbol + 1;
    m = 0;

    for (int ssb_sc_idx = 0; ssb_sc_idx < 240; ssb_sc_idx++)
    {
        if ((ssb_sc_idx&3) == nushift)    //skip DMRS
        {
            k++;
            continue;
        }
        else
        {
            txdataF[(l*fft_size + k)<<1]       = (amp * in[m<<1]) >> 15;
            txdataF[((l*fft_size + k)<<1) + 1] = (amp * in[(m<<1) + 1]) >> 15;
            k++;
            m++;
        }

        if (k >= fft_size)
            k-=fft_size;
    }

    ///symbol 2  [0:47 ; 192:239] -- 72 mod symbols
    //k = first_carrier_offset + ssb_start_subcarrier;
    l++;
    m=180;

    for (int ssb_sc_idx = 0; ssb_sc_idx < 48; ssb_sc_idx++)
    {
        if ((ssb_sc_idx&3) == nushift)
        {
            k++;
            continue;
        }
        else
        {
            txdataF[(l*fft_size + k)<<1]       = (amp * in[m<<1]) >> 15;
            txdataF[((l*fft_size + k)<<1) + 1] = (amp * in[(m<<1) + 1]) >> 15;
            k++;
            m++;
        }

        if (k >= fft_size)
            k-=fft_size;
    }

    k += 144;

    if (k >= fft_size)
        k-=fft_size;

    m=216;

    for (int ssb_sc_idx = 192; ssb_sc_idx < 240; ssb_sc_idx++)
    {
        if ((ssb_sc_idx&3) == nushift)
        {
            k++;
            continue;
        }
        else
        {
            txdataF[(l*fft_size + k)<<1]       = (amp * in[m<<1]) >> 15;
            txdataF[((l*fft_size + k)<<1) + 1] = (amp * in[(m<<1) + 1]) >> 15;
            k++;
            m++;
        }

        if (k >= fft_size)
            k-=fft_size;
    }

    ///symbol 3  [0:239] -- 180 mod symbols
    //k = first_carrier_offset + ssb_start_subcarrier;
    l++;
    m=252;

    for (int ssb_sc_idx = 0; ssb_sc_idx < 240; ssb_sc_idx++)
    {
        if ((ssb_sc_idx&3) == nushift)
        {
            k++;
            continue;
        }
        else
        {
            txdataF[(l*fft_size + k)<<1]       = (amp * in[m<<1]) >> 15;
            txdataF[((l*fft_size + k)<<1) + 1] = (amp * in[(m<<1) + 1]) >> 15;
            k++;
            m++;
        }


        if (k >= fft_size)
            k-=fft_size;
    }


}

/*******************************************************************************************************************************************

-- TS_38211  5.1 Modulation mapper
-- generates tables for modulation

********************************************************************************************************************************************/

void nr_generate_modulation_table()
{
    float sqrt2 = 0.70711;
    float sqrt10 = 0.31623;
    float sqrt42 = 0.15430;
    float sqrt170 = 0.076696;
    float val = 32768.0;
    uint32_t i,j;
    short* table;

    // QPSK
    for (i=0; i<4; i++)
    {
        nr_qpsk_mod_table[i*2]   = (short)(1-2*(i&1))*val*sqrt2*sqrt2;
        nr_qpsk_mod_table[i*2+1] = (short)(1-2*((i>>1)&1))*val*sqrt2*sqrt2;
        //printf("%d j%d\n",nr_qpsk_mod_table[i*2],nr_qpsk_mod_table[i*2+1]);
    }

//#if defined(__SSE2__)
//    //QPSK m128
//    table = (short*) nr_qpsk_byte_mod_table;
//    for (i=0; i<256; i++)
//    {
//        for (j=0; j<4; j++)
//        {
//            table[i*8+(j*2)]   = (short)(1-2*((i>>(j*2))&1))*val*sqrt2*sqrt2;
//            table[i*8+(j*2)+1] = (short)(1-2*((i>>(j*2+1))&1))*val*sqrt2*sqrt2;
//            //printf("%d j%d\n",nr_qpsk_byte_mod_table[i*8+(j*2)],nr_qpsk_byte_mod_table[i*8+(j*2)+1]);
//        }
//    }
//#endif

    //16QAM
//    table = (short*) nr_16qam_byte_mod_table;
//    for (i=0; i<256; i++)
//    {
//        for (j=0; j<2; j++)
//        {
//            table[i*4+(j*2)]   = (short)((1-2*((i>>(j*4))&1))*(2-(1-2*((i>>(j*4+2))&1))))*val*sqrt10*sqrt2;
//            table[i*4+(j*2)+1] = (short)((1-2*((i>>(j*4+1))&1))*(2-(1-2*((i>>(j*4+3))&1))))*val*sqrt10*sqrt2;
//            //printf("%d j%d\n",nr_16qam_byte_mod_table[i*4+(j*2)],nr_16qam_byte_mod_table[i*4+(j*2)+1]);
//        }
//    }

    table = (short*) nr_16qam_mod_table;
    for (i=0; i<16; i++)
    {
        table[i*2]   = (short)((1-2*(i&1))*(2-(1-2*((i>>2)&1))))*val*sqrt10*sqrt2;
        table[i*2+1] = (short)((1-2*((i>>1)&1))*(2-(1-2*((i>>3)&1))))*val*sqrt10*sqrt2;
        //printf("%d j%d\n",table[i*2],table[i*2+1]);
    }

    //64QAM
    table = (short*) nr_64qam_mod_table;
    for (i=0; i<4096; i++)
    {
        for (j=0; j<2; j++)
        {
            table[i*4+(j*2)]   = (short)((1-2*((i>>(j*6))&1))*(4-(1-2*((i>>(j*6+2))&1))*(2-(1-2*((i>>(j*6+4))&1)))))*val*sqrt42*sqrt2;
            table[i*4+(j*2)+1] = (short)((1-2*((i>>(j*6+1))&1))*(4-(1-2*((i>>(j*6+3))&1))*(2-(1-2*((i>>(j*6+5))&1)))))*val*sqrt42*sqrt2;
            //printf("%d j%d\n",table[i*4+(j*2)],table[i*4+(j*2)+1]);
        }
    }

    //256QAM
    table = (short*) nr_256qam_mod_table;
    for (i=0; i<256; i++)
    {
        table[i*2]   = (short)((1-2*(i&1))*(8-(1-2*((i>>2)&1))*(4-(1-2*((i>>4)&1))*(2-(1-2*((i>>6)&1))))))*val*sqrt170*sqrt2;
        table[i*2+1] = (short)((1-2*((i>>1)&1))*(8-(1-2*((i>>3)&1))*(4-(1-2*((i>>5)&1))*(2-(1-2*((i>>7)&1))))))*val*sqrt170*sqrt2;
    }
}













