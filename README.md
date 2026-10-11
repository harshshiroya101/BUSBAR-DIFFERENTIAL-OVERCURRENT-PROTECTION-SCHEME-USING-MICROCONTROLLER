# Busbar Differential & Overcurrent Protection Scheme Using Microcontroller

## Overview

This project presents a microcontroller-based busbar protection prototype that demonstrates **differential protection** and **definite-time overcurrent protection** using an Arduino Uno. Current transformers (CTs) measure currents at two points around the protected zone. Their signals are conditioned using level-shifting circuits so that the AC waveforms can be read by the Arduino's 0–5 V analog inputs.

The Arduino processes the sensed current signals according to the selected protection mode. In differential mode, it evaluates the imbalance between incoming and outgoing current measurements to identify a possible internal busbar fault. In overcurrent mode, it checks whether the measured current exceeds a calibrated pickup threshold for the selected time delay. The controller then sends a command to optically isolated TLP250 driver circuits, which control a pair of anti-parallel TYN612 thyristors used as the AC solid-state switching stage.

The hardware is arranged in separate modules, including CT sensing, signal conditioning, low-voltage reference supplies, bipolar analog supplies, switch inputs, the Arduino controller, isolated gate drivers, and the thyristor switching board. Mode-selection switches provide differential or overcurrent operation, manual reset, fault-bank selection, and nominal 2 s or 5 s overcurrent delay settings.

The prototype is intended for **educational demonstration and laboratory study** of current-transformer sensing, analog signal conditioning, embedded protection logic, optical isolation, and AC thyristor switching. It is not a certified industrial or substation protection relay.

## Key Features

- Arduino Uno-based protection control.
- Differential protection based on the difference between two CT measurements.
- Selectable definite-time overcurrent protection.
- CT signal level shifting for interfacing AC measurements with Arduino ADC inputs.
- Optically isolated TLP250 gate-driver channels.
- Anti-parallel TYN612 thyristors for controlling both AC half-cycles.
- Switchboard controls for protection mode, reset, fault-bank selection, and delay selection.
- Modular hardware intended for staged testing and troubleshooting.

## Main Hardware

- Arduino Uno
- Two current transformers (CT1 and CT2)
- CT burden/sensing resistors
- CT level-shifter circuits and 2.5 V bias references
- Bipolar analog supply for the level-shifter circuits
- TLP250 optically isolated gate drivers
- Two TYN612 thyristors connected in anti-parallel
- Rectifier diodes, voltage regulators, capacitors, resistors, connectors, and control switches
- Transformer-derived low-voltage supplies and a laboratory load/fault arrangement

## Operating Principle

1. CT1 and CT2 sense current at the boundaries of the protected section.
2. Burden resistors convert the CT secondary currents into proportional voltage signals.
3. Level-shifter circuits add a nominal 2.5 V bias so the bipolar AC signals can be measured by the Arduino ADC.
4. The Arduino reads the conditioned signals and applies the selected protection logic.
5. If the configured trip condition is satisfied, the Arduino changes its output to the isolated gate-driver circuit.
6. The driver stage controls the anti-parallel thyristor switching arrangement to interrupt the load current as the thyristor current naturally falls below its holding current, typically near an AC current zero.

## Control Switches

| Switch | Function |
|---|---|
| SW1 | Connects or disconnects the fault bank |
| SW2 | Manual reset request |
| SW3 | Selects the nominal overcurrent delay: 2 s or 5 s |
| SW4 | Selects differential or overcurrent protection mode |

The exact pickup thresholds and signal scaling must be calibrated against the assembled hardware and the firmware in use.

## Testing and Verification

Suggested checks include:

- Inspect wiring, connector polarity, and CT secondary termination with power isolated.
- Verify all supply rails and the nominal 2.5 V bias references.
- Test level-shifter output ranges using safely simulated inputs.
- Confirm Arduino switch recognition and trip-output logic with an isolated low-voltage test circuit.
- Verify that healthy and external through-current conditions do not cause an unwanted trip.
- Verify that an internal-zone fault simulation triggers the differential function.
- Verify overcurrent pickup and selected delay using recorded measurements.
- Record actual ADC readings, current waveforms, and trip times for each test.

## Safety Notes

**This is a laboratory prototype, not a certified protection device.** The circuit involves potentially lethal mains voltage. Do not connect, test, or modify the mains-connected assembly without qualified laboratory supervision, suitable isolation, correctly rated instruments, and appropriate protective equipment. Confirm the actual supply-rail voltages and validate the loaded firmware before energizing the system. Removing an SCR gate command does not force an already-conducting SCR to turn off immediately; current normally must fall below its holding level.

## Limitations

- Performance depends on correct CT polarity, calibrated sensing circuits, and validated firmware.
- The supplied project report does not include traceable numerical measurements or time-stamped trip waveforms, so quantitative performance claims should be supported with actual laboratory records.
- A practical industrial busbar protection system requires additional engineering, including multi-feeder current measurement, CT saturation restraint, dependable fail-safe trip logic, self-diagnostics, insulation and EMC validation, and coordination with qualified circuit breakers.

## Authors

- Harsh Shiroya (23BEE106)
- Rudraksh Mishra (23BEE097)

Department of Electrical Engineering  
Institute of Technology, Nirma University, Ahmedabad, India
