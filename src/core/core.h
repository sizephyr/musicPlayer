#pragma once

#include <pagemanager.h>

#include <QQmlEngine>
#include <QQuickItem>

class Core : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

 public:
  explicit Core(QQuickItem* parent = nullptr);

 public slots:  // NOLINT
  /// \brief для передачи QML-ного синглтона в плюсовый контекст
  Q_INVOKABLE void setPageManager(PageManagement::PageManager*);

 private:
  PageManagement::PageManager* _pageManager{nullptr};
};
