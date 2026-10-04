#include "utils.h"

namespace Utils {

std::chrono::year_month_day Chrono::today() {
  return std::chrono::year_month_day(
      {floor<std::chrono::days>(std::chrono::system_clock::now())});
}

std::chrono::year_month_day Chrono::toChrono(QDate qDate) {
  return std::chrono::year_month_day{
      std::chrono::year{qDate.year()},
      std::chrono::month{static_cast<unsigned>(qDate.month())},
      std::chrono::day{static_cast<unsigned>(qDate.day())}};
}

std::expected<QJsonObject, QJsonParseError> JSON::parseQJsonObject(
    const QByteArray &rawJSO) {
  QJsonParseError parseErr;
  QJsonObject result = QJsonDocument::fromJson(rawJSO, &parseErr).object();
  if (parseErr.error != QJsonParseError::NoError)
    return std::unexpected(parseErr);
  return result;
}

}  // namespace Utils
