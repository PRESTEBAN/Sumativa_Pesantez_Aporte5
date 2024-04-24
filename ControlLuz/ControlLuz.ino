#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

const char* WIFI_SSID = "Me falta el aire aun";
const char* WIFI_PASSWORD = "y el corazon tukun tukun";

// Credenciales de Firebase
const char* API_KEY = "AIzaSyD_kbpLIQEqTJqHvd355rmpPnyEc8Zq7RM";
const char* DATABASE_URL = "https://controlluz-1e524-default-rtdb.firebaseio.com/"; // URL DE LA RTDB
const char* USER_EMAIL = "peruano@peruano.com";
const char* USER_PASSWORD = "123456";

// Objeto para manejar la comunicación con Firebase
FirebaseData fbdo;

// Objeto para la autenticación en Firebase
FirebaseAuth auth;

// Objeto de configuración de Firebase
FirebaseConfig config;

int sensorValue = 0; // Variable para almacenar el valor del LDR

// Nombres de los campos en la base de datos

void setup() {
    Serial.begin(115200);
    setup_WIFI();
    setupFirebase();
    pinMode(35, OUTPUT);
}

void setup_WIFI() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Conectando a Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println("\nConectado a Wi-Fi");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
}

void setupFirebase() {
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;
    config.token_status_callback = tokenStatusCallback;
    config.signer.tokens.legacy_token = "<database secret>";
    Firebase.reconnectNetwork(true);
    fbdo.setBSSLBufferSize(4096, 1024);
    fbdo.setResponseSize(2048);
    Firebase.begin(&config, &auth);
    Firebase.setDoubleDigits(5);
    config.timeout.serverResponse = 10 * 1000;
    Serial.printf("Cliente de Firebase v%s\n\n", FIREBASE_CLIENT_VERSION);
}

void loop() {
    sensorValue = analogRead(35);
    updateField(sensorValue);
    delay(1000);
}

void updateField(int sensorValue) {
    // Actualizar el campo en la base de datos
    if (Firebase.RTDB.setInt(&fbdo, F("/Control luz/ValueLight"), sensorValue)) {
        Serial.println("Valor actualizado correctamente en Firebase");
    } else {
        Serial.println("Error al actualizar el valor en Firebase");
        Serial.println(fbdo.errorReason());
    }
}





