#pragma once

#include <QDateTime>
#include <optional>

namespace Common {
class EndDateTimePoint;
}

namespace Common {

/// \brief дата время, обозначающее начало какого-то процесса (например, приёма)
class BeginDateTimePoint {
 public:
  explicit BeginDateTimePoint(QDateTime);

  /// \brief создание временной метки начала процесса с проверкой, что метка из
  /// будущего
  static std::optional<BeginDateTimePoint> fromFuture(QDateTime);
  /// \brief создание временной метки начала процесса с проверкой, что метка из
  /// прошлого
  static std::optional<BeginDateTimePoint> fromPast(QDateTime);

  [[nodiscard]] QDateTime base() const { return _base; }

  [[nodiscard]] std::weak_ordering operator<=>(const QDateTime &value) const;
  [[nodiscard]] std::weak_ordering operator<=>(
      const BeginDateTimePoint &) const;
  [[nodiscard]] std::weak_ordering operator<=>(const EndDateTimePoint &) const;
  [[nodiscard]] bool operator==(const BeginDateTimePoint &) const = default;

 private:
  QDateTime _base;
};

}  // namespace Common
