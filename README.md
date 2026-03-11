# SensESP BB1 – Backboard 1 (Pantry)

SensESP für die Pantry.

**Hostname:** `sensesp-bb1`  
**Plattform:** ESP32  
**Datei:** `src/main.cpp`

---

## Funktionen

- Status Seeventil Pantry (Offen / Geschlossen)
- Wassersensor Pantry
- Lichtsteuerung (PWM): Hoppelandkallekoje, Salon Port Bug, Salon Port After, Pantry Oven
- Sitzheizung (PWM): 4 Kanäle

---

## GPIO Übersicht

**Gesamt genutzte GPIOs: 11** (3 Inputs, 8 PWM-Outputs)

### Digitale Eingänge

| GPIO | Modus | Funktion | Signal K Pfad | Intervall |
|------|-------|----------|---------------|-----------|
| 36 | INPUT_PULLUP | Seeventil Pantry – **Offen** | `sensors.Seacockvalve.Pantry.Open.value` | 1000 ms |
| 39 | INPUT_PULLUP | Seeventil Pantry – **Geschlossen** | `sensors.Seacockvalve.Pantry.Close.value` | 1000 ms |
| 34 | INPUT_PULLUP | Wassersensor Pantry | `sensors.Watersensor.Pantry.value` | 1000 ms |

### PWM Ausgänge

| GPIO | Funktion | Signal K Pfad |
|------|----------|---------------|
| 32 | Licht – Hoppelandkallekoje Mid | `environment.inside.hoppelandkallekoje.mid.light.value` |
| 33 | Licht – Salon Port Bug | `environment.inside.salon.port.bug.light.value` |
| 25 | Licht – Salon Port After | `environment.inside.salon.port.mid.light.value` |
| 26 | Licht – Pantry Oven | `environment.inside.pantry.oven.light.value` |
| 27 | Sitzheizung Kanal 1 | `environment.inside.salon.port.bug.seatheater.1.value` |
| 14 | Sitzheizung Kanal 2 | `environment.inside.salon.port.bug.seatheater.2.value` |
| 12 | Sitzheizung Kanal 3 | `environment.inside.salon.port.bug.seatheater.3.value` |
| 13 | Sitzheizung Kanal 4 | `environment.inside.salon.port.bug.seatheater.4.value` |

---

## Verwendete Libraries

- [SensESP](https://github.com/SensESP/SensESP) – `sensesp/sensors`, `sensesp/signalk`, `sensesp/system`
- [pwmWrite](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite) – PWM-Steuerung für Licht und Sitzheizung

---

## ⚠️ Bekannter Bug

In `main.cpp` verwendet der Wassersensor (GPIO 34) in der Lambda-Funktion fälschlicherweise `kDigitalInputSCVCPin` (GPIO 39) statt `kDigitalInputWSPin` (GPIO 34). Der Pin wird korrekt als `INPUT_PULLUP` konfiguriert, aber nie gelesen.

**Fix:**
```cpp
// Zeile ~83 – falsch:
[kDigitalInputSCVCPin]() { return digitalRead(kDigitalInputWSPin); }

// Richtig:
[kDigitalInputWSPin]() { return digitalRead(kDigitalInputWSPin); }
```
