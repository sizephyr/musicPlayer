#pragma once

#include <QQmlEngine>
#include <QString>
#include <optional>

class QString;

namespace Common {
Q_NAMESPACE
QML_FOREIGN_NAMESPACE(Common)
QML_NAMED_ELEMENT(Sex)
enum class Sex {
  Undefined,
  Male,
  Female,
};
Q_ENUM_NS(Sex)
QString toString(Sex sex);
std::optional<Sex> fromString(const QString &rawSex);
}; // namespace Common
