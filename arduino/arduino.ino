// ESP8266 Web Server

// připojení potřebných knihoven
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// vytvoření proměnných s názvem WiFi sítě a heslem
const char* nazevWifi = "most";
const char* hesloWifi = "brezinka";

char server[] = "eshop.aeko.cz";
//char mysql[] = "md40.wedos.net";

float cislo = 0.1;
WiFiClient client;

// propojovací pin indikační LED diody
#define LEDka 14

void setup(void) {
  // nastavení LED diody jako výstupní a její vypnutí
  pinMode(LEDka, OUTPUT);
  digitalWrite(LEDka, LOW);
  // zahájení komunikace po sériové lince
  Serial.begin(9600);
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

}

void loop(void) {
  if (client.connect(server, 80)) {
    cislo++;
    Serial.println("Connected");
    client.print("GET /baterky/baterky_zapis.php?data=");
    client.print(cislo);
    client.println(" HTTP/1.1");
    client.println("Host: eshop.aeko.cz");
    Serial.println("hotovo: ");
    Serial.print(cislo);
    // add this
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
    
  delay(5000);
}
