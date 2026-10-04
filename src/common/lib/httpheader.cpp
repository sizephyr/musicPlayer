#include "httpheader.h"

#include <QString>

#include "version.h"

namespace {
namespace HeaderKey {
constinit const auto none = "None";
constinit const auto accept = "Accept";
constinit const auto authorization = "Authorization";
constinit const auto connection = "Connection";
constinit const auto сlientVersion = "Client-Version";
}  // namespace HeaderKey
namespace HeaderValue {
constinit const auto accept = "application/json";
constinit const auto jsonContentType = "application/json";
const auto userAgent = QStringLiteral("TerminalUnit").toUtf8();
}  // namespace HeaderValue
}  // namespace

namespace Common {

QByteArray toRaw(HttpHeader header) {
  using enum HttpHeader;

  switch (header) {
    case Accept:
      return HeaderKey::accept;
    case Authorization:
      return HeaderKey::authorization;
    case Connection:
      return HeaderKey::connection;
    case ClientVersion:
      return HeaderKey::сlientVersion;
    default:
      return HeaderKey::none;
  }
}

QByteArray defaultHeaderValue(int rawHeader) {
  switch (rawHeader) {
    case static_cast<int>(HttpHeader::Accept):
      return HeaderValue::accept;
    case QNetworkRequest::UserAgentHeader:
      return HeaderValue::userAgent;
    case QNetworkRequest::ContentTypeHeader:
      return jsonContentType();
    case static_cast<int>(HttpHeader::ClientVersion):
      return FULL_PROJECT_VERSION;
    default:
      return "";
  }
}

QByteArray jsonContentType() { return HeaderValue::jsonContentType; }

}  // namespace Common
