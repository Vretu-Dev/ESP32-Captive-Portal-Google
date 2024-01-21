# ESP32 Captive Portal Google

[![Hippocratic License HL3-CL-EXTR-FFD-MEDIA-MIL-MY-SV-TAL](https://img.shields.io/static/v1?label=Hippocratic%20License&message=HL3-CL-EXTR-FFD-MEDIA-MIL-MY-SV-TAL&labelColor=5e2751&color=bc8c3d)](https://firstdonoharm.dev/version/3/0/cl-extr-ffd-media-mil-my-sv-tal.html)

A ESP32 Captive Portal written in Arduino C++ (think airport Wi-Fi sign-in page) demo that works on all devices[^1].

This code is an example of a captive portal for the ESP32, using the AsyncTCP library. The code sets up a soft access point with the given SSID and no password and configures a DNS server to redirect all traffic to the captive portal web server running on the ESP32. The captive portal serves an HTML page with a message that all requests will be redirected to this page.

The code also includes some workarounds and documents limitations for different devices and browsers, such as a workaround for Windows 11 captive portal and limitations on the Safari (iOS) browser.

## 🏁 Get Started

Download the firmware from the releases page and select the serial port from the dropdown menu. Press the "Flash ESP" button to flash the ESP32.

If you are running Windows, you will most likely need a driver from [here](https://www.wemos.cc/en/latest/ch340_driver.html) before your computer will show the ESP32 com port in the drop-down menu in ESPhome Flasher.

## ⚙️ Features

```
RAM:   [=         ]  13.8% (used 45188 bytes from 327680 bytes)
Flash: [===       ]  27.2% (used 856037 bytes from 3145728 bytes)
```

- About 0.1s reload time using modern browsers and systems
- Lots of comments that explain how it all works

## 🖼️ User Interface

**Login Page**

![Login](/images/login.png)

**Password Page**

![User interface](/images/password.png)

**Settings Page (4.3.2.1/manage) Login: admin Password: securepassword**

![Backend](/images/backend.png)

## 📝 All original repo documentation

[Repository CDFER/Captive-Portal-ESP32](https://github.com/CDFER/Captive-Portal-ESP32)
