#pragma once

#include <mp3description.h>

#include <QAbstractListModel>
#include <QQmlEngine>

/// \note вообще, у QMediaPlayer есть свои опции плейлиста и хорошо бы
/// использовать его, но мне пока что в лом разбираться, сделаю, как знаю
class PlayListModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QUuid currentTrackUid READ currentTrackUid NOTIFY
                 currentTrackChanged FINAL)

 public:
  enum ItemContentRole {
    Uid = Qt::UserRole + 1,
    Path,
    TrackTitle,
    TrackArtist,
    // CoverImagePath,
  };

  explicit PlayListModel(QObject* parent = nullptr);

  void pushBack(std::shared_ptr<Common::Mp3Description> descr);
  void reset();
  [[nodiscard]] bool empty() const;

  virtual int rowCount(
      const QModelIndex& parent = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual QHash<int, QByteArray> roleNames() const override;

  [[nodiscard]] std::shared_ptr<Common::Mp3Description>
      mp3DescriptionByTrackUid(QUuid) const;
  /// \brief возвращает описание следующего трека, расположенного в списке после
  /// того, чей UID передан в качестве аргумента
  [[nodiscard]] std::shared_ptr<Common::Mp3Description> next(QUuid);
  [[nodiscard]] bool nextTrackAvailable(QUuid);
  /// \brief возвращает описание следующего трека, расположенного в списке перед
  /// того, чей UID передан в качестве аргумента
  [[nodiscard]] std::shared_ptr<Common::Mp3Description> previous(QUuid);
  [[nodiscard]] bool previousTrackAvailable(QUuid);

  std::shared_ptr<Common::Mp3Description> currentTrack() const;
  void setCurrentTrack(std::shared_ptr<Common::Mp3Description> newCurrentTrack);
  void resetCurrentTrack();
  QUuid currentTrackUid() const;

 signals:
  void currentTrackChanged();

 private:
  QList<std::shared_ptr<Common::Mp3Description>> _container;
  std::shared_ptr<Common::Mp3Description> _currentTrack{nullptr};
};
