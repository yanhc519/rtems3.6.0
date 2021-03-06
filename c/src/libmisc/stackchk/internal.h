/*  internal.h
 *
 *  This include file contains internal information
 *  for the RTEMS stack checker.
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

#ifndef __INTERNAL_STACK_CHECK_h
#define __INTERNAL_STACK_CHECK_h

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  This structure is used to fill in and compare the "end of stack"
 *  marker pattern.
 *  pattern area must be a multiple of 4 words.
 */

#ifdef CPU_STACK_CHECK_SIZE
#define PATTERN_SIZE_WORDS      (((CPU_STACK_CHECK_SIZE / 4) + 3) & ~0x3)
#else
#define PATTERN_SIZE_WORDS      4
#endif

#define PATTERN_SIZE_BYTES      (PATTERN_SIZE_WORDS * 4)

typedef struct {
   unsigned32  pattern[ PATTERN_SIZE_WORDS ];
} Stack_check_Control;

/*
 *  The pattern used to fill the entire stack.
 */

#define BYTE_PATTERN 0xA5
#define U32_PATTERN 0xA5A5A5A5

/*
 *  Stack_check_Create_extension
 */

boolean Stack_check_Create_extension(
  Thread_Control *running,
  Thread_Control *the_thread
);

/*
 *  Stack_check_Begin_extension
 */

void Stack_check_Begin_extension(
  Thread_Control *the_thread
);

/*
 *  Stack_check_Switch_extension
 */

void Stack_check_Switch_extension(
  Thread_Control *running,
  Thread_Control *heir
);

/*
 *  Stack_check_Fatal_extension
 */

void Stack_check_Fatal_extension(
    Internal_errors_Source  source,
    boolean                 is_internal,
    unsigned32              status
);

/*
 *  Stack_check_Dump_usage
 */

void Stack_check_Dump_usage( void );

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
