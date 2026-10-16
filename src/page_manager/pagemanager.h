#pragma once

#include <pageclosingcontext.h>
#include <pagecontextmodel.h>

#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>

namespace PageManagement {

/// \brief менеджмент вкладок приложения
class PageManager : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(PageContextModel* currentPageContextModel READ
                 currentPageContextModel NOTIFY currentPageChanged)
  Q_PROPERTY(bool thereIsPreviousPage READ thereIsPreviousPage NOTIFY
                 currentPageChanged)

 public:
  explicit PageManager(QObject* parent = nullptr);

  [[nodiscard]] bool thereIsPreviousPage() const;

  /// \brief переход на следующую страницу
  Q_INVOKABLE void goTo(PageType, std::shared_ptr<IPageContext>);
  /// \brief возврат на предыдущую страницу
  /// \arg опциональный контекст закрытия страницы
  Q_INVOKABLE void goBack(std::optional<PageClosingContext> = std::nullopt);
  /// \brief возврат на главную страницу
  // Q_INVOKABLE void goMain();

  [[nodiscard]] PageContextModel* currentPageContextModel() const;
  [[nodiscard]] std::optional<PageClosingContext> lastPageClosingContext()
      const;

 signals:
  void currentPageChanged();
  void lastPageClosed();

 private:
  void push(PageContextModel* pageContextModel);
  void resetPageClosingContext(
      std::optional<PageClosingContext> = std::nullopt);

  /// \memberof стэк страниц
  QList<PageContextModel*> _pagesStack;
  /// \memberof контекст закрытия последней страницы
  std::optional<PageClosingContext> _pageClosingContext;
};

}  // namespace PageManagement
