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
#include "sensesp/system/lambda_consumer.h"
#include "sensesp_app_builder.h"
#include "pwmWrite.h"

using namespace sensesp;

reactesp::ReactESP app;

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
                    ->get_app();


  // Digital Input Seacock Valve Open
  const uint8_t kDigitalInputSCVOPin = 13;
  const unsigned int kDigitalInputSCVOInterval = 1000;

  // Configure the pin. Replace this with your custom library initialization
  // code!
  pinMode(kDigitalInputSCVOPin, INPUT_PULLUP);

  // Define a new RepeatSensor that reads the pin every 100 ms.
  // Replace the lambda function internals with the input routine of your custom
  // library.

  auto* digital_input_SCVO = new RepeatSensor<bool>(
      kDigitalInputSCVOInterval,
      [kDigitalInputSCVOPin]() { return digitalRead(kDigitalInputSCVOPin); });
  
  // Connect digital input 2 to Signal K output.
  digital_input_SCVO->connect_to(new SKOutputBool(
      "sensors.Seacockvalve.Pantry.Open.value",          // Signal K path
      "/Sensors/Seacockvalve/Pantry/Open/Value",         // configuration path
      new SKMetadata("",                       // No units for boolean values
                     "Seacock Valve Open State")  // Value description
      ));

  // Digital Input Seacock Valve Close
  const uint8_t kDigitalInputSCVCPin = 13;
  const unsigned int kDigitalInputSCVCInterval = 1000;

  // Configure the pin. Replace this with your custom library initialization
  // code!
  pinMode(kDigitalInputSCVCPin, INPUT_PULLUP);

  // Define a new RepeatSensor that reads the pin every 100 ms.
  // Replace the lambda function internals with the input routine of your custom
  // library.

  auto* digital_input_SCVC = new RepeatSensor<bool>(
      kDigitalInputSCVCInterval,
      [kDigitalInputSCVCPin]() { return digitalRead(kDigitalInputSCVCPin); });
  
  // Connect digital input 2 to Signal K output.
  digital_input_SCVC->connect_to(new SKOutputBool(
      "sensors.Seacockvalve.Pantry.Close.value",          // Signal K path
      "/Sensors/Seacockvalve/Pantry/Close/Value",         // configuration path
      new SKMetadata("",                       // No units for boolean values
                     "Seacock Valve Close State")  // Value description
      ));

//Part Lights:
//try to get data from signalk
// Level for lights Hoppelandkallekoje
 
 // GPIO number to use for the PWM output

auto hlkk = new SKValueListener<float>("environment....");
hlkk->connect_to(new LambdaConsumer<float>(
    [](float input) { Pwm pwm = Pwm(); pwm.write(23, input); }));

}

void loop() { app.tick(); }
