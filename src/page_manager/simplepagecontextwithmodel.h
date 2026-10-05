#pragma once

#include <QPointer>

#include "ipagecontext.h"

namespace PageManagement {

/// \brief простая модель страницы, содержащая в контексте модель
/// непосредственно UI, содержимого страницей. Последний доступен по ключу
/// "pageModel"
template <typename PageModel>
  requires std::derived_from<PageModel, QObject>
class SimplePageContextWithModel : public IPageContext {
 public:
  SimplePageContextWithModel() = default;
  SimplePageContextWithModel(PageModel* pageModel, QString pageTitle)
      : _pageTitle(pageTitle), _pageModel(pageModel) {}
  ~SimplePageContextWithModel() = default;

  [[nodiscard]] std::optional<QString> pageTitle() const override {
    return _pageTitle;
  }
  void setPageTitle(std::optional<QString> newPageTitle) {
    _pageTitle = newPageTitle;
  }
  [[nodiscard]] QVariant pageModel() const override {
    return getSpecificData("pageModel");
  }

  [[nodiscard]] QVariant getSpecificData(
      const PageContextSpecificDataKey& key) const override {
    if (key == "pageModel") {
      return QVariant::fromValue(_pageModel.data());
    }
    return {};
  }

 private:
  std::optional<QString> _pageTitle;
  QPointer<PageModel> _pageModel{nullptr};
};

}  // namespace PageManagement
