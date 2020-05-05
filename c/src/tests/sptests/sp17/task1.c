/*  Task_1
 *
 *  This task initializes the signal catcher, sends the first signal
 *  if running on the first node, and loops while waiting for signals.
 *
 *  NOTE: The signal catcher is not reentrant and hence RTEMS_NO_ASR must
 *        be a part of its execution mode.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989, 1990, 1991, 1992, 1993, 1994.
 *  On-Line Applications Research Corporation (OAR).
 *  All rights assigned to U.S. Government, 1994.
 *
 *  This material may be reproduced by or for the U.S. Government pursuant
 *  to the copyright license under the clause at DFARS 252.227-7013.  This
 *  notice must appear in all copies of this file and its derivatives.
 *
 *  $Id$
 */

#include "system.h"

rtems_task Task_1(
  rtems_task_argument argument
)
{
  rtems_status_code status;

  puts( "TA1 - rtems_signal_catch: initializing signal catcher" );
  status = rtems_signal_catch( Process_asr, RTEMS_NO_ASR | RTEMS_NO_PREEMPT );
  directive_failed( status, "rtems_signal_catch" );

  puts( "TA1 - Sending signal to self" );
  status = rtems_signal_send( Task_id[ 1 ], RTEMS_SIGNAL_16 );
  directive_failed( status, "rtems_signal_send" );

  if ( Task_2_preempted == TRUE )
    puts( "TA1 - TA2 correctly preempted me" );

  puts("TA1 - Got Back!!!");

  puts( "*** END OF TEST 17 ***" );
  exit( 0 );
}