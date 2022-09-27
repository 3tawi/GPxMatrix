
// - Lien vidéo: https://youtu.be/bQL0pm41uJY

#ifndef Modeclock_H
#define Modeclock_H


#include "AnalogClock.h"
#include "AnalogClock1.h"
#include "DigitalClock.h"
#include "DigitalClock0.h"
#include "DigitalClock1.h"
#include "DigitalClock2.h"
#include "DigitalClock3.h"
#include "DigitalClock4.h"
#include "DigitalClock5.h"
#include "DigitalClock6.h"
#include "DigitalClock7.h"
#include "plasmaClock.h"

class Modeclock : public list {
  private:
    AnalogClock analogClock;
    AnalogClock1 analogClock1;
    DigitalClock digitalClock;
    DigitalClock0 digitalClock0;
    DigitalClock1 digitalClock1;
    DigitalClock2 digitalClock2;
    DigitalClock3 digitalClock3;
    DigitalClock4 digitalClock4;
    DigitalClock5 digitalClock5;
    DigitalClock6 digitalClock6;
    DigitalClock7 digitalClock7;
    plasmaClock plasmaclock;

    int currentIndex = 0;
    Play* currentItem;

    int getCurrentIndex() {
      return currentIndex;
    }

    static const int Modenm = 12;
    Play* items[Modenm] = {
      &analogClock1,
      &analogClock,
      &digitalClock,
      &digitalClock0,
      &digitalClock1,
      &digitalClock2,
      &digitalClock3,
      &digitalClock4,
      &digitalClock5,
      &digitalClock6,
      &digitalClock7,
      &plasmaclock
    };

  
  public:
    char* Play::name = (char *)"Modeclock";

    void stop() {
      if (currentItem)
        currentItem->stop();
    }

    void start() {
      if (currentItem)
        currentItem->start();
    }

    void move(int step) {
      currentIndex += step;
      if (currentIndex >= Modenm) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = Modenm - 1;
      moveTo(currentIndex);
    }


    unsigned int playframe() {
      return currentItem->playframe();
    }

    void moveTo(int index) {
      if (currentItem)
        currentItem->stop();

      currentIndex = index;

      currentItem = items[currentIndex];

      if (currentItem)
        currentItem->start();
    }  


    bool setModeclock(int index) {
      if (index >= Modenm || index < 0)
        return false;

      moveTo(index);

      return true;
    }
    int getModeIndex() {
      return currentIndex;
    }
};

#endif
