// ESP8266 Web Server

// připojení potřebných knihoven
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// vytvoření proměnných s názvem WiFi sítě a heslem
const char* nazevWifi = "most";
const char* hesloWifi = "brezinka";

IPAddress server_addr(192,168,17,109);  // IP of the MySQL *server* here
char user[] = "root";              // MySQL user login username
char password[] = "";        // MySQL user login password

char INSERT_DATA[] = "INSERT INTO arduino.data (hodnota) VALUES ('%.1f')";
char query[128];
float cislo = 0.1;

WiFiClient client;
MySQL_Connection conn((Client *)&client);

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
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
    // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
    sprintf(query, INSERT_DATA, cislo);
    // Execute the query
    cur_mem->execute(query);
    // Note: since there are no results, we do not need to read any data
    // Deleting the cursor also frees up memory used
    delete cur_mem;
    cislo++;
    Serial.println("Data byla zapsána.");
    conn.close();
  }
  else
    Serial.println("Připojení k mysql databazi selhalo.");
  delay(5000);
}
