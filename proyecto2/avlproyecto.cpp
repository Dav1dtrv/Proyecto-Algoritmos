
#include <iostream>
#include <string>
#include <climits>
#include <cstring>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <cstdlib>
#include <filesystem> // Para verificar existencia (C++17)


using namespace std;



// Clase Url

class Url {
    
    string url;
    int visitas;
    vector<string> fechas;

public:
    Url(string u) : url(u), visitas(1) {
        fechas.push_back(fechaActual());
    }

    Url() : url(""), visitas(0) {}

    Url(const char* u) : url(u), visitas(1) {
        fechas.push_back(fechaActual());
    }

    void visitar() {
        visitas++;
        fechas.push_back(fechaActual());
    }

    int getVisitas() const { return visitas; }
    string getUrl() const { return url; }

    string to_string() const {

        string s = url + " (" + std::to_string(visitas) + " visitas)";

        for (const auto& f : fechas) {
            s += " [" + f + "]";
        }
        return s;
    }

    bool operator<(const Url& other) const { return url < other.url; }
    bool operator>(const Url& other) const { return url > other.url; }
    bool operator==(const Url& other) const { return url == other.url; }

    static string fechaActual() {
        time_t now = time(0);
        char* dt = ctime(&now);
        dt[strlen(dt) - 1] = '\0'; // elimina salto de línea
        return string(dt);
    }

    map<string, int> visitasPorDia() const {
        map<string, int> conteo;
        for (const string& fecha : fechas) {
            conteo[fecha]++;
        }
        return conteo;
    }

    const vector<string>& getFechas() const {
        return fechas;
    }


};




// Clase Nodo para AVL
template <typename T>
class Nodo {
    T dato;
    Nodo* izq;
    Nodo* der;
    int altura;

public:
    Nodo(T d) : dato(d), izq(nullptr), der(nullptr), altura(1) {}

    T& getDato() { return dato; }
    Nodo* getIzq() { return izq; }
    Nodo* getDer() { return der; }
    Nodo*& getIzqRef() { return izq; }
    Nodo*& getDerRef() { return der; }
    void setDato(T d) { dato = d; }
    void setIzq(Nodo* n) { izq = n; }
    void setDer(Nodo* n) { der = n; }
    int getAltura() { return altura; }
    void setAltura(int h) { altura = h; }
};


// Clase Arbol

// Clase Arbol AVL
template <typename T>
class Arbol {
    Nodo<T>* raiz;

    int altura(Nodo<T>* nodo) {
        return nodo ? nodo->getAltura() : 0;
    }

    int balance(Nodo<T>* nodo) {
        return nodo ? altura(nodo->getIzq()) - altura(nodo->getDer()) : 0;
    }

    void actualizarAltura(Nodo<T>* nodo) {
        int altIzq = altura(nodo->getIzq());
        int altDer = altura(nodo->getDer());
        nodo->setAltura(1 + max(altIzq, altDer));
    }

    Nodo<T>* rotarDerecha(Nodo<T>* y) {
        Nodo<T>* x = y->getIzq();
        Nodo<T>* T2 = x->getDer();

        x->setDer(y);
        y->setIzq(T2);

        actualizarAltura(y);
        actualizarAltura(x);

        return x;
    }

    Nodo<T>* rotarIzquierda(Nodo<T>* x) {
        Nodo<T>* y = x->getDer();
        Nodo<T>* T2 = y->getIzq();

        y->setIzq(x);
        x->setDer(T2);

        actualizarAltura(x);
        actualizarAltura(y);

        return y;
    }

    Nodo<T>* insertarAVL(Nodo<T>* nodo, T dato) {
        if (!nodo) return new Nodo<T>(dato);

        if (dato < nodo->getDato())
            nodo->setIzq(insertarAVL(nodo->getIzq(), dato));
        else if (dato > nodo->getDato())
            nodo->setDer(insertarAVL(nodo->getDer(), dato));
        else {
            nodo->getDato().visitar();
            return nodo;
        }

        actualizarAltura(nodo);
        int b = balance(nodo);

        // Rotaciones
        if (b > 1 && dato < nodo->getIzq()->getDato()) return rotarDerecha(nodo);
        if (b < -1 && dato > nodo->getDer()->getDato()) return rotarIzquierda(nodo);
        if (b > 1 && dato > nodo->getIzq()->getDato()) {
            nodo->setIzq(rotarIzquierda(nodo->getIzq()));
            return rotarDerecha(nodo);
        }
        if (b < -1 && dato < nodo->getDer()->getDato()) {
            nodo->setDer(rotarDerecha(nodo->getDer()));
            return rotarIzquierda(nodo);
        }

        return nodo;
    }

    void print_in(Nodo<T>* nodo) {
        if (!nodo) return;
        print_in(nodo->getIzq());
        std::cout << nodo->getDato().to_string() << std::endl;
        print_in(nodo->getDer());
    }

    Nodo<T>* buscar(Nodo<T>* nodo, const T& dato) {
        if (!nodo) return nullptr;
        if (dato == nodo->getDato()) return nodo;
        if (dato < nodo->getDato()) return buscar(nodo->getIzq(), dato);
        return buscar(nodo->getDer(), dato);
    }


public:
    Arbol() : raiz(nullptr) {}

    void add(T dato) {
        raiz = insertarAVL(raiz, dato);
    }
    
    Nodo<T>* eliminar_rec(Nodo<T>* nodo, const T& valor) {
        if (!nodo) return nullptr;
        if (valor < nodo->getDato()) {
            nodo->setIzq(eliminar_rec(nodo->getIzq(), valor));
        } else if (valor > nodo->getDato()) {
            nodo->setDer(eliminar_rec(nodo->getDer(), valor));
        } else {
            if (!nodo->getIzq()) return nodo->getDer();
            if (!nodo->getDer()) return nodo->getIzq();

            Nodo<T>* sucesor = nodo->getDer();
            while (sucesor->getIzq()) sucesor = sucesor->getIzq();
            nodo->setDato(sucesor->getDato());
            nodo->setDer(eliminar_rec(nodo->getDer(), sucesor->getDato()));
        }
        return nodo;
    }

    void insertarOActualizar(string urlStr) {
        T dato(urlStr);
        Nodo<T>* nodo = buscar(raiz, dato);
        if (nodo) {
            nodo->getDato().visitar();
        } else {
            add(dato);
        }
    }

    
    void eliminar(T valor) {
        raiz = eliminar_rec(raiz, valor);
    }

    void eliminarMenosVisitadaLFU() {
        Nodo<T>* minNodo = nullptr;
        int minVisitas = INT_MAX;
        buscarMenosVisitada(raiz, minNodo, minVisitas);
        if (minNodo) {
            std::cout << "Eliminando: " << minNodo->getDato().to_string() << std::endl;
            this->eliminar(minNodo->getDato());  // ← esta línea es la clave
        }
    }

    void print() {
        print_in(raiz);
    }

void buscarMenosVisitada(Nodo<T>* nodo, Nodo<T>*& minNodo, int& minVisitas) {
    if (!nodo) return;
    if (nodo->getDato().getVisitas() < minVisitas) {
        minVisitas = nodo->getDato().getVisitas();
        minNodo = nodo;
    }
    buscarMenosVisitada(nodo->getIzq(), minNodo, minVisitas);
    buscarMenosVisitada(nodo->getDer(), minNodo, minVisitas);
}
    void limpiarPorVisitasBajas(Nodo<T>*& nodo, int umbral) {
        if (!nodo) return;

        limpiarPorVisitasBajas(nodo->getIzqRef(), umbral);
        limpiarPorVisitasBajas(nodo->getDerRef(), umbral);


        if (nodo->getDato().getVisitas() <= umbral) {
            eliminar(nodo->getDato());
        }
    }

    void limpiezaAutomatica(int umbral) {
        limpiarPorVisitasBajas(raiz, umbral);
    }

    void buscarPorPrefijo(string prefijo) {
        buscarPorPrefijoRec(raiz, prefijo);
    }

    void buscarPorPrefijoRec(Nodo<T>* nodo, const string& prefijo) {
        if (!nodo) return;

        buscarPorPrefijoRec(nodo->getIzq(), prefijo);

        if (nodo->getDato().getUrl().find(prefijo) == 0) {  // empieza con el prefijo
            cout << nodo->getDato().to_string() << endl;
        }

        buscarPorPrefijoRec(nodo->getDer(), prefijo);
    }

    void recorrer(function<void(const T&)> f) const {
        recorrerRec(raiz, f);
    }

    void recorrerRec(Nodo<T>* nodo, function<void(const T&)> f) const{
        if (!nodo) return;
        recorrerRec(nodo->getIzq(), f);
        f(nodo->getDato());
        recorrerRec(nodo->getDer(), f);
    }



};

//Usario
class Usuario {
    string nombre;
    Arbol<Url> historial;
    int contadorVisitas = 0;
    const int LIMITE_LIMPIEZA = 10;


public:
    Usuario(string n) : nombre(n) {}

    void visitarURL(string url) {
        historial.insertarOActualizar(url);
        contadorVisitas++;

        if (contadorVisitas >= LIMITE_LIMPIEZA) {
            cout << "\nLimpieza automatica para " << nombre << "\n";
            historial.limpiezaAutomatica(1); // Elimina URLs con ≤ 1 visita
            contadorVisitas = 0; // Reinicia el contador
        }
    }


    void mostrarHistorial() {
        cout << "\nHistorial de " << nombre << ":\n";
        historial.print();
    }

    void aplicarLFU() {
        historial.eliminarMenosVisitadaLFU();
    }

    string getNombre() const { return nombre; }

    void buscarURLsConPrefijo(const string& prefijo) {
        cout << "URLs que comienzan con '" << prefijo << "':\n";
        historial.buscarPorPrefijo(prefijo);
    }

    void estadisticasPorDia() {
        cout << "\nEstadisticas de " << nombre << ":\n";

        map<string, int> totalPorDia;
        map<string, int> visitasPorURL;
        string urlMasVisitada;
        int maxVisitas = 0;

        historial.recorrer([&](const Url& url) {
            visitasPorURL[url.getUrl()] = url.getVisitas();
            if (url.getVisitas() > maxVisitas) {
                maxVisitas = url.getVisitas();
                urlMasVisitada = url.getUrl();
            }

            auto porDia = url.visitasPorDia();
            for (auto& p : porDia) {
                totalPorDia[p.first] += p.second;
            }
        });

        cout << " URL más visitada: " << urlMasVisitada << " con " << maxVisitas << " visitas\n";

        cout << "\n Visitas por día:\n";
        for (auto& p : totalPorDia) {
            cout << p.first << ": " << p.second << " visitas\n";
        }

        cout << "\n Grafico textual por URL:\n";
        for (auto& p : visitasPorURL) {
            cout << p.first << " [" << string(p.second, '#') << "] (" << p.second << ")\n";
        }
    }

    void mostrarGraficoUrls() {
        map<string, int> visitas;

        historial.recorrer([&](const Url& url) {
            visitas[url.getUrl()] = url.getVisitas();
        });

        for (auto& p : visitas) {
            cout << p.first << ": " << string(p.second, '*') << " (" << p.second << ")\n";
        }
    }
#include <filesystem> // Para verificar existencia (C++17)

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------- EXPOSICIÓN SEBASTIÁN -----------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void exportarHistorialComoHTML() const {
    // 1. CSS
    if (!std::filesystem::exists("estilos.css")) {
        std::ofstream css("estilos.css");
        css << R"(body {
  font-family: Arial, sans-serif;
  background: #f2f2f2;
  margin: 0;
}
.container {
  display: flex;
  height: 100vh;
}
.sidebar {
  background: #fff;
  width: 250px;
  padding: 20px;
  border-right: 1px solid #ddd;
}
.search-bar {
  width: 100%;
  padding: 8px;
  margin-bottom: 15px;
  border-radius: 4px;
  border: 1px solid #ccc;
}
.content {
  flex: 1;
  padding: 20px;
  overflow-y: auto;
}
.entrada {
  background: #fff;
  margin-bottom: 10px;
  padding: 10px;
  border-radius: 8px;
  display: flex;
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}
.favicon {
  font-size: 24px;
  margin-right: 10px;
}
.info {
  flex: 1;
}
.top-bar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}
.actions button {
  background-color: #2563eb; /* azul más fuerte */
  color: white;
  border: none;
  padding: 10px 14px;
  border-radius: 6px;
  margin-left: 10px;
  cursor: pointer;
  font-size: 14px;
  font-weight: 600;
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);
  transition: background-color 0.3s, transform 0.2s;
}
.actions button:hover {
  background-color: #1e40af; /* más oscuro al pasar */
  transform: scale(1.03);
}

)";
        css.close();
    }

    // 2. JS
    if (!std::filesystem::exists("script.js")) {
        std::ofstream js("script.js");
        js << R"(
// Filtrar por fecha
document.getElementById('filtrarFecha').addEventListener('click', function () {
    const fecha = document.getElementById('filtroFechaInput').value;
    const secciones = document.querySelectorAll('.historial-dia');
    secciones.forEach(sec => {
        const dataFecha = sec.dataset.fecha;
        sec.style.display = (!fecha || dataFecha === fecha) ? 'block' : 'none';
    });
});

// Exportar HTML visible
document.getElementById('exportarDatos').addEventListener('click', function () {
    const contenido = document.querySelector('main').innerHTML;
    const blob = new Blob([`<html><body>${contenido}</body></html>`], { type: 'text/html' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'exportado.html';
    a.click();
});

// Eliminar visualmente
document.getElementById('eliminarDatos').addEventListener('click', function () {
    document.querySelectorAll('.entrada').forEach(el => el.remove());
});
)";
        js.close();
    }

    // 3. HTML
    std::ofstream archivo("historial.html");
    archivo << R"(<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<title>Historial</title>
<link rel="stylesheet" href="estilos.css">
</head>
<body>
<div class="container">
<aside class="sidebar">
    <h2>Historial</h2>
    <input type="date" id="filtroFechaInput" class="search-bar">
    <nav>
        <ul>
            <li class="activo">📋 Todo</li>
            <li>🕓 Cerradas recientemente</li>
            <li>🖥️ Pestañas de otros dispositivos</li>
        </ul>
    </nav>
</aside>
<main class="content">
<div class="top-bar">
    <span>Todo</span>
    <div class="actions">
        <button id="filtrarFecha">📅 Filtrar por fecha</button>
        <button id="exportarDatos">⇩ Exportar datos</button>
        <button id="eliminarDatos">🗑 Eliminar datos</button>
    </div>
</div>
)";

    // 4. Historial por fecha (desde clase Url)
    std::map<std::string, std::vector<Url>> agrupado;
    historial.recorrer([&](const Url& url) {
        for (const auto& f : url.visitasPorDia()) {
            agrupado[f.first].push_back(url);
        }
    });

    for (const auto& [fecha, urls] : agrupado) {
        // ISO 8601 para data-fecha
        std::string fechaISO = fecha.substr(20, 4) + "-" +
            (fecha.substr(4, 3) == "Jan" ? "01" :
             fecha.substr(4, 3) == "Feb" ? "02" :
             fecha.substr(4, 3) == "Mar" ? "03" :
             fecha.substr(4, 3) == "Apr" ? "04" :
             fecha.substr(4, 3) == "May" ? "05" :
             fecha.substr(4, 3) == "Jun" ? "06" :
             fecha.substr(4, 3) == "Jul" ? "07" :
             fecha.substr(4, 3) == "Aug" ? "08" :
             fecha.substr(4, 3) == "Sep" ? "09" :
             fecha.substr(4, 3) == "Oct" ? "10" :
             fecha.substr(4, 3) == "Nov" ? "11" : "12") + "-" + fecha.substr(8, 2);

        archivo << "<section class='historial-dia' data-fecha='" << fechaISO << "'>\n";
        archivo << "<h3>" << fecha << "</h3>\n";

        for (const Url& url : urls) {
            archivo << "<div class='entrada'>\n";
            archivo << "<span class='favicon'>🌐</span>\n";
            archivo << "<div class='info'>\n";
            archivo << "<p><strong>" << url.getUrl() << "</strong></p>\n";
            archivo << "<small>Visitas: " << url.getVisitas() << "</small>\n";
            archivo << "</div></div>\n";
        }
        archivo << "</section>\n";
    }

    archivo << R"(</main>
</div>
<script src="script.js"></script>
</body>
</html>)";
    archivo.close();

    // 5. Abrir en navegador (Windows)
    system("start historial.html");
}




};

// Sistema
class Sistema {
    vector<Usuario> usuarios;

public:
    void agregarUsuario(string nombre) {
        usuarios.emplace_back(nombre);
    }

    Usuario* getUsuario(string nombre) {
        for (auto& u : usuarios) {
            if (u.getNombre() == nombre) return &u;
        }
        return nullptr;
    }

    void visitarURL(string usuario, string url) {
        Usuario* u = getUsuario(usuario);
        if (u) u->visitarURL(url);
        else cout << "Usuario no encontrado.\n";
    }

    void mostrarHistorialUsuario(string nombre) {
        Usuario* u = getUsuario(nombre);
        if (u) u->mostrarHistorial();
        else cout << "Usuario no encontrado.\n";
    }

    void aplicarLFUUsuario(string nombre) {
        Usuario* u = getUsuario(nombre);
        if (u) u->aplicarLFU();
    }
    void buscarURLsUsuarioPorPrefijo(const string& nombreUsuario, const string& prefijo) {
        Usuario* u = getUsuario(nombreUsuario);
        if (u) u->buscarURLsConPrefijo(prefijo);
        else cout << "Usuario no encontrado.\n";
    }

    void graficosPorUsuario() {
        cout << "\n Graficos de usuarios:\n";
        for (Usuario& u : usuarios) {
            cout << "\nUsuario: " << u.getNombre() << "\n";
            u.mostrarGraficoUrls();
        }
    }

    void estadisticasPorDiaUsuario(string nombre) {
        Usuario* u = getUsuario(nombre);
        if (u) u->estadisticasPorDia();
        else cout << "Usuario no encontrado.\n";
    }

    void mostrarGraficoUsuario(string nombre) {
        Usuario* u = getUsuario(nombre);
        if (u) u->mostrarGraficoUrls();
        else cout << "Usuario no encontrado.\n";
    }
    void exportarHistorialUsuarioHTML(const string& nombre) {
        Usuario* u = getUsuario(nombre);
        if (u) u->exportarHistorialComoHTML();
        else cout << "Usuario no encontrado.\n";
    }



};


int main() {
    Sistema sistema;

    sistema.agregarUsuario("Ana");
    sistema.agregarUsuario("Luis");

    sistema.visitarURL("Ana", "google.com");
    sistema.visitarURL("Ana", "openai.com");
    sistema.visitarURL("Ana", "openai.com");
    sistema.visitarURL("Luis", "youtube.com");
    sistema.visitarURL("Luis", "openai.com");
    sistema.visitarURL("Luis", "youtube.com");

    sistema.mostrarHistorialUsuario("Ana");
    sistema.aplicarLFUUsuario("Ana");
    sistema.mostrarHistorialUsuario("Ana");
    sistema.buscarURLsUsuarioPorPrefijo("Ana", "open");


    // Agregamos dos usuarios
    sistema.agregarUsuario("Sara");
    sistema.agregarUsuario("Valen");

    // Simulación de visitas de Sara
    sistema.visitarURL("Sara", "google.com");
    sistema.visitarURL("Sara", "openai.com");
    sistema.visitarURL("Sara", "openai.com");
    sistema.visitarURL("Sara", "github.com");
    sistema.visitarURL("Sara", "openai.com");

    // Simulación de visitas de Valen
    sistema.visitarURL("Valen", "youtube.com");
    sistema.visitarURL("Valen", "google.com");
    sistema.visitarURL("Valen", "github.com");
    sistema.visitarURL("Valen", "google.com");

    // Mostrar historial completo
    sistema.mostrarHistorialUsuario("Sara");
    sistema.mostrarHistorialUsuario("Valen");

    // Estadísticas por día
    cout << "\n Estadísticas de Sara:\n";
    sistema.estadisticasPorDiaUsuario("Sara");

    cout << "\n Estadísticas de Valen:\n";
    sistema.estadisticasPorDiaUsuario("Valen");

    // Gráficos de visitas por URL
    cout << "\n Gráfico de visitas (Sara):\n";
    sistema.mostrarGraficoUsuario("Sara");

    cout << "\n Gráfico de visitas (Valen):\n";
    sistema.mostrarGraficoUsuario("Valen");

    sistema.getUsuario("Sara")->exportarHistorialComoHTML();



    return 0;
}

