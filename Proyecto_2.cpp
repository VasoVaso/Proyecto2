#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <list>
#include <string>

using namespace std;
bool validGame = true;
string masterGuardian;

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
string getRankString(char rank);
void showGuardiansList(const vector<Guardian*>& guardianList);
void getGuardianInfo(const vector<Guardian*>& guardianList, int index);
void showGuardianInfo(const Guardian& guardian);

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
    int level100 = 0, level90 = 0;
    vector<Guardian*> guardianList;
    ifstream file(fileName);

    if (!file.is_open())
    {
        cerr << ">> Error opening guardians file." << endl;
        return guardianList;
    }

    string line;
    while (getline(file, line))
    {
        istringstream ss(line);
        string name, master, city;
        int level, cityID;
        char rank = 'X';

        getline(ss, name, ',');
        ss >> level;
        ss.ignore(); // Ignorar la coma despu�s del nivel
        getline(ss, master, ',');
        getline(ss, city, ',');

        cityID = getCityID(city);
        if (cityID == -1) { validGame = false; }

        if (level == 100) { rank = 'M'; masterGuardian = name; }
        else if (level >= 90 && level <= 99) { rank = 'G'; }
        else { rank = 'C'; }

        Guardian* nuevoGuardian = new Guardian(name, level, rank, master, city);
        guardianList.push_back(nuevoGuardian);

        if (level == 100) { level100++; }
        if (level >= 90 && level <= 99) { level90++; }
        if (level < 0 || level > 100) { validGame = false; }
    }

    if (level100 > 1 || level90 > 3) { validGame = false; }

    file.close();
    return guardianList;
}

int main()
{
    int option = 0, guardianCount = 0, selectedIndex = 0;
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
    Guardian* top = createHierarchyTree(guardianList, masterGuardian);

    if (validGame == true)
    {
        do
        {
            cout << "   THE GUARDIANS BATTLE" << endl;
            cout << "\n1. View Candidates List.\n2. View Guardian.\n3. Know the Kingdom.\n4. Watch a Battle.\n5. Exit.\n\n>> Your option: ";
            cin >> option;
            switch (option)
            {
            case 1: // MEN� VER LISTA DE CANDIDATOS 
                system("cls");

                do
                {
                    cout << "   View Candidates List" << endl;
                    cout << "\n1. View by Level.\n2. View by hierarchy.\n3. Go back.\n\n>> Your option: ";
                    cin >> option;
                    switch (option)
                    {
                    case 1: // VER RANKING
                        system("cls");

                        cout << ">> Guardians Ranking:\n" << endl;
                        cout << " (M = GRAND MASTER, G = GUARDIAN OF THE KINGDOM, C = CANDIDATE)\n" << endl;
                        printRankTree(rankRoot);
                        cout << "\n";

                        system("pause");
                        system("cls");
                        break;
                    case 2: // VER JERARQU�A
                        system("cls");

                        cout << ">> Guardians Hierarchy:\n" << endl;
                        printHierarchyTree(top);
                        cout << "\n";

                        system("pause");
                        system("cls");
                        break;
                    default:
                        break;
                    }
                } while (option != 3);

                system("cls");
                break;
            case 2: // VER INFORMACI�N DE GUARDI�N SELECCIONADO
                system("cls");

                showGuardiansList(guardianList);
                cout << ">> Select the index of the guardian to see the details: ";
                cin >> selectedIndex;
                cout << "\n";
                getGuardianInfo(guardianList, selectedIndex);
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

void showGuardiansList(const vector<Guardian*>& guardianList) 
{
    cout << ">> List of Guardians:\n" << endl;
    for (size_t i = 0; i < guardianList.size(); ++i) 
    {
        cout << i << ": " << guardianList[i]->name << endl;
    }
    cout << "--------------------------" << endl;
}

void getGuardianInfo(const vector<Guardian*>& guardianList, int index) 
{
    if (index >= 0 && index < guardianList.size()) 
    {
        const Guardian* guardianSelected = guardianList[index];
        showGuardianInfo(*guardianSelected);
    }
    else 
    {
        cout << ">> Invalid index." << endl;
    }
}

void showGuardianInfo(const Guardian& guardian)
{
    cout << ">> Guardian Details:" << endl;
    cout << "Name: " << guardian.name << endl;
    cout << "Level: " << guardian.level << endl;
    string rankString = getRankString(guardian.rank);
    cout << "Rank: " << rankString << endl;
    cout << "Master: " << guardian.master << endl;
    cout << "City: " << guardian.city << endl;
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

string getRankString(char rank)
{
    string rankString;
    if (rank == 'M') { rankString = "Grand Master"; }
    else if (rank == 'G') { rankString = "Guardian of the Kingdom"; }
    else if (rank == 'C') { rankString = "Candidate"; }
    else { rankString = "Unknown"; }
    return rankString;
}