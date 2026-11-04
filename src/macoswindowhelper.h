#pragma once

#include <QtGlobal>

#ifdef Q_OS_MAC

#include <QObject>
#include <QQuickWindow>

class MacOSWindowHelper : public QObject {
  Q_OBJECT
 public:
  explicit MacOSWindowHelper(QQuickWindow* window);

 private slots:
  void handleWindowStateChanged(Qt::WindowState state);
  void handleWindowFlagsChanged(Qt::WindowFlags flags);

 private:
  void applyMacTransparentTitlebar();
  QQuickWindow* _window;
};

#endif