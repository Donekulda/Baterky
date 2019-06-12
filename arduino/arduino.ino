/*
// připojení potřebných knihoven pro ESP
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// vytvoření proměnných s názvem WiFi sítě a heslem
const char* nazevWifi = "ssid wifi routeru";
const char* hesloWifi = "heslo";
*/

#include <SPI.h>
#include <Ethernet.h>

#define VT_PIN A0 
#define AT_PIN A1

byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

char server[] = "eshop.aeko.cz";

int MyRelay = 7; //Pin relé
boolean RelayOn; // Pokud relé je High(zapnuté) tak se vybijí baterie, důvod je ten aby se nepřevibíjela baterie
boolean VoltMeterOnly = true; //pokud je na true tak se sepne rele neměří se proud a baterie se nevybijí, pokud false tak se přepne relé a baterie se vybíji a měří se proud

unsigned long time; //Zapisuje se semka hodnota posledního měření v milisekudách
unsigned long hours = 0; //proměná na které je zapsána hodnota z převodu milisekund na hodiny

//WiFiClient client;
EthernetClient client;

void setup(void) {  
  pinMode(MyRelay, OUTPUT);
  digitalWrite(MyRelay, LOW); //vypnutí relé z důvodu zabránění vybíjení baterie když ještě není připojeno arduino k internetu
  RelayOn = true;
  //Serial.begin(9600);
  /* Použit předtím(při používání ESP), kvůli technickým potížímtím než jsem musel změnit arduino z arduino WeMos na Arduino Uno
  // zahájení komunikace po WiFi s připojením
  // na router skrze zadané přihl. údaje
  WiFi.begin(nazevWifi, hesloWifi);
  // čekání na úspěšné připojení k routeru,
  // v průběhu čekání se vytiskne každých
  // 500 milisekund tečka po sériové lince
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  // odřádkování a výpis informací o úspěšném připojení
  // včetně přidelené IP adresy od routeru
  //Serial.println("");
  //Serial.print("Pripojeno k WiFi siti ");
  //Serial.println(nazevWifi);
  //Serial.print("IP adresa: ");
  //Serial.println(WiFi.localIP());
  */

  // start the Ethernet connection:
  //Serial.println("Initialize Ethernet with DHCP:");
    if (Ethernet.begin(mac) == 0) {
      //Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      //Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      //Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      //Serial.println("Vypni zařízení");
      delay(1);
    }
  }
  // print your local IP address:
  //Serial.print("My IP address: ");
  //Serial.println(Ethernet.localIP());
  if(!VoltMeterOnly){
    digitalWrite(MyRelay, HIGH); //Sepne se relé aby začala se vybíjet baterie
    delay(1000);
  }
  ////time = millis(); //Získá si čas od sepnutí relé pro zvýšení přesnosti měření, bohužel pár prvních sekund nám vytváří nepřesnost 

}

void loop(void) {
  switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      //Serial.println("Error: renewed fail");
      if(!VoltMeterOnly){
        digitalWrite(MyRelay, LOW); //Vypne se vybíjení baterie a počká se 2 sekundy protože relé má nějaké spoždění
        delay(1000);
        ////time = millis();
      }
      break;

    case 2:
      //renewed success
      //Serial.println("Renewed success");
      //print your local IP address:
      //Serial.print("My IP address: ");
      //Serial.println(Ethernet.localIP());
      if(!VoltMeterOnly){
        digitalWrite(MyRelay, HIGH); //Vypne se vybíjení baterie a počká se 2 sekundy protože relé má nějaké spoždění
        delay(1000);
        ////time = millis();
      }
      break;

    case 3:
      //rebind fail
      //Serial.println("Error: rebind fail");
      if(!VoltMeterOnly){
        digitalWrite(MyRelay, LOW); //Vypne se vybíjení baterie a počká se 2 sekundy protože relé má nějaké spoždění
        delay(1000);
        ////time = millis();
      }
      break;

    case 4:
      //rebind success
      //Serial.println("Rebind success");
      //print your local IP address:
      //Serial.print("My IP address: ");
      //Serial.println(Ethernet.localIP());
      if(!VoltMeterOnly){
        digitalWrite(MyRelay, HIGH); //Vypne se vybíjení baterie a počká se 2 sekundy protože relé má nějaké spoždění
        delay(1000);
        ////time = millis();
      }
      break;

    default:
      //nothing happened
      break;
  }
  
  if(!RelayOn && !VoltMeterOnly){ //Důvod je kvůli tomu aby se otestovalo že baterie byla vyměněna/výjmuta
    digitalWrite(MyRelay, HIGH);
    RelayOn = true;
    delay(1000);
    //////time = millis();
  }
  
  if(VoltMeterOnly){
    digitalWrite(MyRelay, LOW); //Když se vypne rele tak se měří jenom napětí 
  }
  
  float vt_read = analogRead(VT_PIN);
  float at_read = analogRead(AT_PIN);

  
  float voltage = vt_read * (5.0 / 1024.0) * 5.0;
  float current = at_read * (5.0 / 1024.0);
  float watts = voltage * current;
  //hours = time/3600000; //nezdařený pokus počítat pomocí milisekund od startu arduina
  //float wattHour = watts * hours; //miliseconds to hours
  //time = millis();
  float wattHour2 = watts * 0.00833333333;

  //Serial.println();
  //Serial.print("Volts: "); 
  //Serial.print(voltage, 3);
  //Serial.print("\tAmps: ");
  //Serial.print(current,3);
  //Serial.print("\tWatts: ");
  //Serial.println(watts,3);
  ////Serial.print("\tWattHour: ");
  ////Serial.print(wattHour,9);
  ////Serial.println();
  //Serial.print("\tWattHour2: ");
  //Serial.print(wattHour2,9);
  //Serial.println();
  ////Serial.print("\tPocet hodin od posledniho měření: ");
  ////Serial.print(hours);
  //Serial.println();

  if (voltage > 2.5){
    if (client.connect(server, 80)) {
      //Serial.println("Connected");
      client.print("GET /baterky/baterky_zapis.php?tabulka=");
      client.print("stav&Napeti=");
      client.print(voltage);
      client.print("&Proud=");
      client.print(current);
      client.print("&Watt=");
      client.print(watts);
      client.print("&Wh=");
      client.print(wattHour2);
      client.println(" HTTP/1.1");
      client.println("Host: eshop.aeko.cz");
      client.println("Connection: close");
      
      client.println();
      //Serial.println();
      
      // and add this
      while(client.connected()) {
        while(client.available()) {
          //Serial.write(client.read());
        }
      }
    }
    //else
      //Serial.println("Připojení k webserveru selhalo.");
  }else if(voltage == 0){
    //Serial.println("Připoj baterku!"); 
  }else{
    //Serial.println("Odpoj baterku!");
    digitalWrite(MyRelay, LOW);
    RelayOn = false;
  }

  if(voltage != 0){ //Pokud baterie není zapnutá tak kontroluj v intervalu 5 sekund
    delay(30000);
  }else if(VoltMeterOnly){
    delay(15000);
  }else{  
    delay(5000);
  }
}
