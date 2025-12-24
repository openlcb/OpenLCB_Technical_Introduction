## PlatformIO Installation & Setup

PlatformIO is a professional embedded development platform that provides a unified build system, library management, and debugging tools. It integrates with VS Code to create a powerful development environment.

### Installing VS Code

If you don't already have Visual Studio Code installed:

1. Download VS Code from [code.visualstudio.com](https://code.visualstudio.com/)
2. Run the installer for your operating system (Windows, macOS, or Linux)
3. Follow the installation wizard with default options
4. Launch VS Code after installation completes

### Installing PlatformIO Extension

1. Open VS Code
2. Click the Extensions icon in the left sidebar (or press `Ctrl+Shift+X` / `Cmd+Shift+X`)
3. Search for "PlatformIO IDE"
4. Click **Install** on the "PlatformIO IDE" extension by PlatformIO
5. Wait for the installation to complete (this may take several minutes as it downloads toolchains)
6. Restart VS Code when prompted

After restarting, you should see a new PlatformIO icon (alien head) in the left sidebar.

> **Note**: The ESP32 platform and toolchain will be installed automatically when you create your first project. PlatformIO handles all the toolchain downloads and configuration for you.
