#include "core.h"

#include <authpagemodel.h>
#include <simplepagecontextwithmodel.h>

Core::Core(QQuickItem* parent) : QQuickItem(parent) {}

void Core::setPageManager(PageManagement::PageManager* pageManager) {
  _pageManager = pageManager;
  // if (!_configured) {
  //   _pageManager->goTo(
  //       PageManagement::PageType::FirstConfigurationPage,
  //       std::make_shared<PageManagement::SimplePageContextWithModel<
  //           FirstConfigurationPage::Model>>(nullptr, true, ""));
  // }
}

void Core::setConfiguredState(bool configured) {
  _configured = configured;
  emit connectionConfiguredChanged();
}

bool Core::connectionConfigured() const { return _configured; }
