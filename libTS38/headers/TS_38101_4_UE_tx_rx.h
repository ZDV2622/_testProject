#ifndef ADD_TS38101_4_UE_tx_rx_H
#define ADD_TS38101_4_UE_tx_rx_H


#include <libTS38/headers/TS_38331_RL16_RRC.h>

/// Table A.1.2-1: TDD UL-DL configuration for SCS 15 kHz  ---------------------------------------------------------------------------------------------------
/// Table A.1.2-2: TDD UL-DL configuration for SCS 30 kHz  ---------------------------------------------------------------------------------------------------

typedef struct TDD_UL_DL_configuration_s
{
    int16_t referenceSubcarrierSpacing; //kHz
    int16_t pattern; // 1/2
    dl_UL_TransmissionPeriodicity_t dl_UL_TransmissionPeriodicity;
    int16_t nrofDownlinkSlots;
    int16_t nrofDownlinkSymbols;
    int16_t nrofUplinkSlots;
    int16_t nrofUplinkSymbols;
    char TDD_Slot_Configuration_pattern[10];
    char Special_Slot_Configuration[14];
} TDD_UL_DL_configuration_t;


static const TDD_UL_DL_configuration_t TDD_UL_DL_configuration_tabl[] =
{
    {15, 1, ms5, 3, 10, 1, 2, {'D','D','D','S','U','D','D','D','S','U'},{'D','D','D','D','D','D','D','D','D','D','G','G','U','U'} }, // FR1.15-1
    {30, 1, ms5, 3, 10, 1, 2, {'D','D','D','D','D','D','D','S','S','U'},{'D','D','D','D','D','D','G','G','G','G','U','U','U','U'} },
    {30, 1, ms2p5, 3, 10, 1, 2, {'D','D','D','S','U','D','D','D','S','U'},{'D','D','D','D','D','D','D','D','D','D','G','G','U','U'}},
    {30, 1, ms2p5, 3, 10, 1, 2, {'D','D','D','S','U','D','D','S','U','U'},{'D','D','D','D','D','D','D','D','D','D','G','G','U','U'}}
};


#endif // ADD_TS38101_4_UE_tx_rx_H
