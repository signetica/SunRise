/*
 * Print out the sun rise and set times, demonstrating the use of the SunRise
 * class.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "SunRise.h"

int
main(int argc, char *argv[]) {
  double latitude = 42;
  double longitude = -90;

  // In Unix/Linux, the Unix time is the seconds since the start of January 1, 1970,
  // in UTC.
  time_t t = time(NULL);

  // On the Arduino, if one uses the popular TimeLib.h library, getting the time
  // in UTC can be more complex.  SunRise needs the UTC Unix time, and returns
  // values in that format.  But TimeLib's now() returns something similar to Unix
  // time, but in a different timezone.

  // Something like this may work:
  //
  // To produce the Unix time in seconds (in UTC) on the Arduino, we have to
  // figure out the difference between whatever is residing in now()
  // and UTC, then add and subtract it to convert to and from UTC.
  // The value of time() seems even more inscrutable and less useful.

#ifdef Arduino
  // Arduino TimeLib only!!!
  time_t t = now();
  struct tm utc_tm = *utcime(&t);
  utc_tm.tm_isdst = -1;
  time_t utcOffset = mktime(&utc_tm) - t;
#else
#define utcOffset 0
#endif

  // Find the last and next lunar set and rise.  The utcOffset is only for Arduino.
  SunRise sr;
  sr.calculate(latitude, longitude, t + utcOffset);

  // Returned values:
  bool sunVisible = sr.isVisible;
  bool sunHasRise = sr.hasRise;
  bool sunHasSet = sr.hasSet;
  float sunRiseAz = sr.riseAz;	      // Where the sun will rise/set in degrees from
  float sunSetAz = sr.setAz;	      // North.

  // Additional returned values requiring conversion from UTC to local time zone
  // on the Arduino.
  time_t sunQueryTime = sr.queryTime - utcOffset;
  time_t sunRiseTime = sr.riseTime - utcOffset;
  time_t sunSetTime = sr.setTime - utcOffset;

  // Use the results as desired (use the utcOffset variables on the Arduino):
  printf("Sun rise/set nearest %.24s for latitude %.2f longitude %.2f:\n",
	 ctime(&sr.queryTime), latitude, longitude);

  printf("Preceding event:\n");
  if ((!sr.hasRise || (sr.hasRise && sr.riseTime > sr.queryTime)) &&
      (!sr.hasSet || (sr.hasSet && sr.setTime > sr.queryTime)))
    printf("\tNo sun rise or set during preceding %d hours\n", SR_WINDOW/2);
  if (sr.hasRise && sr.riseTime < sr.queryTime)
    printf("\tSun rise at %.24s, Azimuth %.2f\n", ctime(&sr.riseTime), sr.riseAz);
  if (sr.hasSet && sr.setTime < sr.queryTime)
    printf("\tSun set at  %.24s, Azimuth %.2f\n", ctime(&sr.setTime), sr.setAz);

  printf("Succeeding event:\n");
  if ((!sr.hasRise || (sr.hasRise && sr.riseTime < sr.queryTime)) &&
      (!sr.hasSet || (sr.hasSet && sr.setTime < sr.queryTime)))
    printf("\tNo sun rise or set during succeeding %d hours\n", SR_WINDOW/2);
  if (sr.hasRise && sr.riseTime > sr.queryTime)
    printf("\tSun rise at %.24s, Azimuth %.2f\n", ctime(&sr.riseTime), sr.riseAz);
  if (sr.hasSet && sr.setTime > sr.queryTime)
    printf("\tSun set at  %.24s, Azimuth %.2f\n", ctime(&sr.setTime), sr.setAz);

  if (sr.isVisible)
    printf("Sun visible.\n");
  else
    printf("Sun not visible.\n");
}
