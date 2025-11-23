#include "kernel/types.h"
#include "user/user.h"

int main() {
    int pid = getpid();
    int ppid = getppid();
    printf("Soy el proceso %d y mi padre es %d\n", pid, ppid);
    exit(0);
}
