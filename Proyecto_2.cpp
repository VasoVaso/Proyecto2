#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <list>
#include <string>

using namespace std;
bool validGame = true;

struct Guardian
{
    string name;
    int level = 0;
    char rank = 'X';
    string city;
    int cityID = 0;
    string master;
    vector<Guardian*> apprentices;

    Guardian(const string& name, int level, char rank, const string& master, const string& city) : name(name), level(level), rank(rank), master(master), city(city) {}
};

struct City
{
    string name;
    int id = 0;
};

class Graph
{
public:
    unordered_map<string, list<string>> adjacencyList;

    void addEdge(const string& vertex1, const string& vertex2)
    {
        adjacencyList[vertex1].push_back(vertex2);
        adjacencyList[vertex2].push_back(vertex1);
    }

    void addConnection(const string& vertex1, const string& vertex2)
    {
        if (adjacencyList.find(vertex1) != adjacencyList.end() && find(adjacencyList[vertex1].begin(), adjacencyList[vertex1].end(), vertex2) == adjacencyList[vertex1].end())
        {
            adjacencyList[vertex1].push_back(vertex2);
            adjacencyList[vertex2].push_back(vertex1);
            cout << ">> Added connection: " << vertex1 << " <-> " << vertex2 << endl;
        }
        else
        {
            cout << ">> The connections isn't possible because the connection already exists or one of the cities doesn't exists." << endl;
        }
    }

    void printGraph()
    {
        for (const auto& pair : adjacencyList)
        {
            cout << " " << pair.first << ":";
            for (const auto& adjacent : pair.second)
            {
                cout << " - " << adjacent << " - ";
            }
            cout << "\n";
        }
    }
};

struct rankNode
{
    Guardian guardian;
    rankNode* left;
    rankNode* right;

    rankNode(const Guardian& guardian) : guardian(guardian), left(nullptr), right(nullptr) {}
};

//----- PROTOTIPOS -----//

// �RBOL BINARIO (RANKING)
rankNode* insertRankNode(rankNode* root, const Guardian* guardian);
void printRankTree(rankNode* root);
void freeRankNode(rankNode* root);

// �RBOL GENERAL (JERARQU�A)
Guardian* createHierarchyTree(const vector<Guardian*>& guardianList, const string& masterGuardian);
void printHierarchyTree(Guardian* node, int hierarchyLevel = 0);
void freeHierarchyTree(Guardian* node);

//
int getCityID(string city);

bool readCitiesFile(const string& fileName, Graph& graph)
{
    int cityExists = 0;
    ifstream file(fileName);
    if (!file.is_open())
    {
        cerr << ">> Error opening cities file." << endl;
        return false;
    }

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string vertex1, vertex2;
        getline(iss, vertex1, ',');
        getline(iss, vertex2);

        // Eliminar espacios en blanco al principio y al final de las cadenas
        vertex1.erase(0, vertex1.find_first_not_of(" \t\n\r\f\v"));
        vertex1.erase(vertex1.find_last_not_of(" \t\n\r\f\v") + 1);
        vertex2.erase(0, vertex2.find_first_not_of(" \t\n\r\f\v"));
        vertex2.erase(vertex2.find_last_not_of(" \t\n\r\f\v") + 1);

        graph.addEdge(vertex1, vertex2);

        cityExists = getCityID(vertex1);
        if (cityExists == -1) { validGame = false; }
        cityExists = getCityID(vertex2);
        if (cityExists == -1) { validGame = false; }
    }

    file.close();
    return true;
}


vector<Guardian*> readGuardianFile(const string& fileName)
{
    vector<Guardian*> guardianList;
    ifstream file(fileName);

    if (!file.is_open())
    {
        cerr << "Error al abrir el archivo." << endl;
        return guardianList;
    }

    string linea;
    while (getline(file, linea))
    {
        istringstream ss(linea);
        string nombre, maestro, ciudad;
        int level;
        char rank = 'X';

        getline(ss, nombre, ',');
        ss >> level;
        ss.ignore(); // Ignorar la coma despu�s del nivel
        getline(ss, maestro, ',');
        getline(ss, ciudad, ',');

        if (level == 100) { rank = 'M'; }
        else if (level >= 90 && level <= 99) { rank = 'G'; }
        else { rank = 'C'; }

        Guardian* nuevoGuardian = new Guardian(nombre, level, rank, maestro, ciudad);
        guardianList.push_back(nuevoGuardian);
    }

    file.close();
    return guardianList;
}

/*
vector<Guardian> readGuardianFile(const string& fileName)
{
    int level100 = 0, level90 = 0;
    vector<Guardian> guardianList;
    ifstream file(fileName);

    if (!file.is_open())
    {
        cerr << ">> Error opening guardians file." << fileName << "\n" << endl;
        return guardianList;
    }

    string linea;
    while (getline(file, linea))
    {
        istringstream ss(linea);
        Guardian guardian;
        getline(ss, guardian.name, ',');
        ss >> guardian.level;
        ss.ignore();
        getline(ss, guardian.master, ',');
        getline(ss, guardian.city);

        guardian.cityID = getCityID(guardian.city);
        if (guardian.cityID == -1) { validGame = false; }

        if (guardian.level == 100) { guardian.rank = 'M'; }
        else if (guardian.level >= 90 && guardian.level <= 99) { guardian.rank = 'G'; }
        else { guardian.rank = 'C'; }

        guardianList.push_back(guardian);

        if (guardian.level == 100) { level100++; }
        if (guardian.level >= 90 && guardian.level <= 99) { level90++; }
        if (guardian.level < 0 || guardian.level > 100) { validGame = false; }
    }

    if (level100 > 1 || level90 > 3) { validGame = false; }

    file.close();
    return guardianList;
}*/

int main()
{
    int option = 0, guardianCount = 0;
    string city1, city2;

    Graph map;

    if (readCitiesFile("cities.conf", map)) {}
    else { validGame = false; }

    vector<Guardian*> guardianList = readGuardianFile("guardians.conf");

    rankNode* rankRoot = nullptr;

    // CREACI�N DEL �RBOL BINARIO (RANKING) A PARTIR DE LA LISTA DE GUARDIANES CARGADOS DESDE EL ARCHIVO
    for (const auto& guardian : guardianList)
    {
        rankRoot = insertRankNode(rankRoot, guardian);
    }

    // CREACI�N DEL �RBOL GENERAL (JERARQU�A) A PARTIR DEL NOMBRE DEL GUARDI�N MAESTRO OBTENIDO DESDE EL ARCHIVO
    Guardian* top = createHierarchyTree(guardianList, "Freya");

    if (validGame == true)
    {
        do
        {
            cout << "   THE GUARDIANS BATTLE" << endl;
            cout << "\n1. View Candidates List.\n2. View Guardian.\n3. Know the Kingdom.\n4. Watch a Battle.\n5. Exit.\n\n>> Your option: ";
            cin >> option;
            switch (option)
            {
            case 1: // VER LISTA DE CANDIDATOS 
                system("cls");

                cout << ">> Guardians Ranking:\n" << endl;
                cout << " (M = GRAND MASTER, G = GUARDIAN OF THE KINGDOM, C = CANDIDATE)\n" << endl;
                printRankTree(rankRoot);
                cout << "\n";

                system("pause");
                system("cls");
                break;
            case 2: // VER INFORMACI�N DE GUARDI�N SELECCIONADO
                system("cls");

                cout << ">> Guardians Hierarchy:\n" << endl;
                printHierarchyTree(top);
                cout << "\n";

                system("pause");
                system("cls");
                break;
            case 3: // ENTRAR EN MEN� DE "CONOCER EL REINO"
                system("cls");

                do
                {
                    cout << "   Know the Kingdom" << endl;
                    cout << "\n1. Visit a city.\n2. View connections between cities.\n3. Create a path between cities.\n4. Go back.\n\n>> Your option: ";
                    cin >> option;
                    switch (option)
                    {
                    case 1: // VER UNA CIUDAD
                        system("cls");

                        system("pause");
                        system("cls");
                        break;
                    case 2: // VER CONEXI�N ENTRE CIUDADES
                        system("cls");

                        cout << ">> Cities connections\n" << endl;
                        map.printGraph();
                        cout << "\n";

                        system("pause");
                        system("cls");
                        break;
                    case 3:
                        system("cls");

                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // LIMPIAR BUFFER
                        cout << ">> Enter the first city: "; getline(cin, city1);
                        cout << ">> Enter the second city: "; getline(cin, city2);
                        cout << "\n";
                        map.addConnection(city1, city2);
                        cout << "\n";

                        system("pause");
                        system("cls");
                        break;
                    default:
                        break;
                    }

                } while (option != 4);

                system("cls");
                break;
            case 4:
                system("cls");
                break;
            default:
                system("cls");
                break;
            }
        } while (option != 5);
    }
    else { cout << ">> Game invalid. Check that the files are correct." << endl; }

    freeRankNode(rankRoot);
    freeHierarchyTree(top);
}

rankNode* insertRankNode(rankNode* root, const Guardian* guardian)
{
    if (root == nullptr)
    {
        return new rankNode(*guardian);
    }

    if (guardian->level < root->guardian.level)
    {
        root->left = insertRankNode(root->left, guardian);
    }
    else
    {
        root->right = insertRankNode(root->right, guardian);
    }

    return root;
}

void printRankTree(rankNode* root)
{
    if (root != nullptr)
    {
        // IN-ORDER
        printRankTree(root->right);
        cout << " " << root->guardian.rank << " " << root->guardian.name << " - Level: " << root->guardian.level << endl;
        printRankTree(root->left);
    }
}

void freeRankNode(rankNode* root)
{
    if (root != nullptr)
    {
        freeRankNode(root->left);
        freeRankNode(root->right);
        delete root;
    }
}

Guardian* createHierarchyTree(const vector<Guardian*>& guardianList, const string& masterGuardian)
{
    Guardian* top = nullptr;

    for (Guardian* guardian : guardianList)
    {
        if (guardian->name == masterGuardian)
        {
            top = guardian;
        }

        for (Guardian* apprentice : guardianList)
        {
            if (apprentice->master == guardian->name)
            {
                guardian->apprentices.push_back(apprentice);
            }
        }
    }

    return top;
}

void printHierarchyTree(Guardian* node, int hierarchyLevel)
{
    if (node == nullptr)
    {
        return;
    }

    for (int i = 0; i < hierarchyLevel; ++i) // ++i (preincremento): Incrementa el valor de i antes de que se eval�e la expresi�n en la que se encuentra
    {
        cout << "\t"; 
    }

    cout << node->name << ", " << "Level " << node->level << " (" << node->city << ")\n";

    for (Guardian* apprentice : node->apprentices)
    {
        printHierarchyTree(apprentice, hierarchyLevel + 1);
    }
}

void freeHierarchyTree(Guardian* node)
{
    if (node == nullptr)
    {
        return;
    }

    for (Guardian* apprentice : node->apprentices)
    {
        freeHierarchyTree(apprentice);
    }

    delete node;
}

int getCityID(string city)
{
    int id = 0;

    if (city == "Tesla Capital") { id = 1; }
    else if (city == "Capital City") { id = 2; }
    else if (city == "Forest City") { id = 3; }
    else if (city == "Valley City") { id = 4; }
    else if (city == "Mountain City") { id = 5; }
    else if (city == "Desert City") { id = 6; }
    else if (city == "Silverstone City") { id = 7; }
    else if (city == "Cliff City") { id = 8; }
    else if (city == "River City") { id = 9; }
    else if (city == "Sunflower Fields City") { id = 10; }
    else if (city == "Hill City") { id = 11; }
    else if (city == "Storm City") { id = 12; }
    else if (city == "Coast City") { id = 13; }
    else if (city == "Island City") { id = 14; }
    else { id = -1; }

    return id;
}