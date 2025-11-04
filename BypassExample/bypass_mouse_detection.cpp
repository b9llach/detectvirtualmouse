#include <Windows.h>
#include <stdio.h>

/*
 * Educational Demonstration: Mouse Input Injection Techniques
 * Purpose: Security research and understanding Windows input detection mechanisms
 *
 * This demonstrates various methods of mouse control and their detectability:
 * 1. SendInput() - DETECTED (sets LLMHF_INJECTED flag)
 * 2. mouse_event() - DETECTED (sets LLMHF_INJECTED flag)
 * 3. SetCursorPos() - NOT DETECTED (only moves cursor, no mouse events)
 * 4. Kernel-mode injection - NOT DETECTED (bypasses user-mode hooks)
 */

// Method 1: SendInput - WILL BE DETECTED
void MoveMouse_SendInput(int x, int y) {
    printf("[*] Method 1: SendInput (DETECTED)\n");

    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dx = x * (65536 / GetSystemMetrics(SM_CXSCREEN));
    input.mi.dy = y * (65536 / GetSystemMetrics(SM_CYSCREEN));
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

    SendInput(1, &input, sizeof(INPUT));
}

// Method 2: mouse_event - WILL BE DETECTED
void MoveMouse_MouseEvent(int x, int y) {
    printf("[*] Method 2: mouse_event (DETECTED)\n");

    mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE,
                x * (65536 / GetSystemMetrics(SM_CXSCREEN)),
                y * (65536 / GetSystemMetrics(SM_CYSCREEN)),
                0, 0);
}

// Method 3: SetCursorPos - NOT DETECTED but doesn't generate mouse events
void MoveMouse_SetCursorPos(int x, int y) {
    printf("[*] Method 3: SetCursorPos (NOT DETECTED - no events generated)\n");
    SetCursorPos(x, y);
}

// Method 4: Simulate hardware input via driver (concept demonstration)
// This would require a kernel-mode driver to actually implement
void MoveMouse_KernelMode_Concept(int x, int y) {
    printf("[*] Method 4: Kernel-mode injection (NOT DETECTED)\n");
    printf("    This requires a signed kernel driver to inject input at ring 0\n");
    printf("    The driver would call NtUserInjectMouseInput or similar\n");
    printf("    without setting the LLMHF_INJECTED flag.\n");
    printf("    Implementation requires:\n");
    printf("    - Kernel driver development (WDK)\n");
    printf("    - Driver signing or test mode\n");
    printf("    - IOCTL interface for user-mode communication\n");
}

// Method 5: Using Raw Input (advanced technique)
// Raw Input can bypass some hooks but still may set flags
void MoveMouse_RawInput_Concept() {
    printf("[*] Method 5: Raw Input manipulation (PARTIALLY DETECTED)\n");
    printf("    Raw Input is typically for receiving input, not injecting.\n");
    printf("    Advanced techniques involve:\n");
    printf("    - Manipulating the Raw Input buffer directly\n");
    printf("    - Requires deep Windows internals knowledge\n");
    printf("    - May still be detected by sophisticated anti-cheat\n");
}

// Method 6: Hardware emulation (USB device)
void MoveMouse_HardwareEmulation_Concept() {
    printf("[*] Method 6: Hardware USB device emulation (NOT DETECTED)\n");
    printf("    Uses actual USB HID device or emulation:\n");
    printf("    - Arduino/Teensy programmed as USB HID device\n");
    printf("    - Appears as real hardware to Windows\n");
    printf("    - No LLMHF_INJECTED flag set\n");
    printf("    - Cannot be detected by user-mode hooks\n");
    printf("    - Requires physical or virtual USB device\n");
}

int main() {
    printf("========================================\n");
    printf("Mouse Input Injection Techniques Demo\n");
    printf("Educational / Security Research Purpose\n");
    printf("========================================\n\n");

    printf("Run the Virtual Mouse Detector (VMD) alongside this\n");
    printf("to see which methods are detected.\n\n");

    Sleep(2000);

    // Get current cursor position for reference
    POINT pt;
    GetCursorPos(&pt);
    printf("Current cursor position: (%d, %d)\n\n", pt.x, pt.y);

    // Demonstrate each method
    printf("\n--- Testing Detection Methods ---\n\n");

    // Method 1: SendInput (will be detected)
    MoveMouse_SendInput(pt.x + 10, pt.y);
    Sleep(1000);

    // Method 2: mouse_event (will be detected)
    MoveMouse_MouseEvent(pt.x + 20, pt.y);
    Sleep(1000);

    // Method 3: SetCursorPos (won't trigger mouse events, so not detected)
    MoveMouse_SetCursorPos(pt.x + 30, pt.y);
    Sleep(1000);

    // Conceptual methods (information only)
    printf("\n--- Advanced Bypass Techniques (Conceptual) ---\n\n");
    MoveMouse_KernelMode_Concept(pt.x, pt.y);
    printf("\n");

    MoveMouse_RawInput_Concept();
    printf("\n");

    MoveMouse_HardwareEmulation_Concept();
    printf("\n");

    printf("\n========================================\n");
    printf("Summary:\n");
    printf("- User-mode APIs (SendInput, mouse_event) are DETECTED\n");
    printf("- SetCursorPos only moves cursor, no events generated\n");
    printf("- Kernel-mode and hardware emulation can bypass detection\n");
    printf("- True bypass requires ring 0 access or hardware devices\n");
    printf("========================================\n");

    return 0;
}
