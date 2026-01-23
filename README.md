# OpenGL C Programming on macOS (Apple Silicon)

This guide provides a complete setup for running OpenGL applications using the native **GLUT** framework on macOS M1/M2/M3 chips using **VS Code**.

---

## 🛠 1. Prerequisites

Ensure you have the **Xcode Command Line Tools** installed. This provides the `clang` compiler needed to build your C code.

Open your Terminal and run:

```bash
xcode-select --install
```

If not installed, install Xcode Command Line Tools:

```
xcode-select --install
```

## 🔍 2. Verify GLUT Framework

macOS comes with GLUT pre-installed. Verify by running:

```
ls /System/Library/Frameworks/GLUT.framework
```

If files are listed → GLUT is available ✅
No installation needed.

**Optional: If you want FreeGLUT via Homebrew (mostly for Linux portability):**

```
brew install freeglut
```

## 🚀 3. Sample OpenGL Program

Create `main.c` in your project folder:

```
#include<GLUT/glut.h>

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);       // Clear screen
    glColor3f(0.2f, 0.7f, 1.0f);        // Set color (light blue)

    glBegin(GL_POLYGON);                // Start polygon (triangle here)
        glVertex2f(-0.5f, -0.3f);       // Bottom-left
        glVertex2f(0.5f, -0.3f);        // Bottom-right
        glVertex2f(0.0f, 0.6f);         // Top-center
    glEnd();

    glFlush();                          // Execute drawing
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                          // Init GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    // Single buffer, RGB
    glutInitWindowSize(500, 500);                   // Window size
    glutCreateWindow("Triangle");                   // Create window
    glClearColor(0,0,0,0);                          // Background = black
    glutDisplayFunc(display);                       // Register display callback
    glutMainLoop();                                 // Start event loop
    return 0;
}
```

## 🏗 4. Compile and Run (Terminal)

Open Terminal, navigate to your project folder:

```
cd /path/to/your/project
```

**Compile using Apple GLUT:**

```
clang main.c -o main -framework OpenGL -framework GLUT -DGL_SILENCE_DEPRECATION
```

Run the program:

```
./main
```

**Output**

![Image](https://i.ibb.co.com/3m2gMYW7/Screenshot-2026-01-23-at-5-48-51-PM.png)

## ⚡ 5. VS Code Automation (One-Click Run)

- In your project folder, create a folder `.vscode`
- Inside `.vscode`, create `tasks.json`:

```
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Run Current OpenGL C File",
      "type": "shell",
      "command": "clang",
      "args": [
        "${file}",
        "-o",
        "${fileDirname}/out",
        "-framework",
        "OpenGL",
        "-framework",
        "GLUT",
        "-DGL_SILENCE_DEPRECATION",
        "&&",
        "${fileDirname}/out"
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "problemMatcher": []
    }
  ]
}
```

- Run the task:
  - Press `⌘ + ⇧ + B`
  - Select Run Current OpenGL C File

## 👤 Author

- `Md Rakibul Hasan`
- `rakibulhasanshuvo206@gmail.com`
