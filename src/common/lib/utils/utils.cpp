#include "utils.h"

#include <QFile>
#include <QImage>
#include <QTemporaryFile>

namespace Utils {

std::chrono::year_month_day Chrono::today() {
  return std::chrono::year_month_day(
      {floor<std::chrono::days>(std::chrono::system_clock::now())});
}

std::chrono::year_month_day Chrono::toChrono(QDate qDate) {
  return std::chrono::year_month_day{
      std::chrono::year{qDate.year()},
      std::chrono::month{static_cast<unsigned>(qDate.month())},
      std::chrono::day{static_cast<unsigned>(qDate.day())}};
}

std::expected<QJsonObject, QJsonParseError> JSON::parseQJsonObject(
    const QByteArray& rawJSO) {
  QJsonParseError parseErr;
  QJsonObject result = QJsonDocument::fromJson(rawJSO, &parseErr).object();
  if (parseErr.error != QJsonParseError::NoError)
    return std::unexpected(parseErr);
  return result;
}

std::expected<QJsonObject, QString> JSON::readJsonFile(
    const std::filesystem::path& filePath) {
  QFile file(filePath);

  if (!file.exists()) {
    return std::unexpected("Файл не найден");
  }
  if (!file.open(QIODevice::ReadOnly)) {
    return std::unexpected("Не удалось открыть файл на чтение");
  }

  QByteArray data = file.readAll();

  QJsonParseError parseError;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

  if (parseError.error != QJsonParseError::NoError) {
    return std::unexpected("Ошибка парсинга файла");
  }

  if (!doc.isObject()) {
    return std::unexpected("Файл не является json-объектом");
  }

  return doc.object();
}

std::expected<std::filesystem::path, QString> Image::saveImageToTemp(
    const QString& filename, const QImage& image) {
  if (image.isNull()) {
    return std::unexpected("Изображение пустое!");
  }

  QTemporaryFile tempFile(filename);

  if (!tempFile.open()) {
    return std::unexpected("Не удалось создать временный файл:" +
                           tempFile.errorString());
  }

  if (!image.save(&tempFile)) {
    return std::unexpected("Не удалось сохранить QImage в файл.");
  }

  tempFile.setAutoRemove(false);

  QString qtPath = tempFile.fileName();
  qDebug() << "Файл сохранен:" << qtPath;

  std::filesystem::path nativePath(qtPath.toStdString());

  return nativePath;
}

}  // namespace Utils
