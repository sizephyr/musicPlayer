#include "configurationbase.h"

#include <utils/utils.h>

#include <QDir>
#include <QGuiApplication>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QStandardPaths>
#include <chrono>

#include "defaultconfig.h"
#include "version.h"

namespace {
const auto loggingCategory = QLoggingCategory("configuration.base");

namespace DefaultValues {
constinit const auto animationDuration = std::chrono::milliseconds(250);
constinit const auto notificationHideAfterMs = std::chrono::milliseconds(4000);
constinit const auto referenceDisplaySize = QSize(1000, 600);
constinit const auto backgroundTransparentnesRatio = 0.6;
constinit const auto volumeRatio = 0.25;
constinit const auto showBackground = true;
}  // namespace DefaultValues

constinit const auto applicationConfigFilename = "ApplicationConfig.json";
constinit const auto networkConfigFilename = "NetworkConfig.json";
constinit const auto unitConfigFilename = "UnitConfig.json";

namespace Keys {
constinit const auto coreNode = "core";
constinit const auto windowNode = "window";
constinit const auto animationNode = "animaltion";
constinit const auto networkNode = "network";
constinit const auto themeNode = "theme";
constinit const auto width = "width";
constinit const auto height = "height";
constinit const auto title = "title";
constinit const auto background = "background";
constinit const auto showBackground = "show_background";
constinit const auto durationMs = "duration_ms";
constinit const auto notificationHideAfterMs = "notification_hide_after_ms";
constinit const auto baseTransferTimeoutMs = "base_transfer_timeout_ms";
constinit const auto backgroundTransparentnesRatio =
    "background_transparentnes_ratio";
constinit const auto configLoaded = "config_loaded";
constinit const auto volumeRatio = "volume_ratio";
}  // namespace Keys

void jsonToMap(const QJsonObject& obj, QSettings::SettingsMap& map,
               const QString& prefix = {}) {
  for (auto it = obj.begin(); it != obj.end(); ++it) {
    const QString key = prefix.isEmpty() ? it.key() : prefix + u'/' + it.key();
    if (it.value().isObject()) {
      jsonToMap(it.value().toObject(), map, key);
    } else {
      map.insert(key, it.value().toVariant());
    }
  }
}

void mapToJson(QJsonObject& obj, QStringView key, const QVariant& value) {
  const int slashIdx = key.indexOf(u'/');
  if (slashIdx == -1) {
    obj.insert(key.toString(), QJsonValue::fromVariant(value));
    return;
  }

  const QString topKey = key.left(slashIdx).toString();
  const QStringView subKey = key.mid(slashIdx + 1);

  QJsonObject subObj = obj.value(topKey).toObject();
  mapToJson(subObj, subKey, value);
  obj.insert(topKey, subObj);
}

}  // namespace

namespace Configuration {

Base::Base(QObject* parent)
    : QObject(parent),
      _format(QSettings::registerFormat("json", &Private::QSettingsHelper::read,
                                        &Private::QSettingsHelper::write)),
      _settings(new QSettings(_format, QSettings::UserScope,
                              QGuiApplication::organizationName(),
                              QGuiApplication::applicationName(), this)) {
  qCInfo(loggingCategory).noquote()
      << "Конфигурация будет загружена из фала" << _settings->fileName();

  if (!readFromContext(Keys::coreNode, Keys::configLoaded, false).toBool()) {
    if (auto loadingResult =
            loadDefaultConfig(_settings->fileName().toStdString());
        !loadingResult) {
      throw std::runtime_error(
          QStringLiteral("Не удалось загрузить конфигурацию по-умлочанию: %1")
              .arg(loadingResult.error())
              .toStdString());
    }
    qCInfo(loggingCategory) << "Загружена конфигурация по-умолчанию";
  }

#ifdef Q_OS_MAC
  connect(this, &Base::showBackgroundChanged, this, &Base::windowFlagsChanged);
#endif
}

void Base::setDisplaySize(QSize size) {
  _displaySize = size;
  emit scaleFactorChanged();
  emit displaySizeChanged();
}

std::expected<void, QString> Base::loadDefaultConfig(
    const std::filesystem::path& configPath) {
  if (auto defaultConfigExp =
          Utils::JSON::parseQJsonObject(Configuration::Private::defaultConfig);
      !defaultConfigExp) {
    return std::unexpected(defaultConfigExp.error().errorString());
  } else {
    return Utils::JSON::writeJsonToFile(defaultConfigExp.value(), configPath);
  }
}

bool Private::QSettingsHelper::read(QIODevice& device,
                                    QSettings::SettingsMap& map) {
  const auto data = device.readAll();
  QJsonParseError error;
  const auto doc = QJsonDocument::fromJson(data, &error);
  if (error.error != QJsonParseError::NoError || !doc.isObject()) {
    return false;
  }

  jsonToMap(doc.object(), map);
  return true;
}

bool Private::QSettingsHelper::write(QIODevice& device,
                                     const QSettings::SettingsMap& map) {
  QJsonObject root;
  for (auto it = map.begin(); it != map.end(); ++it) {
    mapToJson(root, it.key(), it.value());
  }

  const QJsonDocument doc(root);
  return device.write(doc.toJson(QJsonDocument::Indented)) != -1;
}

void Base::writeInContext(const QString& context, const QString& key,
                          const QVariant& value) {
  const auto cleanup = setContextGroup(context);
  _settings->setValue(key, value);
}

QVariant Base::readFromContext(const QString& context, const QString& key,
                               const QVariant& defaultValue) const {
  const auto cleanup = setContextGroup(context);
  return _settings->value(key, defaultValue);
}

int Base::referenceWindowWidth() const {
  return readFromContext(Keys::windowNode, Keys::width,
                         DefaultValues::referenceDisplaySize.width())
      .toInt();
}

int Base::referenceWindowHeight() const {
  return readFromContext(Keys::windowNode, Keys::height,
                         DefaultValues::referenceDisplaySize.height())
      .toInt();
}

int Base::animationDurationMs() const {
  return readFromContext(
             Keys::animationNode, Keys::durationMs,
             QVariant::fromValue(DefaultValues::animationDuration.count()))
      .toInt();
}

int Base::notificationHideAfterMs() const {
  return readFromContext(Keys::animationNode, Keys::notificationHideAfterMs,
                         QVariant::fromValue(
                             DefaultValues::notificationHideAfterMs.count()))
      .toInt();
}

double Base::backgroundTransparentnesRatio() const {
  return readFromContext(Keys::themeNode, Keys::backgroundTransparentnesRatio,
                         DefaultValues::backgroundTransparentnesRatio)
      .toDouble();
}

void Base::setBackgroundTransparentnesRatio(
    double newBackgroundTransparentnesRatio) {
  const auto truncateToHundredths = [](double value) {
    return std::trunc(value * 100.0) / 100.0;
  };

  if (qFuzzyCompare(backgroundTransparentnesRatio(),
                    truncateToHundredths(newBackgroundTransparentnesRatio))) {
    return;
  }

  writeInContext(Keys::themeNode, Keys::backgroundTransparentnesRatio,
                 truncateToHundredths(newBackgroundTransparentnesRatio));

  emit backgroundTransparentnesRatioChanged();
}

QString Base::windowBackground() const {
  return readFromContext(Keys::windowNode, Keys::background, "").toString();
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

  return average(displaySize()) / average(DefaultValues::referenceDisplaySize);
}

Qt::WindowFlags Base::windowFlags() const {
  {
#if defined(Q_OS_LINUX)
    return Qt::Window | Qt::FramelessWindowHint;
#elif defined(Q_OS_MAC)
    if (showBackground()) {
      return Qt::Window;
    } else {
      return Qt::Window | Qt::FramelessWindowHint;
    }
#else
    return Qt::Window;
#endif
  }
}

QSize Base::displaySize() const { return _displaySize.value_or({}); }

QString Base::applicationVersion() const {
  // константа NONE определена в cmake/define_version_h.cmake как признак сборки
  // не из репо
  if (QString::compare(GIT_COMMIT_HASH, "NONE") != 0) {
    return QStringLiteral("%1.%2.%3 - %4")
        .arg(FULL_PROJECT_VERSION, RELEASE_CHANNEL, BUILD_TYPE,
             GIT_COMMIT_HASH);
  } else {
    return QStringLiteral("%1.%2").arg(FULL_PROJECT_VERSION, RELEASE_CHANNEL);
  }
}

double Base::volumeRatio() const {
  return readFromContext(Keys::coreNode, Keys::volumeRatio,
                         DefaultValues::volumeRatio)
      .toDouble();
}

void Base::setVolumeRatio(double newVolumeRatio) {
  const auto truncateToHundredths = [](double value) {
    return std::trunc(value * 100.0) / 100.0;
  };

  if (qFuzzyCompare(volumeRatio(), truncateToHundredths(newVolumeRatio))) {
    return;
  }

  writeInContext(Keys::coreNode, Keys::volumeRatio,
                 truncateToHundredths(newVolumeRatio));

  emit volumeRatioChanged();
}

void Base::setWindowBackground(const QString& newWindowBackground) {
  auto filename = QUrl(newWindowBackground).toLocalFile();
  if (windowBackground() == filename) {
    return;
  }

  writeInContext(Keys::windowNode, Keys::background, filename);

  emit windowBackgroundChanged();
}

bool Base::showBackground() const {
  return readFromContext(Keys::windowNode, Keys::showBackground,
                         DefaultValues::showBackground)
      .toBool();
}

void Base::setShowBackground(bool newShowBackground) {
  if (showBackground() == newShowBackground) {
    return;
  }

  writeInContext(Keys::windowNode, Keys::showBackground, newShowBackground);
  emit showBackgroundChanged();
}

}  // namespace Configuration