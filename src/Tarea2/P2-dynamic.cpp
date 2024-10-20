//
// Created by pablo on 20-10-2024.
//

#include <iostream>
#include <memory>
#include <string>
// Ahora ya no usaremos array, pues existe asignación dinamica usando unorderd_map
#include <array>
#include <fstream>
#include <unordered_map>

// Enum para el estado de deudor (si tiene dicom o no)
enum class DicomStatus {
    NoDeudor,
    Deudor
};

// Estructura del nodo del trie
struct TrieNode {
    // Array de punteros únicos a hijos (dígitos del 0 al 9)
    // Nos aseguramos de inicializar todo a null pointer

    // COMO SE REQUIERE USO DINAMICO DE MEMORA, CAMBIAMOS LA ESTRUCTURA DEL TRIA A UN unordered_map (hashing)
    std::unordered_map<char, std::unique_ptr<TrieNode> > children;

    // Indicador de fin de RUT
    // Por defecto debe ser falso en vez de null
    bool is_end_of_rut = false;

    // Información asociada al RUT
    std::string name;
    std::string address;
    std::string date_of_birth;
    // Por defecto, deudor, si es necesario se puede cambiar a gusto
    DicomStatus debtor_status = DicomStatus::Deudor;

    // Constructor
    TrieNode() = default;
};

// Función para convertir un carácter del rut a su índice correspondiente
int charToIndex(const char c) {
    if (c >= '0' && c <= '9') {
        return c - '0'; // Índices 0-9 para dígitos
    }
    if (c == 'K' || c == 'k') {
        return 10; // Índice 10 para 'K' o 'k'
    }
    return -1; // Carácter inválido
}

// Función para normalizar el RUT (eliminar puntos y guiones, y convertir a mayúsculas)
std::string normalizeRUT(const std::string &input_rut) {
    std::string rut;
    for (const char c: input_rut) {
        if (c == '.' || c == ' ' || c == '-') {
            // Ignora puntos, espacios y guiones (que pueden estar en el rut si se escribe de alguna manera no estandar)
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(c)) || c == 'k' || c == 'K') {
            rut += std::toupper(static_cast<unsigned char>(c)); // Convierte a mayúsculas
        } else {
            // Carácter inválido
            return "";
        }
    }
    return rut;
}

// Función para agregar un RUT con su información asociada
void agregar_RUT(TrieNode *root, const std::string &raw_RUT, const std::string &name,
                 const std::string &address, const std::string &date_of_birth, const DicomStatus status) {
    // Normaliza el RUT y separa el cuerpo del dígito verificador
    std::string RUT = normalizeRUT(raw_RUT);

    if (RUT.empty()) {
        std::cerr << "RUT invalido.\n";
        return;
    }

    TrieNode *node = root;

    // Recorre cada carácter del RUT
    for (char c: RUT) {
        if (node->children.find(c) == node->children.end()) {
            node->children[c] = std::make_unique<TrieNode>();
        }

        node = node->children[c].get();
    }

    // Marca el fin del RUT y almacena la información
    node->is_end_of_rut = true;
    node->name = name;
    node->address = address;
    node->date_of_birth = date_of_birth;
    node->debtor_status = status;
}

// Función para buscar un RUT en el trie
TrieNode *buscar_RUT(TrieNode *root, const std::string &raw_RUT) {
    // Normaliza el RUT
    std::string RUT = normalizeRUT(raw_RUT);

    if (RUT.empty()) {
        return nullptr;
    }

    TrieNode *node = root;

    for (char c: RUT) {
        auto it = node->children.find(c);
        if (it == node->children.end()) {
            return nullptr;
        }

        node = it->second.get();
    }


    // Verifica si es un RUT válido almacenado
    if (node != nullptr && node->is_end_of_rut) {
        return node;
    }

    return nullptr;
}

// Función para marcar un RUT como no deudor
void numero_no_deudor(TrieNode *root, const std::string &raw_RUT) {
    TrieNode *node = buscar_RUT(root, raw_RUT);

    if (node) {
        // Si el RUT existe, se marca como no deudor
        node->debtor_status = DicomStatus::NoDeudor;
        std::cout << "RUT " << raw_RUT << " marcado como NO deudor.\n";
    } else {
        // Si no existe, se agrega y se marca como no deudor (solicitando datos)
        std::string name, address, date_of_birth;
        std::cout << "El RUT no existe. Ingrese los datos para agregarlo.\n";
        std::cout << "Nombre: ";
        std::getline(std::cin, name);
        std::cout << "Dirección: ";
        std::getline(std::cin, address);
        std::cout << "Fecha de nacimiento (DD/MM/AAAA): ";
        std::getline(std::cin, date_of_birth);

        agregar_RUT(root, raw_RUT, name, address, date_of_birth, DicomStatus::NoDeudor);
        std::cout << "RUT " << raw_RUT << " agregado y marcado como NO deudor.\n";
    }
}

// Función para eliminar un RUT del trie
bool borrar_RUT(TrieNode *node, const std::string &RUT, int depth = 0) {
    if (!node) {
        return false;
    }

    if (depth == RUT.size()) {
        if (node->is_end_of_rut) {
            node->is_end_of_rut = false;
            // Si el nodo no tiene hijos, puede ser eliminado
            return node->children.empty();
        }
        return false;
    }

    char c = RUT[depth];
    auto it = node->children.find(c);
    if (it == node->children.end()) {
        return false;
    }

    bool shouldDeleteChild = borrar_RUT(it->second.get(), RUT, depth + 1);

    if (shouldDeleteChild) {
        // Eliminar el hijo
        node->children.erase(it);

        // Verificar si el nodo actual puede ser eliminado
        return !node->is_end_of_rut && node->children.empty();
    }

    return false;
}

// Función para guardar la información del trie en un archivo
void guardarTrieEnArchivo(const TrieNode *node, std::ofstream &file, const std::string &RUT_actual = "") {
    if (!node) {
        return;
    }

    if (node->is_end_of_rut) {
        file << "RUT: " << RUT_actual << "\n";
        file << "Nombre: " << node->name << "\n";
        file << "Direccion: " << node->address << "\n";
        file << "Fecha de Nacimiento: " << node->date_of_birth << "\n";
        // Ocupo aca un ternario para al tiro asignar lo que corresponde en el archivo como texto (traducir el valor del enum).
        file << "Estado: " << (node->debtor_status == DicomStatus::Deudor ? "Deudor" : "No Deudor") << "\n";
        file << "---------------------------------------\n";
    }

    // Recorre los hijos recursivamente
    // volvemos a usar la notacion moderna de c++ para ser mas concisos.
    for (const auto &[key, child]: node->children) {
        guardarTrieEnArchivo(child.get(), file, RUT_actual + key);
    }
}

// Programa principal
int main() {
    // Nodo raíz del trie
    auto root = std::make_unique<TrieNode>();

    // Menú de opciones
    int numero_opcion;
    do {
        std::cout << "\n--- Sistema de Gestion de RUTs ---\n";
        std::cout << "1. Agregar RUT\n";
        std::cout << "2. Marcar RUT como NO deudor\n";
        std::cout << "3. Buscar RUT\n";
        std::cout << "4. Borrar RUT\n";
        std::cout << "5. Guardar informacion en archivo\n";
        std::cout << "6. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> numero_opcion;
        std::cin.ignore(); // Limpiar el buffer

        if (numero_opcion == 1) {
            std::string RUT, name, address, date_of_birth;
            std::cout << "Ingrese el RUT (formato 12345678-9): ";
            std::getline(std::cin, RUT);
            std::cout << "Nombre: ";
            std::getline(std::cin, name);
            std::cout << "Direccion: ";
            std::getline(std::cin, address);
            std::cout << "Fecha de nacimiento (DD/MM/AAAA): ";
            std::getline(std::cin, date_of_birth);
            // como dijimos arriba, por defecto sera deudor
            agregar_RUT(root.get(), RUT, name, address, date_of_birth, DicomStatus::Deudor);
            std::cout << "RUT " << RUT << " agregado como deudor.\n";
        } else if (numero_opcion == 2) {
            std::string RUT;
            std::cout << "Ingrese el RUT a marcar como NO deudor (formato 12345678-9): ";
            std::getline(std::cin, RUT);
            numero_no_deudor(root.get(), RUT);
        } else if (numero_opcion == 3) {
            std::string RUT;
            std::cout << "Ingrese el RUT a buscar (formato 12345678-9): ";
            std::getline(std::cin, RUT);
            TrieNode *result = buscar_RUT(root.get(), RUT);
            if (result) {
                std::cout << "RUT encontrado.\n";
                std::cout << "Nombre: " << result->name << "\n";
                std::cout << "Direccion: " << result->address << "\n";
                std::cout << "Fecha de nacimiento: " << result->date_of_birth << "\n";
                std::cout << "Estado: " << (result->debtor_status == DicomStatus::Deudor ? "Deudor" : "No Deudor") <<
                        "\n";
            } else {
                std::cout << "RUT no encontrado.\n";
            }
        } else if (numero_opcion == 4) {
            std::string RUT;
            std::cout << "Ingrese el RUT a borrar (formato 12345678-9): ";
            std::getline(std::cin, RUT);
            std::string normalized_RUT = normalizeRUT(RUT);
            if (borrar_RUT(root.get(), normalized_RUT)) {
                std::cout << "RUT " << RUT << " borrado del sistema.\n";
            } else {
                std::cout << "RUT no encontrado o no se pudo borrar.\n";
            }
        } else if (numero_opcion == 5) {
            std::ofstream archivo("rut_data.txt");
            if (archivo.is_open()) {
                guardarTrieEnArchivo(root.get(), archivo);
                archivo.close();
                std::cout << "Informacion guardada en 'rut_data.txt' en el directorio acvtual.\n";
            } else {
                std::cerr << "No se pudo abrir el archivo para escribir.\n";
            }
        } else if (numero_opcion == 6) {
            std::cout << "Saliendo del programa.\n";
        } else {
            std::cout << "Opción inva1lida. Intente nuevamente.\n";
        }
    } while (numero_opcion != 6);

    return 0;
}
