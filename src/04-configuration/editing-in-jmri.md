# Editing Configuration in JMRI

## Step-by-Step: Edit Node Name in LccPro

One of the most common configuration tasks is renaming a node to something meaningful (e.g., "Front Door Lock" instead of "async_blink"). Here's how it works:

### 1. Launch JMRI and Open the LccPro Tool

Start JMRI (5.12 or later) and navigate to:
```
Tools → LCC → LccPro
```

You should see a list of nodes currently on your LCC network. Your ESP32 node appears here, identified by its ACDI manufacturer and model information.

```
[SCREENSHOT PLACEHOLDER: LccPro main window showing node list with async_blink]
```

### 2. Select Your Node

In the node list, click on the node you want to configure (e.g., "async_blink").

The node details panel shows:
- Node ID
- Static SNIP Data: Manufacturer ("OpenMRN"), Model ("async_blink"), Hardware/Software versions
- SNIP User Data: User name (currently "async_blink"), user description

```
[SCREENSHOT PLACEHOLDER: LccPro node details, ACDI section]
```

### 3. Open the Configuration Editor

Click the **Configure** button (or double-click the node).

LccPro opens a dialog showing the node's CDI. You'll see tabs for different sections:
- **Identification** (read-only): Static SNIP data from firmware
- **User Info** (editable): SNIP user name and description from config file
- **Settings** (if present): Application-specific configuration

```
[SCREENSHOT PLACEHOLDER: LccPro Configure dialog, tabs visible]
```

### 4. Edit the User Name

Select the **User Info** tab.

You'll see fields like:
```
User Name:  ______________________________
User Description: ______________________________
```

Change the User Name from "async_blink" to something meaningful, like "Kitchen Light".

```
[SCREENSHOT PLACEHOLDER: LccPro User Info tab with edited name "Kitchen Light"]
```

### 5. Save the Configuration

Click **Save** (or **OK**).

LccPro sends the new user name to the ESP32 node via OpenLCB Configuration protocol. The node writes it to SPIFFS at offset 0-127.

Behind the scenes:
```
JMRI → LccPro → Configuration write message → ESP32
          → OpenMRN receives message
          → apply_configuration() is called
          → User name is written to SPIFFS offset 0-127
          → Configuration write response sent back
JMRI ← Configuration write ACK ← ESP32
```

### 6. Verify Persistence

Once saved, the new name persists across reboots:

1. **Immediate Check**: In LccPro, select the node again. The "User Info" tab now shows "Kitchen Light".

2. **Power Cycle Check**: Power off the ESP32, wait a few seconds, power it back on. Reconnect JMRI to the node. The name is still "Kitchen Light"—it was saved to SPIFFS.

3. **Serial Monitor Check** (optional): Watch the ESP32 serial console during boot. You'll see the config file being loaded, including the user name at offset 0-127.

```
[SCREENSHOT PLACEHOLDER: Serial monitor showing config initialization]
```

## What Just Happened

When you edited and saved the name, OpenMRN did three things:

1. **Received** the new value via JMRI's Configuration write protocol
2. **Wrote** it to the config file at offset 0-127 (the SNIP user data area)
3. **Confirmed** the write was successful back to JMRI

The next time the node boots, it reads this config file from SPIFFS and exposes the updated user name via SNIP protocol. The firmware itself didn't change—only the data in flash storage.

## What This Enables

This workflow shows the power of configuration:
- **No recompilation** needed to change the node name
- **Persistent** across power cycles
- **Standard protocol** (OpenLCB Configuration) that any LCC tool understands
- **Centralized** in JMRI—all nodes configured from one place

Later sections of this chapter extend this pattern to make event IDs, blink interval, and other settings configurable too. The same workflow will apply.

## Troubleshooting: Name Doesn't Save

If you edit the name in JMRI but it doesn't save:

1. **Check Connection**: Ensure JMRI is still connected to the node (green indicator in LccPro)
2. **Check Permissions**: The node's CDI must declare the User Info segment as writable. In our example, it is.
3. **Check SPIFFS**: The ESP32's SPIFFS must have free space. If SPIFFS is full, writes fail silently. Check serial output for error messages.
4. **Check apply_configuration()**: In main.cpp, ensure apply_configuration() returns `UPDATED` on success. In v0.1, it does (it returns UPDATED without making changes, since there's nothing to apply).

See Chapter 3's code-walkthrough section for details on the apply_configuration() callback.
