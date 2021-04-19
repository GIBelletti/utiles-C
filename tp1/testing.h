#ifndef TESTING_H
#define TESTING_H

#include <stdbool.h>

// Imprime el mensaje seguido de OK o ERROR y el número de línea. Contabiliza el
#define print_test(mensaje, result) do { \
    real_print_test(mensaje, result, __FILE__, __LINE__, #result); \
  } while (0)

// Función auxiliar para print_test(). No debería ser usada directamente.
void real_print_test(const char* mensaje, bool ok,
		     const char* file, int line, const char* failed_expr);

// Devuelve el número total de errores registrados por print_test().
int failure_count(void);

#endif // TESTING_H
