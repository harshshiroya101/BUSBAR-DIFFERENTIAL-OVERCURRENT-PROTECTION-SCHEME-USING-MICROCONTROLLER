// Busbar Differential & Overcurrent Protection Scheme Using Microcontroller
// Board: Arduino Uno
// Project: Busbar Protection
//
// Pin assignments:
// A0 - CT1 level-shifter output
// A1 - CT2 level-shifter output
// A2 - SW2 manual reset
// A3 - SW3 delay selection (LOW = 2 s, HIGH = 5 s)
// A4 - SW4 mode selection (LOW = differential, HIGH = overcurrent)
// D12 - Optically isolated thyristor-driver control
//
// IMPORTANT:
// This is an educational prototype, not a certified protection relay.
// Verify the sensor scaling, thresholds, switch wiring, firmware behavior,
// and trip-driver polarity using safe, isolated low-voltage tests before use.
// Mains-connected circuits must only be handled under qualified supervision.

const int CT1_PIN = A0;
const int CT2_PIN = A1;
const int SW2_RESET = A2;
const int SW3_DELAY = A3;
const int SW4_MODE = A4;
const int BREAKER_PIN = 12;

// Protection settings: calibrate these values using the actual hardware.
const int BIAS_OFFSET = 512;                 // Nominal 2.5 V bias for 10-bit ADC
const int DIFFERENTIAL_THRESHOLD = 50;       // Differential threshold in ADC counts
const int OVERCURRENT_THRESHOLD = 700;       // Peak threshold in ADC counts

bool isTripped = false;

void setup() {
  pinMode(BREAKER_PIN, OUTPUT);

  // These pins are analog inputs used to read the switch-board voltage.
  pinMode(SW2_RESET, INPUT);
  pinMode(SW3_DELAY, INPUT);
  pinMode(SW4_MODE, INPUT);

  // Assumes HIGH enables the driver and LOW disables its gate command.
  // Confirm this polarity on the actual isolated driver circuit.
  digitalWrite(BREAKER_PIN, HIGH);

  Serial.begin(9600);
  Serial.println("Busbar Protection System Initialized.");
}

void loop() {
  // Read CT channels and remove the nominal level-shifter bias.
  int ct1_raw = analogRead(CT1_PIN) - BIAS_OFFSET;
  int ct2_raw = analogRead(CT2_PIN) - BIAS_OFFSET;

  // Absolute instantaneous samples; these are not RMS measurements.
  int ct1_val = abs(ct1_raw);
  int ct2_val = abs(ct2_raw);

  // Read switch-board signals. Threshold assumes LOW and HIGH voltages
  // are correctly wired and that the inputs never float.
  bool mode_overcurrent = (analogRead(SW4_MODE) > 300);
  bool long_delay = (analogRead(SW3_DELAY) > 300);
  bool reset_pressed = (analogRead(SW2_RESET) > 300);

  unsigned long trip_delay = long_delay ? 5000UL : 2000UL;

  // Manual reset: release the trip latch when reset is requested.
  if (isTripped && reset_pressed) {
    isTripped = false;
    digitalWrite(BREAKER_PIN, HIGH);
    Serial.println("System Reset. Busbar Restored.");
    delay(500);  // Simple debounce; not a safety interlock.
  }

  // Run protection logic only when the trip latch is clear.
  if (!isTripped) {
    if (mode_overcurrent) {
      // Overcurrent mode: delay, then trip if the condition still exists.
      if (ct1_val > OVERCURRENT_THRESHOLD ||
          ct2_val > OVERCURRENT_THRESHOLD) {
        Serial.print("OVERCURRENT FAULT DETECTED! Delay: ");
        Serial.print(trip_delay / 1000);
        Serial.println(" seconds.");

        delay(trip_delay);

        // Re-sample after the delay so a cleared condition does not
        // automatically cause a trip. This is still a simple demonstration,
        // not a validated protection algorithm.
        ct1_raw = analogRead(CT1_PIN) - BIAS_OFFSET;
        ct2_raw = analogRead(CT2_PIN) - BIAS_OFFSET;
        ct1_val = abs(ct1_raw);
        ct2_val = abs(ct2_raw);

        if (ct1_val > OVERCURRENT_THRESHOLD ||
            ct2_val > OVERCURRENT_THRESHOLD) {
          tripSystem("Overcurrent Violation");
        }
      }
    } else {
      // Differential mode: compare magnitudes of the two sampled channels.
      int diff_current = abs(ct1_val - ct2_val);

      if (diff_current > DIFFERENTIAL_THRESHOLD) {
        tripSystem("Busbar Differential Fault");
      }
    }
  }

  // Periodic diagnostics in the Serial Monitor.
  static unsigned long lastLog = 0;
  if (millis() - lastLog > 300UL) {
    Serial.print("CT1: ");
    Serial.print(ct1_val);
    Serial.print(" | CT2: ");
    Serial.print(ct2_val);
    Serial.print(" | Mode: ");
    Serial.println(mode_overcurrent ? "Overcurrent" : "Differential");
    lastLog = millis();
  }
}

// Disable the gate-drive command and latch the trip state.
void tripSystem(const char *reason) {
  isTripped = true;
  digitalWrite(BREAKER_PIN, LOW);

  Serial.print("!!! SYSTEM TRIPPED !!! Reason: ");
  Serial.println(reason);
}
