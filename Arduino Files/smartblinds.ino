#include "HomeSpan.h"
#include "BLINDS.h"

void setup() {
  Serial.begin(115200);

  homeSpan.begin(Category::WindowCoverings, "Window 1");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("Window 1");

      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("1");
      new Characteristic::Model("1");
      new Characteristic::FirmwareRevision(".1");

      new Characteristic::Identify();

    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

    new DEV_MyBlind();
}
void loop() {
  homeSpan.poll();
}