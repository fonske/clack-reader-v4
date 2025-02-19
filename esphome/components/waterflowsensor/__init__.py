import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_PIN

water_flow_sensor_ns = cg.esphome_ns.namespace('water_flow_sensor')
WaterFlowSensor = water_flow_sensor_ns.class_('WaterFlowSensor', cg.Component, sensor.Sensor)

CONF_FLOW_RATE = 'flow_rate'
CONF_TOTAL_USAGE = 'total_usage'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(WaterFlowSensor),
    cv.Required(CONF_PIN): cv.pin,
    cv.Optional(CONF_FLOW_RATE): sensor.sensor_schema(),
    cv.Optional(CONF_TOTAL_USAGE): sensor.sensor_schema(),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config.get(CONF_FLOW_RATE))
    yield sensor.register_sensor(var, config.get(CONF_TOTAL_USAGE))
    cg.add(var.set_pin(config[CONF_PIN]))
