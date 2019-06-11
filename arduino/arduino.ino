/*
// připojení potřebných knihoven
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// vytvoření proměnných s názvem WiFi sítě a heslem
const char* nazevWifi = "most";
const char* hesloWifi = "brezinka";
*/

#include <SPI.h>
#include <Ethernet.h>


#define VT_PIN A0 
#define AT_PIN A1

byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

char server[] = "eshop.aeko.cz";

int MyRelay = 7;
boolean RelayOn;

//WiFiClient client;
EthernetClient client;
//Client client(server, 80);

void setup(void) {
  // zahájení komunikace po sériové lince
  pinMode(MyRelay, OUTPUT);
  digitalWrite(MyRelay, HIGH);
  RelayOn = true;
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  /* Použit před  kvůli technickým potížímtím než jsem musel změnit arduino z arduino WeMos na Arduino Uno
  // zahájení komunikace po WiFi s připojením
  // na router skrze zadané přihl. údaje
  WiFi.begin(nazevWifi, hesloWifi);
  // čekání na úspěšné připojení k routeru,
  // v průběhu čekání se vytiskne každých
  // 500 milisekund tečka po sériové lince
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // odřádkování a výpis informací o úspěšném připojení
  // včetně přidelené IP adresy od routeru
  Serial.println("");
  Serial.print("Pripojeno k WiFi siti ");
  Serial.println(nazevWifi);
  Serial.print("IP adresa: ");
  Serial.println(WiFi.localIP());
  */

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      Serial.println("Vypni zařízení");
      delay(1);
    }
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

}

void loop(void) {
  switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;

    case 2:
      //renewed success
      Serial.println("Renewed success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    default:
      //nothing happened
      break;
  }
  
  float vt_read = analogRead(VT_PIN);
  float at_read = analogRead(AT_PIN);

  
  float voltage = vt_read * (5.0 / 1024.0) * 5.0;
  float current = at_read * (5.0 / 1024.0);
  float watts = voltage * current;
  float wattHour = watts * 0.00833333333;

  Serial.println();
  Serial.print("Volts: "); 
  Serial.print(voltage, 3);
  Serial.print("\tAmps: ");
  Serial.print(current,3);
  Serial.print("\tWatts: ");
  Serial.println(watts,3);
  Serial.print("\tWattHour: ");
  Serial.println(wattHour,3);
  Serial.println();

  if(!RelayOn){
    digitalWrite(MyRelay, HIGH);
    RelayOn = true;
    delay(3000);  
  }
  if (voltage > 2.5){
    if (client.connect(server, 80)) {
      Serial.println("Connected");
      client.print("GET /baterky/baterky_zapis.php?tabulka=");
      client.print("fialova&Napeti=");
      client.print(voltage);
      client.print("&Proud=");
      client.print(current);
      client.print("&Watt=");
      client.print(watts);
      client.print("&Wh=");
      client.print(wattHour);
      client.println(" HTTP/1.1");
      client.println("Host: eshop.aeko.cz");
      client.println("Connection: close");
      
      client.println();
      Serial.println();
      
      // and add this
      while(client.connected()) {
        while(client.available()) {
          Serial.write(client.read());
        }
      }
    }
    else
      Serial.println("Připojení k webserveru selhalo.");
  }else if(voltage == 0){
    Serial.println("Připoj baterku!"); 
  }else{
    Serial.println("Odpoj baterku!");
    digitalWrite(MyRelay, LOW);
    RelayOn = false;
  }

  if(voltage != 0){
    delay(30000);
  }else{
    delay(5000);
  }
}
