#include "macoswindowhelper.h"

#ifdef Q_OS_MAC
#include <objc/message.h>
#include <objc/runtime.h>
#endif

MacOSWindowHelper::MacOSWindowHelper(QQuickWindow* window)
    : QObject(window), _window(window) {
  if (!_window) return;

  // Подключаемся к сигналу изменения состояния окна
  connect(_window, &QQuickWindow::windowStateChanged, this,
          &MacOSWindowHelper::handleWindowStateChanged);
  connect(_window, &QQuickWindow::flagsChanged, this,
          &MacOSWindowHelper::handleWindowFlagsChanged);

  // Применяем настройки сразу при инициализации
  applyMacTransparentTitlebar();
}

void MacOSWindowHelper::handleWindowStateChanged(Qt::WindowState state) {
  // Если окно вернулось в нормальное состояние или максимизировано (не Full
  // Screen)
  if (state == Qt::WindowNoState || state == Qt::WindowMaximized) {
    // macOS сбрасывает маску не сразу, даем Qt завершить переход
    QMetaObject::invokeMethod(this, &MacOSWindowHelper::applyMacTransparentTitlebar,
                              Qt::QueuedConnection);
  }
}

void MacOSWindowHelper::handleWindowFlagsChanged(Qt::WindowFlags flags) {
  QMetaObject::invokeMethod(this, &MacOSWindowHelper::applyMacTransparentTitlebar,
                            Qt::QueuedConnection);
}

void MacOSWindowHelper::applyMacTransparentTitlebar() {
  if (!_window) return;

  if (auto nativeView = reinterpret_cast<id>(_window->winId())) {
    SEL windowSelector = sel_registerName("window");
    auto cocoaWindow =
        ((id (*)(id, SEL))objc_msgSend)(nativeView, windowSelector);

    if (cocoaWindow) {
      // 1. Восстанавливаем FullSizeContentView
      SEL styleMaskSel = sel_registerName("styleMask");
      SEL setStyleMaskSel = sel_registerName("setStyleMask:");
      long long currentMask =
          ((long long (*)(id, SEL))objc_msgSend)(cocoaWindow, styleMaskSel);

      // NSWindowStyleMaskFullSizeContentView = 1 << 15
      long long newMask = currentMask | 32768;
      ((void (*)(id, SEL, long long))objc_msgSend)(cocoaWindow, setStyleMaskSel,
                                                   newMask);

      // 2. Делаем заголовок прозрачным
      SEL setTransparentSel =
          sel_registerName("setTitlebarAppearsTransparent:");
      ((void (*)(id, SEL, bool))objc_msgSend)(cocoaWindow, setTransparentSel,
                                              true);

      // 3. Скрываем текст заголовка
      SEL setVisibilitySel = sel_registerName("setTitleVisibility:");
      ((void (*)(id, SEL, long long))objc_msgSend)(cocoaWindow,
                                                   setVisibilitySel, 1);
    }
  }
}
