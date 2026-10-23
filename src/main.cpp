#include <QGuiApplication>
#include <QQmlApplicationEngine>
#ifdef SHOW_RESOURCES
#include <QDirIterator>
#endif

#include "logger.h"
#include "macoswindowhelper.h"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);
  QGuiApplication::setOrganizationName("com.sizephyr.soft");
  QGuiApplication::setApplicationName("music_player");
  Logger::i();  // иницилизация логгера

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

#ifdef Q_OS_MAC
  if (!engine.rootObjects().isEmpty()) {
    if (auto* window =
            qobject_cast<QQuickWindow*>(engine.rootObjects().first())) {
      new MacOSWindowHelper(window);
    }
  }
#endif

  return QGuiApplication::exec();
}
