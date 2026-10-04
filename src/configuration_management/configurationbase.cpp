#include "configurationbase.h"

#include <QDir>
#include <QLoggingCategory>
#include <QStandardPaths>
#include <chrono>

namespace {
const auto loggingCategory = QLoggingCategory("configuration.base");

constinit const auto defaultAnimationDuration = std::chrono::milliseconds(250);
constinit const auto defaultNotificationHideAfterMs =
    std::chrono::milliseconds(400);
constinit const auto referenceDisplaySize = QSize(1080, 1920);

constinit const auto applicationConfigFilename = "ApplicationConfig.yaml";
constinit const auto networkConfigFilename = "NetworkConfig.yaml";
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
}  // namespace

namespace Configuration {

Base::Base(QObject* parent) : QObject{parent} { load(); }

void Base::setDisplaySize(int height, int width) {
  _displaySize = QSize(width, height);
  emit scaleFactorChanged();
}

void Base::load() {
  auto configPathExp = getApplicationConfigPath();
  if (!configPathExp.has_value()) {
    throw std::runtime_error("Не удалось загрузить конфигурацию приложения: " +
                             configPathExp.error().toStdString());
  }

  try {
    _applicationConfigRoot =
        YAML::LoadFile(std::filesystem::canonical(configPathExp.value()));
  } catch (const std::exception& e) {
    throw std::runtime_error(QString("Невалидная конфигурация приложения: %1")
                                 .arg(e.what())
                                 .toStdString());
  }
}

std::expected<std::filesystem::path, QString> Base::getApplicationConfigPath() {
#ifndef FORCED_APPLICATION_CONFIG_PATH
  QString configDirStr =
      QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

  QString configFilePath =
      QDir(configDirStr).filePath(applicationConfigFilename);

  if (!QFile::exists(configFilePath)) {
    return std::unexpected("Файл конфигурации приложения не найден по пути:" +
                           configFilePath);
  }

  return std::filesystem::path(configFilePath.toStdString());
#else
  return std::filesystem::path(FORCED_APPLICATION_CONFIG_PATH);
#endif
}

std::expected<std::filesystem::path, QString> Base::getNetworkConfigPath() {
#ifndef FORCED_NETWORK_CONFIG_PATH
  QString configDirStr =
      QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

  QString configFilePath = QDir(configDirStr).filePath(networkConfigFilename);

  if (!QFile::exists(configFilePath)) {
    return std::unexpected("Файл конфигурации подключения не найден по пути:" +
                           configFilePath);
  }

  return std::filesystem::path(configFilePath.toStdString());
#else
  return std::filesystem::path(FORCED_NETWORK_CONFIG_PATH);
#endif
}

std::expected<void, QString> Base::parseNetworkConfig(
    const YAML::Node& rawConfig) {
  return {};
}

int Base::windowWidth() const {
  try {
    static const auto width =
        _applicationConfigRoot[Keys::windowNode][Keys::width]  // NOLINT
            .as<int>();

    return width;
  } catch (const YAML::Exception& e) {
    throw std::runtime_error(
        "Конфигурация не содержит ожидаемого поля: window.width");
  }
}

int Base::windowHeight() const {
  try {
    static const auto height =
        _applicationConfigRoot[Keys::windowNode][Keys::height]  // NOLINT
            .as<int>();

    return height;
  } catch (const YAML::Exception& e) {
    throw std::runtime_error(
        "Конфигурация не содержит ожидаемого поля: window.height");
  }
}

QString Base::windowTitle() const {
  try {
    static const auto title =
        _applicationConfigRoot[Keys::windowNode][Keys::title]  // NOLINT
            .as<std::string>();

    return QString::fromStdString(title);
  } catch (const YAML::Exception& e) {
    throw std::runtime_error(
        "Конфигурация не содержит ожидаемого поля: window.title");
  }
}

int Base::animationDurationMs() const {
  try {
    static const auto height =
        _applicationConfigRoot[Keys::animationNode]          // NOLINT
                              [Keys::baseTransferTimeoutMs]  // NOLINT
                                  .as<int>();

    return height;
  } catch (const YAML::Exception& e) {
    return defaultAnimationDuration.count();
  }
}

int Base::baseTransferTimeoutMs() const {
  try {
    static const auto height =
        _applicationConfigRoot[Keys::networkNode]
                              [Keys::baseTransferTimeoutMs]  // NOLINT
                                  .as<int>();

    return height;
  } catch (const YAML::Exception& e) {
    throw std::runtime_error(
        "Конфигурация не содержит ожидаемого поля: "
        "network.base_transfer_timeout_ms");
  }
}

int Base::notificationHideAfterMs() const {
  try {
    static const auto height =
        _applicationConfigRoot[Keys::animationNode]            // NOLINT
                              [Keys::notificationHideAfterMs]  // NOLINT
                                  .as<int>();

    return height;
  } catch (const YAML::Exception& e) {
    return defaultNotificationHideAfterMs.count();
  }
}

QString Base::windowBackground() const {
  try {
    static const auto title =
        _applicationConfigRoot[Keys::windowNode][Keys::background]  // NOLINT
            .as<std::string>();

    return QString::fromStdString(title);
  } catch (const YAML::Exception& e) {
    qCInfo(loggingCategory)
        << "Конфигурация не содержит ожидаемого поля: window:background";
    return ":/qt/qml/Main/img/base_background.jpg";
  }
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

  static const auto factor =
      average(_displaySize.value()) / average(referenceDisplaySize);

  return factor;
}

}  // namespace Configuration
