#pragma once

#include <QNetworkRequest>

namespace Common {
/// \enum расширение QNetworkRequest::KnownHeaders для использования в сетевых
/// взаимодействиях
enum class HttpHeader {
  Accept = QNetworkRequest::NumKnownHeaders + 1,
  Authorization,
  Connection,
  ClientVersion,
};

/// \brief Приведение заголовка к сырой строке
///  для использования в методе QNetworkRequest::setRawHeader
[[nodiscard]] QByteArray toRaw(HttpHeader);
[[nodiscard]] QByteArray defaultHeaderValue(int);
[[nodiscard]] QByteArray jsonContentType();

}  // namespace Common
