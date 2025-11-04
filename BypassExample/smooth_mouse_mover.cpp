#include <Windows.h>
<parameter name="stdio.h">
#include <math.h>

/*
 * Smooth Mouse Movement Example
 * This WILL be detected by VMD (uses SendInput)
 * Purpose: Educational demonstration for testing detection mechanisms
 */

void SmoothMoveMouse(int startX, int startY, int endX, int endY, int durationMs) {
    int steps = durationMs / 10; // 10ms per step

    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;

        // Ease in-out curve for more natural movement
        float easedT = t < 0.5f
            ? 2 * t * t
            : -1 + (4 - 2 * t) * t;

        int currentX = (int)(startX + (endX - startX) * easedT);
        int currentY = (int)(startY + (endY - startY) * easedT);

        // Use SetCursorPos for immediate movement without triggering events
        // Or use SendInput to trigger events (and detection)

        INPUT input = { 0 };
        input.type = INPUT_MOUSE;
        input.mi.dx = currentX * (65536 / GetSystemMetrics(SM_CXSCREEN));
        input.mi.dy = currentY * (65536 / GetSystemMetrics(SM_CYSCREEN));
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

        SendInput(1, &input, sizeof(INPUT));
        Sleep(10);
    }
}

void DrawCircleWithMouse(int centerX, int centerY, int radius, int durationMs) {
    const int steps = 36; // 10 degree increments
    int timePerStep = durationMs / steps;

    for (int i = 0; i <= steps; i++) {
        float angle = (float)i * (2 * 3.14159f / steps);
        int x = centerX + (int)(radius * cos(angle));
        int y = centerY + (int)(radius * sin(angle));

        INPUT input = { 0 };
        input.type = INPUT_MOUSE;
        input.mi.dx = x * (65536 / GetSystemMetrics(SM_CXSCREEN));
        input.mi.dy = y * (65536 / GetSystemMetrics(SM_CYSCREEN));
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

        SendInput(1, &input, sizeof(INPUT));
        Sleep(timePerStep);
    }
}

int main() {
    printf("Smooth Mouse Movement Tester\n");
    printf("This demonstrates various movement patterns\n");
    printf("All movements WILL BE DETECTED by VMD\n\n");

    Sleep(2000);

    POINT pt;
    GetCursorPos(&pt);

    printf("Starting position: (%d, %d)\n", pt.x, pt.y);
    printf("Moving in 3 seconds...\n");
    Sleep(3000);

    // Test 1: Smooth diagonal movement
    printf("Test 1: Smooth diagonal movement\n");
    SmoothMoveMouse(pt.x, pt.y, pt.x + 200, pt.y + 200, 2000);
    Sleep(1000);

    // Test 2: Draw a circle
    printf("Test 2: Drawing a circle\n");
    GetCursorPos(&pt);
    DrawCircleWithMouse(pt.x, pt.y, 100, 3000);
    Sleep(1000);

    // Test 3: Rapid movements (will generate many events)
    printf("Test 3: Rapid movements (spam detection)\n");
    GetCursorPos(&pt);
    for (int i = 0; i < 20; i++) {
        SmoothMoveMouse(pt.x, pt.y, pt.x + 50, pt.y, 100);
        SmoothMoveMouse(pt.x + 50, pt.y, pt.x, pt.y, 100);
    }

    printf("\nAll tests complete. Check VMD output for detections.\n");

    return 0;
}
