#pragma once

#include <chrono>
#include <expected>

#include "begindatetimepoint.h"
#include "enddatetimepoint.h"

namespace Common {

/// \brief временной промежуток, отражающий продолжительность какого-то процесса
/// (например, приёма)
class TimeSlot {
 public:
  using ErrorComment = QString;

  /// \exception std::logic_error если время окончания раньше времени начала
  TimeSlot(BeginDateTimePoint begin, EndDateTimePoint end);
  /// \brief обёртка над конструктором, необходимая для того, чтобы предупредить
  /// исключение и вернуть его в качестве std::unexpected
  static std::expected<TimeSlot, ErrorComment> fromTimePoints(
      const BeginDateTimePoint&, const EndDateTimePoint&);

  [[nodiscard]] BeginDateTimePoint begin() const;
  [[nodiscard]] EndDateTimePoint end() const;

  /// \brief есть ли пересечение временных промежутков
  [[nodiscard]] bool hasOverlap(const TimeSlot&) const;
  std::optional<TimeSlot> overlap(const TimeSlot&);

  /// \brief продолжительность слота
  [[nodiscard]] std::chrono::milliseconds duration() const;

  [[nodiscard]] auto operator<=>(const TimeSlot&) const = default;
  [[nodiscard]] bool operator==(const TimeSlot&) const = default;

 private:
  BeginDateTimePoint _begin;
  EndDateTimePoint _end;
};

}  // namespace Common
