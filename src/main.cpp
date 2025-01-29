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
                    ->enable_system_info_sensors()
                    ->get_app();


   // Digital Input Seacock Valve Open
  const uint8_t kDigitalInputSCVOPin = 36;
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
  const uint8_t kDigitalInputSCVCPin = 39;
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



// Digital Input Water Sensor
  const uint8_t kDigitalInputWSPin = 34;
  const unsigned int kDigitalInputWSInterval = 1000;

  // Configure the pin. Replace this with your custom library initialization
  // code!
  pinMode(kDigitalInputWSPin, INPUT_PULLUP);

  // Define a new RepeatSensor that reads the pin every 100 ms.
  // Replace the lambda function internals with the input routine of your custom
  // library.

  auto* digital_input_WS = new RepeatSensor<bool>(
      kDigitalInputWSInterval,
      [kDigitalInputSCVCPin]() { return digitalRead(kDigitalInputWSPin); });
  
  // Connect digital input 2 to Signal K output.
  digital_input_WS->connect_to(new SKOutputBool(
      "sensors.Watersensor.Pantry.value",          // Signal K path
      "/Sensors/Watersensor/Pantry/Value",         // configuration path
      new SKMetadata("",                       // No units for boolean values
                     "Seacock Valve Close State")  // Value description
      ));

//Part Lights:
//
// Level for lights Hoppelandkallekoje
auto lhlkk = new SKValueListener<float>("environment.inside.hoppelandkallekoje.mid.light.value");
auto* lhlkk_consumer = new LambdaConsumer<float>([](float input) {
    Pwm pwm = Pwm();
    pwm.setFrequency(39,200000);
    pwm.write(39, input);
});
lhlkk->connect_to(lhlkk_consumer);

//Salon
// 1 Bug
auto lsb = new SKValueListener<float>("environment.inside.salon.port.bug.light.value");
auto* lsb_consumer = new LambdaConsumer<float>([](float input) {
    Pwm pwm = Pwm();
    pwm.setFrequency(33,200000);
    pwm.write(33, input); 
});
lsb->connect_to(lsb_consumer);

// 2 After
auto lsa = new SKValueListener<float>("environment.inside.salon.port.mid.light.value");
auto* lsa_consumer = new LambdaConsumer<float>([](float input){
    Pwm pwm = Pwm();
    pwm.setFrequency(25,200000);
    pwm.write(25, input); 
});
lsa->connect_to(lsa_consumer);

//Pantry
//Oven
auto lpo = new SKValueListener<float>("environment.inside.pantry.oven.light.value");
auto lpo_consumer = new LambdaConsumer<float>([](float input) {
    Pwm pwm = Pwm(); 
    pwm.setFrequency(26,200000);
    pwm.write(26, input); 
});
lsa->connect_to(lpo_consumer);


//Seatheater
//Starting Bug
auto sh1 = new SKValueListener<float>("environment.inside.salon.port.bug.seatheater.1.value");
auto* sh1_consumer = new LambdaConsumer<float>([](float input) { 
    Pwm pwm = Pwm(); 
    pwm.setFrequency(27,200000);
    pwm.write(27, input); 
});
sh1->connect_to(sh1_consumer);

auto sh2 = new SKValueListener<float>("environment.inside.salon.port.bug.seatheater.2.value");
auto* sh2_conusmer = new LambdaConsumer<float>([](float input) { 
    Pwm pwm = Pwm(); 
    pwm.setFrequency(14,200000);
    pwm.write(14, input); 
});
sh2->connect_to(sh2_conusmer);

auto sh3 = new SKValueListener<float>("environment.inside.salon.port.bug.seatheater.3.value");
auto* sh3_consumer = new LambdaConsumer<float>([](float input) { 
    Pwm pwm = Pwm(); 
    pwm.setFrequency(12,200000);
    pwm.write(12, input); 
});
sh3->connect_to(sh3_consumer);

auto sh4 = new SKValueListener<float>("environment.inside.salon.port.bug.seatheater.4.value");
auto* sh4_consumer = new LambdaConsumer<float>([](float input) { 
    Pwm pwm = Pwm(); 
    pwm.setFrequency(13,200000);
    pwm.write(13, input); 
});
sh4->connect_to(sh4_consumer);
}

void loop() { SensESPBaseApp::get_event_loop()->tick(); }