#pragma once

#include <QQmlEngine>

namespace Common {
// неймспейс чисто для регистрации в метасистеме qml
namespace BaseButtonQmlRegisterNS {
Q_NAMESPACE
QML_FOREIGN_NAMESPACE(Common::BaseButtonQmlRegisterNS)
QML_NAMED_ELEMENT(BaseButtonStyle)
enum class BaseButtonStyle {
  Main,         // зелёная кнопка с белым текстом
  RedMain,      // белая кнопка с красными рамкой и текстом
  Alternative,  // белая кнопка с зелёными рамкой и текстом
  Red,          // красная кнопка для особо-важных действий
  Inverted      // стиль, обратный Main-стилю
};
Q_ENUM_NS(BaseButtonStyle)
}  // namespace BaseButtonQmlRegisterNS

// для доступа простых классов
using BaseButtonStyle = BaseButtonQmlRegisterNS::BaseButtonStyle;
}  // namespace Common
