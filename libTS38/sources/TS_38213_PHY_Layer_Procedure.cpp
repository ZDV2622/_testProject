#include <libTS38/headers/TS_38213_PHY_Layer_Procedure.h>

//#include <libNFAPI/headers/FAPI_SCF_222.h>
//#include <libTS38/headers/TS_38104_BS_Radio_tx_rx_table.h>
//#include <libTS38/headers/TS_38331_RL16_RRC.h>
//#include <libTS38/headers/TS_38101_4_UE_tx_rx.h>
//#include <libTS38/headers/TS_38213_PHY_Layer_Procedure.h>

#include <stdio.h>

/*******************************************************************************************************************************************

-- 4.1 Cell search (ssb position in time domain, Lmax)
-- algorithm described in 4.1:
-- SSB pattern and frequency will give position of SSB in time  domain and Lmax- max number of SSB which can be configured
-- nr_get_ssb_start_symbol return ofdm start symbol for each ssb index ssb_start_ofdm_symb[Lmax], and Lmax - return structure
-- nr_get_ssb_start_symbol_fromInd - return ssb_start_symbol
-- nr_get_ssb_Lmax - return Lmax

********************************************************************************************************************************************/

void nr_get_ssb_start_symbol(SSB_pattern_t SSB_pattern,
                             int freq,
                             //int ssb_index,
                             //int fgonf;
                             get_ssb_start_symbol_t *cfg)
{
    //int mu = frame_params->numerology_index;
    //int i_ssb = ssb_index;
    int Lmax;
    int symbol = 0;
    uint8_t n, n_temp;
    int case_AC[2] = { 2,8 };
    int case_BD[4] = { 4,8,16,20 };
    int case_E[8] = { 8, 12, 16, 20, 32, 36, 40, 44 };
    Lmax = 0;


    // time domain, Lmax - число ssb в фрейме, определяется типом ssb и dl_CarrierFreq
    // определим L_max и запишем
    float freq_Ghz = freq;//1000000;
    if (freq_Ghz < 6000000)
    {
        if (SSB_pattern == 2)
            Lmax = (freq_Ghz < 2400000) ? 4 : 8;
        else
            Lmax = (freq_Ghz < 3000000) ? 4 : 8;
    }
    else
    {
        Lmax = 64;
    }

    bool ssb_index_mask[64] = {};
    int ssb_start_ofdm_symb[64] = {};

    switch (SSB_pattern)
    {
    case 0: // case A
        //n = i_ssb >> 1;
        //symbol = case_AC[i_ssb % 2] + 14 * n;
        for(int i = 0; i<Lmax; i++)
        {
            //if(ssb_index_mask[i] != 0)
            {
                n = i >> 1;
                ssb_start_ofdm_symb[i] = case_AC[i % 2] + 14 * n;
            }
        }
        break;
    case 1:
        //n = i_ssb >> 2;
        //symbol = case_BD[i_ssb % 4] + 28 * n;
        for(int i = 0; i<Lmax; i++)
        {
            //if(ssb_index_mask[i] != 0)
            {
                n = i >> 2;
                ssb_start_ofdm_symb[i] = case_BD[i % 4] + 28 * n;
            }
        }
        break;
    case 2: // case C
        //n = i_ssb >> 1;
        //symbol = case_AC[i_ssb % 2] + 14 * n;
        for(int i = 0; i<Lmax; i++)
        {
            //if(ssb_index_mask[i] != 0)
            {
                n = i >> 1;
                ssb_start_ofdm_symb[i] = case_AC[i % 2] + 14 * n;
            }
        }
        break;
    case 3: // case D
        //n_temp = i_ssb >> 2;
        //n = n_temp + (n_temp >> 2);
        //symbol = case_BD[i_ssb % 4] + 28 * n;
        for(int i = 0; i<Lmax; i++)
        {
            //if(ssb_index_mask[i] != 0)
            {
                n_temp = i >> 2;
                n = n_temp + (n_temp >> 2);
                ssb_start_ofdm_symb[i] = case_BD[i % 4] + 28 * n;
            }
        }
        break;
    case 4:  // case E
        //n_temp = i_ssb >> 3;
        //n = n_temp + (n_temp >> 2);
        //symbol = case_E[i_ssb % 8] + 56 * n;
        for(int i = 0; i<Lmax; i++)
        {
            n_temp = i >> 3;
            n = n_temp + (n_temp >> 2);
            //if(ssb_index_mask[i] != 0)
            {
                ssb_start_ofdm_symb[i] = case_E[i % 4] + 56 * n;
            }
        }
        break;
    }


    cfg->lmax = Lmax;
    for(int i = 0; i<Lmax; i++)
    {
        cfg->ssb_start_ofdm_symb[i] = ssb_start_ofdm_symb[i];
    }

}


uint16_t nr_get_ssb_start_symbol_fromInd(uint16_t ssb_index,
                                         SSB_pattern_t SSB_pattern
                                        )
{
    //int mu = frame_params->numerology_index;
    //int i_ssb = ssb_index;

    uint8_t n, n_temp;
    int case_AC[2] = { 2,8 };
    int case_BD[4] = { 4,8,16,20 };
    int case_E[8] = { 8, 12, 16, 20, 32, 36, 40, 44 };

    uint16_t i = ssb_index;
    int ssb_start_ofdm_symb=0;

    switch (SSB_pattern)
    {
    case 0: // case A
        n = i >> 1;
        ssb_start_ofdm_symb = case_AC[i % 2] + 14 * n;
        break;
    case 1:
        n = i >> 2;
        ssb_start_ofdm_symb = case_BD[i % 4] + 28 * n;
        break;
    case 2: // case C
        n = i >> 1;
        ssb_start_ofdm_symb = case_AC[i % 2] + 14 * n;
        break;
    case 3: // case D
        n_temp = i >> 2;
        n = n_temp + (n_temp >> 2);
        ssb_start_ofdm_symb = case_BD[i % 4] + 28 * n;
        break;
    case 4:  // case E
        n_temp = i >> 3;
        n = n_temp + (n_temp >> 2);
        ssb_start_ofdm_symb = case_E[i % 4] + 56 * n;
        break;
    }

    return ssb_start_ofdm_symb;
}


uint16_t nr_get_ssb_Lmax(int freq, // supported frequency in Mhz
                         SSB_pattern_t SSB_pattern
                        )
{
    //int mu = frame_params->numerology_index;
    //int i_ssb = ssb_index;
    uint16_t Lmax;


    // time domain, Lmax - число ssb в фрейме, определяется типом ssb и dl_CarrierFreq
    // определим L_max и запишем
    float freq_Ghz = freq;//1000000;
    if (freq_Ghz < 6000000)
    {
        if (SSB_pattern == 2)
            Lmax = (freq_Ghz < 2400000) ? 4 : 8;
        else
            Lmax = (freq_Ghz < 3000000) ? 4 : 8;
    }
    else
    {
        Lmax = 64;
    }

    return Lmax;
}

