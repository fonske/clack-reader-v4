import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_PIN

CODEOWNERS = ["@pletch"]
DEPENDENCIES = ["sensor"]

clack_water_flow_ns = cg.esphome_ns.namespace("clack_water_flow")
WaterFlowSensor = clack_water_flow_ns.class_(
    "WaterFlowSensor", sensor.Sensor, cg.Component
)

CONF_TOTAL_WATER_USAGE = "total_water_usage"
CONF_PULSE_PER_GALLON_GLOBAL = "pulse_per_gallon"
CONF_WATER_METER_FREEZE = "water_meter_freeze"
CONF_WATERMETER_SENSOR = "watermeter_sensor"

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        WaterFlowSensor,
        unit_of_measurement="gal/min",
        icon="mdi:waves-arrow-right",
        accuracy_decimals=2,
    )
    .extend(
        {
            cv.Required(CONF_PIN): pins.internal_gpio_input_pin_schema,
            cv.Required(CONF_TOTAL_WATER_USAGE): cv.use_id(cg.Component),
            cv.Required(CONF_PULSE_PER_GALLON_GLOBAL): cv.use_id(cg.Component),
            cv.Optional(CONF_WATER_METER_FREEZE): cv.use_id(cg.Component),
            cv.Optional(CONF_WATERMETER_SENSOR): cv.use_id(sensor.Sensor),
        }
    )
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))

    total_var = await cg.get_variable(config[CONF_TOTAL_WATER_USAGE])
    cg.add(var.set_total_water_usage(total_var))

    ppg_var = await cg.get_variable(config[CONF_PULSE_PER_GALLON_GLOBAL])
    cg.add(var.set_pulse_per_gallon(ppg_var))

    if CONF_WATER_METER_FREEZE in config:
        freeze_var = await cg.get_variable(config[CONF_WATER_METER_FREEZE])
        cg.add(var.set_water_meter_freeze(freeze_var))

    if CONF_WATERMETER_SENSOR in config:
        watermeter = await cg.get_variable(config[CONF_WATERMETER_SENSOR])
        cg.add(var.set_watermeter_sensor(watermeter))
