#ifndef _BEAM_SEARCH_H
#define _BEAM_SEARCH_H

#include <vector>
#include <algorithm>
#include <memory>

namespace BeamSearch
{

    class PartialSolution
    {

    public:
        // How do we make further solutions.
        // If we can not expand, return an empty vector.
        virtual std::vector<std::shared_ptr<PartialSolution>> expand() const = 0;

        // How do we evaluate the quality of this solution (heuristically)
        virtual double evaluate() const = 0;

        // Virtual destructor required.
        virtual ~PartialSolution() = default;

        // We require a clone.
        virtual std::shared_ptr<PartialSolution> clone() const = 0;
    };

    std::vector<std::shared_ptr<PartialSolution>> beam_search(PartialSolution const &start, size_t width);
} // Namespace BeamSearch

#endif