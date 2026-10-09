
#include "coverprovider.h"

#include <mainpagemodel.h>

namespace MainPage {

QImage CoverProvider::requestImage(const QString& id, QSize* size,
                                   const QSize& requestedSize) {
  if (_model) {
    if (size) *size = _model->currentCover().size();
    return _model->currentCover();
  }
  return QImage();
}

}  // namespace MainPage
