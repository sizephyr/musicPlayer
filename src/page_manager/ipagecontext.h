#pragma once

#include <QString>
#include <QVariant>
#include <optional>

namespace PageManagement {
using PageContextSpecificDataKey = QString;

/// \brief интерфейс вспомогательного класса контекста страницы
class IPageContext {
 public:
  virtual ~IPageContext() = default;

  [[nodiscard]] virtual std::optional<QString> pageTitle() const = 0;
  [[nodiscard]] virtual QVariant pageModel() const = 0;
  [[nodiscard]] virtual QVariant getSpecificData(
      const PageContextSpecificDataKey&) const = 0;
};

}  // namespace PageManagement
