#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BAUD_RATE 115200
#define LED_PIN   16
#define SSID      "ralph_net"
#define PSSWD     "u6perdun"
#define OLED_ADDR 0x3C // OLED display address (for the 128x32)
#define RESET_PIN -1   // no reset pin

WiFiServer server(80);
// reset pin not used on 4-pin OLED module
Adafruit_SSD1306 display(RESET_PIN);  

char line_buf[80];
int char_count = 0;
boolean current_line_is_blank = false;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    
    Serial.begin(BAUD_RATE);
    Serial.print("\nConnecting to ");
    Serial.println(SSID);
    
    WiFi.begin(SSID, PSSWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

    // initialize and clear display       
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();

    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(WiFi.localIP());
    display.display();
}

void loop()
{
    WiFiClient client = server.available();
    if (client)
    {
        Serial.println("\nClient connected");

        memset(line_buf, 0, sizeof(line_buf));
        char_count = 0;
        current_line_is_blank = true;

        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                line_buf[char_count] = c;
                if (char_count < sizeof(line_buf) - 1)
                {
                    char_count++;
                }

                if (c == '\n' && current_line_is_blank)
                {
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    break;
                }
                
                if (c == '\n')
                {
                    if (strstr(line_buf,"GET /led=1") > 0)
                    {
                        digitalWrite(LED_PIN, HIGH);
                        Serial.println("LED is on");

                        display.clearDisplay();
                        display.setCursor(0, 0);
                        display.setTextSize(2);
                        display.setTextColor(WHITE);
                        display.println("LED is on");
                        display.display();
                    }
                    else if (strstr(line_buf,"GET /led=0") > 0)
                    {
                        digitalWrite(LED_PIN, LOW);
                        Serial.println("LED is off");

                        display.clearDisplay();
                        display.setCursor(0, 0);
                        display.setTextSize(2);
                        display.setTextColor(WHITE);
                        display.println("LED is off");
                        display.display();
                    }

                    memset(line_buf, 0, sizeof(line_buf));
                    char_count = 0;
                    current_line_is_blank = true;
                }
                else if (c != '\r')
                {
                    current_line_is_blank = false;
                }
            }
        }

        // give the web browser time to receive the data
        delay(10);

        client.stop();
        Serial.println("Client disconnected");
    }
}
