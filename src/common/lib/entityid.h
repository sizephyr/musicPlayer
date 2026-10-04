#pragma once

#include <QDebug>
#include <QVariant>
#include <cstdint>

/// \brief класс для использования в качестве идентификаторов, защищающий от
/// непреднамеренного спутывания разных идентификаторов
///
/// \example
/// \code
/// void some_function(uint64_t patient_id, uint64_t appointment_id);
///
/// uint64_t patient_id;
/// uint64_t appointment_id;
///
/// // чтобы не было возможности сделать подобных вещей:
/// if (patient_id == appointment_id){/*...*/}
/// // или так:
/// some_function(appointment_id, patient_id);
/// //----------------------------------------
///
/// using patientId = EntityId<struct PatientTag>;
/// using appointmentId = EntityId<struct AppointmentTag>;
///
/// void some_function(patientId patient_id, appointmentId appointment_id);
///
/// patientId patient_id(10);
/// appointmentId appointment_id(10);
///
/// // такие вызовы невозможны, код не скомпилируется
/// if (patient_id == appointment_id){/*...*/}
/// some_function(appointment_id, patient_id);
///
/// \endcode

namespace Common {

template <typename Tag>
struct EntityId {
  uint64_t value;

  explicit EntityId(uint64_t val) : value(val) {}

  auto operator<=>(const EntityId&) const = default;

  friend QDebug operator<<(QDebug debug, const EntityId& id)  // NOLINT
  {
    QDebugStateSaver saver(debug);
    debug.nospace() << "EntityId(" << id.value << ")";
    return debug;
  }

  [[nodiscard]] operator QVariant() const {
    return QVariant::fromValue(value);
  }  // NOLINT
};

}  // namespace Common
