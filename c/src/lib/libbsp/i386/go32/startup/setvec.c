/*  set_vector
 *
 *  This routine installs an interrupt vector under go32.
 *
 *  INPUT:
 *    handler - interrupt handler entry point
 *    vector  - vector number
 *    type    - 0 indicates raw hardware connect
 *              1 indicates RTEMS interrupt connect
 *
 *  RETURNS:
 *    address of previous interrupt handler
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id$
 */

#include <rtems.h>
#include <bsp.h>

#include <dpmi.h>
#include <go32.h>

i386_isr_entry set_vector(                      /* returns old vector */
  rtems_isr_entry     handler,                  /* isr routine        */
  rtems_vector_number vector,                   /* vector number      */
  int                 type                      /* RTEMS or RAW intr  */
)
{
  i386_isr_entry   previous_isr;

  if ( type )  {
     rtems_interrupt_catch( handler, vector, (rtems_isr_entry *) &previous_isr);
  } else {
     _CPU_ISR_install_raw_handler( vector, handler, (proc_ptr *)&previous_isr);
  }
  return previous_isr;
}

