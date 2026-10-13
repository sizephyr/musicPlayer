#pragma once

#include <QObject>
#include <QQuickWindow>

class WindowHelper : public QObject {
  Q_OBJECT
 public:
  explicit WindowHelper(QQuickWindow* window);

 private slots:
  void handleWindowStateChanged(Qt::WindowState state);

 private:
  void applyMacTransparentTitlebar();
  QQuickWindow* m_window;
};
