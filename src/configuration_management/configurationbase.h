#pragma once

#include <yaml-cpp/yaml.h>

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

  [[nodiscard]] bool connectionConfigured() const;
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
  [[nodiscard]] static std::expected<void, QString> parseNetworkConfig(
      const YAML::Node&);

  std::optional<QSize> _displaySize;
  YAML::Node _applicationConfigRoot;
};

}  // namespace Configuration
