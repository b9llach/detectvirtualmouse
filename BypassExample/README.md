# Mouse Detection Bypass Examples

**Purpose:** Educational and security research demonstrations for understanding Windows input detection mechanisms.

## Overview

This directory contains examples demonstrating various mouse input injection techniques and their detectability by user-mode hooks like the Virtual Mouse Detector (VMD).

## Files

1. **bypass_mouse_detection.cpp** - Demonstrates multiple input methods and their detection status
2. **smooth_mouse_mover.cpp** - Creates natural-looking mouse movements (still detected)

## Detection Mechanisms

The VMD detects synthetic mouse input by checking the `LLMHF_INJECTED` (0x01) flag in the `MSLLHOOKSTRUCT.flags` field. This flag is automatically set by Windows when using:

- `SendInput()`
- `mouse_event()`
- Other user-mode input injection APIs

## Bypass Techniques (Educational Overview)

### ✓ Detected by VMD

1. **SendInput()** - Standard Windows API for input injection
2. **mouse_event()** - Legacy API for mouse events
3. **keybd_event()** - Keyboard equivalent (also sets injection flag)

### ✗ NOT Detected by VMD (User-mode hooks only)

#### 1. SetCursorPos()
- Only moves the cursor position
- Does NOT generate mouse events (no WM_MOUSEMOVE messages)
- Limitation: Cannot simulate clicks or other mouse events

#### 2. Kernel-Mode Driver Injection
The most effective bypass requires kernel-mode (ring 0) access:

```c
// Conceptual approach (requires kernel driver):
// 1. Create a kernel driver using Windows Driver Kit (WDK)
// 2. Use undocumented functions like:
//    - NtUserInjectMouseInput()
//    - InjectMouseInput() from win32k.sys
// 3. Inject input directly without setting LLMHF_INJECTED flag
```

**Requirements:**
- Windows Driver Kit (WDK)
- Driver signing certificate OR test signing mode
- Knowledge of Windows kernel programming
- IOCTL interface for user-mode communication

**Detection:**
- Bypasses user-mode hooks completely
- Can be detected by kernel-mode anti-cheat
- Requires ring 0 detection mechanisms

#### 3. Hardware Emulation
Use actual or emulated USB HID devices:

**Physical Approach:**
- Arduino Leonardo / Pro Micro
- Teensy boards
- Raspberry Pi Pico

**Virtual Approach:**
- USB/IP protocol
- Virtual USB device drivers
- QEMU USB passthrough

**Code Example (Arduino):**
```cpp
#include <Mouse.h>

void setup() {
  Mouse.begin();
}

void loop() {
  Mouse.move(1, 1); // Appears as real hardware
  delay(10);
}
```

**Detection:**
- Appears as legitimate hardware to Windows
- No LLMHF_INJECTED flag
- Cannot be detected by user-mode software
- Requires behavioral analysis or device fingerprinting

#### 4. DirectInput Manipulation
- Manipulate DirectInput buffers directly
- Requires advanced Windows internals knowledge
- Limited effectiveness, still may be detected

#### 5. Remote Desktop / Virtual Machine Injection
- Inject input at the hypervisor level
- Pass through input from host to VM
- Bypasses guest OS detection

## Building the Examples

### Using Visual Studio:
```batch
cl.exe bypass_mouse_detection.cpp /Fe:bypass_test.exe user32.lib
cl.exe smooth_mouse_mover.cpp /Fe:smooth_mover.exe user32.lib
```

### Using MinGW:
```bash
g++ bypass_mouse_detection.cpp -o bypass_test.exe -luser32
g++ smooth_mouse_mover.cpp -o smooth_mover.exe -luser32
```

## Testing Procedure

1. **Start the Virtual Mouse Detector:**
   ```
   DetectMouseEvents.exe
   ```

2. **Run the bypass examples:**
   ```
   bypass_test.exe
   smooth_mover.exe
   ```

3. **Observe the VMD console:**
   - Detected events will show `[+] virtual mouse detected`
   - Note which methods trigger detection

## Kernel-Mode Driver Resources

For those interested in kernel-mode bypass research:

### Learning Resources:
- Windows Driver Kit (WDK) Documentation
- "Windows Kernel Programming" by Pavel Yosifovich
- OSR Online (driver development community)

### Required Tools:
- Visual Studio 2019/2022
- Windows Driver Kit (WDK)
- Windows SDK
- Test signing mode or valid driver certificate

### Basic Driver Structure:
```c
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    // Initialize driver
    DriverObject->DriverUnload = UnloadDriver;

    // Create device for IOCTL communication
    IoCreateDevice(...);

    return STATUS_SUCCESS;
}

// IOCTL handler for receiving input commands from user-mode
NTSTATUS DeviceIoControlHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    // Parse input requests
    // Call kernel input injection functions
    // Return results to user-mode
}
```

## Anti-Detection Techniques

To make synthetic input less detectable:

1. **Timing Randomization** - Add random delays between inputs
2. **Human-like Curves** - Use Bezier curves for mouse paths
3. **Velocity Variation** - Vary movement speed naturally
4. **Micro-movements** - Add small random movements
5. **Acceleration Profiles** - Simulate mouse acceleration

## Defensive Countermeasures

If you're building anti-cheat or bot detection:

1. **Kernel-mode hooks** - Hook at ring 0 to detect driver injection
2. **Behavioral analysis** - Detect inhuman patterns (perfect timing, precision)
3. **Device fingerprinting** - Validate connected HID devices
4. **Input timing analysis** - Measure input timing consistency
5. **Machine learning** - Train models on human vs. bot behavior

## Ethical Considerations

These techniques should only be used for:
- Security research and testing
- Authorized penetration testing
- Educational purposes
- Defensive security development
- CTF competitions

**Do NOT use for:**
- Cheating in online games
- Automated fraud or abuse
- Bypassing security on systems you don't own
- Any unauthorized or malicious activities

## Legal Notice

This code is provided for educational and security research purposes only. Users are responsible for ensuring their use complies with applicable laws and terms of service. Unauthorized use of these techniques may violate computer fraud laws, terms of service agreements, and anti-cheat policies.

## References

- [MSDN: SetWindowsHookEx](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowshookexw)
- [MSDN: MSLLHOOKSTRUCT](https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-msllhookstruct)
- [MSDN: SendInput](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput)
- [Windows Driver Kit Documentation](https://docs.microsoft.com/en-us/windows-hardware/drivers/)
