#include <LocalStorage.h>

// ######################################
// -------- ARDUINO FILE SYSTEM ---------
// ######################################

// TODO: Rename to ArduinoFileSystem
void setup()
{
    // start serial on port at 115200 baud and print to serial monitor
    Serial.begin(115200);
    Serial.println("LocalStorage Example");

    // delay for 10 seconds
    delay(10000);

    LittleFSLocalStorage fs;

    // WRITE TO A FILE
    // write "Hello Local Storage" to /test.txt. This will overwrite the file if it already exists.
    fs.writeFile("/test.txt", "Hello Local Storage");

    // write a another text file, /test2.txt
    fs.writeFile("/test2.txt", "Hello File System");

    // READ FROM A FILE
    // read the contents of /test.txt, /test2.txt and print it to Serial
    Serial.println("/test.txt:\n" + fs.readFile("/test.txt"));
    Serial.println("/test2.txt:\n" + fs.readFile("/test2.txt"));
    
    // CHECK IF A FILE (OR DIRECTORY) EXISTS
    // Use the exists function to check if test.txt exists
    Serial.println("/test.txt exists: " + String(fs.exists("/test.txt")));
    
    // LIST FILES IN A DIRECTORY
    // List the content of the root directory
    Serial.println("Content of root:" + fs.lsString("/"));

    // RENAME A FILE
    // Rename /test2.txt to /hello.txt
    fs.rename("/test2.txt", "/hello.txt");

    // List content of the root directory and print
    Serial.println("Content of root:" + fs.lsString("/"));

    // APPEND TO A FILE
    // Append some text to /test.txt. If not '/' preceeds a path, '/' i.e the root directory is assumed.
    fs.appendToFile("test.txt", "\nThis is a useful file system library");
    Serial.println("/test.txt:\n" + fs.readFile("/test.txt"));

    // MAKE A DIRECTORY
    // Make a directory, /data
    fs.mkdir("/data");

    // TOUCH A FILE i.e an empty text file
    // Create 'file_1.txt' in the /data directory
    fs.touch("/data/file_1.txt");
    fs.writeFile("/data/file_1.txt", "Hello File 1");

    // Use the exists function on the directory and the file in it
    Serial.println("/data exists: " + String(fs.exists("/data")));
    Serial.println("/data/file_1.txt exists: " + String(fs.exists("/data/file_1.txt")));

    // Read from the file in the new directory
    Serial.println("/data/file_1.txt:\n" + fs.readFile("/data/file_1.txt"));

    // COPY A FILE
    // Copy a file from the root directory to the new directory
    fs.copyFile("/test.txt", "/data/file_2.txt");

    // MOVE A FILE
    // Move a file from the root directory to the new directory
    fs.moveFile("/hello.txt", "/data/file_3.txt");

    // Create a sub-folder and a file in the sub-folder inside /data
    fs.mkdir("/data/meta");
    fs.writeFile("/data/meta/config.yml", "delay: 100\nduration: 1000");

    // ls the root and new directory
    Serial.println("Content of root:\n" + fs.lsString("/"));
    Serial.println("Content of data:\n" + fs.lsString("/data"));

    // COPY A FOLDER
    // copy the meta directory to the root directory
    fs.copyDir("/data/meta", "/meta");

    // MOVE A FOLDER
    // move /meta to /config and ls root
    fs.moveDir("/meta", "/config");
    Serial.println("Content of root:\n" + fs.lsString("/"));

    // REMOVE A FILE
    // remove test.txt from the root directory
    fs.remove("/test.txt");

    // REMOVE A FOLDER
    // remove the '/data' and '/config' directory from the root directory
    Serial.println("Removing '/data'. Success: " + String(fs.rmdir("/data")));
    Serial.println("Removing '/config'. Success: " + String(fs.rmdir("/config")));

    // ls the root directory
    Serial.println("Content of root:\n" + fs.lsString("/"));
}

void loop()
{
}