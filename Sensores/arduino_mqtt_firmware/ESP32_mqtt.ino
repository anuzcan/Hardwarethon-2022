#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>



//**************************************
//*********** MQTT CONFIG **************
//**************************************
const char *mqtt_server = "driver.cloudmqtt.com";
const int mqtt_port = 18709;
const char *mqtt_user = "hvdscpnh";
const char *mqtt_pass = "RT0dutQN19lg";

/*
const char *type_device = "/fieldsensor";
const char *device_ID = "002";
const char *point_ID = "/tempisque_1";
const char *parameter = "/levelWater";

*/

const char *root_topic_subscribe = "cQpi7CBHWn/garden/simslider/temperature";
const char *root_topic_publish = "alertDisaster/fieldsensor002/tempisque_1/levelWater";



//**************************************
//*********** WIFICONFIG ***************
//**************************************
const char* ssid = "Wokwi-GUEST";
const char* password =  "";



//**************************************
//*********** GLOBALES   ***************
//**************************************
WiFiClient espClient;
PubSubClient client(espClient);
char msg[25];
long count=0;


//************************
//** F U N C I O N E S ***
//************************
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  
  if (!client.connected()) {
		reconnect();
	}

  if (client.connected()){
    String str = "La cuenta es -> " + String(count);
    str.toCharArray(msg,25);
    client.publish(root_topic_publish,msg);
    count++;
    delay(300);
  }
  client.loop();
}




//*****************************
//***    CONEXION WIFI      ***
//*****************************
void setup_wifi(){
	delay(10);
	// Nos conectamos a nuestra red Wifi
	Serial.println();
	Serial.print("Conectando a ssid: ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("Conectado a red WiFi!");
	Serial.println("Dirección IP: ");
	Serial.println(WiFi.localIP());
}



//*****************************
//***    CONEXION MQTT      ***
//*****************************

void reconnect() {

	while (!client.connected()) {
		Serial.print("Intentando conexión Mqtt...");
		// Creamos un cliente ID
		String clientId = "HWthon";
		//clientId += String(random(0xffff), HEX);
		// Intentamos conectar
		if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
			Serial.println("Conectado!");
			// Nos suscribimos
			if(client.subscribe(root_topic_subscribe)){
        Serial.println("Suscripcion ok");
      }else{
        Serial.println("fallo Suscripciión");
      }
		} else {
			Serial.print("falló :( con error -> ");
			Serial.print(client.state());
			Serial.println(" Intentamos de nuevo en 5 segundos");
			delay(5000);
		}
	}
}


//*****************************
//***       CALLBACK        ***
//*****************************

void callback(char* topic, byte* payload, unsigned int length){
	String incoming = "";
	Serial.print("Mensaje recibido desde -> ");
	Serial.print(topic);
	Serial.println("");
	for (int i = 0; i < length; i++) {
		incoming += (char)payload[i];
	}
	incoming.trim();
	Serial.println("Mensaje -> " + incoming);

}
