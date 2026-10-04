#include "pagemanager.h"

#include <simplepagecontextwithmodel.h>

#include <QLoggingCategory>

namespace {
const auto loggingCategory = QLoggingCategory("page_management.page_namager");
}

namespace PageManagement {

PageManagement::PageContextModel *
PageManagement::PageManager::currentPageContextModel() const {
  if (_pagesStack.empty()) {
    return nullptr;
  }
  return _pagesStack.back();
}

PageManager::PageManager(QObject *parent) : QObject(parent) {
  qCInfo(loggingCategory) << "Иницилизация менеджера страниц";
}

bool PageManager::thereIsPreviousPage() const { return _pagesStack.size() > 1; }

void PageManager::goTo(PageType pageType,
                       std::shared_ptr<IPageContext> pageContext) {
  push(new PageContextModel(pageType, pageContext, this));

  qCInfo(loggingCategory)
      << "Переход на страницу"
      << currentPageContextModel()->pageSource().toDisplayString();
  emit currentPageChanged();
}

void PageManager::goBack(std::optional<PageClosingContext> pageClosingContext) {
  qCInfo(loggingCategory) << "Переход на предыдущую страницу";

  resetPageClosingContext(pageClosingContext);

  if (!_pagesStack.empty()) {
    _pagesStack.takeLast()->deleteLater();
    emit currentPageChanged();
  }
  if (_pagesStack.empty()) {
    qCInfo(loggingCategory) << "Последняя страница в стеке закрыта";
    emit lastPageClosed();
  }
}

// void PageManager::goMain() {
//   if (currentPageContextModel()->pageType() == PageType::LoginPage) {
//     _pagesStack.takeLast()->deleteLater();
//     auto context = std::make_shared<SimplePageContextWithModel<QObject>>(
//         nullptr, true, "");
//     goTo(PageType::MainMenuPage, context);
//   } else {
//     while (currentPageContextModel()->pageType() != PageType::MainMenuPage) {
//       _pagesStack.takeLast()->deleteLater();
//       emit currentPageChanged();
//     }
//   }
// }

std::optional<PageClosingContext> PageManager::lastPageClosingContext() const {
  return _pageClosingContext;
}

void PageManager::push(PageContextModel *pageContextModel) {
  _pagesStack.push_back(pageContextModel);
}

void PageManager::resetPageClosingContext(
    std::optional<PageClosingContext> pageClosingContext) {
  if (_pageClosingContext) {
    _pageClosingContext.reset();
  }
  if (pageClosingContext) {
    _pageClosingContext = std::move(pageClosingContext);
  }
}

}  // namespace PageManagement