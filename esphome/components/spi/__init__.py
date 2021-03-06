import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_CLK_PIN, CONF_ID, CONF_MISO_PIN, CONF_MOSI_PIN, CONF_SPI_ID, \
    CONF_CS_PIN
from esphome.core import coroutine

spi_ns = cg.esphome_ns.namespace('spi')
SPIComponent = spi_ns.class_('SPIComponent', cg.Component)
SPIDevice = spi_ns.class_('SPIDevice')
MULTI_CONF = True

CONFIG_SCHEMA = cv.All(cv.Schema({
    cv.GenerateID(): cv.declare_id(SPIComponent),
    cv.Required(CONF_CLK_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_MISO_PIN): pins.gpio_input_pin_schema,
    cv.Optional(CONF_MOSI_PIN): pins.gpio_output_pin_schema,
}), cv.has_at_least_one_key(CONF_MISO_PIN, CONF_MOSI_PIN))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    clk = yield cg.gpio_pin_expression(config[CONF_CLK_PIN])
    cg.add(var.set_clk(clk))
    if CONF_MISO_PIN in config:
        miso = yield cg.gpio_pin_expression(config[CONF_MISO_PIN])
        cg.add(var.set_miso(miso))
    if CONF_MOSI_PIN in config:
        mosi = yield cg.gpio_pin_expression(config[CONF_MOSI_PIN])
        cg.add(var.set_mosi(mosi))

    cg.add_global(spi_ns.using)


SPI_DEVICE_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_SPI_ID): cv.use_id(SPIComponent),
    cv.Required(CONF_CS_PIN): pins.gpio_output_pin_schema,
})


@coroutine
def register_spi_device(var, config):
    parent = yield cg.get_variable(config[CONF_SPI_ID])
    cg.add(var.set_spi_parent(parent))
    pin = yield cg.gpio_pin_expression(config[CONF_CS_PIN])
    cg.add(var.set_cs_pin(pin))
