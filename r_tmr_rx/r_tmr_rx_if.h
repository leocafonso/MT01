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
* Copyright (C) 2013, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name	   : r_cmt_rx_if.h
* Description  : This module creates a timer tick using a CMT channel.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description               
*         : 06.11.2013 2.10    First GSCE Release.
*         : 22.04.2014 2.30    Updates for RX110, RX64M support.
*         : 10.11.2014 2.40    Added support for RX113.
*         : 12.04.2014 2.41    Updated demo project.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API. */
#define CMT_RX_VERSION_MAJOR            (1)
#define CMT_RX_VERSION_MINOR            (00)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum e_tmr_ch       // TMR channel numbers
{
    TMR_CH0=0,
	TMR_CH1,
	TMR_CH2,
	TMR_CH3,
    SCI_NUM_CH
} tmr_ch_t;

typedef enum e_sci_comm       // TMR commands
{
    TMR_START=0,
    SCI_NUM_COMM
} tmr_commands_t;


/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
bool R_TMR_CreatePeriodic(uint32_t frequency_hz, void (* callback)(void * pdata), tmr_ch_t  channel);
bool R_TMR_CreateOneShot(uint8_t period_us, void (* callback)(void * pdata), tmr_ch_t  channel);
bool R_TMR_Control(tmr_ch_t channel, tmr_commands_t command, void * pdata);
bool R_TMR_Stop(tmr_ch_t channel);
uint32_t  R_TMR_GetVersion(void);
