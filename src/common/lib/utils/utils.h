#pragma once

#include <QDate>
#include <QJsonObject>
#include <chrono>
#include <expected>
#include <filesystem>

namespace Utils {

namespace Chrono {
std::chrono::year_month_day today();
std::chrono::year_month_day toChrono(QDate);
}  // namespace Chrono

namespace JSON {
std::expected<QJsonObject, QJsonParseError> parseQJsonObject(
    const QByteArray& rawJSO);
std::expected<QJsonObject, QString> readJsonFile(
    const std::filesystem::path& filePath);
}  // namespace JSON

}  // namespace Utils
