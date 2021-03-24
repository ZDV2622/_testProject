//#include <libINIT/headers/_struct_zdv_cfg.h>
//#include <libOFDM/headers/fft_ifft_radixN.h>
//#include "_nyFFT.h"
//#include <libOFDM/headers/ofdm_modulation.h>
//#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>
//#include <libTS38/headers/TS_38212_RL16_Multiplexing_channelcoding.h>
//#include <libTS38/headers/TS_38212_nr_polar_kronecker_power_matrices.h>
//#include <libORAN/headers/ORAN_BS_SW_WG8.h>

#include <libINIT/headers/_init.h>
#include <libORAN/headers/ORAN_BS_SW_WG8.h>


int16_t txdataF[200000000] = {0};
int txdata[200000000] = {0};

int FrameN = 5; // number of frames for simulation
uint32_t SFN = 0;


int main()
{
    nr_config_full_t cfg; // full configuration structure,
    oran_pbch_proc_config_t oran_pbch_cfg;

    //std::shared_ptr<vector<complex<float>>> buffer = std::make_shared<vector<complex<float>>>();

//    cfg.nfapi_nr_config_tlv.cell_config.phy_cell_id.value = 0;
//    cfg.ssb_start_ofdm_symb[0] = 2;
//    cfg.nrb_zdv = 106;//nrb;
//    //cfg.ssb_start_ofdm_symb[1] = 8;
//    cfg.band_zdv = 77;//band;
//    cfg.nfapi_nr_config_tlv.carrier_config.dl_bandwidth.value = 40;//bandwidth;
//    cfg.nfapi_nr_config_tlv.ssb_config.scs_common.value = 30;//15<<scs_index;
//    cfg.nfapi_nr_config_tlv.cell_config.frame_duplex_type.value = 1;//lte_frame_type;
//    cfg.nfapi_nr_config_tlv.carrier_config.dl_frequency_pA.value = 3300060;//dl_frequency;
//    cfg.nfapi_nr_config_tlv.carrier_config.uplink_frequency_pA.value = 3300060;//ul_frequency;
//    cfg.nfapi_nr_config_tlv.cell_config.phy_cell_id.value = 0;//pci;
//    cfg.TDD_UL_DL_configCommon.referenceSubcarrierSpacing = 30;//15<<scs_index;
//    cfg.TDD_UL_DL_configCommon.pattern1.dl_UL_TransmissionPeriodicity = ms5;//TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].dl_UL_TransmissionPeriodicity;
//    cfg.TDD_UL_DL_configCommon.pattern1.nrofDownlinkSlots = 3;//TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofDownlinkSlots;
//    cfg.TDD_UL_DL_configCommon.pattern1.nrofDownlinkSymbols = 10;//TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofDownlinkSymbols;
//    cfg.TDD_UL_DL_configCommon.pattern1.nrofUplinkSlots = 1;//TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofUplinkSlots;
//    cfg.TDD_UL_DL_configCommon.pattern1.nrofUplinkSymbols = 2;//TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofUplinkSymbols;
//    cfg.TDD_UL_DL_configCommon_ind_zdv;
//    cfg.scs_ssb_zdv = 30;//scs_ssb;
//    cfg.SSB_pattern_zdv = ssb_case_C;//ssb_pattern;
//    cfg.fft_size = 2048;
//    cfg.fft_shift_zdv = cfg.fft_size - cfg.nrb_zdv*12/2; //1412;
//    //int txdataF;
//    printf("test %d \n", cfg.fft_shift_zdv);

    // variables to be set by po


    INIT_CELL_CONFIG(&cfg, &oran_pbch_cfg);
    //INIT_CELL_CONFIG(&cfg);
    int flag;
    std::cin >> flag;

    // Process options

    while(FrameN>0)
    {
        /// generate data for 1 SFN start/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        printf("\nSFN  = %d\n", SFN);
//
//        nr_generate_pss(&cfg,
//                        &txdataF[0],
//                        512 // amp
//                       );
//
//        nr_generate_sss(&cfg,
//                        &txdataF[0],
//                        512 // amp
//                       );
//
//        nr_generate_pbch_dmrs(&cfg,
//                              &txdataF[0],
//                              512 // amp
//                             );

        Oran_Pbch_proc(SFN, &oran_pbch_cfg, &txdataF[0]);

        printf("\n txdataF: \n ");
        for (int i = 0; i < 2000000; ++i)
            if (txdataF[i] != 0)
                printf("[%d] = %hi    ", i,txdataF[i]);


        FrameN = FrameN-1;
        SFN = SFN+1;



    }

//free_buff:
//    free(buff);
//
//
//free_tx_streamer:
//    if(verbose)
//    {
//        fprintf(stderr, "Cleaning up TX streamer.\n");
//    }
//    uhd_tx_streamer_free(&tx_streamer);
//
//free_tx_metadata:
//    if(verbose)
//    {
//        fprintf(stderr, "Cleaning up TX metadata.\n");
//    }
//    uhd_tx_metadata_free(&md);
//
//free_usrp:
//    if(verbose)
//    {
//        fprintf(stderr, "Cleaning up USRP.\n");
//    }
//    if(return_code != EXIT_SUCCESS && usrp != NULL)
//    {
//        uhd_usrp_last_error(usrp, error_string, 512);
//        fprintf(stderr, "USRP reported the following error: %s\n", error_string);
//    }
//    uhd_usrp_free(&usrp);
//
//free_option_strings:
//    if(device_args)
//        free(device_args);




    printf("\n txdataF: \n ");
    for (int i = 0; i < 512*2*1000; ++i)
        if (txdataF[i] != 0)
            printf("[%d] = %hi    ", i,txdataF[i]);

//
    printf("\n txdata: \n ");
    for (int i = cfg.fft_size*2*2; i < cfg.fft_size*2*3; ++i)
        if (txdata[i] != 0)
            printf("[%d] = %hi    ", i,txdata[i]);


//    for(int i = 2136*4; i < 2136*4 +cfg.fft_size*2+cfg.nb_prefix_samples0; i = i+2)
//    {
//        if ((txdata[2*i] != 0) | (txdata[2*i+1] != 0))
//        buffer->push_back(std::complex<float>(txdata[2*i], txdata[2*i+1]));
//        buffer->push_back(std::complex<int>(txdata[i], txdata[i+1]));
//    }
//
//    printf("\nbuffersize = %d", buffer->size());
//
//    std::ofstream rawFile;
//    rawFile.open("txdataCd1.dat",std::ios::binary);
//    rawFile.write((const char*)(&txdata[cfg.fft_size*2*2]), cfg.fft_size*2*sizeof(int));
//    for(int i = 2136*4; i < 2136*4 +cfg.fft_size*2+cfg.nb_prefix_samples0; ++i)
//        for(unsigned int i=0; i<1000; i++)
//    {
//        printf("\n reinterpret_cast = %d  v = %s", i,reinterpret_cast<const char*>(&buffer->at(i)), sizeof(std::complex<int>));
//        rawFile.write(reinterpret_cast<const char*>(&txdata[i]), sizeof(int));
//        rawFile.write(reinterpret_cast<const char*>(&txdata[i]), sizeof(int));
//
//
//    }
//    rawFile.close();

//    idft(IDFT_128,(int16_t*)&txdataF[0],(int16_t*)&txdata[0],1);
//
//    printf("\n txdata: \n ");
//    for (int i = 0; i < 256; ++i)
//        if (txdata[i] != 0)
//            printf("[%d] = %hi    ", i,txdata[i]);

//    FILE *txdataFFile;
//    txdataFFile = fopen("txdata.txt","w");
//    for(int i=8656;i<8656+4096+cfg.nb_prefix_samples;++i){
//        fprintf(txdataFFile,"%i\n",txdata[i]);
//    };

//
//    printf("\ncfg.samples_per_frame  = %d\n", cfg.samples_per_frame);

    //std::complex<double> *txdataC=new std::complex<double>[cfg.fft_size*20];
    //std::complex<double> *tab3=new std::complex<double>[N];

//    for(unsigned int i=0; i<2000; i++)
//    {
//        if (txdata[2*i] != 0)
//            buffer->push_back(std::complex<float>(txdata[2*i], txdata[2*i+1]));
//    }
//
//    std::string binFileName = "txdataCb.dat";
//    std::ofstream rawFile(binFileName, std::ios::binary,std::ios::app);
//    for(unsigned int i=0; i<20; i++)
//    {
//        rawFile.write(reinterpret_cast<const char*>(&buffer->at(i)), sizeof(std::complex<float>));
//    }
//    rawFile.close();



//    printf("\n txdataF_test: \n ");
//    for (int i = 0; i < fftsize; ++i)
//    {
//        printf("[%d] = %hi    ", i,txdataFtest[i]);
//    }
//
//    idft128(&txdataFtest[0],&txdatatest[0],1);
//
//    printf("\n\n txdata_test: \n ");
//    for (int i = 0; i < fftsize; ++i)
//    {
//    if (txdatatest[i] !=0)
//        printf("[%d] = %hi    ", i,txdatatest[i]);
//    }
//
//    dft128((int16_t*)&txdatatest[0],&txdataFtest1[0],1);
//
//    printf("\n\n txdataF_test1: \n ");
//    for (int i = 0; i < fftsize; i++)
//    {
//    if (txdataFtest1[i] !=0)
//        printf("[%d] = %hi    ", i,(int16_t)txdataFtest1[i]);
//    }

//    int fftsize = 4096;
//    printf("\n init test: \n ");
//    int16_t txdataFtest[fftsize] = {0};
//    int16_t txdatatest[fftsize] = {0};
//    int16_t txdataFtest1[fftsize] = {0};
//    int16_t txdatatest1[fftsize] = {0};
//



//    PHY_ofdm_mod((int*)&txdataFtest[0],        // input
//                 (int*)&txdatatest[0],         // output
//                 128,
//                 1,
//                 0,               // number of prefix samples
//                 CYCLIC_PREFIX);



//    for(int i=256; i<fftsize; i++)
//    {
//        if(i%2!=0)
//        {
//            txdataFtest[i] = i;
//        }
//        else txdataFtest[i] = i;
//
//    }








    //txdatatest[0] = 0;
    //txdatatest[64] = 0;


//    dft(DFT_256,(int16_t*)&txdatatest[0],&txdataFtest1[0],1);
//    int fftsize = 256;//cfg.fft_size;
//
//    for(int i=0; i<fftsize; i++)
//    {
//        if(i%2!=0)
//        {
//            txdataF[i] = i;
//        }
//        else
//            txdataF[i] = i;
//
//    }
//
//    fft_ifft_radixN(&txdataF[0],&txdata[0],fftsize,0);
//
//    idft(IDFT_128,(int16_t*)&txdataF[0],(int16_t*)&txdata[0],1);
//
//    dft(DFT_128,(int16_t*)&txdata[0],(int16_t*)&txdataF_1[0],1);
//



    return 0;
}
