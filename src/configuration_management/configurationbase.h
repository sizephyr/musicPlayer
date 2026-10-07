#pragma once

#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QSize>
#include <expected>
#include <filesystem>

namespace Configuration {

class Base : public QObject {
  Q_OBJECT
  Q_PROPERTY(double scaleFactor READ scaleFactor NOTIFY scaleFactorChanged)
  QML_NAMED_ELEMENT(ConfigurationBase)
 public:
  explicit Base(QObject* parent = nullptr);

  Q_INVOKABLE void setDisplaySize(int height, int width);
  [[nodiscard]] Q_INVOKABLE int windowWidth() const;
  [[nodiscard]] Q_INVOKABLE int windowHeight() const;
  [[nodiscard]] Q_INVOKABLE QString windowTitle() const;
  [[nodiscard]] Q_INVOKABLE int animationDurationMs() const;
  [[nodiscard]] Q_INVOKABLE int baseTransferTimeoutMs() const;
  [[nodiscard]] Q_INVOKABLE int notificationHideAfterMs() const;
  [[nodiscard]] Q_INVOKABLE QString windowBackground() const;

  [[nodiscard]] double scaleFactor() const;

 signals:
  void connectionConfiguredChanged();
  void scaleFactorChanged();

 private:
  void load();
  [[nodiscard]] static std::expected<std::filesystem::path, QString>
  getApplicationConfigPath();
  [[nodiscard]] static std::expected<std::filesystem::path, QString>
  getNetworkConfigPath();

  [[nodiscard]] std::optional<QJsonObject> windowObject() const;
  [[nodiscard]] std::optional<QJsonObject> animationObject() const;
  [[nodiscard]] std::optional<QJsonObject> networkObject() const;

  std::optional<QSize> _displaySize;
  QJsonObject _rawApplicationConfig;
};

}  // namespace Configuration
