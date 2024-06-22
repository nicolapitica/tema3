#include <iostream>
#include <vector>
#include <memory>

// Interfata Clona pentru a permite clonarea polimorfa a obiectelor
class Clona {
public:
    // Metoda virtuala pura clone() care va returna o copie a obiectului derivat.
    // std::unique_ptr<Clona> este un smart pointer care gestioneaza automat memoria obiectului clonat.
    virtual std::unique_ptr<Clona> clone() const = 0;
    virtual void info() const = 0; 
    virtual ~Clona() = default;  
};

// Clasa de baza Camera, care deriva din Clona pentru a permite clonarea.
// Folosim template-uri pentru a permite reutilizarea codului pentru diferite tipuri de camere.
template <typename Derivata>
class Camera : public Clona {
public:
    // Metoda clone() care foloseste template-ul pentru a returna o copie a obiectului derivat.
    // std::make_unique<Derivata> creeaza un smart pointer care gestioneaza un nou obiect de tip Derivata.
    std::unique_ptr<Clona> clone() const override {
        return std::make_unique<Derivata>(static_cast<const Derivata&>(*this));
    }
    virtual void info() const override = 0;
};

// Clasa derivata pentru sala tronului, folosind template-ul Camera.
class CameraTron : public Camera<CameraTron> {
public:
    void info() const override {
        std::cout << "camera tronului, ";
    }
};

// Clasa derivata pentru temnita, folosind template-ul Camera.
class Temnita : public Camera<Temnita> {
public:
    void info() const override {
        std::cout << "temnita, ";
    }
};

// Clasa pentru castel, care contine camere.
class Castel {
    // Vector de smart pointers pentru a gestiona camerele.
    // std::unique_ptr<Clona> este folosit pentru a stoca obiecte de tip Clona.
    std::vector<std::unique_ptr<Clona>> camere;
public:
    // Metoda pentru a adauga o camera in castel.
    // std::move(camera) transfera proprietatea smart pointer-ului camera in vectorul camere.
    void adaugaCamera(std::unique_ptr<Clona> camera) {
        camere.push_back(std::move(camera));
    }
    void info() const {
        std::cout<<"Castelul are: ";
        for (const auto& camera : camere) {
            camera->info();
        }
        std::cout<<""<<"\n";
    }
};

// Factory Method pentru crearea camerelor.
class CameraFactory {
public:
    enum TipCamera { CAMERA_TRON, TEMNITA };

    // Metoda pentru a crea camere pe baza tipului specificat.
    // Returneaza un smart pointer catre obiectul creat.
    std::unique_ptr<Clona> creareCamera(TipCamera tip) {
        switch (tip) {
        case CAMERA_TRON:
            return std::make_unique<CameraTron>();
        case TEMNITA:
            return std::make_unique<Temnita>();
        default:
            return nullptr; 
        }
    }
};

// Singleton
class Rege {
    // std::unique_ptr<Rege> este folosit pentru a gestiona automat instanta unica a regelui.
    static std::unique_ptr<Rege> instanta;
    Castel castel;  // Obiectul Castel gestionat de Rege.

    // Constructor privat pentru a preveni crearea de noi instante.
    Rege() = default;

public:
    // Stergerea constructorului de copiere si a operatorului de asignare.
    // Aceasta previne copierea instantei Singleton.
    Rege(const Rege&) = delete;
    Rege& operator=(const Rege&) = delete;

    // Metoda statica pentru a obtine instanta unica a Rege.
    // Creeaza instanta daca nu exista deja.
    static Rege& getInstance() {
        if (!instanta) {
            instanta.reset(new Rege());
        }
        return *instanta;
    }

    void adaugaCameraCastel(std::unique_ptr<Clona> camera) {
        castel.adaugaCamera(std::move(camera));
    }

    void infoCastel() const {
        castel.info();
    }
};

// Initializare instanta singleton cu nullptr, ceea ce inseamna ca nu exista inca o instanta.
std::unique_ptr<Rege> Rege::instanta = nullptr;

int main() {
    // Obtinerea instantei unice a Rege.
    Rege& rege = Rege::getInstance();
    CameraFactory constructie;
    enum Optiune { ADD_CAMERA=1, ADD_TEMNITA,DESC_CASTEL, IESIRE};
    int optiune = 0;
    while (optiune != 4) {
        std::cout << "1. Adauga o camera a tronului"<<""<<"\n";
        std::cout << "2. Adauga o temnita"<<"\n";
        std::cout << "3. Descrie castel"<<"\n";
        std::cout << "4. Incheiere operatiune"<<"\n";
        std::cout << "Alege o optiune: ";
        std::cin >> optiune;
        switch (optiune) {
        case ADD_CAMERA:
            rege.adaugaCameraCastel(constructie.creareCamera(CameraFactory::CAMERA_TRON));
            std::cout << "Camera tronului adaugata."<<"\n";
            break;
        case ADD_TEMNITA:
            rege.adaugaCameraCastel(constructie.creareCamera(CameraFactory::TEMNITA));
            std::cout << "Temnita adaugata."<<"\n";
            break;
        case DESC_CASTEL:
            rege.infoCastel();
            break;
        case IESIRE:
            std::cout << "Iesire program."<<"\n";
            break;
        default:
            std::cout << "Optiunea nu se afla in meniu."<<"\n";
            break;
        }
    }

    return 0; 
}