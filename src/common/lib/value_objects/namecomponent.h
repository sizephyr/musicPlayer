#pragma once

#include <QDebug>
#include <QString>
#include <optional>

namespace Common {
/// \brief Базовый класс для строковых Value Object
template <typename T>
class NameComponent {
 public:
  explicit NameComponent(QString value) : _value(std::move(value).trimmed()) {
    if (_value.simplified().isEmpty()) {
      throw std::invalid_argument("Name component cannot be empty.");
    }
  }
  NameComponent(const char *value)
      : NameComponent(QString::fromUtf8(value).trimmed()) {}
  static std::optional<T> fromQString(const QString &rawValue) {
    try {
      return static_cast<T>(NameComponent<T>(rawValue));
    } catch (std::invalid_argument e [[maybe_unused]]) {
      qWarning() << e.what();
      return std::nullopt;
    }
  }

  operator QString() const { return _value; }
  [[nodiscard]] QString toString() const { return _value; }

  /// \note QString оказывается не имеет operator<=>
  [[nodiscard]] std::strong_ordering operator<=>(
      const NameComponent &other) const {
    if (_value == other._value) return std::strong_ordering::equal;
    return _value < other._value ? std::strong_ordering::less
                                 : std::strong_ordering::greater;
  }
  /// \note Для C++23 также обязательно явно определить оператор равенства,
  /// чтобы компилятор мог генерировать оптимальный код для == и !=.
  /// Это позволит ему просто сравнить длинну строк, чтобы понять, что они не
  /// эквивалентны
  [[nodiscard]] bool operator==(const NameComponent &other) const {
    return _value == other._value;
  }

 private:
  QString _value;
};

}  // namespace Common
