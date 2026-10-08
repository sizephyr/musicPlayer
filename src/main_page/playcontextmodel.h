#pragma once

#include <QObject>
#include <QQmlEngine>

class PlayContextModel : public QObject {
  Q_OBJECT
  QML_ELEMENT
 public:
  explicit PlayContextModel(QObject* parent = nullptr);

 signals:

 private:
  // double progress
};
