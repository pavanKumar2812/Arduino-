#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server;
const char* Ssid = "TechMMakers";
const char* Password = "!nn0vationFactory";
// hold uploaded file
File FSUploadedFile;

void setup()
{
  SPIFFS.begin();
  WiFi.begin(Ssid,Password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HandleIndexFile);
  // list available files
  server.on("/list", HTTP_GET, HandleFileList);
  // handle file upload
  server.on("/upload", HTTP_POST, [](){
    server.send(200, "text/plain", "{\"success\":1}");
  }, HandleFileUpload);
  server.begin();
}

void loop()
{
  server.handleClient();
}

void HandleFileUpload()
{
  HTTPUpload& Upload = server.upload();
  if(Upload.status == UPLOAD_FILE_START)
  {
    String FileName = Upload.filename;
    if(!FileName.startsWith("/"))
    {
      FileName = "/" + FileName;
    }
    Serial.print("handleFileUpload Name: "); Serial.println(FileName);
    FSUploadedFile = SPIFFS.open(FileName, "w");
  } 
  else if(Upload.status == UPLOAD_FILE_WRITE)
  {
    if(FSUploadedFile) 
    {
      FSUploadedFile.write(Upload.buf, Upload.currentSize);
    } 
  }
  else if(Upload.status == UPLOAD_FILE_END)
  {
    if(FSUploadedFile)
    {
      FSUploadedFile.close();
    }
    Serial.print("HandleFileUpload Size: "); Serial.println(Upload.totalSize);
  }
}

void HandleFileList()
{
  String Path = "/";
  // Assuming there are no subdirectories
  Dir Directorie = SPIFFS.openDir(Path);
  String Output = "[";
  while(Directorie.next())
  {
    File Entry = Directorie.openFile("r");
    // Separate by comma if there are multiple files
    if(Output != "[")
    {
      Output += ",";
    }
    Output += String(Entry.name()).substring(1);
    Entry.close();
  }
  Output += "]";
  server.send(200, "text/plain", Output);
}

void HandleIndexFile()
{
  File file = SPIFFS.open("/index.html","r");
  server.streamFile(file, "text/html");
  file.close();
  Serial.println("HTML file is Served");
}

// on cmd line 
// curl -F "file=@C:/Users/Pavan Kumar/Documents/Arduino/ESP8266/ESP8266_WebServerUploadFilesToSPIFFS_FlashMemory_ByHTTP/data/index_1.html" 172.20.10.2/upload
