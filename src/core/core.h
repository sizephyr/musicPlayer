#pragma once

#include <pagemanager.h>

#include <QQmlEngine>
#include <QQuickItem>

class MainMenuItemsModel;

class Core : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(bool connectionConfigured READ connectionConfigured WRITE
                 setConfiguredState NOTIFY connectionConfiguredChanged)
  QML_ELEMENT
 public:
  explicit Core(QQuickItem* parent = nullptr);

  [[nodiscard]] bool connectionConfigured() const;

 signals:
  void connectionConfiguredChanged();

 public slots:  // NOLINT
  /// \brief для передачи QML-ного синглтона в плюсовый контекст
  Q_INVOKABLE void setPageManager(PageManagement::PageManager*);
  Q_INVOKABLE void setConfiguredState(bool);

 private:
  PageManagement::PageManager* _pageManager{nullptr};
  bool _configured{false};
};
