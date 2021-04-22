#include<iostream>
#include<vector>
#include<time.h>
#include<Windows.h>
#include <algorithm>

using namespace std;

vector<vector<int>> population;
vector<vector<int>> parent;
int n = 8;
int tot;
float max_value = (n - 1) * (1.0 * n / 2);

int min_data = max_value;
int max_data = 0;
int index_best;
int limit;

int getValue(vector<int> data) { // check berapa jumlah tabrakan dari 1 dna

    int count = 0;
    for (int i = 0; i < data.size() - 1; i++) {
        for (int j = i + 1; j < data.size(); j++) {
            if (data[i] == data[j] || abs(data[j] - data[i]) == abs(j - i)) {//kolom, baris||diagonal
                count++;
            }
        }
    }
    return max_value - count;
}

void generateRandom(int random_sample) {
    tot = 0;

    vector<int> data;
    for (int i = 0; i < random_sample; i++) {
        for (int j = 0; j < n; j++) {
            data.push_back(rand() % n);
        }
        int val = getValue(data);
        if (val < min_data) {
            min_data = val;
        }
        else if (val > max_data) {
            max_data = val;
        }
        tot += val;
        data.push_back(val);
        population.push_back(data);
        data.clear();
    }
}

void eliminate() {
    cout << (min_data + 3.0 * (max_data - min_data) / 4) / max_value * 100 << endl;//cek presentase ngapuse, trus nti sg nd bawah ini dihapus
    for (int i = 0; i < population.size(); i++) {
        if (population[i][n] < min_data + 3.0 * (max_data - min_data) / 4 && max_data != min_data) {
            population.erase(population.begin() + i);
            i--;
        }
    }

    if (population.size() == 1) {
        generateRandom(30);
    }

    /*sort(population.begin(), population.end());
    population.erase(unique(population.begin(), population.end()), population.end());*/

}



void printAll() {
    for (int i = 0; i < population.size(); i++) {
        cout << "data" << i + 1 << "\t";
        for (int j = 0; j < population[i].size() - 1; j++) {
            cout << population[i][j] << " ";
        }cout << "Value : " << population[i][n] << " Percentage : " << population[i][n] * 1.0 / max_value * 100 << " Fitness Value : " << (population[i][n] * 1.0 / tot) * 100 << "%" << endl;
    }
}



void getParent() {
    cout << "Parent" << endl;
    parent.clear();
    int* check = new int[population.size()];
    bool take;
    int limit;

    if (population.size() / 4 <= 2) {//klo lebih kecil sama dengan 2 , diambil semua jadi parent
        limit = population.size();
    }
    else if (population.size() / 4 >= 40) {
        limit = 40;
    }
    else {
        limit = population.size() / 4;
    }

    for (int i = 0; i < limit; i++) {
        take = false;

        do {
            int index = rand() % population.size();
            if (check[index] != 1) {
                parent.push_back(population[index]);
                check[index] = 1;
                take = true;
            }
        } while (!take);
    }
}

void crossOver(vector<int> parent1, vector<int> parent2) {
    vector<int> child1 = parent1;
    vector<int> child2 = parent2;

    for (int i = rand() % n; i < parent1.size() - 1; i++) {
        child2[i] = parent1[i];
        child1[i] = parent2[i];
    }

    population.push_back(child1);
    population.push_back(child2);

}

void showParent() {
    for (int i = 0; i < parent.size(); i++) {
        cout << "Parent : ";
        for (int j = 0; j < n; j++) {
            cout << parent[i][j] << " ";
        }cout << "Value : " << parent[i][n] << " Percentage : " << parent[i][n] * 1.0 / max_value * 100 << " Fitness Value : " << (parent[i][n] * 1.0 / tot) * 100 << "%" << endl;

    }
}

void reProduce() {
    //showParent();
    cout << "reProduce" << endl;
    for (int i = 0; i < parent.size() - 1; i++) {
        for (int j = i + 1; j < parent.size(); j++) {
            crossOver(parent[i], parent[j]);
        }
    }
}

void mutate_cromosome(vector<int>& data) {
    int index = rand() % n;

    data[index] = rand() % n;
}

void mutate(int prob) {
    //cuma buat ngisi value regenerate
    tot = 0;
    for (int i = 0; i < population.size(); i++) {

        int val = getValue(population[i]);
        if (val < min_data) {
            min_data = val;
        }
        else if (val > max_data) {
            max_data = val;
        }
        population[i][n] = (val);
        tot += val;

    }

    //mulai mutasi
    tot = 0;
    for (int i = 0; i < population.size(); i++) {
        int chance = rand() % 100;
        if (chance < prob) {
            //cout << "Mutate" << endl;
            if (population[i][n] >= max_data) {
                vector<int> data = population[i];//kek pindah ke temp
                mutate_cromosome(data);//hasil clone di mutate
                population.push_back(data);//masukin
                //kenapa dimasukin ke temp dulu?, biar data yg aslie bagus jangan sampe kebuang
            }
            else {
                mutate_cromosome(population[i]);
            }

        }
        int val = getValue(population[i]);

        if (val < min_data) {
            min_data = val;
        }
        else if (val > max_data) {
            max_data = val;
        }
        population[i][n] = (val);
        tot += val;

    }

}

void print(vector<int> data) {
    cout << "Best : ";
    for (int i = 0; i < n; i++) {
        cout << data[i] << " ";
    }cout << "Value : " << data[n] << " Percentage : " << data[n] * 1.0 / max_value * 100 << " Fitness Value : " << (data[n] * 1.0 / tot) * 100 << "%" << endl;

}

int showBest() {
    index_best = 0;
    cout << "Max : " << max_data << endl;
    for (int i = 0; i < population.size(); i++) {
        if (population[i][n] == max_data) {
            print(population[i]);
            index_best = i;
        }
    }

    min_data = max_value;
    max_data = 0;

    for (int i = 0; i < n; i++) { //gambar board
        for (int j = 0; j < n; j++) {
            if (population[index_best][i] == j) {
                cout << " R ";
            }
            else {
                cout << " - ";
            }
        }
        cout << endl;
    }

    return population[index_best][n] * 1.0 / max_value * 100;
}

void main() {
    srand(time(NULL));
    int random_sample;
    cin >> random_sample;
    limit = 85;

    bool generate = true;

    generateRandom(random_sample);
    //printAll();
    eliminate();
    //printAll();

    if (showBest() < 100) {
        do {

            system("CLS");
            if (generate) {
                getParent();
                reProduce();
                mutate(2);
                //printAll();
                eliminate();
                //printAll();
                int val = showBest();
                if (val == 100) {
                    generate = false;
                }
                else if (val > limit) {
                    cout << "Generate ? 1: yes; 0: no;"; cin >> generate;
                    limit = population[index_best][n] * 1.0 / max_value * 100;

                }
            }

        } while (generate);
    }




}