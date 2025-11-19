#include <ESP32S3_SIM7670_OTA.h>

#define MODEM_RX 17
#define MODEM_TX 18

const char apn[] = "airtelgprs.com";
const char user[] = "";
const char pass[] = "";
const char server[] = "domain_name.com"; // Don't include "http://" or "https://"

ESP32S3_SIM7670_OTA ota(Serial1, MODEM_RX, MODEM_TX, apn, user, pass, server, 80);

void setup()
{
    Serial.begin(115200);
    Serial.println("\n🚀 Starting OTA Test");

    ota.begin();
    ota.setVersion("1.0.0");
    ota.setDelay(60000); // check every 1 minute
    ota.setPaths("/release/version.txt", "/release/firmware_v");

    ota.checkAndUpdate(); // performs one update check
}

void loop()
{
    // Optional periodic check
    ota.checkAndUpdate();
}
