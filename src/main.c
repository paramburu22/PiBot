#include "backend/code-generation/generator.h"
#include "backend/support/logger.h"
#include "backend/support/memory-manager.h"
#include "backend/support/shared.h"
#include "frontend/syntactic-analysis/bison-parser.h"
#include <stdio.h>
#include "backend/semantic-analysis/symbol_table.h"


// Estado de la aplicación.
CompilerState state;

// Punto de entrada principal del compilador.
const int main(const int argumentCount, const char ** arguments) {
	// Inicializar estado de la aplicación.
	state.program = NULL;
	state.result = 0;
	state.succeed = false;

	// Mostrar parámetros recibidos por consola.
	for (int i = 0; i < argumentCount; ++i) {
		LogInfo("Argumento %d: '%s'", i, arguments[i]);
	}

	

	// Compilar el programa de entrada.
	LogInfo("Compilando...\n");
	symbolTableInit();
	const int result = yyparse();
	switch (result) {
		case 0:
			// La variable "succeed" es la que setea Bison al identificar el símbolo
			// inicial de la gramática satisfactoriamente.
			if (state.succeed) {
				LogInfo("La compilacion fue exitosa.");
				if(argumentCount < 2) {
					LogInfo("No se recibio el nombre del archivo de entrada.");
					Generator(state.result, NULL);
				}else{
					LogInfo("Se recibio el nombre del archivo de entrada.");
					Generator(state.result, arguments[1]);
				}
				
			}
			else {
				LogError("Se produjo un error en la aplicacion.");
				return -1;
			}
			break;
		case 1:
			LogError("Bison finalizo debido a un error de sintaxis.");
			break;
		case 2:
			LogError("Bison finalizo abruptamente debido a que ya no hay memoria disponible.");
			break;
		default:
			LogError("Error desconocido mientras se ejecutaba el analizador Bison (codigo %d).", result);
	}
	
	LogInfo("Fin.");
	mm_freeAll();
	return result;
}
