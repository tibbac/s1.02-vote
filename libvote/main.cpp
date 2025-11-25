#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

bool estCandidat(const string& x, const vector<string>& candidats) {
    return find(candidats.begin(), candidats.end(), x) != candidats.end();
}

void creerListe(const vector<string>& candidats, map<string, int>& votesCandidats) {
    votesCandidats.clear();
    for (size_t i = 0; i < candidats.size(); ++i) {
        votesCandidats[candidats[i]] = 0;
    }
}

void ajouterVote(const vector<string>& vote, const vector<string>& candidats, map<string, int>& votesCandidats) {
    for (size_t i = 0; i < vote.size(); ++i) {
        if (estCandidat(vote[i], candidats)) {
            votesCandidats[vote[i]] += 1;
            break;
        }
    }
}

void attribuerVotes(const vector<vector<string>>& votes, const vector<string>& candidats, map<string, int>& votesCandidats) {
    for (size_t i = 0; i < votes.size(); ++i) {
        ajouterVote(votes[i], candidats, votesCandidats);
    }
}

void supprimerCandidat(const string& p, vector<string>& candidats) {
    candidats.erase(find(candidats.begin(), candidats.end(), p));
    /*cout << "liste des candidats restants" << endl;
    for (size_t i = 0; i < candidats.size(); ++i) {
        cout << candidats[i] << endl;
    }*/
}

bool verifierTour(const map<string, int>& votesCandidats, const vector<vector<string>>& votes, vector<string>& candidats) {
    int nbVotes = votes.size();
    int maxVotes = 0;
    int minVotes = votes.size();
    string meilleurCandidat;
    string pireCandidat;

    for (pair<string, int> p : votesCandidats) {
        if (p.second > maxVotes) {
            maxVotes = p.second;
            meilleurCandidat = p.first;
        }
        if (p.second < minVotes) {
            minVotes = p.second;
            pireCandidat = p.first;
        }
    }
    if (maxVotes / nbVotes > 0.5) {
        //cout << "il y a une majorite absolue, le gagnant est : " << meilleurCandidat << endl;
        return false;
    }
    /*cout << "Aucune majorite absolue " << endl;
    cout << "On refait un tour" << endl;
    cout << "on supprime donc : " << pireCandidat << endl;*/
    supprimerCandidat(pireCandidat, candidats);
    return true;
}

void demarrerTour(vector<string> candidats, vector<vector<string>> votes) {
    while (true) {
        map<string, int> votesCandidats;
        creerListe(candidats, votesCandidats);
        attribuerVotes(votes, candidats, votesCandidats);
        if (!verifierTour(votesCandidats, votes, candidats)) {
            break;
        }
        if (candidats.size() <= 1) {
            //cout << "Il ne reste qu'un candidat : " << candidats[0] << endl;
            cout << candidats[0] << endl;
            break;
        }
    }
}

void chargerOracle(const string& fichier, vector<string>& candidats, vector<vector<string>>& votes ) {
    ifstream in(fichier);

    string ligne;
    getline(in, ligne);

    istringstream iss(ligne);
    string nom;
    while (iss >> nom) {
        candidats.push_back(nom);
    }

    while (getline(in, ligne)) {
        istringstream iss2(ligne);
        vector<string> vote;
        while (iss2 >> nom) {
            vote.push_back(nom);
        }
        votes.push_back(vote);
    }
}

int main(int argc, char* argv[]) {
    string nomFichier = argv[1];

    vector<vector<string>> votes;
    vector<string> candidats;

    chargerOracle(nomFichier, candidats, votes);

    demarrerTour(candidats, votes);

    return 0;
}
