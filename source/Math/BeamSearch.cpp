#include "Math/BeamSearch.hpp"


namespace BeamSearch {
    

std::vector<std::shared_ptr<PartialSolution>> beam_search(PartialSolution const &start, size_t width)
{
    std::vector<std::shared_ptr<PartialSolution>> candidates;
    std::vector<std::shared_ptr<PartialSolution>> newCandidates;
    candidates.reserve(width);
    newCandidates.reserve(width);

    candidates.push_back(start.clone());

    while (true) {
        for (auto& candidate : candidates) {
            auto expansions = candidate->expand();
            if (!expansions.empty()) {
                newCandidates.insert(newCandidates.end(),
                                     std::make_move_iterator(expansions.begin()),
                                     std::make_move_iterator(expansions.end()));
            }
        }

        if (newCandidates.empty()) break;

        std::sort(newCandidates.begin(), newCandidates.end(),
                  [](const std::shared_ptr<PartialSolution>& a, const std::shared_ptr<PartialSolution>& b) {
                      return a->evaluate() < b->evaluate();
                  });

        if (newCandidates.size() > width) {
            newCandidates.resize(width);
        }

        candidates = std::move(newCandidates);
        newCandidates.clear();
    }

    return candidates;
}

}