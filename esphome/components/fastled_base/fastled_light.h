#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/light/addressable_light.h"

#ifdef USE_POWER_SUPPLY
#include "esphome/components/power_supply/power_supply.h"
#endif

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP32_RAW_PIN_ORDER
#define FASTLED_RMT_BUILTIN_DRIVER true

// Avoid annoying compiler messages
#define FASTLED_INTERNAL

#include "FastLED.h"

namespace esphome {
namespace fastled_base {

class FastLEDLightOutput : public Component, public light::AddressableLight {
 public:
  /// Only for custom effects: Get the internal controller.
  CLEDController *get_controller() const { return this->controller_; }

  inline int32_t size() const override { return this->num_leds_; }

  inline light::ESPColorView operator[](int32_t index) const override {
    return light::ESPColorView(&this->leds_[index].r, &this->leds_[index].g, &this->leds_[index].b, nullptr,
                               &this->effect_data_[index], &this->correction_);
  }

  /// Set a maximum refresh rate in µs as some lights do not like being updated too often.
  void set_max_refresh_rate(uint32_t interval_us) { this->max_refresh_rate_ = interval_us; }

#ifdef USE_POWER_SUPPLY
  void set_power_supply(power_supply::PowerSupply *power_supply) { this->power_supply_ = power_supply; }
#endif

  /// Add some LEDS, can only be called once.
  CLEDController &add_leds(CLEDController *controller, int num_leds) {
    this->controller_ = controller;
    this->num_leds_ = num_leds;
    this->leds_ = new CRGB[num_leds];

    for (int i = 0; i < this->num_leds_; i++)
      this->leds_[i] = CRGB::Black;

    return *this->controller_;
  }

  template<ESPIChipsets CHIPSET, uint8_t DATA_PIN, uint8_t CLOCK_PIN, EOrder RGB_ORDER, uint8_t SPI_DATA_RATE>
  CLEDController &add_leds(int num_leds) {
    switch (CHIPSET) {
      case LPD8806: {
        static LPD8806Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER, SPI_DATA_RATE> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case WS2801: {
        static WS2801Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER, SPI_DATA_RATE> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case WS2803: {
        static WS2803Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER, SPI_DATA_RATE> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case SM16716: {
        static SM16716Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER, SPI_DATA_RATE> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case P9813: {
        static P9813Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER, SPI_DATA_RATE> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case DOTSTAR:
      case APA102: {
        static APA102Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER, SPI_DATA_RATE> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case SK9822: {
        static SK9822Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER, SPI_DATA_RATE> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
    }
  }

  template<ESPIChipsets CHIPSET, uint8_t DATA_PIN, uint8_t CLOCK_PIN> CLEDController &add_leds(int num_leds) {
    switch (CHIPSET) {
      case LPD8806: {
        static LPD8806Controller<DATA_PIN, CLOCK_PIN> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case WS2801: {
        static WS2801Controller<DATA_PIN, CLOCK_PIN> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case WS2803: {
        static WS2803Controller<DATA_PIN, CLOCK_PIN> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case SM16716: {
        static SM16716Controller<DATA_PIN, CLOCK_PIN> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case P9813: {
        static P9813Controller<DATA_PIN, CLOCK_PIN> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case DOTSTAR:
      case APA102: {
        static APA102Controller<DATA_PIN, CLOCK_PIN> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case SK9822: {
        static SK9822Controller<DATA_PIN, CLOCK_PIN> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
    }
  }

  template<ESPIChipsets CHIPSET, uint8_t DATA_PIN, uint8_t CLOCK_PIN, EOrder RGB_ORDER>
  CLEDController &add_leds(int num_leds) {
    switch (CHIPSET) {
      case LPD8806: {
        static LPD8806Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case WS2801: {
        static WS2801Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case WS2803: {
        static WS2803Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case SM16716: {
        static SM16716Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case P9813: {
        static P9813Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case DOTSTAR:
      case APA102: {
        static APA102Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
      case SK9822: {
        static SK9822Controller<DATA_PIN, CLOCK_PIN, RGB_ORDER> CONTROLLER;
        return add_leds(&CONTROLLER, num_leds);
      }
    }
  }

#ifdef FASTLED_HAS_CLOCKLESS
  template<template<uint8_t DATA_PIN, EOrder RGB_ORDER> class CHIPSET, uint8_t DATA_PIN, EOrder RGB_ORDER>
  CLEDController &add_leds(int num_leds) {
    static CHIPSET<DATA_PIN, RGB_ORDER> CONTROLLER;
    return add_leds(&CONTROLLER, num_leds);
  }

  template<template<uint8_t DATA_PIN, EOrder RGB_ORDER> class CHIPSET, uint8_t DATA_PIN>
  CLEDController &add_leds(int num_leds) {
    static CHIPSET<DATA_PIN, RGB> CONTROLLER;
    return add_leds(&CONTROLLER, num_leds);
  }

  template<template<uint8_t DATA_PIN> class CHIPSET, uint8_t DATA_PIN> CLEDController &add_leds(int num_leds) {
    static CHIPSET<DATA_PIN> CONTROLLER;
    return add_leds(&CONTROLLER, num_leds);
  }
#endif

  template<template<EOrder RGB_ORDER> class CHIPSET, EOrder RGB_ORDER> CLEDController &add_leds(int num_leds) {
    static CHIPSET<RGB_ORDER> CONTROLLER;
    return add_leds(&CONTROLLER, num_leds);
  }

  template<template<EOrder RGB_ORDER> class CHIPSET> CLEDController &add_leds(int num_leds) {
    static CHIPSET<RGB> CONTROLLER;
    return add_leds(&CONTROLLER, num_leds);
  }

#ifdef FASTLED_HAS_BLOCKLESS
  template<EBlockChipsets CHIPSET, int NUM_LANES, EOrder RGB_ORDER> CLEDController &add_leds(int num_leds) {
    switch (CHIPSET) {
#ifdef PORTA_FIRST_PIN
      case WS2811_PORTA:
        return add_leds(
            new InlineBlockClocklessController<NUM_LANES, PORTA_FIRST_PIN, NS(320), NS(320), NS(640), RGB_ORDER>(),
            num_leds);
      case WS2811_400_PORTA:
        return add_leds(
            new InlineBlockClocklessController<NUM_LANES, PORTA_FIRST_PIN, NS(800), NS(800), NS(900), RGB_ORDER>(),
            num_leds);
      case WS2813_PORTA:
        return add_leds(new InlineBlockClocklessController<NUM_LANES, PORTA_FIRST_PIN, NS(320), NS(320), NS(640),
                                                           RGB_ORDER, 0, false, 300>(),
                        num_leds);
      case TM1803_PORTA:
        return add_leds(
            new InlineBlockClocklessController<NUM_LANES, PORTA_FIRST_PIN, NS(700), NS(1100), NS(700), RGB_ORDER>(),
            num_leds);
      case UCS1903_PORTA:
        return add_leds(
            new InlineBlockClocklessController<NUM_LANES, PORTA_FIRST_PIN, NS(500), NS(1500), NS(500), RGB_ORDER>(),
            num_leds);
#endif
    }
  }

  template<EBlockChipsets CHIPSET, int NUM_LANES> CLEDController &add_leds(int num_leds) {
    return add_leds<CHIPSET, NUM_LANES, GRB>(num_leds);
  }
#endif

  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)
  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supports_brightness(true);
    traits.set_supports_rgb(true);
    return traits;
  }
  void setup() override;
  void dump_config() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  void clear_effect_data() override {
    for (int i = 0; i < this->size(); i++)
      this->effect_data_[i] = 0;
  }

 protected:
  CLEDController *controller_{nullptr};
  CRGB *leds_{nullptr};
  uint8_t *effect_data_{nullptr};
  int num_leds_{0};
  uint32_t last_refresh_{0};
  optional<uint32_t> max_refresh_rate_{};
#ifdef USE_POWER_SUPPLY
  power_supply::PowerSupply *power_supply_{nullptr};
  bool has_requested_high_power_{false};
#endif
};

}  // namespace fastled_base
}  // namespace esphome
