/* -*- c++ -*- */
/*
 * Copyright 2021 Ron Economos.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "pilotgenerator_cc_impl.h"
#include <volk/volk.h>

namespace gr {
  namespace atsc3 {

    using input_type = gr_complex;
    using output_type = gr_complex;
    pilotgenerator_cc::sptr
    pilotgenerator_cc::make(atsc3_fftsize_t fftsize, int numpayloadsyms, int numpreamblesyms, atsc3_guardinterval_t guardinterval, atsc3_pilotpattern_t pilotpattern, atsc3_scattered_pilot_boost_t pilotboost, atsc3_first_sbs_t firstsbs, atsc3_reduced_carriers_t cred, atsc3_reduced_carriers_t pcred, unsigned int vlength)
    {
      return gnuradio::make_block_sptr<pilotgenerator_cc_impl>(
        fftsize, numpayloadsyms, numpreamblesyms, guardinterval, pilotpattern, pilotboost, firstsbs, cred, pcred, vlength);
    }


    /*
     * The private constructor
     */
    pilotgenerator_cc_impl::pilotgenerator_cc_impl(atsc3_fftsize_t fftsize, int numpayloadsyms, int numpreamblesyms, atsc3_guardinterval_t guardinterval, atsc3_pilotpattern_t pilotpattern, atsc3_scattered_pilot_boost_t pilotboost, atsc3_first_sbs_t firstsbs, atsc3_reduced_carriers_t cred, atsc3_reduced_carriers_t pcred, unsigned int vlength)
      : gr::block("pilotgenerator_cc",
              gr::io_signature::make(1, 1, sizeof(input_type)),
              gr::io_signature::make(1, 1, sizeof(output_type) * vlength)),
        ofdm_fft(vlength, 1),
        ofdm_fft_size(vlength)
    {
      double power, preamble_power, scattered_power;
      double preamble_ifft_power, data_ifft_power;
      int total_preamble_cells, totalcells;
      int first_preamble_cells;
      int preamble_cells;
      int data_cells;
      int sbs_cells;

      fft_size = fftsize;
      pilot_pattern = pilotpattern;
      cred_coeff = cred;
      symbols = numpreamblesyms + numpayloadsyms;
      preamble_symbols = numpreamblesyms;
      switch (fftsize) {
        case FFTSIZE_8K:
          first_preamble_cells = 4307;
          carriers = carriers_table[FFTSIZE_8K][cred];
          max_carriers = carriers_table[FFTSIZE_8K][0];
          preamble_carriers = carriers_table[FFTSIZE_8K][4];
          switch (guardinterval) {
            case GI_1_192:
              preamble_cells = preamble_cells_table[0][pcred];
              preamble_dx = preamble_dx_table[0];
              preamble_power = preamble_power_table[0];
              preamble_ifft_power = preamble_ifft_power_table[0][pcred];
              break;
            case GI_2_384:
              preamble_cells = preamble_cells_table[1][pcred];
              preamble_dx = preamble_dx_table[1];
              preamble_power = preamble_power_table[1];
              preamble_ifft_power = preamble_ifft_power_table[1][pcred];
              break;
            case GI_3_512:
              preamble_cells = preamble_cells_table[2][pcred];
              preamble_dx = preamble_dx_table[2];
              preamble_power = preamble_power_table[2];
              preamble_ifft_power = preamble_ifft_power_table[2][pcred];
              break;
            case GI_4_768:
              preamble_cells = preamble_cells_table[3][pcred];
              preamble_dx = preamble_dx_table[3];
              preamble_power = preamble_power_table[3];
              preamble_ifft_power = preamble_ifft_power_table[3][pcred];
              break;
            case GI_5_1024:
              preamble_cells = preamble_cells_table[4][pcred];
              preamble_dx = preamble_dx_table[4];
              preamble_power = preamble_power_table[4];
              preamble_ifft_power = preamble_ifft_power_table[4][pcred];
              break;
            case GI_6_1536:
              preamble_cells = preamble_cells_table[5][pcred];
              preamble_dx = preamble_dx_table[5];
              preamble_power = preamble_power_table[5];
              preamble_ifft_power = preamble_ifft_power_table[5][pcred];
              break;
            case GI_7_2048:
              preamble_cells = preamble_cells_table[6][pcred];
              preamble_dx = preamble_dx_table[6];
              preamble_power = preamble_power_table[6];
              preamble_ifft_power = preamble_ifft_power_table[6][pcred];
              break;
            default:
              preamble_cells = preamble_cells_table[0][pcred];
              preamble_dx = preamble_dx_table[0];
              preamble_power = preamble_power_table[0];
              preamble_ifft_power = preamble_ifft_power_table[0][pcred];
              break;
          }
          switch (pilotpattern) {
            case PILOT_SP3_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP3_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP3_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP3_2][cred];
              break;
            case PILOT_SP3_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP3_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP3_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP3_4][cred];
              break;
            case PILOT_SP4_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP4_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP4_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP4_2][cred];
              break;
            case PILOT_SP4_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP4_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP4_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP4_4][cred];
              break;
            case PILOT_SP6_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP6_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP6_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP6_2][cred];
              break;
            case PILOT_SP6_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP6_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP6_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP6_4][cred];
              break;
            case PILOT_SP8_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP8_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP8_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP8_2][cred];
              break;
            case PILOT_SP8_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP8_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP8_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP8_4][cred];
              break;
            case PILOT_SP12_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP12_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP12_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP12_2][cred];
              break;
            case PILOT_SP12_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP12_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP12_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP12_4][cred];
              break;
            case PILOT_SP16_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP16_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP16_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP16_2][cred];
              break;
            case PILOT_SP16_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP16_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP16_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP16_4][cred];
              break;
            case PILOT_SP24_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP24_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP24_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP24_2][cred];
              break;
            case PILOT_SP24_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP24_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP24_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP24_4][cred];
              break;
            case PILOT_SP32_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP32_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP32_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP32_2][cred];
              break;
            case PILOT_SP32_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP32_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP32_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP32_4][cred];
              break;
            default:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP3_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP3_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP3_2][cred];
              break;
          }
          break;
        case FFTSIZE_16K:
          first_preamble_cells = 8614;
          carriers = carriers_table[FFTSIZE_16K][cred];
          max_carriers = carriers_table[FFTSIZE_16K][0];
          preamble_carriers = carriers_table[FFTSIZE_16K][4];
          switch (guardinterval) {
            case GI_1_192:
              preamble_cells = preamble_cells_table[7][pcred];
              preamble_dx = preamble_dx_table[7];
              preamble_power = preamble_power_table[7];
              preamble_ifft_power = preamble_ifft_power_table[7][pcred];
              break;
            case GI_2_384:
              preamble_cells = preamble_cells_table[8][pcred];
              preamble_dx = preamble_dx_table[8];
              preamble_power = preamble_power_table[8];
              preamble_ifft_power = preamble_ifft_power_table[8][pcred];
              break;
            case GI_3_512:
              preamble_cells = preamble_cells_table[9][pcred];
              preamble_dx = preamble_dx_table[9];
              preamble_power = preamble_power_table[9];
              preamble_ifft_power = preamble_ifft_power_table[9][pcred];
              break;
            case GI_4_768:
              preamble_cells = preamble_cells_table[10][pcred];
              preamble_dx = preamble_dx_table[10];
              preamble_power = preamble_power_table[10];
              preamble_ifft_power = preamble_ifft_power_table[10][pcred];
              break;
            case GI_5_1024:
              preamble_cells = preamble_cells_table[11][pcred];
              preamble_dx = preamble_dx_table[11];
              preamble_power = preamble_power_table[11];
              preamble_ifft_power = preamble_ifft_power_table[11][pcred];
              break;
            case GI_6_1536:
              preamble_cells = preamble_cells_table[12][pcred];
              preamble_dx = preamble_dx_table[12];
              preamble_power = preamble_power_table[12];
              preamble_ifft_power = preamble_ifft_power_table[12][pcred];
              break;
            case GI_7_2048:
              preamble_cells = preamble_cells_table[13][pcred];
              preamble_dx = preamble_dx_table[13];
              preamble_power = preamble_power_table[13];
              preamble_ifft_power = preamble_ifft_power_table[13][pcred];
              break;
            case GI_8_2432:
              preamble_cells = preamble_cells_table[14][pcred];
              preamble_dx = preamble_dx_table[14];
              preamble_power = preamble_power_table[14];
              preamble_ifft_power = preamble_ifft_power_table[14][pcred];
              break;
            case GI_9_3072:
              preamble_cells = preamble_cells_table[15][pcred];
              preamble_dx = preamble_dx_table[15];
              preamble_power = preamble_power_table[15];
              preamble_ifft_power = preamble_ifft_power_table[15][pcred];
              break;
            case GI_10_3648:
              preamble_cells = preamble_cells_table[16][pcred];
              preamble_dx = preamble_dx_table[16];
              preamble_power = preamble_power_table[16];
              preamble_ifft_power = preamble_ifft_power_table[16][pcred];
              break;
            case GI_11_4096:
              preamble_cells = preamble_cells_table[17][pcred];
              preamble_dx = preamble_dx_table[17];
              preamble_power = preamble_power_table[17];
              preamble_ifft_power = preamble_ifft_power_table[17][pcred];
              break;
            default:
              preamble_cells = preamble_cells_table[7][pcred];
              preamble_dx = preamble_dx_table[7];
              preamble_power = preamble_power_table[7];
              preamble_ifft_power = preamble_ifft_power_table[7][pcred];
              break;
          }
          switch (pilotpattern) {
            case PILOT_SP3_2:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP3_2][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP3_2][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP3_2][cred];
              break;
            case PILOT_SP3_4:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP3_4][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP3_4][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP3_4][cred];
              break;
            case PILOT_SP4_2:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP4_2][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP4_2][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP4_2][cred];
              break;
            case PILOT_SP4_4:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP4_4][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP4_4][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP4_4][cred];
              break;
            case PILOT_SP6_2:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP6_2][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP6_2][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP6_2][cred];
              break;
            case PILOT_SP6_4:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP6_4][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP6_4][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP6_4][cred];
              break;
            case PILOT_SP8_2:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP8_2][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP8_2][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP8_2][cred];
              break;
            case PILOT_SP8_4:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP8_4][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP8_4][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP8_4][cred];
              break;
            case PILOT_SP12_2:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP12_2][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP12_2][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP12_2][cred];
              break;
            case PILOT_SP12_4:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP12_4][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP12_4][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP12_4][cred];
              break;
            case PILOT_SP16_2:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP16_2][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP16_2][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP16_2][cred];
              break;
            case PILOT_SP16_4:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP16_4][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP16_4][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP16_4][cred];
              break;
            case PILOT_SP24_2:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP24_2][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP24_2][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP24_2][cred];
              break;
            case PILOT_SP24_4:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP24_4][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP24_4][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP24_4][cred];
              break;
            case PILOT_SP32_2:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP32_2][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP32_2][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP32_2][cred];
              break;
            case PILOT_SP32_4:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP32_4][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP32_4][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP32_4][cred];
              break;
            default:
              data_ifft_power = data_ifft_power_table_16K[PILOT_SP3_2][cred][pilotboost];
              data_cells = data_cells_table_16K[PILOT_SP3_2][cred];
              sbs_cells = sbs_cells_table_16K[PILOT_SP3_2][cred];
              break;
          }
          break;
        case FFTSIZE_32K:
          first_preamble_cells = 17288;
          carriers = carriers_table[FFTSIZE_32K][cred];
          max_carriers = carriers_table[FFTSIZE_32K][0];
          preamble_carriers = carriers_table[FFTSIZE_32K][4];
          switch (guardinterval) {
            case GI_1_192:
              preamble_cells = preamble_cells_table[18][pcred];
              preamble_dx = preamble_dx_table[18];
              preamble_power = preamble_power_table[18];
              preamble_ifft_power = preamble_ifft_power_table[18][pcred];
              break;
            case GI_2_384:
              preamble_cells = preamble_cells_table[19][pcred];
              preamble_dx = preamble_dx_table[19];
              preamble_power = preamble_power_table[19];
              preamble_ifft_power = preamble_ifft_power_table[19][pcred];
              break;
            case GI_3_512:
              preamble_cells = preamble_cells_table[20][pcred];
              preamble_dx = preamble_dx_table[20];
              preamble_power = preamble_power_table[20];
              preamble_ifft_power = preamble_ifft_power_table[20][pcred];
              break;
            case GI_4_768:
              preamble_cells = preamble_cells_table[21][pcred];
              preamble_dx = preamble_dx_table[21];
              preamble_power = preamble_power_table[21];
              preamble_ifft_power = preamble_ifft_power_table[21][pcred];
              break;
            case GI_5_1024:
              preamble_cells = preamble_cells_table[22][pcred];
              preamble_dx = preamble_dx_table[22];
              preamble_power = preamble_power_table[22];
              preamble_ifft_power = preamble_ifft_power_table[22][pcred];
              break;
            case GI_6_1536:
              preamble_cells = preamble_cells_table[23][pcred];
              preamble_dx = preamble_dx_table[23];
              preamble_power = preamble_power_table[23];
              preamble_ifft_power = preamble_ifft_power_table[23][pcred];
              break;
            case GI_7_2048:
              preamble_cells = preamble_cells_table[24][pcred];
              preamble_dx = preamble_dx_table[24];
              preamble_power = preamble_power_table[24];
              preamble_ifft_power = preamble_ifft_power_table[24][pcred];
              break;
            case GI_8_2432:
              preamble_cells = preamble_cells_table[25][pcred];
              preamble_dx = preamble_dx_table[25];
              preamble_power = preamble_power_table[25];
              preamble_ifft_power = preamble_ifft_power_table[25][pcred];
              break;
            case GI_9_3072:
              if (pilotpattern == PILOT_SP8_2 || pilotpattern == PILOT_SP8_4) {
                preamble_cells = preamble_cells_table[26][pcred];
                preamble_dx = preamble_dx_table[26];
                preamble_power = preamble_power_table[26];
              preamble_ifft_power = preamble_ifft_power_table[26][pcred];
              }
              else {
                preamble_cells = preamble_cells_table[27][pcred];
                preamble_dx = preamble_dx_table[27];
                preamble_power = preamble_power_table[27];
              preamble_ifft_power = preamble_ifft_power_table[27][pcred];
              }
              break;
            case GI_10_3648:
              if (pilotpattern == PILOT_SP8_2 || pilotpattern == PILOT_SP8_4) {
                preamble_cells = preamble_cells_table[28][pcred];
                preamble_dx = preamble_dx_table[28];
                preamble_power = preamble_power_table[28];
              preamble_ifft_power = preamble_ifft_power_table[28][pcred];
              }
              else {
                preamble_cells = preamble_cells_table[29][pcred];
                preamble_dx = preamble_dx_table[29];
                preamble_power = preamble_power_table[29];
              preamble_ifft_power = preamble_ifft_power_table[29][pcred];
              }
              break;
            case GI_11_4096:
              preamble_cells = preamble_cells_table[30][pcred];
              preamble_dx = preamble_dx_table[30];
              preamble_power = preamble_power_table[30];
              preamble_ifft_power = preamble_ifft_power_table[30][pcred];
              break;
            case GI_12_4864:
              preamble_cells = preamble_cells_table[31][pcred];
              preamble_dx = preamble_dx_table[31];
              preamble_power = preamble_power_table[31];
              preamble_ifft_power = preamble_ifft_power_table[31][pcred];
              break;
            default:
              preamble_cells = preamble_cells_table[18][pcred];
              preamble_dx = preamble_dx_table[18];
              preamble_power = preamble_power_table[18];
              preamble_ifft_power = preamble_ifft_power_table[18][pcred];
              break;
          }
          switch (pilotpattern) {
            case PILOT_SP3_2:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP3_2][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP3_2][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP3_2][cred];
              break;
            case PILOT_SP3_4:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP3_4][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP3_4][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP3_4][cred];
              break;
            case PILOT_SP4_2:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP4_2][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP4_2][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP4_2][cred];
              break;
            case PILOT_SP4_4:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP4_4][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP4_4][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP4_4][cred];
              break;
            case PILOT_SP6_2:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP6_2][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP6_2][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP6_2][cred];
              break;
            case PILOT_SP6_4:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP6_4][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP6_4][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP6_4][cred];
              break;
            case PILOT_SP8_2:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP8_2][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP8_2][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP8_2][cred];
              break;
            case PILOT_SP8_4:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP8_4][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP8_4][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP8_4][cred];
              break;
            case PILOT_SP12_2:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP12_2][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP12_2][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP12_2][cred];
              break;
            case PILOT_SP12_4:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP12_4][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP12_4][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP12_4][cred];
              break;
            case PILOT_SP16_2:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP16_2][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP16_2][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP16_2][cred];
              break;
            case PILOT_SP16_4:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP16_4][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP16_4][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP16_4][cred];
              break;
            case PILOT_SP24_2:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP24_2][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP24_2][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP24_2][cred];
              break;
            case PILOT_SP24_4:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP24_4][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP24_4][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP24_4][cred];
              break;
            case PILOT_SP32_2:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP32_2][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP32_2][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP32_2][cred];
              break;
            case PILOT_SP32_4:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP32_4][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP32_4][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP32_4][cred];
              break;
            default:
              data_ifft_power = data_ifft_power_table_32K[PILOT_SP3_2][cred][pilotboost];
              data_cells = data_cells_table_32K[PILOT_SP3_2][cred];
              sbs_cells = sbs_cells_table_32K[PILOT_SP3_2][cred];
              break;
          }
          break;
        default:
          first_preamble_cells = 4307;
          carriers = carriers_table[FFTSIZE_8K][cred];
          max_carriers = carriers_table[FFTSIZE_8K][0];
          preamble_carriers = carriers_table[FFTSIZE_8K][4];
          switch (guardinterval) {
            case GI_1_192:
              preamble_cells = preamble_cells_table[0][pcred];
              preamble_dx = preamble_dx_table[0];
              preamble_power = preamble_power_table[0];
              preamble_ifft_power = preamble_ifft_power_table[0][pcred];
              break;
            case GI_2_384:
              preamble_cells = preamble_cells_table[1][pcred];
              preamble_dx = preamble_dx_table[1];
              preamble_power = preamble_power_table[1];
              preamble_ifft_power = preamble_ifft_power_table[1][pcred];
              break;
            case GI_3_512:
              preamble_cells = preamble_cells_table[2][pcred];
              preamble_dx = preamble_dx_table[2];
              preamble_power = preamble_power_table[2];
              preamble_ifft_power = preamble_ifft_power_table[2][pcred];
              break;
            case GI_4_768:
              preamble_cells = preamble_cells_table[3][pcred];
              preamble_dx = preamble_dx_table[3];
              preamble_power = preamble_power_table[3];
              preamble_ifft_power = preamble_ifft_power_table[3][pcred];
              break;
            case GI_5_1024:
              preamble_cells = preamble_cells_table[4][pcred];
              preamble_dx = preamble_dx_table[4];
              preamble_power = preamble_power_table[4];
              preamble_ifft_power = preamble_ifft_power_table[4][pcred];
              break;
            case GI_6_1536:
              preamble_cells = preamble_cells_table[5][pcred];
              preamble_dx = preamble_dx_table[5];
              preamble_power = preamble_power_table[5];
              preamble_ifft_power = preamble_ifft_power_table[5][pcred];
              break;
            case GI_7_2048:
              preamble_cells = preamble_cells_table[6][pcred];
              preamble_dx = preamble_dx_table[6];
              preamble_power = preamble_power_table[6];
              preamble_ifft_power = preamble_ifft_power_table[6][pcred];
              break;
            default:
              preamble_cells = preamble_cells_table[0][pcred];
              preamble_dx = preamble_dx_table[0];
              preamble_power = preamble_power_table[0];
              preamble_ifft_power = preamble_ifft_power_table[0][pcred];
              break;
          }
          switch (pilotpattern) {
            case PILOT_SP3_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP3_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP3_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP3_2][cred];
              break;
            case PILOT_SP3_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP3_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP3_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP3_4][cred];
              break;
            case PILOT_SP4_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP4_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP4_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP4_2][cred];
              break;
            case PILOT_SP4_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP4_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP4_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP4_4][cred];
              break;
            case PILOT_SP6_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP6_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP6_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP6_2][cred];
              break;
            case PILOT_SP6_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP6_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP6_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP6_4][cred];
              break;
            case PILOT_SP8_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP8_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP8_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP8_2][cred];
              break;
            case PILOT_SP8_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP8_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP8_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP8_4][cred];
              break;
            case PILOT_SP12_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP12_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP12_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP12_2][cred];
              break;
            case PILOT_SP12_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP12_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP12_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP12_4][cred];
              break;
            case PILOT_SP16_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP16_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP16_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP16_2][cred];
              break;
            case PILOT_SP16_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP16_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP16_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP16_4][cred];
              break;
            case PILOT_SP24_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP24_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP24_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP24_2][cred];
              break;
            case PILOT_SP24_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP24_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP24_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP24_4][cred];
              break;
            case PILOT_SP32_2:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP32_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP32_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP32_2][cred];
              break;
            case PILOT_SP32_4:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP32_4][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP32_4][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP32_4][cred];
              break;
            default:
              data_ifft_power = data_ifft_power_table_8K[PILOT_SP3_2][cred][pilotboost];
              data_cells = data_cells_table_8K[PILOT_SP3_2][cred];
              sbs_cells = sbs_cells_table_8K[PILOT_SP3_2][cred];
              break;
          }
          break;
      }
      switch (pilotpattern) {
        case PILOT_SP3_2:
          dx = 3;
          dy = 2;
          scattered_power = scattered_power_table[PILOT_SP3_2][pilotboost];
          break;
        case PILOT_SP3_4:
          dx = 3;
          dy = 4;
          scattered_power = scattered_power_table[PILOT_SP3_4][pilotboost];
          break;
        case PILOT_SP4_2:
          dx = 4;
          dy = 2;
          scattered_power = scattered_power_table[PILOT_SP4_2][pilotboost];
          break;
        case PILOT_SP4_4:
          dx = 4;
          dy = 4;
          scattered_power = scattered_power_table[PILOT_SP4_4][pilotboost];
          break;
        case PILOT_SP6_2:
          dx = 6;
          dy = 2;
          scattered_power = scattered_power_table[PILOT_SP6_2][pilotboost];
          break;
        case PILOT_SP6_4:
          dx = 6;
          dy = 4;
          scattered_power = scattered_power_table[PILOT_SP6_4][pilotboost];
          break;
        case PILOT_SP8_2:
          dx = 8;
          dy = 2;
          scattered_power = scattered_power_table[PILOT_SP8_2][pilotboost];
          break;
        case PILOT_SP8_4:
          dx = 8;
          dy = 4;
          scattered_power = scattered_power_table[PILOT_SP8_4][pilotboost];
          break;
        case PILOT_SP12_2:
          dx = 12;
          dy = 2;
          scattered_power = scattered_power_table[PILOT_SP12_2][pilotboost];
          break;
        case PILOT_SP12_4:
          dx = 12;
          dy = 4;
          scattered_power = scattered_power_table[PILOT_SP12_4][pilotboost];
          break;
        case PILOT_SP16_2:
          dx = 16;
          dy = 2;
          scattered_power = scattered_power_table[PILOT_SP16_2][pilotboost];
          break;
        case PILOT_SP16_4:
          dx = 16;
          dy = 4;
          scattered_power = scattered_power_table[PILOT_SP16_4][pilotboost];
          break;
        case PILOT_SP24_2:
          dx = 24;
          dy = 2;
          scattered_power = scattered_power_table[PILOT_SP24_2][pilotboost];
          break;
        case PILOT_SP24_4:
          dx = 24;
          dy = 4;
          scattered_power = scattered_power_table[PILOT_SP24_4][pilotboost];
          break;
        case PILOT_SP32_2:
          dx = 32;
          dy = 2;
          scattered_power = scattered_power_table[PILOT_SP32_2][pilotboost];
          break;
        case PILOT_SP32_4:
          dx = 32;
          dy = 4;
          scattered_power = scattered_power_table[PILOT_SP32_4][pilotboost];
          break;
        default:
          dx = 3;
          dy = 2;
          scattered_power = scattered_power_table[PILOT_SP3_2][pilotboost];
          break;
      }
      power = pow(10, preamble_power / 20.0);
      pr_bpsk[0] = gr_complex(power, 0.0);
      pr_bpsk[1] = gr_complex(-(power), 0.0);
      power = pow(10, scattered_power / 20.0);
      sp_bpsk[0] = gr_complex(power, 0.0);
      sp_bpsk[1] = gr_complex(-(power), 0.0);
      power = pow(10, 8.52 / 20.0);
      cp_bpsk[0] = gr_complex(power, 0.0);
      cp_bpsk[1] = gr_complex(-(power), 0.0);
      init_prbs();
      frame_symbols[0] = PREAMBLE_SYMBOL;
      total_preamble_cells = 0;
      for (int n = 1; n < numpreamblesyms; n++) {
        frame_symbols[n] = PREAMBLE_SYMBOL;
        total_preamble_cells += preamble_cells;
      }
      if (firstsbs == TRUE) {
        frame_symbols[numpreamblesyms] = SBS_SYMBOL;
        for (int n = 0; n < numpayloadsyms; n++) {
          frame_symbols[n + numpreamblesyms + 1] = DATA_SYMBOL;
        }
      }
      else {
        for (int n = 0; n < numpayloadsyms; n++) {
          frame_symbols[n + numpreamblesyms] = DATA_SYMBOL;
        }
      }
      frame_symbols[numpreamblesyms + numpayloadsyms - 1] = SBS_SYMBOL;
      data_carrier_map.resize(symbols);
      for (std::vector<std::vector<int>>::size_type i = 0; i != data_carrier_map.size(); i++) {
        data_carrier_map[i].resize(max_carriers);
      }
      init_pilots();
      if (firstsbs) {
        totalcells = first_preamble_cells + total_preamble_cells + ((numpayloadsyms - 2) * data_cells) + (sbs_cells * 2);
      }
      else {
        totalcells = first_preamble_cells + total_preamble_cells + ((numpayloadsyms - 1) * data_cells) + sbs_cells;
      }
      input_cells = totalcells;
      printf("input cells = %d\n", input_cells);
      /* -1.98 is a tweak factor to match verification files */
      first_preamble_normalization = 1.0 / std::sqrt((preamble_ifft_power * ((double)preamble_carriers / (double)carriers) - 1.98));
      preamble_normalization = 1.0 / std::sqrt(preamble_ifft_power);
      data_normalization = 1.0 / std::sqrt(data_ifft_power);
      set_output_multiple(symbols);
    }

    /*
     * Our virtual destructor.
     */
    pilotgenerator_cc_impl::~pilotgenerator_cc_impl()
    {
    }

    void
    pilotgenerator_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = input_cells * (noutput_items / symbols);
    }

    void
    pilotgenerator_cc_impl::init_prbs(void)
    {
      int sr = 0x1b;

      for (int i = 0; i < MAX_CARRIERS; i++) {
        int b = ((sr) ^ (sr >> 1) ^ (sr >> 3) ^ (sr >> 4)) & 1;
        prbs[i] = sr & 1;
        sr >>= 1;
        if (b) {
          sr |= 0x1000;
        }
      }
    }

    void
    pilotgenerator_cc_impl::init_pilots()
    {
      for (int symbol = 0; symbol < symbols; ++symbol) {
        int remainder, shift, index, preamblecarriers;
        std::vector<int>& data_carrier_map = this->data_carrier_map[symbol];
        for (int i = 0; i < carriers; i++) {
          data_carrier_map[i] = DATA_CARRIER;
        }
        switch (fft_size) {
          case FFTSIZE_8K:
            if (frame_symbols[symbol] == PREAMBLE_SYMBOL) {
              index = 0;
              if (symbol == 0) {
                preamblecarriers = preamble_carriers;
                shift = (max_carriers - preamble_carriers) / 2;
              }
              else {
                preamblecarriers = carriers;
                shift = 0;
              }
              for (int i = 0; i < max_carriers; i++) {
                if (continual_pilot_table_8K[index] == i) {
                  if (continual_pilot_table_8K[index] > shift) {
                    data_carrier_map[i - shift] = CONTINUAL_CARRIER;
                  }
                  index++;
                }
              }
              for (int i = 0; i < preamblecarriers; i++) {
                if ((i % preamble_dx) == 0) {
                  data_carrier_map[i] = PREAMBLE_CARRIER;
                }
              }
            }
            else if (frame_symbols[symbol] == SBS_SYMBOL) {
              index = shift = 0;
              for (int i = 0; i < max_carriers; i++) {
                if (continual_pilot_table_8K[index] == i) {
                  if (continual_pilot_table_8K[index] > shift) {
                    data_carrier_map[i] = CONTINUAL_CARRIER;
                  }
                  index++;
                }
              }
              for (int i = 0; i < carriers; i++) {
                if ((i % dx) == 0) {
                  data_carrier_map[i] = SCATTERED_CARRIER;
                }
              }
            }
            else {
              index = shift = 0;
              for (int i = 0; i < max_carriers; i++) {
                if (continual_pilot_table_8K[index] == i) {
                  if (continual_pilot_table_8K[index] > shift) {
                    data_carrier_map[i] = CONTINUAL_CARRIER;
                  }
                  index++;
                }
              }
              for (int i = 0; i < carriers; i++) {
                remainder = i % (dx * dy);
                if (remainder == (dx * ((symbol - preamble_symbols) % dy))) {
                  data_carrier_map[i] = SCATTERED_CARRIER;
                }
              }
            }
            if ((frame_symbols[symbol] == SBS_SYMBOL) || (frame_symbols[symbol] == DATA_SYMBOL)) {
              data_carrier_map[0] = SCATTERED_CARRIER;
              data_carrier_map[carriers - 1] = SCATTERED_CARRIER;
              shift = 0;
              switch (pilot_pattern) {
                case PILOT_SP3_2:
                  data_carrier_map[1731 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP3_4:
                  data_carrier_map[1731 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[2886 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5733 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP4_2:
                  data_carrier_map[1732 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP4_4:
                  data_carrier_map[1732 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[2888 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5724 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP6_2:
                  data_carrier_map[1734 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP6_4:
                  data_carrier_map[1734 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[2892 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5730 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP8_2:
                  data_carrier_map[1736 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP8_4:
                  data_carrier_map[1736 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[2896 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5720 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP12_2:
                  data_carrier_map[1740 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP12_4:
                  data_carrier_map[1740 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[2904 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5748 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP16_2:
                  data_carrier_map[1744 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP16_4:
                  data_carrier_map[1744 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[2912 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5744 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP24_2:
                  break;
                case PILOT_SP24_4:
                  break;
                case PILOT_SP32_2:
                  data_carrier_map[1696 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP32_4:
                  switch (cred_coeff) {
                    case CRED_0:
                      data_carrier_map[1696 - shift] = SCATTERED_CARRIER;
                      data_carrier_map[2880 - shift] = SCATTERED_CARRIER;
                      data_carrier_map[5728 - shift] = SCATTERED_CARRIER;
                      break;
                    case CRED_1:
                      break;
                    case CRED_2:
                      data_carrier_map[1696 - shift] = SCATTERED_CARRIER;
                      break;
                    case CRED_3:
                      data_carrier_map[1696 - shift] = SCATTERED_CARRIER;
                      data_carrier_map[2880 - shift] = SCATTERED_CARRIER;
                      break;
                    case CRED_4:
                      data_carrier_map[1696 - shift] = SCATTERED_CARRIER;
                      data_carrier_map[2880 - shift] = SCATTERED_CARRIER;
                      data_carrier_map[5728 - shift] = SCATTERED_CARRIER;
                      break;
                    default:
                      break;
                  }
                  break;
                default:
                  break;
              }
            }
            break;
          case FFTSIZE_16K:
            if (frame_symbols[symbol] == PREAMBLE_SYMBOL) {
              index = 0;
              if (symbol == 0) {
                preamblecarriers = preamble_carriers;
                shift = (max_carriers - preamble_carriers) / 2;
              }
              else {
                preamblecarriers = carriers;
                shift = 0;
              }
              for (int i = 0; i < max_carriers; i++) {
                if (continual_pilot_table_16K[index] == i) {
                  if (continual_pilot_table_16K[index] > shift) {
                    data_carrier_map[i - shift] = CONTINUAL_CARRIER;
                  }
                  index++;
                }
              }
              for (int i = 0; i < preamblecarriers; i++) {
                if ((i % preamble_dx) == 0) {
                  data_carrier_map[i] = PREAMBLE_CARRIER;
                }
              }
            }
            else if (frame_symbols[symbol] == SBS_SYMBOL) {
              index = shift = 0;
              for (int i = 0; i < max_carriers; i++) {
                if (continual_pilot_table_16K[index] == i) {
                  if (continual_pilot_table_16K[index] > shift) {
                    data_carrier_map[i] = CONTINUAL_CARRIER;
                  }
                  index++;
                }
              }
              for (int i = 0; i < carriers; i++) {
                if ((i % dx) == 0) {
                  data_carrier_map[i] = SCATTERED_CARRIER;
                }
              }
            }
            else {
              index = shift = 0;
              for (int i = 0; i < max_carriers; i++) {
                if (continual_pilot_table_16K[index] == i) {
                  if (continual_pilot_table_16K[index] > shift) {
                    data_carrier_map[i] = CONTINUAL_CARRIER;
                  }
                  index++;
                }
              }
              for (int i = 0; i < carriers; i++) {
                remainder = i % (dx * dy);
                if (remainder == (dx * ((symbol - preamble_symbols) % dy))) {
                  data_carrier_map[i] = SCATTERED_CARRIER;
                }
              }
            }
            if ((frame_symbols[symbol] == SBS_SYMBOL) || (frame_symbols[symbol] == DATA_SYMBOL)) {
              data_carrier_map[0] = SCATTERED_CARRIER;
              data_carrier_map[carriers - 1] = SCATTERED_CARRIER;
              shift = 0;
              switch (pilot_pattern) {
                case PILOT_SP3_2:
                  data_carrier_map[3471 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP3_4:
                  data_carrier_map[3471 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5778 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[11469 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP4_2:
                  data_carrier_map[3460 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP4_4:
                  data_carrier_map[3460 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5768 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[11452 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP6_2:
                  data_carrier_map[3462 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP6_4:
                  data_carrier_map[3462 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5772 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[11466 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP8_2:
                  data_carrier_map[3464 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP8_4:
                  data_carrier_map[3464 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5776 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[11448 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP12_2:
                  data_carrier_map[3468 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP12_4:
                  data_carrier_map[3468 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5784 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[11460 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP16_2:
                  data_carrier_map[3472 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP16_4:
                  data_carrier_map[3472 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5792 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[11440 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP24_2:
                  data_carrier_map[3480 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP24_4:
                  data_carrier_map[3480 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5808 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[11496 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP32_2:
                  data_carrier_map[3488 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP32_4:
                  data_carrier_map[3488 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[5824 - shift] = SCATTERED_CARRIER;
                  data_carrier_map[11488 - shift] = SCATTERED_CARRIER;
                  break;
                default:
                  break;
              }
            }
            break;
          case FFTSIZE_32K:
            if (frame_symbols[symbol] == PREAMBLE_SYMBOL) {
              index = 0;
              if (symbol == 0) {
                preamblecarriers = preamble_carriers;
                shift = (max_carriers - preamble_carriers) / 2;
              }
              else {
                preamblecarriers = carriers;
                shift = 0;
              }
              for (int i = 0; i < max_carriers; i++) {
                if (continual_pilot_table_32K[index] == i) {
                  if (continual_pilot_table_32K[index] > shift) {
                    data_carrier_map[i - shift] = CONTINUAL_CARRIER;
                  }
                  index++;
                }
              }
              for (int i = 0; i < preamblecarriers; i++) {
                if ((i % preamble_dx) == 0) {
                  data_carrier_map[i] = PREAMBLE_CARRIER;
                }
              }
            }
            else if (frame_symbols[symbol] == SBS_SYMBOL) {
              index = shift = 0;
              for (int i = 0; i < max_carriers; i++) {
                if (continual_pilot_table_32K[index] == i) {
                  if (continual_pilot_table_32K[index] > shift) {
                    data_carrier_map[i] = CONTINUAL_CARRIER;
                  }
                  index++;
                }
              }
              for (int i = 0; i < carriers; i++) {
                if ((i % dx) == 0) {
                  data_carrier_map[i] = SCATTERED_CARRIER;
                }
              }
            }
            else {
              index = shift = 0;
              for (int i = 0; i < max_carriers; i++) {
                if (continual_pilot_table_32K[index] == i) {
                  if (continual_pilot_table_32K[index] > shift) {
                    data_carrier_map[i] = CONTINUAL_CARRIER;
                  }
                  index++;
                }
              }
              for (int i = 0; i < carriers; i++) {
                remainder = i % (dx * dy);
                if (remainder == (dx * ((symbol - preamble_symbols) % dy))) {
                  data_carrier_map[i] = SCATTERED_CARRIER;
                }
              }
            }
            if ((frame_symbols[symbol] == SBS_SYMBOL) || (frame_symbols[symbol] == DATA_SYMBOL)) {
              data_carrier_map[0] = SCATTERED_CARRIER;
              data_carrier_map[carriers - 1] = SCATTERED_CARRIER;
              shift = 0;
              switch (pilot_pattern) {
                case PILOT_SP3_2:
                  data_carrier_map[6939 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP3_4:
                  break;
                case PILOT_SP4_2:
                  break;
                case PILOT_SP4_4:
                  break;
                case PILOT_SP6_2:
                  data_carrier_map[6942 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP6_4:
                  break;
                case PILOT_SP8_2:
                  data_carrier_map[6920 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP8_4:
                  break;
                case PILOT_SP12_2:
                  data_carrier_map[6924 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP12_4:
                  break;
                case PILOT_SP16_2:
                  data_carrier_map[6928 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP16_4:
                  break;
                case PILOT_SP24_2:
                  data_carrier_map[6936 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP24_4:
                  break;
                case PILOT_SP32_2:
                  data_carrier_map[6944 - shift] = SCATTERED_CARRIER;
                  break;
                case PILOT_SP32_4:
                  break;
                default:
                  break;
              }
            }
            break;
        }
      }
    }

    const gr_complex zero = gr_complex(0.0, 0.0);

    int
    pilotgenerator_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      auto in = static_cast<const input_type*>(input_items[0]);
      auto out = static_cast<output_type*>(output_items[0]);
      int indexin = 0;
      int preamblecarriers;
      int left_nulls, right_nulls;
      float normalization;
      gr_complex* dst;

      for (int i = 0; i < noutput_items; i += (carriers * (symbols - 1)) + preamble_carriers) {
        for (int j = 0; j < symbols; j++) {
          if (frame_symbols[j] == PREAMBLE_SYMBOL) {
            if (j == 0) {
              preamblecarriers = preamble_carriers;
              normalization = first_preamble_normalization;
            }
            else {
              preamblecarriers = carriers;
              normalization = preamble_normalization;
            }
            left_nulls = ((ofdm_fft_size - preamblecarriers) / 2) + 1;
            right_nulls = (ofdm_fft_size - preamblecarriers) / 2;
            for (int n = 0; n < left_nulls; n++) {
              *out++ = zero;
            }
            for (int n = 0; n < preamblecarriers; n++) {
              if (data_carrier_map[j][n] == PREAMBLE_CARRIER) {
                *out++ = pr_bpsk[prbs[n]];
              }
              else if (data_carrier_map[j][n] == CONTINUAL_CARRIER) {
                *out++ = cp_bpsk[prbs[n]];
              }
              else {
                *out++ = in[indexin++];
              }
            }
            for (int n = 0; n < right_nulls; n++) {
              *out++ = zero;
            }
          }
          else {
            normalization = data_normalization;
            left_nulls = ((ofdm_fft_size - carriers) / 2) + 1;
            right_nulls = (ofdm_fft_size - carriers) / 2;
            for (int n = 0; n < left_nulls; n++) {
              *out++ = zero;
            }
            for (int n = 0; n < carriers; n++) {
              if (data_carrier_map[j][n] == SCATTERED_CARRIER) {
                *out++ = sp_bpsk[prbs[n]];
              }
              else if (data_carrier_map[j][n] == CONTINUAL_CARRIER) {
                *out++ = cp_bpsk[prbs[n]];
              }
              else {
                *out++ = in[indexin++];
              }
            }
            for (int n = 0; n < right_nulls; n++) {
              *out++ = zero;
            }
          }
          out -= ofdm_fft_size;
#if 0
          if (equalization_enable == EQUALIZATION_ON) {
              volk_32fc_x2_multiply_32fc(out, out, inverse_sinc, ofdm_fft_size);
          }
#endif
          dst = ofdm_fft.get_inbuf();
          memcpy(&dst[ofdm_fft_size / 2], &out[0], sizeof(gr_complex) * ofdm_fft_size / 2);
          memcpy(&dst[0], &out[ofdm_fft_size / 2], sizeof(gr_complex) * ofdm_fft_size / 2);
          ofdm_fft.execute();
          volk_32fc_s32fc_multiply_32fc(out, ofdm_fft.get_outbuf(), normalization, ofdm_fft_size);
          out += ofdm_fft_size;
        }
      }

      if (indexin != input_cells) {
        printf("input_cells = %d, indexin = %d\n", input_cells, indexin);
      }
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (input_cells);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

    const int pilotgenerator_cc_impl::carriers_table[3][5] = {
      {6913, 6817, 6721, 6625, 6529},
      {13825, 13633, 13441, 13249, 13057},
      {27649, 27265, 26881, 26497, 26113}
    };

    const int pilotgenerator_cc_impl::preamble_dx_table[32] = {
      16, 8, 6, 4, 3, 4, 3, 32, 16, 12, 8, 6, 4, 3, 3, 4,
      4, 3, 32, 32, 24, 16, 12, 8, 6, 6, 8, 3, 8, 3, 3, 3
    };

    const double pilotgenerator_cc_impl::preamble_power_table[32] = {
      5.30, 3.60, 2.90, 1.80, 0.90, 1.80, 0.90, 6.80, 5.30, 4.60, 3.60, 2.90, 2.10, 1.30, 1.30, 2.10,
      2.10, 1.30, 6.80, 6.80, 6.20, 5.30, 4.60, 4.00, 3.20, 3.20, 4.00, 1.30, 4.00, 1.30, 1.30, 1.30
    };

    const double pilotgenerator_cc_impl::scattered_power_table[16][5] = {
      {0.00 , 0.00 , 1.40 , 2.20 , 2.90},
      {0.00 , 1.40 , 2.90 , 3.80 , 4.40},
      {0.00 , 0.60 , 2.10 , 3.00 , 3.60},
      {0.00 , 2.10 , 3.60 , 4.40 , 5.10},
      {0.00 , 1.60 , 3.10 , 4.00 , 4.60},
      {0.00 , 3.00 , 4.50 , 5.40 , 6.00},
      {0.00 , 2.20 , 3.80 , 4.60 , 5.30},
      {0.00 , 3.60 , 5.10 , 6.00 , 6.60},
      {0.00 , 3.20 , 4.70 , 5.60 , 6.20},
      {0.00 , 4.50 , 6.00 , 6.90 , 7.50},
      {0.00 , 3.80 , 5.30 , 6.20 , 6.80},
      {0.00 , 5.20 , 6.70 , 7.60 , 8.20},
      {0.00 , 4.70 , 6.20 , 7.10 , 7.70},
      {0.00 , 6.10 , 7.60 , 8.50 , 9.10},
      {0.00 , 5.40 , 6.90 , 7.70 , 8.40},
      {0.00 , 6.70 , 8.20 , 9.10 , 9.70}
    };

    const double pilotgenerator_cc_impl::preamble_ifft_power_table[32][5] = {
      {8240.53, 8130.20, 8013.76, 7897.31, 7780.87},
      {8322.93, 8211.44, 8093.84, 7976.24, 7858.64},
      {8301.50, 8190.31, 8073.00, 7955.69, 7838.38},
      {8094.28, 7985.96, 7871.52, 7757.08, 7642.65},
      {7737.10, 7633.73, 7524.25, 7414.77, 7305.30},
      {8094.28, 7985.96, 7871.52, 7757.08, 7642.65},
      {7737.10, 7633.73, 7524.25, 7414.77, 7305.30},
      {16051.13, 15836.42, 15603.37, 15382.54, 15155.60},
      {16477.67, 16257.01, 16018.01, 15791.24, 15558.36},
      {16583.95, 16361.81, 16121.33, 15893.08, 15658.71},
      {16643.58, 16420.60, 16179.28, 15950.19, 15714.99},
      {16601.06, 16378.66, 16137.94, 15909.43, 15674.81},
      {16561.26, 16339.42, 16099.24, 15871.28, 15637.21},
      {16020.04, 15805.70, 15573.04, 15352.59, 15126.04},
      {16020.04, 15805.70, 15573.04, 15352.59, 15126.04},
      {16561.26, 16339.42, 16099.24, 15871.28, 15637.21},
      {16561.26, 16339.42, 16099.24, 15871.28, 15637.21},
      {16020.04, 15805.70, 15573.04, 15352.59, 15126.04},
      {32097.48, 31668.05, 31201.95, 30760.29, 30306.41},
      {32097.48, 31668.05, 31201.95, 30760.29, 30306.41},
      {32475.84, 32041.14, 31569.77, 31122.85, 30663.71},
      {32951.95, 32510.63, 32032.64, 31579.09, 31113.33},
      {33165.03, 32720.74, 32239.78, 31783.27, 31314.54},
      {34048.92, 33592.35, 33099.12, 32630.32, 32149.31},
      {33842.90, 33389.18, 32898.80, 32432.87, 31954.71},
      {33842.90, 33389.18, 32898.80, 32432.87, 31954.71},
      {34048.92, 33592.35, 33099.12, 32630.32, 32149.31},
      {32038.72, 31610.06, 31144.72, 30703.83, 30250.72},
      {34048.92, 33592.35, 33099.12, 32630.32, 32149.31},
      {32038.72, 31610.06, 31144.72, 30703.83, 30250.72},
      {32038.72, 31610.06, 31144.72, 30703.83, 30250.72},
      {32038.72, 31610.06, 31144.72, 30703.83, 30250.72}
    };

    const double pilotgenerator_cc_impl::data_ifft_power_table_8K[16][5][5] = {
      {{7206.33, 7206.33, 7646.12, 7967.68, 8302.73}, {7110.33, 7110.33, 7543.95, 7861.57, 8191.33}, {7008.22, 7008.22, 7435.66, 7748.36, 8074.82}, {6906.11, 6906.11, 7327.38, 7636.14, 7957.32}, {6804.00, 6804.00, 7219.10, 7523.92, 7839.81}},
      {{7206.33, 7427.12, 7757.73, 8018.64, 8223.83}, {7110.33, 7327.95, 7654.33, 7910.88, 8114.70}, {7008.22, 7223.66, 7544.82, 7798.01, 7998.45}, {6906.11, 7118.38, 7434.32, 7684.13, 7882.20}, {6804.00, 7013.10, 7324.81, 7571.26, 7765.96}},
      {{7206.33, 7335.49, 7745.44, 8069.14, 8324.24}, {7110.33, 7236.94, 7641.52, 7961.26, 8213.26}, {7008.22, 7133.27, 7532.48, 7846.26, 8095.17}, {6906.11, 7029.60, 7422.45, 7732.26, 7978.08}, {6804.00, 6925.93, 7313.42, 7618.26, 7859.99}},
      {{7206.33, 7477.44, 7769.24, 7971.39, 8181.27}, {7110.33, 7378.52, 7666.26, 7865.29, 8072.61}, {7008.22, 7272.48, 7556.17, 7752.08, 7956.83}, {6906.11, 7166.45, 7446.08, 7639.87, 7841.06}, {6804.00, 7060.42, 7335.99, 7527.66, 7725.28}},
      {{7206.33, 7463.93, 7809.46, 8080.54, 8295.62}, {7110.33, 7364.80, 7704.79, 7972.35, 8184.48}, {7008.22, 7258.56, 7594.01, 7858.05, 8067.22}, {6906.11, 7153.32, 7484.23, 7743.75, 7949.97}, {6804.00, 7048.08, 7373.45, 7630.44, 7832.71}},
      {{7206.33, 7497.87, 7737.93, 7927.21, 8077.51}, {7110.33, 7397.95, 7634.83, 7821.73, 7969.81}, {7008.22, 7290.91, 7525.63, 7709.14, 7855.00}, {6906.11, 7184.88, 7415.42, 7597.55, 7741.20}, {6804.00, 7078.84, 7306.22, 7485.96, 7627.39}},
      {{7206.33, 7492.88, 7814.32, 8024.02, 8243.34}, {7110.33, 7392.96, 7709.54, 7917.41, 8132.67}, {7008.22, 7286.93, 7598.64, 7803.69, 8016.90}, {6906.11, 7180.91, 7488.74, 7689.97, 7900.13}, {6804.00, 7074.88, 7377.85, 7577.25, 7783.36}},
      {{7206.33, 7490.93, 7698.42, 7862.96, 7992.15}, {7110.33, 7391.44, 7595.59, 7758.19, 7885.30}, {7008.22, 7284.84, 7487.64, 7646.30, 7773.33}, {6906.11, 7179.24, 7378.70, 7535.42, 7660.37}, {6804.00, 7072.64, 7269.76, 7424.54, 7547.41}},
      {{7206.33, 7522.86, 7773.18, 7969.29, 8125.67}, {7110.33, 7422.14, 7668.57, 7863.25, 8017.32}, {7008.22, 7316.32, 7558.85, 7751.09, 7901.86}, {6906.11, 7209.49, 7449.13, 7637.93, 7787.40}, {6804.00, 7102.67, 7339.41, 7525.78, 7672.94}},
      {{7206.33, 7475.54, 7648.41, 7783.36, 7891.04}, {7110.33, 7375.99, 7546.56, 7680.17, 7786.06}, {7008.22, 7270.34, 7437.60, 7569.88, 7674.96}, {6906.11, 7164.68, 7329.64, 7460.59, 7562.86}, {6804.00, 7059.02, 7221.68, 7350.29, 7451.76}},
      {{7206.33, 7512.03, 7727.53, 7897.38, 8031.80}, {7110.33, 7411.63, 7624.20, 7792.37, 7925.08}, {7008.22, 7305.13, 7514.76, 7680.24, 7811.25}, {6906.11, 7198.63, 7405.31, 7569.12, 7698.43}, {6804.00, 7093.12, 7296.87, 7457.00, 7584.60}},
      {{7206.33, 7466.13, 7618.63, 7738.99, 7835.14}, {7110.33, 7363.26, 7511.56, 7629.46, 7721.49}, {7008.22, 7260.28, 7409.39, 7526.82, 7619.74}, {6906.11, 7151.31, 7296.21, 7410.19, 7500.99}, {6804.00, 7049.33, 7194.04, 7308.55, 7399.24}},
      {{7206.33, 7491.23, 7669.09, 7809.50, 7920.09}, {7110.33, 7391.43, 7567.41, 7704.99, 7814.54}, {7008.22, 7285.51, 7458.63, 7595.36, 7702.87}, {6906.11, 7179.60, 7349.84, 7484.74, 7591.21}, {6804.00, 7073.68, 7242.05, 7374.11, 7479.54}},
      {{7206.33, 7440.66, 7568.35, 7669.30, 7748.41}, {7110.33, 7341.37, 7467.34, 7566.98, 7645.90}, {7008.22, 7235.96, 7360.21, 7458.55, 7536.28}, {6906.11, 7130.55, 7254.08, 7350.12, 7426.65}, {6804.00, 7026.15, 7146.95, 7242.69, 7318.03}},
      {{7206.33, 7477.75, 7635.15, 7744.12, 7857.61}, {7110.33, 7377.35, 7531.46, 7638.46, 7749.85}, {7008.22, 7272.84, 7425.66, 7531.68, 7641.99}, {6906.11, 7165.32, 7315.85, 7419.91, 7528.12}, {6804.00, 7060.81, 7210.05, 7313.13, 7420.25}},
      {{7206.33, 7420.32, 7532.04, 7620.18, 7690.50}, {7110.33, 7313.29, 7419.22, 7502.79, 7569.50}, {7008.22, 7211.14, 7317.29, 7400.29, 7467.39}, {6906.11, 7109.00, 7215.35, 7298.80, 7365.28}, {6804.00, 7006.86, 7112.42, 7196.30, 7263.17}}
    };

    const double pilotgenerator_cc_impl::data_ifft_power_table_16K[16][5][5] = {
      {{14411.67, 14411.67, 15288.86, 15932.70, 16602.50}, {14219.67, 14219.67, 15085.51, 15720.49, 16379.71}, {14009.33, 14009.33, 14862.84, 15488.94, 16139.59}, {13811.22, 13811.22, 14652.38, 15269.62, 15910.69}, {13607.00, 13607.00, 14435.81, 15044.18, 15676.67}},
      {{14411.67, 14851.86, 15510.50, 16028.89, 16439.91}, {14219.67, 14653.51, 15302.71, 15814.36, 16219.64}, {14009.33, 14437.84, 15077.59, 15582.50, 15982.03}, {13811.22, 14233.38, 14863.69, 15361.87, 15755.65}, {13607.00, 14022.81, 14644.67, 15135.12, 15523.16}},
      {{14411.67, 14668.83, 15488.26, 16134.29, 16645.20}, {14219.67, 14472.72, 15281.42, 15918.52, 16422.23}, {14009.33, 14259.28, 15055.24, 15683.41, 16180.94}, {13811.22, 14057.06, 14842.28, 15461.53, 15951.87}, {13607.00, 13849.72, 14623.21, 15233.53, 15716.68}},
      {{14411.67, 14952.26, 15532.20, 15935.04, 16353.30}, {14219.67, 14752.42, 15325.23, 15721.83, 16133.97}, {14009.33, 14534.24, 15098.94, 15490.30, 15897.32}, {13811.22, 14329.28, 14885.87, 15270.98, 15671.88}, {13607.00, 14117.21, 14665.68, 15045.56, 15441.33}},
      {{14411.67, 14926.41, 15613.87, 16156.56, 16586.36}, {14219.67, 14727.15, 15405.54, 15941.18, 16364.07}, {14009.33, 14509.56, 15178.87, 15706.47, 16123.45}, {13811.22, 14304.20, 14963.42, 15483.98, 15895.05}, {13607.00, 14092.72, 14742.86, 15255.38, 15661.54}},
      {{14411.67, 14989.75, 15467.04, 15842.95, 16141.04}, {14219.67, 14789.90, 15259.85, 15632.00, 15925.64}, {14009.33, 14570.72, 15035.33, 15401.71, 15690.92}, {13811.22, 14364.76, 14823.03, 15183.64, 15469.41}, {13607.00, 14152.69, 14603.62, 14959.46, 15240.79}},
      {{14411.67, 14983.09, 15623.25, 16044.16, 16480.28}, {14219.67, 14783.26, 15414.68, 15828.94, 16259.96}, {14009.33, 14565.10, 15187.77, 15596.39, 16021.30}, {13811.22, 14359.16, 14973.09, 15375.06, 15793.87}, {13607.00, 14147.11, 14751.29, 15148.62, 15561.33}},
      {{14411.67, 14974.58, 15386.60, 15711.94, 15968.72}, {14219.67, 14775.60, 15181.94, 15502.39, 15756.02}, {14009.33, 14557.28, 14957.94, 15273.51, 15523.99}, {13811.22, 14351.19, 14746.17, 15057.86, 15304.17}, {13607.00, 14138.99, 14528.28, 14836.09, 15078.25}},
      {{14411.67, 15041.63, 15540.41, 15932.96, 16244.17}, {14219.67, 14841.20, 15332.19, 15719.86, 16026.47}, {14009.33, 14622.43, 15106.64, 15488.44, 15790.44}, {13811.22, 14414.89, 14892.31, 15269.23, 15567.63}, {13607.00, 14202.24, 14672.87, 15043.92, 15337.71}},
      {{14411.67, 14943.26, 15282.84, 15550.82, 15762.46}, {14219.67, 14744.17, 15079.15, 15342.45, 15551.49}, {14009.33, 14526.74, 14856.11, 15116.75, 15323.18}, {13811.22, 14320.54, 14646.31, 14903.28, 15106.09}, {13607.00, 14109.22, 14430.39, 14683.69, 14883.90}},
      {{14411.67, 15019.66, 15448.67, 15787.59, 16055.82}, {14219.67, 14818.87, 15242.01, 15576.56, 15840.38}, {14009.33, 14599.75, 15018.01, 15347.20, 15607.61}, {13811.22, 14393.85, 14805.24, 15130.07, 15387.07}, {13607.00, 14180.85, 14586.36, 14906.82, 15159.41}},
      {{14411.67, 14920.95, 15221.58, 15458.22, 15645.66}, {14219.67, 14722.22, 15018.45, 15252.17, 15436.38}, {14009.33, 14504.15, 14796.99, 15026.78, 15209.77}, {13811.22, 14299.30, 14587.75, 14814.62, 14994.37}, {13607.00, 14088.34, 14372.40, 14596.34, 14773.87}},
      {{14411.67, 14978.51, 15331.00, 15609.88, 15830.29}, {14219.67, 14777.90, 15126.65, 15400.85, 15618.19}, {14009.33, 14559.96, 14902.97, 15174.49, 15388.75}, {13811.22, 14354.24, 14692.51, 14959.35, 15170.53}, {13607.00, 14142.41, 14475.94, 14739.09, 14947.20}},
      {{14411.67, 14867.25, 15115.96, 15311.51, 15466.70}, {14219.67, 14668.66, 14913.92, 15107.88, 15260.67}, {14009.33, 14452.74, 14694.55, 14884.91, 15036.31}, {13811.22, 14248.04, 14486.41, 14675.16, 14824.17}, {13607.00, 14038.22, 14273.15, 14458.30, 14605.93}},
      {{14411.67, 14950.04, 15261.41, 15477.36, 15702.29}, {14219.67, 14750.23, 15058.02, 15271.03, 15492.78}, {14009.33, 14533.10, 14836.30, 15046.37, 15264.94}, {13811.22, 14327.18, 14626.80, 14832.92, 15048.32}, {13607.00, 14116.15, 14410.20, 14614.37, 14826.59}},
      {{14411.67, 14823.96, 15040.47, 15210.22, 15345.66}, {14219.67, 14620.90, 14830.83, 14997.45, 15128.66}, {14009.33, 14410.50, 14620.85, 14786.35, 14918.33}, {13811.22, 14201.32, 14406.10, 14567.47, 14695.23}, {13607.00, 13997.04, 14202.24, 14363.48, 14491.01}}
    };

    const double pilotgenerator_cc_impl::data_ifft_power_table_32K[16][5][5] = {
      {{28822.33, 28822.33, 30576.34, 31863.75, 33202.05}, {28438.33, 28438.33, 30167.65, 31437.32, 32756.47}, {28017.67, 28017.67, 29723.29, 30974.22, 32275.22}, {27621.44, 27621.44, 29302.38, 30535.58, 31818.42}, {27213.00, 27213.00, 28869.25, 30085.70, 31349.40}},
      {{28822.33, 29700.34, 31015.05, 32051.38, 32872.06}, {28438.33, 29304.65, 30600.47, 31622.33, 32431.52}, {28017.67, 28871.29, 30149.22, 31157.61, 31954.31}, {27621.44, 28463.38, 29723.42, 30716.34, 31502.55}, {27213.00, 28042.25, 29284.40, 30262.84, 31037.56}},
      {{0.00, 0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00, 0.00}},
      {{0.00, 0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00, 0.00}},
      {{28822.33, 29850.37, 31224.70, 32309.62, 33167.83}, {28438.33, 29451.86, 30808.03, 31876.86, 32723.26}, {28017.67, 29016.68, 30353.69, 31407.43, 32242.01}, {27621.44, 28605.95, 29923.80, 30963.45, 31785.21}, {27213.00, 28184.00, 29482.69, 30506.24, 31317.19}},
      {{28822.33, 29973.50, 30925.26, 31675.43, 32269.09}, {28438.33, 29573.80, 30511.88, 31251.52, 31837.30}, {28017.67, 29136.44, 30061.84, 30791.94, 31368.85}, {27621.44, 28724.52, 29636.24, 30355.81, 30924.84}, {27213.00, 28300.38, 29199.42, 29907.46, 30468.61}},
      {{28822.33, 29963.53, 31243.10, 32082.43, 32955.18}, {28438.33, 29563.87, 30824.95, 31653.00, 32513.53}, {28017.67, 29127.54, 30371.14, 31186.90, 32035.22}, {27621.44, 28715.66, 29940.78, 30745.24, 31582.35}, {27213.00, 28291.55, 29499.19, 30292.36, 31116.26}},
      {{28822.33, 29942.86, 30763.97, 31410.90, 31921.87}, {28438.33, 29543.90, 30352.64, 30990.81, 31495.47}, {28017.67, 29107.27, 29905.65, 30534.05, 31032.40}, {27621.44, 28696.09, 29482.10, 30101.74, 30592.78}, {27213.00, 28271.68, 29047.33, 29658.20, 30141.93}},
      {{28822.33, 30080.16, 31074.87, 31858.28, 32479.17}, {28438.33, 29678.30, 30659.43, 31433.10, 32044.77}, {28017.67, 29240.78, 30207.33, 30970.25, 31573.71}, {27621.44, 28826.70, 29779.67, 30531.84, 31126.09}, {27213.00, 28400.40, 29340.78, 30081.21, 30667.25}},
      {{28822.33, 29877.71, 30551.70, 31083.74, 31504.30}, {28438.33, 29478.52, 30144.31, 30668.01, 31083.35}, {28017.67, 29043.66, 29699.25, 30216.61, 30625.74}, {27621.44, 28633.25, 29279.63, 29788.66, 30192.56}, {27213.00, 28209.62, 28846.79, 29349.48, 29747.17}},
      {{28822.33, 30033.92, 30890.95, 31567.00, 32101.85}, {28438.33, 29633.34, 30478.63, 31144.96, 31671.98}, {28017.67, 29196.10, 30029.64, 30686.24, 31206.44}, {27621.44, 28783.31, 29604.09, 30251.97, 30764.35}, {27213.00, 28357.29, 29167.33, 29805.48, 30311.03}},
      {{28822.33, 29830.59, 30426.47, 30895.69, 31267.72}, {28438.33, 29433.12, 30020.22, 30483.58, 30850.16}, {28017.67, 28997.98, 29577.29, 30033.81, 30395.92}, {27621.44, 28588.29, 29158.82, 29609.48, 29966.13}, {27213.00, 28165.37, 28728.11, 29171.93, 29523.12}},
      {{28822.33, 29951.08, 30654.84, 31209.63, 31648.70}, {28438.33, 29550.86, 30245.14, 30792.57, 31225.49}, {28017.67, 29114.97, 29798.77, 30338.84, 30765.60}, {27621.44, 28702.53, 29377.85, 29909.56, 30330.17}, {27213.00, 28278.87, 28943.71, 29468.06, 29882.51}},
      {{28822.33, 29720.43, 30211.16, 30597.95, 30904.27}, {28438.33, 29324.25, 29808.09, 30189.68, 30492.22}, {28017.67, 28891.40, 29368.35, 29744.74, 30042.50}, {27621.44, 28483.00, 28953.06, 29324.25, 29618.22}, {27213.00, 28061.37, 28525.54, 28891.53, 29180.72}},
      {{28822.33, 29893.61, 30514.92, 30944.83, 31391.67}, {28438.33, 29495.00, 30107.15, 30531.17, 30971.65}, {28017.67, 29059.72, 29662.71, 30080.84, 30515.97}, {27621.44, 28647.89, 29243.71, 29655.96, 30083.72}, {27213.00, 28224.84, 28811.49, 29217.85, 29640.26}},
      {{28822.33, 29632.24, 30056.33, 30391.32, 30655.98}, {28438.33, 29237.11, 29655.05, 29985.78, 30246.99}, {28017.67, 28805.32, 29218.10, 29543.57, 29801.34}, {27621.44, 28397.97, 28804.59, 29125.81, 29380.12}, {27213.00, 27978.40, 28379.87, 28695.83, 28946.69}}
    };

    const int pilotgenerator_cc_impl::continual_pilot_table_8K[48] = {
      59, 167, 307, 469, 637, 751, 865, 1031, 1159, 1333, 1447, 1607, 1811, 1943, 2041, 2197,
      2323, 2519, 2605, 2767, 2963, 3029, 3175, 3325, 3467, 3665, 3833, 3901, 4073, 4235, 4325, 4511,
      4627, 4825, 4907, 5051, 5227, 5389, 5531, 5627, 5833, 5905, 6053, 6197, 6353, 6563, 6637, 6809
    };

    const int pilotgenerator_cc_impl::continual_pilot_table_16K[96] = {
      118, 178, 334, 434, 614, 670, 938, 1070, 1274, 1358, 1502, 1618, 1730, 1918, 2062, 2078,
      2318, 2566, 2666, 2750, 2894, 3010, 3214, 3250, 3622, 3686, 3886, 3962, 4082, 4166, 4394, 4558,
      4646, 4718, 5038, 5170, 5210, 5342, 5534, 5614, 5926, 5942, 6058, 6134, 6350, 6410, 6650, 6782,
      6934, 7154, 7330, 7438, 7666, 7742, 7802, 7894, 8146, 8258, 8470, 8494, 8650, 8722, 9022, 9118,
      9254, 9422, 9650, 9670, 9814, 9902, 10102, 10166, 10454, 10598, 10778, 10822, 11062, 11138, 11254, 11318,
      11666, 11758, 11810, 11974, 12106, 12242, 12394, 12502, 12706, 12866, 13126, 13190, 13274, 13466, 13618, 13666
    };

    const int pilotgenerator_cc_impl::continual_pilot_table_32K[192] = {
      236, 316, 356, 412, 668, 716, 868, 1100, 1228, 1268, 1340, 1396, 1876, 1916, 2140, 2236,
      2548, 2644, 2716, 2860, 3004, 3164, 3236, 3436, 3460, 3700, 3836, 4028, 4124, 4132, 4156, 4316,
      4636, 5012, 5132, 5140, 5332, 5372, 5500, 5524, 5788, 6004, 6020, 6092, 6428, 6452, 6500, 6740,
      7244, 7316, 7372, 7444, 7772, 7844, 7924, 8020, 8164, 8308, 8332, 8348, 8788, 8804, 9116, 9140,
      9292, 9412, 9436, 9604, 10076, 10204, 10340, 10348, 10420, 10660, 10684, 10708, 11068, 11132, 11228, 11356,
      11852, 11860, 11884, 12044, 12116, 12164, 12268, 12316, 12700, 12772, 12820, 12988, 13300, 13340, 13564, 13780,
      13868, 14084, 14308, 14348, 14660, 14828, 14876, 14948, 15332, 15380, 15484, 15532, 15604, 15764, 15788, 15796,
      16292, 16420, 16516, 16580, 16940, 16964, 16988, 17228, 17300, 17308, 17444, 17572, 18044, 18212, 18236, 18356,
      18508, 18532, 18844, 18860, 19300, 19316, 19340, 19484, 19628, 19724, 19804, 19876, 20204, 20276, 20332, 20404,
      20908, 21148, 21196, 21220, 21556, 21628, 21644, 21860, 22124, 22148, 22276, 22316, 22508, 22516, 22636, 23012,
      23332, 23492, 23516, 23524, 23620, 23812, 23948, 24188, 24212, 24412, 24484, 24644, 24788, 24932, 25004, 25100,
      25412, 25508, 25732, 25772, 26252, 26308, 26380, 26420, 26548, 26780, 26932, 26980, 27236, 27292, 27332, 27412
    };

    const int pilotgenerator_cc_impl::preamble_cells_table[32][5] = {
      {6432, 6342, 6253, 6164, 6075},
      {6000, 5916, 5833, 5750, 5667},
      {5712, 5632, 5553, 5474, 5395},
      {5136, 5064, 4993, 4922, 4851},
      {4560, 4496, 4433, 4370, 4307},
      {5136, 5064, 4993, 4922, 4851},
      {4560, 4496, 4433, 4370, 4307},
      {13296, 13110, 12927, 12742, 12558},
      {12864, 12684, 12507, 12328, 12150},
      {12576, 12400, 12227, 12052, 11878},
      {12000, 11832, 11667, 11500, 11334},
      {11424, 11264, 11107, 10948, 10790},
      {10272, 10128, 9987, 9844, 9702},
      {9120, 8992, 8867, 8740, 8614},
      {9120, 8992, 8867, 8740, 8614},
      {10272, 10128, 9987, 9844, 9702},
      {10272, 10128, 9987, 9844, 9702},
      {9120, 8992, 8867, 8740, 8614},
      {26592, 26220, 25854, 25484, 25116},
      {26592, 26220, 25854, 25484, 25116},
      {26304, 25936, 25574, 25208, 24844},
      {25728, 25368, 25014, 24656, 24300},
      {25152, 24800, 24454, 24104, 23756},
      {24000, 23664, 23334, 23000, 22668},
      {22848, 22528, 22214, 21896, 21580},
      {22848, 22528, 22214, 21896, 21580},
      {24000, 23664, 23334, 23000, 22668},
      {18240, 17984, 17734, 17480, 17228},
      {24000, 23664, 23334, 23000, 22668},
      {18240, 17984, 17734, 17480, 17228},
      {18240, 17984, 17734, 17480, 17228},
      {18240, 17984, 17734, 17480, 17228}
    };

    const int pilotgenerator_cc_impl::data_cells_table_8K[16][5] = {
      {5711, 5631, 5552, 5473, 5394},
      {6285, 6197, 6110, 6023, 5936},
      {5999, 5915, 5832, 5749, 5666},
      {6429, 6339, 6250, 6161, 6072},
      {6287, 6199, 6112, 6025, 5938},
      {6573, 6481, 6390, 6299, 6208},
      {6431, 6341, 6252, 6163, 6074},
      {6645, 6552, 6460, 6368, 6276},
      {6575, 6483, 6392, 6301, 6210},
      {6717, 6623, 6530, 6437, 6344},
      {6647, 6554, 6462, 6370, 6278},
      {6753, 6660, 6565, 6473, 6378},
      {6719, 6625, 6532, 6439, 6346},
      {6789, 6694, 6600, 6506, 6412},
      {6755, 6661, 6567, 6474, 6380},
      {6807, 6714, 6619, 6524, 6429}
    };

    const int pilotgenerator_cc_impl::data_cells_table_16K[16][5] = {
      {11423, 11263, 11106, 10947, 10789},
      {12573, 12397, 12224, 12049, 11875},
      {11999, 11831, 11666, 11499, 11333},
      {12861, 12681, 12504, 12325, 12147},
      {12575, 12399, 12226, 12051, 11877},
      {13149, 12965, 12784, 12601, 12419},
      {12863, 12683, 12506, 12327, 12149},
      {13293, 13107, 12924, 12739, 12555},
      {13151, 12967, 12786, 12603, 12421},
      {13437, 13249, 13064, 12877, 12691},
      {13295, 13109, 12926, 12741, 12557},
      {13509, 13320, 13134, 12946, 12759},
      {13439, 13251, 13066, 12879, 12693},
      {13581, 13391, 13204, 13015, 12827},
      {13511, 13322, 13136, 12948, 12761},
      {13617, 13428, 13239, 13051, 12861}
    };

    const int pilotgenerator_cc_impl::data_cells_table_32K[16][5] = {
      {22847, 22527, 22213, 21895, 21579},
      {25149, 24797, 24451, 24101, 23753},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {25151, 24799, 24453, 24103, 23755},
      {26301, 25933, 25571, 25205, 24841},
      {25727, 25367, 25013, 24655, 24299},
      {26589, 26217, 25851, 25481, 25113},
      {26303, 25935, 25573, 25207, 24843},
      {26877, 26501, 26131, 25757, 25385},
      {26591, 26219, 25853, 25483, 25115},
      {27021, 26643, 26271, 25895, 25521},
      {26879, 26503, 26133, 25759, 25387},
      {27165, 26785, 26411, 26033, 25657},
      {27023, 26645, 26273, 25897, 25523},
      {27237, 26856, 26481, 26102, 25725}
    };

    const int pilotgenerator_cc_impl::sbs_cells_table_8K[16][5] = {
      {4560, 4496, 4433, 4370, 4307},
      {4560, 4496, 4433, 4370, 4307},
      {5136, 5064, 4993, 4922, 4851},
      {5136, 5064, 4993, 4922, 4851},
      {5712, 5632, 5553, 5474, 5395},
      {5712, 5632, 5553, 5474, 5395},
      {6000, 5916, 5833, 5750, 5667},
      {6000, 5916, 5833, 5750, 5667},
      {6288, 6200, 6113, 6026, 5939},
      {6288, 6200, 6113, 6026, 5939},
      {6432, 6342, 6253, 6164, 6075},
      {6432, 6342, 6253, 6164, 6075},
      {6576, 6484, 6393, 6302, 6211},
      {6576, 6484, 6393, 6302, 6211},
      {6648, 6555, 6463, 6371, 6279},
      {6648, 6555, 6463, 6371, 6279},
    };

    const int pilotgenerator_cc_impl::sbs_cells_table_16K[16][5] = {
      {9120, 8992, 8867, 8740, 8614},
      {9120, 8992, 8867, 8740, 8614},
      {10272, 10128, 9987, 9844, 9702},
      {10272, 10128, 9987, 9844, 9702},
      {11424, 11264, 11107, 10948, 10790},
      {11424, 11264, 11107, 10948, 10790},
      {12000, 11832, 11667, 11500, 11334},
      {12000, 11832, 11667, 11500, 11334},
      {12576, 12400, 12227, 12052, 11878},
      {12576, 12400, 12227, 12052, 11878},
      {12864, 12684, 12507, 12328, 12150},
      {12864, 12684, 12507, 12328, 12150},
      {13152, 12968, 12787, 12604, 12422},
      {13152, 12968, 12787, 12604, 12422},
      {13296, 13110, 12927, 12742, 12558},
      {13296, 13110, 12927, 12742, 12558}
    };

    const int pilotgenerator_cc_impl::sbs_cells_table_32K[16][5] = {
      {18240, 17984, 17734, 17480, 17228},
      {18240, 17984, 17734, 17480, 17228},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {22848, 22528, 22214, 21896, 21580},
      {22848, 22528, 22214, 21896, 21580},
      {24000, 23664, 23334, 23000, 22668},
      {24000, 23664, 23334, 23000, 22668},
      {25152, 24800, 24454, 24104, 23756},
      {25152, 24800, 24454, 24104, 23756},
      {25728, 25368, 25014, 24656, 24300},
      {25728, 25368, 25014, 24656, 24300},
      {26304, 25936, 25574, 25208, 24844},
      {26304, 25936, 25574, 25208, 24844},
      {26592, 26220, 25854, 25484, 25116},
      {26592, 26220, 25854, 25484, 25116}
    };

  } /* namespace atsc3 */
} /* namespace gr */
