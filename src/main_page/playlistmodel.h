#pragma once

#include <QtQml/qqmlregistration.h>
#include <mp3description.h>

#include <QAbstractListModel>

class PlayListModel : public QAbstractListModel {
  Q_OBJECT

  QML_ELEMENT

 public:
  enum ItemContentRole {
    Uid = Qt::UserRole + 1,
    Path,
    TrackTitle,
    TrackArtist,
    CoverImagePath,
  };

  explicit PlayListModel(QObject* parent = nullptr);

  void pushBack(Common::Mp3Description descr);
  void reset();

  virtual int rowCount(
      const QModelIndex& parent = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual QHash<int, QByteArray> roleNames() const override;

  [[nodiscard]] std::optional<Common::Mp3Description> mp3DescriptionByTrackUid(
      QUuid) const;

 signals:

 private:
  QList<Common::Mp3Description> _container;
};
