#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
bool validGame = true;

struct Guardian
{
    string name;
    int level = 0;
    string city;
    int cityID = 0;
    string master;
};

struct City
{
    string name;
};

struct rankNode
{
    Guardian guardian;
    rankNode* left;
    rankNode* right;

    rankNode(const Guardian& guardian) : guardian(guardian), left(nullptr), right(nullptr) {}
};

rankNode* insertRankNode(rankNode* root, const Guardian& guardian);
void printRankTree(rankNode* root);
void freeRankNode(rankNode* root);

int getCityID(string city);

vector<Guardian> readGuardianFile(const string& fileName) 
{
    int level100 = 0, level90 = 0;
    vector<Guardian> guardianList;
    ifstream file(fileName);

    if (!file.is_open()) 
    {
        cerr << ">> Error opening file." << fileName << "\n" << endl;
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

        guardianList.push_back(guardian);

        if (guardian.level == 100) { level100++; }
        if (guardian.level >= 90 && guardian.level <= 99) { level90++; }
        if (guardian.level < 0 || guardian.level > 100) { validGame = false; }
    }

    if (level100 > 1 || level90 > 3) { validGame = false; }

    file.close();
    return guardianList;
}

int main()
{
    int option = 0, guardianCount = 0;

    string guardianFile = "guardians.conf";
    vector<Guardian> guardianList = readGuardianFile(guardianFile);

    rankNode* rankRoot = nullptr;

    // CREACIÓN DEL ÁRBOL BINARIO (RANKING) A PARTIR DE LA LISTA DE GUARDIANES CARGADOS DESDE EL ARCHIVO
    for (const auto& guardian : guardianList) { rankRoot = insertRankNode(rankRoot, guardian); }

    if (validGame == true)
    {
        do
        {
            cout << "   THE GUARDIANS BATTLE" << endl;
            cout << "\n1. View Candidates List.\n2. View Guardian.\n3. Know the Kingdom.\n4. Watch a Battle.\n5. Exit.\n\n>> Your option: ";
            cin >> option;
            switch (option)
            {
            case 1:
                system("cls");

                cout << ">> Guardians Ranking: \n" << endl;
                printRankTree(rankRoot);
                cout << "\n";

                system("pause");
                system("cls");
                break;
            case 2:
                system("cls");

                

                system("pause");
                system("cls");
                break;
            case 3:
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
}

rankNode* insertRankNode(rankNode* root, const Guardian& guardian) 
{
    if (root == nullptr) 
    {
        return new rankNode(guardian);
    }

    if (guardian.level < root->guardian.level) 
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
        printRankTree(root->right);
        if (root->guardian.level >= 90 && root->guardian.level <= 99)
        {
            cout << " " << root->guardian.name << " - Level: " << root->guardian.level << " - CANDIDATE" << endl;
        }
        else if (root->guardian.level == 100)
        {
            cout << " " << root->guardian.name << " - Level: " << root->guardian.level << " - GUARDIAN OF THE KINGDOM" << endl;
        }
        else
        {
            cout << " " << root->guardian.name << " - Level: " << root->guardian.level << endl;
        }
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