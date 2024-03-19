#ifndef SunRise_h
#define SunRise_h

#include <time.h>

/**
 * @def SR_WINDOW
 * @brief Size of event search window in hours.
 *
 * Events further away from the search time than SR_WINDOW/2 will not be
 * found. At higher latitudes the sun rise/set intervals become larger, so if
 * you want to find the nearest events this will need to increase. Larger
 * windows will increase interpolation error. Useful values are probably from
 * 12 - 48 but will depend upon your application.
 * Even integers only.
 */
#define SR_WINDOW 48

/**
 * @def K1
 * @brief A constant used in the calculation of sun rise and set times.
 *
 * This constant is used in the calculation of local mean time. It is equal to 15 degrees (the number of degrees the Earth rotates per hour) converted to radians and multiplied by the sidereal to solar time ratio (1.0027379).
 */
#define K1 15 * (M_PI / 180) * 1.0027379

/**
 * @def M_PI
 * @brief The mathematical constant pi.
 *
 * This macro defines the mathematical constant pi, which is the ratio of a circle's circumference to its diameter. It is approximately equal to 3.14159265358979323846.
 */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/**
 * @struct skyCoordinates_t
 * @brief A struct to hold sky coordinates.
 *
 * This struct holds the right ascension and declination, which are two of the coordinates used to locate objects in the sky.
 */
typedef struct 
{
  /**
   * @brief The right ascension of the object.
   *
   * Right ascension is one of the coordinates used in the equatorial coordinate system. It is measured in hours, minutes, and seconds, and ranges from 0 to 24 hours.
   */
  float RA;

  /**
   * @brief The declination of the object.
   *
   * Declination is one of the coordinates used in the equatorial coordinate system. It is measured in degrees, and ranges from -90 degrees (south) to +90 degrees (north).
   */
  float declination;
} skyCoordinates_t;

/**
 * @class SunRise
 * @brief A class to calculate sun rise and set times.
 *
 * This class provides functions to calculate the times of sun rise and set,
 * as well as the azimuths at which these events occur.
 */
class SunRise
{
public:
  /**
   * @brief The time for which the sun rise/set times are queried.
   */
  time_t queryTime;

  /**
   * @brief The sun rise event, in UTC seconds from the Unix epoch.
   */
  time_t riseTime;

  /**
   * @brief The sun set event, in UTC seconds from the Unix epoch.
   */
  time_t setTime;

  /**
   * @brief Where the sun will rise in degrees from north.
   */
  float riseAz;

  /**
   * @brief Where the sun will set in degrees from north.
   */
  float setAz;

  /**
   * @brief There was a sunrise event found in the search interval.
   *
   * The default search interval is 48 hours, 24 hours before and after `queryTime`.
   */
  bool hasRise;

  /**
   * @brief There was a sunset event found in the search interval.
   *
   * The default search interval is 48 hours, 24 hours before and after `queryTime`.
   */
  bool hasSet;

  /**
   * @brief If the sun is visible at `queryTime`.
   */
  bool isVisible;

  /**
   * @brief Calculate the sun rise and set times.
   *
   * This function calculates the times of sun rise and set, as well as the azimuths at which these events occur.
   *
   * @param latitude The latitude of the location of interest in decimal degrees. It ranges from -90 (south pole) to 90 (north pole).
   * @param longitude The longitude of the location of interest in decimal degrees. It ranges from -180 (west of Greenwich) to 180 (east of Greenwich).
   * @param t The time to search for events, in UTC seconds from the Unix epoch (January 1, 1970). The closest sun rise/set event will be found before and after this time. In polar regions, there may not be an event within the configurable search window, in which case zero, one, or two events may all be found either before or after this time.
   */
  void calculate(float latitude, float longitude, time_t t);

private:
  /**
   * @brief Look for sun rise or set events during an hour.
   *
   * This function checks for sun rise or set events during an hour interval.
   *
   * @param k The interval to check.
   * @param offsetDays The number of days since Jan 1, 2000, 1200UTC.
   * @param latitude The latitude at which to check for events.
   * @param longitude The longitude at which to check for events.
   * @param sp The sky coordinates to use for the check.
   */
  void testSunRiseSet(int i, float offsetDays, float latitude, float longitude, skyCoordinates_t *mp);

  /**
   * @brief Look for sun rise or set events during an hour.
   *
   * This function checks for sun rise or set events during an hour interval.
   *
   * @param k The interval to check.
   * @param offsetDays The number of days since Jan 1, 2000, 1200UTC.
   * @param latitude The latitude at which to check for events.
   * @param longitude The longitude at which to check for events.
   * @param sp The sky coordinates to use for the check.
   */
  skyCoordinates_t sun(float dayOffset);

  /**
   * @brief Perform a 3-point interpolation.
   *
   * This function performs a 3-point interpolation.
   *
   * @param f0 The value at the first point.
   * @param f1 The value at the second point.
   * @param f2 The value at the third point.
   * @param p The interpolation parameter.
   * @return The interpolated value.
   */
  float interpolate(float f0, float f1, float f2, float p);
  
  /**
   * @brief Determine the Julian date from Unix time.
   *
   * This function determines the Julian date from Unix time.
   *
   * @param t The Unix time.
   * @return The Julian date.
   */
  double julianDate(time_t t);

  /**
   * @brief Calculate the local sidereal time.
   *
   * This function calculates the local sidereal time.
   *
   * @param offsetDays The number of days since Jan 1, 2000, 1200UTC.
   * @param longitude The longitude at which to calculate the local sidereal time.
   * @return The local sidereal time in degrees.
   */
  float localSiderealTime(float offsetDays, float longitude);
  
  /**
   * @brief Initialize the class.
   *
   * This function initializes the class.
   */
  void initClass();
};
#endif
