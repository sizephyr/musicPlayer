#pragma once

#include <QDir>
#include <QMediaMetaData>
#include <QObject>
#include <QString>

class FileManageBackend : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString title READ title NOTIFY titleChanged)
  Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)
  Q_PROPERTY(
      QString coverImagePath READ coverImagePath NOTIFY coverImagePathChanged)

 public:
  explicit FileManageBackend(QObject* parent = nullptr);

  Q_INVOKABLE QStringList fileSearch(const QString& path);
  Q_INVOKABLE void parseMp3(const QString& filePath);

  QString title() const;
  QString artist() const;
  QString coverImagePath() const;

 signals:
  void titleChanged();
  void artistChanged();
  void coverImagePathChanged();

 private:
  void parseID3(const QString& filePath);
  void processApicFrame(QByteArrayView frameData);

  QString _title;
  QString _artist;
  QString _coverImagePath;
};
