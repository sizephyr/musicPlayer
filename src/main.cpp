#include <QGuiApplication>
#include <QQmlApplicationEngine>

#ifdef SHOW_RESOURCES
#include <QDirIterator>
#endif
#ifdef Q_OS_MAC
#include <objc/message.h>
#include <objc/runtime.h>

#include <QQuickWindow>
#endif

int main(int argc, char* argv[]) {
  qSetMessagePattern(
      "[%{time yyMMdd h:mm:ss.zzz} "
      "%{category} "
      "%{if-debug}DEBG%{endif}"
      "%{if-info}INFO%{endif}"
      "%{if-warning}WARN%{endif}"
      "%{if-critical}CRIT%{endif}"
      "%{if-fatal}FATL%{endif}] "
      "- %{message}");
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("Main", "Main");

#ifdef SHOW_RESOURCES
  QDirIterator it(":", QDirIterator::Subdirectories);
  while (it.hasNext()) {
    qDebug() << it.next();
  }
#endif

  /// \todo убрать в отдельный файл
#ifdef Q_OS_MAC
  // Настройка прозрачного заголовка через чистый C++ Objective-C Runtime
  if (!engine.rootObjects().isEmpty()) {
    if (auto* window =
            qobject_cast<QQuickWindow*>(engine.rootObjects().first())) {
      // Получаем WId окна (в macOS это указатель на NSView)
      if (auto nativeView = reinterpret_cast<id>(window->winId())) {
        // Получаем NSWindow: id cocoaWindow = [nativeView window];
        SEL windowSelector = sel_registerName("window");
        auto cocoaWindow =
            ((id (*)(id, SEL))objc_msgSend)(nativeView, windowSelector);

        if (cocoaWindow) {
          // 1. Растягиваем контент: [cocoaWindow setStyleMask: ...]
          SEL styleMaskSel = sel_registerName("styleMask");
          SEL setStyleMaskSel = sel_registerName("setStyleMask:");
          long long currentMask =
              ((long long (*)(id, SEL))objc_msgSend)(cocoaWindow, styleMaskSel);

          // NSWindowStyleMaskFullSizeContentView = 1 << 15 (32768)
          long long newMask = currentMask | 32768;
          ((void (*)(id, SEL, long long))objc_msgSend)(
              cocoaWindow, setStyleMaskSel, newMask);

          // 2. Делаем заголовок прозрачным: [cocoaWindow
          // setTitlebarAppearsTransparent:YES];
          SEL setTransparentSel =
              sel_registerName("setTitlebarAppearsTransparent:");
          ((void (*)(id, SEL, bool))objc_msgSend)(cocoaWindow,
                                                  setTransparentSel, true);

          // 3. Скрываем текст: [cocoaWindow
          // setTitleVisibility:NSWindowTitleHidden]; NSWindowTitleHidden = 1
          SEL setVisibilitySel = sel_registerName("setTitleVisibility:");
          ((void (*)(id, SEL, long long))objc_msgSend)(cocoaWindow,
                                                       setVisibilitySel, 1);
        }
      }
    }
  }
#endif

  return QGuiApplication::exec();
}
