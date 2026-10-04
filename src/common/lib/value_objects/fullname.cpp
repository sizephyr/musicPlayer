#include "fullname.h"

namespace Common {

FullName::FullName(LastName lastName, FirstName firstName,
                   std::optional<Patronymic> patronymic)
    : _lastName(std::move(lastName)),
      _firstName(std::move(firstName)),
      _patronymic(std::move(patronymic)) {}

LastName FullName::lastName() const { return _lastName; }

FirstName FullName::firstName() const { return _firstName; }

std::optional<Patronymic> FullName::patronymic() const { return _patronymic; }

QString FullName::initials() const {
  return _lastName + " " + _firstName.toString().first(1) + "." +
         (_patronymic ? (_patronymic->toString().first(1) + ".") : "");
}

QString FullName::toString() const { return static_cast<QString>(*this); }

FullName::operator QString() const {
  return _patronymic
             ? QString("%1 %2 %3").arg(_lastName, _firstName, *_patronymic)
             : QString("%1 %2").arg(_lastName, _firstName);
}

}  // namespace Common
