#pragma once
#include <libNFAPI/headers/FAPI_SCF_222.h>


///---------------------------------------------      TDD-UL-DL-ConfigCommon         -------------------------------------------------------
///The IE TDD-UL-DL-ConfigCommon determines the cell specific Uplink/Downlink TDD configuration

typedef enum {
    ms0p5 = 500,                 /* duration is given in microsecond */
    ms0p625 = 625,
    ms1 = 1000,
    ms1p25 = 1250,
    ms2 = 2000,
    ms2p5 = 2500,
    ms5 = 5000,
    ms10 = 10000,
} dl_UL_TransmissionPeriodicity_t;

typedef enum {
    ms3 = 3000,
    ms4 = 4000
} dl_UL_TransmissionPeriodicity_v1530_t;

typedef struct TDD_UL_DL_Pattern_s {
    /// \ Periodicity of the DL-UL pattern. Corresponds to L1 parameter 'DL-UL-transmission-periodicity' (see 38.211, section FFS_Section)
    dl_UL_TransmissionPeriodicity_t dl_UL_TransmissionPeriodicity;
    /// \ Number of consecutive full DL slots at the beginning of each DL-UL pattern.
    /// Corresponds to L1 parameter 'number-of-DL-slots' (see 38.211, Table 4.3.2-1)
    uint8_t nrofDownlinkSlots;
    /// \ Number of consecutive DL symbols in the beginning of the slot following the last full DL slot (as derived from nrofDownlinkSlots).
    /// If the field is absent or released, there is no partial-downlink slot.
    /// Corresponds to L1 parameter 'number-of-DL-symbols-common' (see 38.211, section FFS_Section).
    uint8_t nrofDownlinkSymbols;
    /// \ Number of consecutive full UL slots at the end of each DL-UL pattern.
    /// Corresponds to L1 parameter 'number-of-UL-slots' (see 38.211, Table 4.3.2-1)
    uint8_t nrofUplinkSlots;
    /// \ Number of consecutive UL symbols in the end of the slot preceding the first full UL slot (as derived from nrofUplinkSlots).
    /// If the field is absent or released, there is no partial-uplink slot.
    /// Corresponds to L1 parameter 'number-of-UL-symbols-common' (see 38.211, section FFS_Section)
    uint8_t nrofUplinkSymbols;
    /// \ for setting a sequence
    //struct TDD_UL_DL_configCommon_s* p_next;
    ///Periodicity of the DL-UL pattern, see TS 38.213 [13], clause 11.1. If the dl-UL-TransmissionPeriodicity-v1530 is signalled,
    ///UE shall ignore the dl-UL-TransmissionPeriodicity (without suffix).
    dl_UL_TransmissionPeriodicity_v1530_t dl_UL_TransmissionPeriodicity_v1530;
} TDD_UL_DL_Pattern_t;

typedef struct TDD_UL_DL_configCommon_s {
    /// Reference SCS used to determine the time domain boundaries in the UL-DL pattern which must be common across all subcarrier specific
    /// virtual carriers, i.e., independent of the actual subcarrier spacing using for data transmission.
    /// Only the values 15 or 30 kHz  (<6GHz), 60 or 120 kHz (>6GHz) are applicable.
    /// Corresponds to L1 parameter 'reference-SCS' (see 38.211, section FFS_Section)
    /// \ subcarrier spacing
    uint8_t referenceSubcarrierSpacing;
    TDD_UL_DL_Pattern_t pattern1;
    TDD_UL_DL_Pattern_t pattern2;  //OPTIONAL, -- Need R
    //struct TDD_UL_DL_configCommon_s* p_next;
} TDD_UL_DL_configCommon_t;
