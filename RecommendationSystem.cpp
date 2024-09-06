#include "RecommendationSystem.h"
#include <map>
#include <algorithm>
#include <random>
#include <iostream>

static std::map<std::string, std::vector<std::string>> userWatchHistory;
static std::map<std::string, int> videoPopularity;

RecommendationSystem::RecommendationSystem() {}
RecommendationSystem::~RecommendationSystem() {}

void RecommendationSystem::updateHistory(const std::string& userId, const std::string& videoId) {
    userWatchHistory[userId].push_back(videoId);
    videoPopularity[videoId]++;
}

std::vector<std::string> RecommendationSystem::getRecommendations(const std::string& userId, const std::string& videoId) {
    std::set<std::string> recommendedVideoIds;
    std::vector<std::pair<int, std::string>> scoredRecommendations;

    // Gather recommendations
    for (auto& entry : userWatchHistory) {
        if (entry.first != userId) {
            for (auto& vid : entry.second) {
                if (std::find(userWatchHistory[userId].begin(), userWatchHistory[userId].end(), vid) == userWatchHistory[userId].end()) {
                    recommendedVideoIds.insert(vid);
                }
            }
        }
    }

    // Score and sort recommendations
    for (auto& vid : recommendedVideoIds) {
        scoredRecommendations.emplace_back(videoPopularity[vid], vid);
    }
    std::sort(scoredRecommendations.rbegin(), scoredRecommendations.rend());

    // Prepare final list
    std::vector<std::string> finalRecommendations;
    for (auto& [score, vid] : scoredRecommendations) {
        finalRecommendations.push_back(vid);
        if (finalRecommendations.size() == 6) break;
    }

    // If not enough recommendations, add random videos
    if (finalRecommendations.size() < 6) {
        int needed = 6 - finalRecommendations.size();
        auto randomVideos = getRandomVideos(needed, recommendedVideoIds);
        finalRecommendations.insert(finalRecommendations.end(), randomVideos.begin(), randomVideos.end());
    }

    return finalRecommendations;
}

std::vector<std::string> RecommendationSystem::getRandomVideos(int count, const std::set<std::string>& exclude) {
    std::vector<std::string> allVideos;
    for (auto& [videoId, _] : videoPopularity) {
        if (exclude.find(videoId) == exclude.end()) {
            allVideos.push_back(videoId);
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allVideos.begin(), allVideos.end(), g);

    return std::vector<std::string>(allVideos.begin(), allVideos.begin() + std::min(count, (int)allVideos.size()));
}
