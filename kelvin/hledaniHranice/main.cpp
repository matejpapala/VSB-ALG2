#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <algorithm>

std::vector<std::vector<int>> combinationsOfSize(const std::vector<int>& elems, int k) {
    std::vector<std::vector<int>> res;
    int n = (int)elems.size();
    if (k > n || k <= 0) return res;

    std::vector<int> idx(k);
    for (int i = 0; i < k; ++i) idx[i] = i;

    while (true) {
        std::vector<int> c;
        c.reserve(k);
        for (int i = 0; i < k; ++i) c.push_back(elems[idx[i]]);
        res.push_back(c);

        int pos = k - 1;
        while (pos >= 0 && idx[pos] == n - k + pos) --pos;
        if (pos < 0) break;

        idx[pos] += 1;
        for (int j = pos + 1; j < k; ++j) idx[j] = idx[j - 1] + 1;
    }

    return res;
}

std::string simplexKey(const std::vector<int>& verts) {
    std::ostringstream oss;
    for (size_t i = 0; i < verts.size(); ++i) {
        if (i > 0) oss << ' ';
        oss << verts[i];
    }
    return oss.str();
}

std::vector<std::vector<int>> loadSimplices(const std::string& filename) {
    std::ifstream fin(filename);
    std::vector<std::vector<int>> simplices;
    std::string line;

    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::vector<int> verts;
        int v;
        while (iss >> v) verts.push_back(v);
        if (verts.empty()) continue;
        std::sort(verts.begin(), verts.end());
        verts.erase(std::unique(verts.begin(), verts.end()), verts.end());
        simplices.push_back(verts);
    }

    return simplices;
}

int main(int argc, char** argv) {
    std::vector<std::vector<int>> simplices = loadSimplices(argv[1]);

    std::map<int, std::set<std::string>> dimSets;
    std::map<std::string, int> faceCount;
    std::set<int> allVertices;

    for (const auto& simplex : simplices) {
        int dim = (int)simplex.size() - 1;
        std::string fullKey = simplexKey(simplex);
        dimSets[dim].insert(fullKey);

        for (int v : simplex) allVertices.insert(v);

        if (simplex.size() >= 2) {
            auto faces = combinationsOfSize(simplex, (int)simplex.size() - 1);
            for (auto& f : faces) {
                std::string fk = simplexKey(f);
                faceCount[fk] += 1;
            }
        }

        for (int r = 1; r < (int)simplex.size(); ++r) {
            auto subs = combinationsOfSize(simplex, r);
            for (auto& sub : subs) {
                int subDim = (int)sub.size() - 1;
                std::string subKey = simplexKey(sub);
                dimSets[subDim].insert(subKey);
            }
        }
    }

    for (int v : allVertices) {
        std::string k = std::to_string(v);
        dimSets[0].insert(k);
    }

    int maxDim = 0;
    for (auto& p : dimSets) if (p.first > maxDim) maxDim = p.first;

    std::vector<long long> k(maxDim + 1, 0);
    for (int d = 0; d <= maxDim; ++d) {
        auto it = dimSets.find(d);
        if (it != dimSets.end()) k[d] = (long long)it->second.size();
    }

    long long chi = 0;
    for (int d = 0; d <= maxDim; ++d) {
        if (d % 2 == 0) chi += k[d];
        else chi -= k[d];
    }

    std::vector<std::vector<int>> boundaryFaces;
    for (const auto& p : faceCount) {
        if (p.second == 1) {
            std::istringstream iss(p.first);
            std::vector<int> faceVerts;
            int vv;
            while (iss >> vv) faceVerts.push_back(vv);
            boundaryFaces.push_back(faceVerts);
        }
    }

    std::sort(boundaryFaces.begin(), boundaryFaces.end(),
              [](const std::vector<int>& A, const std::vector<int>& B){ return A < B; });

    if (k.size() > 0) std::cout << "Vertices: " << k[0] << "\n";
    if (k.size() > 1) std::cout << "Edges: " << k[1] << "\n";
    if (k.size() > 2) std::cout << "Triangles: " << k[2] << "\n";
    if (k.size() > 3) std::cout << "Tetrahedrons: " << k[3] << "\n";

    std::cout << "chi: " << chi << "\n\n";

    if (boundaryFaces.empty()) {
        std::cout << "Boundary:\nis empty\n";
    } else {
        for (const auto& f : boundaryFaces) {
    for (size_t i = 0; i < f.size(); ++i) {
        std::cout << f[i];
        if (i + 1 < f.size()) std::cout << ' ';
    }
    std::cout << "\n";
}

    }

    return 0;
}
