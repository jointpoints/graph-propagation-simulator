/**
 * \file
 *       metric_graph.hpp
 *
 * \author
 *       Andrei Eliseev (JointPoints), 2021
 */
#ifndef RWE__METRIC_GRAPH_HPP__
#define RWE__METRIC_GRAPH_HPP__





#include <vector>   // needed for "vector"
#include <deque>    // needed for "deque"
#include <cstdint>  // needed for "int*_t" and "uint*_t" types
#include <string>   // needed for "string"
#include <iostream> // needed for "iostream"





namespace rwe
{





	// Forward declaration of RWSpace class
	class RWSpace;





	/**
	 * \class MetricGraph
	 * \brief A metric graph class
	 * 
	 * <b>Metric graph</b> is defined as a graph \f$(V,E)\f$ where \f$V\f$ is a set of \b vertices,
	 * \f$E \subset V^2\f$ is a set of \b edges with each edge \f$e \in E\f$ being associated with
	 * a certain interval \f$[0,l(e)]\f$, where \f$l(e) > 0\f$ is called a \b length of the edge.
	 * Edge \f$(v,w) \in E\f$ is called \b directed ('from \f$v\f$ to \f$w\f$'), if \f$(w,v) \notin E\f$.
	 * Such edges will be marked \f$v \rightarrow w\f$ hereafter. Otherwise, if \f$(w,v) \in E\f$,
	 * the edge \f$(v,w) \in E\f$ is called \b undirected ('from \f$v\f$ to \f$w\f$', or 'between \f$v\f$
	 * and \f$w\f$'). Such edges will be marked \f$v-w\f$ hereafter. You may read more about metric
	 * graphs [here](https://en.wikipedia.org/wiki/Quantum_graph#Metric_graphs).
	 * 
	 * This implementation also binds general theory by following additional restrictions:
	 * * \f$V \subset \{0,1,2,...,2^{32}-1\}\f$ (i.e., vertices can only be represented as
	 * non-negative integers up to \f$2^{32}-1\f$);
	 * * \f$\{(v,w),(w,v)\} \subset E \Rightarrow l((v,w)) = l((w,v))\f$ (i.e., if one
	 * can go both ways, the distance between end points is the same).
	 */
	class MetricGraph
	{



	public:



		/// \name Constructors and destructors
		///@{
		
		/**
		 * Default constructor
		 * 
		 * Constructs an empty metric graph.
		 */
		MetricGraph     (void);

		/**
		 * Default destructor
		 * 
		 * Destroys the metric graph.
		 * 
		 * \note All \c RWSpace objects associated with the respective metric graph will be
		 * automatically transferred into the \c dead state.
		 */
		~MetricGraph    (void);

		// Prevent implicit creation of copy-constructor and copy-assignment
		MetricGraph     (MetricGraph &)                 = delete;
		MetricGraph &   operator =  (MetricGraph &)     = delete;

		///@}



		/// \name Operators
		///@{

		/**
		 * Move assignment
		 * 
		 * Moves one metric graph instead of another.
		 * 
		 * \param   other   A metric graph to move from.
		 * 
		 * \note All \c RWSpace objects associated with the old metric graph will remain
		 * associated (they will not be replaced), however, they will be automatically
		 * transferred into the \c invalid state. All \c RWSpace objects associated with
		 * the new metric graph will remain associated and their state will not be
		 * changed.
		 * 
		 * \note The behaviour of \c other is undefined after the assignment.
		 */
		MetricGraph &   operator =  (MetricGraph &&other);

		///@}



		/// \name Accessors
		///@{

		/**
		 * Checks whether vertex is present in the graph
		 * 
		 * This function helps to find out whether a vertex with specified ID is present in the
		 * corresponding graph or not.
		 * 
		 * \param   vertex  Vertex ID.
		 * 
		 * \return
		 * * \c true if vertex is found in graph;
		 * * \c false otherwise.
		 */
		bool const                      checkVertex     (uint32_t const vertex)                             const;

		/**
		 * Get vector of vertices IDs
		 * 
		 * This function helps to find out the IDs of the vertices in the corresponding graph.
		 * 
		 * \return \c std::vector of vertices IDs, i.e. \f$V\f$.
		 */
		std::vector<uint32_t> const     getVertexList   (void)                                              const;

		/**
		 * Gets length of the edge
		 * 
		 * This functions helps to find out the length of the edge connecting vertex \f$v\f$ with
		 * vertex \f$w\f$.
		 * 
		 * \param   out_vertex  ID of vertex \f$v\f$.
		 * \param   in_vertex   ID of vertex \f$w\f$.
		 * 
		 * \return
		 * * Length of the edge, if both \f$v \in V\f$ and \f$w \in V\f$, and either \f$v-w\f$ edge or
		 * \f$v \rightarrow w\f$ edge exists;
		 * * Positive infinity, otherwise.
		 */
		long double const               getEdgeLength   (uint32_t const out_vertex, uint32_t in_vertex)     const;

		/**
		 * Prints edge list into the stream
		 * 
		 * Sends human-readable list of graph edges into the specified output stream.
		 * 
		 * \param   output_stream   Target output stream.
		 * 
		 * \paragraph example Example
		 * Source snippet:
		 * \include{lineno} MetricGraph_outputEdgeList_example.txt
		 * Expected output:
		 * \include MetricGraph_outputEdgeList_output.txt
		 */
		void                            outputEdgeList  (std::ostream &output_stream)                       const;

		///@}



		/// \name Modifiers
		///@{
		
		/**
		 * Updates edge characteristics
		 * 
		 * This function adds a new edge or updates already existing one. The exact behaviour depends
		 * on the parameters and prior configuration of graph. Say, we want to add an edge of length \f$l\f$
		 * from vertex \f$v\f$ to vertex \f$w\f$. Possible cases are:
		 * <table>
		 * <tr align="center">
		 *     <th>Is \f$v \in V\f$?</th>
		 *     <th>Is \f$w \in V\f$?</th>
		 *     <th>Type of existing edge</th>
		 *     <th>Desired type of edge</th>
		 *     <th>Result</th>
		 * </tr>
		 * <tr align="center">
		 *     <td>No</td>
		 *     <td>Yes</td>
		 *     <td rowspan="3">None</td>
		 *     <td rowspan="3">Any</td>
		 *     <td rowspan="3" align="left">All non-existing vertices are added to the graph (i.e., \f$V = V \cup \{v,w\}\f$)
		 *         together with desired edge of length \f$l\f$.</td>
		 * </tr>
		 * <tr align="center">
		 *     <td>Yes</td>
		 *     <td>No</td>
		 * </tr>
		 * <tr align="center">
		 *     <td>No</td>
		 *     <td>No</td>
		 * </tr>
		 * <tr align="center">
		 *     <td rowspan="7">Yes</td>
		 *     <td rowspan="7">Yes</td>
		 *     <td>None</td>
		 *     <td>Any</td>
		 *     <td align="left">Desired edge from \f$v\f$ to \f$w\f$ of length \f$l\f$ is added to the graph.</td>
		 * </tr>
		 * <tr align="center">
		 *     <td rowspan="2">\f$v \rightarrow w\f$</td>
		 *     <td>\f$v \rightarrow w\f$</td>
		 *     <td align="left">The length of existing edge \f$v \rightarrow w\f$ is updated to be \f$l\f$.</td>
		 * </tr>
		 * <tr align="center">
		 *     <td>\f$v-w\f$</td>
		 *     <td align="left">Existing edge \f$v \rightarrow w\f$ is replaced with \f$v-w\f$ of length \f$l\f$.</td>
		 * </tr>
		 * <tr align="center">
		 *     <td rowspan="2">\f$v \leftarrow w\f$</td>
		 *     <td>\f$v \rightarrow w\f$</td>
		 *     <td align="left">Existing edge \f$v \leftarrow w\f$ is replaced with \f$v-w\f$ of length \f$l\f$.</td>
		 * </tr>
		 * <tr align="center">
		 *     <td>\f$v-w\f$</td>
		 *     <td align="left">Existing edge \f$v \leftarrow w\f$ is replaced with \f$v-w\f$ of length \f$l\f$.</td>
		 * </tr>
		 * <tr align="center">
		 *     <td rowspan="2">\f$v-w\f$</td>
		 *     <td>\f$v \rightarrow w\f$</td>
		 *     <td align="left">The length of existing edge \f$v-w\f$ is updated to be \f$l\f$.</td>
		 * </tr>
		 * <tr align="center">
		 *     <td>\f$v-w\f$</td>
		 *     <td align="left">The length of existing edge \f$v-w\f$ is updated to be \f$l\f$.</td>
		 * </tr>
		 * </table>
		 * 
		 * \param   out_vertex      ID of vertex \f$v\f$.
		 * \param   in_vertex       ID of vertex \f$w\f$.
		 * \param   length          Desired length of edge between \f$v\f$ and \f$w\f$ (i.e., \f$l\f$).
		 * \param   is_directed     If \c true, desired type of edge will be \f$v \rightarrow w\f$,
		 *                          if \c false, desired type of edge will be \f$v-w\f$.
		 * 
		 * \note All \c RWSpace objects associated with the respective metric graph will be
		 * automatically transferred into the \c invalid state.
		 * 
		 * \throw invalid_argument if desired length is non-positive.
		 * 
		 * \paragraph example Example
		 * Source snippet:
		 * \include{lineno} MetricGraph_updateEdge_example.txt
		 * Expected output:
		 * \include MetricGraph_updateEdge_output.txt
		 */
		void updateEdge(uint32_t const out_vertex, uint32_t const in_vertex, long double const length, bool const is_directed = false);
		///@}



		/// \name Save/load
		///@{

		/**
		 * Save graph to \c gexf file
		 * 
		 * This function saves respective metric graph to an XML-like file of \c gexf format with
		 * the name specified by user. If the last five symbols of target file name are not
		 * <tt>.gexf</tt>, they will be added automatically.
		 * 
		 * The lengths of edges will be stored in the \c weight attribute of corresponding edges.
		 * 
		 * \param   file_name   Name of a target \c gexf file.
		 * \param   rewrite     If \c false and a file with the specified name already exists, then
		 *                      it will not be rewritten but the ordinal number will be added to the
		 *                      name in parentheses (like <em>'My file name (1).gexf'</em>, <em>'My
		 *                      file name (2).gexf'</em> and so on).
		 */
		void    toGEXF      (std::string const file_name = "Saved files/My metric graph", bool const rewrite = false)   const;

		/**
		 * Load graph from \c gexf file
		 * 
		 * This function loads graph from the specified \c gexf file and merges it with the existing
		 * one by adding all absent vertices, edges and updating their lengths and directions. If a
		 * file with the specified name does not exist, the original data is left unchanged.
		 * 
		 * The following additional requirements need to be met:
		 * 
		 * * Version of the file must be <em>1.2draft</em>;
		 * * Length of each edge must be stored in the \c weight attribute;
		 * * IDs of vertices must meet the requirements mentioned at the top of this page.
		 * 
		 * Isolated vertices are omitted. Mutual edges are treated as undirected. If edge with the same
		 * \c source and \c target is repeated multiple times, it will be treated as it is described in
		 * a table for \ref updateEdge function in the respective order of repetitions.
		 * 
		 * \param   file_name   Name of a source \c gexf file.
		 */
		void    fromGEXF    (std::string const file_name);

		/**
		 * Save graph to \c rweg file
		 * 
		 * This function saves respective metric graph to a binary file of \c rweg format with
		 * the name specified by user. If the last five symbols of target file name are not
		 * <tt>.rweg</tt>, they will be added automatically.
		 * 
		 * \param   file_name   Name of a target \c rweg file.
		 * \param   rewrite     If \c false and a file with the specified name already exists, then
		 *                      it will not be rewritten but the ordinal number will be added to the
		 *                      name in parentheses (like <em>'My file name (1).rweg'</em>, <em>'My
		 *                      file name (2).rweg'</em> and so on).
		 */
		void    toRWEG      (std::string const file_name = "Saved files/My metric graph", bool const rewrite = false)   const;

		/**
		 * Load graph from \c rweg file
		 * 
		 * This function loads graph from the specified \c rweg file and merges it with the existing
		 * one by adding all absent vertices, edges and updating their lengths and directions. If a
		 * file with the specified name does not exist, the original data is left unchanged.
		 * 
		 * \param   file_name   Name of a source \c rweg file.
		 */
		void    fromRWEG    (std::string const file_name);

		///@}
	private:
		friend class RWSpace;

		using VertexList            = std::vector<uint32_t>;
		using LengthList            = std::vector<long double>;
		using DirectionList         = std::vector<bool>;
		using VertexView            = struct {uint32_t id; VertexList adjacents; LengthList lengths; DirectionList is_directed;};
		using EdgeList              = std::vector<VertexView>;
		using Edge                  = std::pair<uint32_t, uint32_t>;

		EdgeList                edges;
		std::vector<RWSpace *>   associated_wanders;

		// Access
		Edge                getEdge             (uint32_t const out_vertex, uint32_t const in_vertex, bool const is_directed = true, bool const strict_mode = false) const;
		std::deque<Edge>    getDepartingEdges   (uint32_t const out_vertex) const;
	};





} // rwe





#endif // RWE__METRIC_GRAPH_HPP__
