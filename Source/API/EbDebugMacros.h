/*
* Copyright(c) 2020 Intel Corporation
*
* This source code is subject to the terms of the BSD 2 Clause License and
* the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
* was not distributed with this source code in the LICENSE file, you can
* obtain it at https://www.aomedia.org/license/software-license. If the Alliance for Open
* Media Patent License 1.0 was not distributed with this source code in the
* PATENTS file, you can obtain it at https://www.aomedia.org/license/patent-license.
*/


/*
* This file contains only debug macros that are used during the development
* and are supposed to be cleaned up every tag cycle
* all macros must have the following format:
* - adding a new feature should be prefixed by FTR_
* - tuning a feature should be prefixed by TUNE_
* - enabling a feature should be prefixed by EN_
* - disabling a feature should be prefixed by DIS_
* - bug fixes should be prefixed by FIX_
* - code refactors should be prefixed by RFCTR_
* - code cleanups should be prefixed by CLN_
* - all macros must have a coherent comment explaining what the MACRO is doing
* - #if 0 / #if 1 are not to be used
*/


#ifndef EbDebugMacros_h
#define EbDebugMacros_h

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    // ============= START SVT_04 =============
#define FIX_IME                       1 // Fix inloop me
#define FTR_TPL_TR                    1 // TPL trailing  support

#define RFCTR_MD_BLOCK_LOOP           1 // Refactor the loop that iterates over all blocks at MD
#define CLN_REMOVE_UNUSED_SIGNALS     1 // Remove mds3_intra_prune_th, and skip_cfl_cost_dev_th
#define FIX_Y_COEFF_FLAG_UPDATE       1 // Fix bug where y_has_coeff flag is overwritten by non-selected tx_types during tx_type_search
#define CLN_ENC_MODE_CHECK            1 // Make enc mode check conform to convention of using "<="
#define FTR_NEW_CYCLES_ALLOC          1 // Replace old cycles allocation with a cycles allocation algorithm that
                                        // does not depend on stats.  Merge cycles allocation and zero-sq-coeff feature.
#define TUNE_M8_TO_MATCH_M7                1
#define FTR_PD2_BLOCK_REDUCTION            1 // Reduce depth refinement based on the complexity of the SB.
#define FTR_REDUCE_MDS2_CAND               1 // Reduce mds3 candidates when mds0 and mds1 select the same best candidate.
#define FTR_DISABLE_ADAPTIVE_ME            1 // Disable adaptive ME
#define FTR_PD2_REDUCE_INTRA               1 // Reduce intra when the me distortion is low
#define CLN_CLEANUP_MDC_CTX                1 // Cleanup mdc context
#define FTR_USE_VAR_IN_FAST_LOOP           1 // Use var in fast loop
#define CLN_REMOVE_UNUSED_CODE             1 // Remove unused code related to nsq stat
#define FTR_PD2_REDUCE_MDS0                1 // Reduce the number of injected blocks
#define FTR_REDUCE_TXT_BASED_ON_DISTORTION 1 // Reduce the number of injected blocks
#define FTR_USE_VAR_IN_FAST_LOOP10BIT      1 // Use var in fast loop 10bit
    // ============= END SVT_04 =============
//FOR DEBUGGING - Do not remove
#define NO_ENCDEC               0 // bypass encDec to test cmpliance of MD. complained achieved when skip_flag is OFF. Port sample code from VCI-SW_AV1_Candidate1 branch
#define DEBUG_TPL               0 // Prints to debug TPL
#define DETAILED_FRAME_OUTPUT   0 // Prints detailed frame output from the library for debugging
#define TUNE_CHROMA_SSIM        0 // Allows for Chroma and SSIM BDR-based Tuning
#define FTR_ENABLE_FIXED_QINDEX_OFFSETS 1

#define FIX_DDL                 1 // Fix deadlock issues
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // EbDebugMacros_h
