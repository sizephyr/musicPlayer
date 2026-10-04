#include "sexvalueobject.h"

#include <QString>

namespace {
constinit const auto maleString = "Мужской";
constinit const auto femaleString = "Женский";
}  // namespace

namespace Common {
QString toString(Sex sex) {
  return sex == Sex::Male ? maleString : femaleString;
}

std::optional<Sex> fromString(const QString &rawSex) {
  if (rawSex == maleString) {
    return Sex::Male;
  }
  if (rawSex == femaleString) {
    return Sex::Female;
  }
  return std::nullopt;
}
}  // namespace Common
