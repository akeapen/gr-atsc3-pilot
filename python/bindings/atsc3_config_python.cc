/*
 * Copyright 2023 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(atsc3_config.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(b0be038f88c3ec79d5b0585c0f4d93ae)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <atsc3/atsc3_config.h>
// pydoc.h is automatically generated in the build directory
#include <atsc3_config_pydoc.h>

void bind_atsc3_config(py::module& m)
{


    py::enum_<::gr::atsc3::atsc3_code_rate_t>(m,"atsc3_code_rate_t")
        .value("C2_15", ::gr::atsc3::atsc3_code_rate_t::C2_15) // 0
        .value("C3_15", ::gr::atsc3::atsc3_code_rate_t::C3_15) // 1
        .value("C4_15", ::gr::atsc3::atsc3_code_rate_t::C4_15) // 2
        .value("C5_15", ::gr::atsc3::atsc3_code_rate_t::C5_15) // 3
        .value("C6_15", ::gr::atsc3::atsc3_code_rate_t::C6_15) // 4
        .value("C7_15", ::gr::atsc3::atsc3_code_rate_t::C7_15) // 5
        .value("C8_15", ::gr::atsc3::atsc3_code_rate_t::C8_15) // 6
        .value("C9_15", ::gr::atsc3::atsc3_code_rate_t::C9_15) // 7
        .value("C10_15", ::gr::atsc3::atsc3_code_rate_t::C10_15) // 8
        .value("C11_15", ::gr::atsc3::atsc3_code_rate_t::C11_15) // 9
        .value("C12_15", ::gr::atsc3::atsc3_code_rate_t::C12_15) // 10
        .value("C13_15", ::gr::atsc3::atsc3_code_rate_t::C13_15) // 11
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_code_rate_t>();
    py::enum_<::gr::atsc3::atsc3_framesize_t>(m,"atsc3_framesize_t")
        .value("FECFRAME_SHORT", ::gr::atsc3::atsc3_framesize_t::FECFRAME_SHORT) // 0
        .value("FECFRAME_NORMAL", ::gr::atsc3::atsc3_framesize_t::FECFRAME_NORMAL) // 1
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_framesize_t>();
    py::enum_<::gr::atsc3::atsc3_constellation_t>(m,"atsc3_constellation_t")
        .value("MOD_QPSK", ::gr::atsc3::atsc3_constellation_t::MOD_QPSK) // 0
        .value("MOD_16QAM", ::gr::atsc3::atsc3_constellation_t::MOD_16QAM) // 1
        .value("MOD_64QAM", ::gr::atsc3::atsc3_constellation_t::MOD_64QAM) // 2
        .value("MOD_256QAM", ::gr::atsc3::atsc3_constellation_t::MOD_256QAM) // 3
        .value("MOD_1024QAM", ::gr::atsc3::atsc3_constellation_t::MOD_1024QAM) // 4
        .value("MOD_4096QAM", ::gr::atsc3::atsc3_constellation_t::MOD_4096QAM) // 5
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_constellation_t>();
    py::enum_<::gr::atsc3::atsc3_guardinterval_t>(m,"atsc3_guardinterval_t")
        .value("GI_RESERVED", ::gr::atsc3::atsc3_guardinterval_t::GI_RESERVED) // 0
        .value("GI_1_192", ::gr::atsc3::atsc3_guardinterval_t::GI_1_192) // 1
        .value("GI_2_384", ::gr::atsc3::atsc3_guardinterval_t::GI_2_384) // 2
        .value("GI_3_512", ::gr::atsc3::atsc3_guardinterval_t::GI_3_512) // 3
        .value("GI_4_768", ::gr::atsc3::atsc3_guardinterval_t::GI_4_768) // 4
        .value("GI_5_1024", ::gr::atsc3::atsc3_guardinterval_t::GI_5_1024) // 5
        .value("GI_6_1536", ::gr::atsc3::atsc3_guardinterval_t::GI_6_1536) // 6
        .value("GI_7_2048", ::gr::atsc3::atsc3_guardinterval_t::GI_7_2048) // 7
        .value("GI_8_2432", ::gr::atsc3::atsc3_guardinterval_t::GI_8_2432) // 8
        .value("GI_9_3072", ::gr::atsc3::atsc3_guardinterval_t::GI_9_3072) // 9
        .value("GI_10_3648", ::gr::atsc3::atsc3_guardinterval_t::GI_10_3648) // 10
        .value("GI_11_4096", ::gr::atsc3::atsc3_guardinterval_t::GI_11_4096) // 11
        .value("GI_12_4864", ::gr::atsc3::atsc3_guardinterval_t::GI_12_4864) // 12
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_guardinterval_t>();
    py::enum_<::gr::atsc3::atsc3_mimo_scattered_pilot_encoding_t>(m,"atsc3_mimo_scattered_pilot_encoding_t")
        .value("MSPE_WALSH_HADAMARD_PILOTS", ::gr::atsc3::atsc3_mimo_scattered_pilot_encoding_t::MSPE_WALSH_HADAMARD_PILOTS) // 0
        .value("MSPE_NULL_PILOTS", ::gr::atsc3::atsc3_mimo_scattered_pilot_encoding_t::MSPE_NULL_PILOTS) // 1
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_mimo_scattered_pilot_encoding_t>();
    py::enum_<::gr::atsc3::atsc3_time_info_flag_t>(m,"atsc3_time_info_flag_t")
        .value("TIF_NOT_INCLUDED", ::gr::atsc3::atsc3_time_info_flag_t::TIF_NOT_INCLUDED) // 0
        .value("TIF_MS_PRECISION", ::gr::atsc3::atsc3_time_info_flag_t::TIF_MS_PRECISION) // 1
        .value("TIF_US_PRECISION", ::gr::atsc3::atsc3_time_info_flag_t::TIF_US_PRECISION) // 2
        .value("TIF_NS_PRECISION", ::gr::atsc3::atsc3_time_info_flag_t::TIF_NS_PRECISION) // 3
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_time_info_flag_t>();
    py::enum_<::gr::atsc3::atsc3_papr_t>(m,"atsc3_papr_t")
        .value("PAPR_OFF", ::gr::atsc3::atsc3_papr_t::PAPR_OFF) // 0
        .value("PAPR_TR", ::gr::atsc3::atsc3_papr_t::PAPR_TR) // 1
        .value("PAPR_ACE", ::gr::atsc3::atsc3_papr_t::PAPR_ACE) // 2
        .value("PAPR_BOTH", ::gr::atsc3::atsc3_papr_t::PAPR_BOTH) // 3
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_papr_t>();
    py::enum_<::gr::atsc3::atsc3_frame_length_mode_t>(m,"atsc3_frame_length_mode_t")
        .value("FLM_TIME_ALIGNED", ::gr::atsc3::atsc3_frame_length_mode_t::FLM_TIME_ALIGNED) // 0
        .value("FLM_SYMBOL_ALIGNED", ::gr::atsc3::atsc3_frame_length_mode_t::FLM_SYMBOL_ALIGNED) // 1
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_frame_length_mode_t>();
    py::enum_<::gr::atsc3::atsc3_additional_parity_mode_t>(m,"atsc3_additional_parity_mode_t")
        .value("APM_K0", ::gr::atsc3::atsc3_additional_parity_mode_t::APM_K0) // 0
        .value("APM_K1", ::gr::atsc3::atsc3_additional_parity_mode_t::APM_K1) // 1
        .value("APM_K2", ::gr::atsc3::atsc3_additional_parity_mode_t::APM_K2) // 2
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_additional_parity_mode_t>();
    py::enum_<::gr::atsc3::atsc3_miso_t>(m,"atsc3_miso_t")
        .value("MISO_OFF", ::gr::atsc3::atsc3_miso_t::MISO_OFF) // 0
        .value("MISO_64", ::gr::atsc3::atsc3_miso_t::MISO_64) // 1
        .value("MISO_256", ::gr::atsc3::atsc3_miso_t::MISO_256) // 2
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_miso_t>();
    py::enum_<::gr::atsc3::atsc3_miso_tx_t>(m,"atsc3_miso_tx_t")
        .value("MISO_TX_1_OF_2", ::gr::atsc3::atsc3_miso_tx_t::MISO_TX_1_OF_2) // 0
        .value("MISO_TX_2_OF_2", ::gr::atsc3::atsc3_miso_tx_t::MISO_TX_2_OF_2) // 1
        .value("MISO_TX_1_OF_3", ::gr::atsc3::atsc3_miso_tx_t::MISO_TX_1_OF_3) // 2
        .value("MISO_TX_2_OF_3", ::gr::atsc3::atsc3_miso_tx_t::MISO_TX_2_OF_3) // 3
        .value("MISO_TX_3_OF_3", ::gr::atsc3::atsc3_miso_tx_t::MISO_TX_3_OF_3) // 4
        .value("MISO_TX_1_OF_4", ::gr::atsc3::atsc3_miso_tx_t::MISO_TX_1_OF_4) // 5
        .value("MISO_TX_2_OF_4", ::gr::atsc3::atsc3_miso_tx_t::MISO_TX_2_OF_4) // 6
        .value("MISO_TX_3_OF_4", ::gr::atsc3::atsc3_miso_tx_t::MISO_TX_3_OF_4) // 7
        .value("MISO_TX_4_OF_4", ::gr::atsc3::atsc3_miso_tx_t::MISO_TX_4_OF_4) // 8
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_miso_tx_t>();
    py::enum_<::gr::atsc3::atsc3_fftsize_t>(m,"atsc3_fftsize_t")
        .value("FFTSIZE_8K", ::gr::atsc3::atsc3_fftsize_t::FFTSIZE_8K) // 0
        .value("FFTSIZE_16K", ::gr::atsc3::atsc3_fftsize_t::FFTSIZE_16K) // 1
        .value("FFTSIZE_32K", ::gr::atsc3::atsc3_fftsize_t::FFTSIZE_32K) // 2
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_fftsize_t>();
    py::enum_<::gr::atsc3::atsc3_pilotpattern_t>(m,"atsc3_pilotpattern_t")
        .value("PILOT_SP3_2", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP3_2) // 0
        .value("PILOT_SP3_4", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP3_4) // 1
        .value("PILOT_SP4_2", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP4_2) // 2
        .value("PILOT_SP4_4", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP4_4) // 3
        .value("PILOT_SP6_2", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP6_2) // 4
        .value("PILOT_SP6_4", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP6_4) // 5
        .value("PILOT_SP8_2", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP8_2) // 6
        .value("PILOT_SP8_4", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP8_4) // 7
        .value("PILOT_SP12_2", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP12_2) // 8
        .value("PILOT_SP12_4", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP12_4) // 9
        .value("PILOT_SP16_2", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP16_2) // 10
        .value("PILOT_SP16_4", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP16_4) // 11
        .value("PILOT_SP24_2", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP24_2) // 12
        .value("PILOT_SP24_4", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP24_4) // 13
        .value("PILOT_SP32_2", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP32_2) // 14
        .value("PILOT_SP32_4", ::gr::atsc3::atsc3_pilotpattern_t::PILOT_SP32_4) // 15
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_pilotpattern_t>();
    py::enum_<::gr::atsc3::atsc3_reduced_carriers_t>(m,"atsc3_reduced_carriers_t")
        .value("CRED_0", ::gr::atsc3::atsc3_reduced_carriers_t::CRED_0) // 0
        .value("CRED_1", ::gr::atsc3::atsc3_reduced_carriers_t::CRED_1) // 1
        .value("CRED_2", ::gr::atsc3::atsc3_reduced_carriers_t::CRED_2) // 2
        .value("CRED_3", ::gr::atsc3::atsc3_reduced_carriers_t::CRED_3) // 3
        .value("CRED_4", ::gr::atsc3::atsc3_reduced_carriers_t::CRED_4) // 4
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_reduced_carriers_t>();
    py::enum_<::gr::atsc3::atsc3_l1_fec_mode_t>(m,"atsc3_l1_fec_mode_t")
        .value("L1_FEC_MODE_1", ::gr::atsc3::atsc3_l1_fec_mode_t::L1_FEC_MODE_1) // 0
        .value("L1_FEC_MODE_2", ::gr::atsc3::atsc3_l1_fec_mode_t::L1_FEC_MODE_2) // 1
        .value("L1_FEC_MODE_3", ::gr::atsc3::atsc3_l1_fec_mode_t::L1_FEC_MODE_3) // 2
        .value("L1_FEC_MODE_4", ::gr::atsc3::atsc3_l1_fec_mode_t::L1_FEC_MODE_4) // 3
        .value("L1_FEC_MODE_5", ::gr::atsc3::atsc3_l1_fec_mode_t::L1_FEC_MODE_5) // 4
        .value("L1_FEC_MODE_6", ::gr::atsc3::atsc3_l1_fec_mode_t::L1_FEC_MODE_6) // 5
        .value("L1_FEC_MODE_7", ::gr::atsc3::atsc3_l1_fec_mode_t::L1_FEC_MODE_7) // 6
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_l1_fec_mode_t>();
    py::enum_<::gr::atsc3::atsc3_fec_type_t>(m,"atsc3_fec_type_t")
        .value("FEC_TYPE_BCH_16K", ::gr::atsc3::atsc3_fec_type_t::FEC_TYPE_BCH_16K) // 0
        .value("FEC_TYPE_BCH_64K", ::gr::atsc3::atsc3_fec_type_t::FEC_TYPE_BCH_64K) // 1
        .value("FEC_TYPE_CRC_16K", ::gr::atsc3::atsc3_fec_type_t::FEC_TYPE_CRC_16K) // 2
        .value("FEC_TYPE_CRC_64K", ::gr::atsc3::atsc3_fec_type_t::FEC_TYPE_CRC_64K) // 3
        .value("FEC_TYPE_ONLY_16K", ::gr::atsc3::atsc3_fec_type_t::FEC_TYPE_ONLY_16K) // 4
        .value("FEC_TYPE_ONLY_64K", ::gr::atsc3::atsc3_fec_type_t::FEC_TYPE_ONLY_64K) // 5
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_fec_type_t>();
    py::enum_<::gr::atsc3::atsc3_first_sbs_t>(m,"atsc3_first_sbs_t")
        .value("SBS_OFF", ::gr::atsc3::atsc3_first_sbs_t::SBS_OFF) // 0
        .value("SBS_ON", ::gr::atsc3::atsc3_first_sbs_t::SBS_ON) // 1
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_first_sbs_t>();
    py::enum_<::gr::atsc3::atsc3_scattered_pilot_boost_t>(m,"atsc3_scattered_pilot_boost_t")
        .value("SPB_0", ::gr::atsc3::atsc3_scattered_pilot_boost_t::SPB_0) // 0
        .value("SPB_1", ::gr::atsc3::atsc3_scattered_pilot_boost_t::SPB_1) // 1
        .value("SPB_2", ::gr::atsc3::atsc3_scattered_pilot_boost_t::SPB_2) // 2
        .value("SPB_3", ::gr::atsc3::atsc3_scattered_pilot_boost_t::SPB_3) // 3
        .value("SPB_4", ::gr::atsc3::atsc3_scattered_pilot_boost_t::SPB_4) // 4
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_scattered_pilot_boost_t>();
    py::enum_<::gr::atsc3::atsc3_l1_select_t>(m,"atsc3_l1_select_t")
        .value("L1_BASIC", ::gr::atsc3::atsc3_l1_select_t::L1_BASIC) // 0
        .value("L1_DETAIL", ::gr::atsc3::atsc3_l1_select_t::L1_DETAIL) // 1
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_l1_select_t>();
    py::enum_<::gr::atsc3::atsc3_frequency_interleaver_t>(m,"atsc3_frequency_interleaver_t")
        .value("FREQ_PREAMBLE_ONLY", ::gr::atsc3::atsc3_frequency_interleaver_t::FREQ_PREAMBLE_ONLY) // 0
        .value("FREQ_ALL_SYMBOLS", ::gr::atsc3::atsc3_frequency_interleaver_t::FREQ_ALL_SYMBOLS) // 1
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_frequency_interleaver_t>();
    py::enum_<::gr::atsc3::atsc3_showlevels_t>(m,"atsc3_showlevels_t")
        .value("SHOWLEVELS_OFF", ::gr::atsc3::atsc3_showlevels_t::SHOWLEVELS_OFF) // 0
        .value("SHOWLEVELS_ON", ::gr::atsc3::atsc3_showlevels_t::SHOWLEVELS_ON) // 1
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_showlevels_t>();
    py::enum_<::gr::atsc3::atsc3_min_time_to_next_t>(m,"atsc3_min_time_to_next_t")
        .value("MTTN_50", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_50) // 0
        .value("MTTN_100", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_100) // 1
        .value("MTTN_150", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_150) // 2
        .value("MTTN_200", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_200) // 3
        .value("MTTN_250", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_250) // 4
        .value("MTTN_300", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_300) // 5
        .value("MTTN_350", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_350) // 6
        .value("MTTN_400", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_400) // 7
        .value("MTTN_500", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_500) // 8
        .value("MTTN_600", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_600) // 9
        .value("MTTN_700", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_700) // 10
        .value("MTTN_800", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_800) // 11
        .value("MTTN_900", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_900) // 12
        .value("MTTN_1000", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_1000) // 13
        .value("MTTN_1100", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_1100) // 14
        .value("MTTN_1200", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_1200) // 15
        .value("MTTN_1300", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_1300) // 16
        .value("MTTN_1500", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_1500) // 17
        .value("MTTN_1700", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_1700) // 18
        .value("MTTN_1900", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_1900) // 19
        .value("MTTN_2100", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_2100) // 20
        .value("MTTN_2300", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_2300) // 21
        .value("MTTN_2500", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_2500) // 22
        .value("MTTN_2700", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_2700) // 23
        .value("MTTN_2900", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_2900) // 24
        .value("MTTN_3300", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_3300) // 25
        .value("MTTN_3700", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_3700) // 26
        .value("MTTN_4100", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_4100) // 27
        .value("MTTN_4500", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_4500) // 28
        .value("MTTN_4900", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_4900) // 29
        .value("MTTN_5300", ::gr::atsc3::atsc3_min_time_to_next_t::MTTN_5300) // 30
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_min_time_to_next_t>();
    py::enum_<::gr::atsc3::atsc3_bootstrap_mode_t>(m,"atsc3_bootstrap_mode_t")
        .value("BOOTSTRAP_VALIDATION", ::gr::atsc3::atsc3_bootstrap_mode_t::BOOTSTRAP_VALIDATION) // 0
        .value("BOOTSTRAP_INTERPOLATION", ::gr::atsc3::atsc3_bootstrap_mode_t::BOOTSTRAP_INTERPOLATION) // 1
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_bootstrap_mode_t>();
    py::enum_<::gr::atsc3::atsc3_time_interleaver_mode_t>(m,"atsc3_time_interleaver_mode_t")
        .value("TI_MODE_OFF", ::gr::atsc3::atsc3_time_interleaver_mode_t::TI_MODE_OFF) // 0
        .value("TI_MODE_CONVOLUTIONAL", ::gr::atsc3::atsc3_time_interleaver_mode_t::TI_MODE_CONVOLUTIONAL) // 1
        .value("TI_MODE_HYBRID", ::gr::atsc3::atsc3_time_interleaver_mode_t::TI_MODE_HYBRID) // 2
        .value("TI_MODE_RESERVED", ::gr::atsc3::atsc3_time_interleaver_mode_t::TI_MODE_RESERVED) // 3
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_time_interleaver_mode_t>();
    py::enum_<::gr::atsc3::atsc3_time_interleaver_depth_t>(m,"atsc3_time_interleaver_depth_t")
        .value("TI_DEPTH_512", ::gr::atsc3::atsc3_time_interleaver_depth_t::TI_DEPTH_512) // 0
        .value("TI_DEPTH_724", ::gr::atsc3::atsc3_time_interleaver_depth_t::TI_DEPTH_724) // 1
        .value("TI_DEPTH_887", ::gr::atsc3::atsc3_time_interleaver_depth_t::TI_DEPTH_887) // 2
        .value("TI_DEPTH_1024", ::gr::atsc3::atsc3_time_interleaver_depth_t::TI_DEPTH_1024) // 3
        .value("TI_DEPTH_1254", ::gr::atsc3::atsc3_time_interleaver_depth_t::TI_DEPTH_1254) // 4
        .value("TI_DEPTH_1448", ::gr::atsc3::atsc3_time_interleaver_depth_t::TI_DEPTH_1448) // 5
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_time_interleaver_depth_t>();
    py::enum_<::gr::atsc3::atsc3_plp_fec_mode_t>(m,"atsc3_plp_fec_mode_t")
        .value("PLP_FEC_NONE", ::gr::atsc3::atsc3_plp_fec_mode_t::PLP_FEC_NONE) // 0
        .value("PLP_FEC_CRC32", ::gr::atsc3::atsc3_plp_fec_mode_t::PLP_FEC_CRC32) // 1
        .value("PLP_FEC_BCH", ::gr::atsc3::atsc3_plp_fec_mode_t::PLP_FEC_BCH) // 2
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_plp_fec_mode_t>();
    py::enum_<::gr::atsc3::atsc3_ldm_injection_level_t>(m,"atsc3_ldm_injection_level_t")
        .value("LDM_LEVEL_00DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_00DB) // 0
        .value("LDM_LEVEL_05DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_05DB) // 1
        .value("LDM_LEVEL_10DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_10DB) // 2
        .value("LDM_LEVEL_15DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_15DB) // 3
        .value("LDM_LEVEL_20DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_20DB) // 4
        .value("LDM_LEVEL_25DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_25DB) // 5
        .value("LDM_LEVEL_30DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_30DB) // 6
        .value("LDM_LEVEL_35DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_35DB) // 7
        .value("LDM_LEVEL_40DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_40DB) // 8
        .value("LDM_LEVEL_45DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_45DB) // 9
        .value("LDM_LEVEL_50DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_50DB) // 10
        .value("LDM_LEVEL_60DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_60DB) // 11
        .value("LDM_LEVEL_70DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_70DB) // 12
        .value("LDM_LEVEL_80DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_80DB) // 13
        .value("LDM_LEVEL_90DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_90DB) // 14
        .value("LDM_LEVEL_100DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_100DB) // 15
        .value("LDM_LEVEL_110DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_110DB) // 16
        .value("LDM_LEVEL_120DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_120DB) // 17
        .value("LDM_LEVEL_130DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_130DB) // 18
        .value("LDM_LEVEL_140DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_140DB) // 19
        .value("LDM_LEVEL_150DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_150DB) // 20
        .value("LDM_LEVEL_160DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_160DB) // 21
        .value("LDM_LEVEL_170DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_170DB) // 22
        .value("LDM_LEVEL_180DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_180DB) // 23
        .value("LDM_LEVEL_190DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_190DB) // 24
        .value("LDM_LEVEL_200DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_200DB) // 25
        .value("LDM_LEVEL_210DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_210DB) // 26
        .value("LDM_LEVEL_220DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_220DB) // 27
        .value("LDM_LEVEL_230DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_230DB) // 28
        .value("LDM_LEVEL_240DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_240DB) // 29
        .value("LDM_LEVEL_250DB", ::gr::atsc3::atsc3_ldm_injection_level_t::LDM_LEVEL_250DB) // 30
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_ldm_injection_level_t>();
    py::enum_<::gr::atsc3::atsc3_pilotgenerator_mode_t>(m,"atsc3_pilotgenerator_mode_t")
        .value("PILOTGENERATOR_FREQ", ::gr::atsc3::atsc3_pilotgenerator_mode_t::PILOTGENERATOR_FREQ) // 0
        .value("PILOTGENERATOR_TIME", ::gr::atsc3::atsc3_pilotgenerator_mode_t::PILOTGENERATOR_TIME) // 1
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_pilotgenerator_mode_t>();
    py::enum_<::gr::atsc3::atsc3_cyclicprefixer_mode_t>(m,"atsc3_cyclicprefixer_mode_t")
        .value("CYCLICPREFIXER_SINGLE_SUBFRAME", ::gr::atsc3::atsc3_cyclicprefixer_mode_t::CYCLICPREFIXER_SINGLE_SUBFRAME) // 0
        .value("CYCLICPREFIXER_DUAL_SUBFRAME_PLP0", ::gr::atsc3::atsc3_cyclicprefixer_mode_t::CYCLICPREFIXER_DUAL_SUBFRAME_PLP0) // 1
        .value("CYCLICPREFIXER_DUAL_SUBFRAME_PLP1", ::gr::atsc3::atsc3_cyclicprefixer_mode_t::CYCLICPREFIXER_DUAL_SUBFRAME_PLP1) // 2
        .export_values()
    ;

    py::implicitly_convertible<int, ::gr::atsc3::atsc3_cyclicprefixer_mode_t>();



}








