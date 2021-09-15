/*=============================================================================

                  T I M E   S E R V I C E   S U B S Y S T E M

                  J U L I A N   T I M E   C O N V E R S I O N


GENERAL DESCRIPTION
  This module contains procedures to convert seconds to and from Julian
  calendar units.

  IMPORTANT NOTE: The time calculations in this module are only valid for
  dates and times from 6 Jan 1988, 00:00:00 to 28 Feb 2100, 23:59:59.  This
  is due to the fact that this module assumes that all years evenly divisible
  by 4 are leap years. Unfortunately, century years (e.g., 1900, 2000, 2100)
  must also be evenly divisible by 400 in order to be leap years, and so the
  year 2100 does not qualify as a leap year.


EXTERNALIZED FUNCTIONS
  time_jul_from_secs
    This function converts a specified number of elapsed seconds since
    the base date to its equivalent Julian date and time.

  time_jul_to_secs
    This function converts a specified Julian date and time to an
    equivalent number of elapsed seconds since the base date.


INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.


Copyright (c) 1994, 1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999, 2003 by QUALCOMM, Incorporated.  All Rights Reserved.

=============================================================================*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //depot/asic/msmshared/services/time/time_jul.c#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
08/08/03   ajn     Coding standard update.
07/24/03   ajn     Xfer'd clkjul.c from MSM#### CLOCK VU's to general TIME VU

=============================================================================*/



/*=============================================================================

                           INCLUDE FILES

=============================================================================*/

#include "time_jul.h"
// #include "misc.h"       /* Prototype for direct call to div4x2 */



/*=============================================================================

                           DATA DEFINITIONS

=============================================================================*/


/*-----------------------------------------------------------------------------
                       Time Translation Tables

  The following tables are used for making conversions between
  Julian dates and number of seconds.
-----------------------------------------------------------------------------*/

/* This is the number of days in a leap year set.
   A leap year set includes 1 leap year, and 3 normal years. */
#define TIME_JUL_QUAD_YEAR        (366+(3*365))

/* The year_tab table is used for determining the number of days which
   have elapsed since the start of each year of a leap year set. It has
   1 extra entry which is used when trying to find a 'bracketing' year.
   The search is for a day >= year_tab[i] and day < year_tab[i+1]. */

static const uint16_t year_tab[] = {
        0,                              /* Year 0 (leap year) */
        366,                            /* Year 1             */
        366+365,                        /* Year 2             */
        366+365+365,                    /* Year 3             */
        366+365+365+365                 /* Bracket year       */
};


/* The norm_month_tab table holds the number of cumulative days that have
   elapsed as of the end of each month during a non-leap year. */

static const uint16_t norm_month_tab[] = {
        0,                                    /* --- */
        31,                                   /* Jan */
        31+28,                                /* Feb */
        31+28+31,                             /* Mar */
        31+28+31+30,                          /* Apr */
        31+28+31+30+31,                       /* May */
        31+28+31+30+31+30,                    /* Jun */
        31+28+31+30+31+30+31,                 /* Jul */
        31+28+31+30+31+30+31+31,              /* Aug */
        31+28+31+30+31+30+31+31+30,           /* Sep */
        31+28+31+30+31+30+31+31+30+31,        /* Oct */
        31+28+31+30+31+30+31+31+30+31+30,     /* Nov */
        31+28+31+30+31+30+31+31+30+31+30+31   /* Dec */
};

/* The leap_month_tab table holds the number of cumulative days that have
   elapsed as of the end of each month during a leap year. */

static const uint16_t leap_month_tab[] = {
        0,                                    /* --- */
        31,                                   /* Jan */
        31+29,                                /* Feb */
        31+29+31,                             /* Mar */
        31+29+31+30,                          /* Apr */
        31+29+31+30+31,                       /* May */
        31+29+31+30+31+30,                    /* Jun */
        31+29+31+30+31+30+31,                 /* Jul */
        31+29+31+30+31+30+31+31,              /* Aug */
        31+29+31+30+31+30+31+31+30,           /* Sep */
        31+29+31+30+31+30+31+31+30+31,        /* Oct */
        31+29+31+30+31+30+31+31+30+31+30,     /* Nov */
        31+29+31+30+31+30+31+31+30+31+30+31   /* Dec */
};

/* The day_offset table holds the number of days to offset
   as of the end of each year. */

static const uint16_t day_offset[] = {
        1,                                    /* Year 0 (leap year) */
        1+2,                                  /* Year 1             */
        1+2+1,                                /* Year 2             */
        1+2+1+1                               /* Year 3             */
};



/*-----------------------------------------------------------------------------
  Date conversion constants
-----------------------------------------------------------------------------*/

/* 5 days (duration between Jan 1 and Jan 6), expressed as seconds. */

#define TIME_JUL_OFFSET_S         432000UL


/* This is the year upon which all time values used by the Clock Services
** are based.  NOTE:  The user base day (GPS) is Jan 6 1980, but the internal
** base date is Jan 1 1980 to simplify calculations */

#define TIME_JUL_BASE_YEAR        1980



/*=============================================================================

FUNCTION TIME_JUL_TO_SECS

DESCRIPTION
  This procedure converts a specified Julian date and time to an
  equivalent number of elapsed seconds since the base date.

DEPENDENCIES
  None

RETURN VALUE
  Number of elapsed seconds since base date.

SIDE EFFECTS
  None

=============================================================================*/

uint32_t time_jul_to_secs(const time_julian_type *julian) {
    /* Time in various units (days, hours, minutes, and finally seconds) */
    uint32_t time;

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* First, compute number of days contained in the last whole leap
       year set. */

    time = ( (julian->year - TIME_JUL_BASE_YEAR) / 4L ) * TIME_JUL_QUAD_YEAR;


    /* Now, add in days since last leap year to start of this month. */

    if ( (julian->year & 0x3) == 0 )
    {
        /* If this is a leap year, add in only days since the beginning of the
           year to start of this month. */

        time += leap_month_tab[ julian->month - 1 ];
    }
    else
    {
        /* If this is not a leap year, add in days since last leap year plus
           days since start of this year to start of this month. */

        time += year_tab[ julian->year & 0x3 ];

        time += norm_month_tab[ julian->month - 1 ];
    }

    /* Add in days in current month. */
    time += julian->day - 1;

    /* Add in elapsed hours, minutes, and seconds  */
    time = time * 24  +  julian->hour;
    time = time * 60  +  julian->minute;
    time = time * 60  +  julian->second;


    /* Subtract number of seconds from base (GPS) date of 6 Jan 1980 to
       calculation base date of 1 Jan 1980 */

    time -= TIME_JUL_OFFSET_S;


    /* Return elapsed seconds. */
    return time;

} /* time_jul_to_secs */

