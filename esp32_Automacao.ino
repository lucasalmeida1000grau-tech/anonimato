#include <WiFi.h>
#include <HTTPClient.h>

const char* WIFI_SSID = "Lima77";
const char* WIFI_PASSWORD = "LUBRIFICANTEXOTAL";

// IP DO COMPUTADOR QUE ESTÁ RODANDO O NODE.JS
const char* SERVER_URL =
  "http://192.168.1.100:3000/trigger";

const int BOTAO_GPIO = 23;

bool ultimoEstado = HIGH;

unsigned long ultimoClique = 0;

const unsigned long DEBOUNCE_MS = 500;


void conectarWiFi() {

  Serial.print("Conectando ao Wi-Fi");

  WiFi.mode(WIFI_STA);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );

  int tentativas = 0;

  while (
    WiFi.status() != WL_CONNECTED &&
    tentativas < 30
  ) {

    delay(500);

    Serial.print(".");

    tentativas++;

  }

  Serial.println();


  if (
    WiFi.status() == WL_CONNECTED
  ) {

    Serial.println(
      "Wi-Fi conectado!"
    );

    Serial.print(
      "IP do ESP32: "
    );

    Serial.println(
      WiFi.localIP()
    );

  } else {

    Serial.println(
      "Nao foi possivel conectar ao Wi-Fi."
    );

  }

}


void acionarAutomacao() {

  if (
    WiFi.status() != WL_CONNECTED
  ) {

    conectarWiFi();

  }


  if (
    WiFi.status() != WL_CONNECTED
  ) {

    Serial.println(
      "Sem Wi-Fi. Automacao nao enviada."
    );

    return;

  }


  HTTPClient http;

  Serial.println(
    "Enviando comando para o computador..."
  );


  http.begin(
    SERVER_URL
  );

  http.addHeader(
    "Content-Type",
    "application/json"
  );


  int codigo =
    http.POST("{}");


  Serial.print(
    "Resposta HTTP: "
  );

  Serial.println(
    codigo
  );


  if (
    codigo > 0
  ) {

    Serial.println(
      http.getString()
    );

  }


  http.end();

}


void setup() {

  Serial.begin(
    115200
  );

  delay(1000);


  Serial.println(
    "Iniciando ESP32..."
  );


  pinMode(
    BOTAO_GPIO,
    INPUT_PULLUP
  );


  conectarWiFi();

}


void loop() {

  bool estado =
    digitalRead(
      BOTAO_GPIO
    );


  if (
    ultimoEstado == HIGH &&
    estado == LOW
  ) {

    unsigned long agora =
      millis();


    if (
      agora - ultimoClique >=
      DEBOUNCE_MS
    ) {

      ultimoClique =
        agora;


      Serial.println(
        "BOTAO PRESSIONADO!"
      );


      acionarAutomacao();

    }

  }


  ultimoEstado =
    estado;


  delay(20);

}
