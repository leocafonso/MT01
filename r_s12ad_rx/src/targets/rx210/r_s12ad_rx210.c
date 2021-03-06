/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_s12ad_rx210.c
* Description  : ROM tables and support code for RX210.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*           22.02.2014 1.00    Initial Release.
*           05.06.2014 1.30    Fixed channel mask bug in adc_enable_chans()
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
#ifdef BSP_MCU_RX210
#include "r_s12ad_rx_private.h"
#include "r_s12ad_rx_config.h"
/* Public interface header file for this package. */
#include "r_s12ad_rx210_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
extern adc_ctrl_t g_dcb;

static adc_err_t adc_enable_chans(adc_ch_cfg_t *p_config);
extern void adc_enable_s12adi0(void);


/* In ROM. A/D Data Register pointers */

volatile __evenaccess uint16_t * const  dreg_ptrs[ADC_REG_MAX] =
                      { &S12AD.ADDR0,       // channel 0
                        &S12AD.ADDR1,       // channel 1
                        &S12AD.ADDR2,       // channel 2…
                        &S12AD.ADDR3,
                        &S12AD.ADDR4,
                        &S12AD.ADDR5,       
                        &S12AD.ADDR6,
                        &S12AD.ADDR7,       
                        &S12AD.ADDR8,
                        &S12AD.ADDR9,
                        &S12AD.ADDR10,
                        &S12AD.ADDR11,
                        &S12AD.ADDR12,
                        &S12AD.ADDR13,
                        &S12AD.ADDR14,
                        &S12AD.ADDR15,
                        &S12AD.ADTSDR,    // temperature sensor
                        &S12AD.ADOCDR,    // voltage sensor
                        &S12AD.ADDBLDR,   // double trigger register
                        &S12AD.ADRD.WORD  // self-diagnosis register
                      };

                      
/* In ROM. Sample State (SST) Register pointers */

// 8-bit register pointers
volatile __evenaccess uint8_t * const  sreg_ptrs[ADC_SST_REG_MAX] =
                      { &S12AD.ADSSTR0,     // channel 0
                        &S12AD.ADSSTR1,     // channel 1
                        &S12AD.ADSSTR2,     // channel 2…
                        &S12AD.ADSSTR3,
                        &S12AD.ADSSTR4,
                        &S12AD.ADSSTR5,
                        &S12AD.ADSSTR6,
                        &S12AD.ADSSTR7,
                        &S12AD.ADSSTRL,     // channels 8-15
                        &S12AD.ADSSTRT,     // temperature sensor
                        &S12AD.ADSSTRO      // voltage sensor
                      };


/******************************************************************************
* Function Name: adc_open
* Description  : This function applies power to the A/D peripheral, sets the
*                operational mode, trigger sources, interrupt priority, and
*                configurations common to all channels and sensors. If interrupt
*                priority is non-zero, the function takes a callback function
*                pointer for notifying the user at interrupt level whenever a
*                scan has completed.
*
* NOTE: The temperature sensor on the RX210 functionally behaves like a
*       regular software trigger. But instead of using the ADST bit, it has its
*       own PGAEN bit. For this bit to work, you must also have TRSA=0x0A,
*       TRGE=1, MSTP(TEMPS)=0, and TSEN=1.
*       The ADST bit will work with this configuration and will also work with
*       just MSTP(TEMPS)=0 and TSEN=1. However, the value read does not include
*       the temperature sensor gain value. This behavior is not documented in
*       the HW Manual, and accuracy is unknown.
*       Because of this, and portability concerns, the driver API is written
*       to look like the temp sensor runs on a normal software trigger.
*
*       -Gain values will always be read.
*       -ADST could be used to check for scan complete when using PGAEN.
*
* Arguments    : mode-
*                    Operational mode (see enumeration below)
*                p_cfg-
*                    Pointer to configuration structure (see below)
*                p_callback-
*                    Optional pointer to function called from interrupt when
*                    a scan completes
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_AD_LOCKED-
*                    Open() call is in progress elsewhere
*                ADC_ERR_AD_NOT_CLOSED-
*                    Peripheral is still running in another mode; Perform
*                    R_ADC_Close() first
*                ADC_ERR_INVALID_ARG-
*                    mode or element of p_cfg structure has invalid value.
*                ADC_ERR_ILLEGAL_ARG-
*                    an argument is illegal based upon mode
*                ADC_ERR_MISSING_PTR-
*                    p_cfg pointer is FIT_NO_PTR/NULL
*******************************************************************************/
adc_err_t adc_open(adc_mode_t const       mode,
                   adc_cfg_t * const      p_cfg,
                   void         (* const  p_callback)(void *p_args))
{


#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    if ((p_cfg == NULL) || (p_cfg == FIT_NO_PTR))
    {
        return ADC_ERR_MISSING_PTR;
    }

    /* Check for valid argument values */
    if ((mode >= ADC_MODE_MAX)
     || ((p_cfg->trigger >= ADC_TRIG_HW_MAX) && (p_cfg->trigger != ADC_TRIG_SOFTWARE))
     || (p_cfg->priority > BSP_MCU_IPL_MAX)
     || (p_cfg->add_cnt >= ADC_ADD_MAX)
     || (p_cfg->trigger == ADC_TRIG_PLACEHOLDER)
     || ((p_cfg->clearing != ADC_CLEAR_AFTER_READ_OFF) && (p_cfg->clearing != ADC_CLEAR_AFTER_READ_ON)))

    {
        return ADC_ERR_INVALID_ARG;
    }

    /* If interrupt driven, must have callback function */
    if ((p_cfg->priority != 0)
     && ((p_callback == NULL) || (p_callback == FIT_NO_FUNC)))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    if (p_cfg->add_cnt == ADC_ADD_OFF)
    {
        /* Check alignment values only if addition is off */
        if ((p_cfg->alignment != ADC_ALIGN_LEFT) && (p_cfg->alignment != ADC_ALIGN_RIGHT))
        {
            return ADC_ERR_INVALID_ARG;
        }
    }
    else // addition on
    {
        /* Addition not allowed with temperature sensor on RX210 */
        if (mode == ADC_MODE_SS_TEMPERATURE)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    /* For portability, only allow software trigger because that is the functional
     * behavior of the sensor. Will map to actual "synchronous temperature trigger"
     * (0x0A) later. */
    if ((mode == ADC_MODE_SS_TEMPERATURE) && (p_cfg->trigger != ADC_TRIG_SOFTWARE))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }


    /* In double trigger mode, SW and async triggers not allowed */
    if ((mode == ADC_MODE_SS_ONE_CH_DBLTRIG)
     && ((p_cfg->trigger == ADC_TRIG_SOFTWARE) || (p_cfg->trigger == ADC_TRIG_ASYNC_ADTRG0)))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    /* Group checking; only synchronous triggers allowed; must be unique */
    if ((mode == ADC_MODE_SS_MULTI_CH_GROUPED) || (mode == ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A))
    {
        if ((p_cfg->trigger == ADC_TRIG_ASYNC_ADTRG0)
         || (p_cfg->trigger_groupb == ADC_TRIG_ASYNC_ADTRG0)
         || (p_cfg->trigger_groupb == ADC_TRIG_PLACEHOLDER)
         || (p_cfg->trigger == p_cfg->trigger_groupb)
         || (p_cfg->trigger == ADC_TRIG_SOFTWARE)
         || (p_cfg->trigger_groupb == ADC_TRIG_SOFTWARE))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        if ((p_cfg->priority_groupb > BSP_MCU_IPL_MAX)
         || (p_cfg->trigger >= ADC_TRIG_HW_MAX)
         || (p_cfg->trigger_groupb >= ADC_TRIG_HW_MAX))
        {
            return ADC_ERR_INVALID_ARG;
        }

        if ((p_cfg->priority_groupb != 0)   // interrupt driven; must have callback func
         && ((p_callback == NULL) || (p_callback == FIT_NO_FUNC)))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }
#endif // parameter checking


    if (g_dcb.opened == true)
    {
        return ADC_ERR_AD_NOT_CLOSED;
    }
    if (R_BSP_HardwareLock(BSP_LOCK_S12AD) == false)
    {
        return ADC_ERR_AD_LOCKED;
    }


    /* APPLY POWER TO PERIPHERAL */

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    MSTP(S12AD) = 0;
    if (mode == ADC_MODE_SS_TEMPERATURE)
    {
        MSTP(TEMPS) = 0;
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    S12AD.ADCSR.WORD = 0;
    S12AD.ADEXICR.WORD = 0;
    TEMPS.TSCR.BYTE = 0;


    /* SET MODE RELATED REGISTER FIELDS */

    g_dcb.mode = mode;
    if ((mode == ADC_MODE_SS_MULTI_CH_GROUPED)
     || (mode == ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A))
    {
        S12AD.ADCSR.BIT.ADCS = ADC_ADCS_GROUP_SCAN;
    }
    else
    {
        if ((mode == ADC_MODE_CONT_ONE_CH) || (mode == ADC_MODE_CONT_MULTI_CH))
        {
            S12AD.ADCSR.BIT.ADCS = ADC_ADCS_CONT_SCAN;
        }
        // other modes have ADCS=0
    }

    if ((mode == ADC_MODE_SS_ONE_CH_DBLTRIG)
     || (mode == ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A))
    {
        S12AD.ADCSR.BIT.DBLE = 1;                   // enable double trigger
    }


    /* SET TRIGGER AND INTERRUPT PRIORITY REGISTER FIELDS */

    if (mode == ADC_MODE_SS_TEMPERATURE)
    {
        S12AD.ADSTRGR.BIT.TRSA = 0x0A;  // synchronous temperature trigger
    }
    if (p_cfg->trigger != ADC_TRIG_SOFTWARE)
    {
        S12AD.ADSTRGR.BIT.TRSA = p_cfg->trigger;
    }
    if (p_cfg->trigger == ADC_TRIG_ASYNC_ADTRG0)
    {
        S12AD.ADCSR.BIT.EXTRG = 1;      // set ext trigger for async trigger
    }
    if (S12AD.ADCSR.BIT.ADCS == ADC_ADCS_GROUP_SCAN)
    {
        S12AD.ADSTRGR.BIT.TRSB = p_cfg->trigger_groupb;
        IPR(S12AD,GBADI) = p_cfg->priority_groupb;
    }
    IPR(S12AD,S12ADI0) = p_cfg->priority;


    /* SET REGISTER FIELDS FOR REMAINING PARAMETERS */

    S12AD.ADADC.BIT.ADC = p_cfg->add_cnt;
    S12AD.ADCER.WORD = (uint16_t) (p_cfg->alignment | p_cfg->clearing);

    /* SAVE CALLBACK FUNCTION POINTER */
    g_dcb.callback = p_callback;


    /* MARK DRIVER AS OPENED */
    g_dcb.opened = true;
    R_BSP_HardwareUnlock(BSP_LOCK_S12AD);

    return ADC_SUCCESS;
}


/******************************************************************************
* Function Name: adc_control
* Description  : This function provides commands for enabling channels and
*                sensors and for runtime operations. These include enabling/
*                disabling trigger sources and interrupts, initiating a
*                software trigger, and checking for scan completion.
*
* NOTE: Enabling a channel or a sensor, or setting the sample state count reg
*       cannot be done while the ADSCR.ADST bit is set (conversion in progress).
*       Because these commands should only be called once during initialization
*       before triggers are enabled, this should not be an issue. Registers
*       with this restriction include ADANSA, ADANSB, ADADS, ADADC, ADSSTR,
*       ADEXICR, and some bits in ADSCR and TSCR.
*       No runtime operational sequence checking of any kind is performed.
*
* Arguments    : cmd-
*                    Command to run
*                p_args-
*                    Pointer to optional configuration structure
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_MISSING_PTR-
*                    p_args pointer is FIT_NO_PTR/NULL when required as an argument
*                ADC_ERR_INVALID_ARG-
*                    cmd or element of p_args structure has invalid value.
*                ADC_ERR_ILLEGAL_CMD-
*                    cmd is illegal based upon mode
*                ADC_ERR_SCAN_NOT_DONE-
*                    The requested scan has not completed
*                ADC_ERR_UNKNOWN
*                    Did not receive expected hardware response
*******************************************************************************/
adc_err_t adc_control(adc_cmd_t const     cmd,
                      void * const        p_args)
{
adc_err_t       err=ADC_SUCCESS;
adc_time_t      *p_sample;
adc_dda_t   *charge;


    switch (cmd)
    {
    case ADC_CMD_SET_DDA_STATE_CNT:
        charge = (adc_dda_t *) p_args;
#if (ADC_CFG_PARAM_CHECKING_ENABLE == 1)
        if ((g_dcb.mode == ADC_MODE_SS_TEMPERATURE)
         || (g_dcb.mode == ADC_MODE_SS_INT_REF_VOLT))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
        if ((charge->method >= ADC_DDA_MAX)
         || ((charge->method != ADC_DDA_OFF) && ((charge->num_states == 0) || (charge->num_states > ADC_DDA_STATE_CNT_MAX))))
        {
            return ADC_ERR_INVALID_ARG;
        }
#endif
        if (charge->method == ADC_DDA_OFF)
        {
            S12AD.ADDISCR.BYTE = 0;
        }
        else
        {
            // NOTE: Using Disconnect Detection Assist adds num_states x (#chans) ADCLKS to scan time
            S12AD.ADDISCR.BYTE = (uint8_t)((charge->method == ADC_DDA_PRECHARGE) ? 0x10 : 0);
            S12AD.ADDISCR.BYTE |= charge->num_states;
        }
    break;


    case ADC_CMD_SET_SAMPLE_STATE_CNT:
        p_sample = (adc_time_t *) p_args;
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
        if ((p_sample->reg_id >= ADC_SST_REG_MAX)
         || (p_sample->num_states < ADC_SST_CNT_MIN))
        {
            return ADC_ERR_INVALID_ARG;
        }
#endif
        *sreg_ptrs[p_sample->reg_id] = p_sample->num_states;
    break;


    case ADC_CMD_ENABLE_CHANS:
        err = adc_enable_chans((adc_ch_cfg_t *) p_args);
    break;


    case ADC_CMD_ENABLE_TEMP_SENSOR:
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
        if (g_dcb.mode != ADC_MODE_SS_TEMPERATURE)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
#endif
        // WARNING! Cannot use 50MHz PCLKD here. Must be < 3.6Mhz. Want sampling time around 70us
        // (see HW Manual section 35.3.2 "Setting of 12-Bit A/D Converter")
        S12AD.ADEXICR.BIT.TSS = 1;                  // select temperature sensor

        S12AD.ADCSR.BIT.TRGE = 1;                   // enable trigger
        TEMPS.TSCR.BIT.PGAGAIN = ADC_CFG_PGA_GAIN;  // set gain
        TEMPS.TSCR.BIT.TSEN = 1;                    // enable temp sensor
                                                    // no temperature addition on RX210
        // NOTE: Documentation is incorrect! 0 is to start and 1 is to stop
        // Amplifier must be started for sensor to connect with ADC
        TEMPS.TSCR.BIT.PGAEN = 0;

        adc_enable_s12adi0();                       // setup interrupt handling

    break;


    case ADC_CMD_ENABLE_VOLT_SENSOR:
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
        if (g_dcb.mode != ADC_MODE_SS_INT_REF_VOLT)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
#endif
        S12AD.ADEXICR.BIT.OCS = 1;                  // select ref voltage sensor
        if (S12AD.ADADC.BIT.ADC != ADC_ADD_OFF)
        {
            S12AD.ADEXICR.BIT.OCSAD = 1;            // enable addition
        }
        adc_enable_s12adi0();                       // setup interrupt handling
    break;


    case ADC_CMD_ENABLE_TRIG:
        S12AD.ADCSR.BIT.TRGE = 1;           // enable sync/async triggers
    break;


    case ADC_CMD_DISABLE_TRIG:
        S12AD.ADCSR.BIT.TRGE = 0;           // disable sync/async triggers
    break;


    case ADC_CMD_SCAN_NOW:
        /* For all parts, set the ADST bit to start a software scan EXCEPT
         * for the temperature sensor on the RX210. There use PGAEN to trigger
         * a conversion.
         */
        if (g_dcb.mode == ADC_MODE_SS_TEMPERATURE)
        {
            if (TEMPS.TSCR.BIT.PGAEN == 0)
            {
                TEMPS.TSCR.BIT.PGAEN = 1;
            }
            else
            {
                err = ADC_ERR_SCAN_NOT_DONE;
            }
        }
        else
        {
            if (S12AD.ADCSR.BIT.ADST == 0)
            {
                S12AD.ADCSR.BIT.ADST = 1;
            }
            else
            {
                err = ADC_ERR_SCAN_NOT_DONE;
            }
        }
    break;


    case ADC_CMD_CHECK_SCAN_DONE:           // default/Group A or Group B
        if (g_dcb.mode == ADC_MODE_SS_TEMPERATURE)
        {
           if (TEMPS.TSCR.BIT.PGAEN == 1)
           {
               err = ADC_ERR_SCAN_NOT_DONE;
           }
        }
        else
        {
            if (S12AD.ADCSR.BIT.ADST == 1)
            {
                err = ADC_ERR_SCAN_NOT_DONE;
            }
        }
    break;


    case ADC_CMD_CHECK_SCAN_DONE_GROUPA:
        if (ICU.IR[IR_S12AD_S12ADI0].BIT.IR == 1)
        {
            ICU.IR[IR_S12AD_S12ADI0].BIT.IR = 0;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
    break;


    case ADC_CMD_CHECK_SCAN_DONE_GROUPB:
        if (ICU.IR[IR_S12AD_GBADI].BIT.IR == 1)
        {
            ICU.IR[IR_S12AD_GBADI].BIT.IR = 0;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
    break;


    case ADC_CMD_ENABLE_INT:
        S12AD.ADCSR.BIT.ADIE = 1;           // enable S12ADI0 interrupt
    break;


    case ADC_CMD_DISABLE_INT:
        S12AD.ADCSR.BIT.ADIE = 0;           // disable S12ADI0 interrupt
    break;


    case ADC_CMD_ENABLE_INT_GROUPB:
        S12AD.ADCSR.BIT.GBADIE = 1;         // enable GBADI interrupt
    break;


    case ADC_CMD_DISABLE_INT_GROUPB:
        S12AD.ADCSR.BIT.GBADIE = 0;         // disable GBADI interrupt
    break;


    default:
        err = ADC_ERR_INVALID_ARG;
    break;
    }

    return err;
}


/******************************************************************************
* Function Name: adc_enable_chans
* Description  : This function does extensive checking on channel mask
*                settings based upon operational mode. Mask registers are
*                initialized and interrupts enabled in peripheral. Interrupts
*                are also enabled in ICU if corresponding priority is not 0.
*
* NOTE: A negative number is stored in two's complement form.
*       A quick way to change a binary number into two's complement is to
*       start at the right (LSB) and moving left, don't change any bits
*       until after the first "1" is reached.
*       Number          2's complement
*       0010 0110       1101 1010
*       0000 0001       1111 1111
*       Another way is to do a 1's complement on the number, then add 1 to that.
*       Number          1's complement  + 1
*       0010 0110       1101 1001       1101 1010
*       0000 0001       1111 1110       1111 1111
*
* Arguments    : p_config
*                    Pointer to channel config structure containing masks
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_INVALID_ARG-
*                    reg_id contains an invalid value.
*                ADC_ERR_ILLEGAL_ARG-
*                    an argument is illegal based upon mode
*******************************************************************************/
static adc_err_t adc_enable_chans(adc_ch_cfg_t *p_config)
{
uint16_t    i=0;
uint32_t    tmp_mask=0;
uint32_t    a_mask,b_mask;


#if ADC_CFG_PARAM_CHECKING_ENABLE == 1

    /* This command is illegal for sensor modes */
    if ((g_dcb.mode == ADC_MODE_SS_TEMPERATURE)
     || (g_dcb.mode == ADC_MODE_SS_INT_REF_VOLT))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    /* Verify at least one bonded channel is selected */
    if ((p_config->chan_mask == 0)
     || ((p_config->chan_mask & ADC_INVALID_CH_MASK) != 0))
    {
        return ADC_ERR_INVALID_ARG;
    }

    /* Verify at least one unique bonded channel is selected for Group B */
    if (S12AD.ADCSR.BIT.ADCS == ADC_ADCS_GROUP_SCAN)
    {
        if ((p_config->chan_mask_groupb == 0)
         || ((p_config->chan_mask_groupb & ADC_INVALID_CH_MASK) != 0))
        {
            return ADC_ERR_INVALID_ARG;
        }
        else if ((p_config->chan_mask & p_config->chan_mask_groupb) != 0)
        {
            return ADC_ERR_ILLEGAL_ARG;         // same chan in both groups
        }
        else
        {
            tmp_mask = p_config->chan_mask_groupb;
        }
    }
    else
    {
        p_config->chan_mask_groupb = 0;         // for addition mask checking
    }

    /* Addition mask should not include bits from inactive channels */
    if (S12AD.ADADC.BIT.ADC != ADC_ADD_OFF)
    {
        tmp_mask |= p_config->chan_mask;        // tmp_mask is Group A and B combined
        // Bit-AND with 1s-complement
        if ((p_config->add_mask & ~tmp_mask) != 0)
        {
            return ADC_ERR_INVALID_ARG;
        }
    }
    else
    {
        // WARNING! Other features messed up if add_mask is non-zero when addition is turned off!
        p_config->add_mask = 0;
    }

    /* Verify only 1 bit is set in default/Group A mask */
    if ((g_dcb.mode == ADC_MODE_SS_ONE_CH)
     || (g_dcb.mode == ADC_MODE_CONT_ONE_CH)
     || (S12AD.ADCSR.BIT.DBLE == 1))        // double trigger mode
    {
        tmp_mask = p_config->chan_mask;     // tmp_mask is non-Group/Group A chans
        // Bit-AND with 2s-complement (see note in function header)
        if ((tmp_mask & (~tmp_mask + 1)) != tmp_mask)
        {
            return ADC_ERR_INVALID_ARG;
        }
    }

    if (p_config->sample_hold_mask != 0)
    {
        /* S&H chans can only be 0,1,2 and must have at least minimum state count specified */
        if ((p_config->sample_hold_mask > 0x7)
         || (p_config->sample_hold_states < ADC_SST_SH_CNT_MIN))
        {
            return ADC_ERR_INVALID_ARG;
        }

        /* S&H channels must also be set in main channel mask or group B channel mask */
        a_mask = p_config->sample_hold_mask & p_config->chan_mask;
        b_mask = p_config->sample_hold_mask & p_config->chan_mask_groupb;
        if ((a_mask != p_config->sample_hold_mask)
         && ((S12AD.ADCSR.BIT.ADCS == ADC_ADCS_GROUP_SCAN) && (b_mask != p_config->sample_hold_mask)))
        {
            return ADC_ERR_INVALID_ARG;
        }

        /* S&H channels cannot be a double trigger channel (can be in group B) */
        if ((S12AD.ADCSR.BIT.DBLE == 1) && (a_mask != 0))
        {
            return ADC_ERR_INVALID_ARG;
        }
    }

    if (p_config->diag_method >= ADC_DIAG_MAX)
    {
        return ADC_ERR_INVALID_ARG;
    }
    /* NOTE: Kumagai says ok to have diag with double trigger (occurs on odd scan)
     * HW manual p1284 says illegal.
     */
#endif // parameter checking


    /* SET MASKS FOR ALL CHANNELS */

    S12AD.ADANSA.WORD = (uint16_t) (p_config->chan_mask & 0xFFFF);
    S12AD.ADANSB.WORD = (uint16_t) (p_config->chan_mask_groupb & 0xFFFF);
    S12AD.ADADS.WORD = (uint16_t) (p_config->add_mask & 0xFFFF);


    // NOTE: S&H adds to scan time because normal state machine still runs.
    // adds 10 + sample_hold_states ADCLKS to scan time
    S12AD.ADSHCR.BIT.SHANS = p_config->sample_hold_mask;
    S12AD.ADSHCR.BIT.SSTSH = p_config->sample_hold_states;


    /* SET SELF DIAGNOSIS REGISTERS (VIRTUAL CHANNEL) */

    if (p_config->diag_method == ADC_DIAG_OFF)
    {
        S12AD.ADCER.BIT.DIAGM = 0;
    }
    else
    {
        // NOTE: Using Self Diagnosis adds 30+(ch0 SST) ADCLKS to scan time.
        // (ch0 can still be used with self diagnosis on)
        if (p_config->diag_method == ADC_DIAG_ROTATE_VOLTS)
        {
            S12AD.ADCER.BIT.DIAGLD = 0;
            S12AD.ADCER.BIT.DIAGVAL = 1;
        }
        else
        {
            S12AD.ADCER.BIT.DIAGLD = 1;
            S12AD.ADCER.BIT.DIAGVAL = (uint8_t)(p_config->diag_method & 0x3);   // 2-bit value;
        }
        S12AD.ADCER.BIT.DIAGM = 1;
    }


    /* SET DOUBLE TRIGGER CHANNEL */

    if (S12AD.ADCSR.BIT.DBLE == 1)
    {
        tmp_mask = p_config->chan_mask;     // tmp_mask is non-Group/Group A chans
        while (tmp_mask >>= 1)              // determine bit/ch number
        {
            i++;
        }
        S12AD.ADCSR.BIT.DBLANS = i;
    }


    /* ENABLE INTERRUPTS */

    adc_enable_s12adi0();
    if (S12AD.ADCSR.BIT.ADCS == ADC_ADCS_GROUP_SCAN)
    {
        IR(S12AD,GBADI) = 0;                // clear flag
        S12AD.ADCSR.BIT.GBADIE = 1;         // enable in peripheral
        if (ICU.IPR[IPR_S12AD_GBADI].BYTE != 0)
        {
            IEN(S12AD,GBADI) = 1;           // enable in ICU
        }
    }

    return ADC_SUCCESS;
}

#endif
