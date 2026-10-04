#include "headermodel.h"

HeaderModel::HeaderModel(QQuickItem *parent)
    : QQuickItem(parent), _currentUserInitials("Фамилиев И.О.") {}

QString HeaderModel::currentUserInitials() const {
  return _currentUserInitials;
}

int HeaderModel::notifyInterval() { return 3000; }

QJSValue HeaderModel::callBeforeClose() const { return _callBeforeClose; }

void HeaderModel::setCallBeforeClose(const QJSValue &callBeforeClose) {
  _callBeforeClose = callBeforeClose;
  emit callBeforeCloseChanged();
}
