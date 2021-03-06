/*  void Shm_Get_configuration( localnode, &shmcfg )
 *
 *  This routine initializes, if necessary, and returns a pointer
 *  to the Shared Memory Configuration Table for the MVME147.
 *
 *  INPUT PARAMETERS:
 *    localnode - local node number
 *    shmcfg    - address of pointer to SHM Config Table
 *
 *  OUTPUT PARAMETERS:
 *    *shmcfg   - pointer to SHM Config Table
 *
 *  NOTES:  The SIGLP interrupt on the MVME147 is used as an interprocessor
 *          interrupt.
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  MVME147 port for TNI - Telecom Bretagne
 *  by Dominique LE CAMPION (Dominique.LECAMPION@enst-bretagne.fr)
 *  June 1996
 *
 *  $Id$
 */

#include <bsp.h>
#include <rtems.h>
#include "shm.h"

#define INTERRUPT 1                   /* MVME147 target supports both */
#define POLLING   0                   /* polling and interrupt modes  */

shm_config_table BSP_shm_cfgtbl;

rtems_unsigned32 *BSP_int_address()
{
  rtems_unsigned32 id, offset;

  id      = (rtems_unsigned32) vme_lcsr->gcsr_base_address;
  offset  = (id << 4) & 0xF0;
  offset |= 0xffff0003; /* points to GCSR global 1 */
  return( (rtems_unsigned32 * ) offset );
}

void Shm_Get_configuration(
  rtems_unsigned32   localnode,
  shm_config_table **shmcfg
)
{
  /* A shared mem space has bee left between RAM_END and DRAM_END 
   on the first node*/
  if (localnode == 1)
    BSP_shm_cfgtbl.base       = (vol_u32 *) RAM_END; 
  else
    BSP_shm_cfgtbl.base       = (vol_u32 *) (DRAM_END + RAM_END);

  BSP_shm_cfgtbl.length       = DRAM_END - RAM_END; 
  BSP_shm_cfgtbl.format       = SHM_BIG;
  
  BSP_shm_cfgtbl.cause_intr   = Shm_Cause_interrupt;
  
#ifdef NEUTRAL_BIG
  BSP_shm_cfgtbl.convert      = NULL_CONVERT;
#else
  BSP_shm_cfgtbl.convert      = CPU_swap_u32;
#endif

#if (POLLING==1)
  BSP_shm_cfgtbl.poll_intr    = POLLED_MODE;
  BSP_shm_cfgtbl.Intr.address = NO_INTERRUPT;
  BSP_shm_cfgtbl.Intr.value   = NO_INTERRUPT;
  BSP_shm_cfgtbl.Intr.length  = NO_INTERRUPT;
#else
  BSP_shm_cfgtbl.poll_intr    = INTR_MODE;
  BSP_shm_cfgtbl.Intr.address = BSP_int_address(); /* GCSR global 1 */
  BSP_shm_cfgtbl.Intr.value   = 0x01; /* SIGLP */
  BSP_shm_cfgtbl.Intr.length  = BYTE;
#endif

  *shmcfg = &BSP_shm_cfgtbl;

}
