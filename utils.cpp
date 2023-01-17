#include "utils.h"

qint64 GetCurrentTime(){
  return QDateTime::currentMSecsSinceEpoch();
}
