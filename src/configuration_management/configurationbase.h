#pragma once

#include <QQmlEngine>
#include <QSettings>
#include <QSize>
#include <expected>
#include <filesystem>

namespace Configuration {

class Base : public QObject {
  Q_OBJECT
  QML_NAMED_ELEMENT(ConfigurationBase)

  Q_PROPERTY(QSize displaySize READ displaySize WRITE setDisplaySize NOTIFY
                 displaySizeChanged)
  Q_PROPERTY(QString applicationVersion READ applicationVersion CONSTANT)
  Q_PROPERTY(double scaleFactor READ scaleFactor NOTIFY scaleFactorChanged)
  Q_PROPERTY(int referenceWindowWidth READ referenceWindowWidth CONSTANT)
  Q_PROPERTY(int referenceWindowHeight READ referenceWindowHeight CONSTANT)
  Q_PROPERTY(int animationDurationMs READ animationDurationMs CONSTANT)
  Q_PROPERTY(int notificationHideAfterMs READ notificationHideAfterMs CONSTANT)
  Q_PROPERTY(QString windowBackground READ windowBackground WRITE
                 setWindowBackground NOTIFY windowBackgroundChanged)
  Q_PROPERTY(Qt::WindowFlags windowFlags READ windowFlags CONSTANT)
  Q_PROPERTY(
      double backgroundTransparentnesRatio READ backgroundTransparentnesRatio
          WRITE setBackgroundTransparentnesRatio NOTIFY
              backgroundTransparentnesRatioChanged)
  Q_PROPERTY(double volumeRatio READ volumeRatio WRITE setVolumeRatio NOTIFY
                 volumeRatioChanged)

 public:
  explicit Base(QObject* parent = nullptr);

  [[nodiscard]] QString applicationVersion() const;
  [[nodiscard]] int referenceWindowWidth() const;
  [[nodiscard]] int referenceWindowHeight() const;
  [[nodiscard]] int animationDurationMs() const;
  [[nodiscard]] int notificationHideAfterMs() const;
  [[nodiscard]] QString windowBackground() const;
  [[nodiscard]] QSize displaySize() const;
  [[nodiscard]] double scaleFactor() const;
  [[nodiscard]] static Qt::WindowFlags windowFlags();
  [[nodiscard]] double backgroundTransparentnesRatio() const;
  [[nodiscard]] double volumeRatio() const;

  void setDisplaySize(QSize);
  void setBackgroundTransparentnesRatio(
      double newBackgroundTransparentnesRatio);
  void setVolumeRatio(double newVolumeRatio);
  void setWindowBackground(const QString& newWindowBackground);

 signals:
  void connectionConfiguredChanged();
  void scaleFactorChanged();
  void displaySizeChanged();
  void backgroundTransparentnesRatioChanged();
  void volumeRatioChanged();
  void windowBackgroundChanged();

 private:
  [[nodiscard]] static std::expected<void, QString> loadDefaultConfig(
      const std::filesystem::path&);

  [[nodiscard]] static std::expected<void, QString>
  makeDefaultApplicationConfig();

  /// \brief лок контекстной групы, для того, чтобы не прописывать пути
  /// полностью
  [[nodiscard]] auto setContextGroup(const auto& groupName) const {
    _settings->beginGroup(groupName);
    return QScopeGuard([this] { _settings->endGroup(); });
  }
  void writeInContext(const QString& context, const QString& key,
                      const QVariant& value);
  [[nodiscard]] QVariant readFromContext(const QString& context,
                                         const QString& key,
                                         const QVariant& defaultValue) const;

  std::optional<QSize> _displaySize;
  const QSettings::Format _format;
  QSettings* _settings;
};

namespace Private {
class QSettingsHelper {
  friend class Configuration::Base;
  [[nodiscard]] static bool read(QIODevice& device,
                                 QSettings::SettingsMap& map);

  [[nodiscard]] static bool write(QIODevice& device,
                                  const QSettings::SettingsMap& map);
};

}  // namespace Private

}  // namespace Configuration
