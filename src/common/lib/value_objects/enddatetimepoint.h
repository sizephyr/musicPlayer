#pragma once

#include <QDateTime>
#include <optional>

namespace Common {
class BeginDateTimePoint;
}

namespace Common {

/// \brief дата время, обозначающее окончание какого-то процесса (например,
/// приёма)
class EndDateTimePoint {
 public:
  using ErrorComment = QString;

  explicit EndDateTimePoint(QDateTime);

  /// \brief создание временной метки окончания процесса с проверкой, что метка
  /// из будущего
  static std::optional<EndDateTimePoint> fromFuture(QDateTime);
  /// \brief создание временной метки окончания процесса с проверкой, что метка
  /// из прошлого
  static std::optional<EndDateTimePoint> fromPast(QDateTime);

  [[nodiscard]] QDateTime base() const { return _base; }

  [[nodiscard]] std::weak_ordering operator<=>(const EndDateTimePoint &) const;
  [[nodiscard]] std::weak_ordering operator<=>(const QDateTime &value) const;
  [[nodiscard]] std::weak_ordering operator<=>(
      const BeginDateTimePoint &value) const;
  [[nodiscard]] bool operator==(const EndDateTimePoint &) const = default;

 private:
  QDateTime _base;
};

}  // namespace Common
