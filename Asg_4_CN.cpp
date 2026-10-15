
#include <iostream>
#include <string>
#include <climits>
using namespace std;

const int N = 4;
const int INF = 999;

int main() {
    int cost[N][N], dist[N][N], nextHop[N][N];

    cout << "Enter the cost matrix (4x4):\n";
    cout << "Enter INF for no direct connection:\n";

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            string s;
            cin >> s;
            cost[i][j] = (s == "INF" || s == "inf") ? INF : stoi(s);
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = cost[i][j];
            if (i == j)
                nextHop[i][j] = i;
            else if (cost[i][j] != INF)
                nextHop[i][j] = j;
            else
                nextHop[i][j] = -1;
        }
    }

    // Distance Vector Routing
    bool updated;
    do {
        updated = false;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    if (dist[i][k] != INF && dist[k][j] != INF &&
                        dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        nextHop[i][j] = nextHop[i][k];
                        updated = true;
                    }
                }
            }
        }
    } while (updated);

    // Individual Routing Tables
    cout << "\n========== INDIVIDUAL ROUTING TABLES ==========\n";
    for (int i = 0; i < N; i++) {
        cout << "\nRouting Table for Router r" << i << ":\n";
        cout << "Destination\tCost\tNext Hop\n";
        for (int j = 0; j < N; j++) {
            cout << "r" << j << "\t\t";
            if (dist[i][j] >= INF)
                cout << "INF\t-\n";
            else
                cout << dist[i][j] << "\t" 
                     << (nextHop[i][j] == i ? "-" :
                         "r" + to_string(nextHop[i][j])) << "\n";
        }
    }

    // Consolidated Routing Table
    cout << "\n========== CONSOLIDATED ROUTING TABLE ==========\n";
    cout << "Source\tDestination\tCost\tNext Hop\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << "r" << i << "\tr" << j << "\t\t";
            if (dist[i][j] >= INF)
                cout << "INF\t-\n";
            else
                cout << dist[i][j] << "\t"
                     << (nextHop[i][j] == i ? "-" :
                         "r" + to_string(nextHop[i][j])) << "\n";
        }
    }

    return 0;
}
