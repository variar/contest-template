#include <lib_template/dummy.h>

#include <boost/any.hpp>

#include <plog/Log.h>

#include <seqan/graph_algorithms.h>

#include <mpir.h>
#include <boost/multiprecision/gmp.hpp>

namespace boost_mp = boost::multiprecision;
using rational = boost_mp::number<boost_mp::backends::gmp_int,
                                  boost_mp::expression_template_option::et_off>;


using namespace seqan;

using TGraph = Graph<Directed<>>;
using TVertexDescriptor = VertexDescriptor<TGraph>::Type;
using TEdgeIterator = Iterator<TGraph, EdgeIterator>::Type;
using TSize = Size<TGraph>::Type;

Dummy::Dummy() 
{
    LOG_INFO << "Dummy lib constructed";

    // Create graph with 9 directed edges (0,1), (0,2)
    TSize numEdges = 9;
    TVertexDescriptor edges[] = {0, 1, 0, 2, 0, 4, 1, 3, 1, 4, 2, 1, 3, 0, 3, 2, 4, 3};
    TGraph g;
    addEdges(g, edges, numEdges);
    // Print graph.
    LOG_INFO << g;

    // Fill external property map with edge weights and assign to graph.
    rational weights[] = {3, 8, -4, 1, 7, 4, 2, -5, 6};
    String<rational > weightMap;
    assignEdgeMap(weightMap, g, weights);

    // Run Floyd-Warshall algorithm.
    String<rational > distMat;
    String<TVertexDescriptor> predMat;
    floydWarshallAlgorithm(distMat, predMat, g, weightMap);

    // Print result to stdout.
    unsigned int len = static_cast<unsigned>(std::sqrt(static_cast<double >(length(distMat))));
    for (TSize row = 0; row < len; ++row)
        for (TSize col = 0; col < len; ++col)
        {
            LOG_INFO << row << "," << col << " (Distance="
                      << getValue(distMat, row * len + col) << "): ";
        }


}
