#include "pagecontextmodel.h"

#include <utility>

namespace PageManagement {

PageContextModel::PageContextModel(QObject* parent)
    : PageContextModel(PageType::None, nullptr, parent) {}

PageContextModel::PageContextModel(PageType type,
                                   std::shared_ptr<IPageContext> context,
                                   QObject* parent)
    : QObject{parent}, _pageType(type), _pageContext(std::move(context)) {}

QString PageContextModel::pageTitle() const {
  if (_pageContext) {
    if (auto titleOpt = _pageContext->pageTitle()) {
      return titleOpt.value();
    }
  }

  return "";
}

QUrl PageContextModel::pageSource() const {
  using enum PageType;
  const auto prefix = QStringLiteral("qrc:/qt/qml/modules/");

  switch (_pageType) {
    case MainPage:
      return {prefix + "main_page/MainPage.qml"};
    default:
      return {};
  }
}

PageType PageContextModel::pageType() const { return _pageType; }

QVariant PageContextModel::contextSpecificData(
    const PageContextSpecificDataKey& key) {
  if (_pageContext) {
    return _pageContext->getSpecificData(key);
  }
  return {};
}

}  // namespace PageManagement
