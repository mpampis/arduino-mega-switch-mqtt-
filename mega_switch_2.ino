#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
byte mac[] = {   0x00, 0xAB, 0xBA, 0xCC, 0xDE, 0x01 };
IPAddress ip(192, 168, 2, 202);
IPAddress server(192, 168, 2, 11);
EthernetClient ethClient;
PubSubClient client(ethClient);
const int buttonPin[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49}; // switch pins

const int lenCnt = 28;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
    char pin[5];
    String str1, str2, str3;
void setup() {

  for (int p = 0; p < lenCnt; p++)
  {
    pinMode(buttonPin[p], INPUT); // Set the mode to INPUT
  }
  Serial.begin(9600);
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(1500);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  for (int p = 0; p < lenCnt; p++)
  {
    buttonState = digitalRead(buttonPin[p]);
    lastButtonState = EEPROM.read(buttonPin[p]);
    EEPROM.update(buttonPin[p], buttonState);
    int minus = buttonPin[p] - 22;

if (buttonPin[p] <= 31){
    //String str;
    str1 = String("0");
    str2 = String(minus);
    str1 += str2;
    }
    else 
{
  str1 =String(minus);
    }

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      Serial.print(buttonPin[p]);
      Serial.println(" ON");
      str3 = String("1");
      str1 += str3;
      str1.toCharArray(pin, 5);

      client.publish("/Home/Center1/", pin);

    } 
    else {
      Serial.print(buttonPin[p]);
      Serial.println(" OFF");
      str3 = String("0");
      str1 += str3;
      str1.toCharArray(pin, 5);
      client.publish("/Home/Center1/" , pin);
    }
   delay(50);
  }
}
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoswitch")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("tele/arduinomega/LWT", "Online");
      // ... and resubscribe
  //    client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

}
