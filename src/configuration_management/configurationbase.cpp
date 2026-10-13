#include "configurationbase.h"

#include <utils/utils.h>

#include <QDir>
#include <QLoggingCategory>
#include <QStandardPaths>
#include <chrono>

#include "defaultconfig.h"

namespace {
const auto loggingCategory = QLoggingCategory("configuration.base");

constinit const auto defaultAnimationDuration = std::chrono::milliseconds(250);
constinit const auto defaultNotificationHideAfterMs =
    std::chrono::milliseconds(4000);
constinit const auto referenceDisplaySize = QSize(1000, 600);

constinit const auto applicationConfigFilename = "ApplicationConfig.json";
constinit const auto networkConfigFilename = "NetworkConfig.json";
constinit const auto unitConfigFilename = "UnitConfig.json";
constinit const auto windowNode = "window";

namespace Keys {
constinit const auto windowNode = "window";
constinit const auto animationNode = "animaltion";
constinit const auto networkNode = "network";
constinit const auto width = "width";
constinit const auto height = "height";
constinit const auto title = "title";
constinit const auto background = "background";
constinit const auto durationMs = "duration_ms";
constinit const auto notificationHideAfterMs = "notification_hide_after_ms";
constinit const auto baseTransferTimeoutMs = "base_transfer_timeout_ms";
}  // namespace Keys

auto appConfigLocation() {
  QString configDirStr =
      QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

  return QDir(configDirStr).filePath(applicationConfigFilename);
}
}  // namespace

namespace Configuration {

Base::Base(QObject* parent) : QObject(parent) { load(); }

void Base::setDisplaySize(QSize size) {
  _displaySize = size;
  emit scaleFactorChanged();
  emit displaySizeChanged();
}

void Base::load() {
  auto configPathExp = getApplicationConfigPath();

  // Если пути нет, создаем конфигурацию по умолчанию
  if (!configPathExp.has_value()) {
    if (auto configCreatingResult = makeDefaultApplicationConfig();
        !configCreatingResult) {
      throw std::runtime_error(
          "Не удалось создать конфигурацию приложения по умолчанию: " +
          configCreatingResult.error().toStdString());
    }

    // Пытаемся получить путь повторно после успешного создания файла
    configPathExp = getApplicationConfigPath();
    if (!configPathExp.has_value()) {
      throw std::runtime_error(
          "Критическая ошибка: Путь к конфигурации недоступен после попытки её "
          "создания.");
    }
  }

  const auto configPath = configPathExp.value();

  std::error_code ec;
  const auto canonicalPath = std::filesystem::canonical(configPath, ec);

  if (ec) {
    throw std::runtime_error(
        "Файл конфигурации не найден или недоступен по пути '" +
        configPath.string() + "'. Системная ошибка: " + ec.message());
  }

  auto rawConfigExp = Utils::JSON::readJsonFile(canonicalPath);
  if (!rawConfigExp) {
    throw std::runtime_error(QString("Невалидная конфигурация приложения: %1")
                                 .arg(rawConfigExp.error())
                                 .toStdString());
  }

  _rawApplicationConfig = rawConfigExp.value();
  qDebug() << "Конфигурация успешно загружена:" << _rawApplicationConfig;
}

std::expected<std::filesystem::path, QString> Base::getApplicationConfigPath() {
  if (!QFile::exists(appConfigLocation())) {
    return std::unexpected("Файл конфигурации приложения не найден по пути:" +
                           appConfigLocation());
  }

  return std::filesystem::path(appConfigLocation().toStdString());
}

std::expected<std::filesystem::path, QString> Base::getNetworkConfigPath() {
  QString configDirStr =
      QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

  QString configFilePath = QDir(configDirStr).filePath(networkConfigFilename);

  if (!QFile::exists(configFilePath)) {
    return std::unexpected("Файл конфигурации подключения не найден по пути:" +
                           configFilePath);
  }

  return std::filesystem::path(configFilePath.toStdString());
}

std::expected<void, QString> Base::makeDefaultApplicationConfig() {
  if (auto configObjExp = Utils::JSON::parseQJsonObject(Private::defaultConfig);
      !configObjExp) {
    return std::unexpected("Не удалось скомпоновать дефолтный конфиг: " +
                           configObjExp.error().errorString());
  } else if (auto writeResult = Utils::JSON::writeJsonToFile(  // NOLINT
                 configObjExp.value(),
                 std::filesystem::path(appConfigLocation().toStdString()));
             !writeResult) {
    return std::unexpected(writeResult.error());
  } else {  // NOLINT
    return {};
  }
}

std::optional<QJsonObject> Base::windowObject() const {
  if (_rawApplicationConfig.contains(Keys::windowNode) &&
      _rawApplicationConfig.value(Keys::windowNode).isObject()) {
    return _rawApplicationConfig.value(Keys::windowNode).toObject();
  }

  return std::nullopt;
}

std::optional<QJsonObject> Base::animationObject() const {
  if (_rawApplicationConfig.contains(Keys::animationNode) &&
      _rawApplicationConfig.value(Keys::animationNode).isObject()) {
    return _rawApplicationConfig.value(Keys::animationNode).toObject();
  }

  return std::nullopt;
}

std::optional<QJsonObject> Base::networkObject() const {
  if (_rawApplicationConfig.contains(Keys::networkNode) &&
      _rawApplicationConfig.value(Keys::networkNode).isObject()) {
    return _rawApplicationConfig.value(Keys::networkNode).toObject();
  }

  return std::nullopt;
}

int Base::referenceWindowWidth() const {
  if (auto windowObjectOpt = windowObject();
      windowObjectOpt.has_value() && windowObjectOpt->contains(Keys::width) &&
      windowObjectOpt->value(Keys::width).isDouble()) {
    return windowObjectOpt->value(Keys::width).toInt();
  }

  return {};  // этот метод вызывается в QML, поэтому не std::optional
}

int Base::referenceWindowHeight() const {
  if (auto windowObjectOpt = windowObject();
      windowObjectOpt.has_value() && windowObjectOpt->contains(Keys::height) &&
      windowObjectOpt->value(Keys::height).isDouble()) {
    return windowObjectOpt->value(Keys::height).toInt();
  }

  return {};  // этот метод вызывается в QML, поэтому не std::optional
}

QString Base::windowTitle() const {
  if (auto windowObjectOpt = windowObject();
      windowObjectOpt.has_value() && windowObjectOpt->contains(Keys::title) &&
      windowObjectOpt->value(Keys::title).isString()) {
    return windowObjectOpt->value(Keys::title).toString();
  }

  return {};  // этот метод вызывается в QML, поэтому не std::optional
}

int Base::animationDurationMs() const {
  if (auto animationObjectOpt = animationObject();
      animationObjectOpt.has_value() &&
      animationObjectOpt->contains(Keys::durationMs) &&
      animationObjectOpt->value(Keys::durationMs).isDouble()) {
    return animationObjectOpt->value(Keys::durationMs).toInt();
  }

  return defaultAnimationDuration
      .count();  // этот метод вызывается в QML, поэтому не std::optional
}

int Base::baseTransferTimeoutMs() const {
  if (auto networkObjectOpt = networkObject();
      networkObjectOpt.has_value() &&
      networkObjectOpt->contains(Keys::baseTransferTimeoutMs) &&
      networkObjectOpt->value(Keys::baseTransferTimeoutMs).isDouble()) {
    return networkObjectOpt->value(Keys::baseTransferTimeoutMs).toInt();
  }

  return {};  // этот метод вызывается в QML, поэтому не std::optional
}

int Base::notificationHideAfterMs() const {
  if (auto animationObjectOpt = animationObject();
      animationObjectOpt.has_value() &&
      animationObjectOpt->contains(Keys::durationMs) &&
      animationObjectOpt->value(Keys::durationMs).isDouble()) {
    return animationObjectOpt->value(Keys::notificationHideAfterMs).toInt();
  }

  return defaultNotificationHideAfterMs
      .count();  // этот метод вызывается в QML, поэтому не std::optional
}

QString Base::windowBackground() const {
  if (auto windowObjectOpt = windowObject();
      windowObjectOpt.has_value() &&
      windowObjectOpt->contains(Keys::background) &&
      windowObjectOpt->value(Keys::background).isString()) {
    return windowObjectOpt->value(Keys::background).toString();
  }

  return {};  // этот метод вызывается в QML, поэтому не std::optional
}

double Base::scaleFactor() const {
  auto average = [](const QSize& size) {
    return (static_cast<double>(size.height()) +
            static_cast<double>(size.width())) /
           2;
  };
  if (!_displaySize.has_value()) {
    return 1.0;
  }

  return average(displaySize()) / average(referenceDisplaySize);
}

Qt::WindowFlags Base::windowFlags() {
  {
#if defined(Q_OS_LINUX)
    return Qt::Window | Qt::FramelessWindowHint;
#elif defined(Q_OS_MAC)
    return Qt::Window;
#else
    return Qt::Window;
#endif
  }
}

QSize Base::displaySize() const { return _displaySize.value_or({}); }

}  // namespace Configuration