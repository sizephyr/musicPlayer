#pragma once

#include <ipagecontext.h>
#include <pagetype.h>

#include <QObject>
#include <QQmlEngine>

namespace PageManagement {

/// \brief верхнеуровневый контекст страницы
class PageContextModel : public QObject {
  Q_OBJECT
  QML_NAMED_ELEMENT(PageContextModel)

  Q_PROPERTY(QString pageTitle READ pageTitle NOTIFY contextChanged)
  Q_PROPERTY(QUrl pageSource READ pageSource NOTIFY contextChanged)

 public:
  explicit PageContextModel(QObject* parent = nullptr);  // for QML
  PageContextModel(PageType, std::shared_ptr<IPageContext>,
                   QObject* parent = nullptr);

  [[nodiscard]] QString pageTitle() const;
  /// \todo продумать систему соотнесения типов страниц к источникам
  [[nodiscard]] QUrl pageSource() const;

  [[nodiscard]] PageType pageType() const;
  [[nodiscard]] Q_INVOKABLE QVariant
  contextSpecificData(const PageManagement::PageContextSpecificDataKey& key);

 signals:
  void contextChanged();

 private:
  PageType _pageType{PageType::None};
  std::shared_ptr<IPageContext> _pageContext{nullptr};
};

}  // namespace PageManagement
