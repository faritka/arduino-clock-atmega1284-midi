#include "ClockSync.h"

/**
 * Constructor
 */
ClockSync::ClockSync(ClockConfig *clockConfig, ClockMusic *clockMusic, ClockScreen *clockScreen) 
    : config(clockConfig), music(clockMusic), screen(clockScreen)
{
    updateCounter = 0;
}

void ClockSync::update()
{
    //the number of the tries before giving up
    const byte numberOfTries = 5;

    //show the infor on the clock screen
    screen->showMenuValue("Syncing", true);

    while (updateCounter < numberOfTries) {    
        if (getFromServer()) {                
#ifdef DEBUG
            Serial.println(F("The SD sync was successful"));
#endif
            screen->showMenuValue("Success", true);
            break;
        }
        else {
#ifdef DEBUG
            Serial.println(F("The SD sync from the Internet has failed"));
#endif
            screen->showMenuValue("Failed", true);
            updateCounter++;
        }
    }

}

boolean ClockSync::getFromServer()
{
    //the error flag
    boolean noError = true;

    //wait for this time for the data
    const unsigned int readTimeout = 30000;
    unsigned long startTime = millis();

    //the number of  files
    int numberOfFiles = 0;
    int i = 0;

    //connect to the internet
    ClockInternet internet(config);
    WiFly wifi = internet.connect();

    if (!strlen(config->getSyncServerName())) {
        Serial.println(F("The sync server name is not defined"));
        return false;
    } 

    if (!strlen(config->getSyncServerUrl())) {
        Serial.println(F("The sync server URL is not defined"));
        return false;
    } 

    //if not associated, the connection has failed 
    if (!wifi.isAssociated()) {
        return false;
    }

    //the old connection is active, close it
    if (wifi.isConnected()) {
        wifi.close();
    }

    //connect to the webserver
    if (wifi.open(config->getSyncServerName(), 80)) {
        Serial.print("Connected to ");
        Serial.println(config->getSyncServerName());

        //send the request
        //the 2-day forecast
        wifi.print("GET ");
        wifi.print(config->getSyncServerUrl());
        wifi.println(" HTTP/1.0");
        wifi.print("Host: ");
        wifi.println(config->getSyncServerName());
        wifi.println();
    }
    else {
#ifdef DEBUG
        Serial.print(F("Failed to connect to the sync server: "));
        Serial.println(config->getSyncServerName());
#endif
        return false;
    }

    //read the response from the server    
    //skip the HTTP headers
    if (!wifi.match(F("\r\n\r\n"), readTimeout)) {
        Serial.println(F("No HTTP header match "));
        return false;
    }

    //the stream to read the JSON data
    aJsonStream jsonStream(&wifi);
    aJsonObject *root = aJson.parse(&jsonStream);

    if (!root) {
#ifdef DEBUG
        Serial.println(F("No JSON data"));
#endif
        return false;
    }

    aJsonObject *serverName = aJson.getObjectItem(root, "serverName"); 
    if (!serverName) {
        Serial.println(F("No server name "));
        return false;
    }

    Serial.print(F("The server Name: "));
    Serial.println(serverName->valuestring);

    aJsonObject *receivedFiles = aJson.getObjectItem(root, "files"); 
    if (!receivedFiles) {
        Serial.println(F("No files received"));
        return false;
    }

    if (wifi.isConnected()) {
        Serial.println("Closing the first connection");
        wifi.close();
    }

    numberOfFiles = aJson.getArraySize(receivedFiles);
    Serial.print(F("Array size: "));
    Serial.println(numberOfFiles);

    //store each file information
    ClockFileInfo files[numberOfFiles];

    aJsonObject *fileName = NULL;
    aJsonObject *fileSize = NULL;
    aJsonObject *fileUrl = NULL;

    for (i = 0; i < numberOfFiles; i++) {
        aJsonObject *receivedFile = aJson.getArrayItem(receivedFiles, i);
        if (!receivedFile) {
            continue;
        }
        fileName = aJson.getObjectItem(receivedFile, "file"); 
        fileSize = aJson.getObjectItem(receivedFile, "size"); 
        fileUrl = aJson.getObjectItem(receivedFile, "url"); 

        //save the values
        files[i].size = atol(fileSize->valuestring);
        files[i].name = fileName->valuestring;
        files[i].url = fileUrl->valuestring;
    }

    //clean the memory
    aJson.deleteItem(root);

    for (i = 0; i < numberOfFiles; i++) {
        if (!updateFile(files[i], internet, wifi, config->getSyncServerName())) {
            noError = false;
        }
    }

    //get all files on SD beginning from the root and delete those that are not on the server
    File rootDirectory = SD.open("/");
    deleteOldFiles(rootDirectory, files, numberOfFiles);
    rootDirectory.close();

    //update the list of songs from the SD card
    if (noError) {
        music->getSongList();    
    }

    return noError;
}

boolean ClockSync::updateFile(ClockFileInfo &fileInfo, ClockInternet &internet, WiFly &wifi, const char *site)
{
    //the error flag
    boolean noError = false;

    //the header match timeout
    const unsigned int timeout = 10000;
    //the file read timeout
    const unsigned long readTimeout = 600000L;
    //the buffer to read the file data
    byte buf[256];
    //SD file
    File file;

    //from String to char
    char fileName[fileInfo.name.length() + 1];
    fileInfo.name.toCharArray(fileName, fileInfo.name.length() + 1);

    //the directory of the file
    char fileDir[fileInfo.name.length() + 1];
    strcpy(fileDir, fileName);
    char *p = strrchr(fileDir, '/');
    if (p) {
        *p = '\0';
    }
    else {
        fileDir[0] = '\0';
    }

#ifdef DEBUG
     Serial.print(F("fileName: "));
     Serial.println(fileInfo.name);
     Serial.print(F("fileSize: "));
     Serial.println(fileInfo.size);
     Serial.print(F("fileUrl: "));
     Serial.println(fileInfo.url);
     Serial.print(F("fileDir: "));
     Serial.println(fileDir);
#endif

    //check if the same file already exists
    if (SD.exists(fileName)) {
#ifdef DEBUG
        Serial.println(F("File exists"));
#endif
        file = SD.open(fileName, FILE_READ);
        if (file) {
#ifdef DEBUG
            Serial.print(F("Existing file size: "));
            Serial.println(file.size());
#endif

            if (file.size() == fileInfo.size) {
#ifdef DEBUG
                Serial.print(F("File already exists, not updated: "));
                Serial.println(fileName);
#endif
                file.close();
                return true;
            }
            file.close();
        }
    }

#ifdef DEBUG
    //get and write the file
    Serial.print(F("Getting the file"));
    Serial.println(fileName);
#endif

    //connect to the webserver
    if (wifi.open(site, 80)) {
#ifdef DEBUG
        Serial.print("Connected to ");
        Serial.println(site);
#endif
        //send the request
        wifi.print("GET ");
        wifi.print(fileInfo.url);
        wifi.println(" HTTP/1.0");
        wifi.print("Host: ");
        wifi.println(site);
        wifi.println();
    }
    else {
#ifdef DEBUG
        Serial.print(F("Failed to connect to the file update server: "));
        Serial.println(site);
#endif
        return false;
    }

    //create the dir if it doesn't exist
    if (strlen(fileDir) && !SD.exists(fileDir)) {
        if (!SD.mkdir(fileDir)) {
#ifdef DEBUG
            Serial.print(F("Cannot create the dir: "));
            Serial.println(fileDir);
#endif
            return false;
        }
    }

    //write the file to SD
    file = SD.open(fileName, O_WRITE | O_CREAT | O_TRUNC);
    if (!file) {
#ifdef DEBUG
        Serial.print(F("Cannot create the file for writing: "));
        Serial.println(fileName);
#endif
        return false;
    }

    //read the response from the server    
    //skip the HTTP headers
    if (!wifi.match(F("\r\n\r\n"), timeout)) {
#ifdef DEBUG
        Serial.println(F("No HTTP header match "));
#endif
        return false;
    }

    //read the file from the Internet
    unsigned long startTime = millis();
    int receivedBytes = 0;
    unsigned long totalBytes = 0;

    while (1) {
        receivedBytes = internet.read(buf, sizeof(buf));

        //write to SD
        file.write(buf, receivedBytes);

        totalBytes += receivedBytes;
        //Serial.print("Buf [");
        //Serial.write(buf, receivedBytes);
        //Serial.println("]");        

        //Serial.print("Total bytes: ");
        //Serial.println(totalBytes);  

        //all bytes have been received
        if (totalBytes == fileInfo.size) {
#ifdef DEBUG
            Serial.println(F("All bytes have been received"));    
#endif
            screen->showMenuValue(String("+") + fileInfo.name, true);

            noError = true;
            break;
        }

        //nothing has been received
        if (receivedBytes == 0) {
#ifdef DEBUG
            Serial.println(F("0 bytes have been received"));
#endif
            break;
        }

        //if the timeout has been reached
        if ((millis() - startTime) > readTimeout) {
#ifdef DEBUG
            Serial.println(F("The timeout has been reached"));
#endif
            break;
        }
    }

    file.close();
    if (wifi.isConnected()) {
        wifi.close();
    }

    return noError;
}

void ClockSync::deleteOldFiles(File &dir, ClockFileInfo *files, int numberOfFiles)
{
    int i = 0;

    //in case this is not the first time we opened the directory, rewind it
    dir.rewindDirectory();

    while (true) {
        bool found = false;
        File entry = dir.openNextFile();
        //skip the slash if it's the root directory
        String fileName = (!strcmp(dir.name(), "/") ? String("") : String("/") + dir.name());

        if (!entry) {
#ifdef DEBUG
            Serial.println(F("No more files"));
#endif
            break;
        }

        if (entry.isDirectory()) {
            Serial.println("/");
            deleteOldFiles(entry, files, numberOfFiles);
        }
        else {
            fileName += String("/") + entry.name();
#ifdef DEBUG
            Serial.print(F("Filename: "));
            Serial.println(fileName);
#endif
        }

        entry.close();

        //don't touch directories
        if (entry.isDirectory()) {
            continue;
        }

        //check if it exists in the list of files on the server
        for (i = 0; i < numberOfFiles; i++) {
            if (files[i].name.equalsIgnoreCase(fileName)) {
                Serial.print(F("Found: "));
                Serial.println(fileName);
                found = true;
                break;
            }
        }

        //delete the file if not found
        if (!found) {
            char tmp[fileName.length() + 1];
            fileName.toCharArray(tmp, fileName.length() + 1);
            if (!SD.remove(tmp)) {
#ifdef DEBUG
                Serial.print(F("Can't delete the file: "));
                Serial.println(tmp);
#endif
            }
            else {
#ifdef DEBUG
                Serial.print(F("Deleted: "));
                Serial.println(tmp);
#endif
            }
        }
    }
}

