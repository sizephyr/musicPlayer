#include "core.h"

#include <mainpagemodel.h>
#include <simplepagecontextwithmodel.h>

Core::Core(QQuickItem* parent) : QQuickItem(parent) {}

void Core::setPageManager(PageManagement::PageManager* pageManager) {
  _pageManager = pageManager;
  _pageManager->goTo(
      PageManagement::PageType::MainPage,
      std::make_shared<
          PageManagement::SimplePageContextWithModel<MainPage::Model>>(nullptr,
                                                                       ""));
}
