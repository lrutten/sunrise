#include <iostream>
#include <ctime>
#include <cmath>
#include "sunset.h"

#define ONE_HOUR    ((double)(60 * 60))
#define TIMEZONE    +1

// This location is Rooierheidestraat 11, Diepenbeek
#define LATITUDE    50.93010
#define LONGITUDE   5.41078


bool isDst(int y, int m, int d)
{
    struct tm tm1;

    tm1.tm_year = y - 1900;
    tm1.tm_mon = m - 1;
    tm1.tm_mday = d;
    tm1.tm_hour = 12;
    tm1.tm_min = tm1.tm_sec = 0;
    tm1.tm_isdst = -1;
    mktime(&tm1);

    //printf("at %d:%02d:%02d on %d-%02d-%02d, DST %s in effect\n",
    //    tm1.tm_hour, tm1.tm_min, tm1.tm_sec,
    //    tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,
    //    tm1.tm_isdst ? "was" : "was not");

   return tm1.tm_isdst > 0;
}


bool isSchrikkel(int y)
{
   if (y%4 != 0)
   {
      return false;
   }
   else
   if (y%100 != 0)
   {
      return true;
   }
   else
   if (y%400 == 0)
   {
      return true;
   }
   else
   {
      return false;
   }
}

struct RiseSet
{
   int r_h;
   int r_m;
   int s_h;
   int s_m;
};

RiseSet calculate(SunSet &sun, int y, int m, int d)
{
   RiseSet rs;
   auto rightnow = std::time(nullptr);

   sun.setCurrentDate(y, m, d);
   double sunrise = sun.calcSunrise();
   double sunset = sun.calcSunset();
   double civilSunrise = sun.calcCivilSunrise();
   double nauticalSunrise = sun.calcNauticalSunrise();
   double astroSunrise = sun.calcAstronomicalSunrise();
   int moonphase = sun.moonPhase(static_cast<int>(rightnow));

   int i_sunrise = (int) std::round(sunrise);
   int i_sunset = (int) std::round(sunset);

   rs.r_h = i_sunrise/60;
   rs.r_m = i_sunrise%60;
   rs.s_h = i_sunset/60;
   rs.s_m = i_sunset%60;

   return rs;
}

int lengte_maand(int m)
{
   if (m==1 || m==3 || m==5 || m==7 || m==8 || m==10 || m==12)
   {
      return 31;
   }
   else
   if (m == 2)
   {
      if (isSchrikkel(m))
      {
         return 29;
      }
      else
      {
         return 28;
      }
   }
   else
   {
      return 30;
   }
}

void toon_maand(SunSet &sun, int y, int m)
{
   int m_len = lengte_maand(m);
   for (int d = 1; d<=m_len; d++)
   {
      int dst = isDst(y, m, d) ? 1 : 0;
      RiseSet rs = calculate(sun, y, m, d);
      std::cout << "|" << m << " | " << d << " | " << (rs.r_h + dst) << ":" << rs.r_m  << " | " << (rs.s_h + dst) << ":" << rs.s_m << "|\n";
   }
}

int main(int argc, char **argv)
{
   SunSet sun;

   auto rightnow = std::time(nullptr);
   struct tm *tad = std::localtime(&rightnow);
   sun.setPosition(LATITUDE, LONGITUDE, tad->tm_gmtoff / ONE_HOUR);

   if (argc == 1)
   {
      // vandaag
      RiseSet rs = calculate(sun, tad->tm_year + 1900, tad->tm_mon + 1, tad->tm_mday);

      std::cout << "sunrise h " << rs.r_h << "\n";
      std::cout << "sunrise m " << rs.r_m << "\n";
      std::cout << "sunset  h " << rs.s_h << "\n";
      std::cout << "sunset  m " << rs.s_m << "\n";
   }
   else
   if (argc == 2)
   {
      // jaar
      try
      {
         int y = std::stoi(argv[1]);
         if (y >= 1900 && y <= 2100)
         {
            std::cout << "| Maand  | Dag  | Op  | Onder  |\n";
            std::cout << "|--------|------|-----|--------|\n";

            for (int m=1; m<=12; m++)
            {
               toon_maand(sun, y, m);
            }
         }
      }
      catch (std::invalid_argument e)
      {
         std::cout << "error integer\n";
      }
   }
   else
   if (argc == 3)
   {
      // jaar en maand
      try
      {
         int y = std::stoi(argv[1]);
         int m = std::stoi(argv[2]);
         if (y >= 1900 && y <= 2100 && m >= 1 && m <= 12)
         {
            std::cout << "| Maand  | Dag  | Op  | Onder  |\n";
            std::cout << "|--------|------|-----|--------|\n";

            toon_maand(sun, y, m);
         }
      }
      catch (std::invalid_argument e)
      {
         std::cout << "error integer\n";
      }
   }

   return 0;
}


