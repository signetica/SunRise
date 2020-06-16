#ifndef SunRise_h
#define SunRise_h

#include <time.h>

// Size of event search window in hours.
// Events further away from the search time than MR_WINDOW/2 will not be
// found.  At higher latitudes the sun rise/set intervals become larger, so if
// you want to find the nearest events this will need to increase.  Larger
// windows will increase interpolation error.  Useful values are probably from
// 12 - 48 but will depend upon your application.

#define SR_WINDOW   48	    // Even integer

struct sunCoordinates {
    double RA;		    // Right ascension
    double declination;	    // Declination
};

class SunRise {
  public:
    time_t queryTime = 0;
    time_t riseTime = 0;
    time_t setTime = 0;
    float riseAz = 0;
    float setAz = 0;
    bool hasRise = false;
    bool hasSet = false;
    bool isVisible = false;

    SunRise(double latitude, double longitude, time_t t);

  private:
    void testSunRiseSet(int i, double offsetDays, double latitude, double longitude,
			 struct sunCoordinates *mp);
    struct sunCoordinates sun(double dayOffset);
    double interpolate(double f0, double f1, double f2, double p);
    double julianDate(time_t t);
    double localSiderealTime(double offsetDays, double longitude);
};
#endif
