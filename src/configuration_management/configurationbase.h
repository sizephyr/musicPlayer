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
  Q_PROPERTY(QSize displaySize READ displaySize WRITE setDisplaySize NOTIFY
                 displaySizeChanged)
  Q_PROPERTY(double scaleFactor READ scaleFactor NOTIFY scaleFactorChanged)
  QML_NAMED_ELEMENT(ConfigurationBase)
 public:
  explicit Base(QObject* parent = nullptr);

  void setDisplaySize(QSize);
  [[nodiscard]] Q_INVOKABLE int windowWidth() const;
  [[nodiscard]] Q_INVOKABLE int windowHeight() const;
  [[nodiscard]] Q_INVOKABLE QString windowTitle() const;
  [[nodiscard]] Q_INVOKABLE int animationDurationMs() const;
  [[nodiscard]] Q_INVOKABLE int baseTransferTimeoutMs() const;
  [[nodiscard]] Q_INVOKABLE int notificationHideAfterMs() const;
  [[nodiscard]] Q_INVOKABLE QString windowBackground() const;

  [[nodiscard]] double scaleFactor() const;
  [[nodiscard]] QSize displaySize() const;

 signals:
  void connectionConfiguredChanged();
  void scaleFactorChanged();
  void displaySizeChanged();

 private:
  void load();
  [[nodiscard]] static std::expected<std::filesystem::path, QString>
  getApplicationConfigPath();
  [[nodiscard]] static std::expected<std::filesystem::path, QString>
  getNetworkConfigPath();
  [[nodiscard]] static std::expected<void, QString>
  makeDefaultApplicationConfig();

  [[nodiscard]] std::optional<QJsonObject> windowObject() const;
  [[nodiscard]] std::optional<QJsonObject> animationObject() const;
  [[nodiscard]] std::optional<QJsonObject> networkObject() const;

  std::optional<QSize> _displaySize;
  QJsonObject _rawApplicationConfig;
};

}  // namespace Configuration
