#include "logger.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QMutexLocker>
#include <QStandardPaths>

Logger::Logger() : _defaultHandler(nullptr) {
  qSetMessagePattern(
      "[%{time yyMMdd h:mm:ss.zzz} "
      "%{category} "
      "%{if-debug}DEBG%{endif}"
      "%{if-info}INFO%{endif}"
      "%{if-warning}WARN%{endif}"
      "%{if-critical}CRIT%{endif}"
      "%{if-fatal}FATL%{endif}] "
      "- %{message}");

  QString baseDir =
      QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

  QDir dir(baseDir);
  dir.mkpath("logs");

  _logFile.setFileName(dir.absoluteFilePath("logs/application.log"));
  if (_logFile.open(QIODevice::WriteOnly | QIODevice::Append |
                    QIODevice::Text)) {
    _logStream.setDevice(&_logFile);
  }

  _defaultHandler = qInstallMessageHandler(Logger::messageHandler);
}

Logger::~Logger() {
  qInstallMessageHandler(nullptr);
  if (_logFile.isOpen()) {
    _logStream.flush();
    _logFile.close();
  }
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext& context,
                            const QString& msg) {
  auto& logger = i();

  QString fullLogLine = qFormatLogMessage(type, context, msg) + "\n";

  {
    QMutexLocker locker(&logger._mutex);
    if (logger._logFile.isOpen()) {
      logger._logStream << fullLogLine;
      logger._logStream.flush();
    }
  }

  if (logger._defaultHandler) {
    logger._defaultHandler(type, context, msg);
  }
}
