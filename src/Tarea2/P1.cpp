#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

// Asumiremos un largo maximo de palabra traducida como 100 caracteres,  para alocar memoria
#define NUM_CHILDREN 26 // Capearemos los hijos al numero de letras del abecedario (sin tildes ni caracteres especiales)
#define MAX_TRANSLATION_LENGTH 100
#define MAX_LINE_LENGTH 256

/* Estructura de un nodo del trie */
struct trie_node {
    char traducciones[MAX_TRANSLATION_LENGTH]; // Traducciones almacenadas como una cadena separada por comas
    struct trie_node *hijos[NUM_CHILDREN]; // Arreglo de punteros a hijos
};

// Nota: aca usaremos la notación moderna (C++ 23) para los NULL (nullptr)
struct trie_node *root = nullptr; // Nodo raíz del trie

/* Función de comparación
 * (se comporta como un wrapper de quicksort, pero especifico para la estructura que
 * estamos utilizando)
 *
 *
 */
int cmp_func(const void *a, const void *b) {
    const char *sa = *(const char **) a;
    const char *sb = *(const char **) b;
    return strcmp(sa, sb);
}

/* Reserva memoria para un nuevo nodo */
struct trie_node *nuevo_nodo(void) {
    struct trie_node *node = new trie_node;
    node->traducciones[0] = '\0'; // Inicializa traducciones como un string vacio
    // Aca usaremos los loops for con sintaxis de c++ moderna (usando el operador -> y la deduccion de tipo automatica)
    for (auto &hijo: node->hijos)
        hijo = nullptr;
    return node;
}

/* Elimina un nodo y todos sus hijos */
void borrar_nodo(struct trie_node *pnode) {
    if (pnode == nullptr)
        return;
    for (const auto &hijo: pnode->hijos) {
        if (hijo != nullptr)
            borrar_nodo(hijo);
    }
    delete pnode;
}

/* Divide un string por comas y agrega las palabras al arreglo */
void split_and_add(char *str, char **words, int &word_count) {
    char *token = strtok(str, ",");
    while (token != nullptr && word_count < 50) {
        // Elimina espacios al inicio y al final
        while (*token == ' ')
            ++token;
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ') {
            *end = '\0';
            --end;
        }
        // Verifica duplicados
        bool duplicate = false;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            words[word_count] = new char[strlen(token) + 1];
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(nullptr, ",");
    }
}

/* Fusiona y ordena las traducciones existentes y nuevas */
void merge_and_sort_translations(const char *existing, const char *new_translations, char *result) {
    const int MAX_WORDS = 50;
    char *words[MAX_WORDS];
    int word_count = 0;

    // Divide las traducciones existentes
    char temp_existing[MAX_TRANSLATION_LENGTH];
    strncpy(temp_existing, existing, MAX_TRANSLATION_LENGTH - 1);
    temp_existing[MAX_TRANSLATION_LENGTH - 1] = '\0';
    split_and_add(temp_existing, words, word_count);

    // Divide las nuevas traducciones
    char temp_new[MAX_TRANSLATION_LENGTH];
    strncpy(temp_new, new_translations, MAX_TRANSLATION_LENGTH - 1);
    temp_new[MAX_TRANSLATION_LENGTH - 1] = '\0';
    split_and_add(temp_new, words, word_count);

    // Ordena las palabras usando quicksort
    qsort(words, word_count, sizeof(char *), cmp_func);

    // Une las palabras en una cadena separada por comas
    result[0] = '\0';
    for (int i = 0; i < word_count; ++i) {
        if (i > 0)
            strcat(result, ",");
        strcat(result, words[i]);
        delete[] words[i]; // Libera memoria
    }
}

/* Agrega una palabra al trie con su traducción */
int agregar_palabra(const char *palabra, const char *traduccion) {
    struct trie_node *node = root;
    int index;
    for (int i = 0; palabra[i] != '\0'; ++i) {
        char c = palabra[i];
        if (c >= 'a' && c <= 'z')
            index = c - 'a';
        else if (c >= 'A' && c <= 'Z')
            index = c - 'A';
        else
            return 0; // Caracter inválido (no está en ascci entre A-Z)

        if (node->hijos[index] == nullptr)
            // NOTA: Tecnicamente acá podria existir la posibilidad de generarse un "memory leak"
            // debido a la llamada recursiva. El tema es que como al principio limitamos el largo de palabra
            // usando MAX_LINE_LENGTH, habrá una constante que indicara empiricamente cuan lejos puede llegar la recusrión
            node->hijos[index] = nuevo_nodo();
        node = node->hijos[index];
    }

    // Fusiona las traducciones existentes con las nuevas
    char merged_translations[MAX_TRANSLATION_LENGTH * 2];
    merge_and_sort_translations(node->traducciones, traduccion, merged_translations);

    // Almacena las traducciones fusionadas
    strncpy(node->traducciones, merged_translations, MAX_TRANSLATION_LENGTH - 1);
    node->traducciones[MAX_TRANSLATION_LENGTH - 1] = '\0';

    return 1;
}

/* Busca una palabra en el trie y retorna sus traducciones */
char *buscar_palabra(const char *palabra) {
    struct trie_node *node = root;
    int index;
    for (int i = 0; palabra[i] != '\0'; ++i) {
        const char c = palabra[i];
        if (c >= 'a' && c <= 'z')
            index = c - 'a';
        else if (c >= 'A' && c <= 'Z')
            index = c - 'A';
        else
            return nullptr; // Caracter inválido

        if (node->hijos[index] == nullptr)
            return nullptr; // Palabra no encontrada
        node = node->hijos[index];
    }

    if (node->traducciones[0] != '\0')
        return node->traducciones;
    else
        return nullptr;
}

/* Programa principal */
int main() {
    root = nuevo_nodo();

    char line[MAX_LINE_LENGTH];

    std::cout <<
            "Ingrese palabras y traducciones (palabra traduccion), o solo una palabra para buscar:\n";
    while (fgets(line, sizeof(line), stdin)) {
        // Elimina el carácter de nueva líneapara evitar
        // problemas con saltos dentro del diccionario
        const size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        // Salta líneas vacías
        if (strlen(line) == 0)
            continue;

        // Divide la línea en palabra y traducción
        const char *palabra = strtok(line, " ");
        char *traduccion = strtok(nullptr, "");


        if (traduccion != nullptr) {
            // Elimina espacios adicionales en traducción
            while (*traduccion == ' ')
                ++traduccion;
            // Agrega la palabra y su traducción
            agregar_palabra(palabra, traduccion);
            std::cout << "Palabra '" << palabra << "' agregada con traduccion '" << traduccion << "'.\n";
        } else {
            // Busca la palabra
            const char *result = buscar_palabra(palabra);
            if (result != nullptr)
                std::cout << palabra << " -> " << result << std::endl;
            else
                std::cout << "La palabra '" << palabra << "' no se encuentra en el diccionario.\n";
        }
    }

    // Libera memoria del trie
    borrar_nodo(root);
    root = nullptr;

    return 0;
}


