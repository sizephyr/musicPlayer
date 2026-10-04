#pragma once

#include <QNetworkReply>
#include <expected>

namespace Common {
/// \enum Информационные, успешные, редиректы, клиентские и серверные HTTP-коды
/// ответов (RFC 9110) самые часто-используемые. см. \link
/// https://datatracker.ietf.org/doc/html/rfc9110
enum class HttpCode : int {
  // Successful 2xx
  Ok = 200,
  Created = 201,
  Accepted = 202,
  NoContent = 204,
  PartialContent = 206,

  // Redirection 3xx
  MovedPermanently = 301,
  Found = 302,
  SeeOther = 303,
  NotModified = 304,
  TemporaryRedirect = 307,
  PermanentRedirect = 308,

  // Client Error 4xx
  BadRequest = 400,
  Unauthorized = 401,
  Forbidden = 403,
  NotFound = 404,
  MethodNotAllowed = 405,
  NotAcceptable = 406,
  RequestTimeout = 408,
  Conflict = 409,
  Gone = 410,
  ContentTooLarge = 413,  // RFC 9110 (бывший Payload Too Large)
  UnsupportedMediaType = 415,
  UnprocessableContent = 422,  // RFC 9110 (бывший Unprocessable Entity)
  TooManyRequests = 429,

  // Server Error 5xx
  InternalServerError = 500,
  NotImplemented = 501,
  BadGateway = 502,
  ServiceUnavailable = 503,
  GatewayTimeout = 504
};

/**
 \brief Извлекает HTTP-код из ответа сети.
 \param Указатель на завершённый QNetworkReply.
 \return std::expected с HttpCode в случае успеха, либо с
 QNetworkReply::NetworkError при сетевой ошибке.
 */
[[nodiscard]] std::expected<HttpCode, QNetworkReply::NetworkError>
parseHttpCode(const QNetworkReply* reply) noexcept;
[[nodiscard]] bool isUnexpectedCode(QNetworkReply* response,
                                    HttpCode expectedCode);

}  // namespace Common
