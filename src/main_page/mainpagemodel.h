#pragma once

#include <QObject>
#include <QQmlEngine>

namespace MainPage {

class Model : public QObject {
  Q_OBJECT
  QML_ELEMENT
 public:
  explicit Model(QObject* parent = nullptr);

 signals:
};

}  // namespace MainPage
