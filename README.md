# Modern Music Player

A modern, native music player built with a focus on performance and clean architecture.

# Screenshots

<img width="1010" height="604" alt="image" src="https://github.com/user-attachments/assets/65351165-142b-4bd2-baad-18204fc4e22d" />
<img width="2560" height="1441" alt="image" src="https://github.com/user-attachments/assets/20594389-4f11-4d61-a148-fac5c5423904" />
<img width="997" height="643" alt="image" src="https://github.com/user-attachments/assets/6f1352a0-10db-4ba4-aaf1-7d4991d8b6df" />

# Requirements

The project targets modern desktop operating systems. It offers full platform feature integration and the best user experience on macOS. It is fully functional and supported on Linux. Windows compatibility is currently untested.

# Tech Stack

The application is written in standard C++23. It utilizes Qt 6.11.3 for its core framework and interface components.

# Dependencies

The build process requires the Qt 6.11.3 framework libraries. Asynchronous operations and coroutine support are handled via the [QCoro]([url](https://github.com/qcoro/qcoro?ysclid=mw2y5a6561640227271)) library. Standard C++23 compliant compiler tools are necessary for compilation.

# Installation

The project uses the standard CMake build system. On macOS, the build artifacts can be packaged into a DMG installer using CPack. Packaging configuration for other operating systems is not implemented.
