#include "utils.h"

namespace utils{
qint64 GetCurrentTime(){
  return QDateTime::currentSecsSinceEpoch();
}
}

