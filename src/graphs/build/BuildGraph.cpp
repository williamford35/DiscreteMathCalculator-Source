#include <algorithm>
/* This source code is under MIT license. You can find the
   complete license file in the root of the application directory. */

#include <wx/msgdlg.h>
#include <wx/colour.h>

#include "common/randomc.h"

#include "DiscreteMathCalculatorMain.h"
#include "graphs/build/BuildGraph.h"
#include "graphs/graphFileFormatException.h"
#include "graphs/build/GraphEdgeStyles.h"

#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/FastHierarchyLayout.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/basic/simple_graph_alg.h>
#include <ogdf/basic/Thread.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <atomic>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/planarity/PlanRep.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/basic/Queue.h>
#include <ogdf/basic/geometry.h>
#include <ogdf/basic/simple_graph_alg.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/tree/TreeLayout.h>
#include <stdio.h>

#include <ogdf/basic/Logger.h>

#ifdef _WIN64
#include <windows.h>
#include <tchar.h>
#endif

#define BUFSIZE 1024

#include <string.h>
#include <math.h>
#include "common/Utils.h"

#include <lemon/hypercube_graph.h>
#include "graphs/build/HypercubeVisitor.h"
#include <lemon/grid_graph.h>
#include <lemon/bfs.h>
#include <lemon/connectivity.h>

#include "graphs/algorithms/DigraphAlgorithmCanvas.h"
#include "graphs/algorithms/GraphAlgorithmCanvas.h"
#include "graphs/enter/EnterGraphDialog.h"

#include <string>
using namespace std;

BuildGraph::BuildGraph (const wxString& title, GraphType type, GraphLayout layout,
                        bool forceFFD700, bool parallelEdges, wxWindow *parent) :
	graphFigureTitle (title), graphType (type), graphLayout (layout), requireFFD700(forceFFD700),
	vertexFont (12, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false),
	allowParallelEdges (parallelEdges), drawingCompleteGraph (false)
{
	init();
	drawMyGraph = NULL;
}

BuildGraph::~BuildGraph()
{
	delete drawMyGraph;
}

void BuildGraph::init()
{
	GA.init (G,
	         GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
	         GraphAttributes::nodeLabel | GraphAttributes::nodeStyle | GraphAttributes::nodeType |
	         GraphAttributes::edgeStyle | GraphAttributes::edgeLabel |
	         GraphAttributes::nodeTemplate |
	         GraphAttributes::edgeArrow | GraphAttributes::edgeIntWeight | GraphAttributes::edgeType |
	         GraphAttributes::edgeDoubleWeight);

	if (graphFigureTitle.length() != 0)
	{
		graphLabel = G.newNode();
		GA.shape (graphLabel) = Shape::Ellipse;
		GA.width (graphLabel) = 35.0;
		GA.height (graphLabel) = 35.0;
		GA.strokeWidth (graphLabel) = 0.0;
		GA.strokeType (graphLabel) = ogdf::StrokeType::None;
		GA.strokeColor (graphLabel) = ogdf::Color::Name::White;
		GA.fillPattern (graphLabel) = ogdf::FillPattern::None;
		GA.label (graphLabel) = string ( (const char *) graphFigureTitle.c_str() );
		// GA.x(graphLabel) = 20;
		// GA.y(graphLabel) = 25;
	}
}

void BuildGraph::hierarchicalLayout (bool arrangeCCs)
{
	SugiyamaLayout SL;

	SL.setRanking (new OptimalRanking);
	SL.setCrossMin (new MedianHeuristic);

	if (!arrangeCCs)
		SL.arrangeCCs (false);

	FastHierarchyLayout *fhl = new FastHierarchyLayout;
	OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;

	if ( (graphType == undir) || (graphType == digraph) )
	{
		if (drawingCompleteGraph)
		{
			ohl->layerDistance (25.0);
			ohl->nodeDistance (12.0);
			ohl->weightBalancing (0.8);
		}
		else
		{
			ohl->layerDistance (20.0);
			ohl->nodeDistance (10.0);
			ohl->weightBalancing (0.8);
			SL.setLayout (ohl);
		}
	}
	else
	{
		if (drawingCompleteGraph)
		{
			fhl->layerDistance (35.0);
			fhl->nodeDistance (30.0);
		}
		else
		{
			fhl->layerDistance (25.0);
			fhl->nodeDistance (15.0);
		}

		SL.setLayout (fhl);
	}

	SL.call (GA);
}

void BuildGraph::forceDirectedLayout (int unitEdgeLen)
{
	FMMMLayout fmmm;

	fmmm.useHighLevelOptions (true);
	fmmm.unitEdgeLength (unitEdgeLen);
	fmmm.newInitialPlacement (false);
	fmmm.pageFormat (FMMMOptions::PageFormatType::Landscape);
	fmmm.qualityVersusSpeed (FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
	fmmm.call (GA);
	graphLayout = forcedirected;
}

bool BuildGraph::orthogonalLayout()
{
	PlanarizationLayout pl;
	SubgraphPlanarizer crossMin;

	if (checkForSelfLoops() )
	{
		wxString message, caption;

		message.Printf (wxT ("Self-loops are not allowed in orthogonal layout.") );
		caption.append (wxT ("Note") );

		wxMessageDialog dialog (NULL, message, caption, wxOK | wxICON_INFORMATION );
		dialog.ShowModal();

		return false;
	}

	auto *ps = new PlanarSubgraphFast<int>;
	ps->runs (100);
	VariableEmbeddingInserter *ves = new VariableEmbeddingInserter;
	ves->removeReinsert (RemoveReinsertType::All);

	crossMin.setSubgraph (ps);
	crossMin.setInserter (ves);

	/*EmbedderMinDepthMaxFaceLayers *emb = new EmbedderMinDepthMaxFaceLayers;
	pl.setEmbedder(emb);*/

	OrthoLayout *ol = new OrthoLayout;
	ol->separation (35.0);
	ol->cOverhang (0.1);
	// ol->setOptions(2+4);
	pl.setPlanarLayouter (ol);

	pl.call (GA);
	GA.removeUnnecessaryBendsHV();

	return true;
}

void BuildGraph::circularLayout()
{

	CircularLayout CL;

	if (drawingCompleteGraph)
	{
		CL.minDistCircle (200);
		CL.minDistSibling (550.0);
		CL.minDistLevel (500);
		CL.minDistCC (100);
	}
	else
	{
		CL.minDistCircle (35);
		CL.minDistSibling (75.0);
		CL.minDistLevel (75);
		CL.minDistCC (100);
	}

	CL.call (GA);
}

void BuildGraph::insertEdge (const wxString& src, const wxString& dest,
                             const wxString& strWeight,
                             const wxPen& edgePen,
                             const wxColour& srcColor,
                             const wxColour& destColor,
                             bool addSpecialArrow)
{
	map<wxString, node>::iterator iter;
	node srcVertex = NULL, destVertex = NULL;
	ogdf::edge e;
	int w;
	string edgeText;
	string srcLabel = string ( (const char *) src.c_str() ), destLabel = string ( (const char *) dest.c_str() );
	long edgeWeight;

	/*if (src == dest)
	   return;  // no self-loops*/

	if (findEdge (src,dest) != NULL && !allowParallelEdges)
		return;

	iter = str2node.find (src);

	if (iter != str2node.end() )
		srcVertex = iter->second;
	else
	{
		srcVertex = G.newNode();
		str2node[src] = srcVertex;
		GA.strokeWidth (srcVertex) = 1.0;
		w = getStringExtent (src, vertexFont);

		//GA.width(srcVertex) = 25.0;
		if (w > 35.0)
			GA.width (srcVertex) = w + 4;
		else
			GA.width (srcVertex) = 35.0;

		GA.height (srcVertex) = 35.0;
		GA.shape (srcVertex) = Shape::Rect;
		wxString s = srcColor.GetAsString (wxC2S_HTML_SYNTAX);
		if (requireFFD700)
			s = "FFD700";
		if ((s != "FFD700"))
			GA.fillColor (srcVertex) = (const char *) srcColor.GetAsString (wxC2S_HTML_SYNTAX).c_str();
		else
			GA.fillColor(srcVertex) = ogdf::Color(255,215,0);
	}

	if (destLabel[0] != '#')
	{
		iter = str2node.find (dest);

		if (iter != str2node.end() )
			destVertex = iter->second;
		else
		{
			destVertex = G.newNode();
			str2node[dest] = destVertex;
			GA.strokeWidth (destVertex) = 1.0;
			w = getStringExtent (dest, vertexFont);

			//GA.width(destVertex) = 25.0;
			if (w > 35.0)
				GA.width (destVertex) = w + 4;
			else
				GA.width (destVertex) = 35.0;

			GA.height (destVertex) = 35.0;
			GA.shape (destVertex) = Shape::Rect;
			wxString s = destColor.GetAsString (wxC2S_HTML_SYNTAX);

			if (requireFFD700)
				s = "FFD700";
			if ((s != "FFD700"))
				GA.fillColor (destVertex) = (const char *) destColor.GetAsString (wxC2S_HTML_SYNTAX).c_str();
			else
				GA.fillColor(destVertex) = ogdf::Color(255,215,0);

			const char * c2 = (const char *) srcColor.GetAsString (wxC2S_HTML_SYNTAX).c_str();
		}
	}
	else
	{
		wxString s = srcColor.GetAsString (wxC2S_HTML_SYNTAX);

		if ((s != "FFD700"))
			GA.fillColor (srcVertex) = (const char *) srcColor.GetAsString (wxC2S_HTML_SYNTAX).c_str();
		else
			GA.fillColor(srcVertex) = ogdf::Color(255,215,0);

		const char * c1 = (const char *) srcColor.GetAsString (wxC2S_HTML_SYNTAX).c_str();
	}

	if (srcLabel[0] != '~')
		GA.label (srcVertex) = srcLabel;

	if (destLabel[0] != '~' && destLabel[0] != '#')
		GA.label (destVertex) = destLabel;

	if (destLabel[0] != '#')
	{
		e = G.newEdge (srcVertex, destVertex);
		GA.strokeColor(e) = ogdf::Color(edgePen.GetColour().Red(),
		                                edgePen.GetColour().Green(),
		                                edgePen.GetColour().Blue());
		GA.strokeWidth(e) = edgePen.GetWidth();

		if (graphType == digraph || graphType == digraphw || addSpecialArrow)
			GA.arrowType (e) = ogdf::EdgeArrow::Last;
		else
			GA.arrowType (e) = ogdf::EdgeArrow::None;

		if (edgePen == wxNORMAL_PEN)
			GA.strokeType(e) = ogdf::StrokeType::Solid;
		else if (edgePen == wxTREE_EDGE_PEN)
			GA.strokeType(e) = ogdf::StrokeType::Solid;
		else if (edgePen == wxBACK_EDGE_PEN)
			GA.strokeType (e) = ogdf::StrokeType::Dot;
		else if (edgePen == wxCROSS_EDGE_PEN)
			GA.strokeType(e) = ogdf::StrokeType::Dash;
		else if (edgePen == wxFORWARD_EDGE_PEN)
			GA.strokeType(e) = ogdf::StrokeType::Dashdot;
		else if (edgePen == wxwxDASHED_GRAPH_EDGE_PEN)
			GA.strokeType(e) = ogdf::StrokeType::Dot;
		else if (edgePen == wxSHORTEST_PATH_PEN)
			GA.strokeType(e) = ogdf::StrokeType::Solid;
		else if (edgePen == wxHIDE_PEN)
			GA.strokeType (e) = ogdf::StrokeType::None;
		else
			GA.strokeType(e) = ogdf::StrokeType::Solid;

		if (strWeight.length() != 0)
		{
			GA.label (e) = (const char *) strWeight.c_str();
			strWeight.ToLong (&edgeWeight);
			GA.intWeight (e) = (int) edgeWeight;
		}
	}
}

ogdf::Graph *BuildGraph::getGraph()
{
	return &G;
}

ogdf::GraphAttributes *BuildGraph::getGraphAttributes()
{
	return &GA;
}

void BuildGraph::buildFromGML(const wxString& gfilename, GraphType *gtype,
                              bool *parallelEdges, bool hideEdges, bool standardVertexColor)
{
	wxString typeOfGraph;
	std::ifstream gmlIn (string ( (const char *) gfilename.c_str() ) );
	int poundPos;

	GraphIO::logger.localLogLevel(Logger::Level::Minor);
	Logger::globalLogLevel(Logger::Level::Minor);

	if (!GraphIO::readGML (GA, G, gmlIn) )
		throw graphFileFormatException ("Syntax error reading the GML file");

	Logger::slout(Logger::Level::Minor) << endl;

	wxTextFile gmlFile;

	gmlFile.Open (gfilename);
	typeOfGraph = gmlFile.GetLastLine();
	gmlFile.Close();

	poundPos = typeOfGraph.Find (L'#');
	typeOfGraph = typeOfGraph (poundPos+2, poundPos+9);
	typeOfGraph = typeOfGraph.BeforeFirst (L' ');

	if (typeOfGraph.IsSameAs (wxT ("undir") ) )
		graphType = undir;
	else if (typeOfGraph.IsSameAs (wxT ("undirw") ) )
		graphType = undirw;
	else if (typeOfGraph.IsSameAs (wxT ("digraph") ) )
		graphType = digraph;
	else if (typeOfGraph.IsSameAs (wxT ("digraphw") ) )
		graphType = digraphw;
	else
		throw graphFileFormatException ("The format of the GML file is incorrect");

	*gtype = graphType;

	setLayout (graphLayout);
	*parallelEdges = allowParallelEdges = hasParallelEdges();

	/*for(node v : G.nodes)
		GA.fillColor (v) = (const char *) wxColour (255, 215, 0, 0).GetAsString (wxC2S_HTML_SYNTAX).c_str();*/
	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(),
	                             drawingCanvas, graphType, graphLayout);

	if (hideEdges && (graphType == undirw) )
		drawMyGraph->draw (hideEdges);
	else if (standardVertexColor)
		drawMyGraph->draw (false, true);
	else
		drawMyGraph->draw(false);
}

bool BuildGraph::buildRelationFromGML (const wxString& gfilename, bool isHasseDiagram)
{
	wxString typeOfGraph;
	int poundPos;
	std::ifstream gmlIn (string ( (const char *) gfilename.c_str() ) );

	if (!GraphIO::readGML (GA, G, gmlIn) )
		throw graphFileFormatException ("Failure reading graph GML file");

	wxTextFile gmlFile;

	gmlFile.Open (gfilename);
	poundPos = typeOfGraph.Find (L'#');
	typeOfGraph = typeOfGraph (poundPos+2, poundPos+9);
	typeOfGraph = typeOfGraph.BeforeFirst (L' ');

	if (isHasseDiagram)
	{
		if ( (typeOfGraph.IsSameAs (wxT ("digraph") ) ) || (typeOfGraph.IsSameAs (wxT ("undirw") ) )
		      || (typeOfGraph.IsSameAs (wxT ("digraphw") ) ) )
			return false;
	}
	else
	{
		if ( (typeOfGraph.IsSameAs (wxT ("undir") ) ) || (typeOfGraph.IsSameAs (wxT ("undirw") ) )
		      || (typeOfGraph.IsSameAs (wxT ("digraphw") ) ) )
			return false;
	}

	// setLayout (forcedirected);

	drawingCanvas->setGA (&GA);
	eraseCanvas();

	if (isHasseDiagram)
		drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(),
		                             drawingCanvas, undir, hierarchical);
	else
		drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(),
		                             drawingCanvas, digraph, forcedirected);

	drawMyGraph->draw (false);
	return true;
}

bool BuildGraph::hasParallelEdges()
{
	node t;
	List<node> nodeList;
	List<node>::iterator nodeIter;
	List<ogdf::edge> nodeEdges;
	List<ogdf::edge>:: iterator edgeIter;

	G.allNodes (nodeList);

	for (nodeIter = nodeList.begin(); nodeIter != nodeList.end(); nodeIter++)
	{
		t = *nodeIter;
		t->outEdges (nodeEdges);

		if (parallelEdges (nodeEdges) )
			return true;
	}

	return false;
}

bool BuildGraph::parallelEdges (const List<ogdf::edge>& edgeList)
{
	List<ogdf::edge>::const_iterator i,j,end;
	node target;

	end = edgeList.rbegin();

	for (i=edgeList.begin(); i != end; i++)
	{
		target = (**i).target();
		j = i;
		j++;

		for (; j != edgeList.end(); j++)
			if ( (**j).target() == target)
				return true;
	}

	return false;
}

void BuildGraph::buildFromGRPH (const wxString& gfilename, bool hideEdges, bool useStandardVertexColor)
{
	wxTextFile grphFile;
	//bool weighted;

	if (!grphFile.Open (gfilename) )
		throw graphFileFormatException ("Failure reading graph GRPH file");

	//weighted = (graphType == undirw || graphType == digraphw) ? true : false;
	buildGraphFromTextfile (&grphFile,hideEdges);
	grphFile.Close();

	setLayout (graphLayout);

	// saveGraph("C:\\graphtest.gml");

	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             graphType, graphLayout);
	drawMyGraph->draw (hideEdges, useStandardVertexColor);
}

void BuildGraph::buildFromTSP (const wxString& tspfilename, bool hideEdges)
{
	wxTextFile grphFile;
	//bool weighted;

	if (!grphFile.Open (tspfilename) )
		throw graphFileFormatException ("Failure reading TSP file");

	//weighted = (graphType == undirw || graphType == digraphw) ? true : false;
	buildGraphForTSP (&grphFile, hideEdges);
	grphFile.Close();

	graphLayout = hierarchical;
	setLayout (graphLayout);
	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             graphType, graphLayout);
	drawMyGraph->draw (hideEdges);
}


void BuildGraph::buildGraphFromTextfile (wxTextFile *grphFile, bool hideEdges)
{
	wxString grphFileLine, token;
	wxString srcVertex, destVertex, weight = wxT ("");
	int count;

	for (grphFileLine = grphFile->GetFirstLine(); !grphFile->Eof(); grphFileLine = grphFile->GetNextLine() )
	{
		wxStringTokenizer tkz (grphFileLine, wxT ("\t"), wxTOKEN_DEFAULT);
		count = 1;

		while (tkz.HasMoreTokens() )
		{
			token = tkz.GetNextToken();

			if (count == 1)
				srcVertex = token;
			else if (count == 2)
				destVertex = token;
			else
				weight = token;

			count++;
		}

		if (hideEdges)
			insertEdge (srcVertex, destVertex, weight, wxHIDE_PEN,
			            wxColour (255, 215, 0, 0),wxColour (255, 215, 0, 0));
		else
			insertEdge (srcVertex, destVertex,weight, wxNORMAL_PEN,
			            wxColour (255, 215, 0, 0),wxColour (255, 215, 0, 0),false);
	}
}

void BuildGraph::buildGraphForTSP (wxTextFile *grphFile, bool hideEdges)
{
	wxString grphFileLine;
	wxString v, edgew;
	vector<wxString> vertex;
	vector<double> edgeWeight;
	double weight;
	int n, expectedNumberOfEdges, i, j, k;
	bool haveVertices = false;

	// create a vector of vertices
	for (grphFileLine = grphFile->GetFirstLine(); !grphFile->Eof(); grphFileLine = grphFile->GetNextLine() )
	{
		wxStringTokenizer tkz (grphFileLine, wxT ("\t ") );

		while (tkz.HasMoreTokens() )
		{
			v = tkz.GetNextToken();

			if (v.IsSameAs (wxT ("--") ) )
			{
				haveVertices = true;
				break;
			}

			vertex.push_back (v);
		}

		if (haveVertices)
			break;
	}

	n = vertex.size();
	expectedNumberOfEdges = (n* (n - 1) ) / 2;

	// read the weights
	while (!grphFile->Eof() )
	{
		grphFileLine = grphFile->GetNextLine();
		wxStringTokenizer tkz (grphFileLine, wxT ("\t ") );

		while (tkz.HasMoreTokens() )
		{
			v = tkz.GetNextToken();

			if (v.ToDouble (&weight) )
				edgeWeight.push_back (weight);
			else
			{

			}
		}
	}

	if (edgeWeight.size() == expectedNumberOfEdges)
	{
		k = 0;

		for (i = 0; i < n; i++)
			for (j = i + 1; j < n; j++)
			{
				edgew = wxString::Format (wxT ("%.7f"), edgeWeight[k]);

				if (hideEdges)
					insertEdge (vertex[i], vertex[j], edgew, wxHIDE_PEN,
					            wxColour(255,215,0), wxColour(255,215,0));
				else
					insertEdge (vertex[i], vertex[j], edgew, wxNORMAL_PEN,
					            wxColour(255,215,0), wxColour(255,215,0));

				k++;
			}
	}
	else
	{

	}
}

void BuildGraph::setDrawingCanvas (DrawingCanvas *canvas)
{
	drawingCanvas = canvas;
}

void BuildGraph::eraseCanvas (bool eraseGraph)
{
	wxRect graphRect = drawingCanvas->GetTotalBoundingBox();
	ShapeList shapeList;

	drawingCanvas->GetShapesInside (graphRect, shapeList);

	if (shapeList.size() != 0)
	{
		(drawingCanvas->getSFDiagramManager() )->Clear();
		drawingCanvas->SetScale (1.0);
		// drawingCanvas->RefreshCanvas (true, drawingCanvas->GetRect());
	}

	if (eraseGraph)
		G.clear();
}

void BuildGraph::addVertexToEmptyGraph (const wxString& v)
{
	node v_node;
	int w;

	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(),
	                             drawingCanvas, graphType, graphLayout);
	v_node = G.newNode();
	GA.x (v_node) = 50;
	GA.y (v_node) = 50 ;
	GA.label (v_node) = (const char *) v.c_str();
	GA.strokeWidth (v_node) = 1.0;
	w = getStringExtent (v, vertexFont);

	if (w > 35.0)
		GA.width (v_node) = w + 4;
	else
		GA.width (v_node) = 35.0;

	GA.height (v_node) = 35.0;
	GA.shape (v_node) = Shape::Rect;
	GA.fillColor (v_node) = ogdf::Color(255,215,0);

	drawMyGraph->draw();
}

bool BuildGraph::addVertex (const wxString& v)
{
	node v_node = findVertex (v);
	int w;
	DRect boundingBox;
	DPoint p;

	if (v_node != NULL)
	{
		wxString message, caption;

		message.Printf (wxT ("Vertex %s is already in the graph."), v.c_str() );
		caption.append (wxT ("Note") );

		wxMessageDialog dialog (NULL, message, caption, wxOK | wxICON_INFORMATION);
		dialog.ShowModal();

		return false;
	}

	boundingBox = GA.boundingBox();
	boundingBox.normalize();
	p = boundingBox.p1();
	v_node = G.newNode();
	GA.x (v_node) = p.m_x + boundingBox.width() + drawMyGraph->getMaxVertexWidth() + 35;
	GA.y (v_node) = p.m_y + boundingBox.height() /2 ;
	GA.label (v_node) = (const char *) v.c_str();
	GA.fillColor (v_node) = ogdf::Color(255,215,0);
	GA.strokeWidth (v_node) = 1.0;
	w = getStringExtent (v, vertexFont);

	if (w > 35.0)
		GA.width (v_node) = w + 4;
	else
		GA.width (v_node) = 35.0;

	GA.height (v_node) = 35.0;
	GA.shape (v_node) = Shape::Rect;

	eraseCanvas();
	drawMyGraph->draw();

	return true;
}

bool BuildGraph::eraseVertex (const wxString& v)
{
	node v_node = findVertex (v);

	if (G.numberOfNodes() == 0)
		return false;

	if (v_node == NULL)
	{
		wxString message, caption;

		message.Printf (wxT ("Vertex %s is not in the graph."), v.c_str() );
		caption.append (wxT ("Note") );

		wxMessageDialog dialog (NULL, message, caption, wxOK | wxICON_INFORMATION);
		dialog.ShowModal();

		return false;
	}

	G.delNode (v_node);
	eraseCanvas();
	drawMyGraph->draw();

	return true;
}

bool BuildGraph::addEdge (const wxString& v, const wxString& w, long weight)
{
	node v_node = findVertex (v), w_node = findVertex (w);
	edge e;
	wxString strWeight;

	if (G.numberOfNodes() == 0)
		return false;

	if (v_node == NULL || w_node == NULL)
	{
		wxString message, caption;

		if (v_node == NULL)
			message.Printf (wxT ("Vertex %s is not in the graph."), v.c_str() );
		else
			message.Printf (wxT ("Vertex %s is not in the graph."), w.c_str() );

		caption.append (wxT ("Note") );

		wxMessageDialog dialog (NULL, message, caption, wxOK | wxICON_INFORMATION);
		dialog.ShowModal();

		return false;
	}

	strWeight.Printf (wxT ("%d"), (int) weight);

	if (findEdge (v,w) == NULL)
	{
		e = G.newEdge (v_node, w_node);

		if (weight != -2147483647L)
		{
			GA.label (e) = (const char *) strWeight.c_str();
			GA.intWeight (e) = (int) weight;
		}
	}
	else if (allowParallelEdges)
	{
		e = G.newEdge (v_node, w_node);

		if (weight != -2147483647L)
		{
			GA.label (e) = (const char *) strWeight.c_str();
			GA.intWeight (e) = (int) weight;
		}
	}

	switch (graphLayout)
	{
	case hierarchical:
		hierarchicalLayout();
		break;

	case forcedirected:
		forceDirectedLayout();
		break;

	case orthogonal:
		if (!orthogonalLayout() )
			return true;

		break;

	case circular:
		circularLayout();
		break;
	}

	eraseCanvas();
	drawMyGraph->draw();
	drawingCanvas->RefreshCanvas (true, drawingCanvas->GetRect() );

	return true;
}

bool BuildGraph::eraseEdge (const wxString& v, const wxString& w, bool drawGraph)
{
	ogdf::edge e;

	if (G.numberOfNodes() == 0)
		return false;

	e = findEdge (v,w);

	if (e == NULL)
	{
		wxString message, caption;

		message.Printf (wxT ("Edge %s %s is not in the graph."), v.c_str(), w.c_str() );
		caption.append (wxT ("Note") );

		wxMessageDialog dialog (NULL, message, caption, wxOK | wxICON_INFORMATION);
		dialog.ShowModal();

		return false;
	}

	G.delEdge (e);

	if (drawGraph)
	{
		eraseCanvas();
		drawMyGraph->draw();
	}

	return true;
}

node BuildGraph::findVertex (const wxString& v)
{
	List<node> vertexList;
	List<node>::iterator iter;
	string vertex = (const char *) (v.c_str() );

	G.allNodes (vertexList);

	for (iter = vertexList.begin(); iter != vertexList.end(); iter++)
		if (GA.label (*iter) == vertex)
			return *iter;

	return NULL;
}

ogdf::edge BuildGraph::findEdge (const wxString& v, const wxString& w)
{
	List<ogdf::edge> edgeList;
	List<ogdf::edge>::iterator iter;
	wxString ev_label, ew_label;

	//String(v.c_str());
	node ev, ew;

	G.allEdges (edgeList);

	for (iter = edgeList.begin(); iter != edgeList.end(); iter++)
	{
		ev = (*iter)->source();
		ew = (*iter)->target();
		ev_label = wxString (GA.label (ev).c_str(),wxConvUTF8);
		ew_label = wxString (GA.label (ew).c_str(),wxConvUTF8);

		if (graphType == digraph || graphType == digraphw)
		{
			if (ev_label == v && ew_label == w)
				return *iter;
		}
		else if ( (ev_label == v && ew_label == w) || (ev_label == w && ew_label == v) )
			return *iter;
	}

	return NULL;
}

void BuildGraph::redraw (GraphType type, GraphLayout layout, double scale,
                         bool hideEdges, wxWindow *discMathCalcMain)
{
	List<edge> allGraphEdges;
	ListIterator<edge> edgeIter;
	edge e;

	graphLayout = layout;
	graphType = type;

	if (G.numberOfNodes() == 0)
		return;

	drawingCanvas->SetScale (1.0);
	setLayout (graphLayout);
	eraseCanvas();
	drawMyGraph->setTypeAndLayout (type, layout);
	drawMyGraph->draw (hideEdges, true);
	(drawingCanvas->getSFDiagramManager() )->UpdateAll();
	drawingCanvas->RefreshCanvas (true, drawingCanvas->GetRect() );
	( (DiscreteMathCalculatorFrame *) discMathCalcMain)->resetSlider();
}

void BuildGraph::getGraphType (GraphType& gtype, bool& parallelEdges)
{
	gtype = graphType;
	parallelEdges = allowParallelEdges;
}

void BuildGraph::assignVertexLabelsAndWeights()
{
	node v;
	ogdf::edge e;
	int i = 0;
	string s;
	// CRandomMersenne rnd;

	for (node v : G.nodes)
	{
		char tmp[20];

		sprintf (tmp, "%d", i++);
		GA.label (v) = string (tmp);
	}

	if (graphType == undirw || graphType == digraphw)
		for (edge e : G.edges)
		{
			char tmp[20];

			sprintf (tmp, "%d", rnd.IRandomX (1,50) );
			GA.label (e) = string (tmp);
		}
}

void BuildGraph::drawRandomGraph (RandomGraphType rtype, int m, int n)
{
	double p1, p2;
	CRandomMersenne rnd;
	node v;
	List<node> nodes;
	List<node>::iterator iter;
	DfsAcyclicSubgraph DAS;
	int max;

	switch (rtype)
	{
	case general:
		randomGraph (G, m, n);

		if (!allowParallelEdges)
			ogdf::makeParallelFreeUndirected (G);

		break;

	case simple:
		max = m * (m-1) / 2;

		if (n > max)
			n = max;
		else if (n < m)
			n = m;

		ogdf::randomSimpleGraph (G, m, n);
		ogdf::makeConnected (G);
		break;

	case biconnected:
		ogdf::randomBiconnectedGraph (G, m, n);
		ogdf::makeConnected (G);
		break;

	case triconnected:
		p1 = 0.25 + 0.15*rnd.Random();
		p2 = 0.25 + 0.15*rnd.Random();
		ogdf::randomTriconnectedGraph (G, m, p1, p2);
		break;

	case planar:
		ogdf::randomPlanarConnectedGraph (G, m, n);
		break;

	case ::acyclic:
		ogdf::randomTree (G, m, m, m);
		break;

	case ::bipartite:
		graphType = undir;
		ogdf::completeBipartiteGraph (G, m, n);
		break;
	}

	for (node v : G.nodes)
	{
		GA.width (v) = GA.height (v) = 35.0;
		GA.fillColor (v) =  ogdf::Color(255,215,0);
	}

	for (edge e : G.edges)
	{
		GA.arrowType (e) = ogdf::EdgeArrow::None;
	}

	assignVertexLabelsAndWeights();

	if (rtype == ::bipartite)
		graphLayout = hierarchical;
	else if (rtype == ::planar)
		graphLayout = orthogonal;
	else if (rtype == ::acyclic)
		graphLayout = GraphLayout::tree;

	setLayout (graphLayout);

	drawingCanvas->setGA (&GA);
	drawingCanvas->SetScale (1.0);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,\
	                             graphType, graphLayout);
	drawMyGraph->draw();
}

bool BuildGraph::drawRandomDigraph (RandomDigraphType rdtype, int m, int n,
                                    wxRadioBox* graphLayoutRadioBox)
{
	double p1, p2;
	CRandomMersenne rnd;
	node v;
	List<node> nodes;
	List<node>::iterator iter;
	int max;

	switch (rdtype)
	{
	case RandomDigraphType::randomDigraph:
		ogdf::randomDigraph (G, m, 0.50);
		break;

	case strongly_connected:
		if (!buildDigraphWithStrongComponents (m,n) )
			return false;

		graphLayout = circular;
		graphLayoutRadioBox->SetSelection (3);
		break;

	case RandomDigraphType::dag:
		ogdf::randomDigraph (G, m, 0.60);
		ogdf::makeAcyclic (G);
		break;

	case tournament:
		buildTournamentDigraph (m);
		break;
	}

	for (node v : G.nodes)
	{
		GA.width (v) = GA.height (v) = 35.0;
		GA.fillColor (v) =  ogdf::Color(255,215,0);
	}

	assignVertexLabelsAndWeights();

	setLayout (graphLayout);

	drawingCanvas->setGA (&GA);
	drawingCanvas->SetScale (1.0);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,\
	                             graphType, graphLayout);
	drawMyGraph->draw();
	return true;
}

void BuildGraph::buildTournamentDigraph (int numVertices)
{
	List<edge> allGraphEdges;
	ListIterator<edge> edgeIter;
	node v, w;
	edge e;
	static CRandomMersenne rnd;
	int direction;

	ogdf::completeGraph (G, numVertices);

	G.allEdges (allGraphEdges);
	edgeIter = allGraphEdges.begin();

	while (edgeIter != allGraphEdges.end() )
	{
		e = *edgeIter;
		v = (*edgeIter)->source();
		w = (*edgeIter)->target();
		G.delEdge (e);
		direction = rnd.IRandomX (0, 1);

		if (direction == 0)
			G.newEdge (v,w);
		else
			G.newEdge (w,v);

		edgeIter++;
	}
}

void BuildGraph::makeStrongComponent (Graph &g, int numVertices)
{
	ogdf::GraphAttributes GSC;
	bool stronglyConnected;
	NodeArray<int> c (g);

	GSC.init (g,
	          GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
	          GraphAttributes::nodeLabel | GraphAttributes::nodeStyle | GraphAttributes::nodeType |
	          GraphAttributes::edgeStyle | GraphAttributes::edgeStyle | GraphAttributes::edgeLabel |
	          GraphAttributes::nodeStyle | GraphAttributes::nodeTemplate |
	          GraphAttributes::edgeArrow | GraphAttributes::edgeIntWeight | GraphAttributes::edgeType |
	          GraphAttributes::edgeDoubleWeight);
	stronglyConnected = false;

	while (!stronglyConnected)
	{
		ogdf::randomDigraph (g,numVertices,0.25);
		ogdf::makeConnected (g);
		stronglyConnected = (strongComponents (g, c) == 1);

		if (!stronglyConnected)
		{
			c.init (g);
			g.clear();
		}
	}
}

bool BuildGraph::buildDigraphWithStrongComponents (int numComponents,
      int maxPerComponent)
{
	vector<ogdf::Graph> comp (numComponents);
	static CRandomMersenne rnd;
	int i, componentSize;

	makeStrongComponent (G,maxPerComponent);

	for (i = 1; i < numComponents; i++)
	{
		componentSize = rnd.IRandomX (1,maxPerComponent);
		makeStrongComponent (comp[i],componentSize);
		addStrongComponent (G,comp[i]);
	}

	graphLayout = circular;
	return true;
}

void BuildGraph::addStrongComponent (ogdf::Graph& G, ogdf::Graph& component)
{
	List<node> allGraphNodes;
	List<edge> allGraphEdges;
	ListIterator<edge> edgeIter;
	ListIterator<node> vertexIter;
	map<node,node> ComptoG;
	ogdf::node n, vcomp, v, wcomp, w;
	ogdf::edge e;

	node lastInG = G.lastNode(), firstInComponent = component.firstNode();
	component.allEdges (allGraphEdges);
	component.allNodes (allGraphNodes);
	vertexIter = allGraphNodes.begin();

	while (vertexIter != allGraphNodes.end() )
	{
		n = G.newNode();
		ComptoG[*vertexIter] = n;
		vertexIter++;
	}

	edgeIter = allGraphEdges.begin();

	while (edgeIter != allGraphEdges.end() )
	{
		e = *edgeIter;
		vcomp = e->source();
		v = ComptoG[vcomp];
		wcomp = e->target();
		w = ComptoG[wcomp];
		G.newEdge (v,w);
		edgeIter++;
	}

	G.newEdge (lastInG, ComptoG[firstInComponent]);
}

void BuildGraph::drawKn (int n)
{
	node v;

	drawingCompleteGraph = true;
	ogdf::completeGraph (G,n);

	for (node v : G.nodes)
	{
		GA.width (v) = GA.height (v) = 35.0;
		GA.fillColor (v) =  ogdf::Color(255,215,0);
	}

	assignVertexLabelsAndWeights();

	setLayout (graphLayout);

	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             graphType, graphLayout);
	drawMyGraph->draw();
}

void BuildGraph::drawGridGraph (int m, int n)
{
	ogdf::gridGraph (G, n, m, false, false);
	ogdf::List<node> nodeList;
	ogdf::ListIterator<node> nodeIter;
	int i, j, k;

	G.allNodes (nodeList);
	nodeIter = nodeList.begin();
	k = 0;

	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			GA.label (*nodeIter) = std::to_string (k++);
			GA.fillColor (*nodeIter) =  ogdf::Color(255,215,0);
			nodeIter++;
		}
	}

	allowParallelEdges = false;
	graphType = undir;
	drawingCompleteGraph = true;
	orthogonalLayout();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             undir, forcedirected);
	drawMyGraph->draw();
}

void BuildGraph::drawTSP (int n)
{
	node v;

	drawingCompleteGraph = true;
	ogdf::completeGraph (G,n);

	graphType = undirw;

	for (node v : G.nodes)
	{
		GA.width (v) = GA.height (v) = 35.0;
		GA.fillColor (v) =  ogdf::Color(255,215,0);
	}

	generate_TSP_graph (n);

	graphLayout = hierarchical;
	setLayout (graphLayout);
	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             graphType, graphLayout);
	drawMyGraph->draw();
}

void BuildGraph::drawHamCycle (int numVertices,int numEdges)
{
	node v;

	drawingCompleteGraph = false;
	graphType = undir;
	ogdf::randomBiconnectedGraph (G, numVertices, numEdges);
	ogdf::makeConnected (G);

	for (node v : G.nodes)
	{
		GA.width (v) = GA.height (v) = 35.0;
		GA.fillColor (v) =  ogdf::Color(255,215,0);
	}

	assignVertexLabelsAndWeights();

	setLayout (graphLayout);

	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             graphType, graphLayout);
	drawMyGraph->draw();
}

bool BuildGraph::checkForSelfLoops()
{
	List<edge> edges;
	List<edge>::iterator edgeIterator;
	node source, dest;

	G.allEdges (edges);

	for (edgeIterator = edges.begin(); edgeIterator != edges.end(); edgeIterator++)
	{
		source = (**edgeIterator).source();
		dest = (**edgeIterator).target();

		if (source == dest)
			return true;
	}

	return false;
}

int BuildGraph::numberOfSelfLoops()
{
	List<edge> edges;
	List<edge>::iterator edgeIterator;
	node source, dest;
	int numSelfLoops = 0;

	G.allEdges (edges);

	for (edgeIterator = edges.begin(); edgeIterator != edges.end(); edgeIterator++)
	{
		source = (**edgeIterator).source();
		dest = (**edgeIterator).target();

		if (source == dest)
			numSelfLoops++;
	}

	return numSelfLoops;
}

void BuildGraph::launchRunAlgorithms (wxWindow* parent)
{
	if (graphType == digraph || graphType == digraphw)
	{
		DigraphAlgorithmCanvas *runAlgorithmsFrame;

		runAlgorithmsFrame = new DigraphAlgorithmCanvas (parent, * (drawingCanvas->getSFDiagramManager() ),
		      drawingCanvas->GetScale(), &GA,
		      &G, wxID_ANY, wxDefaultPosition,
		      wxDefaultSize, graphType);
		runAlgorithmsFrame->Show();
		wxTheApp->SetTopWindow (runAlgorithmsFrame);
	}
	else
	{
		GraphAlgorithmCanvas *runAlgorithmsFrame;

		runAlgorithmsFrame = new GraphAlgorithmCanvas (parent, * (drawingCanvas->getSFDiagramManager() ),
		      drawingCanvas->GetScale(), &GA,
		      &G, wxID_ANY, wxDefaultPosition,
		      wxDefaultSize, graphType,this);
		runAlgorithmsFrame->Show();
		wxTheApp->SetTopWindow (runAlgorithmsFrame);
	}
}

void BuildGraph::drawHypergraph (int n)
{
	HypercubeGraph hypercube (n);
	HypercubeVisitor visitor (&hypercube, this);
	lemon::BfsVisit<lemon::HypercubeGraph, HypercubeVisitor> myHypercube (hypercube, visitor);

	myHypercube.init();

	for (HypercubeGraph::NodeIt n (hypercube); n != INVALID; ++n)
		if (!myHypercube.reached (n) )
		{
			myHypercube.addSource (n);
			myHypercube.start();
		}

	allowParallelEdges = false;
	graphType = undir;
	drawingCompleteGraph = true;
	hierarchicalLayout ();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             undir, forcedirected);
	drawMyGraph->draw();
}

void BuildGraph::drawMyDialogGraph (const vector<GraphEdge>& g)
{
	for (unsigned int i=0; i < g.size(); i++)
		insertEdge (g[i].edgeSource, g[i].edgeDest, g[i].edgeWeight,wxNORMAL_PEN,
		            wxColour (255, 215, 0, 0),wxColour (255, 215, 0, 0));

	setLayout (graphLayout);

	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             graphType, graphLayout);
	drawMyGraph->draw(false, true);
}

void BuildGraph::drawMyRelationGraph (const vector<RelationGraphEdge>& g, bool drawDirectedGraph, wxWindow *discMathCalcMain,
                                      const vector<wxString>& vertices)
{
    // Create every vertex before adding edges. insertEdge() uses str2node.
    for (const wxString& label : vertices)
    {
        if (str2node.find(label) != str2node.end())
            continue;
        ogdf::node v = G.newNode();
        str2node[label] = v;
        GA.label(v) = std::string(label.mb_str());
        GA.width(v) = std::max(35.0, double(getStringExtent(label, vertexFont) + 4));
        GA.height(v) = 35.0;
        GA.strokeWidth(v) = 1.0;
        GA.shape(v) = ogdf::Shape::Rect;
        GA.fillColor(v) = ogdf::Color(255, 215, 0);
    }
	for (unsigned int i=0; i < g.size(); i++)
	{
		insertEdge (g[i].edgeSource, g[i].edgeDest,wxT(""),wxNORMAL_PEN,
		            wxColour (255, 215, 0, 0),wxColour (255, 215, 0, 0));
	}

	setLayout (graphLayout);

	drawingCanvas->setGA (&GA);
	eraseCanvas();

	if (drawDirectedGraph)
		drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
		                             digraph, graphLayout);
	else
		drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
		                             undir, graphLayout);

	drawMyGraph->draw();
	redraw (graphType, graphLayout, 1.0, false, discMathCalcMain);
}

void BuildGraph::drawGeneralizedPetersonGraph (int n, int k)
{
	wxString *u = new wxString[n], *v = new wxString[n];
	wxString format;
	int i;

	for (i=0; i < n; i++)
	{
		format.Printf (wxT ("u%d"), i);
		u[i] = format;
		format.Printf (wxT ("v%d"), i);
		v[i] = format;
	}

	for (i=0; i < n; i++)
	{
		insertEdge (u[i%n],u[ (i+1) %n],wxT(""),wxNORMAL_PEN,
		            wxColour (255, 215, 0, 0),wxColour (255, 215, 0, 0));
		insertEdge (u[i%n], v[i%n],wxT(""),wxNORMAL_PEN,
		            wxColour (255, 215, 0, 0),wxColour (255, 215, 0, 0));
		insertEdge (v[i%n], v[ (i+k) %n],wxT(""),wxNORMAL_PEN,
		            wxColour (255, 215, 0, 0),wxColour (255, 215, 0, 0));
	}

	allowParallelEdges = false;
	graphType = undir;
	graphLayout = forcedirected;
	forceDirectedLayout();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             undir, forcedirected);
	delete[] u;
	delete[] v;

	drawMyGraph->draw(false,true);
}

void BuildGraph::drawGraphWithClique (int cliqueSize, int numberOfVertices, int numberOfEdges)
{
	node v, w;
	CRandomMersenne rnd;
	string s;
	int additionalEdges = numberOfEdges - (cliqueSize) * (cliqueSize + 1) /2;
	int i;
	vector<node> cliqueNodes;

	drawingCompleteGraph = true;
	ogdf::completeGraph (G,cliqueSize);

	for (node v : G.nodes)
		cliqueNodes.push_back (v);

	assignVertexLabelsAndWeights();

	for (i = cliqueSize; i < numberOfVertices; i++)
	{
		char tmp[20];
		sprintf (tmp, "%d", i);
		v = G.newNode();
		GA.label (v) = string (tmp);
	}

	for (i=1; i <= additionalEdges; i++)
	{
		do
		{
			v = G.chooseNode();
			w = G.chooseNode();
		}
		while (v == w);

		G.newEdge (v, w);
	}

	for (node v : G.nodes)
	{
		if (v->degree() <= 1)
		{
			for (int j=0; j < 3; j++)
			{
				i = rnd.IRandomX (0,cliqueSize-1);
				G.newEdge (v, cliqueNodes[i]);
			}
		}
	}

	for (node v : G.nodes)
	{
		GA.width (v) = GA.height (v) = 35.0;
		GA.fillColor (v) =  ogdf::Color(255,215,0);
	}

	for (edge e : G.edges)
	{
		GA.arrowType (e) = ogdf::EdgeArrow::None;
	}

	drawingCompleteGraph = true;
	circularLayout();
	graphLayout = circular;
	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             graphType, graphLayout);
	drawMyGraph->draw();
}

void BuildGraph::drawTransposeGraph (wxWindow *discMathCalcMain)
{
	if (G.empty() )
		return;

	if (! (graphType == digraph || graphType == digraphw) )
		return;

	List<edge> edgeList;
	ListIterator<edge> edgeIter;

	G.allEdges (edgeList);

	for (edgeIter = edgeList.begin(); edgeIter != edgeList.end(); edgeIter++)
		G.reverseEdge (*edgeIter);

	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawingCanvas->SetScale (1.0);

	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             graphType, graphLayout);
	drawMyGraph->draw();
	redraw (graphType, graphLayout, 1.0, false, discMathCalcMain);
}

void BuildGraph::drawComplementaryGraph (wxWindow *discMathCalcMain)
{
	if (G.empty() )
		return;

	if (! (graphType == undir || graphType == undirw) )
		return;

	node u,v;
	List<node> nodeList;
	ListIterator<node> nodeIter;
	List<edge> edgeList;
	ListIterator<edge> edgeIter;
	vector<edge> originalEdges;
	vector<node> nodes;
	unsigned int i, j;
	string labelString;
	node titleNode;
	DRect boundingBox;
	DPoint p;

	G.allEdges (edgeList);

	for (edgeIter = edgeList.begin(); edgeIter != edgeList.end(); edgeIter++)
		originalEdges.push_back (*edgeIter);

	G.allNodes (nodeList);

	for (nodeIter = nodeList.begin(); nodeIter != nodeList.end(); nodeIter++)
	{
		if (GA.shape (*nodeIter) == Shape::Ellipse)
		{
			labelString = GA.label (*nodeIter);
			G.delNode (*nodeIter);
			continue;
		}

		nodes.push_back (*nodeIter);
	}

	for (i=0; i < nodes.size(); i++)
	{
		u = nodes[i];

		for (j=i+1; j < nodes.size(); j++)
		{
			v = nodes[j];

			if (!edgeBetween (u, v) && u != v)
				G.newEdge (u,v);
		}
	}

	makeParallelFreeUndirected (G);
	G.allEdges (edgeList);

	for (edgeIter = edgeList.begin(); edgeIter != edgeList.end(); edgeIter++)
	{
		GA.arrowType (*edgeIter) = ogdf::EdgeArrow::None;

		if (originalEdge (originalEdges, *edgeIter) )
			G.delEdge (*edgeIter);
	}

	if (labelString.length() != 0)
	{
		titleNode = G.newNode();
		GA.label (titleNode) = "          " + labelString;
		GA.shape (titleNode) = Shape::Ellipse;
		GA.width (titleNode) = 35.0;
		GA.height (titleNode) = 35.0;
		GA.strokeWidth (titleNode) = 0.0;
		GA.strokeType (titleNode) = ogdf::StrokeType::None;
		GA.strokeColor (titleNode) = ogdf::Color::Name::White;
		GA.fillPattern (titleNode) = ogdf::FillPattern::None;
	}

	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawingCanvas->SetScale (1.0);
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             graphType, graphLayout);
	drawMyGraph->draw();
	redraw (graphType, graphLayout, 1.0, false, discMathCalcMain);
}

void BuildGraph::treeLayout()

{
	TreeLayout gtreeLayout;
	static CRandomMersenne rnd;

	gtreeLayout.siblingDistance (35.0);
	gtreeLayout.levelDistance (40.0);
	gtreeLayout.subtreeDistance (50.0);

	if (rnd.IRandomX (0, 1) == 0)
		gtreeLayout.rootSelection (TreeLayout::RootSelectionType::Sink);
	else
		gtreeLayout.rootSelection (TreeLayout::RootSelectionType::Source);

	if (rnd.IRandomX (0, 1) == 0)
		gtreeLayout.orientation (Orientation::leftToRight);
	else
		gtreeLayout.orientation (Orientation::rightToLeft);

	gtreeLayout.call (GA);
}

void BuildGraph::setLayout (GraphLayout graphLayout)
{
	switch (graphLayout)
	{
	case hierarchical:
		hierarchicalLayout();
		break;

	case forcedirected:
		forceDirectedLayout();
		break;

	case orthogonal:
		if (!orthogonalLayout() )
			return;

		break;

	case circular:
		circularLayout();
		break;

	case GraphLayout::tree:
		treeLayout();
		break;
	}
}

bool BuildGraph::edgeBetween (node u, node v)
{
	List<edge> edgeList;
	ListIterator<edge> edgeIter;
	edge e;

	u->adjEdges (edgeList);

	for (edgeIter=edgeList.begin(); edgeIter != edgeList.end(); edgeIter++)
	{
		e = *edgeIter;

		if ( (*e).source() == v || (*e).target() == v)
			return true;
	}

	return false;
}

bool BuildGraph::originalEdge (const vector<edge>& originalEdges, edge e)
{
	for (unsigned int i=0; i < originalEdges.size(); i++)
		if (originalEdges[i] == e)
			return true;

	return false;
}

void BuildGraph::randomGraph (Graph &G, int n, int m)
{
	static CRandomMersenne rnd;
	static CRandomMersenne pedge1, pedge2;
	int v1, v2, i, j, k;

	G.clear();

	Array<node> v (n);

	for (i = 0; i < n; i++)
		v[i] = G.newNode();

	for (i = 0; i < m; i++)
	{
		v1 = rnd.IRandomX (0, n-1);
		v2 = rnd.IRandomX (0, n-1);

		G.newEdge (v[v1],v[v2]);
	}
}

GraphLayout BuildGraph::getGraphLayout()
{
	return graphLayout;
}

void BuildGraph::labelTSPvertices (int n, vector<node>& nodeArr)
{
	char tmp[20];
	node v;
	int i = 0;

	nodeArr.clear();

	for (node v: G.nodes)
	{
		GA.label (v) = to_string (i);
		nodeArr.push_back (v);
		i++;
	}
}

void BuildGraph::generate_TSP_graph (int n)
{
	int max = -1;
	int weight;
	int i, j;
	vector<node> nodeArr;
	edge e;
	char tmp[20];

	labelTSPvertices (n, nodeArr);

	for (i=0; i < n; i++)
	{
		for (j=0; j < n; j++)
		{
			if (j == i)
				continue;

			e = G.searchEdge (nodeArr[i], nodeArr[j]);
			weight = rnd.IRandomX (0, 15);

			if (weight > max)
				max = weight;

			GA.label (e) = std::to_string (weight);
		}
	}

	for (edge e : G.edges)
	{
		string s;

		s = GA.label (e);
		weight = std::stoi (s);
		weight += max;
		GA.label (e) = std::to_string (weight);
	}
}

bool BuildGraph::buildRelationFromREL(const wxString& relFileName,
                                      bool hideEdges)
{
	wxTextFile relationFile;

	if (!relationFile.Open (relFileName) )
		throw graphFileFormatException ("Failure reading graph REL file");

	buildGraphFromTextfile (&relationFile,hideEdges);
	relationFile.Close();

	setLayout (forcedirected);

	drawingCanvas->setGA (&GA);
	eraseCanvas();
	drawMyGraph = new DrawGraph (&GA, &G, drawingCanvas->getSFDiagramManager(), drawingCanvas,
	                             digraph, forcedirected);
	drawMyGraph->draw (hideEdges, true);

	return true;
}
