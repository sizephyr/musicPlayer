#pragma once

#include "notifymanagernotifiesviewmodel.h"
#include <QObject>
#include <QQmlEngine>

class NotifyManagerModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(NotifyManagerNotifiesViewModel *viewModel READ viewModel NOTIFY
                 viewModelChanged)
  QML_ELEMENT
  QML_SINGLETON
public:
  explicit NotifyManagerModel(QObject *parent = nullptr);

  [[nodiscard]] NotifyManagerNotifiesViewModel *viewModel() const;

  Q_INVOKABLE void info(const QString &title, const QString &message,
                        bool autoHide = false);
  Q_INVOKABLE void warning(const QString &title, const QString &message,
                           bool autoHide = false);
  Q_INVOKABLE void error(const QString &title, const QString &message,
                         bool autoHide = false);

signals:
  void viewModelChanged();

private:
  NotifyManagerNotifiesViewModel *_viewModel{nullptr};
};
