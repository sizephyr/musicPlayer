#pragma once

#include <QQmlEngine>

namespace Common {
namespace StatusLabelColorQMLRegistrationNS {
Q_NAMESPACE
QML_NAMED_ELEMENT(StatusLabelColor)
/// \brief справа на короткой карточке есть небольшие лейблы. Их цвет задаётся
/// посредством этого перечисления
/// \note при добавлении значений, необходимо так же добавить соотношение
/// значений перечисления к цвету в UI
enum StatusLabelColor {
  Gray = Qt::gray,
  LightGreen = Qt::green,
  LightYellow = Qt::yellow,
  LightRed = Qt::red,
};
Q_ENUM_NS(StatusLabelColor)
}  // namespace StatusLabelColorQMLRegistrationNS
using StatusLabelColor = StatusLabelColorQMLRegistrationNS::StatusLabelColor;

struct StatusLabel {
  StatusLabelColor _color{StatusLabelColor::Gray};
  QString _text;
};
}  // namespace Common