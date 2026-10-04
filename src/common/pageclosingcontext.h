#pragma once

#include <QObject>
#include <QVariantHash>

namespace PageManagement {

/// \brief интерфейс контекста закрытия последней страницы для возможности
/// передачи вызывавшему окну каких-то данных о результатах работы закрывшейся
class PageClosingContext {
 public:
  [[nodiscard]] QVariant get(const QString &key) const {
    return _data.value(key);
  }
  void add(const QString &key, const QVariant &value) {
    _data.insert(key, value);
  }

 private:
  QVariantHash _data;
};

}  // namespace PageManagement
