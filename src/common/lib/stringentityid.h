#pragma once

#include <QDebug>
#include <QHash>
#include <QRegularExpression>
#include <QString>
#include <QStringView>
#include <QVariant>
#include <functional>
#include <optional>
#include <stdexcept>
#include <utility>

namespace Common {
/**
 * \note AI-gen описание
 *
 * \brief Шаблон для создания строго типизированных строковых идентификаторов
 * (Strong ID) с опциональной валидацией.
 *
 * Класс защищает от непреднамеренного перепутывания или сравнения логически
 * разных типов идентификаторов (например, СНИЛС и Полис) на этапе компиляции, а
 * также гарантирует корректность формата данных.
 *
 * ### Основные возможности:
 * 1. **Строгая типизация:** Невозможно присвоить или сравнить разные сущности,
 * даже если внутри они являются строками.
 * 2. **Компиляционная валидация (C++20/23 Концепты):** Если структура-тег
 * (`Tag`) содержит статическое поле `pattern`, включается автоматическая
 * проверка строки по регулярному выражению.
 * 3. **Безопасное создание:** Наличие методов `fromString` возвращающих
 * `std::optional` для обработки сырых данных без исключений.
 * 4. **Современные стандарты:** Использование оператора трехстороннего
 * сравнения (`<=>`) из C++20/23 и легковесных `QStringView` из Qt 6.
 *
 * ### Пример использования без валидации:
 * \code
 * using UserId = Common::StringEntityId<struct UserIdTag>;
 * using OrderId = Common::StringEntityId<struct OrderIdTag>;
 *
 * UserId user{"123"};
 * OrderId order{"123"};
 *
 * // if (user == order) {} // Ошибка компиляции! Разные типы.
 * \endcode
 *
 * ### Пример использования с валидацией:
 * \code
 * struct SnilsTag {
 *     static constexpr QStringView pattern = uR"(^\d{3}-\d{3}-\d{3}\s\d{2}\$)";
 * // Формат: 123-456-789 00
 * };
 * using Snils = Common::StringEntityId<SnilsTag>;
 *
 * // 1. Прямое создание (вызовет std::invalid_argument при ошибке)
 * try {
 *     Snils validSnils{"111-222-333 44"};
 *     // Snils invalidSnils{"wrong-format"}; // Бросит исключение!
 * } catch (const std::invalid_argument& e) { ... }
 *
 * // 2. Безопасный парсинг (без исключений)
 * auto parsed = Snils::fromString("111-222-333 44");
 * if (parsed) {
 *     qDebug() << "Успешно создан СНИЛС:" << *parsed;
 * }
 * \endcode
 */

// Концепт проверяет, содержит ли тег статическое регулярное выражение
template <typename T>
concept HasPattern = requires {
  { T::pattern } -> std::convertible_to<QStringView>;
};

template <typename Tag>
struct StringEntityId {
  QString value;

  // Конструкторы выполняют валидацию
  explicit StringEntityId(QString val) : value(std::move(val)) {
    validate(value);
  }

  explicit StringEntityId(QStringView val) : value(val.toString()) {
    validate(value);
  }

  explicit StringEntityId(const std::string& val)
      : value(QString::fromStdString(val)) {
    validate(value);
  }

  [[nodiscard]] static std::optional<StringEntityId> fromString(
      const std::string& val) noexcept {
    QString qstr = QString::fromStdString(val);
    if (isValid(qstr)) {
      return StringEntityId(std::move(qstr), InternalConstruct{});
    }
    return std::nullopt;
  }

  /// Статический метод для безопасного создания (std::optional)
  [[nodiscard]] static std::optional<StringEntityId> fromString(
      const QString& val) noexcept {
    if (isValid(val)) {
      return StringEntityId(val, InternalConstruct{});
    }
    return std::nullopt;
  }

  [[nodiscard]] static std::optional<StringEntityId> fromString(
      QStringView val) noexcept {
    if (isValid(val)) {
      return StringEntityId(val.toString(), InternalConstruct{});
    }
    return std::nullopt;
  }

  [[nodiscard]] auto operator<=>(const StringEntityId&) const = default;

  friend QDebug operator<<(QDebug debug, const StringEntityId& identifier) {
    QDebugStateSaver saver(debug);
    debug.nospace() << "StringEntityId(" << identifier.value << ")";
    return debug;
  }

  [[nodiscard]] operator QVariant() const {  // NOLINT
    return QVariant::fromValue(value);
  }
  [[nodiscard]] explicit operator QString() const { return value; }

  /// \brief Проверяет соответствие строки регулярному выражению тега без
  /// создания экземпляра класса. Доступен только если для Tag задан pattern.
  [[nodiscard]] static bool checkPattern(QStringView val) noexcept
    requires HasPattern<Tag>
  {
    return isValid(val);
  }

 private:
  struct InternalConstruct {};
  StringEntityId(QString val, [[maybe_unused]] InternalConstruct)  // NOLINT
      : value(std::move(val)) {}

  // Создаем QRegularExpression только если у тега есть паттерн
  [[nodiscard]] static const QRegularExpression& regex()
    requires HasPattern<Tag>
  {
    static const QRegularExpression regularExpression(Tag::pattern.toString());
    return regularExpression;
  }

  [[nodiscard]] static bool isValid(QStringView val) noexcept {
    if constexpr (!HasPattern<Tag>) {
      return true;  // Нет паттерна — всегда валидно
    } else {
      return regex().matchView(val).hasMatch();
    }
  }

  static void validate(QStringView val) {
    if (!isValid(val)) {
      throw std::invalid_argument(
          "StringEntityId validation failed: input does not match the required "
          "pattern.");
    }
  }
};

}  // namespace Common

template <typename Tag>
struct std::hash<Common::StringEntityId<Tag>> {
  size_t operator()(
      const Common::StringEntityId<Tag>& identifier) const noexcept {
    return qHash(identifier.value);
  }
};
