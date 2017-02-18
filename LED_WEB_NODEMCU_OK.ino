#include <ESP8266WiFi.h>

const char* ssid     = "NETLIFE-ACOSTA"; //Aquí tu red
const char* password = "****.****.****"; //Aquí tu contraseña
int ledPin = 13;      //Corresponde a GPIO13
WiFiServer server(80);

void setup() 
  {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  //Conexiones de Red

  Serial.println();
  Serial.println();
  Serial.print("Conectando a... ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  
  Serial.println("");
  Serial.println("WiFi connected");

  //Iniciamos el servidor
  server.begin();
  Serial.println("Servidor iniciado");

  //Imprimir la Dirección IP
  Serial.print("Usr la siguiente Dirección IP para enlazar: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  }

void loop() 

{
  // Comprobaremos si se ha conectado el cliente
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  //Esperar hasta que el cliente envíe datos
    Serial.print("nuevo cliente");
    while(!client.available()){
    delay(1);
  }
  //Leer la primera línea de pedido
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

  //Enlazar el pedido

  int value = LOW;
  if (request.indexOf("/LED=ON")!=-1)
  {
    digitalWrite(ledPin,HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF")!=-1)
  {
    digitalWrite(ledPin,LOW);
    value = LOW;
  }

  //Enlazar el led de acuerdo al pedido
  //Regresar la respuesta

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //importante, no olvidarse este espacio
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("Estado del led es ahora: ");

  if(value==HIGH)
    {
    client.print("On");
    }
  else
    {
    client.print("Off");
    }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>ENCENDER</button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>APAGAR</button></a><br />"); 
  client.println("</html>");

   delay(1);
   Serial.println("Cliente desconectado");
   Serial.println("");
}
