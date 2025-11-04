# 👻 Juego "Fantasmas" (Versión Pac-Man en C)

Proyecto desarrollado en lenguaje **C** usando el entorno **Code::Blocks**.

---

## 🧩 Descripción

Este proyecto es una versión tipo **Pac-Man**, donde el jugador controla al personaje principal, recolecta monedas y evita a los fantasmas.  
El objetivo es conseguir la mayor puntuación posible antes de perder las tres vidas disponibles o recolectar todas las monedas para ganar.

---

## 🕹️ Controles

| Tecla | Acción |
|-------|---------|
| **W / A / S / D** | Movimiento (arriba, izquierda, abajo, derecha) |
| **F** | Detiene el programa |
| **Enter** | Selecciona opciones en el menú |

---

## 💾 Requisitos

- **Code::Blocks** (o cualquier compilador compatible con C, preferiblemente en **Windows**).  
- Librerías estándar:  
  `stdio.h`, `stdlib.h`, `windows.h`, `string.h`, `stdbool.h`.

---

## ⚙️ Compilación y ejecución

1. Abre Code::Blocks.  
2. Crea un nuevo proyecto tipo *Console Application (C)*.  
3. Copia el código del archivo `1 Fantasmas.c` dentro del proyecto.  
4. Compila (`Ctrl + F9`) y ejecuta (`Ctrl + F10`).

---

## 💀 Vidas del jugador

El juego comienza con **3 vidas** (`int vida = 3;`).  
Cada vez que el fantasma toca al jugador, se resta una vida.  
Cuando las vidas llegan a **0**, se muestra el mensaje **“PERDISTE!!!”** y el juego termina.

---

## 🧠 Versiones disponibles

Este repositorio contiene **dos versiones del juego:**

| Versión | Descripción |
|----------|-------------|
| **1 Fantasma** | Versión inicial con un solo enemigo (más sencilla). |
| **4 Fantasmas** | Versión avanzada con cuatro enemigos activos simultáneamente, mayor dificultad. |

Ambas versiones están incluidas como archivos `.c` independientes dentro del repositorio.

---

## 🧑‍💻 Autores

**Realizado por:**
- Eddy Zamora  
- María Belén García  
- Tiffany Jordán  

---

## 📝 Notas

- La tecla **F** detiene el programa en cualquier momento.  
- Los ganadores se guardan en el archivo `archivo.dat`.  
- Cada ganador se registra con su **nombre y puntaje final**.  
- Proyecto creado con fines académicos y recreativos.

---

## 📂 Repositorio

🔗 [Ver en GitHub](https://github.com/otakuhxh/Pacman)

---

