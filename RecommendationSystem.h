#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H

#include <string>
#include <vector>
#include <set>

class RecommendationSystem {
public:
    RecommendationSystem();
    ~RecommendationSystem();
    std::vector<std::string> getRecommendations(const std::string& userId, const std::string& videoId);
    void updateHistory(const std::string& userId, const std::string& videoId);

private:
    std::vector<std::string> getRandomVideos(int count, const std::set<std::string>& exclude);
};

#endif
