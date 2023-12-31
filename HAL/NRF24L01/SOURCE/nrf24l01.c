/*
=======================================================================================================================
Author       : Mamoun
Module       : NRF24L01
File Name    : nrf24l01.c
Date Created : Nov 22, 2023
Description  : Source file for the NRF24L01 module driver.
=======================================================================================================================
*/


/*=====================================================================================================================
                                               < Includes >
=====================================================================================================================*/

#include "../../../OTHERS/std_types.h"
#include "../../../OTHERS/common_macros.h"
#include "../../../MCAL/GPIO/INCLUDES/gpio.h"
//#include "../../../MCAL/SPI/INCLUDES/spi.h"
#include "../INCLUDES/nrf24l01_prv.h"
#include "../INCLUDES/nrf24l01_cfg.h"
#include "../INCLUDES/nrf24l01.h"

/*=====================================================================================================================
                                           < Global Variables >
=====================================================================================================================*/

/* Array holding the 5-byte address of the transmitter [configured in "nrf24l01_cfg.h" file]. */
uint8 G_transmitterAddress[NRF24L01_TX_RX_ADDRESS_SIZE] = {NRF24L01_TRANSMITTER_ADDRESS};

/* 5 arrays holding the 5-byte addresses of the 5 data pipes [configured in "nrf24l01_cfg.h" file]. */
uint8 G_receiverPipe0Address[NRF24L01_TX_RX_ADDRESS_SIZE] = {NRF24L01_RECEIVER_PIPE0_ADDRESS};  /* Data pipe 0. */
uint8 G_receiverPipe1Address[NRF24L01_TX_RX_ADDRESS_SIZE] = {NRF24L01_RECEIVER_PIPE1_ADDRESS};  /* Data pipe 1. */
uint8 G_receiverPipe2Address[NRF24L01_TX_RX_ADDRESS_SIZE] = {NRF24L01_RECEIVER_PIPE2_ADDRESS};  /* Data pipe 2. */
uint8 G_receiverPipe3Address[NRF24L01_TX_RX_ADDRESS_SIZE] = {NRF24L01_RECEIVER_PIPE3_ADDRESS};  /* Data pipe 3. */
uint8 G_receiverPipe4Address[NRF24L01_TX_RX_ADDRESS_SIZE] = {NRF24L01_RECEIVER_PIPE4_ADDRESS};  /* Data pipe 4. */
uint8 G_receiverPipe5Address[NRF24L01_TX_RX_ADDRESS_SIZE] = {NRF24L01_RECEIVER_PIPE5_ADDRESS};  /* Data pipe 5. */

/*=====================================================================================================================
                                          < Functions Definitions >
=====================================================================================================================*/

/*=====================================================================================================================
 * [Function Name] : NRF24L01_init
 * [Description]   : Initialize the NRF24L01 module.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void NRF24L01_init(void)
{
    /* Disable the nRF chip while initialization. */
    GPIO_writePin(NRF24L01_SPI_CE_PORT_ID,NRF24L01_SPI_CE_PIN_ID,GPIO_LOW_PIN);

    NRF24L01_reset();                                                   /* Reset all the nRF chip registers. */

    NRF24L01_writeRegister(NRF24L01_CONFIG_REG,0X00);                                        /* Disable CRC. */
    NRF24L01_writeRegister(NRF24L01_EN_AA_REG,0X00);      /* Diable auto acknowledgement for all data pipes. */
    NRF24L01_writeRegister(NRF24L01_EN_RXADDR_REG,0X00);                           /* Diable all data pipes. */
    NRF24L01_writeRegister(NRF24L01_SETUP_RETR_REG,0X00);           /* Disable the automatic retransmission. */
    NRF24L01_writeRegister(NRF24L01_RF_CH_REG,0X00);                         /* Reset the frequency channel. */

    /* Activate the nRF chip after completing the initialization. */
    GPIO_writePin(NRF24L01_SPI_CE_PORT_ID,NRF24L01_SPI_CE_PIN_ID,GPIO_HIGH_PIN);
}

/*=====================================================================================================================
 * [Function Name] : NRF24L01_reset
 * [Description]   : Reset the NRF24L01 module by resetting its registers.
 * [Arguments]     : The function takes no arguments.
 * [return]        : The function returns void.
 ====================================================================================================================*/
void NRF24L01_reset(void)
{
    /* Reset all the nRF chip registers. */
    NRF24L01_writeRegister(NRF24L01_CONFIG_REG,0X08);
    NRF24L01_writeRegister(NRF24L01_EN_AA_REG,0X3F);
    NRF24L01_writeRegister(NRF24L01_EN_RXADDR_REG,0X03);
    NRF24L01_writeRegister(NRF24L01_SETUP_AW_REG,0X03);
    NRF24L01_writeRegister(NRF24L01_SETUP_RETR_REG,0X03);
    NRF24L01_writeRegister(NRF24L01_RF_CH_REG,0X02);
    NRF24L01_writeRegister(NRF24L01_RF_SETUP_REG,0X0E);
    NRF24L01_writeRegister(NRF24L01_STATUS_REG,0X0E);
    NRF24L01_writeRegister(NRF24L01_OBSERVE_TX_REG,0X00);
    NRF24L01_writeRegister(NRF24L01_RPD_REG,0X00);
    NRF24L01_writeRegister(NRF24L01_RX_PW_P0_REG,0X00);
    NRF24L01_writeRegister(NRF24L01_RX_PW_P1_REG,0X00);
    NRF24L01_writeRegister(NRF24L01_RX_PW_P2_REG,0X00);
    NRF24L01_writeRegister(NRF24L01_RX_PW_P3_REG,0X00);
    NRF24L01_writeRegister(NRF24L01_RX_PW_P4_REG,0X00);
    NRF24L01_writeRegister(NRF24L01_RX_PW_P5_REG,0X00);
    NRF24L01_writeRegister(NRF24L01_FIFO_STATUS_REG,0X11);
    NRF24L01_writeRegister(NRF24L01_DYNPD_REG,0X00);
    NRF24L01_writeRegister(NRF24L01_FEATURE_REG,0X00);
}

/*=====================================================================================================================
 * [Function Name] : NRF24L01_sendCommand
 * [Description]   : Send a specific command to the NRF24L01 module.
 * [Arguments]     : <a_command>      -> Indicates to the required command.
 * [return]        : The function returns void.
 ====================================================================================================================*/
static void NRF24L01_sendCommand(uint8 a_command)
{
    /* Select the nRF chip for SPI transmission. */
    GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_LOW_PIN);

    //SPI_sendData(a_command);                                       /* Send the required command through SPI. */

    /* Unselect the nRF chip after completing the transmission. */
    GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_HIGH_PIN);
}

/*=====================================================================================================================
 * [Function Name] : NRF24L01_readRegister
 * [Description]   : Read the value of a specific register in the NRF24L01 module.
 * [Arguments]     : <a_registerAddress>   -> Indicates to the address of the required register.
 *                   <a_ptr2value>         -> Pointer to variable to store the register value.
 * [return]        : The function returns void.
 ====================================================================================================================*/
static void NRF24L01_readRegister(uint8 a_registerAddress, uint8* a_ptr2value)
{
    a_registerAddress = ((a_registerAddress & 0X1F) | (NRF24L01_R_REGISTER_COMMAND));

    /* Select the nRF chip for SPI transmission. */
    GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_LOW_PIN);

    //SPI_sendData(a_registerAddress);                           /* Send the register address through SPI. */
    //SPI_receiveData(a_ptr2value);                                /* Read the register value through SPI. */

    /* Unselect the nRF chip after completing the transmission. */
    GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_HIGH_PIN);
}

/*=====================================================================================================================
 * [Function Name] : NRF24L01_writeRegister
 * [Description]   : Write a specific value in a specific register in the NRF24L01 module.
 * [Arguments]     : <a_registerAddress>   -> Indicates to the address of the required register.
 *                   <a_value>             -> Indicates to the required value to be written.
 * [return]        : The function returns void.
 ====================================================================================================================*/
static void NRF24L01_writeRegister(uint8 a_registerAddress, uint8 a_value)
{
    a_registerAddress = ((a_registerAddress & 0X1F) | (NRF24L01_W_REGISTER_COMMAND));

    /* Select the nRF chip for SPI transmission. */
    GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_LOW_PIN);

    //SPI_sendData(a_registerAddress);                               /* Send the register address through SPI. */
    //SPI_sendData(a_value);                                           /* Send the register value through SPI. */

    /* Unselect the nRF chip after completing the transmission. */
    GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_HIGH_PIN);
}

/*=====================================================================================================================
 * [Function Name] : NRF24L01_writeRegisterMulti
 * [Description]   : Write a specific value [multiple bytes] in a specific register in the NRF24L01 module.
 * [Arguments]     : <a_registerAddress>   -> Indicates to the address of the required register.
 *                   <a_ptr2value>         -> Pointer to the required value to be written.
 *                   <a_size>              -> Indicates to the size of the data.
 * [return]        : The function returns void.
 ====================================================================================================================*/
static void NRF24L01_writeRegisterMulti(uint8 a_registerAddress, uint8* a_ptr2value, uint8 a_size)
{
    a_registerAddress = ((a_registerAddress & 0X1F) | (NRF24L01_W_REGISTER_COMMAND));

    /* Select the nRF chip for SPI transmission. */
    GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_LOW_PIN);

    //SPI_sendData(a_registerAddress);                           /* Send the register address through SPI. */
    //SPI_sendDataMulti(a_ptr2value,a_size);                       /* Send the register value through SPI. */

    /* Unselect the nRF chip after completing the transmission. */
    GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_HIGH_PIN);
}

/*=====================================================================================================================
 * [Function Name] : NRF24L01_switchToTransmitterMode
 * [Description]   : Reconfigure the NRF24L01 module to work in transmitter mode.
 * [Arguments]     : <a_channel>      -> Indicates to the transmission channel number.
 * [return]        : The function returns error status: - No Errors.
 *                                                      - Channel Number Error.
 ====================================================================================================================*/
NRF24L01_errorStatusType NRF24L01_switchToTransmitterMode(uint8 a_channel)
{
    NRF24L01_errorStatusType LOC_errorStatus = NRF24L01_NO_ERRORS;
    uint8 LOC_registerValue = 0;

    if(a_channel > NRF24L01_MAX_CHANNEL_NUMBER)
    {
        LOC_errorStatus = NRF24L01_CHANNEL_NUMBER_ERROR;
    }

    else
    {
        /* Disable the nRF chip while configuration. */
        GPIO_writePin(NRF24L01_SPI_CE_PORT_ID,NRF24L01_SPI_CE_PIN_ID,GPIO_LOW_PIN);

        /* Set the 5-byte address of the transmitter. */
        NRF24L01_writeRegisterMulti(NRF24L01_TX_ADDR_REG,G_transmitterAddress,NRF24L01_TX_RX_ADDRESS_SIZE);

        /* Set the transmitter channel number. */
        NRF24L01_writeRegister(NRF24L01_RF_CH_REG,a_channel);

        /* Turn the module power up and select the TX mode [with preserving the other bits]. */
        NRF24L01_readRegister(NRF24L01_CONFIG_REG,&LOC_registerValue);
        LOC_registerValue = ((LOC_registerValue & 0XFC) | (0X02));
        NRF24L01_writeRegister(NRF24L01_CONFIG_REG,LOC_registerValue);

        /* Activate the nRF chip after completing the configuration. */
        GPIO_writePin(NRF24L01_SPI_CE_PORT_ID,NRF24L01_SPI_CE_PIN_ID,GPIO_HIGH_PIN);
    }

    return LOC_errorStatus;
}

/*=====================================================================================================================
 * [Function Name] : NRF24L01_switchToReceiverMode
 * [Description]   : Reconfigure the NRF24L01 module to work in receiver mode.
 * [Arguments]     : <a_channel>      -> Indicates to the transmission channel number.
 *                   <a_dataPipe>     -> Indicates to the required data pipe to be enabled.
 * [return]        : The function returns error status: - No Errors.
 *                                                      - Channel Number Error.
 *                                                      - Data Pipe Error.
 ====================================================================================================================*/
NRF24L01_errorStatusType NRF24L01_switchToReceiverMode(uint8 a_channel, NRF24L01_dataPipeType a_dataPipe)
{
    NRF24L01_errorStatusType LOC_errorStatus = NRF24L01_NO_ERRORS;
    uint8 LOC_registerValue = 0;

    if(a_channel > NRF24L01_MAX_CHANNEL_NUMBER)
    {
        LOC_errorStatus = NRF24L01_CHANNEL_NUMBER_ERROR;
    }

    else if(a_dataPipe > NRF24L01_DATA_PIPE_5)
    {
        LOC_errorStatus = NRF24L01_DATA_PIPE_ERROR;
    }

    else
    {
        /* Disable the nRF chip while configuration. */
        GPIO_writePin(NRF24L01_SPI_CE_PORT_ID,NRF24L01_SPI_CE_PIN_ID,GPIO_LOW_PIN);

        /* Set the transmitter channel number. */
        NRF24L01_writeRegister(NRF24L01_RF_CH_REG,a_channel);

        /* Select the required data pipe to be enabled. */
        NRF24L01_writeRegister(NRF24L01_EN_RXADDR_REG,(1 << a_dataPipe));

        /* Set the 5-byte address of the required data pipe, and set the pay load size. */
        switch (a_dataPipe)
        {
        case NRF24L01_DATA_PIPE_0:
            /* Set the 5-byte address of the data pipe 0. */
            NRF24L01_writeRegisterMulti(NRF24L01_RX_ADDR_P0_REG,G_receiverPipe0Address,NRF24L01_TX_RX_ADDRESS_SIZE);
            NRF24L01_writeRegister(NRF24L01_RX_PW_P0_REG,NRF24L01_PAYLOAD_SIZE);    /* Set the pay load size. */
            break;

        case NRF24L01_DATA_PIPE_1:
            /* Set the 5-byte address of the data pipe 1. */
            NRF24L01_writeRegisterMulti(NRF24L01_RX_ADDR_P1_REG,G_receiverPipe1Address,NRF24L01_TX_RX_ADDRESS_SIZE);
            NRF24L01_writeRegister(NRF24L01_RX_PW_P1_REG,NRF24L01_PAYLOAD_SIZE);    /* Set the pay load size. */
            break;
            
        case NRF24L01_DATA_PIPE_2:
            /* Set the 5-byte address of the data pipe 2. */
            NRF24L01_writeRegisterMulti(NRF24L01_RX_ADDR_P2_REG,G_receiverPipe2Address,NRF24L01_TX_RX_ADDRESS_SIZE);
            NRF24L01_writeRegister(NRF24L01_RX_PW_P2_REG,NRF24L01_PAYLOAD_SIZE);    /* Set the pay load size. */
            break;

        case NRF24L01_DATA_PIPE_3:
            /* Set the 5-byte address of the data pipe 3. */
            NRF24L01_writeRegisterMulti(NRF24L01_RX_ADDR_P3_REG,G_receiverPipe3Address,NRF24L01_TX_RX_ADDRESS_SIZE);
            NRF24L01_writeRegister(NRF24L01_RX_PW_P3_REG,NRF24L01_PAYLOAD_SIZE);    /* Set the pay load size. */
            break;

        case NRF24L01_DATA_PIPE_4:
            /* Set the 5-byte address of the data pipe 4. */
            NRF24L01_writeRegisterMulti(NRF24L01_RX_ADDR_P4_REG,G_receiverPipe4Address,NRF24L01_TX_RX_ADDRESS_SIZE);
            NRF24L01_writeRegister(NRF24L01_RX_PW_P4_REG,NRF24L01_PAYLOAD_SIZE);    /* Set the pay load size. */
            break;

        case NRF24L01_DATA_PIPE_5:
            /* Set the 5-byte address of the data pipe 5. */
            NRF24L01_writeRegisterMulti(NRF24L01_RX_ADDR_P5_REG,G_receiverPipe5Address,NRF24L01_TX_RX_ADDRESS_SIZE);
            NRF24L01_writeRegister(NRF24L01_RX_PW_P5_REG,NRF24L01_PAYLOAD_SIZE);    /* Set the pay load size. */
            break;

        default:
            /* Do Nothing. */
            break;
        }

        /* Turn the module power up and select the RX mode [with preserving the other bits]. */
        NRF24L01_readRegister(NRF24L01_CONFIG_REG,&LOC_registerValue);
        LOC_registerValue = ((LOC_registerValue & 0XFC) | (0X03));
        NRF24L01_writeRegister(NRF24L01_CONFIG_REG,LOC_registerValue);

        /* Activate the nRF chip after completing the configuration. */
        GPIO_writePin(NRF24L01_SPI_CE_PORT_ID,NRF24L01_SPI_CE_PIN_ID,GPIO_HIGH_PIN);
    }

    return LOC_errorStatus;
}

/*=====================================================================================================================
 * [Function Name] : NRF24L01_checkDataPipe
 * [Description]   : Check if the data pipe Receiver buffer is empty or not.
 * [Arguments]     : <a_dataPipe>      -> Indicates to the required data pipe to be checked.
 *                   <a_ptr2status>    -> Pointer to a variable to store the data pipe status.
 * [return]        : The function returns error status: - No Errors.
 *                                                      - Data Pipe Error.
 *                                                      - Null Pointer Error.
 ====================================================================================================================*/
NRF24L01_errorStatusType NRF24L01_checkDataPipe(NRF24L01_dataPipeType a_dataPipe, NRF24L01_dataPipeStatusType* a_ptr2status)
{
    NRF24L01_errorStatusType LOC_errorStatus = NRF24L01_NO_ERRORS;
    uint8 LOC_registerValue = 0;

    if(a_dataPipe > NRF24L01_DATA_PIPE_5)
    {
        LOC_errorStatus = NRF24L01_DATA_PIPE_ERROR;
    }

    else if(a_ptr2status == NULL_PTR)
    {
        LOC_errorStatus = NRF24L01_NULL_PTR_ERROR;
    }

    else
    {
        *a_ptr2status = NRF24L01_DATA_PIPE_EMPTY;

        /* Read the value of the status register. */
        NRF24L01_readRegister(NRF24L01_STATUS_REG,&LOC_registerValue);

        /* Check if the RX_DR flag is set and the value of RX_P_NO bits are equal to the required data pipe or not. */
        if((LOC_registerValue & (1 << 6)) && (((LOC_registerValue & 0X0E) >> 1) == a_dataPipe))
        {
            /* Clear the RX_DR flag by writting 1 on it. */
            NRF24L01_writeRegister(NRF24L01_STATUS_REG,0X4E);

            /* Change the data pipe status to Not Empty. */
            *a_ptr2status = NRF24L01_DATA_PIPE_NOT_EMPTY;
        }
    }

    return LOC_errorStatus;
}

/*=====================================================================================================================
 * [Function Name] : NRF24L01_sendData
 * [Description]   : Transmit data through the NRF24L01 module.
 * [Arguments]     : <a_ptr2data>    -> Pointer to the required data to be transmitted.
 *                   <a_size>        -> Indicates to the size of the data.
 * [return]        : The function returns error status: - No Errors.
 *                                                      - Null Pointer Error.
 ====================================================================================================================*/
NRF24L01_errorStatusType NRF24L01_sendData(uint8* a_ptr2data, uint8 a_size)
{
    NRF24L01_errorStatusType LOC_errorStatus = NRF24L01_NO_ERRORS;

    if(a_ptr2data == NULL_PTR)
    {
        LOC_errorStatus = NRF24L01_NO_ERRORS;
    }

    else
    {
        /* Select the nRF chip for SPI transmission. */
        GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_LOW_PIN);

        //SPI_sendData(NRF24L01_W_TX_PAYLOAD_COMMAND);       /* Send the write payload command through SPI. */
        //SPI_sendDataMulti(a_ptr2data,a_size);                      /* Send the required data through SPI. */

        /* Unselect the nRF chip after completing the transmission. */
        GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_HIGH_PIN);
    }

    return LOC_errorStatus;
}

/*=====================================================================================================================
 * [Function Name] : NRF24L01_readData
 * [Description]   : Read data from the NRF24L01 receiver buffer.
 * [Arguments]     : <a_ptr2data>    -> Pointer to array to store the received data.
 *                   <a_size>        -> Indicates to the size of the data.
 * [return]        : The function returns error status: - No Errors.
 *                                                      - Null Pointer Error.
 ====================================================================================================================*/
NRF24L01_errorStatusType NRF24L01_readData(uint8* a_ptr2data, uint8 a_size)
{
    NRF24L01_errorStatusType LOC_errorStatus = NRF24L01_NO_ERRORS;

    if(a_ptr2data == NULL_PTR)
    {
        LOC_errorStatus = NRF24L01_NO_ERRORS;
    }

    else
    {
        /* Select the nRF chip for SPI transmission. */
        GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_LOW_PIN);

        //SPI_sendData(NRF24L01_R_RX_PAYLOAD_COMMAND);        /* Send the read payload command through SPI. */
        //SPI_readDataMulti(a_ptr2data,a_size);                      /* Read the received data through SPI. */

        /* Unselect the nRF chip after completing the transmission. */
        GPIO_writePin(NRF24L01_SPI_CSN_PORT_ID,NRF24L01_SPI_CSN_PIN_ID,GPIO_HIGH_PIN);
    }

    return LOC_errorStatus;
}

