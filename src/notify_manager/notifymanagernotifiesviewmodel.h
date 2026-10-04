#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QString>
#include <QVector>

// Простая структура для хранения данных уведомления
struct Notification;

class NotifyManagerNotifiesViewModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

 public:
  enum NotifyType {
    Info,
    Warning,
    Error,
  };
  Q_ENUM(NotifyType)

  enum NotifyRoles {
    TitleRole = Qt::UserRole + 1,
    MessageRole,
    TypeRole,
    AutoHideRole,
  };

  explicit NotifyManagerNotifiesViewModel(QObject *parent = nullptr);

  [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex &index,
                              int role) const override;
  [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
  bool removeRows(int row, int count, const QModelIndex &parent) override;

  void addNotification(const Notification &notification);
  void addNotification(const QString &title, const QString &message,
                       NotifyType type, bool autoHide = false);
  Q_INVOKABLE bool remove(int index);
  void clearAll();

 private:
  QVector<Notification> _notifications;
};

struct Notification {
  using enum NotifyManagerNotifiesViewModel::NotifyType;

  QString title;
  QString message;
  NotifyManagerNotifiesViewModel::NotifyType type{Info};
  bool autoHide{false};

  bool operator==(const Notification &) const = default;

  std::strong_ordering operator<<(const Notification &other) const {
    if (auto cmp = title.compare(other.title); cmp != 0) {
      return cmp <=> 0;
    }
    if (auto cmp = message.compare(other.message); cmp != 0) {
      return cmp <=> 0;
    }
    if (auto cmp = type <=> other.type; cmp != 0) {
      return cmp;
    }
    return static_cast<int>(autoHide) <=> static_cast<int>(other.autoHide);
  }
};
