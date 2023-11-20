/*
=======================================================================================================================
Author       : Mamoun
Module       : NVIC
File Name    : nvic_cfg.h
Date Created : Nov 17, 2023
Description  : Configuration file for the STM32F303 NVIC driver.
=======================================================================================================================
*/


#ifndef MCAL_NVIC_INCLUDES_NVIC_CFG_H_
#define MCAL_NVIC_INCLUDES_NVIC_CFG_H_

/*=====================================================================================================================
                                               < Includes >
=====================================================================================================================*/



/*=====================================================================================================================
                                < Definitions and Static Configurations >
=====================================================================================================================*/

/*=====================================================================================================================
The NVIC priority group level can only configured as: - SCB_PRIGROUP_LEVEL_A
                                                      - SCB_PRIGROUP_LEVEL_B
    [Check "SCB/INCLUDES/scb.h" for more details]     - SCB_PRIGROUP_LEVEL_C
                                                      - SCB_PRIGROUP_LEVEL_D
                                                      - SCB_PRIGROUP_LEVEL_E
=====================================================================================================================*/
#define NVIC_PRIORITY_GROUP_LEVEL                    (SCB_PRIGROUP_LEVEL_A)

#endif /* MCAL_NVIC_INCLUDES_NVIC_CFG_H_ */
