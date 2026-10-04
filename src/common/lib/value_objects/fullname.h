#pragma once

#include <QMetaType>
#include <QString>
#include <optional>

#include "firstname.h"
#include "lastname.h"
#include "patronymic.h"

namespace Common {

class FullName {
  Q_GADGET
  Q_PROPERTY(QString lastName READ lastName CONSTANT)
  Q_PROPERTY(QString firstName READ firstName CONSTANT)
  Q_PROPERTY(QString patronymic READ patronymicOrEmptyString CONSTANT)

 public:
  FullName(LastName lastName, FirstName firstName,
           std::optional<Patronymic> patronymic = std::nullopt);

  [[nodiscard]] LastName lastName() const;
  [[nodiscard]] FirstName firstName() const;
  [[nodiscard]] std::optional<Patronymic> patronymic() const;
  /// \brief Хелпер, чтобы не возвращать std::optional в метасистему
  [[nodiscard]] QString patronymicOrEmptyString() const {
    return patronymic() ? static_cast<QString>(*patronymic()) : QString();
  }

  /// \brief возвращает инициалы пациента в формате "Фамилия И.О."
  /// \todo подумать насчёт форматирования инициалов, как есть форматирование в
  /// QDate::toString()
  [[nodiscard]] QString initials() const;
  [[nodiscard]] QString toString() const;

  explicit operator QString() const;

  auto operator<=>(const FullName &other) const = default;

  LastName _lastName;
  FirstName _firstName;
  std::optional<Patronymic> _patronymic;
};

}  // namespace Common
