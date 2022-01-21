/*******************************************************************************
* File Name: Timer_Echo.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the Timer_Echo
*  component
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Timer_Echo.h"

uint8 Timer_Echo_initVar = 0u;


/*******************************************************************************
* Function Name: Timer_Echo_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default Timer_Echo configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (Timer_Echo__QUAD == Timer_Echo_CONFIG)
        Timer_Echo_CONTROL_REG = Timer_Echo_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Timer_Echo_TRIG_CONTROL1_REG  = Timer_Echo_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        Timer_Echo_SetInterruptMode(Timer_Echo_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        Timer_Echo_SetCounterMode(Timer_Echo_COUNT_DOWN);
        Timer_Echo_WritePeriod(Timer_Echo_QUAD_PERIOD_INIT_VALUE);
        Timer_Echo_WriteCounter(Timer_Echo_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (Timer_Echo__QUAD == Timer_Echo_CONFIG) */

    #if (Timer_Echo__TIMER == Timer_Echo_CONFIG)
        Timer_Echo_CONTROL_REG = Timer_Echo_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        Timer_Echo_TRIG_CONTROL1_REG  = Timer_Echo_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Timer_Echo_SetInterruptMode(Timer_Echo_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        Timer_Echo_WritePeriod(Timer_Echo_TC_PERIOD_VALUE );

        #if (Timer_Echo__COMPARE == Timer_Echo_TC_COMP_CAP_MODE)
            Timer_Echo_WriteCompare(Timer_Echo_TC_COMPARE_VALUE);

            #if (1u == Timer_Echo_TC_COMPARE_SWAP)
                Timer_Echo_SetCompareSwap(1u);
                Timer_Echo_WriteCompareBuf(Timer_Echo_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == Timer_Echo_TC_COMPARE_SWAP) */
        #endif  /* (Timer_Echo__COMPARE == Timer_Echo_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (Timer_Echo_CY_TCPWM_V2 && Timer_Echo_TIMER_UPDOWN_CNT_USED && !Timer_Echo_CY_TCPWM_4000)
            Timer_Echo_WriteCounter(1u);
        #elif(Timer_Echo__COUNT_DOWN == Timer_Echo_TC_COUNTER_MODE)
            Timer_Echo_WriteCounter(Timer_Echo_TC_PERIOD_VALUE);
        #else
            Timer_Echo_WriteCounter(0u);
        #endif /* (Timer_Echo_CY_TCPWM_V2 && Timer_Echo_TIMER_UPDOWN_CNT_USED && !Timer_Echo_CY_TCPWM_4000) */
    #endif  /* (Timer_Echo__TIMER == Timer_Echo_CONFIG) */

    #if (Timer_Echo__PWM_SEL == Timer_Echo_CONFIG)
        Timer_Echo_CONTROL_REG = Timer_Echo_CTRL_PWM_BASE_CONFIG;

        #if (Timer_Echo__PWM_PR == Timer_Echo_PWM_MODE)
            Timer_Echo_CONTROL_REG |= Timer_Echo_CTRL_PWM_RUN_MODE;
            Timer_Echo_WriteCounter(Timer_Echo_PWM_PR_INIT_VALUE);
        #else
            Timer_Echo_CONTROL_REG |= Timer_Echo_CTRL_PWM_ALIGN | Timer_Echo_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (Timer_Echo_CY_TCPWM_V2 && Timer_Echo_PWM_UPDOWN_CNT_USED && !Timer_Echo_CY_TCPWM_4000)
                Timer_Echo_WriteCounter(1u);
            #elif (Timer_Echo__RIGHT == Timer_Echo_PWM_ALIGN)
                Timer_Echo_WriteCounter(Timer_Echo_PWM_PERIOD_VALUE);
            #else 
                Timer_Echo_WriteCounter(0u);
            #endif  /* (Timer_Echo_CY_TCPWM_V2 && Timer_Echo_PWM_UPDOWN_CNT_USED && !Timer_Echo_CY_TCPWM_4000) */
        #endif  /* (Timer_Echo__PWM_PR == Timer_Echo_PWM_MODE) */

        #if (Timer_Echo__PWM_DT == Timer_Echo_PWM_MODE)
            Timer_Echo_CONTROL_REG |= Timer_Echo_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (Timer_Echo__PWM_DT == Timer_Echo_PWM_MODE) */

        #if (Timer_Echo__PWM == Timer_Echo_PWM_MODE)
            Timer_Echo_CONTROL_REG |= Timer_Echo_CTRL_PWM_PRESCALER;
        #endif  /* (Timer_Echo__PWM == Timer_Echo_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        Timer_Echo_TRIG_CONTROL1_REG  = Timer_Echo_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        Timer_Echo_SetInterruptMode(Timer_Echo_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (Timer_Echo__PWM_PR == Timer_Echo_PWM_MODE)
            Timer_Echo_TRIG_CONTROL2_REG =
                    (Timer_Echo_CC_MATCH_NO_CHANGE    |
                    Timer_Echo_OVERLOW_NO_CHANGE      |
                    Timer_Echo_UNDERFLOW_NO_CHANGE);
        #else
            #if (Timer_Echo__LEFT == Timer_Echo_PWM_ALIGN)
                Timer_Echo_TRIG_CONTROL2_REG = Timer_Echo_PWM_MODE_LEFT;
            #endif  /* ( Timer_Echo_PWM_LEFT == Timer_Echo_PWM_ALIGN) */

            #if (Timer_Echo__RIGHT == Timer_Echo_PWM_ALIGN)
                Timer_Echo_TRIG_CONTROL2_REG = Timer_Echo_PWM_MODE_RIGHT;
            #endif  /* ( Timer_Echo_PWM_RIGHT == Timer_Echo_PWM_ALIGN) */

            #if (Timer_Echo__CENTER == Timer_Echo_PWM_ALIGN)
                Timer_Echo_TRIG_CONTROL2_REG = Timer_Echo_PWM_MODE_CENTER;
            #endif  /* ( Timer_Echo_PWM_CENTER == Timer_Echo_PWM_ALIGN) */

            #if (Timer_Echo__ASYMMETRIC == Timer_Echo_PWM_ALIGN)
                Timer_Echo_TRIG_CONTROL2_REG = Timer_Echo_PWM_MODE_ASYM;
            #endif  /* (Timer_Echo__ASYMMETRIC == Timer_Echo_PWM_ALIGN) */
        #endif  /* (Timer_Echo__PWM_PR == Timer_Echo_PWM_MODE) */

        /* Set other values from customizer */
        Timer_Echo_WritePeriod(Timer_Echo_PWM_PERIOD_VALUE );
        Timer_Echo_WriteCompare(Timer_Echo_PWM_COMPARE_VALUE);

        #if (1u == Timer_Echo_PWM_COMPARE_SWAP)
            Timer_Echo_SetCompareSwap(1u);
            Timer_Echo_WriteCompareBuf(Timer_Echo_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == Timer_Echo_PWM_COMPARE_SWAP) */

        #if (1u == Timer_Echo_PWM_PERIOD_SWAP)
            Timer_Echo_SetPeriodSwap(1u);
            Timer_Echo_WritePeriodBuf(Timer_Echo_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == Timer_Echo_PWM_PERIOD_SWAP) */
    #endif  /* (Timer_Echo__PWM_SEL == Timer_Echo_CONFIG) */
    
}


/*******************************************************************************
* Function Name: Timer_Echo_Enable
********************************************************************************
*
* Summary:
*  Enables the Timer_Echo.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    Timer_Echo_BLOCK_CONTROL_REG |= Timer_Echo_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (Timer_Echo__PWM_SEL == Timer_Echo_CONFIG)
        #if (0u == Timer_Echo_PWM_START_SIGNAL_PRESENT)
            Timer_Echo_TriggerCommand(Timer_Echo_MASK, Timer_Echo_CMD_START);
        #endif /* (0u == Timer_Echo_PWM_START_SIGNAL_PRESENT) */
    #endif /* (Timer_Echo__PWM_SEL == Timer_Echo_CONFIG) */

    #if (Timer_Echo__TIMER == Timer_Echo_CONFIG)
        #if (0u == Timer_Echo_TC_START_SIGNAL_PRESENT)
            Timer_Echo_TriggerCommand(Timer_Echo_MASK, Timer_Echo_CMD_START);
        #endif /* (0u == Timer_Echo_TC_START_SIGNAL_PRESENT) */
    #endif /* (Timer_Echo__TIMER == Timer_Echo_CONFIG) */
    
    #if (Timer_Echo__QUAD == Timer_Echo_CONFIG)
        #if (0u != Timer_Echo_QUAD_AUTO_START)
            Timer_Echo_TriggerCommand(Timer_Echo_MASK, Timer_Echo_CMD_RELOAD);
        #endif /* (0u != Timer_Echo_QUAD_AUTO_START) */
    #endif  /* (Timer_Echo__QUAD == Timer_Echo_CONFIG) */
}


/*******************************************************************************
* Function Name: Timer_Echo_Start
********************************************************************************
*
* Summary:
*  Initializes the Timer_Echo with default customizer
*  values when called the first time and enables the Timer_Echo.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Timer_Echo_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time Timer_Echo_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the Timer_Echo_Start() routine.
*
*******************************************************************************/
void Timer_Echo_Start(void)
{
    if (0u == Timer_Echo_initVar)
    {
        Timer_Echo_Init();
        Timer_Echo_initVar = 1u;
    }

    Timer_Echo_Enable();
}


/*******************************************************************************
* Function Name: Timer_Echo_Stop
********************************************************************************
*
* Summary:
*  Disables the Timer_Echo.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_BLOCK_CONTROL_REG &= (uint32)~Timer_Echo_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the Timer_Echo. This function is used when
*  configured as a generic Timer_Echo and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the Timer_Echo to operate in
*   Values:
*   - Timer_Echo_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - Timer_Echo_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - Timer_Echo_MODE_QUAD - Quadrature decoder
*         - Timer_Echo_MODE_PWM - PWM
*         - Timer_Echo_MODE_PWM_DT - PWM with dead time
*         - Timer_Echo_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_MODE_MASK;
    Timer_Echo_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - Timer_Echo_MODE_X1 - Counts on phi 1 rising
*         - Timer_Echo_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - Timer_Echo_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_QUAD_MODE_MASK;
    Timer_Echo_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - Timer_Echo_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - Timer_Echo_PRESCALE_DIVBY2    - Divide by 2
*         - Timer_Echo_PRESCALE_DIVBY4    - Divide by 4
*         - Timer_Echo_PRESCALE_DIVBY8    - Divide by 8
*         - Timer_Echo_PRESCALE_DIVBY16   - Divide by 16
*         - Timer_Echo_PRESCALE_DIVBY32   - Divide by 32
*         - Timer_Echo_PRESCALE_DIVBY64   - Divide by 64
*         - Timer_Echo_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_PRESCALER_MASK;
    Timer_Echo_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the Timer_Echo runs
*  continuously or stops when terminal count is reached.  By default the
*  Timer_Echo operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_ONESHOT_MASK;
    Timer_Echo_CONTROL_REG |= ((uint32)((oneShotEnable & Timer_Echo_1BIT_MASK) <<
                                                               Timer_Echo_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetPWMMode(uint32 modeMask)
{
    Timer_Echo_TRIG_CONTROL2_REG = (modeMask & Timer_Echo_6BIT_MASK);
}



/*******************************************************************************
* Function Name: Timer_Echo_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_PWM_SYNC_KILL_MASK;
    Timer_Echo_CONTROL_REG |= ((uint32)((syncKillEnable & Timer_Echo_1BIT_MASK)  <<
                                               Timer_Echo_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_PWM_STOP_KILL_MASK;
    Timer_Echo_CONTROL_REG |= ((uint32)((stopOnKillEnable & Timer_Echo_1BIT_MASK)  <<
                                                         Timer_Echo_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_PRESCALER_MASK;
    Timer_Echo_CONTROL_REG |= ((uint32)((deadTime & Timer_Echo_8BIT_MASK) <<
                                                          Timer_Echo_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - Timer_Echo_INVERT_LINE   - Inverts the line output
*         - Timer_Echo_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_INV_OUT_MASK;
    Timer_Echo_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: Timer_Echo_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_WriteCounter(uint32 count)
{
    Timer_Echo_COUNTER_REG = (count & Timer_Echo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Echo_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 Timer_Echo_ReadCounter(void)
{
    return (Timer_Echo_COUNTER_REG & Timer_Echo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - Timer_Echo_COUNT_UP       - Counts up
*     - Timer_Echo_COUNT_DOWN     - Counts down
*     - Timer_Echo_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - Timer_Echo_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_UPDOWN_MASK;
    Timer_Echo_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_WritePeriod(uint32 period)
{
    Timer_Echo_PERIOD_REG = (period & Timer_Echo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Echo_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 Timer_Echo_ReadPeriod(void)
{
    return (Timer_Echo_PERIOD_REG & Timer_Echo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_RELOAD_CC_MASK;
    Timer_Echo_CONTROL_REG |= (swapEnable & Timer_Echo_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_WritePeriodBuf(uint32 periodBuf)
{
    Timer_Echo_PERIOD_BUF_REG = (periodBuf & Timer_Echo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Echo_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 Timer_Echo_ReadPeriodBuf(void)
{
    return (Timer_Echo_PERIOD_BUF_REG & Timer_Echo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_CONTROL_REG &= (uint32)~Timer_Echo_RELOAD_PERIOD_MASK;
    Timer_Echo_CONTROL_REG |= ((uint32)((swapEnable & Timer_Echo_1BIT_MASK) <<
                                                            Timer_Echo_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void Timer_Echo_WriteCompare(uint32 compare)
{
    #if (Timer_Echo_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Timer_Echo_CY_TCPWM_4000) */

    #if (Timer_Echo_CY_TCPWM_4000)
        currentMode = ((Timer_Echo_CONTROL_REG & Timer_Echo_UPDOWN_MASK) >> Timer_Echo_UPDOWN_SHIFT);

        if (((uint32)Timer_Echo__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)Timer_Echo__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (Timer_Echo_CY_TCPWM_4000) */
    
    Timer_Echo_COMP_CAP_REG = (compare & Timer_Echo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Echo_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 Timer_Echo_ReadCompare(void)
{
    #if (Timer_Echo_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Timer_Echo_CY_TCPWM_4000) */

    #if (Timer_Echo_CY_TCPWM_4000)
        currentMode = ((Timer_Echo_CONTROL_REG & Timer_Echo_UPDOWN_MASK) >> Timer_Echo_UPDOWN_SHIFT);
        
        regVal = Timer_Echo_COMP_CAP_REG;
        
        if (((uint32)Timer_Echo__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Timer_Echo__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Timer_Echo_16BIT_MASK);
    #else
        return (Timer_Echo_COMP_CAP_REG & Timer_Echo_16BIT_MASK);
    #endif /* (Timer_Echo_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Timer_Echo_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void Timer_Echo_WriteCompareBuf(uint32 compareBuf)
{
    #if (Timer_Echo_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (Timer_Echo_CY_TCPWM_4000) */

    #if (Timer_Echo_CY_TCPWM_4000)
        currentMode = ((Timer_Echo_CONTROL_REG & Timer_Echo_UPDOWN_MASK) >> Timer_Echo_UPDOWN_SHIFT);

        if (((uint32)Timer_Echo__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)Timer_Echo__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (Timer_Echo_CY_TCPWM_4000) */
    
    Timer_Echo_COMP_CAP_BUF_REG = (compareBuf & Timer_Echo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Echo_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 Timer_Echo_ReadCompareBuf(void)
{
    #if (Timer_Echo_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (Timer_Echo_CY_TCPWM_4000) */

    #if (Timer_Echo_CY_TCPWM_4000)
        currentMode = ((Timer_Echo_CONTROL_REG & Timer_Echo_UPDOWN_MASK) >> Timer_Echo_UPDOWN_SHIFT);

        regVal = Timer_Echo_COMP_CAP_BUF_REG;
        
        if (((uint32)Timer_Echo__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)Timer_Echo__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & Timer_Echo_16BIT_MASK);
    #else
        return (Timer_Echo_COMP_CAP_BUF_REG & Timer_Echo_16BIT_MASK);
    #endif /* (Timer_Echo_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: Timer_Echo_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 Timer_Echo_ReadCapture(void)
{
    return (Timer_Echo_COMP_CAP_REG & Timer_Echo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Echo_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 Timer_Echo_ReadCaptureBuf(void)
{
    return (Timer_Echo_COMP_CAP_BUF_REG & Timer_Echo_16BIT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Timer_Echo_TRIG_LEVEL     - Level
*     - Timer_Echo_TRIG_RISING    - Rising edge
*     - Timer_Echo_TRIG_FALLING   - Falling edge
*     - Timer_Echo_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_TRIG_CONTROL1_REG &= (uint32)~Timer_Echo_CAPTURE_MASK;
    Timer_Echo_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Timer_Echo_TRIG_LEVEL     - Level
*     - Timer_Echo_TRIG_RISING    - Rising edge
*     - Timer_Echo_TRIG_FALLING   - Falling edge
*     - Timer_Echo_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_TRIG_CONTROL1_REG &= (uint32)~Timer_Echo_RELOAD_MASK;
    Timer_Echo_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Timer_Echo_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Timer_Echo_TRIG_LEVEL     - Level
*     - Timer_Echo_TRIG_RISING    - Rising edge
*     - Timer_Echo_TRIG_FALLING   - Falling edge
*     - Timer_Echo_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_TRIG_CONTROL1_REG &= (uint32)~Timer_Echo_START_MASK;
    Timer_Echo_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Timer_Echo_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Timer_Echo_TRIG_LEVEL     - Level
*     - Timer_Echo_TRIG_RISING    - Rising edge
*     - Timer_Echo_TRIG_FALLING   - Falling edge
*     - Timer_Echo_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_TRIG_CONTROL1_REG &= (uint32)~Timer_Echo_STOP_MASK;
    Timer_Echo_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Timer_Echo_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - Timer_Echo_TRIG_LEVEL     - Level
*     - Timer_Echo_TRIG_RISING    - Rising edge
*     - Timer_Echo_TRIG_FALLING   - Falling edge
*     - Timer_Echo_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_TRIG_CONTROL1_REG &= (uint32)~Timer_Echo_COUNT_MASK;
    Timer_Echo_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << Timer_Echo_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - Timer_Echo_CMD_CAPTURE    - Trigger Capture/Switch command
*     - Timer_Echo_CMD_RELOAD     - Trigger Reload/Index command
*     - Timer_Echo_CMD_STOP       - Trigger Stop/Kill command
*     - Timer_Echo_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    Timer_Echo_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: Timer_Echo_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the Timer_Echo.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - Timer_Echo_STATUS_DOWN    - Set if counting down
*     - Timer_Echo_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 Timer_Echo_ReadStatus(void)
{
    return ((Timer_Echo_STATUS_REG >> Timer_Echo_RUNNING_STATUS_SHIFT) |
            (Timer_Echo_STATUS_REG & Timer_Echo_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: Timer_Echo_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - Timer_Echo_INTR_MASK_TC       - Terminal count mask
*     - Timer_Echo_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetInterruptMode(uint32 interruptMask)
{
    Timer_Echo_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: Timer_Echo_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - Timer_Echo_INTR_MASK_TC       - Terminal count mask
*     - Timer_Echo_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Timer_Echo_GetInterruptSourceMasked(void)
{
    return (Timer_Echo_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: Timer_Echo_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - Timer_Echo_INTR_MASK_TC       - Terminal count mask
*     - Timer_Echo_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 Timer_Echo_GetInterruptSource(void)
{
    return (Timer_Echo_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: Timer_Echo_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - Timer_Echo_INTR_MASK_TC       - Terminal count mask
*     - Timer_Echo_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_ClearInterrupt(uint32 interruptMask)
{
    Timer_Echo_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: Timer_Echo_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - Timer_Echo_INTR_MASK_TC       - Terminal count mask
*     - Timer_Echo_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void Timer_Echo_SetInterrupt(uint32 interruptMask)
{
    Timer_Echo_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
