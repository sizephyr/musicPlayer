#include <QGuiApplication>
#include <QQmlApplicationEngine>

#ifdef SHOW_RESOURCES
#include <QDirIterator>
#endif
#include "windowhelper.h"

int main(int argc, char* argv[]) {
  qSetMessagePattern(
      "[%{time yyMMdd h:mm:ss.zzz} "
      "%{category} "
      "%{if-debug}DEBG%{endif}"
      "%{if-info}INFO%{endif}"
      "%{if-warning}WARN%{endif}"
      "%{if-critical}CRIT%{endif}"
      "%{if-fatal}FATL%{endif}] "
      "- %{message}");
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("Main", "Main");

#ifdef SHOW_RESOURCES
  QDirIterator it(":", QDirIterator::Subdirectories);
  while (it.hasNext()) {
    qDebug() << it.next();
  }
#endif

  if (!engine.rootObjects().isEmpty()) {
    if (auto* window =
            qobject_cast<QQuickWindow*>(engine.rootObjects().first())) {
      new WindowHelper(window);
    }
  }

  return QGuiApplication::exec();
}
