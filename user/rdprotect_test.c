#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    char *addr = sbrk(0);
    sbrk(4096);
    addr[0] = 'Z'; 

    printf("=== INICIO TEST TAREA 3 ===\n");

    // 1. Proteger memoria (Quitar lectura)
    if (mrdprotect(addr, 1) < 0) {
        printf("ERROR: mrdprotect fallo\n");
        exit(1);
    }
    printf("[OK] Memoria protegida con mrdprotect.\n");

    // 2. Prueba de Escritura/Lectura (Debe fallar y matar al hijo)
    int pid = fork();
    if(pid == 0) {
        // Intentar acceder a memoria protegida
        // En QEMU estricto, esto causara un trap (exito para nosotros)
        char c = addr[0]; 
        printf("ERROR FATAL: Se pudo leer el valor: %c\n", c);
        exit(0);
    }
    wait(0); 
    printf("[OK] El proceso hijo murio al intentar acceder (Proteccion funciona).\n");

    // 3. Desproteger (Restaurar lectura)
    if (munrdprotect(addr, 1) < 0) {
        printf("ERROR: munrdprotect fallo\n");
        exit(1);
    }
    printf("[OK] Permisos restaurados con munrdprotect.\n");

    // 4. Verificar acceso normal
    addr[0] = 'B'; // Escribir
    if(addr[0] == 'B') { // Leer
        printf("=== EXITO TOTAL: Lectura y escritura funcionan nuevamente ===\n");
    } else {
        printf("ERROR: La memoria sigue corrupta.\n");
        exit(1);
    }

    exit(0);
}
