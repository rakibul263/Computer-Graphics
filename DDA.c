#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Function to find maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

int main() {
    int x1, y1, x2, y2;

    printf("Enter starting point (x1 y1): ");
    scanf("%d %d", &x1, &y1);
    printf("Enter ending point (x2 y2): ");
    scanf("%d %d", &x2, &y2);

    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = max(abs(dx), abs(dy));
    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = x1;
    float y = y1;

    printf("\nPoints on the line:\n");
    for (int i = 0; i <= steps; i++) {
        printf("(%d, %d)\n", (int)round(x), (int)round(y));
        x += xInc;
        y += yInc;
    }

    return 0;
}
