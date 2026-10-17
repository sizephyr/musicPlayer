#pragma once

#include <utils/singleton.h>

#include <QFile>
#include <QMessageLogContext>
#include <QMutex>
#include <QString>
#include <QTextStream>

class Logger : public Singleton<Logger> {
  friend class Singleton<Logger>;

  Logger();
  ~Logger();

 private:
  static void messageHandler(QtMsgType type, const QMessageLogContext& context,
                             const QString& msg);

  QFile _logFile;
  QTextStream _logStream;
  QMutex _mutex;
  QtMessageHandler _defaultHandler;
};
