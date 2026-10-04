#include "begindatetimepoint.h"

#include <utility>

#include "enddatetimepoint.h"

namespace Common {

BeginDateTimePoint::BeginDateTimePoint(QDateTime base)
    : _base(std::move(base)) {}

std::optional<BeginDateTimePoint> BeginDateTimePoint::fromFuture(
    QDateTime base) {
  if (base < QDateTime::currentDateTime()) {
    return std::nullopt;
  }
  return BeginDateTimePoint(base);
}

std::weak_ordering BeginDateTimePoint::operator<=>(
    const QDateTime &value) const {
  return base() <=> value;
}

std::weak_ordering BeginDateTimePoint::operator<=>(
    const BeginDateTimePoint &) const = default;

std::weak_ordering BeginDateTimePoint::operator<=>(
    const EndDateTimePoint &value) const {
  return base() <=> value.base();
}

}  // namespace Common
