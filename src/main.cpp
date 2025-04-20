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
//#include <pwmWrite.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//Pwm pwm = Pwm(); // Create a Pwm object
auto pwm = Adafruit_PWMServoDriver(0x40); // Create a Pwm object with the I2C address 0x40

//////////////////////////////////////
// Sensoren
#define SEACOCK_OPEN_PIN D0
#define SEACOCK_CLOSE_PIN D1
#define WATERSENESOR_PIN D3


// Aktoren am PCA9685

#define LIGHTS_HOPPELANDEKALLEKOJE_PIN 0
#define LIGHTS_HOPPELANDEKALLEKOJE_LEDSTRIP_PIN 1
#define LIGHTS_SALON_PORT_BUG_PIN 2
#define LIGHTS_SALON_PORT_AFTER_PIN 3
#define LIGHTS_PANTRY_OVEN_PIN 4
#define LIGHTS_PANTRY_SINK_PIN 5
#define LIGHTS_SALON_FLOOR_PIN 6

#define SEATHEATER_1_PIN 12
#define SEATHEATER_2_PIN 13
#define SEATHEATER_3_PIN 14
#define SEATHEATER_4_PIN 15

//////////////////////////////////////

using namespace sensesp;

// The setup function performs one-time application initialization.
void setup() {
  SetupLogging();

  // Construct the global SensESPApp() object
  SensESPAppBuilder builder;
  sensesp_app = (&builder)
                    // Set a custom hostname for the app.
                    ->set_hostname("sensesp-BB1")
                    // Optionally, hard-code the WiFi and Signal K server
                    // settings. This is normally not needed.
                    //->set_wifi("My WiFi SSID", "my_wifi_password")
                    //->set_sk_server("192.168.10.3", 80)
                    //->enable_system_info_sensors()
                    ->enable_ota("ThisIsMyPassword!")
                    ->enable_ip_address_sensor()
                    ->get_app();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initial PWM 
pwm.begin(); // Initialize the PWM driver
pwm.setOscillatorFrequency(27000000);
pwm.setPWMFreq(1600);


const unsigned int kInterval = 10000;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Digital Input Seacock Valve Open
  const uint8_t kDigitalInputSCVOPin = SEACOCK_OPEN_PIN;
  
  pinMode(kDigitalInputSCVOPin, INPUT_PULLUP);
  
  auto digital_input_SCVO = std::make_shared<RepeatSensor<bool>>(
    kInterval, [kDigitalInputSCVOPin]() { return digitalRead(kDigitalInputSCVOPin); });

  auto di_SCVO_metadata = std::make_shared<SKMetadata>(
      "",  // No units for boolean values
      "Seacock Valve Open State"  // Value description
  );
  auto di_SCVO_output = std::make_shared<SKOutputBool>(
      "sensors.Seacockvalve.Pantry.Open.value",          // Signal K path
      "/Sensors/Seacockvalve/Pantry/Open/Value",         // configuration path
      di_SCVO_metadata);
  digital_input_SCVO->connect_to(di_SCVO_output);

  // Digital Input Seacock Valve Close
  const uint8_t kDigitalInputSCVCPin = SEACOCK_CLOSE_PIN;
  pinMode(kDigitalInputSCVCPin, INPUT_PULLUP);

  auto digital_input_SCVC = std::make_shared<RepeatSensor<bool>>(
    kInterval, [kDigitalInputSCVCPin]() { return digitalRead(kDigitalInputSCVCPin); });
  
  auto di_SCVC_metadata = std::make_shared<SKMetadata>(
      "",  // No units for boolean values
      "Seacock Valve Close State"  // Value description
  );
  auto di_SCVC_output = std::make_shared<SKOutputBool>(
      "sensors.Seacockvalve.Pantry.Close.value",          // Signal K path
      "/Sensors/Seacockvalve/Pantry/Close/Value",         // configuration path
      di_SCVC_metadata);
  digital_input_SCVC->connect_to(di_SCVC_output);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Digital Input Water Sensor
  const uint8_t kWATERSENESOR_PIN = WATERSENESOR_PIN;
  pinMode(kWATERSENESOR_PIN, INPUT_PULLUP);

  auto digital_input_WS = std::make_shared<RepeatSensor<bool>>(
    kInterval, [kWATERSENESOR_PIN]() { return digitalRead(kWATERSENESOR_PIN); });

  auto di_WS_metadata = std::make_shared<SKMetadata>(
      "",  // No units for boolean values
      "Watersensor Pantry State"  // Value description
  );
  auto di_WS_output = std::make_shared<SKOutputBool>(
      "sensors.Watersensor.Pantry.value",          // Signal K path
      "/Sensors/Watersensor/Pantry/Value",         // configuration path
      di_WS_metadata);
  digital_input_WS->connect_to(di_WS_output);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Part Lights:
//
// Hoppelandkallekoje Spot
auto lhlkk = std::make_shared<SKValueListener<float>>(
    "electrical.inside.hoppelandkallekoje.spot.light.value", CHANGE);
    auto lhlkk_consumer = std::make_shared<LambdaConsumer<float>>(
    [](float input) {
      pwm.setPWM(LIGHTS_HOPPELANDEKALLEKOJE_PIN, input, 0);
      debugI("Hoppelandkallekoje Spot: %f", input);
    });
lhlkk->connect_to(lhlkk_consumer);


// Hoppelandkallekoje LED Strip
auto lhlkksl = std::make_shared<SKValueListener<float>>(
    "electrical.inside.hoppelandkallekoje.ledstrip.light.value", CHANGE);
auto lhlkksl_consumer = std::make_shared<LambdaConsumer<float>>(
    [](float input) {
      pwm.setPWM(LIGHTS_HOPPELANDEKALLEKOJE_LEDSTRIP_PIN, input, 0);
      debugI("Hoppelandkallekoje Streifen: %f", input);
    });
lhlkksl->connect_to(lhlkksl_consumer);

//Salon 1 Bug
auto lsb = std::make_shared<SKValueListener<float>>(
    "electrical.inside.salon.port.bow.light.value", CHANGE);
auto* lsb_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setPWM(LIGHTS_SALON_PORT_BUG_PIN, input, 0);
    debugI("Salon Port Bug: %f", input);
});
lsb->connect_to(lsb_consumer);

// Salon 2 After
auto lsa = std::make_shared<SKValueListener<float>>(
    "electrical.inside.salon.port.stern.light.value", CHANGE);
auto* lsa_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setPWM(LIGHTS_SALON_PORT_AFTER_PIN, input, 0);
    debugI("Salon Port After: %f", input);
});
lsa->connect_to(lsa_consumer);

//Pantry
//Oven
auto lpo = std::make_shared<SKValueListener<float>>(
    "electrical.inside.pantry.oven.light.value", CHANGE);
auto* lpo_consumer = new LambdaConsumer<float>([](float input) {
  pwm.setPWM(LIGHTS_PANTRY_OVEN_PIN, input, 0);  
    debugI("Pantry Oven: %f", input);
});
lpo->connect_to(lpo_consumer);

//Pantry Sink
auto lps = std::make_shared<SKValueListener<float>>(
    "electrical.inside.pantry.sink.light.value", CHANGE);
auto* lps_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setPWM(LIGHTS_PANTRY_SINK_PIN, input, 0);
    debugI("Pantry Sink: %f", input);
});
lps->connect_to(lps_consumer);

//Salon Floor
auto lsf = std::make_shared<SKValueListener<float>>(
    "electrical.inside.salon.port.floor.light.value", CHANGE);
auto* lsf_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setPWM(LIGHTS_SALON_FLOOR_PIN, input, 0);
    debugI("Salon Floor: %f", input);
});
lsf->connect_to(lsf_consumer);


//Starting Bug
auto sh1 = new SKValueListener<float>("electrical.inside.salon.port.seatheater.1.value", CHANGE, "Seatheater 1");
auto* sh1_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setPWM(SEATHEATER_1_PIN, input, 0);
    debugI("SeatHeater 1: %f", input);
});
sh1->connect_to(sh1_consumer);

auto sh2 = new SKValueListener<float>("electrical.inside.salon.port.seatheater.2.value", CHANGE, "Seatheater 2");
auto* sh2_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setPWM(SEATHEATER_2_PIN, input, 0);
    debugI("SeatHeater 2: %f", input);
});
sh2->connect_to(sh2_consumer);

auto sh3 = new SKValueListener<float>("electrical.inside.salon.port.seatheater.3.value", CHANGE, "Seatheater 3");
auto* sh3_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setPWM(SEATHEATER_3_PIN, input, 0);
    debugI("SeatHeater 3: %f", input);
});
sh3->connect_to(sh3_consumer);

auto sh4 = new SKValueListener<float>("electrical.inside.salon.port.seatheater.4.value", CHANGE, "Seatheater 4");
auto* sh4_consumer = new LambdaConsumer<float>([](float input) {
    pwm.setPWM(SEATHEATER_4_PIN, input, 0);
    debugI("SeatHeater 4: %f", input);
});
sh4->connect_to(sh4_consumer);



  // To avoid garbage collecting all shared pointers created in setup(),
  // loop from here.
  while (true) {
    loop();
  }
}

void loop() { event_loop()->tick(); }
