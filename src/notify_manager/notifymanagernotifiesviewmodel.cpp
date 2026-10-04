#include "notifymanagernotifiesviewmodel.h"

NotifyManagerNotifiesViewModel::NotifyManagerNotifiesViewModel(QObject *parent)
    : QAbstractListModel(parent) {}

int NotifyManagerNotifiesViewModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    return 0;
  }

  return static_cast<int>(_notifications.size());
}

QVariant NotifyManagerNotifiesViewModel::data(const QModelIndex &index,
                                              int role) const {
  if (!index.isValid()) {
    return {};
  }

  const int row = index.row();
  if (row < 0 || row >= _notifications.size()) {
    return {};
  }

  const auto &[title, message, type, autoHide] = _notifications[row];

  switch (role) {
    case TitleRole:
      return title;
    case MessageRole:
      return message;
    case TypeRole:
      return type;
    case AutoHideRole:
      return autoHide;
    default:
      return {};
  }
}

QHash<int, QByteArray> NotifyManagerNotifiesViewModel::roleNames() const {
  return {
      {TitleRole, "title"},
      {MessageRole, "message"},
      {TypeRole, "type"},
      {AutoHideRole, "autoHide"},
  };
}

void NotifyManagerNotifiesViewModel::addNotification(
    const Notification &notification) {
  beginInsertRows(QModelIndex(), rowCount({}), rowCount({}));
  _notifications.push_back(notification);
  endInsertRows();
}

void NotifyManagerNotifiesViewModel::addNotification(const QString &title,
                                                     const QString &message,
                                                     NotifyType type,
                                                     bool autoHide) {
  beginInsertRows(QModelIndex(), rowCount({}), rowCount({}));
  _notifications.push_back({
      .title = title,
      .message = message,
      .type = type,
      .autoHide = autoHide,
  });
  endInsertRows();
}

bool NotifyManagerNotifiesViewModel::removeRows(int row, int count,
                                                const QModelIndex &parent) {
  if (parent.isValid() || row < 0 || count < 0 ||
      (row + count) > _notifications.size()) {
    return false;
  }

  if (count == 0) {
    return true;
  }

  beginRemoveRows(parent, row, row + count - 1);

  _notifications.remove(row, count);

  endRemoveRows();
  return true;
}

bool NotifyManagerNotifiesViewModel::remove(int index) {
  return removeRows(index, 1, {});
}

void NotifyManagerNotifiesViewModel::clearAll() {
  if (_notifications.isEmpty()) {
    return;
  }

  beginResetModel();
  _notifications.clear();
  endResetModel();
}
