#include "httpcode.h"

namespace Common {

std::expected<HttpCode, QNetworkReply::NetworkError> parseHttpCode(
    const QNetworkReply* reply) noexcept {
  if (reply == nullptr) {
    return std::unexpected(QNetworkReply::UnknownNetworkError);
  }

  // Проверяем наличие транспортной или сетевой ошибки Qt (например, DNS,
  // Timeout, Connection Refused)
  if (reply->error() != QNetworkReply::NoError &&
      reply->error() <= QNetworkReply::UnknownNetworkError) {
    return std::unexpected(reply->error());
  }

  const QVariant statusCode =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  if (!statusCode.isValid()) {
    return std::unexpected(reply->error() != QNetworkReply::NoError
                               ? reply->error()
                               : QNetworkReply::UnknownNetworkError);
  }

  return static_cast<HttpCode>(statusCode.toInt());
}

bool isUnexpectedCode(QNetworkReply* response, HttpCode expectedCode) {
  if (auto httpCodeExp = parseHttpCode(response);
      !httpCodeExp || httpCodeExp.value() != expectedCode) {
    if (httpCodeExp && httpCodeExp.value() != expectedCode) {
      return true;
    }
    return true;
  }

  return false;
}

}  // namespace Common
