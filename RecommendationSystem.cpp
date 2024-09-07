#include "RecommendationSystem.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

static std::map<std::string, std::vector<std::string>> userWatchHistory; // Maps userId to list of watched videoIds
static std::map<std::string, int> videoPopularity;                       // Maps videoId to the number of times it has been watched

RecommendationSystem::RecommendationSystem() {}

RecommendationSystem::~RecommendationSystem() {}

// Update user history and video popularity when a video is watched
void RecommendationSystem::updateHistory(const std::string &userId, const std::string &videoId)
{
    // Check if the user already exists in the watch history, if not, initialize
    if (userWatchHistory.find(userId) == userWatchHistory.end())
    {
        userWatchHistory[userId] = std::vector<std::string>();
    }

    // If the user hasn't already watched this video, add it to their history
    if (std::find(userWatchHistory[userId].begin(), userWatchHistory[userId].end(), videoId) == userWatchHistory[userId].end())
    {
        userWatchHistory[userId].push_back(videoId);
    }

    // Update the video's popularity (increment the watch count)
    videoPopularity[videoId]++;
}

// Get recommendations for a user based on shared video history with other users
std::vector<std::string> RecommendationSystem::getRecommendations(const std::string &userId, const std::string &videoId)
{
    std::map<std::string, int> candidateVideos; // Stores candidate videos and their popularity

    // Get the list of videos that the user has watched
    const std::vector<std::string> &userVideos = userWatchHistory[userId];

    // Compare the user's watch history with other users
    for (const auto &[otherUserId, otherUserVideos] : userWatchHistory)
    {
        if (otherUserId != userId)
        { // Skip comparing the user to themselves
            // Check if there are any shared videos between the user and other users
            bool hasSharedVideo = false;
            for (const auto &vid : otherUserVideos)
            {
                if (std::find(userVideos.begin(), userVideos.end(), vid) != userVideos.end())
                {
                    hasSharedVideo = true;
                    break;
                }
            }

            // If there are shared videos, collect the other user's videos (excluding the current video)
            if (hasSharedVideo)
            {
                for (const auto &vid : otherUserVideos)
                {
                    if (vid != videoId && std::find(userVideos.begin(), userVideos.end(), vid) == userVideos.end())
                    {
                        candidateVideos[vid] = videoPopularity[vid]; // Track the popularity of the candidate video
                    }
                }
            }
        }
    }

    // If no shared videos are found, return an empty vector
    if (candidateVideos.empty())
    {
        return {};
    }

    // Sort the candidate videos by their popularity (highest first)
    std::vector<std::pair<std::string, int>> sortedCandidates(candidateVideos.begin(), candidateVideos.end());
    std::sort(sortedCandidates.begin(), sortedCandidates.end(), [](const auto &a, const auto &b)
              {
                  return b.second < a.second; // Sort by view count in descending order
              });

    // Extract the video IDs from the sorted candidates
    std::vector<std::string> finalRecommendations;
    for (const auto &[vid, popularity] : sortedCandidates)
    {
        finalRecommendations.push_back(vid);
    }

    return finalRecommendations; // Return the final list of recommended videos
}
