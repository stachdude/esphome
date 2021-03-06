#pragma once

#include "esphome/core/automation.h"
#include "climate.h"

namespace esphome {
namespace climate {

template<typename... Ts> class ControlAction : public Action<Ts...> {
 public:
  explicit ControlAction(Climate *climate) : climate_(climate) {}

  TEMPLATABLE_VALUE(ClimateMode, mode)
  TEMPLATABLE_VALUE(float, target_temperature)
  TEMPLATABLE_VALUE(float, target_temperature_low)
  TEMPLATABLE_VALUE(float, target_temperature_high)
  TEMPLATABLE_VALUE(bool, away)

  void play(Ts... x) override {
    auto call = this->climate_->make_call();
    call.set_target_temperature(this->mode_.optional_value(x...));
    call.set_target_temperature_low(this->target_temperature_low_.optional_value(x...));
    call.set_target_temperature_high(this->target_temperature_high_.optional_value(x...));
    call.set_away(this->away_.optional_value(x...));
    call.perform();
  }

 protected:
  Climate *climate_;
};

}  // namespace climate
}  // namespace esphome
