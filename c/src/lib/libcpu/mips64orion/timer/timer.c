/*  timer.c
 *
 *  This file contains the initialization code for the IDT 4650 timer driver.
 *
 *  Author:     Craig Lebakken <craigl@transition.com>
 *
 *  COPYRIGHT (c) 1996 by Transition Networks Inc.
 *
 *  To anyone who acknowledges that this file is provided "AS IS"
 *  without any express or implied warranty:
 *      permission to use, copy, modify, and distribute this file
 *      for any purpose is hereby granted without fee, provided that
 *      the above copyright notice and this notice appears in all
 *      copies, and that the name of Transition Networks not be used in
 *      advertising or publicity pertaining to distribution of the
 *      software without specific, written prior permission.
 *      Transition Networks makes no representations about the suitability
 *      of this software for any purpose.
 *
 *  derived from src/lib/libbsp/no_cpu/no_bsp/timer/timer.c
 *
 *  This file manages the benchmark timer used by the RTEMS Timing Test
 *  Suite.  Each measured time period is demarcated by calls to
 *  Timer_initialize() and Read_timer().  Read_timer() usually returns
 *  the number of microseconds since Timer_initialize() exitted.
 *
 *  NOTE: It is important that the timer start/stop overhead be
 *        determined when porting or modifying this code.
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

#ifndef lint
static char _sccsid[] = "@(#)timer.c 08/20/96     1.5\n";
#endif

#include <rtems.h>
#include <bsp.h>

#define CLOCKS_PER_MICROSECOND ( CPU_CLOCK_RATE_MHZ )
#define TIMER_MAX_VALUE 0xffffffff

extern unsigned32 mips_read_timer( void );

static rtems_boolean Timer_driver_Find_average_overhead;
static unsigned32 Timer_initial_value = 0;

void Timer_initialize( void )
{
   Timer_initial_value = mips_read_timer();
  /*
   *  Somehow start the timer
   */

  /* Timer on 4650 is always running */
}

/*
 *  The following controls the behavior of Read_timer().
 *
 *  AVG_OVEREHAD is the overhead for starting and stopping the timer.  It
 *  is usually deducted from the number returned.
 *
 *  LEAST_VALID is the lowest number this routine should trust.  Numbers
 *  below this are "noise" and zero is returned.
 */

#define AVG_OVERHEAD      8  /* It typically takes X.X microseconds */
                             /* (Y countdowns) to start/stop the timer. */
                             /* This value is in cycles. */
#define LEAST_VALID       1  /* Don't trust a clicks value lower than this */

int Read_timer( void )
{
  unsigned64 clicks;
  unsigned32 total;

  /*
   *  Read the timer and see how many clicks it has been since we started.
   */

  clicks = mips_read_timer();   /* XXX: read some HW here */
  if (clicks < Timer_initial_value)
  {
      clicks += TIMER_MAX_VALUE;
  }
  clicks -= Timer_initial_value;

  /*
   *  Total is calculated by taking into account the number of timer overflow
   *  interrupts since the timer was initialized and clicks since the last
   *  interrupts.
   */
#if 0 /* leave total in number of cycles */
   total = clicks / CLOCKS_PER_MICROSECOND;
#else
   total = clicks;
#endif

  if ( Timer_driver_Find_average_overhead == 1 )
    return total;          /* in # cycles units */
  else {
    if ( total < LEAST_VALID )
      return 0;            /* below timer resolution */
  /*
   *  leave total in cycles
   */
      return (total - AVG_OVERHEAD);
    }
}

/*
 *  Empty function call used in loops to measure basic cost of looping
 *  in Timing Test Suite.
 */

rtems_status_code Empty_function( void )
{
  return RTEMS_SUCCESSFUL;
}

void Set_find_average_overhead(
  rtems_boolean find_flag
)
{
  Timer_driver_Find_average_overhead = find_flag;
}
