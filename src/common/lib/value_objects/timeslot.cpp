#include "timeslot.h"

namespace Common {

TimeSlot::TimeSlot(BeginDateTimePoint begin, EndDateTimePoint end)
    : _begin(std::move(begin)), _end(std::move(end)) {
  if (_begin > _end) {
    throw std::logic_error(
        "Время начала не может быть позже времени окончания");
  }
}

bool TimeSlot::hasOverlap(const TimeSlot& otherSlot) const {
  return *this == otherSlot ||
         // текущий начинается, когда другой ещё не закончен
         (begin() < otherSlot.end()) ||
         // другой начинается, когда текущий ещё не закончен
         (otherSlot.begin() < end());
}

std::optional<TimeSlot> TimeSlot::overlap(const TimeSlot& otherSlot) {
  if (!hasOverlap(otherSlot)) {
    return std::nullopt;
  }
  if (*this == otherSlot) {
    return *this;
  }
  if (begin() < otherSlot.end()) {
    return TimeSlot(begin(), otherSlot.end());
  }
  return TimeSlot(otherSlot.begin(), end());
}

std::chrono::milliseconds TimeSlot::duration() const {
  return std::chrono::milliseconds(_begin.base().msecsTo(_end.base()));
}

BeginDateTimePoint TimeSlot::begin() const { return _begin; }

EndDateTimePoint TimeSlot::end() const { return _end; }

std::expected<TimeSlot, TimeSlot::ErrorComment> TimeSlot::fromTimePoints(
    const BeginDateTimePoint& begin, const EndDateTimePoint& end) {
  try {
    return TimeSlot(begin, end);
  } catch (std::logic_error e) {
    return std::unexpected(e.what());
  }
}

}  // namespace Common
