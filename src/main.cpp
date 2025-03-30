// Signal K application template file.
//
// This application demonstrates core SensESP concepts in a very
// concise manner. You can build and upload the application as is
// and observe the value changes on the serial port monitor.
//
// You can use this source file as a basis for your own projects.
// Remove the parts that are not relevant to you, and add your own code
// for external hardware libraries.

#include "sensesp/sensors/analog_input.h"
#include "sensesp/sensors/digital_input.h"
#include "sensesp/sensors/sensor.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/signalk/signalk_value_listener.h"
#include "sensesp/system/valueconsumer.h"
#include "sensesp/system/lambda_consumer.h"
#include "sensesp_app_builder.h"
#include <pwmWrite.h>

Pwm pwm = Pwm(); // Create a Pwm object

//////////////////////////////////////
#define SEACOCK_OPEN_PIN D0
#define SEACOCK_CLOSE_PIN D1

#define WATERSENESOR_PIN D3

#define LIGHTS_HOPPELANDEKALLEKOJE_PIN 0
#define LIGHTS_SALON_PORT_BUG_PIN 1
#define LIGHTS_SALON_PORT_AFTER_PIN 2
#define LIGHTS_PANTRY_OVEN_PIN 3
#define LIGHTS_PANTRY_SINK_PIN 4
#define LIGHTS_SALON_FLOOR_PIN 5


//////////////////////////////////////

using namespace sensesp;

// The setup function performs one-time application initialization.
void setup() {
  SetupLogging();

  // Construct the global SensESPApp() object
  SensESPAppBuilder builder;
  sensesp_app = (&builder)
                    // Set a custom hostname for the app.
                    ->set_hostname("sensesp-bb1")
                    // Optionally, hard-code the WiFi and Signal K server
                    // settings. This is normally not needed.
                    //->set_wifi("My WiFi SSID", "my_wifi_password")
                    //->set_sk_server("192.168.10.3", 80)
                    //->enable_system_info_sensors()
                    ->enable_ota("ThisIsMyPassword!")
                    ->get_app();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Digital Input Seacock Valve Open
  const uint8_t kDigitalInputSCVOPin = SEACOCK_OPEN_PIN;
  const unsigned int kInterval = 1000;

    pinMode(kDigitalInputSCVOPin, INPUT_PULLUP);
    auto* digital_input_SCVO = new RepeatSensor<bool>(
        kInterval,
      [kDigitalInputSCVOPin]() { return digitalRead(kDigitalInputSCVOPin); });

    // Connect digital input 2 to Signal K output.
    digital_input_SCVO->connect_to(new SKOutputBool(
      "sensors.Seacockvalve.Pantry.Open.value",          // Signal K path
      "/Sensors/Seacockvalve/Pantry/Open/Value",         // configuration path
      new SKMetadata("",                       // No units for boolean values
                     "Seacock Valve Open State")  // Value description
    ));



  // Digital Input Seacock Valve Close
  const uint8_t kDigitalInputSCVCPin = SEACOCK_CLOSE_PIN;

  // Configure the pin. Replace this with your custom library initialization
  // code!
  pinMode(kDigitalInputSCVCPin, INPUT_PULLUP);

  // Define a new RepeatSensor that reads the pin every 100 ms.
  // Replace the lambda function internals with the input routine of your custom
  // library.

  auto* digital_input_SCVC = new RepeatSensor<bool>(
    kInterval,
      [kDigitalInputSCVCPin]() { return digitalRead(kDigitalInputSCVCPin); });
  
  // Connect digital input 2 to Signal K output.
  digital_input_SCVC->connect_to(new SKOutputBool(
      "sensors.Seacockvalve.Pantry.Close.value",          // Signal K path
      "/Sensors/Seacockvalve/Pantry/Close/Value",         // configuration path
      new SKMetadata("",                       // No units for boolean values
                     "Seacock Valve Close State")  // Value description
      ));


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Digital Input Water Sensor
    const uint8_t kWATERSENESOR_PIN = WATERSENESOR_PIN;

  pinMode(kWATERSENESOR_PIN, INPUT_PULLUP);

  // Define a new RepeatSensor that reads the pin every 100 ms.
  // Replace the lambda function internals with the input routine of your custom
  // library.

  auto* digital_input_WS = new RepeatSensor<bool>(
    kInterval, 
    [kWATERSENESOR_PIN]() { return digitalRead(kWATERSENESOR_PIN); });
  
  // Connect digital input 2 to Signal K output.
  digital_input_WS->connect_to(new SKOutputBool(
      "sensors.Watersensor.Pantry.value",          // Signal K path
      "/Sensors/Watersensor/Pantry/Value",         // configuration path
      new SKMetadata("",                       // No units for boolean values
                     "Watersensor Pantry State")  // Value description
      ));
    

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Part Lights:
//
// Hoppelandkallekoje
auto lhlkk = new FloatSKListener("electrical.inside.hoppelandkallekoje.mid.light.value", CHANGE);
auto* lhlkk_consumer = new LambdaConsumer<float>([](float input) {
  pwm.setFrequency(LIGHTS_HOPPELANDEKALLEKOJE_PIN, 1000); // Set frequency to 1kHz
  pwm.write(LIGHTS_HOPPELANDEKALLEKOJE_PIN, input);
    debugI("Hoppelandkallekoje: %f", input);
});
lhlkk->connect_to(lhlkk_consumer);

//Salon 1 Bug
auto lsb = new SKValueListener<float>("electrical.inside.salon.port.bug.light.value", CHANGE);
auto* lsb_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setFrequency(LIGHTS_SALON_PORT_BUG_PIN, 1000); // Set frequency to 1kHz
    pwm.write(LIGHTS_SALON_PORT_BUG_PIN, input);
    debugI("Salon Port Bug: %f", input);
});
lsb->connect_to(lsb_consumer);

// Salon 2 After
auto lsa = new SKValueListener<float>("electrical.inside.salon.port.mid.light.value", CHANGE);
auto* lsa_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setFrequency(LIGHTS_SALON_PORT_AFTER_PIN, 1000); // Set frequency to 1kHz
    pwm.write(LIGHTS_SALON_PORT_AFTER_PIN, input);
  debugI("Salon Port After: %f", input);
});
lsa->connect_to(lsa_consumer);

//Pantry
//Oven
auto lpo = new SKValueListener<float>("electrical.inside.pantry.oven.light.value", CHANGE);
auto* lpo_consumer = new LambdaConsumer<float>([](float input) {
  pwm.setFrequency(LIGHTS_PANTRY_OVEN_PIN, 1000); // Set frequency to 1kHz
  pwm.write(LIGHTS_PANTRY_OVEN_PIN, input);  
    debugI("Pantry Oven: %f", input);
});
lsa->connect_to(lpo_consumer);

//Pantry Sink
auto lps = new SKValueListener<float>("electrical.inside.pantry.sink.light.value", CHANGE);
auto* lps_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setFrequency(LIGHTS_PANTRY_SINK_PIN, 1000); // Set frequency to 1kHz
    pwm.write(LIGHTS_PANTRY_SINK_PIN, input);
    debugI("Pantry Sink: %f", input);
});
lps->connect_to(lps_consumer);

//Salon Floor
auto lsf = new SKValueListener<float>("electrical.inside.salon.floor.light.value", CHANGE);
auto* lsf_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setFrequency(LIGHTS_SALON_FLOOR_PIN, 1000); // Set frequency to 1kHz
    pwm.write(LIGHTS_SALON_FLOOR_PIN, input);
    debugI("Salon Floor: %f", input);
});
lsf->connect_to(lsf_consumer);

  // To avoid garbage collecting all shared pointers created in setup(),
  // loop from here.
  while (true) {
    loop();
  }
}

void loop() { event_loop()->tick(); }
