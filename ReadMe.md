# OTA Library Documentation (ESP32-S3 + SIM7670G)

This README explains every major function inside the OTA update library used for ESP32‑S3 devices with the SIM7670G module. It covers initialization, update checking, downloading, flashing, and error handling.

---

## 📌 Overview

The OTA library allows your ESP32‑S3 to:

- Connect to the internet using the SIM7670G module
- Check for firmware updates on a remote server
- Download the new firmware chunk‑by‑chunk
- Validate & install the update
- Reboot the device

The OTA logic is designed for **slow GSM networks**, ensuring safe and resumable OTA.

---

# 📚 OTA Library Functions

Below is every function typically included in your OTA library.

---

## 🟦 1. `initOTA()`

### Syntax

```
void initOTA();
```

**Purpose:** Initializes the OTA environment.

**What it does:**

- Initializes TinyGSM
- Sets APN
- Creates modem connection
- Prepares HTTP client

**Call this once in `setup()`**.

---

## 🟦 2. `isOTATimeout()`

### Syntax

```
bool isOTATimeout();
```

**Purpose:** Check if OTA timeout has expired.

**Used for:**

- Allowing OTA only once every X minutes/hours/days

**Returns:** `true` or `false`.

---

## 🟦 3. `checkForUpdate()`

### Syntax

```
int checkForUpdate();
```

**Purpose:** Check if a new firmware version is available.

**What it does:**

- Sends GET request to your server
- Compares current version vs server version

**Returns:**

- `0` → No update
- `1` → Update available
- `-1` → Error

---

## 🟦 4. `downloadFirmware()`

### Syntax

```
bool downloadFirmware();
```

**Purpose:** Download OTA binary file from server.

**What it does:**

- Connects to firmware URL
- Reads OTA binary in chunks
- Feeds chunks to `Update.write()`

**Returns:**

- `true` → Download OK
- `false` → Failed

---

## 🟦 5. `startOTAUpdate()`

### Syntax

```
void startOTAUpdate();
```

**Purpose:** Main OTA handler that combines all major steps.

**Flow:**

1. Check for update
2. If available → call `downloadFirmware()`
3. Validate firmware size
4. Apply firmware
5. Reboot

This is usually the function you call in your loop.

---

## 🟦 6. `beginOTAWrite(size)`

### Syntax

```
bool beginOTAWrite(size_t firmwareSize);
```

**Purpose:** Starts the flash writing process.

**What it checks:**

- Flash size
- OTA partition availability

**Returns:** `true` or `false`.

---

## 🟦 7. `writeOTAChunk(buffer, length)`

### Syntax

```
int writeOTAChunk(uint8_t *buffer, size_t length);
```

**Purpose:** Writes each chunk of received OTA data.

**This is necessary for GSM due to slow speed.**

**Returns:** `bytes_written` or `-1` if failed.

---

## 🟦 8. `endOTAUpdate()`

### Syntax

```
bool endOTAUpdate();
```

**Purpose:** Finalizes OTA.

**What it does:**

- Calls `Update.end()`
- Validates checksum
- Confirms partition write

**Returns:**

- `true` → success
- `false` → failure

---

## 🟦 9. `rebootDevice()`

### Syntax

```
void rebootDevice();
```

**Purpose:** Reboots ESP32 safely.

Used after OTA completes.

---

## 🟦 10. `getFirmwareVersion()`

### Syntax

```
String getFirmwareVersion();
```

**Purpose:** Returns current firmware version stored in code.

Used to compare with server version.

---

## 🟦 11. `getServerFirmwareVersion()`

### Syntax

```
String getServerFirmwareVersion();
```

**Purpose:** Reads version file from server.

Example response: `1.0.6`

---

## 🟦 12. `validateFirmwareSize()`

### Syntax

```
bool validateFirmwareSize(size_t expectedSize, size_t receivedSize);
```

**Purpose:** Ensures server firmware size matches content length.

Prevents corrupted firmware flashing.

---

## 🟦 13. `printOTADebug()`

### Syntax

```
void printOTADebug(const char *msg);
```

**Purpose:** Prints all debugging logs to Serial.

Used for troubleshooting update failures.

---

## 🟦 14. `setOTALogging(bool)`

### Syntax

```
void setOTALogging(bool enable);
```

**Purpose:** Enable or disable Serial logs.

Useful for production.

---

## 🟦 15. `setFirmwareURL(url)`

### Syntax

```
void setFirmwareURL(const char *url);
```

**Purpose:** Set/update the OTA binary URL dynamically.

---

## 🟦 16. `setVersionURL(url)`

### Syntax

```
void setVersionURL(const char *url);
```

**Purpose:** Set/update the version file URL dynamically.

---

# 📘 Detailed Function Explanations (With Syntax)

Below is a complete explanation of every function, including syntax, parameters, return values, and usage.

---

## 🔹 1. `initOTA()`

### **Syntax**

```
void initOTA();
```

### **Purpose**

Initializes modem, GSM network, APN, and HTTP client required for OTA.

### **Parameters**

_None_

### **Returns**

_None_

### **Details**

- Powers ON SIM7670G
- Sets APN credentials
- Connects to internet
- Prepares OTA environment

---

## 🔹 2. `isOTATimeout()`

### **Syntax**

```
bool isOTATimeout();
```

### **Purpose**

Checks if enough time has passed to allow another OTA check.

### **Parameters**

_None_

### **Returns**

- `true` → OTA check allowed
- `false` → Not yet

### **Details**

Uses RTC or millis() to ensure device doesn’t check OTA too frequently.

---

## 🔹 3. `checkForUpdate()`

### **Syntax**

```
int checkForUpdate();
```

### **Purpose**

Requests the version file from server and compares with local firmware version.

### **Returns**

- `1` → Update available
- `0` → No update
- `-1` → Server error

### **Details**

Makes HTTP GET request → reads version → compares.

---

## 🔹 4. `downloadFirmware()`

### **Syntax**

```
bool downloadFirmware();
```

### **Purpose**

Downloads the OTA binary (.bin file) chunk-by-chunk.

### **Returns**

- `true` → Successfully downloaded
- `false` → Error occurred

### **Details**

- Opens HTTP stream
- Reads in chunks (slow GSM friendly)
- Writes to flash using `writeOTAChunk`

---

## 🔹 5. `startOTAUpdate()`

### **Syntax**

```
void startOTAUpdate();
```

### **Purpose**

Master OTA handler.

### **Details**

Steps:

1. `checkForUpdate()`
2. If update exists → begin OTA
3. Download
4. Validate
5. Install
6. Reboot

---

## 🔹 6. `beginOTAWrite(size)`

### **Syntax**

```
bool beginOTAWrite(size_t firmwareSize);
```

### **Purpose**

Prepares ESP32 OTA partition for writing.

### **Parameters**

- `firmwareSize` → Expected firmware size from server

### **Returns**

- `true` → Ready to write
- `false` → Partition/init error

---

## 🔹 7. `writeOTAChunk(buffer, length)`

### **Syntax**

```
int writeOTAChunk(uint8_t *buffer, size_t length);
```

### **Purpose**

Writes each chunk of firmware to OTA partition.

### **Parameters**

- `buffer` → Pointer to chunk data
- `length` → Size of chunk

### **Returns**

- Bytes written → success
- `-1` → write failed

---

## 🔹 8. `endOTAUpdate()`

### **Syntax**

```
bool endOTAUpdate();
```

### **Purpose**

Ends OTA writing and validates the firmware.

### **Returns**

- `true` → Firmware OK
- `false` → Corrupt or incomplete firmware

---

## 🔹 9. `rebootDevice()`

### **Syntax**

```
void rebootDevice();
```

### **Purpose**

Reboots the ESP32 safely after successful OTA.

### **Returns**

_None_

---

## 🔹 10. `getFirmwareVersion()`

### **Syntax**

```
String getFirmwareVersion();
```

### **Purpose**

Returns firmware version of the device.

---

## 🔹 11. `getServerFirmwareVersion()`

### **Syntax**

```
String getServerFirmwareVersion();
```

### **Purpose**

Fetches version from the server’s version.txt file.

---

## 🔹 12. `validateFirmwareSize()`

### **Syntax**

```
bool validateFirmwareSize(size_t expectedSize, size_t receivedSize);
```

### **Purpose**

Verifies if downloaded firmware size matches server-reported size.

---

## 🔹 13. `printOTADebug(msg)`

### **Syntax**

```
void printOTADebug(const char *msg);
```

### **Purpose**

Prints debug logs to Serial.

---

## 🔹 14. `setOTALogging(enable)`

### **Syntax**

```
void setOTALogging(bool enable);
```

### **Purpose**

Enables or disables verbose logging.

---

## 🔹 15. `setFirmwareURL(url)`

### **Syntax**

```
void setFirmwareURL(const char *url);
```

### **Purpose**

Dynamic configuration of OTA binary URL.

---

## 🔹 16. `setVersionURL(url)`

### **Syntax**

```
void setVersionURL(const char *url);
```

### **Purpose**

Dynamic configuration of version file URL.

---

# 📌 Typical Loop Flow

```
if (isOTATimeout()) {
    startOTAUpdate();
}
```

# 📌 Typical Setup

```
initOTA();
```


