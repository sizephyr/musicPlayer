#include "enddatetimepoint.h"

#include <utility>

#include "begindatetimepoint.h"

namespace Common {

EndDateTimePoint::EndDateTimePoint(QDateTime base) : _base(std::move(base)) {}

std::optional<EndDateTimePoint> EndDateTimePoint::fromFuture(QDateTime base) {
  if (base < QDateTime::currentDateTime()) {
    return std::nullopt;
  }
  return EndDateTimePoint(base);
}

std::weak_ordering EndDateTimePoint::operator<=>(const QDateTime &value) const {
  return base() <=> value;
}

std::weak_ordering EndDateTimePoint::operator<=>(
    const EndDateTimePoint &) const = default;

std::weak_ordering EndDateTimePoint::operator<=>(
    const BeginDateTimePoint &value) const {
  return base() <=> value.base();
}

}  // namespace Common
