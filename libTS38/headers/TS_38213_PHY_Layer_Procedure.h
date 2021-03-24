#pragma once

#include <libNFAPI/headers/FAPI_SCF_222.h>
#include <libTS38/headers/TS_38104_BS_Radio_tx_rx_table.h>
#include <libTS38/headers/TS_38331_RL16_RRC.h>
#include <libTS38/headers/TS_38101_4_UE_tx_rx.h>
#include <libTS38/headers/TS_38213_PHY_Layer_Procedure.h>
#include <libINIT/headers/_struct_zdv_cfg.h>

// for help
typedef struct get_ssb_start_symbol_s{
    int lmax;
    int ssb_start_ofdm_symb[64];
}get_ssb_start_symbol_t;

void nr_get_ssb_start_symbol(SSB_pattern_t SSB_pattern,
                                          int freq,
                                          //int ssb_index,
                                          get_ssb_start_symbol_t *cfg);


uint16_t nr_get_ssb_start_symbol_fromInd(uint16_t ssb_index,
                                         SSB_pattern_t SSB_pattern  // caseA, B,///
                                        );

uint16_t nr_get_ssb_Lmax(int freq, // supported frequency
                         SSB_pattern_t SSB_pattern
                        );
