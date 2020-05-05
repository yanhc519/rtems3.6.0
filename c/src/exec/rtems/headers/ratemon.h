/*  ratemon.h
 *
 *  This include file contains all the constants, structures, and
 *  prototypes associated with the Rate Monotonic Manager.  This manager
 *  provides facilities to implement tasks which execute in a periodic fashion.
 *
 *  Directives provided are:
 *
 *     + create a rate monotonic timer
 *     + cancel a period
 *     + delete a rate monotonic timer
 *     + conclude current and start the next period
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

#ifndef __RTEMS_RATE_MONOTONIC_h
#define __RTEMS_RATE_MONOTONIC_h

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/score/watchdog.h>

/*
 *  The following enumerated type defines the states in which a
 *  period may be.
 */

typedef enum {
  RATE_MONOTONIC_INACTIVE,               /* off chain, never initialized */
  RATE_MONOTONIC_OWNER_IS_BLOCKING,      /* on chain, owner is blocking on it */
  RATE_MONOTONIC_ACTIVE,                 /* on chain, running continuously */
  RATE_MONOTONIC_EXPIRED_WHILE_BLOCKING, /* on chain, expired while owner was */
                                         /*   was blocking on it */
  RATE_MONOTONIC_EXPIRED                 /* off chain, will be reset by next */
                                         /*   rtems_rate_monotonic_period */
}   Rate_Monotonic_Period_states;

/*
 *  The following constant is the interval passed to the rate_monontonic_period
 *  directive to obtain status information.
 */

#define RTEMS_PERIOD_STATUS       WATCHDOG_NO_TIMEOUT

/*
 *  The following structure defines the control block used to manage
 *  each period.
 */

typedef struct {
  Objects_Control               Object;
  Watchdog_Control              Timer;
  Rate_Monotonic_Period_states  state;
  Thread_Control               *owner;
}   Rate_monotonic_Control;

RTEMS_EXTERN Objects_Information _Rate_monotonic_Information;

/*
 *  _Rate_monotonic_Manager_initialization
 *
 *  DESCRIPTION:
 *
 *  This routine performs the initialization necessary for this manager.
 */

void _Rate_monotonic_Manager_initialization(
  unsigned32 maximum_periods
);

/*
 *  rtems_rate_monotonic_create
 *
 *  DESCRIPTION:
 *
 *  This routine implements the rate_monotonic_create directive.  The
 *  period will have the name name.  It returns the id of the
 *  created period in ID.
 */

rtems_status_code rtems_rate_monotonic_create(
  rtems_name    name,
  Objects_Id   *id
);

/*
 *  rtems_rate_monotonic_ident
 *
 *  DESCRIPTION:
 *
 *  This routine implements the rtems_rate_monotonic_ident directive.
 *  This directive returns the period ID associated with name.
 *  If more than one period is named name, then the period
 *  to which the ID belongs is arbitrary.
 */

rtems_status_code rtems_rate_monotonic_ident(
  rtems_name    name,
  Objects_Id   *id
);

/*
 *  rtems_rate_monotonic_cancel
 *
 *  DESCRIPTION:
 *
 *  This routine implements the rtems_rate_monotonic_cancel directive.  This
 *  directive stops the period associated with ID from continuing to
 *  run.
 */

rtems_status_code rtems_rate_monotonic_cancel(
  Objects_Id id
);

/*
 *  rtems_rate_monotonic_delete
 *
 *  DESCRIPTION:
 *
 *  This routine implements the rtems_rate_monotonic_delete directive.  The
 *  period indicated by ID is deleted.
 */

rtems_status_code rtems_rate_monotonic_delete(
  Objects_Id id
);

/*
 *  rtems_rate_monotonic_period
 *
 *  DESCRIPTION:
 *
 *  This routine implements the rtems_rate_monotonic_period directive.  When
 *  length is non-zero, this directive initiates the period associated with
 *  ID from continuing for a period of length.  If length is zero, then
 *  result is set to indicate the current state of the period.
 */

rtems_status_code rtems_rate_monotonic_period(
  Objects_Id      id,
  rtems_interval  length
);

/*
 *  _Rate_monotonic_Timeout
 *
 *  DESCRIPTION:
 *
 *  This routine is invoked when the period represented
 *  by ID expires.  If the task which owns this period is blocked
 *  waiting for the period to expire, then it is readied and the
 *  period is restarted.  If the owning task is not waiting for the
 *  period to expire, then the period is placed in the EXPIRED
 *  state and not restarted.
 */

void _Rate_monotonic_Timeout (
  Objects_Id  id,
  void       *ignored
);

#ifndef __RTEMS_APPLICATION__
#include <rtems/rtems/ratemon.inl>
#endif

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */