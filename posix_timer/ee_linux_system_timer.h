/* ###*B*###
 * --------------------------------------------------------------------------
 * |              _    _ _______     .----.      _____         _____        |
 * |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
 * |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
 * |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
 * |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
 * |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
 * |                              . _ _  .                                  |
 * --------------------------------------------------------------------------
 * 
 * All Rights Reserved.
 * ny use of this source code is subject to a license agreement with the
 * AUTOSAR development cooperation.
 * More information is available at www.autosar.org.
 * 
 * Disclaimer
 * 
 * This work (specification and/or software implementation) and the material
 * contained in it, as released by AUTOSAR, is for the purpose of information
 * only. AUTOSAR and the companies that have contributed to it shall not be
 * liable for any use of the work.
 * 
 * The material contained in this work is protected by copyright and other
 * types of intellectual property rights. The commercial exploitation of the
 * material contained in this work requires a license to such intellectual
 * property rights.
 * 
 * This work may be utilized or reproduced without any modification, in any
 * form or by any means, for informational purposes only. For any other
 * purpose, no part of the work may be utilized or reproduced, in any form
 * or by any means, without permission in writing from the publisher.
 * 
 * The work has been developed for automotive applications only. It has
 * neither been developed, nor tested for non-automotive applications.
 * 
 * The word AUTOSAR and the AUTOSAR logo are registered trademarks.
 * --------------------------------------------------------------------------
 * ###*E*### */

/** \file	ee_linux_system_timer.h
 *  \brief	System Timer driver header file.
 *
 *  \author	Mohamed Ezzat
 *  \date	2025
 */

#ifndef	OSEE_LINUX_SYSTEM_TIMER_H
#define	OSEE_LINUX_SYSTEM_TIMER_H


#if (defined(__cplusplus))
extern "C" {
#endif

/** \brief	System Timer Initialization. */
void osEE_linux_system_timer_init(void);

/** \brief	System Timer Sleep. */
void osEE_linux_system_timer_sleep(void);

/** \brief	System Timer Wakeup. */
void osEE_linux_system_timer_wakeup(void);

void set_callback(void *);
#if (defined(__cplusplus))
}
#endif

#endif	/* OSEE_linux_SYSTEM_TIMER_H */
