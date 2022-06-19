#include <LocalStorage.h>

void setup()
{
    
    // start serial on port at 115200 baud and print to serial monitor
    Serial.begin(115200);
    Serial.println("LocalStorage Example");

    // delay for 10 seconds
    delay(10000);

    // write "Hello Local Storage" to /test.txt
    localStorage.writeFile("/test.txt", "Hello Local Storage");

    // read the contents of /test.txt and print to Serial
    Serial.println(localStorage.readFile("/test.txt"));
}

void loop()
{}