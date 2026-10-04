#include "notifymanagermodel.h"

NotifyManagerModel::NotifyManagerModel(QObject *parent)
    : QObject{parent}, _viewModel(new NotifyManagerNotifiesViewModel(this)) {}

NotifyManagerNotifiesViewModel *NotifyManagerModel::viewModel() const {
  return _viewModel;
}

void NotifyManagerModel::info(const QString &title, const QString &message,
                              bool autoHide) {
  _viewModel->addNotification({
      .title = title,
      .message = message,
      .type = NotifyManagerNotifiesViewModel::Info,
      .autoHide = autoHide,
  });
}

void NotifyManagerModel::warning(const QString &title, const QString &message,
                                 bool autoHide) {
  _viewModel->addNotification({
      .title = title,
      .message = message,
      .type = NotifyManagerNotifiesViewModel::Warning,
      .autoHide = autoHide,
  });
}

void NotifyManagerModel::error(const QString &title, const QString &message,
                               bool autoHide) {
  _viewModel->addNotification({
      .title = title,
      .message = message,
      .type = NotifyManagerNotifiesViewModel::Error,
      .autoHide = autoHide,
  });
}
