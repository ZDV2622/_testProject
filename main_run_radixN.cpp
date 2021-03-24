//#include <stdio.h>
//#include <iostream>
#include <libINIT/headers/_struct_zdv_cfg.h>
#include <libINIT/headers/_init.h>
//#include <libOFDM/headers/_fft.h>
#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>
#include <tmmintrin.h>

//#include "_nyFFT.h"
#include <libOFDM/headers/fft_ifft_radixN.h>
#include <libOFDM/headers/ofdm_modulation.h>

#include <complex>
#include <iostream>
#include <memory>
#include <fstream>
#include <iterator>
#include <random>
#include <vector>




#include <uhd.h>

#include "getopt.h"

#include <math.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/utils/thread.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>

#define EXECUTE_OR_GOTO(label, ...) \
    if(__VA_ARGS__){ \
        return_code = EXIT_FAILURE; \
        goto label; \
    }


using namespace std;
std::shared_ptr<vector<std::complex<float>>> buffer = std::make_shared<vector<std::complex<float>>>();

//int16_t txdataF_1[20000000] = {0};
//int16_t txdataF[20000000] = {0};
//int16_t txdata[20000000] = {0};

namespace po = boost::program_options;


bool stop_signal_called = false;

void sigint_handler(int code)
{
    (void)code;
    stop_signal_called = true;
}


//fprintf(stderr, "Creating USRP with args \"%s\"...\n", device_args);


//int txdataF_1[20000000] = {0};
int txdataF[200000000] = {0};
int txdata[200000000] = {0};

int FrameN = 1000;
int SFN = 0;


int main(int argc, char* argv[])
{
    nr_config_full_t cfg;

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



    INIT_CELL_CONFIG(&cfg);
    int flag;
    std::cin >> flag;

    size_t samps_per_buff;
    float* buff = NULL;
    const void** buffs_ptr = NULL;
    // Process options

    while(FrameN>0)
    {
        /// generate data for 1 SFN start/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        printf("\nSFN  = %d\n", SFN);

        nr_generate_pss(&cfg,
                        &txdataF[0],
                        512 // amp
                       );

        nr_generate_sss(&cfg,
                        &txdataF[0],
                        512 // amp
                       );

        nr_generate_pbch_dmrs(&cfg,
                              &txdataF[0],
                              512 // amp
                             );


        int OFDM_SYMBOL_SIZE_COMPLEX_SAMPLES0 = (2*cfg.fft_size + cfg.nb_prefix_samples0);
        printf("\nOFDM_SYMBOL_SIZE_COMPLEX_SAMPLES0 = %d\n", OFDM_SYMBOL_SIZE_COMPLEX_SAMPLES0);
        PHY_ofdm_mod_radixN(&txdataF[0],        // input
                            &txdata[0],         // output
                            cfg.fft_size,
                            1,                 // number of symbols
                            cfg.nb_prefix_samples0,               // number of prefix samples
                            CYCLIC_PREFIX);
        printf("\nOFDM_SYMBOL 1-7n");
        PHY_ofdm_mod_radixN(&txdataF[cfg.fft_size*2],        // input
                            &txdata[OFDM_SYMBOL_SIZE_COMPLEX_SAMPLES0],         // output
                            cfg.fft_size,
                            6,
                            cfg.nb_prefix_samples,               // number of prefix samples
                            CYCLIC_PREFIX);


        FrameN = FrameN-1;
        SFN = SFN+1;


        /// txdata to file after each SFN
        for(int i = 0; i < cfg.samples_per_frame-1; i=i+2)
        {
            //if ((txdata[2*i] != 0) | (txdata[2*i+1] != 0))
            buffer->push_back(std::complex<float>(txdata[i], txdata[i+1]));
        }
        std::ofstream rawFile;
        rawFile.open("txdataC.dat",std::ios::binary);
        for(unsigned int i=0; i<buffer->size(); i++)
        {
            rawFile.write(reinterpret_cast<const char*>(&buffer->at(i)), sizeof(std::complex<float>));
        }
        rawFile.close();

        /// generate data for 1 SFN stop/////////////////////////////////////////////////////////////////////////////////////////////////////////////


        // variables to be set by po
        std::string args;
        std::string wire;
        double seconds_in_future;
        size_t total_num_samps;
        double rate, freq, lo_offset;
        float ampl;

        // setup the program options
        po::options_description desc("Allowed options");
        // clang-format off
        desc.add_options()
        ("help", "help message")
        ("args", po::value<std::string>(&args)->default_value(""), "single uhd device address args")
        ("wire", po::value<std::string>(&wire)->default_value(""), "the over the wire type, sc16, sc8, etc")
        ("secs", po::value<double>(&seconds_in_future)->default_value(1.5), "number of seconds in the future to transmit")
        ("nsamps", po::value<size_t>(&total_num_samps)->default_value(10000), "total number of samples to transmit")
        ("rate", po::value<double>(&rate)->default_value(100e6/16), "rate of outgoing samples")
        ("ampl", po::value<float>(&ampl)->default_value(float(0.3)), "amplitude of each sample")
        ("freq", po::value<double>(&freq)->default_value(float(900000000)), "RF center frequency in Hz")
        ("freq", po::value<double>(&lo_offset)->default_value(float(1)), "offset")
        ("dilv", "specify to disable inner-loop verbose")
        ;
        // clang-format on
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        // print the help message
        if (vm.count("help"))
        {
            std::cout << boost::format("UHD TX Timed Samples %s") % desc << std::endl;
            return ~0;
        }

        bool verbose = vm.count("dilv") == 0;

        // create a usrp device
        std::cout << std::endl;
        std::cout << boost::format("Creating the usrp device with: %s...") % args
                  << std::endl;
        uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args);
        std::cout << boost::format("Using Device: %s") % usrp->get_pp_string() << std::endl;



        // set the center frequency
        if (not vm.count("freq"))
        {
            std::cerr << "Please specify the center frequency with --freq" << std::endl;
            return ~0;
        }
        std::cout << boost::format("Setting TX Freq: %f MHz...") % (freq / 1e6) << std::endl;
        std::cout << boost::format("Setting TX LO Offset: %f MHz...") % (lo_offset / 1e6)
                  << std::endl;
        uhd::tune_request_t tune_request;
        tune_request = uhd::tune_request_t(freq, lo_offset);
        //if (vm.count("int-n"))
            //tune_request.args = uhd::device_addr_t("mode_n=integer");
        usrp->set_tx_freq(tune_request);
        std::cout << boost::format("Actual TX Freq: %f MHz...") % (usrp->get_tx_freq() / 1e6)
                  << std::endl
                  << std::endl;



        // set the tx sample rate
        std::cout << boost::format("Setting TX Rate: %f Msps...") % (rate / 1e6) << std::endl;
        usrp->set_tx_rate(rate);
        std::cout << boost::format("Actual TX Rate: %f Msps...") % (usrp->get_tx_rate() / 1e6)
                  << std::endl
                  << std::endl;

        std::cout << boost::format("Setting device timestamp to 0...") << std::endl;
        usrp->set_time_now(uhd::time_spec_t(0.0));

        // create a transmit streamer
        uhd::stream_args_t stream_args("fc32", wire); // complex floats
        uhd::tx_streamer::sptr tx_stream = usrp->get_tx_stream(stream_args);

        // allocate buffer with data to send
//        std::vector<std::complex<float>> buff(
//                                          tx_stream->get_max_num_samps(), std::complex<float>(ampl, ampl));


        /// ------------------------------------------------------------------------------------
        //printf("\nbuffer set start\n");
        size_t samps_per_buff;
        float* buffer = NULL;
        const void** buffs_ptr = NULL;
        samps_per_buff = (size_t)(cfg.samples_per_frame-1);
        buffer = (float*)calloc(sizeof(float), samps_per_buff+1);
        buffs_ptr = (const void**)&buff;
        size_t i = 0;

        for(i = 0; i < (samps_per_buff); i+=2)
        {
            buffer[i]   = (float)txdata[i];
            buffer[i+1] = (float)txdata[i+1];
        }
        //printf("\nbuffer set end\n");

        /// ---------------------------------------------------------------------------------

        // setup metadata for the first packet
        uhd::tx_metadata_t md;
        md.start_of_burst = false;
        md.end_of_burst   = false;
        md.has_time_spec  = true;
        md.time_spec      = uhd::time_spec_t(seconds_in_future);

        // the first call to send() will block this many seconds before sending:
        const double timeout =
            seconds_in_future + 0.1; // timeout (delay before transmit + padding)

        size_t num_acc_samps = 0; // number of accumulated samples
        //printf("\nbefor while\n");
        //total_num_samps = samps_per_buff;

        for(int i=0; i<samps_per_buff-total_num_samps-total_num_samps; i=i+total_num_samps)
        {
            size_t num_tx_samps = tx_stream->send(&buffer[i], total_num_samps, md, timeout);
            //printf("    samples send in SFN[%d]  |||  now sent = %i    ||||| total buff = [%d]", SFN,i,samps_per_buff);
        }
//        while (num_acc_samps < total_num_samps)
//        {
//        printf("\nin while\n");
//            //size_t samps_to_send = std::min(total_num_samps - num_acc_samps, buff.size());
//
//            // send a single packet
//            //size_t num_tx_samps = tx_stream->send(&buff.front(), samps_to_send, md, timeout);
//
//            /// //////////////////////////////////////
//
//            //size_t samps_to_send = std::min(total_num_samps - num_acc_samps, samps_per_buff);
//            printf("samples to send = %d", samps_to_send);
//            size_t buffer_ind = 0;
//            // send a single packet
//            size_t num_tx_samps = tx_stream->send(&buffer[0], samps_to_send, md, timeout);
//            buffer_ind = buffer_ind+num_tx_samps;
//            /// //////////////////////////////////////
//
//
//            // do not use time spec for subsequent packets
//            md.has_time_spec = false;
//
//            if (num_tx_samps < samps_to_send)
//                std::cerr << "Send timeout..." << std::endl;
//            if (verbose)
//                std::cout << boost::format("Sent packet: %u samples") % num_tx_samps
//                          << std::endl;
//
//            num_acc_samps += num_tx_samps;
//        }


        /// /////////////////////////////////  end send data and free buffer

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
    for (int i = cfg.fft_size*2*2; i < cfg.fft_size*2*3; ++i)
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
