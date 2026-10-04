#pragma once

#include <QDate>
#include <QJsonObject>
#include <chrono>
#include <expected>

namespace Utils {

namespace Chrono {
std::chrono::year_month_day today();
std::chrono::year_month_day toChrono(QDate);
}  // namespace Chrono

namespace JSON {
std::expected<QJsonObject, QJsonParseError> parseQJsonObject(
    const QByteArray &rawJSO);
}

}  // namespace Utils
