#include "metric_graph/metric_graph.hpp"

#include <iostream>





int main(void)
{
	rand_walks::MetricGraph graph;
	std::cout << graph.getVertexCount() << '\n';

	return 0;
}
