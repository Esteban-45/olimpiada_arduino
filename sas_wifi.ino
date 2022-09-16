#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

//#include <Parsing-impl.h>
//#include <Uri.h>



#define PIN_TRIG D5
#define PIN_ECHO D6

float tiempo, distancia;

String ssid = "Jose";
String password = "zjos1271";
String url = "http://192.168.43.136/PruebaArduino/hola.php?";
int pausa = 800;
WiFiServer server(80);

//IPAddress ip_local(192,168,43,253);
//IPAddress gateway(192,168,43,1);
//IPAddress subnet(255,255,0,0);

unsigned long start_time;

void leerTemperatura() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(4);


  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  tiempo = pulseIn(PIN_ECHO, HIGH);
  distancia = tiempo / 58.3;

  Serial.println(distancia);
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  start_time = millis();

  //Conexion WIFI
 WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
  }
  Serial.println("");
  Serial.println("******************************");
  Serial.print("Conectado a la red WiFi: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("macAdress: ");
  Serial.println(WiFi.macAddress());
  Serial.println("******************************");

  server.begin();



}

void loop() {
  if (millis() - start_time >= pausa) {
    leerTemperatura();
    start_time = millis();
  }

WiFiClient client = server.available(); //objeto de la clase WiFiClient

  //if (!client) {
   // return;
 // }
  //Serial.println("Nuevo Cliente");
  //while (!client.available()) {
   // delay(1);
  //}

  //String peticion = client.readStringUntil('\r');
  //Serial.println(peticion);
  //client.flush(); //limpiua la peticion de los clientes

  //client.println("HTTPS/1.1 200 OK");
  //client.println("");
  //client.println("");
  //client.println("");
  //client.println("");

  //INICIA LA PAGINA

  //client.println("<!DOCTYPE html><html lang='es'><head>");
  //client.println("<meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'>");
  //client.println("<title>Document</title>");
  //client.println("<link rel='stylesheet' href="+css+">");
  //client.println("</head><body class='fondo'><div class='cont'>");
  //client.println("<p class='letra'>PROXIMIDAD: " + String(distancia) + "</p>");
  //client.println("</div></body></html>");

  //FIN DE LA PAGINA



  HTTPClient http;
  String postData = "dis="+String(distancia);
  http.begin(client,url + postData);
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String respuesta = http.getString();

  Serial.println(httpCode);
  Serial.println(respuesta);
  Serial.println(url+String(postData));

  http.end();
  

  delay(5000);
  Serial.println("Peticion finalizada");
  Serial.println("");

}
