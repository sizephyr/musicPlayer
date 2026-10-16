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
  QML_NAMED_ELEMENT(ConfigurationBase)

  Q_PROPERTY(QSize displaySize READ displaySize WRITE setDisplaySize NOTIFY
                 displaySizeChanged)
  Q_PROPERTY(double scaleFactor READ scaleFactor NOTIFY scaleFactorChanged)
  Q_PROPERTY(Qt::WindowFlags windowFlags READ windowFlags CONSTANT)
  Q_PROPERTY(QString applicationVersion READ applicationVersion CONSTANT)
 public:
  explicit Base(QObject* parent = nullptr);

  [[nodiscard]] Q_INVOKABLE int referenceWindowWidth() const;
  [[nodiscard]] Q_INVOKABLE int referenceWindowHeight() const;
  [[nodiscard]] Q_INVOKABLE QString windowTitle() const;
  [[nodiscard]] Q_INVOKABLE int animationDurationMs() const;
  [[nodiscard]] Q_INVOKABLE int baseTransferTimeoutMs() const;
  [[nodiscard]] Q_INVOKABLE int notificationHideAfterMs() const;
  [[nodiscard]] Q_INVOKABLE QString windowBackground() const;

  void setDisplaySize(QSize);
  [[nodiscard]] QSize displaySize() const;
  [[nodiscard]] double scaleFactor() const;
  [[nodiscard]] static Qt::WindowFlags windowFlags();

  QString applicationVersion() const;

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
