#pragma once

#include <QQuickItem>
#include <QString>

class HeaderModel : public QQuickItem {
  Q_OBJECT

  Q_PROPERTY(QString currentUserInitials READ currentUserInitials NOTIFY
                 currentUserInitialsChanged)
  Q_PROPERTY(int notifyInterval READ notifyInterval CONSTANT)
  Q_PROPERTY(QJSValue callBeforeClose READ callBeforeClose WRITE
                 setCallBeforeClose NOTIFY callBeforeCloseChanged)

  QML_ELEMENT
  QML_SINGLETON
 public:
  explicit HeaderModel(QQuickItem *parent = nullptr);

  [[nodiscard]] QString currentUserInitials() const;
  /// \todo реализовать манипулирование посредством конфига
  [[nodiscard]] static int notifyInterval();

  [[nodiscard]] QJSValue callBeforeClose() const;
  void setCallBeforeClose(const QJSValue &);

 signals:
  void currentPageNameChanged();
  void currentUserInitialsChanged();
  void goToPreviousPage();
  void showNotify(const QString &);
  void canBeClosed();
  void callBeforeCloseChanged();

 private:
  QString _currentUserInitials;

  /// \brief callable-функция для вызова перед тем, как заголовок выполнит
  /// закртыие страницы. Нужен для того, чтобы страницы могли перехватывать
  /// ивенты закрытия. ОБЯЗАТЕЛЬНЫМ УСЛОВИЕМ является необходимость вызвать
  /// сигнал canBeClosed после отработки прерывания
  QJSValue _callBeforeClose;
};
