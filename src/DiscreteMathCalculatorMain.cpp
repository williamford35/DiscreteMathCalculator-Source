#include <wx/dcclient.h>
#include <functional>
#include <utility>
#include <set>
#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/textctrl.h>
#include <limits>
#include <wx/dialog.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/dcbuffer.h>
#include <wx/filefn.h>
#include <wx/tokenzr.h>
#include <wx/choicdlg.h>
#include <wx/listctrl.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/utils.h>
#include <map>
#include <wx/display.h>

/////////////////////////////////////////////////////////////////////////////
// Name:        DiscreteMathCalculatorMain.cpp
// Purpose:
// Author:      William Ford (wford333@gmail.com)
// Created:
// Copyright:   (c) 2023 William Ford
// Licence:     GNU General Public Licence version 3
/////////////////////////////////////////////////////////////////////////////

#define FOR_DISTRIBUTION
// #undef FOR_DISTRIBUTION

#ifdef _DEBUG
#include "wxStringToCstr.h"
#endif // _DEBUG

#include "DiscreteMathCalculatorMain.h"
#include "AboutDiscreteMathCalculator.h"


#ifdef __APPLE__
#include "CFBundle.h"
#endif

#include "common/report.h"
#include "common/listingTooLongException.h"
#include "common/range_error.h"
#include "common/ImageFiles.h"
#include "common/runApplication.h"
#include "common/MenuEventHandler.h"
#include "common/itoa.h"

#include "biginteger_calculator/euler.h"
#include "biginteger_calculator/seed.h"
#include "biginteger_calculator/SpellNumberDialog.h"

#include "counting-enumeration/AsymptoticEstimate.h"
#include "counting-enumeration/NamedSequences.h"
#include "counting-enumeration/useAsymptoticEstimateException.h"
#include "counting-enumeration/ResultMagnitude.h"
#include "counting-enumeration/ResultMagnitude2.h"
#include "triangles/Triangles.h"
#include "trees/btrFileFormatException.h"
#include "trees/binary/EnterBinaryTree.h"
#include "trees/EnterData.h"
#include "trees/search/stree.h"
#include "trees/search/AVLTree.h"
#include "trees/RandomData.h"
#include "trees/huffman/EnterHuffmanData.h"
#include "trees/huffman/HuffmanTree.h"
#include "trees/expression/ExpressionTree.h"
#include "trees/binary/BinaryTreeDrawingCanvas.h"
#include "trees/search/SearchTreeDrawingCanvas.h"
#include "trees/huffman/HuffmanDrawingCanvas.h"
#include "trees/expression/ExpressionTreeDrawingCanvas.h"
#include "trees/heap/HeapDrawingCanvas.h"
#include "trees/general/GeneralTreeDrawingCanvas.h"
#include "trees/general/EnterGeneralTree.h"
#include "trees/isomorphism/TreeIsomorphism.h"
#include "trees/isomorphism/TreeIsomorphismDrawingCanvas.h"

#include "graphs/graphFileFormatException.h"
#include "graphs/build/GraphEdgeStyles.h"
#include "graphs/enter/EnterGraphDialog.h"
#include "graphs/draw/DrawGraph.h"
#include "graphs/build/RandomGraph.h"
#include "graphs/algorithms/matching/StableMarriageDialog.h"
#include "graphs/build/Peterson.h"
#include "graphs/AnalyzeGraph.h"
#include "graphs/algorithms/isomorphism/IsomorphismDrawingCanvas.h"
#include "graphs/algorithms/InputIntegerData.h"
#include "graphs/algorithms/DisplayAlgorithmResults.h"
#include "graphs/algorithms/digraphicalsequence.h"
#include "graphs/build/RandomClique.h"
#include "graphs/build/HamCycle.h"
#include "graphs/algorithms/closure/ClosureDisplay.h"
#include "elliptic-curves/defineEllipticCurve.h"

#include "RelationsPartialOrders/RelationInput.h"
#include "RelationsPartialOrders/PartialOrderDialog.h"

#include "discrete-logarithm/DiscreteLog.h"

// #include "elliptic-curves/curves.h"
#include "elliptic-curves/schoof.h"
#include "elliptic-curves/schoofException.h"
#include "elliptic-curves/FindECBasepoint.h"
#include "elliptic-curves/plot/plotellipticcurve.h"
// #include "elliptic-curves/generate_verifyable_curve.h"
#include "elliptic-curves/DiscreteLogarithmEc.h"
#include "elliptic-curves/securedomain.h"
#include "elliptic-curves/ecdlp.h"
#include "elliptic-curves/collectBigFactors.h"
#include "elliptic-curves/securedomain.h"
#include "elliptic-curves/RSAFunctions.h"
#include "elliptic-curves/plot/pointplotellipticcurve.h"
#include "factor/factordialog.h"

#include <sstream>
#include <stdexcept>
#include <string>

#include <wx/msgdlg.h>
#include <wx/textfile.h>
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
#include <wx/tooltip.h>
#include <wx/utils.h>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/extended_graph_alg.h>
#include <ogdf/basic/simple_graph_alg.h>

#include "NTL/ntlException.h"
#include <NTL/pair_GF2EX_long.h>

#include <sys/time.h>
#include <iostream>
using namespace std;
#include <fstream>

// class __APPLE__;
using namespace std;

//(*InternalHeaders(DiscreteMathCalculatorFrame)

//*)

// helper functions
enum wxbuildinfoformat { short_f, long_f };

wxString wxbuildinfo (wxbuildinfoformat format)
{
	wxString wxbuild (wxVERSION_STRING);

	if (format == long_f)
	{
#if defined(__WXMSW__)
		wxbuild << _T ("-Windows");
#elif defined(__UNIX__)
		wxbuild << _T ("-Linux");
#endif

#if wxUSE_UNICODE
		wxbuild << _T ("-Unicode build");
#else
		wxbuild << _T ("-ANSI build");
#endif // wxUSE_UNICODE
	}

	return wxbuild;
}

// IdInit(DiscreteMathCalculatorFrame)
const long DiscreteMathCalculatorFrame::ID_A_VALUE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_A_VALUE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_B_VALUE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_B_VALUE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_N_VALUE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_N_VALUE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_RESULT_VALUE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_RESULT_VALUE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DATA_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SPECIAL_OPERATIONS_STATICTEXT1 = wxNewId();
const long DiscreteMathCalculatorFrame::ID_CLEAR_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BASIC_OPERATORS_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_IS_A_PRIME_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRANSFER_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MEMORY_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ADD_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ADD_MOD_N_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PHI_N_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TO_A_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MEMORY_STORE_1_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MEMORY_STORE_2_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUB_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUB_MOD_N_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_NULL_FILL_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PHI_A_B_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TO_B_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MEMORY_1_PLUS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MEMORY_2_PLUS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MUL_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MUL_MOD_N_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_X_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_X_BIT_PRIME_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TO_N_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MEMORY_RECALL_1_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MEMORY_RECALL_2_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DIV_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MOD_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GCD_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MEMORY_CLEAR_1_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_MEMORY_CLEAR_2_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_POWER_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_POWER_MOD_N_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_LEAST_COMMON_MULTIPLE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_INVERSE_MOD_N_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COPRIME_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_RESULT_MAGNITUDE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SPELL_RESULT_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_OPERATORS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_NUMBER_THEORETIC_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_N_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_N_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_K_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_K_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_END_ENUMERATION_RESULT_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_RESULT_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_RESULT_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_COUNTING_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_STATICLINE1 = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_LIST_PERMUTATIONS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_LIST_COMBINATIONS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_LIST_BUTTONS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_FACTORIAL_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_PERMUTATIONS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_COMBINATIONS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_FIBONACCI_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_CATALAN_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_SCHRODER_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_BELL_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_STIRLING_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_NUMBER_COMPUTATION_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_ESTIMATE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_RESULT_MAGNITUDE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_ASYMPTOTIC_ESTIMATE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_RESULT_MAGNITUDE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_ESTIMATE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COUNTING_AND_ENUMERATION_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_DATA_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_DATA_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_DATA_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_OUTPUT_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_PASCAL_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_HOSOYA_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_CATALAN_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_SCHRODER_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_BELL_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_BUTTONS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRIANGLES_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_SET_SIZE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_SET_SIZE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_K_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_K_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_SIZE_K_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_COMPLETE_SIZE_K_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_NUMBER_OF_SUBSETS_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_NUMBER_OF_SUBSETS_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_NUMBER_OF_SUBSSETS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_LISTINGS_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_NUMBER_OF_SUBSETS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_LIST_ALL_SUBSETS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_LIST_ALL_K_ELEMENT_SUBSETS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_BUTTONS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBSETS_ASYMPTOTIC_ESTIMATE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_N_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_N_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_K_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_K_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_N_K_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITONS_BELL_NUMBER_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_BELL_NUMBER_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_BELL_NUMBER_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_STIRLING_NUMBER_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_STIRLING_NUMBER_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_STIRLING_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_LISTING_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_LISTING_LABEL_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_LISTING_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_BELL_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_ESTIMATE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_RESULT_MAGNITUDE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_ASYMPTOTIC_ESTIMATE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_STIRLING_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITONS_LIST_PARTITIONS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_LIST_STIRLING_PARTITIONS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITIONS_BUTTONS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTITONS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_GENERAL_TREE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_GENERAL_BINARY_TREE_ENTER_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_GENERAL_TREE_TITLE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_GENERAL_TREE_TITLE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_TITLE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_ENTER_BTR_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_ENTER_BTR_FROM_FILE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_BTR_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_STATICLINE = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_ENTER_GML_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_ENTER_GENERAL_TREE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_HUFFMAN_TREE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_STATICTEXT2 = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREE_HUFFMAN_TREE_TITLE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_HUFFMAN_TREE_TITLE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_HUFFMAN_TREE_ENTER_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_HUFFMAN_TREE_STATICLINE = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_HUFFMAN_TREE_FROM_GML_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_HUFFMAN_TREE_ENTER_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_HUFFMAN_TREE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_EXPRESSION_TREE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_STATICTEXT3 = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_EXPRESSION_TREE_TITLE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_EXPRESSION_TREE_TITLE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_EXPRESSION_TREE_ENTER_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_STATICLINE2 = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_EXPRESSION_TREE_FROM_GML_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_EXPRESSION_TREE_ENTER_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_EXPRESSION_TREE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_HUFFMAN_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_STATICLINE1 = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_STATICLINE2 = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_STATICLINE3 = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_SEARCH_TREE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_TITLE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_SEARCH_TREES_TITLE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_TITLE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_ENTER_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_SEARCH_TREE_ENTER_FROM_FILE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_RANDOM_DATA_ENTER_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_CONTROL_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_STATICLINE = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_FROM_GML_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_ENTER_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RADIOBOX = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_TITLE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_SELECTION_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_ENTER_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_TEXTFILE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RANDOM_DATA_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_CONTROL_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_STATICLINE = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_GML_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_CONTROL_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_HEAP_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_HEAP_TITLE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_HEAP_TITLE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BUILD_HEAP_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ENTER_HEAP_RADIOBOX = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ENTER_HEAP_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ENTER_RANDOM_HEAP_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ENTER_HEAP_FROM_TEXTFILE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ENTER_HEAP_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_HEAP_STATICLINE = wxNewId();
const long DiscreteMathCalculatorFrame::ID_HEAP_ENTER_FROM_GML_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PANEL2 = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREE_TITLE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREE_TITLE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREE_TITLE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREE_ENTER_FROM_TEXTFILE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREE_ENTER_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREE_STATICLINE = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREE_ENTER_FROM_GTR_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREES_ENTER_FROM_GML_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PANEL1 = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_HEAP_GENERAL_TREE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREES_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_PREVIEW_WINDOW = wxNewId();
const long DiscreteMathCalculatorFrame::ID_RELATIONS_PARTIAL_ORDERS_DRAWING_WINDOW = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_PREVIEW_SLIDER = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_PREVIEW_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_TYPE_RADIOBOX = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_WEIGHTED_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_WEIGHTED_CHECKBOX = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ALLOW_PARALLEL_EDGES_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ALLOW_PARALLEL_EDGES_CHECKBOX = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_HIDE_EDGES_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_HIDE_EDGES_CHECKBOX = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_LAYOUT_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_LAYOUT_ALGORITHM_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_LAYOUT_RADIOBOX = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_LAYOUT_ALGORITHM_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_WEIGHTED_LAYOUT_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_FROM_TSP_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_TYPE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_TITLE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_TITLE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_TITLE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ENTER_FROM_DIALOG_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ENTER_FROM_GRPH_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ENTER_FROM_GML_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ENTER_FROM_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_REDRAW_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_REDRAW_RANDOM_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ENTER_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_VERTEX_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_VERTEX_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ERASE_VERTEX_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_VERTEX_BUTTON_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_VERTEX_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_VERTEX_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_EDGE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_EDGE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ERASE_EDGE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_EDGE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_EDGE_DEST_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_EDGE_DEST_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_EDGE_DEST_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_EDGE_WEIGHT_STATICTEXT = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_EDGE_WEIGHT_TEXTCTRL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_EDGE_WEIGHT_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_ADD_ERASE_EDGE_CHOICE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_BUTTON_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ADD_ERASE_EDGE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_ISOMORPHISM_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SUBGRAPH_ISOMORPHISM_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_CLEAR_GRAPH_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ANALYZE_GRAPH_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TRANSPOSE_GRAPH_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_COMPLEMENTARY_GRAPH_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_RUN_ALGORITHMS_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_CREATE_TSP_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_BUTTON_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_NOTEBOOK = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::idMenuQuit = wxNewId();
const long DiscreteMathCalculatorFrame::ID_CREATE_RANDOM_GRAPH_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRID_GRAPH_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHICAL_SEQUENCE_GRAPH_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DIGRAPHICAL_SEQUENCE_GRAPH_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DRAW_KN_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_MAKE_CLIQUE = wxNewId();
const long DiscreteMathCalculatorFrame::ID_HYPERCUBE_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_STABLE_MARRIAGE_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_CREATE_HAMCYCLE_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_CREATE_GENERALIZED_PETERSON_GRAPH_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_HELP_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_HELP_ABOUT_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_STATUSBAR = wxNewId();
//*)
const long DiscreteMathCalculatorFrame::ID_GRAPHS_PREVIEW_WINDOW_PRINT_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPHS_PREVIEW_WINDOW_SAVE_AS_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_SAVE_AS_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_RELATIONS_AND_PARTIALORDERS_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GF_P_ELLIPTIC_CURVE_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ENCRYPTION_PANEL = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BUILD_GF_p_SAVECURVEPARAMETERSMENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BUILD_GF_p_READCURVEPARAMETERSMENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BUILD_GF_p_DECIMALMENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_SAVE_CURVE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_READ_CURVE_BUTTON = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ECCPUBLICKEYAPPLICATIONMENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ECDIGITALSIGNATUREAPPLICATIONMENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PAIRINGSAPPLICATIONMENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DISCRETE_LOGARITHM_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DIGITAL_SIGNATURE_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_RSA_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_FACTOR_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DIFFIE_HELLMANKEYEXCHANGE_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_ECDLP_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ELLIPTIC_CURVE_DIGITAL_SIGNATURE_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BINARY_TREE_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TXT_TREE_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GML_TREE_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GENERAL_TREE_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_RELATIONS_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_PARTIAL_ORDER_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_ECC_SAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_GRAPH_LAYOUTS_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TYPES_OF_UNDIRECTED_GRAPHS_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_TYPES_OF_DIRECTED_GRAPHS_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_UNDIRECTED_GRAPH_ALGORITHMS_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_DIRECTED_GRAPH_ALGORITHMS_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_SEARCH_TREE_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_HEAP_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_HUFFMAN_TREE_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_EXPRESSION_TREE_EXAMPLES_MENUITEM = wxNewId();
const long DiscreteMathCalculatorFrame::ID_BALANCED_SEARCH_TREE_EXAMPLES_MENUITEM = wxNewId();
BEGIN_EVENT_TABLE (DiscreteMathCalculatorFrame, wxFrame)
	EVT_MENU (wxID_EXIT, DiscreteMathCalculatorFrame::OnExit)
	EVT_CLOSE (DiscreteMathCalculatorFrame::OnCloseWindow)
	EVT_MENU (wxID_ABOUT, DiscreteMathCalculatorFrame::OnAbout)
END_EVENT_TABLE()

static void FitFrameToScreen(wxFrame* frame)
{
    if (!frame)
        return;

    int displayIndex = wxDisplay::GetFromWindow(frame);

    if (displayIndex == wxNOT_FOUND)
        displayIndex = 0;

    wxDisplay display(displayIndex);

    wxRect area = display.GetClientArea();

    wxSize desired = frame->GetSize();

    int width = std::min(desired.GetWidth(), area.GetWidth());
    int height = std::min(desired.GetHeight(), area.GetHeight());

    frame->SetSize(width, height);
    frame->SetPosition(wxPoint(
        area.x + (area.width - width) / 2,
        area.y + (area.height - height) / 2
    ));
}

#if defined(_WIN64) || defined(__linux__)
DiscreteMathCalculatorFrame::DiscreteMathCalculatorFrame (wxWindow* parent, wxWindowID id)
#else
DiscreteMathCalculatorFrame::DiscreteMathCalculatorFrame (wxWindow* parent,
      wxWindowID id,
      const wxString& title,
      const wxPoint& pos,
      const wxSize& size,
      long style,
      const wxString& name)
	: wxFrame ( (wxFrame*) NULL, wxID_ANY, title, pos, size)
#endif
{
	// Initialize(DiscreteMathCalculatorFrame)
	wxFlexGridSizer* partitionsStirlingFlexGridSizer;
	wxFlexGridSizer* graphsAddEraseEdgeDestFlexGridSizer;
	wxFlexGridSizer* graphsWeightedLayoutFlexGridSizer;
	wxFlexGridSizer* trianglesDataFlexGridSizer;
	wxFlexGridSizer* graphPanelFlexGridSizer;
	wxFlexGridSizer* countingAndEnumerationFlexGridSizer;
	wxFlexGridSizer* graphsPreviewPanelFlexGridSizer;
	wxGridSizer* enterHeapGridSizer;
	wxFlexGridSizer* bigIntegerCalculatorPageFlexGridSizer;
	wxBoxSizer* graphsAddEraseVertexBoxSizer;
	wxGridSizer* subsetsMagnitudeGridSizer;
	wxFlexGridSizer* subsetsSetSizeKFlexGridSizer;
	wxFlexGridSizer* partitionsFlexGridSizer;
	wxFlexGridSizer* binaryTreesGeneralTreeEnterFlexGridSizer;
	wxBoxSizer* graphsAddEraseEdgeBoxSizer;
	wxFlexGridSizer* graphEdgeWeightFlexGridSizer;
	wxFlexGridSizer* binarySearchTreeSEnterFlexGridSizer;
	wxFlexGridSizer* generalTreeFlexGridSizer;
	wxGridSizer* binaryTreesHuffmanTreeEnterGridSizer;
	wxFlexGridSizer* graphsTitleFlexGridSizer;
	wxMenu* discreteMathCalculatorFileMenu;
	wxGridSizer* binaryTreesExpressionTreeEnterGridSizer;
	wxFlexGridSizer* partitionsBellNumberFlexGridSizer;
	wxGridSizer* generalTreeEnterGridSizer;
	wxFlexGridSizer* graphsAddEraseEdgeSourceFlexGridSizer;
	wxFlexGridSizer* binaryTreesControlFlexGridSizer;
	wxFlexGridSizer* binaryTreesHuffmanFlexGridSizer;
	wxFlexGridSizer* partitionsListingLabelFlexGridSizer;
	wxGridSizer* partitionsMagnitudeGridSizer;
	wxGridSizer* binaryTreesHuffmanTreeGridSizer;
	wxGridSizer* trianglesButtonsGridSizer;
	wxFlexGridSizer* graphsWeightedFlexGridSizer;
	wxFlexGridSizer* binarySearchTreesBalanceTitleFlexGridSizer;
	wxFlexGridSizer* graphsLayoutFlexGridSizer;
	wxBoxSizer* binaryTreesExpressionTreeTitleBoxSizer;
	wxGridSizer* discreteMathCalculatorGridSizer;
	wxGridSizer* partitionsButtonsGridSizer;
	wxFlexGridSizer* partitions_n_k_FlexGridSizer;
	wxFlexGridSizer* graphsAddEraseEdgeFlexGridSizer;
	wxGridSizer* subsetsButtonsGridSizer;
	wxFlexGridSizer* graphsEnterFromFlexGridSizer;
	wxFlexGridSizer* heapFlexGridSizer;
	wxBoxSizer* graphsAddEraseEdgeChoiceBoxSizer;
	wxBoxSizer* subsetsSizeKBoxSizer;
	wxFlexGridSizer* trianglesFlexGridSizer;
	wxGridSizer* countingAndEnumerationMagnitudeGridSizer;
	wxFlexGridSizer* subsetsFlexGridSizer;
	wxGridSizer* bigIntegerOperatorsGridSizer;
	wxFlexGridSizer* graphsAddEraseButtonFlexGridSizer;
	wxGridSizer* GridSizer1;
	wxGridSizer* binarySearchTreesBalanceAlgorithmsTitleGridSizer;
	wxFlexGridSizer* graphControlPanelFlexGridSizer;
	wxBoxSizer* generalTreeTitleBoxSizer;
	wxGridSizer* binaryTreesBTRGridSizer;
	wxBoxSizer* heapGeneralTreeBoxSizer;
	wxBoxSizer* subsetsNumberOfKElementSubsetsBoxSizer;
	wxFlexGridSizer* binarySearchTreesTitleFlexGridSizer;
	wxFlexGridSizer* bigIntegerDataFlexGridSizer;
	wxBoxSizer* heapBuildTitleBoxSizer;
	wxBoxSizer* binaryTreesHuffmanTreeTitleBoxSizer;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* binaryTreesTitleBoxSizer;
	wxFlexGridSizer* binaryTreesFlexGridSizer;
	wxFlexGridSizer* binarySearchTreesBalanceAlgorithmsFlexGridSizer;
	wxBoxSizer* binarySearchTreesBalanceAlgorithmsControlBoxSizer;
	wxFlexGridSizer* graphsTypePanelFlexGridSizer;
	wxBoxSizer* subsetsNumberOfSubsetsBoxSizer;
	wxFlexGridSizer* FlexGridSizer1;
	wxBoxSizer* binarySearchTreesControlBoxSizer;
	wxGridSizer* binaryTreesExpressionTreeGridSizer;
	wxGridSizer* countingAndEnumerationCountingGridSizer;
	wxBoxSizer* binarySearchTreesTitleBoxSizer;
	wxGridSizer* graphsEnterGridSizer;
	time_t seed;
	struct timeval tp;
	//get current timestamp in milliseconds
	long long mslong;


	Create (parent,
	        wxID_ANY,
	        _ ("DiscreteMathCalculator"),
	        wxDefaultPosition,
	        wxDefaultSize,
	        wxDEFAULT_FRAME_STYLE | wxRESIZE_BORDER,
	        _T ("wxID_ANY") );

wxIcon FrameIcon(imagefile("combcalc.xpm"));
	SetIcon (FrameIcon);

	// BIG INTEGER CALCULATOR PAGE

discreteMathCalculatorPanel = new wxPanel(
    this,
    ID_DISCRETE_MATH_CALCULATOR_PANEL,
    wxDefaultPosition,
    wxDefaultSize,
    wxTAB_TRAVERSAL,
    _T("ID_DISCRETE_MATH_CALCULATOR_PANEL")
);
	discreteMathCalculatorGridSizer = new wxGridSizer (0, 1, 0, 0);
	discreteMathCalculatorNotebook = new wxNotebook (discreteMathCalculatorPanel,
	      ID_DISCRETE_MATH_CALCULATOR_NOTEBOOK,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_DISCRETE_MATH_CALCULATOR_NOTEBOOK") );
	numberTheoreticPanel = new wxPanel (discreteMathCalculatorNotebook,
	                                    ID_NUMBER_THEORETIC_PANEL,
	                                    wxDefaultPosition,
	                                    wxDefaultSize,
	                                    wxTAB_TRAVERSAL,
	                                    _T ("ID_NUMBER_THEORETIC_PANEL") );
	bigIntegerCalculatorPageFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	bigIntegerCalculatorPageFlexGridSizer->AddGrowableCol (0);
	bigIntegerCalculatorPageFlexGridSizer->AddGrowableRow (1);
	dataPanel = new wxPanel (
	   numberTheoreticPanel, ID_DATA_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_DATA_PANEL") );
	bigIntegerDataFlexGridSizer = new wxFlexGridSizer (4, 2, 0, 0);
	bigIntegerDataFlexGridSizer->AddGrowableCol (1);
	aValueStaticText = new wxStaticText (
	   dataPanel, ID_A_VALUE_STATICTEXT, _ ("a"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_A_VALUE_STATICTEXT") );
	bigIntegerDataFlexGridSizer->Add (
	   aValueStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	aValueTextCtrl = new wxTextCtrl (dataPanel,
	                                 ID_A_VALUE_TEXTCTRL,
	                                 wxEmptyString,
	                                 wxDefaultPosition,
	                                 wxDefaultSize,
	                                 0,
	                                 wxDefaultValidator,
	                                 _T ("ID_A_VALUE_TEXTCTRL") );
	bigIntegerDataFlexGridSizer->Add (
	   aValueTextCtrl, 1, wxALL | wxEXPAND, 1);
	bValueStaticText = new wxStaticText (
	   dataPanel, ID_B_VALUE_STATICTEXT, _ ("b"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_B_VALUE_STATICTEXT") );
	bigIntegerDataFlexGridSizer->Add (
	   bValueStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	bValueTextCtrl = new wxTextCtrl (dataPanel,
	                                 ID_B_VALUE_TEXTCTRL,
	                                 wxEmptyString,
	                                 wxDefaultPosition,
	                                 wxDefaultSize,
	                                 0,
	                                 wxDefaultValidator,
	                                 _T ("ID_B_VALUE_TEXTCTRL") );
	bigIntegerDataFlexGridSizer->Add (
	   bValueTextCtrl, 1, wxALL | wxEXPAND, 1);
	nValueStaticText = new wxStaticText (
	   dataPanel, ID_N_VALUE_STATICTEXT, _ ("n"), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_N_VALUE_STATICTEXT") );
	bigIntegerDataFlexGridSizer->Add (
	   nValueStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	nValueTextCtrl = new wxTextCtrl (dataPanel,
	                                 ID_N_VALUE_TEXTCTRL,
	                                 wxEmptyString,
	                                 wxDefaultPosition,
	                                 wxDefaultSize,
	                                 0,
	                                 wxDefaultValidator,
	                                 _T ("ID_N_VALUE_TEXTCTRL") );
	bigIntegerDataFlexGridSizer->Add (
	   nValueTextCtrl, 1, wxALL | wxEXPAND, 1);
	resultValueStaticText = new wxStaticText (dataPanel,
	      ID_RESULT_VALUE_STATICTEXT,
	      _ ("Result"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_RESULT_VALUE_STATICTEXT") );
	bigIntegerDataFlexGridSizer->Add (
	   resultValueStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	resultValueTextCtrl = new wxTextCtrl (dataPanel,
	                                      ID_RESULT_VALUE_TEXTCTRL,
	                                      wxEmptyString,
	                                      wxDefaultPosition,
	                                      wxDefaultSize,
	                                      0,
	                                      wxDefaultValidator,
	                                      _T ("ID_RESULT_VALUE_TEXTCTRL") );
	bigIntegerDataFlexGridSizer->Add (
	   resultValueTextCtrl, 1, wxALL | wxEXPAND, 1);
	dataPanel->SetSizer (bigIntegerDataFlexGridSizer);
	bigIntegerDataFlexGridSizer->Fit (dataPanel);
	bigIntegerDataFlexGridSizer->SetSizeHints (dataPanel);
	bigIntegerCalculatorPageFlexGridSizer->Add (
	   dataPanel, 1, wxALL | wxEXPAND, 1);
	operatorsPanel = new wxPanel (numberTheoreticPanel,
	                              ID_OPERATORS_PANEL,
	                              wxDefaultPosition,
	                              wxDefaultSize,
	                              wxTAB_TRAVERSAL,
	                              _T ("ID_OPERATORS_PANEL") );
	bigIntegerOperatorsGridSizer = new wxGridSizer(11, 7, 15, 15);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	specialOperationsStaticText = new wxStaticText (operatorsPanel,
	      ID_SPECIAL_OPERATIONS_STATICTEXT1,
	      _ ("Operations"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_SPECIAL_OPERATIONS_STATICTEXT1") );
	bigIntegerOperatorsGridSizer->Add (
	   specialOperationsStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	clearButton = new wxButton (operatorsPanel,
	                            ID_CLEAR_BUTTON,
	                            _ ("Clear"),
	                            wxDefaultPosition,
	                            wxSize (120, 32),
	                            0,
	                            wxDefaultValidator,
	                            _T ("ID_CLEAR_BUTTON") );
	clearButton->SetToolTip (_ ("Clear memory registers 1 and 2") );
	bigIntegerOperatorsGridSizer->Add (
	   clearButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	basicOperatorsStaticText = new wxStaticText (operatorsPanel,
	      ID_BASIC_OPERATORS_STATICTEXT,
	      _ ("Basic Operators"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_BASIC_OPERATORS_STATICTEXT") );
	bigIntegerOperatorsGridSizer->Add (
	   basicOperatorsStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	isAPrimeButton = new wxButton (operatorsPanel,
	                               ID_IS_A_PRIME_BUTTON,
	                               _ ("Is a Prime\?"),
	                               wxDefaultPosition,
	                               wxSize (120, 32),
	                               0,
	                               wxDefaultValidator,
	                               _T ("ID_IS_A_PRIME_BUTTON") );
	isAPrimeButton->SetToolTip (_ ("Test a to determine if it is prime") );
	bigIntegerOperatorsGridSizer->Add (
	   isAPrimeButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	transferStaticText = new wxStaticText (operatorsPanel,
	                                       ID_TRANSFER_STATICTEXT,
	                                       _ ("Transfer"),
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       0,
	                                       _T ("ID_TRANSFER_STATICTEXT") );
	bigIntegerOperatorsGridSizer->Add (
	   transferStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	memoryStaticText = new wxStaticText (operatorsPanel,
	                                     ID_MEMORY_STATICTEXT,
	                                     _ ("Memory"),
	                                     wxDefaultPosition,
	                                     wxDefaultSize,
	                                     0,
	                                     _T ("ID_MEMORY_STATICTEXT") );
	bigIntegerOperatorsGridSizer->Add (
	   memoryStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 5);
	addButton = new wxButton (operatorsPanel,
	                          ID_ADD_BUTTON,
	                          _ ("+"),
	                          wxDefaultPosition,
	                          wxSize (120, 32),
	                          0,
	                          wxDefaultValidator,
	                          _T ("ID_ADD_BUTTON") );
	addButton->SetToolTip (_ ("Compute a + b") );
	bigIntegerOperatorsGridSizer->Add (addButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	add_Mod_n_Button = new wxButton (operatorsPanel,
	                                 ID_ADD_MOD_N_BUTTON,
	                                 _ ("+ (mod n)"),
	                                 wxDefaultPosition,
	                                 wxSize (120, 32),
	                                 0,
	                                 wxDefaultValidator,
	                                 _T ("ID_ADD_MOD_N_BUTTON") );
	add_Mod_n_Button->SetToolTip (_ ("Compute a + b (mod n)") );
	bigIntegerOperatorsGridSizer->Add (
	   add_Mod_n_Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	phiNButton = new wxButton (operatorsPanel,
	                           ID_PHI_N_BUTTON,
	                           _ ("phi(n)"),
	                           wxDefaultPosition,
	                           wxSize (120, 32),
	                           0,
	                           wxDefaultValidator,
	                           _T ("ID_PHI_N_BUTTON") );
	phiNButton->SetToolTip (_ ("Compute the Euler phi function of n") );
	bigIntegerOperatorsGridSizer->Add (phiNButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	to_a_Button = new wxButton (operatorsPanel,
	                            ID_TO_A_BUTTON,
	                            _ ("->a"),
	                            wxDefaultPosition,
	                            wxSize (120, 32),
	                            0,
	                            wxDefaultValidator,
	                            _T ("ID_TO_A_BUTTON") );
	to_a_Button->SetToolTip (_ ("Copy the result to a") );
	bigIntegerOperatorsGridSizer->Add (
	   to_a_Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	memoryStore1Button = new wxButton (operatorsPanel,
	                                   ID_MEMORY_STORE_1_BUTTON,
	                                   _ ("MS1"),
	                                   wxDefaultPosition,
	                                   wxSize (120, 32),
	                                   0,
	                                   wxDefaultValidator,
	                                   _T ("ID_MEMORY_STORE_1_BUTTON") );
	memoryStore1Button->SetToolTip (_ ("Store result in memory register 1") );
	bigIntegerOperatorsGridSizer->Add (
	   memoryStore1Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	memoryStore2Button = new wxButton (operatorsPanel,
	                                   ID_MEMORY_STORE_2_BUTTON,
	                                   _ ("MS2"),
	                                   wxDefaultPosition,
	                                   wxSize (120, 32),
	                                   0,
	                                   wxDefaultValidator,
	                                   _T ("ID_MEMORY_STORE_2_BUTTON") );
	memoryStore2Button->SetToolTip (_ ("Store result in memory register 2") );
	bigIntegerOperatorsGridSizer->Add (
	   memoryStore2Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	subButton = new wxButton (operatorsPanel,
	                          ID_SUB_BUTTON,
	                          _ ("-"),
	                          wxDefaultPosition,
	                          wxSize (120, 32),
	                          0,
	                          wxDefaultValidator,
	                          _T ("ID_SUB_BUTTON") );
	subButton->SetToolTip (_ ("Compute a - b") );
	bigIntegerOperatorsGridSizer->Add (subButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	sub_mod_n_Button = new wxButton (operatorsPanel,
	                                 ID_SUB_MOD_N_BUTTON,
	                                 _ ("- (mod n)"),
	                                 wxDefaultPosition,
	                                 wxSize (120, 32),
	                                 0,
	                                 wxDefaultValidator,
	                                 _T ("ID_SUB_MOD_N_BUTTON") );
	sub_mod_n_Button->SetToolTip (_ ("Compute a - b (mod n)") );
	bigIntegerOperatorsGridSizer->Add (
	   sub_mod_n_Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	nullFillStaticText = new wxStaticText (
	   operatorsPanel, ID_NULL_FILL_STATICTEXT, _ (""), wxDefaultPosition, wxDefaultSize, 0, _T ("ID_NULL_FILL_STATICTEXT") );
	bigIntegerOperatorsGridSizer->Add (
	   nullFillStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	phi_a_b_Button = new wxButton (operatorsPanel,
	                               ID_PHI_A_B_BUTTON,
	                               _ ("phi(a*b)"),
	                               wxDefaultPosition,
	                               wxSize (120, 32),
	                               0,
	                               wxDefaultValidator,
	                               _T ("ID_PHI_A_B_BUTTON") );
	phi_a_b_Button->SetToolTip (_ ("Compute the Euler phi function of a*b. Both a and b must be prime.") );
	bigIntegerOperatorsGridSizer->Add (
	   phi_a_b_Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	to_b_Button = new wxButton (operatorsPanel,
	                            ID_TO_B_BUTTON,
	                            _ ("->b"),
	                            wxDefaultPosition,
	                            wxSize (120, 32),
	                            0,
	                            wxDefaultValidator,
	                            _T ("ID_TO_B_BUTTON") );
	to_b_Button->SetToolTip (_ ("Copy the result to b") );
	bigIntegerOperatorsGridSizer->Add (
	   to_b_Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	memory1PlusButton = new wxButton (operatorsPanel,
	                                  ID_MEMORY_1_PLUS_BUTTON,
	                                  _ ("M1+"),
	                                  wxDefaultPosition,
	                                  wxSize (120, 32),
	                                  0,
	                                  wxDefaultValidator,
	                                  _T ("ID_MEMORY_1_PLUS_BUTTON") );
	memory1PlusButton->SetToolTip (_ ("Add result to memory register 1") );
	bigIntegerOperatorsGridSizer->Add (
	   memory1PlusButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	memory2PlusButton = new wxButton (operatorsPanel,
	                                  ID_MEMORY_2_PLUS_BUTTON,
	                                  _ ("M2+"),
	                                  wxDefaultPosition,
	                                  wxSize (120, 32),
	                                  0,
	                                  wxDefaultValidator,
	                                  _T ("ID_MEMORY_2_PLUS_BUTTON") );
	memory2PlusButton->SetToolTip (_ ("Add result to memory register 2") );
	bigIntegerOperatorsGridSizer->Add (
	   memory2PlusButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	mulButton = new wxButton (operatorsPanel,
	                          ID_MUL_BUTTON,
	                          _ ("*"),
	                          wxDefaultPosition,
	                          wxSize (120, 32),
	                          0,
	                          wxDefaultValidator,
	                          _T ("ID_MUL_BUTTON") );
	mulButton->SetToolTip (_ ("Compute a * b") );
	bigIntegerOperatorsGridSizer->Add (mulButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	mul_mod_n_Button = new wxButton (operatorsPanel,
	                                 ID_MUL_MOD_N_BUTTON,
	                                 _ ("* (mod n)"),
	                                 wxDefaultPosition,
	                                 wxSize (120, 32),
	                                 0,
	                                 wxDefaultValidator,
	                                 _T ("ID_MUL_MOD_N_BUTTON") );
	mul_mod_n_Button->SetToolTip (_ ("Compute a * b (mod n)") );
	bigIntegerOperatorsGridSizer->Add (
	   mul_mod_n_Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	xTextCtrl = new wxTextCtrl (operatorsPanel,
	                            ID_X_TEXTCTRL,
	                            wxEmptyString,
	                            wxDefaultPosition,
	                            wxSize(55,25),
	                            0,
	                            wxDefaultValidator,
	                            _T ("ID_X_TEXTCTRL") );
	bigIntegerOperatorsGridSizer->Add (xTextCtrl, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	x_bit_prime_Button = new wxButton (operatorsPanel,
	                                   ID_X_BIT_PRIME_BUTTON,
	                                   _ ("x-bit prime"),
	                                   wxDefaultPosition,
	                                   wxSize (120, 32),
	                                   0,
	                                   wxDefaultValidator,
	                                   _T ("ID_X_BIT_PRIME_BUTTON") );
	x_bit_prime_Button->SetToolTip (_ ("Generate an x-bit prime") );
	bigIntegerOperatorsGridSizer->Add (
	   x_bit_prime_Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	to_n_Button = new wxButton (operatorsPanel,
	                            ID_TO_N_BUTTON,
	                            _ ("->n"),
	                            wxDefaultPosition,
	                            wxSize (120, 32),
	                            0,
	                            wxDefaultValidator,
	                            _T ("ID_TO_N_BUTTON") );
	to_n_Button->SetToolTip (_ ("Copy the result to n") );
	bigIntegerOperatorsGridSizer->Add (
	   to_n_Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	memoryRecall1Button = new wxButton (operatorsPanel,
	                                    ID_MEMORY_RECALL_1_BUTTON,
	                                    _ ("MR1"),
	                                    wxDefaultPosition,
	                                    wxSize (120, 32),
	                                    0,
	                                    wxDefaultValidator,
	                                    _T ("ID_MEMORY_RECALL_1_BUTTON") );
	memoryRecall1Button->SetToolTip (_ ("Recall memory register 1") );
	bigIntegerOperatorsGridSizer->Add (
	   memoryRecall1Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	memoryRecall2Button = new wxButton (operatorsPanel,
	                                    ID_MEMORY_RECALL_2_BUTTON,
	                                    _ ("MR2"),
	                                    wxDefaultPosition,
	                                    wxSize (120, 32),
	                                    0,
	                                    wxDefaultValidator,
	                                    _T ("ID_MEMORY_RECALL_2_BUTTON") );
	memoryRecall2Button->SetToolTip (_ ("Recall memory register 2") );
	bigIntegerOperatorsGridSizer->Add (
	   memoryRecall2Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	divButton = new wxButton (operatorsPanel,
	                          ID_DIV_BUTTON,
	                          _ ("/"),
	                          wxDefaultPosition,
	                          wxSize (120, 32),
	                          0,
	                          wxDefaultValidator,
	                          _T ("ID_DIV_BUTTON") );
	divButton->SetToolTip (_ ("Compute a / b") );
	bigIntegerOperatorsGridSizer->Add (divButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	modButton = new wxButton (operatorsPanel,
	                          ID_MOD_BUTTON,
	                          _ ("mod"),
	                          wxDefaultPosition,
	                          wxSize (120, 32),
	                          0,
	                          wxDefaultValidator,
	                          _T ("ID_MOD_BUTTON") );
	modButton->SetToolTip (_ ("Compute a mod b") );
	bigIntegerOperatorsGridSizer->Add (modButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	gcdButton = new wxButton (operatorsPanel,
	                          ID_GCD_BUTTON,
	                          _ ("gcd(a,b)"),
	                          wxDefaultPosition,
	                          wxSize (120, 32),
	                          0,
	                          wxDefaultValidator,
	                          _T ("ID_GCD_BUTTON") );
	gcdButton->SetToolTip (_ ("Compute the greatest common divisor of a and b") );
	bigIntegerOperatorsGridSizer->Add (gcdButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	memoryClear1Button = new wxButton (operatorsPanel,
	                                   ID_MEMORY_CLEAR_1_BUTTON,
	                                   _ ("MC1"),
	                                   wxDefaultPosition,
	                                   wxSize (120, 32),
	                                   0,
	                                   wxDefaultValidator,
	                                   _T ("ID_MEMORY_CLEAR_1_BUTTON") );
	memoryClear1Button->SetToolTip (_ ("Clear memory register 1") );
	bigIntegerOperatorsGridSizer->Add (
	   memoryClear1Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	memoryClear2Button = new wxButton (operatorsPanel,
	                                   ID_MEMORY_CLEAR_2_BUTTON,
	                                   _ ("MC2"),
	                                   wxDefaultPosition,
	                                   wxSize (120, 32),
	                                   0,
	                                   wxDefaultValidator,
	                                   _T ("ID_MEMORY_CLEAR_2_BUTTON") );
	memoryClear2Button->SetToolTip (_ ("Clear memory register 2") );
	bigIntegerOperatorsGridSizer->Add (
	   memoryClear2Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	powerButton = new wxButton (operatorsPanel,
	                            ID_POWER_BUTTON,
	                            _ ("^"),
	                            wxDefaultPosition,
	                            wxSize (120, 32),
	                            0,
	                            wxDefaultValidator,
	                            _T ("ID_POWER_BUTTON") );
	powerButton->SetToolTip (_ ("Compute a ^ b") );
	bigIntegerOperatorsGridSizer->Add (
	   powerButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	power_mod_n_Button = new wxButton (operatorsPanel,
	                                   ID_POWER_MOD_N_BUTTON,
	                                   _ ("^ (mod n)"),
	                                   wxDefaultPosition,
	                                   wxSize (120, 32),
	                                   0,
	                                   wxDefaultValidator,
	                                   _T ("ID_POWER_MOD_N_BUTTON") );
	power_mod_n_Button->SetToolTip (_ ("Compute a ^ b (mod n)") );
	bigIntegerOperatorsGridSizer->Add (
	   power_mod_n_Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	leastCommonMultipleButton = new wxButton (operatorsPanel,
	      ID_LEAST_COMMON_MULTIPLE_BUTTON,
	      _ ("lcm(a,b)"),
	      wxDefaultPosition,
	      wxSize (120, 32),
	      0,
	      wxDefaultValidator,
	      _T ("ID_LEAST_COMMON_MULTIPLE_BUTTON") );
	leastCommonMultipleButton->SetToolTip (_ ("Compute the least common multiple of a and b") );
	bigIntegerOperatorsGridSizer->Add (
	   leastCommonMultipleButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);

	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);

	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);

	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);

	/*bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);*/

	BigIntegerCalculatorXbitIntegerButton = new wxButton( operatorsPanel, wxID_ANY, wxT("Random integer"), wxDefaultPosition, wxDefaultSize, 0 );
	BigIntegerCalculatorXbitIntegerButton->SetToolTip (
	   _ ("Compute an integer with the specified number of bits") );
	bigIntegerOperatorsGridSizer->Add (
	   BigIntegerCalculatorXbitIntegerButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);

	/*bigIntegerOperatorsGridSizer->Add (
	  -1, -1, 1, wxALL | wxEXPAND, 1);*/

	/*bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);*/

	BigIntegerCalculatorXbitIntegerTextCtrl = new wxTextCtrl( operatorsPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	bigIntegerOperatorsGridSizer->Add (
	   BigIntegerCalculatorXbitIntegerTextCtrl, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);

	inverse_mod_a_Button = new wxButton (operatorsPanel,
	                                     ID_INVERSE_MOD_N_BUTTON,
	                                     _ ("inv a (mod n)"),
	                                     wxDefaultPosition,
	                                     wxSize (120, 32),
	                                     0,
	                                     wxDefaultValidator,
	                                     _T ("ID_INVERSE_MOD_N_BUTTON") );
	inverse_mod_a_Button->SetToolTip (
	   _ ("Find the number d such that a*d = 1 (mod n). a and n must be relatively prime.") );
	bigIntegerOperatorsGridSizer->Add (
	   inverse_mod_a_Button, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	coprimeButton = new wxButton (operatorsPanel,
	                              ID_COPRIME_BUTTON,
	                              _ ("coprime(a)"),
	                              wxDefaultPosition,
	                              wxSize (120, 32),
	                              0,
	                              wxDefaultValidator,
	                              _T ("ID_COPRIME_BUTTON") );
	coprimeButton->SetToolTip (_ ("Find a random integer e relatively prime to integer a (gcd(e,a) = 1)") );
	bigIntegerOperatorsGridSizer->Add (
	   coprimeButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	resultMagnitudeButton = new wxButton (operatorsPanel,
	                                      ID_RESULT_MAGNITUDE_BUTTON,
	                                      _ ("Result Magnitude"),
	                                      wxDefaultPosition,
	                                      wxSize (128, 32),
	                                      0,
	                                      wxDefaultValidator,
	                                      _T ("ID_RESULT_MAGNITUDE_BUTTON") );
	resultMagnitudeButton->SetToolTip (_ ("Display the magnitude of the result") );
	bigIntegerOperatorsGridSizer->Add (
	   resultMagnitudeButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	spellResultButton = new wxButton (operatorsPanel,
	                                  ID_SPELL_RESULT_BUTTON,
	                                  _ ("Spell Result"),
	                                  wxDefaultPosition,
	                                  wxSize (120, 32),
	                                  0,
	                                  wxDefaultValidator,
	                                  _T ("ID_SPELL_RESULT_BUTTON") );
	spellResultButton->SetToolTip (_ ("Spell the result in English") );
	bigIntegerOperatorsGridSizer->Add (
	   spellResultButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	bigIntegerOperatorsGridSizer->Add (-1, -1, 1, wxALL | wxEXPAND | wxALIGN_LEFT | wxALIGN_BOTTOM, 1);
	operatorsPanel->SetSizer (bigIntegerOperatorsGridSizer);
	bigIntegerOperatorsGridSizer->Fit (operatorsPanel);
	bigIntegerOperatorsGridSizer->SetSizeHints (operatorsPanel);
	bigIntegerCalculatorPageFlexGridSizer->Add (
	   operatorsPanel, 1, wxALL | wxEXPAND, 1);
	numberTheoreticPanel->SetSizer (bigIntegerCalculatorPageFlexGridSizer);
	bigIntegerCalculatorPageFlexGridSizer->Fit (numberTheoreticPanel);
	bigIntegerCalculatorPageFlexGridSizer->SetSizeHints (numberTheoreticPanel);

	// COUNTING AND EMUMERATION PAGE

	countingAndEnumerationPanel = new wxScrolledWindow(
		discreteMathCalculatorNotebook,
		ID_COUNTING_AND_ENUMERATION_PANEL,
		wxDefaultPosition,
		wxDefaultSize,
		wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL,
		_T("ID_COUNTING_AND_ENUMERATION_PANEL")
);

	// Enable horizontal and vertical scrolling.
	countingAndEnumerationPanel->SetScrollRate(10, 10);
	countingAndEnumerationFlexGridSizer = new wxFlexGridSizer (6, 3, 0, 0);
	countingAndEnumerationFlexGridSizer->AddGrowableCol (1);
	// countingAndEnumerationFlexGridSizer->AddGrowableRow (3);
	countingAndEnumeration_n_StaticText = new wxStaticText (countingAndEnumerationPanel,
	      ID_COUNTING_AND_ENUMERATION_N_STATICTEXT,
	      _ ("n"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_COUNTING_AND_ENUMERATION_N_STATICTEXT") );
	countingAndEnumerationFlexGridSizer->Add (
	   countingAndEnumeration_n_StaticText, 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
	countingAndEnumeration_n_TextCtrl = new wxTextCtrl (countingAndEnumerationPanel,
	      ID_COUNTING_AND_ENUMERATION_N_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_N_TEXTCTRL") );
	countingAndEnumerationFlexGridSizer->Add (
	   countingAndEnumeration_n_TextCtrl, 1, wxALL | wxEXPAND, 2);
	countingAndEnumerationFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	countingAndEnumeration_k_StaticText = new wxStaticText (countingAndEnumerationPanel,
	      ID_COUNTING_AND_ENUMERATION_K_STATICTEXT,
	      _ ("k"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_COUNTING_AND_ENUMERATION_K_STATICTEXT") );
	countingAndEnumerationFlexGridSizer->Add (
	   countingAndEnumeration_k_StaticText, 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 2);
	countingAndEnumeration_k_TextCtrl = new wxTextCtrl (countingAndEnumerationPanel,
	      ID_COUNTING_AND_ENUMERATION_K_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_K_TEXTCTRL") );
	countingAndEnumerationFlexGridSizer->Add (
	   countingAndEnumeration_k_TextCtrl, 1, wxALL | wxEXPAND, 2);
	countingAndEnumerationFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	countingAndEnumeration_Result_StaticText = new wxStaticText (countingAndEnumerationPanel,
	      ID_COUNTING_END_ENUMERATION_RESULT_STATICTEXT,
	      _ ("Result"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_COUNTING_END_ENUMERATION_RESULT_STATICTEXT") );
	countingAndEnumerationFlexGridSizer->Add (
	   countingAndEnumeration_Result_StaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
#ifdef _WIN64
	countingAndEnumerationResultTextCtrl = new wxTextCtrl (countingAndEnumerationPanel,
	      ID_COUNTING_AND_ENUMERATION_RESULT_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTE_RICH,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_RESULT_TEXTCTRL") );
#else
	countingAndEnumerationResultTextCtrl = new wxTextCtrl (countingAndEnumerationPanel,
	      ID_COUNTING_AND_ENUMERATION_RESULT_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_RESULT_TEXTCTRL") );
#endif
	// countingAndEnumerationResultTextCtrl->SetMaxLength(32768);
	countingAndEnumerationFlexGridSizer->Add (countingAndEnumerationResultTextCtrl,
	      1,
	      wxALL | wxEXPAND,
	      2);
	countingAndEnumerationFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	countingAndEnumeration_result_StaticText = new wxStaticText (countingAndEnumerationPanel,
	      ID_COUNTING_AND_ENUMERATION_RESULT_STATICTEXT,
	      _ ("Listing"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_COUNTING_AND_ENUMERATION_RESULT_STATICTEXT") );
	countingAndEnumerationFlexGridSizer->Add (
	   countingAndEnumeration_result_StaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	countingAndEnumeration_listing_TextCtrl = new wxTextCtrl (countingAndEnumerationPanel,
	      ID_COUNTING_AND_ENUMERATION_COUNTING_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTE_MULTILINE | wxHSCROLL,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_COUNTING_TEXTCTRL") );
	wxFont countingAndEnumeration_listing_TextCtrlFont (
	   10, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, false, _T ("Courier New"), wxFONTENCODING_DEFAULT);
	countingAndEnumeration_listing_TextCtrl->SetFont (countingAndEnumeration_listing_TextCtrlFont);
	// Preserve sufficient height for the Listing area.
	countingAndEnumeration_listing_TextCtrl->SetMinSize(
    	wxSize(-1, 400));
	countingAndEnumeration_listing_TextCtrl->SetInitialSize(
    	wxSize(-1, 500));
	countingAndEnumerationFlexGridSizer->Add (countingAndEnumeration_listing_TextCtrl,
	      1,
	      wxALL | wxEXPAND,
	      2);
	countingAndEnumerationListButtonsPanel = new wxPanel (countingAndEnumerationPanel,
	      ID_COUNTING_AND_ENUMERATION_LIST_BUTTONS_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_COUNTING_AND_ENUMERATION_LIST_BUTTONS_PANEL") );
	FlexGridSizer1 = new wxFlexGridSizer (13, 1, 0, 0);
	countingAndEnumerationListPermutationsButton =
	   new wxButton (countingAndEnumerationListButtonsPanel,
	                 ID_COUNTING_AND_ENUMERATION_LIST_PERMUTATIONS_BUTTON,
	                 _ ("List Permutations"),
	                 wxDefaultPosition,
	                 wxDefaultSize,
	                 0,
	                 wxDefaultValidator,
	                 _T ("ID_COUNTING_AND_ENUMERATION_LIST_PERMUTATIONS_BUTTON") );
	countingAndEnumerationListPermutationsButton->SetToolTip (_ ("List all n! or P(n,k) orderings") );
	FlexGridSizer1->Add (
	   countingAndEnumerationListPermutationsButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 3);
	countingAndEnumerationListCombinationsButton =
	   new wxButton (countingAndEnumerationListButtonsPanel,
	                 ID_COUNTING_AND_ENUMERATION_LIST_COMBINATIONS_BUTTON,
	                 _ ("List Combinations"),
	                 wxDefaultPosition,
	                 wxDefaultSize,
	                 0,
	                 wxDefaultValidator,
	                 _T ("ID_COUNTING_AND_ENUMERATION_LIST_COMBINATIONS_BUTTON") );
	countingAndEnumerationListCombinationsButton->SetToolTip (_ ("List all combinations C(n,k)") );
	FlexGridSizer1->Add (
	   countingAndEnumerationListCombinationsButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 3);
	countingAndEnumerationListButtonsPanel->SetSizer (FlexGridSizer1);
	FlexGridSizer1->Fit (countingAndEnumerationListButtonsPanel);
	FlexGridSizer1->SetSizeHints (countingAndEnumerationListButtonsPanel);
	countingAndEnumerationFlexGridSizer->Add (countingAndEnumerationListButtonsPanel,
	      1,
	      wxALL | wxEXPAND,
	      2);
	countingAndEnumerationFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	countingAndEnumerationNumberComputationPanel =
	   new wxPanel (countingAndEnumerationPanel,
	                ID_COUNTING_AND_ENUMERATION_NUMBER_COMPUTATION_PANEL,
	                wxDefaultPosition,
	                wxDefaultSize,
	                wxTAB_TRAVERSAL,
	                _T ("ID_COUNTING_AND_ENUMERATION_NUMBER_COMPUTATION_PANEL") );
	countingAndEnumerationCountingGridSizer = new wxGridSizer (0, 8, 0, 0);
	countingAndEnumerationFactorialButton = new wxButton (countingAndEnumerationNumberComputationPanel,
	      ID_COUNTING_AND_ENUMERATION_FACTORIAL_BUTTON,
	      _ ("n!"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_FACTORIAL_BUTTON") );
	countingAndEnumerationFactorialButton->SetToolTip (_ ("Compute n factorial") );
	countingAndEnumerationCountingGridSizer->Add (
	   countingAndEnumerationFactorialButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	countingAndEnumerationPermutationsButton = new wxButton (countingAndEnumerationNumberComputationPanel,
	      ID_COUNTING_AND_ENUMERATION_PERMUTATIONS_BUTTON,
	      _ ("P(n,k)"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_PERMUTATIONS_BUTTON") );
	countingAndEnumerationPermutationsButton->SetToolTip (_ ("Compute the permutation of n items\ntaken k at a time") );
	countingAndEnumerationCountingGridSizer->Add (
	   countingAndEnumerationPermutationsButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	countingAndEnumerationCombinationsButton = new wxButton (countingAndEnumerationNumberComputationPanel,
	      ID_COUNTING_AND_ENUMERATION_COMBINATIONS_BUTTON,
	      _ ("C(n,k)"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_COMBINATIONS_BUTTON") );
	countingAndEnumerationCombinationsButton->SetToolTip (
	   _ ("Compute the number of combinations of n items\ntaken k at a time") );
	countingAndEnumerationCountingGridSizer->Add (
	   countingAndEnumerationCombinationsButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	countingAndEnumerationFibonacciButton = new wxButton (countingAndEnumerationNumberComputationPanel,
	      ID_COUNTING_AND_ENUMERATION_FIBONACCI_BUTTON,
	      _ ("Fibonacci(n)"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_FIBONACCI_BUTTON") );
	countingAndEnumerationFibonacciButton->SetToolTip (_ ("Compute Fibonacci number n") );
	countingAndEnumerationCountingGridSizer->Add (
	   countingAndEnumerationFibonacciButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	countingAndEnumerationCatalanButton = new wxButton (countingAndEnumerationNumberComputationPanel,
	      ID_COUNTING_AND_ENUMERATION_CATALAN_BUTTON,
	      _ ("C(n)"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_CATALAN_BUTTON") );
	countingAndEnumerationCatalanButton->SetToolTip (
	   _ ("Compute Catalan number n - the number of full binary trees with n + 1 leaves and other applications") );
	countingAndEnumerationCountingGridSizer->Add (
	   countingAndEnumerationCatalanButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	countingAndEnumerationSchroderButton = new wxButton (countingAndEnumerationNumberComputationPanel,
	      ID_COUNTING_AND_ENUMERATION_SCHRODER_BUTTON,
	      _ ("S(n)"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_SCHRODER_BUTTON") );
	countingAndEnumerationSchroderButton->SetToolTip (
	   _ ("Compute Schroder number n - the number of lattice paths in the Cartesian plane that start at (0, 0), end at "
	      "(n,n), contain no points above the line y=x, and are composed only of steps (0, 1), (1, 0), and (1, 1)") );
	countingAndEnumerationCountingGridSizer->Add (
	   countingAndEnumerationSchroderButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	countingAndEnumerationBellButton = new wxButton (countingAndEnumerationNumberComputationPanel,
	      ID_COUNTING_AND_ENUMERATION_BELL_BUTTON,
	      _ ("Bell(n)"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_BELL_BUTTON") );
	countingAndEnumerationBellButton->SetToolTip (
	   _ ("Compute Bell number of n - the number of partitions of a set with n elements") );
	countingAndEnumerationCountingGridSizer->Add (
	   countingAndEnumerationBellButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	countingAndEnumerationStirlingButton = new wxButton (countingAndEnumerationNumberComputationPanel,
	      ID_COUNTING_AND_ENUMERATION_STIRLING_BUTTON,
	      _ ("S(n,k)"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_STIRLING_BUTTON") );
	countingAndEnumerationStirlingButton->SetToolTip (_ ("Compute the Stirling number of the second kind - the number of "
	      "ways to partition an n-element set into k subsets.") );
	countingAndEnumerationCountingGridSizer->Add (
	   countingAndEnumerationStirlingButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	countingAndEnumerationNumberComputationPanel->SetSizer (countingAndEnumerationCountingGridSizer);
	countingAndEnumerationCountingGridSizer->Fit (countingAndEnumerationNumberComputationPanel);
	countingAndEnumerationCountingGridSizer->SetSizeHints (countingAndEnumerationNumberComputationPanel);
	countingAndEnumerationFlexGridSizer->Add (countingAndEnumerationNumberComputationPanel,
	      1,
	      wxALL | wxEXPAND,
	      1);
	countingAndEnumerationFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	countingAndEnumerationFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	countingAndEnumerationEstimatePanel = new wxPanel (countingAndEnumerationPanel,
	      ID_COUNTING_AND_ENUMERATION_ESTIMATE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_COUNTING_AND_ENUMERATION_ESTIMATE_PANEL") );
	countingAndEnumerationMagnitudeGridSizer = new wxGridSizer (0, 4, 0, 0);
	countingAndEnumerationMagnitudeGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	countingAndEnumerationResultMagnitudeButton =
	   new wxButton (countingAndEnumerationEstimatePanel,
	                 ID_COUNTING_AND_ENUMERATION_RESULT_MAGNITUDE_BUTTON,
	                 _ ("Result Magnitude"),
	                 wxDefaultPosition,
	                 wxSize (-1, -1),
	                 0,
	                 wxDefaultValidator,
	                 _T ("ID_COUNTING_AND_ENUMERATION_RESULT_MAGNITUDE_BUTTON") );
	countingAndEnumerationResultMagnitudeButton->SetToolTip (_ ("Display the magnitude of the result") );
	countingAndEnumerationMagnitudeGridSizer->Add (
	   countingAndEnumerationResultMagnitudeButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	countingAndEnumerationAsymptoticEstimateButton =
	   new wxButton (countingAndEnumerationEstimatePanel,
	                 ID_COUNTING_AND_ENUMERATION_ASYMPTOTIC_ESTIMATE_BUTTON,
	                 _ ("Asymptotic Estimate"),
	                 wxDefaultPosition,
	                 wxSize (-1, -1),
	                 0,
	                 wxDefaultValidator,
	                 _T ("ID_COUNTING_AND_ENUMERATION_ASYMPTOTIC_ESTIMATE_BUTTON") );
	countingAndEnumerationAsymptoticEstimateButton->SetToolTip (_ ("Estimate the value of very large numbers") );
	countingAndEnumerationMagnitudeGridSizer->Add (countingAndEnumerationAsymptoticEstimateButton,
	      1,
	      wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	      1);
	countingAndEnumerationMagnitudeGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	countingAndEnumerationEstimatePanel->SetSizer (countingAndEnumerationMagnitudeGridSizer);
	countingAndEnumerationMagnitudeGridSizer->Fit (countingAndEnumerationEstimatePanel);
	countingAndEnumerationMagnitudeGridSizer->SetSizeHints (countingAndEnumerationEstimatePanel);
	countingAndEnumerationFlexGridSizer->Add (countingAndEnumerationEstimatePanel,
	      1,
	      wxALL | wxEXPAND,
	      2);
	countingAndEnumerationFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
// Assign the layout to the scrollable page.
countingAndEnumerationPanel->SetSizer(
    countingAndEnumerationFlexGridSizer
);

	// Determine the space required by all controls.
	wxSize requiredSize =
		countingAndEnumerationFlexGridSizer->CalcMin();
// Set the virtual area to the required dimensions.
    countingAndEnumerationPanel->SetVirtualSize(requiredSize);

    // Enable scrolling in both directions.
	countingAndEnumerationPanel->SetScrollRate(10, 10);

	// TRIANGLES PAGE

	trianglesPanel = new wxPanel (discreteMathCalculatorNotebook,
	                              ID_TRIANGLES_PANEL,
	                              wxDefaultPosition,
	                              wxDefaultSize,
	                              wxTAB_TRAVERSAL,
	                              _T ("ID_TRIANGLES_PANEL") );
	trianglesFlexGridSizer = new wxFlexGridSizer(4, 1, 0, 0);
	trianglesFlexGridSizer->AddGrowableCol (0);
	trianglesFlexGridSizer->AddGrowableRow (1);
	trianglesDataPanel = new wxPanel (trianglesPanel,
	                                  ID_TRIANGLES_DATA_PANEL,
	                                  wxDefaultPosition,
	                                  wxDefaultSize,
	                                  wxTAB_TRAVERSAL,
	                                  _T ("ID_TRIANGLES_DATA_PANEL") );
	trianglesDataFlexGridSizer = new wxFlexGridSizer (0, 3, 0, 0);
	trianglesDataFlexGridSizer->AddGrowableCol (2);
	trianglesDataStaticText = new wxStaticText (trianglesDataPanel,
	      ID_TRIANGLES_DATA_STATICTEXT,
	      _ ("n"),
	      wxDefaultPosition,
	      wxSize (40, 20),
	      wxALIGN_CENTRE,
	      _T ("ID_TRIANGLES_DATA_STATICTEXT") );
	trianglesDataFlexGridSizer->Add (
	   trianglesDataStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	trianglesDataTextCtrl = new wxTextCtrl (trianglesDataPanel,
	                                        ID_TRIANGLES_DATA_TEXTCTRL,
	                                        wxEmptyString,
	                                        wxDefaultPosition,
	                                        wxSize(75,-1),
	                                        0,
	                                        wxDefaultValidator,
	                                        _T ("ID_TRIANGLES_DATA_TEXTCTRL") );
	trianglesDataFlexGridSizer->Add (
	   trianglesDataTextCtrl, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	trianglesFlexGridSizer->Add(
    	trianglesDataPanel, 0, wxALL | wxEXPAND, 2);
	trianglesDataPanel->SetSizer (trianglesDataFlexGridSizer);
	trianglesDataFlexGridSizer->Fit (trianglesDataPanel);
	trianglesDataFlexGridSizer->SetSizeHints (trianglesDataPanel);
	trianglesOutputTextCtrl = new wxTextCtrl (trianglesPanel,
	      ID_TRIANGLES_OUTPUT_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTE_MULTILINE | wxHSCROLL,
	      wxDefaultValidator,
	      _T ("ID_TRIANGLES_OUTPUT_TEXTCTRL") );
	wxFont trianglesOutputTextCtrlFont (
		14, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL,
		false, _T("Courier New"), wxFONTENCODING_DEFAULT);
	trianglesOutputTextCtrl->SetFont (trianglesOutputTextCtrlFont);
	trianglesFlexGridSizer->Add (
	   trianglesOutputTextCtrl, 1, wxALL | wxEXPAND, 2);
	trianglesButtonsPanel = new wxPanel (trianglesPanel,
	                                     ID_TRIANGLES_BUTTONS_PANEL,
	                                     wxDefaultPosition,
	                                     wxDefaultSize,
	                                     wxTAB_TRAVERSAL,
	                                     _T ("ID_TRIANGLES_BUTTONS_PANEL") );
	trianglesButtonsGridSizer = new wxGridSizer(1, 5, 0, 5);
	triangelsPascalButton = new wxButton (trianglesButtonsPanel,
	                                      ID_TRIANGLES_PASCAL_BUTTON,
	                                      _ ("Pascal"),
	                                      wxDefaultPosition,
	                                      wxDefaultSize,
	                                      0,
	                                      wxDefaultValidator,
	                                      _T ("ID_TRIANGLES_PASCAL_BUTTON") );
	triangelsPascalButton->SetToolTip (
	   _ ("Display the Pascal triangle - C(m,n) = C(m-1,n-1) + C(m-1,n),\nC(0,0) = 1, C(m,m) = 1") );
	trianglesButtonsGridSizer->Add (
	   triangelsPascalButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	trianglesHosoyaButton = new wxButton (trianglesButtonsPanel,
	                                      ID_TRIANGLES_HOSOYA_BUTTON,
	                                      _ ("Hosoya"),
	                                      wxDefaultPosition,
	                                      wxDefaultSize,
	                                      0,
	                                      wxDefaultValidator,
	                                      _T ("ID_TRIANGLES_HOSOYA_BUTTON") );
	trianglesHosoyaButton->SetToolTip (_ ("H(n,i) = F(i+1) * F(n-i+1), where F (k) is Fibonacci number k.\nThe first "
	                                      "column and the diagonal are Fibonacci numbers") );
	trianglesButtonsGridSizer->Add (
	   trianglesHosoyaButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	trianlgesCatalanButton = new wxButton (trianglesButtonsPanel,
	                                       ID_TRIANGLES_CATALAN_BUTTON,
	                                       _ ("Catalan"),
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       0,
	                                       wxDefaultValidator,
	                                       _T ("ID_TRIANGLES_CATALAN_BUTTON") );
	trianlgesCatalanButton->SetToolTip (
	   _ ("Each element is equal to the one above plus the one to the left. The sum of each row is equal to the last "
	      "element of the next row and also equal to the Catalan number C(n)") );
	trianglesButtonsGridSizer->Add (
	   trianlgesCatalanButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	trianglesSchroderButton = new wxButton (trianglesButtonsPanel,
	                                        ID_TRIANGLES_SCHRODER_BUTTON,
	                                        _ ("Schroder"),
	                                        wxDefaultPosition,
	                                        wxDefaultSize,
	                                        0,
	                                        wxDefaultValidator,
	                                        _T ("ID_TRIANGLES_SCHRODER_BUTTON") );
	trianglesSchroderButton->SetToolTip (_ ("S(n,k)=S(n,k-1)+S(n-1,k-1)+S(n-1,k),  n,k >= 0; (n,k) != (0,0),\nS(n,k) = 0, "
	                                        "k < 0 or n < 0. The diagonal is the sequence of Schroder numbers.") );
	trianglesButtonsGridSizer->Add (
	   trianglesSchroderButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	trianglesBellButton = new wxButton (trianglesButtonsPanel,
	                                    ID_TRIANGLES_BELL_BUTTON,
	                                    _ ("Bell"),
	                                    wxDefaultPosition,
	                                    wxDefaultSize,
	                                    0,
	                                    wxDefaultValidator,
	                                    _T ("ID_TRIANGLES_BELL_BUTTON") );
	trianglesBellButton->SetToolTip (
	   _ ("Begin the first row with the number one, and begin subsequent rows with last number of the previous row. "
	      "Rows are filled out by adding the number in the preceding column to the number above it. The diagonal "
	      "contains the sequence of Bell numbers.") );
	trianglesButtonsGridSizer->Add (
	   trianglesBellButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	trianglesButtonsPanel->SetSizer(trianglesButtonsGridSizer);

	trianglesButtonsPanel->SetMinSize(
    	wxSize(-1, trianglesButtonsGridSizer->CalcMin().GetHeight())
	);
	trianglesFlexGridSizer->Add(
		trianglesButtonsPanel,
		0,
		wxLEFT | wxRIGHT | wxTOP | wxEXPAND,
		2
	);

trianglesFlexGridSizer->AddSpacer(20);
	trianglesPanel->SetSizer (trianglesFlexGridSizer);
	// SUBSETS PAGE

	subsetsPanel = new wxPanel (discreteMathCalculatorNotebook,
	                            ID_SUBSETS_PANEL,
	                            wxDefaultPosition,
	                            wxDefaultSize,
	                            wxTAB_TRAVERSAL,
	                            _T ("ID_SUBSETS_PANEL") );
	subsetsFlexGridSizer = new wxFlexGridSizer (7, 1, 0, 0);
	subsetsFlexGridSizer->AddGrowableCol (0);
	subsetsFlexGridSizer->AddGrowableRow (3);
	subsetsCompleteSizeKPanel = new wxPanel (subsetsPanel,
	      ID_SUBSETS_COMPLETE_SIZE_K_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_SUBSETS_COMPLETE_SIZE_K_PANEL") );
	subsetsSizeKBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	subsetsSetSizeKPanel = new wxPanel (subsetsCompleteSizeKPanel,
	                                    ID_SUBSETS_SIZE_K_PANEL,
	                                    wxDefaultPosition,
	                                    wxDefaultSize,
	                                    wxTAB_TRAVERSAL,
	                                    _T ("ID_SUBSETS_SIZE_K_PANEL") );
	subsetsSetSizeKFlexGridSizer = new wxFlexGridSizer (0, 4, 0, 0);
	subsetsSetSizeKFlexGridSizer->AddGrowableCol (1);
	subsetsSetSizeKFlexGridSizer->AddGrowableCol (3);
	subsetsSetSizeStaticText = new wxStaticText (subsetsSetSizeKPanel,
	      ID_SUBSETS_SET_SIZE_STATICTEXT,
	      _ ("Set Size"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_SUBSETS_SET_SIZE_STATICTEXT") );
	subsetsSetSizeKFlexGridSizer->Add (
	   subsetsSetSizeStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	subsetsSetSizeTextCtrl = new wxTextCtrl (subsetsSetSizeKPanel,
	      ID_SUBSETS_SET_SIZE_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_SUBSETS_SET_SIZE_TEXTCTRL") );
	subsetsSetSizeKFlexGridSizer->Add (
	   subsetsSetSizeTextCtrl, 1, wxALL | wxEXPAND, 1);
	subsetsKStaticText = new wxStaticText (subsetsSetSizeKPanel,
	                                       ID_SUBSETS_K_STATICTEXT,
	                                       _ ("K"),
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       0,
	                                       _T ("ID_SUBSETS_K_STATICTEXT") );
	subsetsSetSizeKFlexGridSizer->Add (
	   subsetsKStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	subsetsKTextCtrl = new wxTextCtrl (subsetsSetSizeKPanel,
	                                   ID_SUBSETS_K_TEXTCTRL,
	                                   wxEmptyString,
	                                   wxDefaultPosition,
	                                   wxDefaultSize,
	                                   0,
	                                   wxDefaultValidator,
	                                   _T ("ID_SUBSETS_K_TEXTCTRL") );
	subsetsSetSizeKFlexGridSizer->Add (
	   subsetsKTextCtrl, 1, wxALL | wxEXPAND, 1);
	subsetsSetSizeKPanel->SetSizer (subsetsSetSizeKFlexGridSizer);
	subsetsSetSizeKFlexGridSizer->Fit (subsetsSetSizeKPanel);
	subsetsSetSizeKFlexGridSizer->SetSizeHints (subsetsSetSizeKPanel);
	subsetsSizeKBoxSizer->Add (subsetsSetSizeKPanel, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	subsetsSizeKBoxSizer->Add (-1, -1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	subsetsCompleteSizeKPanel->SetSizer (subsetsSizeKBoxSizer);
	subsetsSizeKBoxSizer->Fit (subsetsCompleteSizeKPanel);
	subsetsSizeKBoxSizer->SetSizeHints (subsetsCompleteSizeKPanel);
	subsetsFlexGridSizer->Add (
	   subsetsCompleteSizeKPanel, 1, wxALL | wxEXPAND, 1);
	subsetsNumberOfSubsetsPanel = new wxPanel (subsetsPanel,
	      ID_SUBSETS_NUMBER_OF_SUBSSETS_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_SUBSETS_NUMBER_OF_SUBSSETS_PANEL") );
	subsetsNumberOfSubsetsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	subsetsNumberOfSubsetsStaticText = new wxStaticText (subsetsNumberOfSubsetsPanel,
	      ID_SUBSETS_NUMBER_OF_SUBSETS_STATICTEXT,
	      _ ("Subsets"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxALIGN_RIGHT,
	      _T ("ID_SUBSETS_NUMBER_OF_SUBSETS_STATICTEXT") );
	subsetsNumberOfSubsetsBoxSizer->Add (
	   subsetsNumberOfSubsetsStaticText, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	subsetsNumberofSubsetsTextCtrl = new wxTextCtrl (subsetsNumberOfSubsetsPanel,
	      ID_SUBSETS_NUMBER_OF_SUBSETS_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_SUBSETS_NUMBER_OF_SUBSETS_TEXTCTRL") );
	subsetsNumberOfSubsetsBoxSizer->Add (
	   subsetsNumberofSubsetsTextCtrl, 6, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	subsetsNumberOfSubsetsPanel->SetSizer (subsetsNumberOfSubsetsBoxSizer);
	subsetsNumberOfSubsetsBoxSizer->Fit (subsetsNumberOfSubsetsPanel);
	subsetsNumberOfSubsetsBoxSizer->SetSizeHints (subsetsNumberOfSubsetsPanel);
	subsetsFlexGridSizer->Add (
	   subsetsNumberOfSubsetsPanel, 1, wxALL | wxEXPAND, 1);
	subsetsNumberOfKElementSubsetsPanel = new wxPanel (subsetsPanel,
	      ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_PANEL") );
	subsetsNumberOfKElementSubsetsBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	subsetsNumberOfKElementSubsetsStaticText =
	   new wxStaticText (subsetsNumberOfKElementSubsetsPanel,
	                     ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_STATICTEXT,
	                     _ ("K Element Subsets"),
	                     wxDefaultPosition,
	                     wxDefaultSize,
	                     wxALIGN_RIGHT,
	                     _T ("ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_STATICTEXT") );
	subsetsNumberOfKElementSubsetsBoxSizer->Add (
	   subsetsNumberOfKElementSubsetsStaticText, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	subsetsNumberOfKElementSubsetsTextCtrl = new wxTextCtrl (subsetsNumberOfKElementSubsetsPanel,
	      ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_TEXTCTRL") );
	subsetsNumberOfKElementSubsetsBoxSizer->Add (
	   subsetsNumberOfKElementSubsetsTextCtrl, 6, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	subsetsNumberOfKElementSubsetsPanel->SetSizer (subsetsNumberOfKElementSubsetsBoxSizer);
	subsetsNumberOfKElementSubsetsBoxSizer->Fit (subsetsNumberOfKElementSubsetsPanel);
	subsetsNumberOfKElementSubsetsBoxSizer->SetSizeHints (subsetsNumberOfKElementSubsetsPanel);
	subsetsFlexGridSizer->Add (subsetsNumberOfKElementSubsetsPanel,
	                           1,
	                           wxALL | wxEXPAND,
	                           5);
	subsetsListingsTextCtrl = new wxTextCtrl (subsetsPanel,
	      ID_SUBSETS_LISTINGS_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTE_MULTILINE | wxHSCROLL,
	      wxDefaultValidator,
	      _T ("ID_SUBSETS_LISTINGS_TEXTCTRL") );
	wxFont subsetsListingsTextCtrlFont (
		14, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
		wxFONTWEIGHT_NORMAL, false,
		_T("Courier New"), wxFONTENCODING_DEFAULT);
	subsetsListingsTextCtrl->SetFont (subsetsListingsTextCtrlFont);
	subsetsFlexGridSizer->Add (
	   subsetsListingsTextCtrl, 1, wxALL | wxEXPAND, 5);
	subsetsButtonsPanel = new wxPanel (subsetsPanel,
	                                   ID_SUBSETS_BUTTONS_PANEL,
	                                   wxDefaultPosition,
	                                   wxDefaultSize,
	                                   wxTAB_TRAVERSAL,
	                                   _T ("ID_SUBSETS_BUTTONS_PANEL") );
	subsetsButtonsGridSizer = new wxGridSizer (0, 4, 0, 0);
	subsetsNumberOfSubsetsButton = new wxButton (subsetsButtonsPanel,
	      ID_SUBSETS_NUMBER_OF_SUBSETS_BUTTON,
	      _ ("Number of Subsets"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_SUBSETS_NUMBER_OF_SUBSETS_BUTTON") );
	subsetsNumberOfSubsetsButton->SetToolTip (_ ("The number of elements in the powerset") );
	subsetsButtonsGridSizer->Add (
	   subsetsNumberOfSubsetsButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	subsetsNumberOfKElementSubsetsButton = new wxButton (subsetsButtonsPanel,
	      ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_BUTTON,
	      _ ("Number of K-Element Subsets"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_BUTTON") );
	subsetsNumberOfKElementSubsetsButton->SetToolTip (_ ("The number of K-element subsets in the powerset") );
	subsetsButtonsGridSizer->Add (
	   subsetsNumberOfKElementSubsetsButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	subsetsListAllSubsetsButton = new wxButton (subsetsButtonsPanel,
	      ID_SUBSETS_LIST_ALL_SUBSETS_BUTTON,
	      _ ("List All Subsets"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_SUBSETS_LIST_ALL_SUBSETS_BUTTON") );
	subsetsListAllSubsetsButton->SetToolTip (_ ("List all subsets of the set. Limited to 5000 lines.") );
	subsetsButtonsGridSizer->Add (
	   subsetsListAllSubsetsButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	subsetsListAllKElementSubsetsButton = new wxButton (subsetsButtonsPanel,
	      ID_SUBSETS_LIST_ALL_K_ELEMENT_SUBSETS_BUTTON,
	      _ ("List All K-Element Subsets"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_SUBSETS_LIST_ALL_K_ELEMENT_SUBSETS_BUTTON") );
	subsetsListAllKElementSubsetsButton->SetToolTip (
	   _ ("List all K-element subsets of the powerset. Limited to 5000 lines.") );
	subsetsButtonsGridSizer->Add (
	   subsetsListAllKElementSubsetsButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	subsetsButtonsPanel->SetSizer (subsetsButtonsGridSizer);
	subsetsButtonsGridSizer->Fit (subsetsButtonsPanel);
	subsetsButtonsGridSizer->SetSizeHints (subsetsButtonsPanel);
	subsetsFlexGridSizer->Add (
	   subsetsButtonsPanel, 1, wxALL | wxEXPAND, 5);
	subsetsEstimatePanel = new wxPanel (subsetsPanel,
	                                    ID_SUBSETS_ESTIMATE_PANEL,
	                                    wxDefaultPosition,
	                                    wxDefaultSize,
	                                    wxTAB_TRAVERSAL,
	                                    _T ("ID_SUBSETS_ESTIMATE_PANEL") );
	subsetsMagnitudeGridSizer = new wxGridSizer (0, 4, 0, 0);
	subsetsMagnitudeGridSizer->Add (-1, -1, 1, wxALL | wxEXPAND, 1);
	subsetsResultMagnitudeButton = new wxButton (subsetsEstimatePanel,
	      ID_SUBSETS_RESULT_MAGNITUDE_BUTTON,
	      _ ("Result Magnitude"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_SUBSETS_RESULT_MAGNITUDE_BUTTON") );
	subsetsResultMagnitudeButton->SetToolTip (_ ("Display the magnitude of the result") );
	subsetsMagnitudeGridSizer->Add (
	   subsetsResultMagnitudeButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	subsetsAsymptoticEstimateButton = new wxButton (subsetsEstimatePanel,
	      ID_SUBSETS_ASYMPTOTIC_ESTIMATE_BUTTON,
	      _ ("Asymptotic Estimate"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_SUBSETS_ASYMPTOTIC_ESTIMATE_BUTTON") );
	subsetsAsymptoticEstimateButton->SetToolTip (_ ("Estimate the value of very large numbers") );
	subsetsMagnitudeGridSizer->Add (
	   subsetsAsymptoticEstimateButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	subsetsMagnitudeGridSizer->Add (-1, -1, 1, wxALL | wxEXPAND, 1);
	subsetsEstimatePanel->SetSizer (subsetsMagnitudeGridSizer);
	subsetsMagnitudeGridSizer->Fit (subsetsEstimatePanel);
	subsetsMagnitudeGridSizer->SetSizeHints (subsetsEstimatePanel);
	subsetsFlexGridSizer->Add (
	   subsetsEstimatePanel, 1, wxALL | wxEXPAND, 10);
	subsetsFlexGridSizer->Add (-1, -1, 1, wxALL | wxEXPAND, 1);
	subsetsPanel->SetSizer (subsetsFlexGridSizer);
	subsetsFlexGridSizer->Fit (subsetsPanel);
	subsetsFlexGridSizer->SetSizeHints (subsetsPanel);

	// PARTITIONS PAGE

	partitionsPanel = new wxPanel (discreteMathCalculatorNotebook,
	                               ID_PARTITONS_PANEL,
	                               wxDefaultPosition,
	                               wxDefaultSize,
	                               wxTAB_TRAVERSAL,
	                               _T ("ID_PARTITONS_PANEL") );
	partitionsFlexGridSizer = new wxFlexGridSizer (8, 1, 0, 0);
	partitionsFlexGridSizer->AddGrowableCol (0);
	partitionsFlexGridSizer->AddGrowableRow (4);
	partitions_n_k_Panel = new wxPanel (partitionsPanel,
	                                    ID_PARTITIONS_N_K_PANEL,
	                                    wxDefaultPosition,
	                                    wxDefaultSize,
	                                    wxTAB_TRAVERSAL,
	                                    _T ("ID_PARTITIONS_N_K_PANEL") );
	partitions_n_k_FlexGridSizer = new wxFlexGridSizer (0, 5, 0, 0);
	partitions_n_k_FlexGridSizer->AddGrowableCol (4);
	partitions_n_k_FlexGridSizer->AddGrowableRow (0);
	partitions_n_StaticText = new wxStaticText (partitions_n_k_Panel,
	      ID_PARTITIONS_N_STATICTEXT,
	      _ ("n"),
	      wxDefaultPosition,
	      wxSize (42, 20),
	      wxALIGN_RIGHT | wxALIGN_CENTRE,
	      _T ("ID_PARTITIONS_N_STATICTEXT") );
	partitions_n_k_FlexGridSizer->Add (partitions_n_StaticText, 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);
	partitions_n_TextCtrl = new wxTextCtrl (partitions_n_k_Panel,
	                                        ID_PARTITIONS_N_TEXTCTRL,
	                                        wxEmptyString,
	                                        wxDefaultPosition,
	                                        wxSize(75,-1),
	                                        0,
	                                        wxDefaultValidator,
	                                        _T ("ID_PARTITIONS_N_TEXTCTRL") );
	partitions_n_k_FlexGridSizer->Add (
	   partitions_n_TextCtrl, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	partitions_k_StaticText = new wxStaticText (partitions_n_k_Panel,
	      ID_PARTITIONS_K_STATICTEXT,
	      _ ("k"),
	      wxDefaultPosition,
	      wxSize(-1,-1),
	      wxALIGN_RIGHT,
	      _T ("ID_PARTITIONS_K_STATICTEXT") );
	partitions_n_k_FlexGridSizer->Add (partitions_k_StaticText, 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
	partitions_k_TextCtrl = new wxTextCtrl (partitions_n_k_Panel,
	                                        ID_PARTITIONS_K_TEXTCTRL,
	                                        wxEmptyString,
	                                        wxDefaultPosition,
	                                        wxSize(75,-1),
	                                        0,
	                                        wxDefaultValidator,
	                                        _T ("ID_PARTITIONS_K_TEXTCTRL") );
	partitions_n_k_FlexGridSizer->Add (
	   partitions_k_TextCtrl, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	partitions_n_k_FlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	partitions_n_k_Panel->SetSizer (partitions_n_k_FlexGridSizer);
	partitions_n_k_FlexGridSizer->Fit (partitions_n_k_Panel);
	partitions_n_k_FlexGridSizer->SetSizeHints (partitions_n_k_Panel);
	partitionsFlexGridSizer->Add (
	   partitions_n_k_Panel, 1, wxALL | wxEXPAND, 1);
	partitionsBellNumberPanel = new wxPanel (partitionsPanel,
	      ID_PARTITIONS_BELL_NUMBER_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_PARTITIONS_BELL_NUMBER_PANEL") );
	partitionsBellNumberFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	partitionsBellNumberFlexGridSizer->AddGrowableCol (1);
	partitionsBellNumberStaticText = new wxStaticText (partitionsBellNumberPanel,
	      ID_PARTITONS_BELL_NUMBER_STATICTEXT,
	      _ ("Bell number"),
	      wxDefaultPosition,
	      wxSize (100, 20),
	      wxALIGN_LEFT,
	      _T ("ID_PARTITONS_BELL_NUMBER_STATICTEXT") );
	partitionsBellNumberFlexGridSizer->Add (
	   partitionsBellNumberStaticText, 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
	partitionsBellNumberTextCtrl = new wxTextCtrl (partitionsBellNumberPanel,
	      ID_PARTITIONS_BELL_NUMBER_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_PARTITIONS_BELL_NUMBER_TEXTCTRL") );
	partitionsBellNumberFlexGridSizer->Add (
	   partitionsBellNumberTextCtrl, 1, wxALL | wxEXPAND, 5);
	partitionsBellNumberPanel->SetSizer (partitionsBellNumberFlexGridSizer);
	partitionsBellNumberFlexGridSizer->Fit (partitionsBellNumberPanel);
	partitionsBellNumberFlexGridSizer->SetSizeHints (partitionsBellNumberPanel);
	partitionsFlexGridSizer->Add (
	   partitionsBellNumberPanel, 1, wxALL | wxEXPAND, 1);
	partitionsStirlingPanel = new wxPanel (partitionsPanel,
	                                       ID_PARTITIONS_STIRLING_PANEL,
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       wxTAB_TRAVERSAL,
	                                       _T ("ID_PARTITIONS_STIRLING_PANEL") );
	partitionsStirlingFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	partitionsStirlingFlexGridSizer->AddGrowableCol (1);
	partitonsStirlingNumberStaticText = new wxStaticText (partitionsStirlingPanel,
	      ID_PARTITIONS_STIRLING_NUMBER_STATICTEXT,
	      _ ("Stirling number of the second kind"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_PARTITIONS_STIRLING_NUMBER_STATICTEXT") );
	partitionsStirlingFlexGridSizer->Add (
	   partitonsStirlingNumberStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	partitionsStirlingNumberTextCtrl = new wxTextCtrl (partitionsStirlingPanel,
	      ID_PARTITIONS_STIRLING_NUMBER_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_PARTITIONS_STIRLING_NUMBER_TEXTCTRL") );
	partitionsStirlingFlexGridSizer->Add (
	   partitionsStirlingNumberTextCtrl, 1, wxALL | wxEXPAND, 5);
	partitionsStirlingPanel->SetSizer (partitionsStirlingFlexGridSizer);
	partitionsStirlingFlexGridSizer->Fit (partitionsStirlingPanel);
	partitionsStirlingFlexGridSizer->SetSizeHints (partitionsStirlingPanel);
	partitionsFlexGridSizer->Add (
	   partitionsStirlingPanel, 1, wxALL | wxEXPAND, 1);
	partitionsListingLabelPanel = new wxPanel (partitionsPanel,
	      ID_PARTITIONS_LISTING_LABEL_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_PARTITIONS_LISTING_LABEL_PANEL") );
	partitionsListingLabelFlexGridSizer = new wxFlexGridSizer (0, 3, 0, 0);
	partitionsListingLabelFlexGridSizer->AddGrowableCol (1);
	partitionsListingLabelFlexGridSizer->Add (-1, -1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	partitionsListingStaticText = new wxStaticText (partitionsListingLabelPanel,
	      ID_PARTITIONS_LISTING_STATICTEXT,
	      _ ("Listing"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_PARTITIONS_LISTING_STATICTEXT") );
	partitionsListingLabelFlexGridSizer->Add (
	   partitionsListingStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	partitionsListingLabelFlexGridSizer->Add (-1, -1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	partitionsListingLabelPanel->SetSizer (partitionsListingLabelFlexGridSizer);
	partitionsListingLabelFlexGridSizer->Fit (partitionsListingLabelPanel);
	partitionsListingLabelFlexGridSizer->SetSizeHints (partitionsListingLabelPanel);
	partitionsFlexGridSizer->Add (
	   partitionsListingLabelPanel, 1, wxALL | wxEXPAND, 5);
	partitonsListingTextCtrl = new wxTextCtrl (partitionsPanel,
	      ID_PARTITIONS_LISTING_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTE_MULTILINE | wxHSCROLL,
	      wxDefaultValidator,
	      _T ("ID_PARTITIONS_LISTING_TEXTCTRL") );
	wxFont partitonsListingTextCtrlFont (
	   14, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, false, _T ("Times New Roman"), wxFONTENCODING_DEFAULT);
	partitonsListingTextCtrl->SetFont (partitonsListingTextCtrlFont);
	partitionsFlexGridSizer->Add (
	   partitonsListingTextCtrl, 1, wxALL | wxEXPAND, 5);
	partitionsButtonsPanel = new wxPanel (partitionsPanel,
	                                      ID_PARTITIONS_BUTTONS_PANEL,
	                                      wxDefaultPosition,
	                                      wxDefaultSize,
	                                      wxTAB_TRAVERSAL,
	                                      _T ("ID_PARTITIONS_BUTTONS_PANEL") );
	partitionsButtonsGridSizer = new wxGridSizer (0, 4, 0, 0);
	partitionsBellButton = new wxButton (partitionsButtonsPanel,
	                                     ID_PARTITIONS_BELL_BUTTON,
	                                     _ ("Bell(n)"),
	                                     wxDefaultPosition,
	                                     wxDefaultSize,
	                                     0,
	                                     wxDefaultValidator,
	                                     _T ("ID_PARTITIONS_BELL_BUTTON") );
	partitionsBellButton->SetToolTip (_ ("Compute the number of possible partitions in a set with n elements.") );
	partitionsButtonsGridSizer->Add (
	   partitionsBellButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	partitionsStirlingButton = new wxButton (partitionsButtonsPanel,
	      ID_PARTITIONS_STIRLING_BUTTON,
	      _ ("Stirling(n,k)"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_PARTITIONS_STIRLING_BUTTON") );
	partitionsStirlingButton->SetToolTip (_ ("Compute the number of ways to partition an n-element set into k subsets.") );
	partitionsButtonsGridSizer->Add (
	   partitionsStirlingButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	partitionsListPartitionsButton = new wxButton (partitionsButtonsPanel,
	      ID_PARTITONS_LIST_PARTITIONS_BUTTON,
	      _ ("List All Partitions"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_PARTITONS_LIST_PARTITIONS_BUTTON") );
	partitionsListPartitionsButton->SetToolTip (_ ("List all possible partitions of a set. Limited to 5000 lines.") );
	partitionsButtonsGridSizer->Add (
	   partitionsListPartitionsButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	partitionsListStirlingPartitionsButton = new wxButton (partitionsButtonsPanel,
	      ID_PARTITIONS_LIST_STIRLING_PARTITIONS_BUTTON,
	      _ ("List k-Element Partitions"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_PARTITIONS_LIST_STIRLING_PARTITIONS_BUTTON") );
	partitionsListStirlingPartitionsButton->SetToolTip (
	   _ ("List all k-element partitions of an n-element set - limited to 5000 lines") );
	partitionsButtonsGridSizer->Add (
	   partitionsListStirlingPartitionsButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	partitionsButtonsPanel->SetSizer (partitionsButtonsGridSizer);
	partitionsButtonsGridSizer->Fit (partitionsButtonsPanel);
	partitionsButtonsGridSizer->SetSizeHints (partitionsButtonsPanel);
	partitionsFlexGridSizer->Add(
    	partitionsButtonsPanel, 0, wxALL | wxEXPAND, 2);
	partitionsEstimatePanel = new wxPanel (partitionsPanel,
	                                       ID_COUNTING_AND_ENUMERATION_ESTIMATE_PANEL,
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       wxTAB_TRAVERSAL,
	                                       _T ("ID_COUNTING_AND_ENUMERATION_ESTIMATE_PANEL") );
	partitionsMagnitudeGridSizer = new wxGridSizer (0, 4, 0, 0);
	partitionsMagnitudeGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	partitionsResultMagnitudeButton = new wxButton (partitionsEstimatePanel,
	      ID_SUBSETS_RESULT_MAGNITUDE_BUTTON,
	      _ ("Result Magnitude"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_RESULT_MAGNITUDE_BUTTON") );
	partitionsResultMagnitudeButton->SetToolTip (_ ("Display the magnitude of the result") );
	partitionsMagnitudeGridSizer->Add (
	   partitionsResultMagnitudeButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	partitionsAsymptoticEstimateButton = new wxButton (partitionsEstimatePanel,
	      ID_COUNTING_AND_ENUMERATION_ASYMPTOTIC_ESTIMATE_BUTTON,
	      _ ("Asymptotic Estimate"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COUNTING_AND_ENUMERATION_ASYMPTOTIC_ESTIMATE_BUTTON") );
	partitionsAsymptoticEstimateButton->SetToolTip (_ ("Estimate the value of very large numbers") );
	partitionsMagnitudeGridSizer->Add (
	   partitionsAsymptoticEstimateButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	partitionsMagnitudeGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	partitionsEstimatePanel->SetSizer (partitionsMagnitudeGridSizer);
	partitionsMagnitudeGridSizer->Fit (partitionsEstimatePanel);
	partitionsMagnitudeGridSizer->SetSizeHints (partitionsEstimatePanel);
	partitionsFlexGridSizer->Add(
    	partitionsEstimatePanel, 0, wxALL | wxEXPAND, 2);

	// Reserve space beneath the bottom button row.
	partitionsFlexGridSizer->AddSpacer(20);

	partitionsPanel->SetSizer(partitionsFlexGridSizer);
	// TREES PAGE

	treesPanel = new wxPanel (discreteMathCalculatorNotebook,
	                          ID_BINARY_TREES_PANEL,
	                          wxDefaultPosition,
	                          wxDefaultSize,
	                          wxTAB_TRAVERSAL,
	                          _T ("ID_BINARY_TREES_PANEL") );
	binaryTreesFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	binaryTreesFlexGridSizer->AddGrowableCol (0);
	binaryTreesFlexGridSizer->AddGrowableRow (1);
	binaryTreesControlPanel = new wxPanel (treesPanel,
	                                       ID_BINARY_TREES_CONTROL_PANEL,
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       wxTAB_TRAVERSAL,
	                                       _T ("ID_BINARY_TREES_CONTROL_PANEL") );
	binaryTreesControlFlexGridSizer = new wxFlexGridSizer (3, 3, 0, 0);
	binaryTreesControlFlexGridSizer->AddGrowableCol (0);
	binaryTreesControlFlexGridSizer->AddGrowableCol (1);
	binaryTreesControlFlexGridSizer->AddGrowableCol (2);
	binaryTreesControlFlexGridSizer->AddGrowableRow (0);
	binaryTreesControlFlexGridSizer->AddGrowableRow (1);
	binaryTreesControlFlexGridSizer->AddGrowableRow (2);
	binaryTreesGeneralTreeStaticText = new wxStaticText (binaryTreesControlPanel,
	      ID_BINARY_TREES_GENERAL_TREE_STATICTEXT,
	      _ ("Binary Trees"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_BINARY_TREES_GENERAL_TREE_STATICTEXT") );
	wxFont binaryTreesGeneralTreeStaticTextFont (
	   14, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, true, _T ("Arial"), wxFONTENCODING_DEFAULT);
	binaryTreesGeneralTreeStaticText->SetFont (binaryTreesGeneralTreeStaticTextFont);
	binaryTreesControlFlexGridSizer->Add (
	   binaryTreesGeneralTreeStaticText, 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 2);
	binaryTreesEnterGeneralTreePanel = new wxPanel (binaryTreesControlPanel,
	      ID_BINARY_TREES_ENTER_GENERAL_TREE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_TREES_ENTER_GENERAL_TREE_PANEL") );
	binaryTreesGeneralTreeEnterFlexGridSizer = new wxFlexGridSizer (9, 1, 0, 0);
	binaryTreesGeneralTreeEnterFlexGridSizer->AddGrowableCol (0);
	binaryTreesGeneralTreeEnterFlexGridSizer->AddGrowableRow (8);
	binaryTreesGeneralBinaryTreeEnterStaticText =
	   new wxStaticText (binaryTreesEnterGeneralTreePanel,
	                     ID_BINARY_TREES_GENERAL_BINARY_TREE_ENTER_STATICTEXT,
	                     _ ("Build a General Binary Tree"),
	                     wxDefaultPosition,
	                     wxDefaultSize,
	                     0,
	                     _T ("ID_BINARY_TREES_GENERAL_BINARY_TREE_ENTER_STATICTEXT") );
	binaryTreesGeneralTreeEnterFlexGridSizer->Add (
	   binaryTreesGeneralBinaryTreeEnterStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	binaryTreesTitlePanel = new wxPanel (binaryTreesEnterGeneralTreePanel,
	                                     ID_BINARY_TREES_TITLE_PANEL,
	                                     wxDefaultPosition,
	                                     wxDefaultSize,
	                                     wxTAB_TRAVERSAL,
	                                     _T ("ID_BINARY_TREES_TITLE_PANEL") );
	binaryTreesTitleBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	binaryTreesGeneralTreeTitleStaticText = new wxStaticText (binaryTreesTitlePanel,
	      ID_BINARY_TREES_GENERAL_TREE_TITLE_STATICTEXT,
	      _ ("Binary Tree Title"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_BINARY_TREES_GENERAL_TREE_TITLE_STATICTEXT") );
	binaryTreesTitleBoxSizer->Add (
	   binaryTreesGeneralTreeTitleStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	binaryTreesGeneralTreeTitleTextCtrl = new wxTextCtrl (binaryTreesTitlePanel,
	      ID_BINARY_TREES_GENERAL_TREE_TITLE_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_GENERAL_TREE_TITLE_TEXTCTRL") );
	binaryTreesTitleBoxSizer->Add (
	   binaryTreesGeneralTreeTitleTextCtrl, 2, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	binaryTreesTitleBoxSizer->Add (-1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesTitlePanel->SetSizer (binaryTreesTitleBoxSizer);
	binaryTreesTitleBoxSizer->Fit (binaryTreesTitlePanel);
	binaryTreesTitleBoxSizer->SetSizeHints (binaryTreesTitlePanel);
	binaryTreesGeneralTreeEnterFlexGridSizer->Add (
	   binaryTreesTitlePanel, 1, wxALL | wxEXPAND, 5);
	binaryTreesGeneralTreeEnterFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesBTRPanel = new wxPanel (binaryTreesEnterGeneralTreePanel,
	                                   ID_BINARY_TREES_BTR_PANEL,
	                                   wxDefaultPosition,
	                                   wxDefaultSize,
	                                   wxTAB_TRAVERSAL,
	                                   _T ("ID_BINARY_TREES_BTR_PANEL") );
	binaryTreesBTRGridSizer = new wxGridSizer (0, 4, 0, 0);
	binaryTreesBTRGridSizer->Add (-1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesEnterBTRButton = new wxButton (binaryTreesBTRPanel,
	      ID_BINARY_TREES_ENTER_BTR_BUTTON,
	      _ ("Enter"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_ENTER_BTR_BUTTON") );
	binaryTreesEnterBTRButton->SetToolTip (_ ("Enter a binary tree in postorder") );
	binaryTreesBTRGridSizer->Add (
	   binaryTreesEnterBTRButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binaryTreesEnterBTRFromFileButton = new wxButton (binaryTreesBTRPanel,
	      ID_BINARY_TREES_ENTER_BTR_FROM_FILE_BUTTON,
	      _ ("From BTR File"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_ENTER_BTR_FROM_FILE_BUTTON") );
	binaryTreesEnterBTRFromFileButton->SetToolTip (_ ("Ener a binary tree from a \"btr\" file in postorder") );
	binaryTreesBTRGridSizer->Add (
	   binaryTreesEnterBTRFromFileButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binaryTreesBTRGridSizer->Add (-1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesBTRPanel->SetSizer (binaryTreesBTRGridSizer);
	binaryTreesBTRGridSizer->Fit (binaryTreesBTRPanel);
	binaryTreesBTRGridSizer->SetSizeHints (binaryTreesBTRPanel);
	binaryTreesGeneralTreeEnterFlexGridSizer->Add (
	   binaryTreesBTRPanel, 1, wxALL | wxEXPAND, 2);
	binaryTreesStaticLine = new wxStaticLine (binaryTreesEnterGeneralTreePanel,
	      ID_BINARY_TREES_STATICLINE,
	      wxDefaultPosition,
	      wxSize (10, -1),
	      wxLI_HORIZONTAL,
	      _T ("ID_BINARY_TREES_STATICLINE") );
	binaryTreesGeneralTreeEnterFlexGridSizer->Add (
	   binaryTreesStaticLine, 1, wxALL | wxEXPAND, 2);
	binaryTreesGeneralTreeEnterFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesGeneralTreeEnterFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesEnterGMLFileButton = new wxButton (binaryTreesEnterGeneralTreePanel,
	      ID_BINARY_TREES_ENTER_GML_BUTTON,
	      _ ("From GML File"),
	      wxDefaultPosition,
	      wxSize (-1,25),
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_ENTER_GML_BUTTON") );
	binaryTreesEnterGMLFileButton->SetToolTip (_ ("Display a binary tree saved in GML format") );
	binaryTreesGeneralTreeEnterFlexGridSizer->Add (
	   binaryTreesEnterGMLFileButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binaryTreesGeneralTreeEnterFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesEnterGeneralTreePanel->SetSizer (binaryTreesGeneralTreeEnterFlexGridSizer);
	binaryTreesGeneralTreeEnterFlexGridSizer->Fit (binaryTreesEnterGeneralTreePanel);
	binaryTreesGeneralTreeEnterFlexGridSizer->SetSizeHints (binaryTreesEnterGeneralTreePanel);
	binaryTreesControlFlexGridSizer->Add (
	   binaryTreesEnterGeneralTreePanel, 1, wxALL | wxEXPAND, 2);
	binaryTreesHuffmanPanel = new wxPanel (binaryTreesControlPanel,
	                                       ID_BINARY_TREES_HUFFMAN_PANEL,
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       wxTAB_TRAVERSAL,
	                                       _T ("ID_BINARY_TREES_HUFFMAN_PANEL") );
	binaryTreesHuffmanFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);
	binaryTreesHuffmanFlexGridSizer->AddGrowableCol (0);
	binaryTreesHuffmanFlexGridSizer->AddGrowableRow (0);
	binaryTreesHuffmanFlexGridSizer->AddGrowableRow (1);
	binaryTreesHuffmanFlexGridSizer->AddGrowableRow (2);
	binaryTreesHuffmanTreePanel = new wxPanel (binaryTreesHuffmanPanel,
	      ID_BINARY_TREES_HUFFMAN_TREE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_TREES_HUFFMAN_TREE_PANEL") );
	binaryTreesHuffmanTreeGridSizer = new wxGridSizer (3, 1, 0, 0);
	binaryTreeesHuffmanTreeStaticText = new wxStaticText (binaryTreesHuffmanTreePanel,
	      ID_BINARY_TREES_HUFFMAN_TREE_STATICTEXT,
	      _ ("Build a Huffman Tree"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_BINARY_TREES_HUFFMAN_TREE_STATICTEXT") );
	binaryTreesHuffmanTreeGridSizer->Add (
	   binaryTreeesHuffmanTreeStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	binaryTreesHuffmanTreeTitlePanel = new wxPanel (binaryTreesHuffmanTreePanel,
	      ID_BINARY_TREES_HUFFMAN_TREE_TITLE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_TREES_HUFFMAN_TREE_TITLE_PANEL") );
	binaryTreesHuffmanTreeTitleBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	binaryTreesHuffmanTreeTitleStaticText = new wxStaticText (binaryTreesHuffmanTreeTitlePanel,
	      ID_STATICTEXT2,
	      _ ("Huffman Tree Title"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_STATICTEXT2") );
	binaryTreesHuffmanTreeTitleBoxSizer->Add (
	   binaryTreesHuffmanTreeTitleStaticText, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	binaryTreesHuffmanTreeTitleTextCtrl = new wxTextCtrl (binaryTreesHuffmanTreeTitlePanel,
	      ID_BINARY_TREE_HUFFMAN_TREE_TITLE_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREE_HUFFMAN_TREE_TITLE_TEXTCTRL") );
	binaryTreesHuffmanTreeTitleBoxSizer->Add (
	   binaryTreesHuffmanTreeTitleTextCtrl, 2, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	binaryTreesHuffmanTreeTitlePanel->SetSizer (binaryTreesHuffmanTreeTitleBoxSizer);
	binaryTreesHuffmanTreeTitleBoxSizer->Fit (binaryTreesHuffmanTreeTitlePanel);
	binaryTreesHuffmanTreeTitleBoxSizer->SetSizeHints (binaryTreesHuffmanTreeTitlePanel);
	binaryTreesHuffmanTreeGridSizer->Add (
	   binaryTreesHuffmanTreeTitlePanel, 1, wxALL | wxEXPAND, 5);
	binaryTreesHuffmanTreeEnterPanel = new wxPanel (binaryTreesHuffmanTreePanel,
	      ID_BINARY_TREES_HUFFMAN_TREE_ENTER_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_TREES_HUFFMAN_TREE_ENTER_PANEL") );
	binaryTreesHuffmanTreeEnterGridSizer = new wxGridSizer (0, 5, 0, 0);
	binaryTreesHuffmanTreeEnterGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesHuffmanTreeEnterButton = new wxButton (binaryTreesHuffmanTreeEnterPanel,
	      ID_BINARY_TREES_HUFFMAN_TREE_ENTER_BUTTON,
	      _ ("Enter"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_HUFFMAN_TREE_ENTER_BUTTON") );
	binaryTreesHuffmanTreeEnterButton->SetToolTip (_ ("Enter frequencies for characters \'A\'-\'Z\', draw a Huffman tree, "
	      "and display the bit codes and compression ratio") );
	binaryTreesHuffmanTreeEnterGridSizer->Add (
	   binaryTreesHuffmanTreeEnterButton, 1, wxALL, 2);
	binaryTreesHuffmanTreeStaticLine = new wxStaticLine (binaryTreesHuffmanTreeEnterPanel,
	      ID_BINARY_TREES_HUFFMAN_TREE_STATICLINE,
	      wxDefaultPosition,
	      wxSize (2, 30),
	      wxLI_VERTICAL,
	      _T ("ID_BINARY_TREES_HUFFMAN_TREE_STATICLINE") );
	binaryTreesHuffmanTreeEnterGridSizer->Add (
	   binaryTreesHuffmanTreeStaticLine, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	/*binaryTreesHuffmanTreeFromGMLButton = new wxButton (binaryTreesHuffmanTreeEnterPanel,
	      ID_BINARY_TREES_HUFFMAN_TREE_FROM_GML_BUTTON,
	      _ ("From GML File"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_HUFFMAN_TREE_FROM_GML_BUTTON") );
	binaryTreesHuffmanTreeFromGMLButton->SetToolTip (_ ("Enter a Huffman tree saved in a GML file") );
	binaryTreesHuffmanTreeEnterGridSizer->Add (
	   binaryTreesHuffmanTreeFromGMLButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);*/
	binaryTreesHuffmanTreeEnterGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesHuffmanTreeEnterPanel->SetSizer (binaryTreesHuffmanTreeEnterGridSizer);
	binaryTreesHuffmanTreeEnterGridSizer->Fit (binaryTreesHuffmanTreeEnterPanel);
	binaryTreesHuffmanTreeEnterGridSizer->SetSizeHints (binaryTreesHuffmanTreeEnterPanel);
	binaryTreesHuffmanTreeGridSizer->Add (
	   binaryTreesHuffmanTreeEnterPanel, 1, wxALL | wxEXPAND, 5);
	binaryTreesHuffmanTreePanel->SetSizer (binaryTreesHuffmanTreeGridSizer);
	binaryTreesHuffmanTreeGridSizer->Fit (binaryTreesHuffmanTreePanel);
	binaryTreesHuffmanTreeGridSizer->SetSizeHints (binaryTreesHuffmanTreePanel);
	binaryTreesHuffmanFlexGridSizer->Add (
	   binaryTreesHuffmanTreePanel, 1, wxALL | wxEXPAND, 2);
	binaryTreesExpressionTreePanel = new wxPanel (binaryTreesHuffmanPanel,
	      ID_BINARY_TREES_EXPRESSION_TREE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_TREES_EXPRESSION_TREE_PANEL") );
	binaryTreesExpressionTreeGridSizer = new wxGridSizer (3, 1, 0, 0);
	binaryTreesExpressionTreeStaticText = new wxStaticText (binaryTreesExpressionTreePanel,
	      ID_BINARY_TREES_EXPRESSION_TREE_STATICTEXT,
	      _ ("Build an Expression Tree"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_BINARY_TREES_EXPRESSION_TREE_STATICTEXT") );
	binaryTreesExpressionTreeGridSizer->Add (
	   binaryTreesExpressionTreeStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binaryTreesExpressionTreeTitlePanel = new wxPanel (binaryTreesExpressionTreePanel,
	      ID_BINARY_TREES_EXPRESSION_TREE_TITLE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_TREES_EXPRESSION_TREE_TITLE_PANEL") );
	binaryTreesExpressionTreeTitleBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	binaryTreesExpressionTreeTitleStaticText = new wxStaticText (binaryTreesExpressionTreeTitlePanel,
	      ID_STATICTEXT3,
	      _ ("Expression Tree Title"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_STATICTEXT3") );
	binaryTreesExpressionTreeTitleBoxSizer->Add (
	   binaryTreesExpressionTreeTitleStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binaryTreesExpressionTreeTitleTextCtrl = new wxTextCtrl (binaryTreesExpressionTreeTitlePanel,
	      ID_BINARY_TREES_EXPRESSION_TREE_TITLE_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_EXPRESSION_TREE_TITLE_TEXTCTRL") );
	binaryTreesExpressionTreeTitleBoxSizer->Add (
	   binaryTreesExpressionTreeTitleTextCtrl, 2, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	binaryTreesExpressionTreeTitlePanel->SetSizer (binaryTreesExpressionTreeTitleBoxSizer);
	binaryTreesExpressionTreeTitleBoxSizer->Fit (binaryTreesExpressionTreeTitlePanel);
	binaryTreesExpressionTreeTitleBoxSizer->SetSizeHints (binaryTreesExpressionTreeTitlePanel);
	binaryTreesExpressionTreeGridSizer->Add (binaryTreesExpressionTreeTitlePanel,
	      1,
	      wxALL | wxEXPAND,
	      5);
	binaryTreesExpressionTreeEnterPanel = new wxPanel (binaryTreesExpressionTreePanel,
	      ID_BINARY_TREES_EXPRESSION_TREE_ENTER_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_TREES_EXPRESSION_TREE_ENTER_PANEL") );
	binaryTreesExpressionTreeEnterGridSizer = new wxGridSizer (0, 5, 0, 0);
	binaryTreesExpressionTreeEnterGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesExpressionTreeEnterButton = new wxButton (binaryTreesExpressionTreeEnterPanel,
	      ID_BINARY_TREES_EXPRESSION_TREE_ENTER_BUTTON,
	      _ ("Enter"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_EXPRESSION_TREE_ENTER_BUTTON") );
	binaryTreesExpressionTreeEnterButton->SetToolTip (
	   _ ("Emter an integer expression in infix format using operators\n+, -, *, /, ^, and unary minus. Draw the "
	      "expression tree and\nthe corresponding postfix expression") );
	binaryTreesExpressionTreeEnterGridSizer->Add (
	   binaryTreesExpressionTreeEnterButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binaryTreesExpressionTreeStaticLine = new wxStaticLine (binaryTreesExpressionTreeEnterPanel,
	      ID_STATICLINE2,
	      wxDefaultPosition,
	      wxSize (2, 35),
	      wxLI_VERTICAL,
	      _T ("ID_STATICLINE2") );
	binaryTreesExpressionTreeEnterGridSizer->Add (
	   binaryTreesExpressionTreeStaticLine, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binaryTreesExpressionTreeFromGMLButton = new wxButton (binaryTreesExpressionTreeEnterPanel,
	      ID_BINARY_TREES_EXPRESSION_TREE_FROM_GML_BUTTON,
	      _ ("From GML File"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_EXPRESSION_TREE_FROM_GML_BUTTON") );
	binaryTreesExpressionTreeFromGMLButton->SetToolTip (_ ("Enter an expression tree saved in a GML file.") );
	binaryTreesExpressionTreeEnterGridSizer->Add (
	   binaryTreesExpressionTreeFromGMLButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binaryTreesExpressionTreeEnterGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binaryTreesExpressionTreeEnterPanel->SetSizer (binaryTreesExpressionTreeEnterGridSizer);
	binaryTreesExpressionTreeEnterGridSizer->Fit (binaryTreesExpressionTreeEnterPanel);
	binaryTreesExpressionTreeEnterGridSizer->SetSizeHints (binaryTreesExpressionTreeEnterPanel);
	binaryTreesExpressionTreeGridSizer->Add (binaryTreesExpressionTreeEnterPanel,
	      1,
	      wxALL | wxEXPAND,
	      2);
	binaryTreesExpressionTreePanel->SetSizer (binaryTreesExpressionTreeGridSizer);
	binaryTreesExpressionTreeGridSizer->Fit (binaryTreesExpressionTreePanel);
	binaryTreesExpressionTreeGridSizer->SetSizeHints (binaryTreesExpressionTreePanel);
	binaryTreesHuffmanFlexGridSizer->Add (
	   binaryTreesExpressionTreePanel, 1, wxALL | wxEXPAND, 2);
	binaryTreesHuffmanPanel->SetSizer (binaryTreesHuffmanFlexGridSizer);
	binaryTreesHuffmanFlexGridSizer->Fit (binaryTreesHuffmanPanel);
	binaryTreesHuffmanFlexGridSizer->SetSizeHints (binaryTreesHuffmanPanel);
	binaryTreesControlFlexGridSizer->Add (
	   binaryTreesHuffmanPanel, 1, wxALL | wxEXPAND, 2);
	binaryTreesStaticLine1 = new wxStaticLine (binaryTreesControlPanel,
	      ID_BINARY_TREES_STATICLINE1,
	      wxDefaultPosition,
	      wxSize (10, 3),
	      wxLI_HORIZONTAL,
	      _T ("ID_BINARY_TREES_STATICLINE1") );
	binaryTreesControlFlexGridSizer->Add (
	   binaryTreesStaticLine1, 1, wxALL | wxEXPAND, 5);
	binaryTreesStaticLine2 = new wxStaticLine (binaryTreesControlPanel,
	      ID_BINARY_TREES_STATICLINE2,
	      wxDefaultPosition,
	      wxSize (10, 3),
	      wxLI_HORIZONTAL,
	      _T ("ID_BINARY_TREES_STATICLINE2") );
	binaryTreesControlFlexGridSizer->Add (
	   binaryTreesStaticLine2, 1, wxALL | wxEXPAND, 5);
	binaryTreesStaticLine3 = new wxStaticLine (binaryTreesControlPanel,
	      ID_BINARY_TREES_STATICLINE3,
	      wxDefaultPosition,
	      wxSize (10, 3),
	      wxLI_HORIZONTAL,
	      _T ("ID_BINARY_TREES_STATICLINE3") );
	binaryTreesControlFlexGridSizer->Add (
	   binaryTreesStaticLine3, 1, wxALL | wxEXPAND, 5);
	binaryTreesSearchTreeStaticText = new wxStaticText (binaryTreesControlPanel,
	      ID_BINARY_TREES_SEARCH_TREE_STATICTEXT,
	      _ ("Search Trees"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_BINARY_TREES_SEARCH_TREE_STATICTEXT") );
	wxFont binaryTreesSearchTreeStaticTextFont (
	   14, wxSWISS, wxFONTSTYLE_NORMAL, wxNORMAL, true, _T ("Arial"), wxFONTENCODING_DEFAULT);
	binaryTreesSearchTreeStaticText->SetFont (binaryTreesSearchTreeStaticTextFont);
	binaryTreesControlFlexGridSizer->Add (
	   binaryTreesSearchTreeStaticText, 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 2);
	binarySearchTreesEnterPanel = new wxPanel (binaryTreesControlPanel,
	      ID_BINARY_SEARCH_TREES_ENTER_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_SEARCH_TREES_ENTER_PANEL") );
	binarySearchTreeSEnterFlexGridSizer = new wxFlexGridSizer (6, 1, 0, 0);
	binarySearchTreeSEnterFlexGridSizer->AddGrowableCol (0);
	binarySearchTreeSEnterFlexGridSizer->AddGrowableRow (1);
	binarySearchTreesStaticText = new wxStaticText (binarySearchTreesEnterPanel,
	      ID_BINARY_SEARCH_TREES_STATICTEXT,
	      _ ("Build a Binary Search Tree"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_BINARY_SEARCH_TREES_STATICTEXT") );
	binarySearchTreeSEnterFlexGridSizer->Add (
	   binarySearchTreesStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	binarySearchTreesTitlePanel = new wxPanel (binarySearchTreesEnterPanel,
	      ID_BINARY_SEARCH_TREES_TITLE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_SEARCH_TREES_TITLE_PANEL") );
	binarySearchTreesTitleBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	binarySearchTreesTitleStaticText = new wxStaticText (binarySearchTreesTitlePanel,
	      ID_BINARY_SEARCH_TREES_TITLE_STATICTEXT,
	      _ ("Title"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxALIGN_CENTRE,
	      _T ("ID_BINARY_SEARCH_TREES_TITLE_STATICTEXT") );
	binarySearchTreesTitleBoxSizer->Add (
	   binarySearchTreesTitleStaticText, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	binaryTreesSearchTreesTitleTextCtrl = new wxTextCtrl (binarySearchTreesTitlePanel,
	      ID_BINARY_TREES_SEARCH_TREES_TITLE_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_SEARCH_TREES_TITLE_TEXTCTRL") );
	binarySearchTreesTitleBoxSizer->Add (
	   binaryTreesSearchTreesTitleTextCtrl, 3, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binarySearchTreesTitlePanel->SetSizer (binarySearchTreesTitleBoxSizer);
	binarySearchTreesTitleBoxSizer->Fit (binarySearchTreesTitlePanel);
	binarySearchTreesTitleBoxSizer->SetSizeHints (binarySearchTreesTitlePanel);
	binarySearchTreeSEnterFlexGridSizer->Add (
	   binarySearchTreesTitlePanel, 1, wxALL | wxEXPAND, 5);
	binarySearchTreesEnterControlPanel = new wxPanel (binarySearchTreesEnterPanel,
	      ID_BINARY_SEARCH_TREES_CONTROL_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_SEARCH_TREES_CONTROL_PANEL") );
	binarySearchTreesControlBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	binarySearchTreesEnterButton = new wxButton (binarySearchTreesEnterControlPanel,
	      ID_BINARY_SEARCH_TREES_ENTER_BUTTON,
	      _ ("Enter"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_SEARCH_TREES_ENTER_BUTTON") );
	binarySearchTreesEnterButton->SetToolTip (_ ("Enter integer data for a binary search tree") );
	binarySearchTreesControlBoxSizer->Add (
	   binarySearchTreesEnterButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	binarySearchTreesEnterFromFileButton = new wxButton (binarySearchTreesEnterControlPanel,
	      ID_BINARY_TREES_SEARCH_TREE_ENTER_FROM_FILE_BUTTON,
	      _ ("From Text File"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_TREES_SEARCH_TREE_ENTER_FROM_FILE_BUTTON") );
	binarySearchTreesEnterFromFileButton->SetToolTip (_ ("Input integer data for a binary search tree from a textfile") );
	binarySearchTreesControlBoxSizer->Add (
	   binarySearchTreesEnterFromFileButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	binarySearchTreesRandomDataEnterButton = new wxButton (binarySearchTreesEnterControlPanel,
	      ID_BINARY_SEARCH_TREES_RANDOM_DATA_ENTER_BUTTON,
	      _ ("Random Data"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_SEARCH_TREES_RANDOM_DATA_ENTER_BUTTON") );
	binarySearchTreesRandomDataEnterButton->SetToolTip (
	   _ ("Generate a random binary search tree with nodes in the range 0 .. 99") );
	binarySearchTreesControlBoxSizer->Add (
	   binarySearchTreesRandomDataEnterButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binarySearchTreesEnterControlPanel->SetSizer (binarySearchTreesControlBoxSizer);
	binarySearchTreesControlBoxSizer->Fit (binarySearchTreesEnterControlPanel);
	binarySearchTreesControlBoxSizer->SetSizeHints (binarySearchTreesEnterControlPanel);
	binarySearchTreeSEnterFlexGridSizer->Add (binarySearchTreesEnterControlPanel,
	      1,
	      wxALL | wxEXPAND,
	      5);
	binarySearchTreesStaticLine = new wxStaticLine (binarySearchTreesEnterPanel,
	      ID_BINARY_SEARCH_TREES_STATICLINE,
	      wxDefaultPosition,
	      wxSize (2, -1),
	      wxLI_HORIZONTAL,
	      _T ("ID_BINARY_SEARCH_TREES_STATICLINE") );
	binarySearchTreeSEnterFlexGridSizer->Add (
	   binarySearchTreesStaticLine, 1, wxALL | wxEXPAND, 2);
	binarySearchTreeSEnterFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binarySearchTreesFromGMLButton = new wxButton (binarySearchTreesEnterPanel,
	      ID_BINARY_SEARCH_TREES_FROM_GML_BUTTON,
	      _ ("From GML File"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_BINARY_SEARCH_TREES_FROM_GML_BUTTON") );
	binarySearchTreesFromGMLButton->SetToolTip (_ ("Draw a binary search tree stored in a GML file") );
	binarySearchTreeSEnterFlexGridSizer->Add (
	   binarySearchTreesFromGMLButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	binarySearchTreesEnterPanel->SetSizer (binarySearchTreeSEnterFlexGridSizer);
	binarySearchTreeSEnterFlexGridSizer->Fit (binarySearchTreesEnterPanel);
	binarySearchTreeSEnterFlexGridSizer->SetSizeHints (binarySearchTreesEnterPanel);
	binaryTreesControlFlexGridSizer->Add (
	   binarySearchTreesEnterPanel, 1, wxALL | wxEXPAND, 1);
	binarySearchTreesBalanceAlgorithmsPanel = new wxPanel (binaryTreesControlPanel,
	      ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_PANEL,
	      wxDefaultPosition,
	      wxSize (309, 170),
	      wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_PANEL") );
	binarySearchTreesBalanceAlgorithmsFlexGridSizer = new wxFlexGridSizer (5, 1, 0, 0);
	binarySearchTreesBalanceAlgorithmsFlexGridSizer->AddGrowableCol (0);
	binarySearchTreesBalanceAlgorithmsFlexGridSizer->AddGrowableRow (0);
	binarySearchTreesBalanceAlgorithmsSelectionPanel =
	   new wxPanel (binarySearchTreesBalanceAlgorithmsPanel,
	                ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_SELECTION_PANEL,
	                wxDefaultPosition,
	                wxDefaultSize,
	                wxTAB_TRAVERSAL,
	                _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_SELECTION_PANEL") );
	binarySearchTreesBalanceTitleFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	binarySearchTreesBalanceTitleFlexGridSizer->AddGrowableCol (0);
	binarySearchTreesBalanceTitleFlexGridSizer->AddGrowableRow (0);
	binarySearchTreesBalanceTitlePanel = new wxPanel (binarySearchTreesBalanceAlgorithmsSelectionPanel,
	      ID_BINARY_SEARCH_TREES_BALANCE_TITLE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_BINARY_SEARCH_TREES_BALANCE_TITLE_PANEL") );
	binarySearchTreesTitleFlexGridSizer = new wxFlexGridSizer (0, 3, 0, 0);
	binarySearchTreesTitleFlexGridSizer->AddGrowableCol (2);
	wxString __wxRadioBoxChoices_1[2] = { _ ("AVL"), _ ("Red-Black") };
	binarySearchTreesBalanceAlgorithmsRadioBox =
	   new wxRadioBox (binarySearchTreesBalanceTitlePanel,
	                   ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RADIOBOX,
	                   _ ("Balanced Tree"),
	                   wxDefaultPosition,
	                   wxDefaultSize,
	                   2,
	                   __wxRadioBoxChoices_1,
	                   1,
	                   wxRA_SPECIFY_COLS,
	                   wxDefaultValidator,
	                   _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RADIOBOX") );
	binarySearchTreesBalanceAlgorithmsRadioBox->SetToolTip (_ ("Choose the balancing algorithm used") );
	binarySearchTreesTitleFlexGridSizer->Add (binarySearchTreesBalanceAlgorithmsRadioBox,
	      1,
	      wxEXPAND | wxALL,
	      2);
	binarySearchTreesBalanceAlgorithmsTitleStaticText =
	   new wxStaticText (binarySearchTreesBalanceTitlePanel,
	                     ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_STATICTEXT,
	                     _ ("Title"),
	                     wxDefaultPosition,
	                     wxDefaultSize,
	                     0,
	                     _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_STATICTEXT") );
	binarySearchTreesTitleFlexGridSizer->Add (
	   binarySearchTreesBalanceAlgorithmsTitleStaticText, 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 2);
	binarySearchTreesBalanceAlgorithmsTitlePanel =
	   new wxPanel (binarySearchTreesBalanceTitlePanel,
	                ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_PANEL,
	                wxDefaultPosition,
	                wxDefaultSize,
	                wxTAB_TRAVERSAL,
	                _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_PANEL") );
	binarySearchTreesBalanceAlgorithmsTitleGridSizer = new wxGridSizer (3, 0, 0, 0);
	binarySearchTreesBalanceAlgorithmsTitleGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binarySearchTreesBalanceAlgorithmsTitleTextCtrl =
	   new wxTextCtrl (binarySearchTreesBalanceAlgorithmsTitlePanel,
	                   ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_TEXTCTRL,
	                   wxEmptyString,
	                   wxDefaultPosition,
	                   wxDefaultSize,
	                   0,
	                   wxDefaultValidator,
	                   _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_TEXTCTRL") );
	binarySearchTreesBalanceAlgorithmsTitleGridSizer->Add (binarySearchTreesBalanceAlgorithmsTitleTextCtrl,
	      1,
	      wxALL | wxEXPAND,
	      2);
	binarySearchTreesBalanceAlgorithmsTitleGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binarySearchTreesBalanceAlgorithmsTitlePanel->SetSizer (binarySearchTreesBalanceAlgorithmsTitleGridSizer);
	binarySearchTreesBalanceAlgorithmsTitleGridSizer->Fit (binarySearchTreesBalanceAlgorithmsTitlePanel);
	binarySearchTreesBalanceAlgorithmsTitleGridSizer->SetSizeHints (binarySearchTreesBalanceAlgorithmsTitlePanel);
	binarySearchTreesTitleFlexGridSizer->Add (binarySearchTreesBalanceAlgorithmsTitlePanel,
	      1,
	      wxALL | wxEXPAND,
	      2);
	binarySearchTreesBalanceTitlePanel->SetSizer (binarySearchTreesTitleFlexGridSizer);
	binarySearchTreesTitleFlexGridSizer->Fit (binarySearchTreesBalanceTitlePanel);
	binarySearchTreesTitleFlexGridSizer->SetSizeHints (binarySearchTreesBalanceTitlePanel);
	binarySearchTreesBalanceTitleFlexGridSizer->Add (binarySearchTreesBalanceTitlePanel,
	      1,
	      wxALL | wxEXPAND,
	      2);
	binarySearchTreesBalanceAlgorithmsSelectionPanel->SetSizer (binarySearchTreesBalanceTitleFlexGridSizer);
	binarySearchTreesBalanceTitleFlexGridSizer->Fit (binarySearchTreesBalanceAlgorithmsSelectionPanel);
	binarySearchTreesBalanceTitleFlexGridSizer->SetSizeHints (binarySearchTreesBalanceAlgorithmsSelectionPanel);
	binarySearchTreesBalanceAlgorithmsFlexGridSizer->Add (binarySearchTreesBalanceAlgorithmsSelectionPanel,
	      1,
	      wxALL | wxEXPAND,
	      2);
	binarySearchTreesBalanceAlgorithmsControlPanel =
	   new wxPanel (binarySearchTreesBalanceAlgorithmsPanel,
	                ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_CONTROL_PANEL,
	                wxDefaultPosition,
	                wxDefaultSize,
	                wxTAB_TRAVERSAL,
	                _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_CONTROL_PANEL") );
	binarySearchTreesBalanceAlgorithmsControlBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	binarySearchTreesBalanceAlgorithmsEnterButton =
	   new wxButton (binarySearchTreesBalanceAlgorithmsControlPanel,
	                 ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_ENTER_BUTTON,
	                 _ ("Enter"),
	                 wxDefaultPosition,
	                 wxDefaultSize,
	                 0,
	                 wxDefaultValidator,
	                 _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_ENTER_BUTTON") );
	binarySearchTreesBalanceAlgorithmsEnterButton->SetToolTip (_ ("Enter integer data for a balanced binary search tree") );
	binarySearchTreesBalanceAlgorithmsControlBoxSizer->Add (binarySearchTreesBalanceAlgorithmsEnterButton,
	      1,
	      wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	      2);
	binarySearchTreesBalanceAlgorithmsFromTextfileButton =
	   new wxButton (binarySearchTreesBalanceAlgorithmsControlPanel,
	                 ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_TEXTFILE_BUTTON,
	                 _ ("From Text File"),
	                 wxDefaultPosition,
	                 wxDefaultSize,
	                 0,
	                 wxDefaultValidator,
	                 _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_TEXTFILE_BUTTON") );
	binarySearchTreesBalanceAlgorithmsFromTextfileButton->SetToolTip (
	   _ ("Input integer data for a balanced binary search tree from a textfile") );
	binarySearchTreesBalanceAlgorithmsControlBoxSizer->Add (binarySearchTreesBalanceAlgorithmsFromTextfileButton,
	      1,
	      wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	      2);
	binarySearchTreesBalanceAlgorithmsRandomDataButton =
	   new wxButton (binarySearchTreesBalanceAlgorithmsControlPanel,
	                 ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RANDOM_DATA_BUTTON,
	                 _ ("Random Data"),
	                 wxDefaultPosition,
	                 wxDefaultSize,
	                 0,
	                 wxDefaultValidator,
	                 _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RANDOM_DATA_BUTTON") );
	binarySearchTreesBalanceAlgorithmsRandomDataButton->SetToolTip (
	   _ ("Generate a random balanced binary search tree with nodes in the range 0 .. 99") );
	binarySearchTreesBalanceAlgorithmsControlBoxSizer->Add (
	   binarySearchTreesBalanceAlgorithmsRandomDataButton, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 2);
	binarySearchTreesBalanceAlgorithmsControlPanel->SetSizer (binarySearchTreesBalanceAlgorithmsControlBoxSizer);
	binarySearchTreesBalanceAlgorithmsControlBoxSizer->Fit (binarySearchTreesBalanceAlgorithmsControlPanel);
	binarySearchTreesBalanceAlgorithmsControlBoxSizer->SetSizeHints (binarySearchTreesBalanceAlgorithmsControlPanel);
	binarySearchTreesBalanceAlgorithmsFlexGridSizer->Add (binarySearchTreesBalanceAlgorithmsControlPanel,
	      1,
	      wxALL | wxEXPAND,
	      2);
	binarySearchTreesBalanceAlgorithmsStaticLine =
	   new wxStaticLine (binarySearchTreesBalanceAlgorithmsPanel,
	                     ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_STATICLINE,
	                     wxDefaultPosition,
	                     wxSize (2, -1),
	                     wxLI_HORIZONTAL,
	                     _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_STATICLINE") );
	binarySearchTreesBalanceAlgorithmsFlexGridSizer->Add (binarySearchTreesBalanceAlgorithmsStaticLine,
	      1,
	      wxALL | wxEXPAND,
	      2);
	binarySearchTreesBalanceAlgorithmsFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 2);
	binarySearchTreesBalanceAlgorithmsFromGMLButton =
	   new wxButton (binarySearchTreesBalanceAlgorithmsPanel,
	                 ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_GML_BUTTON,
	                 _ ("From GML File"),
	                 wxDefaultPosition,
	                 wxDefaultSize,
	                 0,
	                 wxDefaultValidator,
	                 _T ("ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_GML_BUTTON") );
	binarySearchTreesBalanceAlgorithmsFromGMLButton->SetToolTip (
	   _ ("Draw a balanced binary search tree stored in a GML file") );
	binarySearchTreesBalanceAlgorithmsFlexGridSizer->Add (binarySearchTreesBalanceAlgorithmsFromGMLButton,
	      1,
	      wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	      2);
	binarySearchTreesBalanceAlgorithmsPanel->SetSizer (binarySearchTreesBalanceAlgorithmsFlexGridSizer);
	binarySearchTreesBalanceAlgorithmsFlexGridSizer->SetSizeHints (binarySearchTreesBalanceAlgorithmsPanel);
	binaryTreesControlFlexGridSizer->Add (binarySearchTreesBalanceAlgorithmsPanel,
	                                      1,
	                                      wxALL | wxEXPAND,
	                                      1);
	binaryTreesControlPanel->SetSizer (binaryTreesControlFlexGridSizer);
	binaryTreesControlFlexGridSizer->Fit (binaryTreesControlPanel);
	binaryTreesControlFlexGridSizer->SetSizeHints (binaryTreesControlPanel);
	binaryTreesFlexGridSizer->Add (
	   binaryTreesControlPanel, 1, wxALL | wxEXPAND, 0);
	heapGeneralTreePanel = new wxPanel (treesPanel,
	                                    ID_HEAP_GENERAL_TREE_PANEL,
	                                    wxDefaultPosition,
	                                    wxDefaultSize,
	                                    wxTAB_TRAVERSAL,
	                                    _T ("ID_HEAP_GENERAL_TREE_PANEL") );
	heapGeneralTreeBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	heapPanel = new wxPanel (heapGeneralTreePanel,
	                         ID_PANEL2,
	                         wxDefaultPosition,
	                         wxDefaultSize,
	                         wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	                         _T ("ID_PANEL2") );
	heapFlexGridSizer = new wxFlexGridSizer (6, 1, 0, 0);
	heapFlexGridSizer->AddGrowableCol (0);
	heapFlexGridSizer->AddGrowableRow (1);
	heapFlexGridSizer->AddGrowableRow (2);
	heapStaticText = new wxStaticText (heapPanel,
	                                   ID_HEAP_STATICTEXT,
	                                   _ ("Build a Heap"),
	                                   wxDefaultPosition,
	                                   wxDefaultSize,
	                                   0,
	                                   _T ("ID_HEAP_STATICTEXT") );
	heapFlexGridSizer->Add (heapStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	heapBuildTitlePanel = new wxPanel (
	   heapPanel, ID_BUILD_HEAP_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_BUILD_HEAP_PANEL") );
	heapBuildTitleBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	heapBuildTitleStaticText = new wxStaticText (heapBuildTitlePanel,
	      ID_HEAP_TITLE_STATICTEXT,
	      _ ("Heap Title"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_HEAP_TITLE_STATICTEXT") );
	heapBuildTitleBoxSizer->Add (heapBuildTitleStaticText, 1, wxALL | wxALIGN_CENTER_VERTICAL, 1);
	heapBuildTitleTextCtrl = new wxTextCtrl (heapBuildTitlePanel,
	      ID_HEAP_TITLE_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_HEAP_TITLE_TEXTCTRL") );
	heapBuildTitleBoxSizer->Add (
	   heapBuildTitleTextCtrl, 3, wxALL | wxALIGN_CENTER_VERTICAL, 1);
	heapBuildTitlePanel->SetSizer (heapBuildTitleBoxSizer);
	heapBuildTitleBoxSizer->Fit (heapBuildTitlePanel);
	heapBuildTitleBoxSizer->SetSizeHints (heapBuildTitlePanel);
	heapFlexGridSizer->Add (
	   heapBuildTitlePanel, 1, wxALL | wxEXPAND, 1);
	enterHeapPanel = new wxPanel (
	   heapPanel, ID_ENTER_HEAP_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_ENTER_HEAP_PANEL") );
	enterHeapGridSizer = new wxGridSizer (0, 4, 0, 0);
	wxString __wxRadioBoxChoices_2[2] = { _ ("Minimum"), _ ("Maximum") };
	enterHeapRadioBox = new wxRadioBox (enterHeapPanel,
	                                    ID_ENTER_HEAP_RADIOBOX,
	                                    _ ("Type"),
	                                    wxDefaultPosition,
	                                    wxSize (70, -1),
	                                    2,
	                                    __wxRadioBoxChoices_2,
	                                    1,
	                                    wxRA_SPECIFY_COLS,
	                                    wxDefaultValidator,
	                                    _T ("ID_ENTER_HEAP_RADIOBOX") );
	enterHeapRadioBox->SetToolTip (_ ("Choose the type of heap to be drawn") );
	enterHeapGridSizer->Add (
	   enterHeapRadioBox, 1, wxALL | wxEXPAND, 1);
	enterHeapButton = new wxButton (enterHeapPanel,
	                                ID_ENTER_HEAP_BUTTON,
	                                _ ("Enter"),
	                                wxDefaultPosition,
	                                wxDefaultSize,
	                                0,
	                                wxDefaultValidator,
	                                _T ("ID_ENTER_HEAP_BUTTON") );
	enterHeapButton->SetToolTip (_ ("Enter integer data") );
	enterHeapGridSizer->Add (enterHeapButton, 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);
	enterRandomHeapButton = new wxButton (enterHeapPanel,
	                                      ID_ENTER_RANDOM_HEAP_BUTTON,
	                                      _ ("Random Data"),
	                                      wxDefaultPosition,
	                                      wxDefaultSize,
	                                      0,
	                                      wxDefaultValidator,
	                                      _T ("ID_ENTER_RANDOM_HEAP_BUTTONID_ENTER_RANDOM_HEAP_BUTTON") );
	enterRandomHeapButton->SetToolTip (_ ("Generate a random heap whose values are in the range 0..99") );
	enterHeapGridSizer->Add (enterRandomHeapButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	enterHeapFromTextfileButton = new wxButton (enterHeapPanel,
	      ID_ENTER_HEAP_FROM_TEXTFILE_BUTTON,
	      _ ("From Text File"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_ENTER_HEAP_FROM_TEXTFILE_BUTTON") );
	enterHeapFromTextfileButton->SetToolTip (_ ("Open a textfile that contains integer data") );
	enterHeapGridSizer->Add (enterHeapFromTextfileButton, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 1);
	enterHeapPanel->SetSizer (enterHeapGridSizer);
	enterHeapGridSizer->Fit (enterHeapPanel);
	enterHeapGridSizer->SetSizeHints (enterHeapPanel);
	heapFlexGridSizer->Add (enterHeapPanel, 1, wxALL | wxEXPAND, 1);
	heapStaticLine = new wxStaticLine (
	   heapPanel, ID_HEAP_STATICLINE, wxDefaultPosition, wxSize (2, -1), wxLI_HORIZONTAL, _T ("ID_HEAP_STATICLINE") );
	heapFlexGridSizer->Add (heapStaticLine, 1, wxALL | wxEXPAND, 1);
	heapFlexGridSizer->Add (-1, -1, 1, wxALL | wxEXPAND, 1);
	heapEnterFromGMLButton = new wxButton (heapPanel,
	                                       ID_HEAP_ENTER_FROM_GML_BUTTON,
	                                       _ ("From GML File"),
	                                       wxDefaultPosition,
	                                       wxSize (205,35),
	                                       0,
	                                       wxDefaultValidator,
	                                       _T ("ID_HEAP_ENTER_FROM_GML_BUTTON") );
	heapEnterFromGMLButton->SetToolTip (_ ("Draw a heap stored in a GML file") );
	heapFlexGridSizer->Add (heapEnterFromGMLButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	heapPanel->SetSizer (heapFlexGridSizer);
	heapFlexGridSizer->Fit (heapPanel);
	heapFlexGridSizer->SetSizeHints (heapPanel);
	heapGeneralTreeBoxSizer->Add (
	   heapPanel, 1, wxALL | wxEXPAND, 1);
	generalTreePanel = new wxPanel (heapGeneralTreePanel,
	                                ID_GENERAL_TREE_PANEL,
	                                wxDefaultPosition,
	                                wxDefaultSize,
	                                wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	                                _T ("ID_GENERAL_TREE_PANEL") );
	generalTreeFlexGridSizer = new wxFlexGridSizer (6, 1, 0, 0);
	generalTreeFlexGridSizer->AddGrowableCol (0);
	generalTreeFlexGridSizer->AddGrowableRow (1);
	generalTreeFlexGridSizer->AddGrowableRow (2);
	generalTreeStaticText = new wxStaticText (generalTreePanel,
	      ID_GENERAL_TREE_STATICTEXT,
	      _ ("General Tree"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_GENERAL_TREE_STATICTEXT") );
	generalTreeFlexGridSizer->Add (
	   generalTreeStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	generalTreeTitlePanel = new wxPanel (generalTreePanel,
	                                     ID_GENERAL_TREE_TITLE_PANEL,
	                                     wxDefaultPosition,
	                                     wxDefaultSize,
	                                     wxTAB_TRAVERSAL,
	                                     _T ("ID_GENERAL_TREE_TITLE_PANEL") );
	generalTreeTitleBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	generalTreeTitleStaticText = new wxStaticText (generalTreeTitlePanel,
	      ID_GENERAL_TREE_TITLE_STATICTEXT,
	      _ ("Tree Title"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxALIGN_RIGHT,
	      _T ("ID_GENERAL_TREE_TITLE_STATICTEXT") );
	generalTreeTitleBoxSizer->Add (generalTreeTitleStaticText, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	generalTreeTitleTextCtrl = new wxTextCtrl (generalTreeTitlePanel,
	      ID_GENERAL_TREE_TITLE_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GENERAL_TREE_TITLE_TEXTCTRL") );
	generalTreeTitleBoxSizer->Add (
	   generalTreeTitleTextCtrl, 3, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	generalTreeTitleBoxSizer->Add (-1, -1, 1, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	generalTreeTitlePanel->SetSizer (generalTreeTitleBoxSizer);
	generalTreeTitleBoxSizer->Fit (generalTreeTitlePanel);
	generalTreeTitleBoxSizer->SetSizeHints (generalTreeTitlePanel);
	generalTreeFlexGridSizer->Add (
	   generalTreeTitlePanel, 1, wxALL | wxEXPAND, 2);
	generalTreeEnterPanel = new wxPanel (generalTreePanel,
	                                     ID_GENERAL_TREE_ENTER_PANEL,
	                                     wxDefaultPosition,
	                                     wxDefaultSize,
	                                     wxTAB_TRAVERSAL,
	                                     _T ("ID_GENERAL_TREE_ENTER_PANEL") );
	generalTreeEnterGridSizer = new wxGridSizer (0, 3, 0, 0);
	generalTreeEnterGridSizer->Add (-1, -1, 1, wxALL | wxEXPAND, 2);
	generalTreeEnterButton = new wxButton (generalTreeEnterPanel,
	                                       ID_GENERAL_TREE_ENTER_FROM_TEXTFILE_BUTTON,
	                                       _ ("Enter"),
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       0,
	                                       wxDefaultValidator,
	                                       _T ("ID_GENERAL_TREE_ENTER_FROM_TEXTFILE_BUTTON") );
	generalTreeEnterButton->SetToolTip (_ ("Enter a general tree by specifying a node and its children") );
	generalTreeEnterGridSizer->Add (
	   generalTreeEnterButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
	generalTreeEnterGridSizer->Add (-1, -1, 1, wxALL | wxEXPAND, 2);
	generalTreeEnterPanel->SetSizer (generalTreeEnterGridSizer);
	generalTreeEnterGridSizer->Fit (generalTreeEnterPanel);
	generalTreeEnterGridSizer->SetSizeHints (generalTreeEnterPanel);
	generalTreeFlexGridSizer->Add (
	   generalTreeEnterPanel, 1, wxALL | wxEXPAND, 2);
	generalTreeStaticLine = new wxStaticLine (generalTreePanel,
	      ID_GENERAL_TREE_STATICLINE,
	      wxDefaultPosition,
	      wxSize (2, -1),
	      wxLI_HORIZONTAL,
	      _T ("ID_GENERAL_TREE_STATICLINE") );
	generalTreeFlexGridSizer->Add (
	   generalTreeStaticLine, 1, wxALL | wxEXPAND, 2);
	generalTreeFlexGridSizer->Add (-1, -1, 1, wxALL | wxEXPAND, 2);
	Panel1 =
	   new wxPanel (generalTreePanel, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T ("ID_PANEL1") );
	BoxSizer1 = new wxBoxSizer (wxHORIZONTAL);
	generalTreeEnterFromGTRButton = new wxButton (Panel1,
	      ID_GENERAL_TREE_ENTER_FROM_GTR_BUTTON,
	      _ ("From GTR File"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GENERAL_TREE_ENTER_FROM_GTR_BUTTON") );
	generalTreeEnterFromGTRButton->SetToolTip (
	   _ ("Enter a general tree from a textfile by specifying a node and its children") );
	BoxSizer1->Add (generalTreeEnterFromGTRButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	generalTreesEnterFromGMLButton = new wxButton (Panel1,
	      ID_GENERAL_TREES_ENTER_FROM_GML_BUTTON,
	      _ ("From GML File"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GENERAL_TREES_ENTER_FROM_GML_BUTTON") );
	generalTreesEnterFromGMLButton->SetToolTip (_ ("Enter a general tree stored in a GML file") );
	BoxSizer1->Add (generalTreesEnterFromGMLButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	Panel1->SetSizer (BoxSizer1);
	BoxSizer1->Fit (Panel1);
	BoxSizer1->SetSizeHints (Panel1);
	generalTreeFlexGridSizer->Add (Panel1, 1, wxALL | wxEXPAND, 1);
	generalTreePanel->SetSizer (generalTreeFlexGridSizer);
	generalTreeFlexGridSizer->Fit (generalTreePanel);
	generalTreeFlexGridSizer->SetSizeHints (generalTreePanel);
	heapGeneralTreeBoxSizer->Add (
	   generalTreePanel, 1, wxALL | wxEXPAND, 2);
	heapGeneralTreePanel->SetSizer (heapGeneralTreeBoxSizer);
	heapGeneralTreeBoxSizer->Fit (heapGeneralTreePanel);
	heapGeneralTreeBoxSizer->SetSizeHints (heapGeneralTreePanel);
	binaryTreesFlexGridSizer->Add (
	   heapGeneralTreePanel, 1, wxALL | wxEXPAND, 0);
	treesPanel->SetSizer (binaryTreesFlexGridSizer);
	binaryTreesFlexGridSizer->Fit (treesPanel);
	binaryTreesFlexGridSizer->SetSizeHints (treesPanel);

	// GRAPH PAGE

	graphsPanel = new wxPanel (discreteMathCalculatorNotebook,
	                           ID_GRAPHS_PANEL,
	                           wxDefaultPosition,
	                           wxDefaultSize,
	                           wxTAB_TRAVERSAL,
	                           _T ("ID_GRAPHS_PANEL") );
	graphPanelFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	graphPanelFlexGridSizer->AddGrowableCol (0);
	graphPanelFlexGridSizer->AddGrowableRow (0);

	// setup the preview panel with slider

	graphsPreviewPanel = new wxPanel (graphsPanel,
	                                  ID_GRAPHS_PREVIEW_PANEL,
	                                  wxDefaultPosition,
	                                  wxDefaultSize,
	                                  wxTAB_TRAVERSAL,
	                                  _T ("ID_GRAPHS_PREVIEW_PANEL") );
	graphsPreviewPanelFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	graphsPreviewPanelFlexGridSizer->AddGrowableCol (0);
	graphsPreviewPanelFlexGridSizer->AddGrowableRow (0);
	previewWindow = new DrawingCanvas (&m_canvas,
	                                   graphsPreviewPanel,
	                                   ID_GRAPHS_PREVIEW_WINDOW,
	                                   wxDefaultPosition,
	                                   wxDefaultSize,
	                                   wxHSCROLL | wxVSCROLL);
	graphsPreviewPanelFlexGridSizer->Add (
	   previewWindow, 1, wxALL | wxEXPAND, 1);
	graphsPreviewSlider = new wxSlider (graphsPreviewPanel,
	                                    ID_GRAPHS_PREVIEW_SLIDER,
	                                    50,
	                                    2,
	                                    99,
	                                    wxDefaultPosition,
	                                    wxDefaultSize,
	                                    0,
	                                    wxDefaultValidator,
	                                    _T ("ID_GRAPHS_PREVIEW_SLIDER") );
	graphsPreviewSlider->SetToolTip (_ ("Set canvas scale") );
	graphsPreviewPanelFlexGridSizer->Add (
	   graphsPreviewSlider, 1, wxALL | wxEXPAND, 1);
	graphsPreviewPanel->SetSizer (graphsPreviewPanelFlexGridSizer);
	graphsPreviewPanelFlexGridSizer->Fit (graphsPreviewPanel);
	graphsPreviewPanelFlexGridSizer->SetSizeHints (graphsPreviewPanel);
	graphPanelFlexGridSizer->Add (
	   graphsPreviewPanel, 1, wxALL | wxEXPAND, 1);

	// setup panel that determines graph type and creates the graph

	graphControlPanel = new wxPanel (graphsPanel,
	                                 ID_GRAPHS_BUTTON_PANEL,
	                                 wxDefaultPosition,
	                                 wxDefaultSize,
	                                 wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	                                 _T ("ID_GRAPHS_BUTTON_PANEL") );
	graphControlPanelFlexGridSizer = new wxFlexGridSizer (1, 5, 0, 0);
	graphControlPanelFlexGridSizer->AddGrowableRow (0);
	graphControlPanelFlexGridSizer->AddGrowableCol (1);
	graphControlPanelFlexGridSizer->AddGrowableCol (2);
	graphControlPanelFlexGridSizer->AddGrowableCol (3);
	graphControlPanelFlexGridSizer->AddGrowableCol (4);
	graphsTypePanel = new wxPanel (graphControlPanel,
	                               ID_GRAPHS_TYPE_PANEL,
	                               wxDefaultPosition,
	                               wxDefaultSize,
	                               wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	                               _T ("ID_GRAPHS_TYPE_PANEL") );
	graphsTypePanelFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	graphsTypePanelFlexGridSizer->AddGrowableCol (1);
	wxString __wxRadioBoxChoices_3[2] = { _ ("Undirected"), _ ("DiGraph") };
	graphsGraphTypeRadioBox = new wxRadioBox (graphsTypePanel,
	      ID_GRAPHS_TYPE_RADIOBOX,
	      _ ("Type"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      2,
	      __wxRadioBoxChoices_3,
	      1,
	      wxRA_HORIZONTAL,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_TYPE_RADIOBOX") );
	graphsGraphTypeRadioBox->SetToolTip (_ ("Specify a directed graph (digraph) or an undirected graph") );
	graphsTypePanelFlexGridSizer->Add (
	   graphsGraphTypeRadioBox, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsWeightedLayoutPanel = new wxPanel (graphsTypePanel,
	      ID_GRAPHS_WEIGHTED_LAYOUT_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_GRAPHS_WEIGHTED_LAYOUT_PANEL") );
	graphsWeightedLayoutFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	graphsWeightedLayoutFlexGridSizer->AddGrowableCol (1);
	graphsWeightedLayoutFlexGridSizer->AddGrowableRow (0);
	graphsWeightedPanel = new wxPanel (graphsWeightedLayoutPanel,
	                                   ID_GRAPHS_LAYOUT_PANEL,
	                                   wxDefaultPosition,
	                                   wxDefaultSize,
	                                   wxTAB_TRAVERSAL,
	                                   _T ("ID_GRAPHS_LAYOUT_PANEL") );
	graphsWeightedFlexGridSizer = new wxFlexGridSizer (9, 1, 0, 0);
	graphsWeightedFlexGridSizer->AddGrowableRow (0);
	graphsWeightedFlexGridSizer->AddGrowableRow (3);
	graphsWeightedFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	graphsWeightedStaticText = new wxStaticText (graphsWeightedPanel,
	      ID_GRAPHS_WEIGHTED_STATICTEXT,
	      _ ("Weighted"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_GRAPHS_WEIGHTED_STATICTEXT") );
	graphsWeightedFlexGridSizer->Add (
	   graphsWeightedStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsWeightedCheckBox = new wxCheckBox (graphsWeightedPanel,
	      ID_GRAPHS_WEIGHTED_CHECKBOX,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_WEIGHTED_CHECKBOX") );
	graphsWeightedCheckBox->SetValue (false);
	graphsWeightedCheckBox->SetToolTip (_ ("Check if the edges in the graph are weighted") );
	graphsWeightedFlexGridSizer->Add (
	   graphsWeightedCheckBox, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsWeightedFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	graphsAllowParallelEdgesStaticText = new wxStaticText (graphsWeightedPanel,
	      ID_GRAPHS_ALLOW_PARALLEL_EDGES_STATICTEXT,
	      _ ("Allow Parallel\nEdges"),
	      wxDefaultPosition,
	      wxSize (100, 35),
	      wxALIGN_CENTRE,
	      _T ("ID_GRAPHS_ALLOW_PARALLEL_EDGES_STATICTEXT") );
	graphsWeightedFlexGridSizer->Add (
	   graphsAllowParallelEdgesStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAllowParallelEdgesCheckBox = new wxCheckBox (graphsWeightedPanel,
	      ID_GRAPHS_ALLOW_PARALLEL_EDGES_CHECKBOX,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_ALLOW_PARALLEL_EDGES_CHECKBOX") );
	graphsAllowParallelEdgesCheckBox->SetValue (false);
	graphsAllowParallelEdgesCheckBox->SetToolTip (_ ("Check if parallel edges are allowed") );
	graphsWeightedFlexGridSizer->Add (
	   graphsAllowParallelEdgesCheckBox, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsWeightedFlexGridSizer->Add (
	   -1, -1, 1, wxALL | wxEXPAND, 1);
	graphsHideEdgesStaticText = new wxStaticText (graphsWeightedPanel,
	      ID_GRAPHS_HIDE_EDGES_STATICTEXT,
	      _ ("Hide Edges"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_GRAPHS_HIDE_EDGES_STATICTEXT") );
	graphsWeightedFlexGridSizer->Add (
	   graphsHideEdgesStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsHideEdgesCheckBox = new wxCheckBox (graphsWeightedPanel,
	      ID_GRAPHS_HIDE_EDGES_CHECKBOX,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_HIDE_EDGES_CHECKBOX") );
	graphsHideEdgesCheckBox->SetValue (false);
	graphsHideEdgesCheckBox->SetToolTip (_ ("Check to hide the edges in a large graph") );
	graphsWeightedFlexGridSizer->Add (
	   graphsHideEdgesCheckBox, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsWeightedPanel->SetSizer (graphsWeightedFlexGridSizer);
	graphsWeightedFlexGridSizer->Fit (graphsWeightedPanel);
	graphsWeightedFlexGridSizer->SetSizeHints (graphsWeightedPanel);
	graphsWeightedLayoutFlexGridSizer->Add (
	   graphsWeightedPanel, 1, wxALL | wxEXPAND, 1);
	graphsLayoutPanel = new wxPanel (graphsWeightedLayoutPanel,
	                                 ID_GRAPHS_LAYOUT_ALGORITHM_PANEL,
	                                 wxDefaultPosition,
	                                 wxDefaultSize,
	                                 wxTAB_TRAVERSAL,
	                                 _T ("ID_GRAPHS_LAYOUT_ALGORITHM_PANEL") );
	graphsLayoutFlexGridSizer = new wxFlexGridSizer (2, 1, 0, 0);
	graphsLayoutAlgorithmStaticText = new wxStaticText (graphsLayoutPanel,
	      ID_GRAPH_LAYOUT_ALGORITHM_STATICTEXT,
	      _ ("Layout"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_GRAPH_LAYOUT_ALGORITHM_STATICTEXT") );
	graphsLayoutFlexGridSizer->Add (
	   graphsLayoutAlgorithmStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	wxString __wxRadioBoxChoices_4[4] = { _ ("Hierarchical"), _ ("Force-directed"), _ ("Orthogonal"), _ ("Circular") };
	graphLayoutRadioBox = new wxRadioBox (graphsLayoutPanel,
	                                      ID_GRAPH_LAYOUT_RADIOBOX,
	                                      wxEmptyString,
	                                      wxDefaultPosition,
	                                      wxDefaultSize,
	                                      4,
	                                      __wxRadioBoxChoices_4,
	                                      1,
	                                      wxRA_HORIZONTAL,
	                                      wxDefaultValidator,
	                                      _T ("ID_GRAPH_LAYOUT_RADIOBOX") );
	graphLayoutRadioBox->SetToolTip (_ ("Choose one of four graph layout algorithms") );
	graphsLayoutFlexGridSizer->Add (graphLayoutRadioBox, 1, wxALL | wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL, 1);
	graphsLayoutPanel->SetSizer (graphsLayoutFlexGridSizer);
	graphsLayoutFlexGridSizer->Fit (graphsLayoutPanel);
	graphsLayoutFlexGridSizer->SetSizeHints (graphsLayoutPanel);
	graphsWeightedLayoutFlexGridSizer->Add (
	   graphsLayoutPanel, 1, wxALL | wxEXPAND, 1);
	graphsWeightedLayoutPanel->SetSizer (graphsWeightedLayoutFlexGridSizer);
	graphsWeightedLayoutFlexGridSizer->Fit (graphsWeightedLayoutPanel);
	graphsWeightedLayoutFlexGridSizer->SetSizeHints (graphsWeightedLayoutPanel);
	graphsTypePanelFlexGridSizer->Add (
	   graphsWeightedLayoutPanel, 1, wxALL | wxEXPAND, 1);
	graphsTypePanel->SetSizer (graphsTypePanelFlexGridSizer);
	graphsTypePanelFlexGridSizer->Fit (graphsTypePanel);
	graphsTypePanelFlexGridSizer->SetSizeHints (graphsTypePanel);
	graphControlPanelFlexGridSizer->Add (
	   graphsTypePanel, 1, wxALL | wxEXPAND, 1);
	graphsEnterPanel = new wxPanel (graphControlPanel,
	                                ID_GRAPHS_ENTER_PANEL,
	                                wxDefaultPosition,
	                                wxDefaultSize,
	                                wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	                                _T ("ID_GRAPHS_ENTER_PANEL") );
	graphsEnterGridSizer = new wxGridSizer (3, 1, 0, 0);
	graphsEnterFromPanel = new wxPanel (graphsEnterPanel,
	                                    ID_GRAPHS_ENTER_FROM_PANEL,
	                                    wxDefaultPosition,
	                                    wxDefaultSize,
	                                    wxTAB_TRAVERSAL,
	                                    _T ("ID_GRAPHS_ENTER_FROM_PANEL") );
	graphsEnterFromFlexGridSizer = new wxFlexGridSizer (0, 5, 0, 0);
	graphsEnterFromDialogButton = new wxButton (graphsEnterFromPanel,
	      ID_GRAPHS_ENTER_FROM_DIALOG_BUTTON,
	      _ ("Enter"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_ENTER_FROM_DIALOG_BUTTON") );
	graphsEnterFromDialogButton->SetToolTip (_ ("Specify a graph by entering vertex pairs") );
	graphsEnterFromFlexGridSizer->Add (
	   graphsEnterFromDialogButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsEnterFromGRPHButton = new wxButton (graphsEnterFromPanel,
	      ID_GRAPHS_ENTER_FROM_GRPH_BUTTON,
	      _ ("From GRPH"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_ENTER_FROM_GRPH_BUTTON") );
	graphsEnterFromGRPHButton->SetToolTip (_ ("Read a textfile containing graph edges and draw the graph") );
	graphsEnterFromFlexGridSizer->Add (
	   graphsEnterFromGRPHButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsEnterFromGMLButton = new wxButton (graphsEnterFromPanel,
	      ID_GRAPHS_ENTER_FROM_GML_BUTTON,
	      _ ("From GML"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_ENTER_FROM_GML_BUTTON") );
	graphsEnterFromGMLButton->SetToolTip (_ ("Draw a graph stored in GML format") );
	graphsEnterFromFlexGridSizer->Add (
	   graphsEnterFromGMLButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsEnterFromTSP = new wxButton (graphsEnterFromPanel,
	                                   ID_GRAPHS_FROM_TSP_BUTTON,
	                                   _ ("From TSP"),
	                                   wxDefaultPosition,
	                                   wxDefaultSize,
	                                   0,
	                                   wxDefaultValidator,
	                                   _T ("ID_GRAPHS_FROM_TSP_BUTTON") );
	graphsEnterFromTSP->SetToolTip (_ ("Draw a graph stored in TSP format") );
	graphsEnterFromFlexGridSizer->Add (
	   graphsEnterFromTSP, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsEnterFromPanel->SetSizer (graphsEnterFromFlexGridSizer);
	graphsEnterFromFlexGridSizer->Fit (graphsEnterFromPanel);
	graphsEnterFromFlexGridSizer->SetSizeHints (graphsEnterFromPanel);
	graphsEnterGridSizer->Add (graphsEnterFromPanel, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsRedrawRandomPanel = new wxPanel (graphsEnterPanel,
	                                       ID_REDRAW_RANDOM_PANEL,
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       wxTAB_TRAVERSAL,
	                                       _T ("ID_REDRAW_RANDOM_PANEL") );
	GridSizer1 = new wxGridSizer (0, 3, 0, 0);
	GridSizer1->Add (-1, -1, 1, wxALL | wxEXPAND, 1);
	graphRedrawButton = new wxButton (graphsRedrawRandomPanel,
	                                  ID_GRAPH_REDRAW_BUTTON,
	                                  _ ("Redraw"),
	                                  wxDefaultPosition,
	                                  wxDefaultSize,
	                                  0,
	                                  wxDefaultValidator,
	                                  _T ("ID_GRAPH_REDRAW_BUTTON") );
	graphRedrawButton->SetToolTip (_ ("Draw the current graph using another layout algorithm") );
	GridSizer1->Add (graphRedrawButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	GridSizer1->Add (-1, -1, 1, wxALL | wxEXPAND, 1);
	graphsRedrawRandomPanel->SetSizer (GridSizer1);
	GridSizer1->Fit (graphsRedrawRandomPanel);
	GridSizer1->SetSizeHints (graphsRedrawRandomPanel);
	graphsEnterGridSizer->Add (
	   graphsRedrawRandomPanel, 1, wxALL | wxEXPAND, 1);
	graphsEnterPanel->SetSizer (graphsEnterGridSizer);
	graphsEnterGridSizer->Fit (graphsEnterPanel);
	graphsEnterGridSizer->SetSizeHints (graphsEnterPanel);
	graphControlPanelFlexGridSizer->Add (
	   graphsEnterPanel, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddEraseVertexPanel = new wxPanel (graphControlPanel,
	      ID_GRAPHS_ADD_ERASE_VERTEX_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	      _T ("ID_GRAPHS_ADD_ERASE_VERTEX_PANEL") );
	graphsAddEraseVertexBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	graphsAddEraseVertexButtonPanel = new wxPanel (graphsAddEraseVertexPanel,
	      ID_GRAPHS_ADD_ERASE_VERTEX_BUTTON_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_GRAPHS_ADD_ERASE_VERTEX_BUTTON_PANEL") );
	graphsAddEraseButtonFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);
	graphsAddEraseVertexStaticText = new wxStaticText (graphsAddEraseVertexButtonPanel,
	      ID_GRAPHS_ADD_ERASE_VERTEX_STATICTEXT,
	      _ ("Vertex"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_GRAPHS_ADD_ERASE_VERTEX_STATICTEXT") );
	graphsAddEraseButtonFlexGridSizer->Add (
	   graphsAddEraseVertexStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddVertexButton = new wxButton (graphsAddEraseVertexButtonPanel,
	                                      ID_GRAPHS_ADD_VERTEX_BUTTON,
	                                      _ ("Add"),
	                                      wxDefaultPosition,
	                                      wxDefaultSize,
	                                      0,
	                                      wxDefaultValidator,
	                                      _T ("ID_GRAPHS_ADD_VERTEX_BUTTON") );
	graphsAddVertexButton->SetToolTip (_ ("Add a vertex to the graph") );
	graphsAddEraseButtonFlexGridSizer->Add (
	   graphsAddVertexButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsEraseVertexButton = new wxButton (graphsAddEraseVertexButtonPanel,
	                                        ID_GRAPHS_ERASE_VERTEX_BUTTON,
	                                        _ ("Erase"),
	                                        wxDefaultPosition,
	                                        wxDefaultSize,
	                                        0,
	                                        wxDefaultValidator,
	                                        _T ("ID_GRAPHS_ERASE_VERTEX_BUTTON") );
	graphsEraseVertexButton->SetToolTip (_ ("Remove a vertex and all its associated edges from the graph") );
	graphsAddEraseButtonFlexGridSizer->Add (
	   graphsEraseVertexButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddEraseVertexButtonPanel->SetSizer (graphsAddEraseButtonFlexGridSizer);
	graphsAddEraseButtonFlexGridSizer->Fit (graphsAddEraseVertexButtonPanel);
	graphsAddEraseButtonFlexGridSizer->SetSizeHints (graphsAddEraseVertexButtonPanel);
	graphsAddEraseVertexBoxSizer->Add (
	   graphsAddEraseVertexButtonPanel, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddEraseVertexTextCtrl = new wxTextCtrl (graphsAddEraseVertexPanel,
	      ID_GRAPHS_ADD_ERASE_VERTEX_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxSize (20, 20),
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_ADD_ERASE_VERTEX_TEXTCTRL") );
	graphsAddEraseVertexBoxSizer->Add (
	   graphsAddEraseVertexTextCtrl, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddEraseVertexPanel->SetSizer (graphsAddEraseVertexBoxSizer);
	graphsAddEraseVertexBoxSizer->Fit (graphsAddEraseVertexPanel);
	graphsAddEraseVertexBoxSizer->SetSizeHints (graphsAddEraseVertexPanel);
	graphControlPanelFlexGridSizer->Add (
	   graphsAddEraseVertexPanel, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddEraseEdgePanel = new wxPanel (graphControlPanel,
	                                       ID_ADD_ERASE_EDGE_PANEL,
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	                                       _T ("ID_ADD_ERASE_EDGE_PANEL") );
	graphsAddEraseEdgeBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	graphsAddEraseEdgeButtonsPanel = new wxPanel (graphsAddEraseEdgePanel,
	      ID_GRAPHS_ADD_ERASE_EDGE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_GRAPHS_ADD_ERASE_EDGE_PANEL") );
	graphsAddEraseEdgeFlexGridSizer = new wxFlexGridSizer (3, 1, 0, 0);
	graphsAddEraseEdgeFlexGridSizer->AddGrowableCol (0);
	graphsAddEraseEdgeFlexGridSizer->AddGrowableRow (0);
	graphsAddEraseEdgeStaticText = new wxStaticText (graphsAddEraseEdgeButtonsPanel,
	      ID_GRAPHS_ADD_ERASE_EDGE_STATICTEXT,
	      _ ("Edge"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_GRAPHS_ADD_ERASE_EDGE_STATICTEXT") );
	graphsAddEraseEdgeFlexGridSizer->Add (
	   graphsAddEraseEdgeStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddEdgeButton = new wxButton (graphsAddEraseEdgeButtonsPanel,
	                                    ID_GRAPHS_ADD_EDGE_BUTTON,
	                                    _ ("Add"),
	                                    wxDefaultPosition,
	                                    wxDefaultSize,
	                                    0,
	                                    wxDefaultValidator,
	                                    _T ("ID_GRAPHS_ADD_EDGE_BUTTON") );
	graphsAddEdgeButton->SetToolTip (_ ("Add an edge between two existing vertices") );
	graphsAddEraseEdgeFlexGridSizer->Add (
	   graphsAddEdgeButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsEraseEdgeButton = new wxButton (graphsAddEraseEdgeButtonsPanel,
	                                      ID_GRAPHS_ERASE_EDGE_BUTTON,
	                                      _ ("Erase"),
	                                      wxDefaultPosition,
	                                      wxDefaultSize,
	                                      0,
	                                      wxDefaultValidator,
	                                      _T ("ID_GRAPHS_ERASE_EDGE_BUTTON") );
	graphsEraseEdgeButton->SetToolTip (_ ("Remove an edge between two existing vertices") );
	graphsAddEraseEdgeFlexGridSizer->Add (
	   graphsEraseEdgeButton, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddEraseEdgeButtonsPanel->SetSizer (graphsAddEraseEdgeFlexGridSizer);
	graphsAddEraseEdgeFlexGridSizer->Fit (graphsAddEraseEdgeButtonsPanel);
	graphsAddEraseEdgeFlexGridSizer->SetSizeHints (graphsAddEraseEdgeButtonsPanel);
	graphsAddEraseEdgeBoxSizer->Add (
	   graphsAddEraseEdgeButtonsPanel, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddEraseEdgeChoicePanel = new wxPanel (graphsAddEraseEdgePanel,
	      ID_GRAPHS_ADD_ERASE_EDGE_CHOICE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_GRAPHS_ADD_ERASE_EDGE_CHOICE_PANEL") );
	graphsAddEraseEdgeChoiceBoxSizer = new wxBoxSizer (wxVERTICAL);
	graphsAddEraseEdgeSourcePanel = new wxPanel (graphsAddEraseEdgeChoicePanel,
	      ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_PANEL") );
	graphsAddEraseEdgeSourceFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	graphsAddEraseEdgeSourceFlexGridSizer->AddGrowableCol (1);
	graphsAddEraseEdgeSourceStaticText = new wxStaticText (graphsAddEraseEdgeSourcePanel,
	      ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_STATICTEXT,
	      _ ("Src"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_STATICTEXT") );
	graphsAddEraseEdgeSourceFlexGridSizer->Add (
	   graphsAddEraseEdgeSourceStaticText, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddEraseEdgeSourceTextCtrl = new wxTextCtrl (graphsAddEraseEdgeSourcePanel,
	      ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_TEXTCTRL") );
	graphsAddEraseEdgeSourceFlexGridSizer->Add (
	   graphsAddEraseEdgeSourceTextCtrl, 1, wxALL | wxEXPAND, 1);
	graphsAddEraseEdgeSourcePanel->SetSizer (graphsAddEraseEdgeSourceFlexGridSizer);
	graphsAddEraseEdgeSourceFlexGridSizer->Fit (graphsAddEraseEdgeSourcePanel);
	graphsAddEraseEdgeSourceFlexGridSizer->SetSizeHints (graphsAddEraseEdgeSourcePanel);
	graphsAddEraseEdgeChoiceBoxSizer->Add (
	   graphsAddEraseEdgeSourcePanel, 1, wxALL | wxEXPAND, 1);
	graphsAddEraseEdgeDestPanel = new wxPanel (graphsAddEraseEdgeChoicePanel,
	      ID_GRAPHS_ADD_ERASE_EDGE_DEST_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_GRAPHS_ADD_ERASE_EDGE_DEST_PANEL") );
	graphsAddEraseEdgeDestFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	graphsAddEraseEdgeDestFlexGridSizer->AddGrowableCol (1);
	graphsAddEraseEdgeDestFlexGridSizer->AddGrowableRow (0);
	graphsAddEraseEdgeDestStaticText = new wxStaticText (graphsAddEraseEdgeDestPanel,
	      ID_GRAPHS_ADD_ERASE_EDGE_DEST_STATICTEXT,
	      _ ("Dest"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_GRAPHS_ADD_ERASE_EDGE_DEST_STATICTEXT") );
	graphsAddEraseEdgeDestFlexGridSizer->Add (
	   graphsAddEraseEdgeDestStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphsAddEraseEdgeDestTextCtrl = new wxTextCtrl (graphsAddEraseEdgeDestPanel,
	      ID_GRAPHS_ADD_ERASE_EDGE_DEST_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_ADD_ERASE_EDGE_DEST_TEXTCTRL") );
	graphsAddEraseEdgeDestFlexGridSizer->Add (
	   graphsAddEraseEdgeDestTextCtrl, 1, wxALL | wxEXPAND, 1);
	graphsAddEraseEdgeDestPanel->SetSizer (graphsAddEraseEdgeDestFlexGridSizer);
	graphsAddEraseEdgeDestFlexGridSizer->Fit (graphsAddEraseEdgeDestPanel);
	graphsAddEraseEdgeDestFlexGridSizer->SetSizeHints (graphsAddEraseEdgeDestPanel);
	graphsAddEraseEdgeChoiceBoxSizer->Add (
	   graphsAddEraseEdgeDestPanel, 1, wxALL | wxEXPAND, 1);
	graphEdgeWeightPanel = new wxPanel (graphsAddEraseEdgeChoicePanel,
	                                    ID_GRAPH_EDGE_WEIGHT_PANEL,
	                                    wxDefaultPosition,
	                                    wxDefaultSize,
	                                    wxTAB_TRAVERSAL,
	                                    _T ("ID_GRAPH_EDGE_WEIGHT_PANEL") );
	graphEdgeWeightFlexGridSizer = new wxFlexGridSizer (0, 2, 0, 0);
	graphEdgeWeightStaticText = new wxStaticText (graphEdgeWeightPanel,
	      ID_GRAPH_EDGE_WEIGHT_STATICTEXT,
	      _ ("Weight"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      _T ("ID_GRAPH_EDGE_WEIGHT_STATICTEXT") );
	graphEdgeWeightFlexGridSizer->Add (
	   graphEdgeWeightStaticText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphEdgeWeightTextCtrl = new wxTextCtrl (graphEdgeWeightPanel,
	      ID_GRAPH_EDGE_WEIGHT_TEXTCTRL,
	      wxEmptyString,
	      wxDefaultPosition,
	      wxSize(35,-1),
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPH_EDGE_WEIGHT_TEXTCTRL") );
	graphEdgeWeightFlexGridSizer->Add (graphEdgeWeightTextCtrl, 1, wxALL | wxALIGN_LEFT | wxALIGN_BOTTOM, 1);
	graphEdgeWeightPanel->SetSizer (graphEdgeWeightFlexGridSizer);
	graphEdgeWeightFlexGridSizer->Fit (graphEdgeWeightPanel);
	graphEdgeWeightFlexGridSizer->SetSizeHints (graphEdgeWeightPanel);
	graphsAddEraseEdgeChoiceBoxSizer->Add (graphEdgeWeightPanel, 1, wxALL | wxEXPAND, 1);
	graphsAddEraseEdgeChoicePanel->SetSizer (graphsAddEraseEdgeChoiceBoxSizer);
	graphsAddEraseEdgeChoiceBoxSizer->Fit (graphsAddEraseEdgeChoicePanel);
	graphsAddEraseEdgeChoiceBoxSizer->SetSizeHints (graphsAddEraseEdgeChoicePanel);
	graphsAddEraseEdgeBoxSizer->Add (
	   graphsAddEraseEdgeChoicePanel, 1, wxALL, 1);
	graphsAddEraseEdgePanel->SetSizer (graphsAddEraseEdgeBoxSizer);
	graphsAddEraseEdgeBoxSizer->Fit (graphsAddEraseEdgePanel);
	graphsAddEraseEdgeBoxSizer->SetSizeHints (graphsAddEraseEdgePanel);
	graphControlPanelFlexGridSizer->Add (
	   graphsAddEraseEdgePanel, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);
	graphPanelFlexGridSizer->Add (
	   graphControlPanel, 1, wxALL | wxEXPAND, 1);

	// setup action panel buttons

	wxPanel* graphActionPanel;
	wxGridSizer* graphActionPanelGridSizer;

	graphActionPanel = new wxPanel (graphsPanel,
	                                ID_GRAPH_BUTTON_PANEL,
	                                wxDefaultPosition,
	                                wxDefaultSize,
	                                wxSIMPLE_BORDER | wxTAB_TRAVERSAL,
	                                _T ("ID_GRAPH_BUTTON_PANEL") );
	graphPanelFlexGridSizer->Add (graphActionPanel, 1, wxALL | wxEXPAND, 1);
	graphActionPanelGridSizer = new wxGridSizer (0, 7, 0, 0);
	graphActionPanel->SetSizer (graphActionPanelGridSizer);

	clearGraphButton = new wxButton (graphActionPanel,
	                                 ID_CLEAR_GRAPH_BUTTON,
	                                 _ ("Clear"),
	                                 wxDefaultPosition,
	                                 wxDefaultSize,
	                                 0,
	                                 wxDefaultValidator,
	                                 _T ("ID_CLEAR_GRAPH_BUTTON") );
	clearGraphButton->SetToolTip (_ ("Clear all the graph components") );
	graphActionPanelGridSizer->Add (clearGraphButton, 1, wxALL | wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL, 1);

	analyzeGraphButton = new wxButton (graphActionPanel,
	                                   ID_ANALYZE_GRAPH_BUTTON,
	                                   _ ("Analyze Graph"),
	                                   wxDefaultPosition,
	                                   wxDefaultSize,
	                                   0,
	                                   wxDefaultValidator,
	                                   _T ("ID_ANALYZE_GRAPH_BUTTON") );
	analyzeGraphButton->SetToolTip (_ ("Display properties of the graph") );
	graphActionPanelGridSizer->Add (analyzeGraphButton, 1, wxALL | wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL, 1);

	transposeGraphButton = new wxButton (graphActionPanel,
	                                     ID_TRANSPOSE_GRAPH_BUTTON,
	                                     _ ("Transpose Graph"),
	                                     wxDefaultPosition,
	                                     wxDefaultSize,
	                                     0,
	                                     wxDefaultValidator,
	                                     _T ("ID_TRANSPOSE_GRAPH_BUTTON") );
	transposeGraphButton->SetToolTip (_ ("Reverse the edge directions in the digraph") );
	graphActionPanelGridSizer->Add (transposeGraphButton, 1, wxALL | wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL, 1);

	complementaryGraphButton = new wxButton (graphActionPanel,
	      ID_COMPLEMENTARY_GRAPH_BUTTON,
	      _ ("Complementary Graph"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_COMPLEMENTARY_GRAPH_BUTTON") );
	complementaryGraphButton->SetToolTip (_ ("Construct the undirected graph with the same vertex set but whose edge set consists of the edges not present in the original undirected graph") );
	graphActionPanelGridSizer->Add (complementaryGraphButton, 1, wxALL | wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL, 1);

	graphRunAlgorithmsButton = new wxButton (graphActionPanel,
	      ID_GRAPH_RUN_ALGORITHMS_BUTTON,
	      _ ("Run Algorithms"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_GRAPHS_RUN_ALGORITHMS_BUTTON") );
	graphRunAlgorithmsButton->SetToolTip (_ ("Run one or more algorithms using the current graph") );
	graphActionPanelGridSizer->Add (graphRunAlgorithmsButton, 1, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 1);

	subGraphIsomorphismButton = new wxButton (graphActionPanel,
	      ID_SUBGRAPH_ISOMORPHISM_BUTTON,
	      _ ("Subgraph isomorphism"),
	      wxDefaultPosition,
	      wxDefaultSize,
	      0,
	      wxDefaultValidator,
	      _T ("ID_SUBGRAPH_ISOMORPHISM_BUTTON") );
	subGraphIsomorphismButton->SetToolTip (_ ("Determine if a graph G1 is isomorphic to a subset of graph G2") );
	graphActionPanelGridSizer->Add (subGraphIsomorphismButton, 1, wxALL | wxALIGN_CENTER | wxALIGN_CENTER_HORIZONTAL, 1);

	graphIsomorphismButton = new wxButton (graphActionPanel,
	                                       ID_GRAPH_ISOMORPHISM_BUTTON,
	                                       _ ("Graph isomorphism"),
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       0,
	                                       wxDefaultValidator,
	                                       _T ("ID_GRAPH_ISOMORPHISM_BUTTON") );
	graphIsomorphismButton->SetToolTip (_ ("Determine if two graphs are isomorphic") );
	graphActionPanelGridSizer->Add (graphIsomorphismButton, 1, wxALL | wxALIGN_CENTER | wxALIGN_CENTER_HORIZONTAL, 1);

	graphControlPanel->SetSizer (graphControlPanelFlexGridSizer);
	graphControlPanelFlexGridSizer->Fit (graphControlPanel);
	graphControlPanelFlexGridSizer->SetSizeHints (graphControlPanel);
	graphsPanel->SetSizer (graphPanelFlexGridSizer);
	graphPanelFlexGridSizer->Fit (graphsPanel);
	graphPanelFlexGridSizer->SetSizeHints (graphsPanel);

	// RELATIONS AND PARTIAL ORDER PAGE

	relationsAndPartialOrdersPanel = new wxPanel (discreteMathCalculatorNotebook,
	      ID_RELATIONS_AND_PARTIALORDERS_PANEL,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxTAB_TRAVERSAL,
	      _T ("ID_RELATIONS_AND_PARTIALORDERS_PANEL") );

	RelationsPartialOrdersButtonsPanel = new wxPanel (relationsAndPartialOrdersPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	PartialOrderInputPanel = new wxPanel (RelationsPartialOrdersButtonsPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	RelationClosurePanel = new wxPanel (RelationsPartialOrdersButtonsPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	InputRelationPanel = new wxPanel (RelationsPartialOrdersButtonsPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,100), wxTAB_TRAVERSAL);
	InputRelationButtonsPanel = new wxPanel (InputRelationPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	RelationClosureButtonsPanel = new wxPanel (RelationClosurePanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);
	PartialOrderInputButtonsPanel = new wxPanel (PartialOrderInputPanel, wxID_ANY, wxDefaultPosition, wxSize (-1,-1), wxTAB_TRAVERSAL);

	// reflexive closure button
	RelationReflexiveClosureButton = new wxButton (RelationClosureButtonsPanel, wxID_ANY, _ ("Reflexive"), wxDefaultPosition, wxSize (-1,-1), 0);
	RelationReflexiveClosureButton->SetToolTip (_ ("Display the reflexive closure of the relation") );

	// symmetric closure button
	RelationSymmetricClosureButton = new wxButton (RelationClosureButtonsPanel, wxID_ANY, _ ("Symmetric"), wxDefaultPosition, wxSize (-1,-1), 0);
	RelationSymmetricClosureButton->SetToolTip (_ ("Display the symmetric closure of the relation") );

	// transitive closure button
	RelationTransiveClosureButton = new wxButton (RelationClosureButtonsPanel, wxID_ANY, _ ("Transitive"), wxDefaultPosition, wxSize (-1,-1), 0);
	RelationTransiveClosureButton->SetToolTip (_ ("Display the transitive closure of the relation") );
	// Relation properties button
	RelationPropertiesButton = new wxButton(
		RelationClosureButtonsPanel,
		wxID_ANY,
		_("Properties"),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);

RelationPropertiesButton->SetToolTip(
    _("Determine the mathematical properties of the relation")
);
	// enter Hasse diagram button
	HasseDiagramEnterButton = new wxButton (PartialOrderInputButtonsPanel, wxID_ANY, _ ("Enter"), wxDefaultPosition, wxSize (-1,-1), 0);
	HasseDiagramEnterButton->SetToolTip (_ ("Enter a partial order using one of two means and draw its Hasse diagram") );

	// box sizer for relationsAndPartialOrdersPanel
	wxBoxSizer* RelationsPartialOrdersBoxSizer = new wxBoxSizer (wxVERTICAL);
	relationsAndPartialOrdersPanel->SetSizer (RelationsPartialOrdersBoxSizer);

	// drawing canvas for relations and Hasse diagram
	relationPartialOrderDrawing = new DrawingCanvas (&m_relationCanvas,
	      relationsAndPartialOrdersPanel,
	      ID_RELATIONS_PARTIAL_ORDERS_DRAWING_WINDOW,
	      wxDefaultPosition,
	      wxDefaultSize,
	      wxHSCROLL | wxVSCROLL);
	relationPartialOrderDrawing->SetScrollRate (5, 5);

	// add drawing canvas to RelationsPartialOrdersBoxSizer
	RelationsPartialOrdersBoxSizer->Add (relationPartialOrderDrawing, 5, wxALL|wxEXPAND, 1);

	// create slider for drawing canvas
	relationPartialOrderSlider = new wxSlider (relationsAndPartialOrdersPanel, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize (-1,-1), wxSL_HORIZONTAL);

	// add slider to RelationsPartialOrdersBoxSizer
	RelationsPartialOrdersBoxSizer->Add (relationPartialOrderSlider, 0, wxALL|wxEXPAND, 5);

	// add RelationsPartialOrdersButtonsPanel to
	// RelationsPartialOrdersBoxSizer
	RelationsPartialOrdersBoxSizer->Add(
		RelationsPartialOrdersButtonsPanel, 0, wxALL|wxEXPAND, 1);	wxGridSizer* ButtonsPanelGidSizer = new wxGridSizer (2, 2, 0, 0);

	// set RelationsPartialOrdersButtonsPanel to have sizer
	// ButtonsPanelGidSizer
	RelationsPartialOrdersButtonsPanel->SetSizer (ButtonsPanelGidSizer);

	// add InputRelationPanel to ButtonsPanelGidSizer
	ButtonsPanelGidSizer->Add (InputRelationPanel, 0, wxALL|wxEXPAND, 1);

	// create InputRelationBoxSizer
	wxBoxSizer* InputRelationBoxSizer = new wxBoxSizer (wxVERTICAL);
	// make InputRelationBoxSizer sizer for InputRelationPanel
	InputRelationPanel->SetSizer (InputRelationBoxSizer);

	// create "Relation" static text and add to
	// InputRelationBoxSizer
	InputRelationStaticText = new wxStaticText (InputRelationPanel, wxID_ANY, _ ("Relation"), wxDefaultPosition, wxSize (-1,-1), 0);
	InputRelationBoxSizer->Add (InputRelationStaticText, 0, wxALL|wxALIGN_CENTER, 5);

	// add InputRelationButtonsPanel to InputRelationBoxSizer
	InputRelationBoxSizer->Add (InputRelationButtonsPanel, 0, wxALL|wxALIGN_CENTER, 1);

	// create InputRelationGridSizer and set it as sizer
	// for InputRelationButtonsPanel
	wxGridSizer* InputRelationGridSizer = new wxGridSizer (1, 4, 0, 0);
	InputRelationButtonsPanel->SetSizer (InputRelationGridSizer);

	// add enter relation input button to
	// InputRelationGridSizer
	// enter button
	InputRelationEnterButton = new wxButton (InputRelationButtonsPanel, wxID_ANY, _ ("Enter"), wxDefaultPosition, wxSize (-1,-1), 0);
	InputRelationGridSizer->Add (InputRelationEnterButton, 0, wxALL, WXC_FROM_DIP(5));
	InputRelationEnterButton->SetToolTip (_ ("Enter data for a relation and draw a digraph") );

	// output to GML button
	OutputRelationGMLButton = new wxButton(InputRelationButtonsPanel, wxID_ANY, _("Save as GML"), wxDefaultPosition, wxSize(-1,-1), 0);
	InputRelationGridSizer->Add(OutputRelationGMLButton, 0, wxALL, WXC_FROM_DIP(5));
	OutputRelationGMLButton->SetToolTip (_ ("Save the relation in GML format") );

	// input from GML button
	InputRelationFromGMLButton = new wxButton(InputRelationButtonsPanel, wxID_ANY, _("Input from GML"), wxDefaultPosition, wxDLG_UNIT(InputRelationButtonsPanel, wxSize(-1,-1)), 0);
	InputRelationGridSizer->Add(InputRelationFromGMLButton, 0, wxALL, WXC_FROM_DIP(5));
	InputRelationFromGMLButton->SetToolTip (_ ("Input the relation from GML format") );

	// enter from REL button
	InputRelationRELButton = new wxButton(InputRelationButtonsPanel, wxID_ANY, _("From REL"), wxDefaultPosition, wxDLG_UNIT(InputRelationButtonsPanel, wxSize(-1,-1)), 0);
	InputRelationGridSizer->Add(InputRelationRELButton, 0, wxALL, WXC_FROM_DIP(5));
	InputRelationRELButton->SetToolTip (_ ("Input a relation from .rel format file") );

	// add RelationClosurePanel to ButtonsPanelGidSizer
	ButtonsPanelGidSizer->Add (RelationClosurePanel, 1, wxALL|wxEXPAND, 1);

	// create RelationClosureBoxSizer and set as sizer
	// for RelationClosurePanel
	wxBoxSizer* RelationClosureBoxSizer = new wxBoxSizer (wxVERTICAL);
	RelationClosurePanel->SetSizer (RelationClosureBoxSizer);

	// create static text with name "Closure" and add it
	// to RelationClosureBoxSizer
	RelationClosureStaticText = new wxStaticText (RelationClosurePanel, wxID_ANY, _ ("Closure"), wxDefaultPosition, wxSize (-1,-1), 0);
	RelationClosureBoxSizer->Add (RelationClosureStaticText, 0, wxALL|wxALIGN_CENTER, 5);

	// add RelationClosureButtonsPanel to
	// RelationClosureBoxSizer
	RelationClosureBoxSizer->Add (RelationClosureButtonsPanel, 0, wxALL|wxALIGN_CENTER, 1);

	// create wxGridSizer for the closure buttons
	// and set as the sizer for RelationClosureButtonsPanel
	wxGridSizer* RelationPanelClosureGridSizer = new wxGridSizer (1, 4, 0, 0);
	RelationClosureButtonsPanel->SetSizer (RelationPanelClosureGridSizer);

	// add reflexive closure button to grid sizer
	RelationPanelClosureGridSizer->Add (RelationReflexiveClosureButton, 0, wxALL, 5);

	// add symmetric closure button to grid sizer
	RelationPanelClosureGridSizer->Add (RelationSymmetricClosureButton, 0, wxALL, 5);

	// add transitive closure button to grid sizer
	RelationPanelClosureGridSizer->Add (RelationTransiveClosureButton, 0, wxALL, 5);
	RelationPanelClosureGridSizer->Add(
		RelationPropertiesButton, 0, wxALL, 5);
	// add PartialOrderInputPanel to grid sizer
	ButtonsPanelGidSizer->Add (PartialOrderInputPanel, 0, wxALL|wxEXPAND, 1);

	// create PartialOrderInputPanelBoxSizer
	// and set as sizer for PartialOrderInputPanel
	wxBoxSizer* PartialOrderInputPanelBoxSizer = new wxBoxSizer (wxVERTICAL);
	PartialOrderInputPanel->SetSizer (PartialOrderInputPanelBoxSizer);

	// create static text "Partial Order" and add
	// to PartialOrderInputPanelBoxSizer
	PartialOrderInputPanelStaticText = new wxStaticText (PartialOrderInputPanel, wxID_ANY, _ ("Partial Order"), wxDefaultPosition, wxSize (-1,-1), 0);
	PartialOrderInputPanelBoxSizer->Add (PartialOrderInputPanelStaticText, 0, wxALL|wxALIGN_CENTER, 5);

	// add PartialOrderInputButtonsPanel to
	// PartialOrderInputPanelBoxSizer
	PartialOrderInputPanelBoxSizer->Add (PartialOrderInputButtonsPanel, 0, wxALL|wxALIGN_CENTER, 1);

	// create PartialOrderInputButtonsGridSizer and
	// set it as sizer for PartialOrderInputButtonsPanel
	wxGridSizer* PartialOrderInputButtonsGridSizer = new wxGridSizer (1, 4, 0, 0);
	PartialOrderInputButtonsPanel->SetSizer (PartialOrderInputButtonsGridSizer);

	// add Hasse diagram input button to grid sizer
	PartialOrderInputButtonsGridSizer->Add (HasseDiagramEnterButton, 0, wxALL, 1);

	OutputPartialOrderGMLButton = new wxButton(PartialOrderInputButtonsPanel, wxID_ANY, _("Save as GML"), wxDefaultPosition, wxDLG_UNIT(PartialOrderInputButtonsPanel, wxSize(-1,-1)), 0);
	PartialOrderInputButtonsGridSizer->Add(OutputPartialOrderGMLButton, 0, wxALL, WXC_FROM_DIP(5));
	OutputPartialOrderGMLButton->SetToolTip (_ ("Output the partial order to GML format") );

	// input from GML button
	InputPartialOrderFromGMLButton = new wxButton(PartialOrderInputButtonsPanel, wxID_ANY, _("Input from GML"), wxDefaultPosition, wxDLG_UNIT(PartialOrderInputButtonsPanel, wxSize(-1,-1)), 0);
	PartialOrderInputButtonsGridSizer->Add(InputPartialOrderFromGMLButton, 0, wxALL, WXC_FROM_DIP(5));
	InputPartialOrderFromGMLButton->SetToolTip (_ ("Input the partial order from GML format") );

	// ELLIPTIC CURVE PAGE

	EllipticCurvePanel = new wxPanel (discreteMathCalculatorNotebook,
	                                  ID_GF_P_ELLIPTIC_CURVE_PANEL,
	                                  wxDefaultPosition,
	                                  wxDefaultSize,
	                                  wxTAB_TRAVERSAL,
	                                  _T ("ID_GF_P_ELLIPTIC_CURVE_PANEL") );

	wxFlexGridSizer* ECCFlexGridSizer = new wxFlexGridSizer (4, 1, 0, 0);
	ECCFlexGridSizer->SetFlexibleDirection ( wxBOTH );
	ECCFlexGridSizer->SetNonFlexibleGrowMode ( wxFLEX_GROWMODE_SPECIFIED );
	ECCFlexGridSizer->AddGrowableCol (0);
	ECCFlexGridSizer->AddGrowableRow (1);
	EllipticCurvePanel->SetSizer (ECCFlexGridSizer);

	ECCDataPanel = new wxPanel (EllipticCurvePanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (EllipticCurvePanel, wxSize (-1,-1) ), wxTAB_TRAVERSAL);
	ECCDataPanel->SetToolTip (_ ("Enter the elliptic curve.") );

	ECCFlexGridSizer->Add (ECCDataPanel, 1, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxFlexGridSizer* ECCDataPanelFlexGridSizer = new wxFlexGridSizer (6, 2, 0, 0);
	ECCDataPanelFlexGridSizer->SetFlexibleDirection ( wxBOTH );
	ECCDataPanelFlexGridSizer->SetNonFlexibleGrowMode ( wxFLEX_GROWMODE_SPECIFIED );
	ECCDataPanelFlexGridSizer->AddGrowableCol (1);
	ECCDataPanel->SetSizer (ECCDataPanelFlexGridSizer);

	ECCEllipticCurveLabelStaticText = new wxStaticText (ECCDataPanel, wxID_ANY, _ ("elliptic curve"), wxDefaultPosition, wxDLG_UNIT (ECCDataPanel, wxSize (-1,-1) ), 0);

	ECCDataPanelFlexGridSizer->Add (ECCEllipticCurveLabelStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCEllipticCurvePanel = new wxPanel (ECCDataPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCDataPanel, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	ECCDataPanelFlexGridSizer->Add (ECCEllipticCurvePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* ECCEllipticCurveBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ECCEllipticCurvePanel->SetSizer (ECCEllipticCurveBoxSizer);

	ECCPrimeStaticText = new wxStaticText (ECCEllipticCurvePanel, wxID_ANY, _ ("p:"), wxDefaultPosition, wxDLG_UNIT (ECCEllipticCurvePanel, wxSize (-1,-1) ), 0);

	ECCEllipticCurveBoxSizer->Add (ECCPrimeStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCPrimeTextCtrl = new wxTextCtrl (ECCEllipticCurvePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCEllipticCurvePanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCPrimeTextCtrl->SetHint (wxT ("") );
#endif

	ECCEllipticCurveBoxSizer->Add (ECCPrimeTextCtrl, 2, wxALL, WXC_FROM_DIP (1) );

	ECCCurveOrderStaticText = new wxStaticText (ECCEllipticCurvePanel, wxID_ANY, _ ("curve order:"), wxDefaultPosition, wxDLG_UNIT (ECCEllipticCurvePanel, wxSize (-1,-1) ), 0);

	ECCEllipticCurveBoxSizer->Add (ECCCurveOrderStaticText, 0, wxALL, WXC_FROM_DIP (1) );

	ECCCurveOrderTextCtrl = new wxTextCtrl (ECCEllipticCurvePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCEllipticCurvePanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCCurveOrderTextCtrl->SetHint (wxT ("") );
#endif

	ECCEllipticCurveBoxSizer->Add (ECCCurveOrderTextCtrl, 2, wxALL, WXC_FROM_DIP (1) );

	ECCDataPanelFlexGridSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (1) );

	ECCSeedABPanel = new wxPanel (ECCDataPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCDataPanel, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	ECCDataPanelFlexGridSizer->Add (ECCSeedABPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* ECCSeedABBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ECCSeedABPanel->SetSizer (ECCSeedABBoxSizer);

	ECCSeedStaticText = new wxStaticText (ECCSeedABPanel, wxID_ANY, _ ("seed:"), wxDefaultPosition, wxDLG_UNIT (ECCSeedABPanel, wxSize (-1,-1) ), 0);

	ECCSeedABBoxSizer->Add (ECCSeedStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCSeedTextCtrl = new wxTextCtrl (ECCSeedABPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCSeedABPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCSeedTextCtrl->SetHint (wxT ("") );
#endif

	ECCSeedABBoxSizer->Add (ECCSeedTextCtrl, 2, wxALL, WXC_FROM_DIP (1) );

	ECCCoefficientAStaticText = new wxStaticText (ECCSeedABPanel, wxID_ANY, _ ("A:"), wxDefaultPosition, wxDLG_UNIT (ECCSeedABPanel, wxSize (-1,-1) ), 0);

	ECCSeedABBoxSizer->Add (ECCCoefficientAStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCCoefficientATextCtrl = new wxTextCtrl (ECCSeedABPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCSeedABPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCCoefficientATextCtrl->SetHint (wxT ("") );
#endif

	ECCSeedABBoxSizer->Add (ECCCoefficientATextCtrl, 2, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCCoefficientBStaticText = new wxStaticText (ECCSeedABPanel, wxID_ANY, _ ("B:"), wxDefaultPosition, wxDLG_UNIT (ECCSeedABPanel, wxSize (-1,-1) ), 0);

	ECCSeedABBoxSizer->Add (ECCCoefficientBStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCCoefficientBTextCtrl = new wxTextCtrl (ECCSeedABPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCSeedABPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCCoefficientBTextCtrl->SetHint (wxT ("") );
#endif

	ECCSeedABBoxSizer->Add (ECCCoefficientBTextCtrl, 2, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCBasePointStaticText = new wxStaticText (ECCDataPanel, wxID_ANY, _ ("base point P"), wxDefaultPosition, wxDLG_UNIT (ECCDataPanel, wxSize (-1,-1) ), 0);

	ECCDataPanelFlexGridSizer->Add (ECCBasePointStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (5) );

	ECCBasePointPanel = new wxPanel (ECCDataPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCDataPanel, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	ECCDataPanelFlexGridSizer->Add (ECCBasePointPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* ECCBasePointBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ECCBasePointPanel->SetSizer (ECCBasePointBoxSizer);

	ECCBasePoint_x_CoordinateStaticText = new wxStaticText (ECCBasePointPanel, wxID_ANY, _ ("x:"), wxDefaultPosition, wxDLG_UNIT (ECCBasePointPanel, wxSize (-1,-1) ), 0);

	ECCBasePointBoxSizer->Add (ECCBasePoint_x_CoordinateStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCBasePoint_x_CoordinateTextCtrl = new wxTextCtrl (ECCBasePointPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCBasePointPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCBasePoint_x_CoordinateTextCtrl->SetHint (wxT ("") );
#endif

	ECCBasePointBoxSizer->Add (ECCBasePoint_x_CoordinateTextCtrl, 2, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCBasePoint_y_CoordinateStaticText = new wxStaticText (ECCBasePointPanel, wxID_ANY, _ ("y:"), wxDefaultPosition, wxDLG_UNIT (ECCBasePointPanel, wxSize (-1,-1) ), 0);

	ECCBasePointBoxSizer->Add (ECCBasePoint_y_CoordinateStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCBasePoint_y_CoordinateTextCtrl = new wxTextCtrl (ECCBasePointPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCBasePointPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCBasePoint_y_CoordinateTextCtrl->SetHint (wxT ("") );
#endif

	ECCBasePointBoxSizer->Add (ECCBasePoint_y_CoordinateTextCtrl, 2, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCDataPanelFlexGridSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (1) );

	ECCOrderCofactorPanel = new wxPanel (ECCDataPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCDataPanel, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	ECCDataPanelFlexGridSizer->Add (ECCOrderCofactorPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* ECCOrderCofactorBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ECCOrderCofactorPanel->SetSizer (ECCOrderCofactorBoxSizer);

	ECCSubgroupOrderStaticText = new wxStaticText (ECCOrderCofactorPanel, wxID_ANY, _ ("base point order:"), wxDefaultPosition, wxDLG_UNIT (ECCOrderCofactorPanel, wxSize (-1,-1) ), 0);

	ECCOrderCofactorBoxSizer->Add (ECCSubgroupOrderStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCSubgroupOrderTextCtrl = new wxTextCtrl (ECCOrderCofactorPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCOrderCofactorPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCSubgroupOrderTextCtrl->SetHint (wxT ("") );
#endif

	ECCOrderCofactorBoxSizer->Add (ECCSubgroupOrderTextCtrl, 2, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	ECCSubgroupCofactorStaticText = new wxStaticText (ECCOrderCofactorPanel, wxID_ANY, _ ("cofactor:"), wxDefaultPosition, wxDLG_UNIT (ECCOrderCofactorPanel, wxSize (-1,-1) ), 0);

	ECCOrderCofactorBoxSizer->Add (ECCSubgroupCofactorStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCSubgroupCofactorTextCtrl = new wxTextCtrl (ECCOrderCofactorPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCOrderCofactorPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCSubgroupCofactorTextCtrl->SetHint (wxT ("") );
#endif

	ECCOrderCofactorBoxSizer->Add (ECCSubgroupCofactorTextCtrl, 2, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	ECCPointQStaticText = new wxStaticText (ECCDataPanel, wxID_ANY, _ ("point Q"), wxDefaultPosition, wxDLG_UNIT (ECCDataPanel, wxSize (-1,-1) ), 0);

	ECCDataPanelFlexGridSizer->Add (ECCPointQStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCPointQPanel = new wxPanel (ECCDataPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCDataPanel, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	ECCDataPanelFlexGridSizer->Add (ECCPointQPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* ECCPointQPanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ECCPointQPanel->SetSizer (ECCPointQPanelBoxSizer);

	ECCPointQ_x_CoordinateStaticText = new wxStaticText (ECCPointQPanel, wxID_ANY, _ ("x:"), wxDefaultPosition, wxDLG_UNIT (ECCPointQPanel, wxSize (-1,-1) ), 0);

	ECCPointQPanelBoxSizer->Add (ECCPointQ_x_CoordinateStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCPointQ_x_CoordinateTextCtrl = new wxTextCtrl (ECCPointQPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCPointQPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCPointQ_x_CoordinateTextCtrl->SetHint (wxT ("") );
#endif

	ECCPointQPanelBoxSizer->Add (ECCPointQ_x_CoordinateTextCtrl, 2, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCPointQ_y_CoordinateStaticText = new wxStaticText (ECCPointQPanel, wxID_ANY, _ ("y:"), wxDefaultPosition, wxDLG_UNIT (ECCPointQPanel, wxSize (-1,-1) ), 0);

	ECCPointQPanelBoxSizer->Add (ECCPointQ_y_CoordinateStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCPointQ_y_CoordinateTextCtrl = new wxTextCtrl (ECCPointQPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCPointQPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCPointQ_y_CoordinateTextCtrl->SetHint (wxT ("") );
#endif

	ECCPointQPanelBoxSizer->Add (ECCPointQ_y_CoordinateTextCtrl, 2, wxALL, WXC_FROM_DIP (1) );
	ECCResultStaticText = new wxStaticText (ECCDataPanel, wxID_ANY, _ ("result"), wxDefaultPosition, wxDLG_UNIT (ECCDataPanel, wxSize (-1,-1) ), 0);

	ECCDataPanelFlexGridSizer->Add (ECCResultStaticText, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCResultPanel = new wxPanel (ECCDataPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCDataPanel, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	ECCDataPanelFlexGridSizer->Add (ECCResultPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxBoxSizer* ECCResultBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ECCResultPanel->SetSizer (ECCResultBoxSizer);

	ECCResult_x_coordinateStaticText = new wxStaticText (ECCResultPanel, wxID_ANY, _ ("x:"), wxDefaultPosition, wxDLG_UNIT (ECCResultPanel, wxSize (-1,-1) ), 0);

	ECCResultBoxSizer->Add (ECCResult_x_coordinateStaticText, 0, wxALL, WXC_FROM_DIP (1) );

	ECCResult_x_coordinateTextCtrl = new wxTextCtrl (ECCResultPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCResultPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCResult_x_coordinateTextCtrl->SetHint (wxT ("") );
#endif

	ECCResultBoxSizer->Add (ECCResult_x_coordinateTextCtrl, 2, wxALL, WXC_FROM_DIP (1) );

	ECCResult_y_coordinateStaticText = new wxStaticText (ECCResultPanel, wxID_ANY, _ ("y:"), wxDefaultPosition, wxDLG_UNIT (ECCResultPanel, wxSize (-1,-1) ), 0);

	ECCResultBoxSizer->Add (ECCResult_y_coordinateStaticText, 0, wxALL, WXC_FROM_DIP (1) );

	ECCResult_y_coordinateTextCtrl = new wxTextCtrl (ECCResultPanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCResultPanel, wxSize (-1,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCResult_y_coordinateTextCtrl->SetHint (wxT ("") );
#endif

	ECCResultBoxSizer->Add (ECCResult_y_coordinateTextCtrl, 2, wxALL, WXC_FROM_DIP (1) );

	ECCOperatorPanel = new wxPanel (EllipticCurvePanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (EllipticCurvePanel, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	ECCFlexGridSizer->Add (ECCOperatorPanel, 1, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxFlexGridSizer* ECCOperatorPanelFlexGridSizer = new wxFlexGridSizer (3, 0, 0, 0);
	ECCOperatorPanelFlexGridSizer->SetFlexibleDirection ( wxBOTH );
	ECCOperatorPanelFlexGridSizer->SetNonFlexibleGrowMode ( wxFLEX_GROWMODE_SPECIFIED );
	ECCOperatorPanelFlexGridSizer->AddGrowableCol (0);
	ECCOperatorPanelFlexGridSizer->AddGrowableCol (1);
	ECCOperatorPanelFlexGridSizer->AddGrowableCol (2);
	ECCOperatorPanelFlexGridSizer->AddGrowableCol (3);
	ECCOperatorPanelFlexGridSizer->AddGrowableRow (0);
	ECCOperatorPanelFlexGridSizer->AddGrowableRow (1);
	//ECCOperatorPanelFlexGridSizer->AddGrowableRow (2);
	//ECCOperatorPanelFlexGridSizer->AddGrowableRow (3);
	ECCOperatorPanel->SetSizer (ECCOperatorPanelFlexGridSizer);
	ECCClearButton = new wxButton (ECCOperatorPanel, wxID_ANY, _ ("clear"), wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), 0);
	ECCClearButton->SetToolTip (_ ("Clear elliptic curve data.") );
	ECCOperatorPanelFlexGridSizer->Add (ECCClearButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );

	// ECCOperatorPanelFlexGridSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );

	/*ECCBasePointOrderButton = new wxButton (ECCOperatorPanel, wxID_ANY, _ ("base point order"), wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), 0);
	ECCBasePointOrderButton->SetToolTip (_ ("Order of the cyclic group generated by the base point.") );
	ECCOperatorPanelFlexGridSizer->Add (ECCBasePointOrderButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );*/

	ECCVerifyButton = new wxButton (ECCOperatorPanel, wxID_ANY, _ ("check validity"), wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), 0);
	ECCVerifyButton->SetToolTip (_ ("Verify that ec coeffcients derived from a random 160-bit seed.") );
	ECCOperatorPanelFlexGridSizer->Add (ECCVerifyButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );
	ECCCheckCurveButton = new wxButton(
		ECCOperatorPanel,
		wxID_ANY,
		_("check curve"),
		wxDefaultPosition,
		wxDefaultSize,
		0);

	ECCCheckCurveButton->SetToolTip(
		_("Check that p is prime and the elliptic curve is nonsingular."));

	ECCOperatorPanelFlexGridSizer->Add(
		ECCCheckCurveButton,
		1,
		wxALL | wxALIGN_CENTER_HORIZONTAL,
		WXC_FROM_DIP(1));
	ECCRandomPointQButton = new wxButton (ECCOperatorPanel, wxID_ANY, _ ("random point"), wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), 0);
	ECCRandomPointQButton->SetToolTip (_ ("Generate a random point on the elliptic curve") );
	ECCOperatorPanelFlexGridSizer->Add (ECCRandomPointQButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );

	ECCToQButton = new wxButton (ECCOperatorPanel, wxID_ANY, _ ("->Q"), wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), 0);
	ECCToQButton->SetToolTip (_ ("Copy x and y coordinates of result to point Q") );
	ECCOperatorPanelFlexGridSizer->Add (ECCToQButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );

	ECCTRandom160BitSeedButton = new wxButton (ECCOperatorPanel, wxID_ANY, _ ("random 160-bit seed"), wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), 0);
	ECCTRandom160BitSeedButton->SetToolTip (_ ("Generate a random 160-bit seed") );
	ECCOperatorPanelFlexGridSizer->Add (ECCTRandom160BitSeedButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );

	ECCtoSeedButton = new wxButton (ECCOperatorPanel, wxID_ANY, _ ("->seed"), wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), 0);
	ECCtoSeedButton->SetToolTip (_ ("Copy 160-bit scalar to seed") );
	ECCOperatorPanelFlexGridSizer->Add (ECCtoSeedButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );

	ECCRandomkBitPrimePanel = new wxPanel (ECCOperatorPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	ECCOperatorPanelFlexGridSizer->Add (ECCRandomkBitPrimePanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );

	wxBoxSizer* ECCRandomkBitPrimePanelBoxSizer = new wxBoxSizer (wxHORIZONTAL);
	ECCRandomkBitPrimePanel->SetSizer (ECCRandomkBitPrimePanelBoxSizer);

	ECCNumberBitsInPrimeStaticText = new wxStaticText (ECCRandomkBitPrimePanel, wxID_ANY, _ ("k"),
	      wxDefaultPosition, wxDLG_UNIT (ECCRandomkBitPrimePanel, wxSize (-1,-1) ),
	      wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL);

	ECCRandomkBitPrimePanelBoxSizer->Add (ECCNumberBitsInPrimeStaticText, 1, wxALL, WXC_FROM_DIP (1) );

	ECCNumberBitsInPrimeTextCtrl = new wxTextCtrl (ECCRandomkBitPrimePanel, wxID_ANY, wxT (""), wxDefaultPosition, wxDLG_UNIT (ECCRandomkBitPrimePanel, wxSize (35,-1) ), 0);
#if wxVERSION_NUMBER >= 3000
	ECCNumberBitsInPrimeTextCtrl->SetHint (wxT ("") );
#endif

	ECCRandomkBitPrimePanelBoxSizer->Add (ECCNumberBitsInPrimeTextCtrl, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCkBitPrimeButton = new wxButton (ECCRandomkBitPrimePanel, wxID_ANY, _ ("k-bit prime"), wxDefaultPosition, wxDLG_UNIT (ECCRandomkBitPrimePanel, wxSize (-1,-1) ), 0);
	ECCkBitPrimeButton->SetToolTip(wxT("Generate a random k-bit prime."));
	ECCTopButton = new wxButton (ECCOperatorPanel, wxID_ANY, _ ("->p"), wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), 0);
	ECCTopButton->SetToolTip (_ ("Copy scalar result value to p") );
	ECCOperatorPanelFlexGridSizer->Add (ECCTopButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );
	ECCRandomkBitPrimePanelBoxSizer->Add (ECCkBitPrimeButton, 0, wxALL, WXC_FROM_DIP (1) );
	// ECCOperatorPanelFlexGridSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );
	//ECCOperatorPanelFlexGridSizer->Add (0, 0, 1, wxALL, WXC_FROM_DIP (5) );

	ECCStaticLine1 = new wxStaticLine (ECCOperatorPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), wxLI_HORIZONTAL);

	ECCOperatorPanelFlexGridSizer->Add (ECCStaticLine1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP (1) );

	ECCStaticLine2 = new wxStaticLine (ECCOperatorPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), wxLI_HORIZONTAL);

	ECCOperatorPanelFlexGridSizer->Add (ECCStaticLine2, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP (1) );

	ECCStaticLine3 = new wxStaticLine (ECCOperatorPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), wxLI_HORIZONTAL);

	ECCOperatorPanelFlexGridSizer->Add (ECCStaticLine3, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP (1) );

	ECCStaticLine4 = new wxStaticLine (ECCOperatorPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), wxLI_HORIZONTAL);

	ECCOperatorPanelFlexGridSizer->Add (ECCStaticLine4, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP (1) );

	//ECCStaticLine5 = new wxStaticLine (ECCOperatorPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (ECCOperatorPanel, wxSize (-1,-1) ), wxLI_HORIZONTAL);

	//ECCOperatorPanelFlexGridSizer->Add (ECCStaticLine5, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP (1) );

	ECCCurvePanel = new wxPanel (EllipticCurvePanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT (EllipticCurvePanel, wxSize (-1,-1) ), wxTAB_TRAVERSAL);

	ECCFlexGridSizer->Add (ECCCurvePanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP (1) );

	wxFlexGridSizer* ECCCurvePanelGridSizer =
		new wxFlexGridSizer(1, 6, 0, 5);

	for (int col = 0; col < 6; ++col)
	{
		ECCCurvePanelGridSizer->AddGrowableCol(col, 1);
	}
	ECCCurvePanel->SetSizer (ECCCurvePanelGridSizer);

	ECCCreateSecureDomainParameters = new wxButton (ECCCurvePanel, wxID_ANY, _ ("Build a simple curve"), wxDefaultPosition, wxDLG_UNIT (ECCCurvePanel, wxSize (-1,-1) ), 0);
	ECCCreateSecureDomainParameters->SetToolTip (_ ("Build a small random curve for educational purposes") );
	ECCCurvePanelGridSizer->Add (ECCCreateSecureDomainParameters, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );

	ECCPlotSimpleECButton = new wxButton (ECCCurvePanel, wxID_ANY, _ ("Plot simple ec"), wxDefaultPosition, wxDLG_UNIT (ECCCurvePanel, wxSize (-1,-1) ), 0);
	ECCPlotSimpleECButton->SetToolTip (_ ("Plot a simple curve like y^2=x^3+Ax+B over the real numbers.") );
	ECCCurvePanelGridSizer->Add (ECCPlotSimpleECButton, 0, wxALL|wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );

	ECCPointPlotSimpleECButton = new wxButton (ECCCurvePanel, wxID_ANY, _ ("Point plot simple ec"), wxDefaultPosition, wxDLG_UNIT (ECCCurvePanel, wxSize (-1,-1) ), 0);
	ECCPointPlotSimpleECButton->SetToolTip (_ ("Plot a simple curve like y^2=x^3+Ax+B (mod p).") );
	ECCCurvePanelGridSizer->Add (ECCPointPlotSimpleECButton, 0, wxALL|wxALIGN_CENTER, WXC_FROM_DIP (1) );

	ECCDiscreteLogarithmButton = new wxButton (ECCCurvePanel, wxID_ANY, _ ("Discrete logarithm"), wxDefaultPosition, wxDLG_UNIT (ECCCurvePanel, wxSize (-1,-1) ), 0);
	ECCDiscreteLogarithmButton->SetToolTip (_ ("Attempt to solve the ECDLP using one of three algorithms.") );
	ECCCurvePanelGridSizer->Add (ECCDiscreteLogarithmButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );

	ECCSaveCurveParametersButton = new wxButton (ECCCurvePanel, wxID_ANY, _ ("Save curve parameters"), wxDefaultPosition, wxDLG_UNIT (ECCCurvePanel, wxSize (-1,-1) ), 0);
	ECCSaveCurveParametersButton->SetToolTip (_ ("Save parameters of the curve.") );
	ECCCurvePanelGridSizer->Add (ECCSaveCurveParametersButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );

    ECCReadCurveParametersButton = new wxButton (ECCCurvePanel, wxID_ANY, _ ("Read curve parameters"), wxDefaultPosition, wxDLG_UNIT (ECCCurvePanel, wxSize (-1,-1) ), 0);
	ECCReadCurveParametersButton->SetToolTip (_ ("Read parameters for a curve.") );
	ECCCurvePanelGridSizer->Add (ECCReadCurveParametersButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, WXC_FROM_DIP (1) );
	SetName (wxT ("EllipticCurves") );
//	SetSize (1400,800/**<  */);

	if (GetSizer() )
	{
		GetSizer()->Fit (this);
	}

	// CREATE THE PAGES AND THE MENU BAR

	discreteMathCalculatorNotebook->AddPage (numberTheoreticPanel, _ ("Big Integer Calculator"), false);
	discreteMathCalculatorNotebook->AddPage (countingAndEnumerationPanel, _ ("Counting and Enumeration"), false);
	discreteMathCalculatorNotebook->AddPage (trianglesPanel, _ ("Triangles"), false);
	discreteMathCalculatorNotebook->AddPage (subsetsPanel, _ ("Subsets"), false);
	discreteMathCalculatorNotebook->AddPage (partitionsPanel, _ ("Partitions"), false);
	discreteMathCalculatorNotebook->AddPage (treesPanel, _ ("Trees"), false);
	discreteMathCalculatorNotebook->AddPage (graphsPanel, _ ("Graphs"), false);
	discreteMathCalculatorNotebook->AddPage (relationsAndPartialOrdersPanel, _ ("Relations and Partial Orders"), false);
	discreteMathCalculatorNotebook->AddPage (EllipticCurvePanel, _ ("Elliptic Cubic Curves"), false);
	discreteMathCalculatorGridSizer->Add (
	   discreteMathCalculatorNotebook, 1, wxALL | wxEXPAND, 1);

	// Assign the notebook layout to the main panel.
	discreteMathCalculatorPanel->SetSizer(
		discreteMathCalculatorGridSizer
	);

	// Create a sizer for the main application frame.
	wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);

	// Make the main panel fill the available frame area.
	frameSizer->Add(
		discreteMathCalculatorPanel,
		1,
		wxEXPAND,
		0
	);

	// Assign the sizer to the frame.
	SetSizer(frameSizer);

	// Update the layout.
	Layout();

	discreteMathCalculatorMenuBar = new wxMenuBar();
	buildFileMenu();
	buildHelpMenu();
	buildBigIntegerToolsMenu();
	SetMenuBar (discreteMathCalculatorMenuBar);
	DiscreteMathCalculatorStatusBar =
	   new wxStatusBar (this, ID_DISCRETE_MATH_CALCULATOR_STATUSBAR, 0, _T ("ID_DISCRETE_MATH_CALCULATOR_STATUSBAR") );
	int __wxStatusBarWidths_1[1] = { -1 };
	int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
	DiscreteMathCalculatorStatusBar->SetFieldsCount (1, __wxStatusBarWidths_1);
	DiscreteMathCalculatorStatusBar->SetStatusStyles (1, __wxStatusBarStyles_1);
	SetStatusBar (DiscreteMathCalculatorStatusBar);

	// SETUP EVENT HANDLERS

	Connect (ID_CLEAR_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnclearNumbersButtonClick);
	Connect (ID_IS_A_PRIME_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnisAPrimeButtonClick);
	Connect (ID_ADD_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnaddButtonClick);
	Connect (ID_ADD_MOD_N_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onadd_Mod_n_ButtonClick);
	Connect (ID_PHI_N_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnphiNButtonClick);
	Connect (ID_TO_A_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onto_a_ButtonClick);
	Connect (ID_MEMORY_STORE_1_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnmemoryStore1ButtonClick);
	Connect (ID_MEMORY_STORE_2_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnmemoryStore2ButtonClick);
	Connect (ID_SUB_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnsubButtonClick);
	Connect (ID_SUB_MOD_N_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onsub_mod_n_ButtonClick);
	Connect (ID_PHI_A_B_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onphi_a_b_ButtonClick);
	Connect (ID_TO_B_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onto_b_ButtonClick);
	Connect (ID_MEMORY_1_PLUS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onmemory1PlusButtonClick);
	Connect (ID_MEMORY_2_PLUS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onmemory2PlusButtonClick);
	Connect (ID_MUL_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnmulButtonClick);
	Connect (ID_MUL_MOD_N_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onmul_mod_n_ButtonClick);
	BigIntegerCalculatorXbitIntegerButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnBigIntegerCalculatorXbitRandomIntegerButtonPress), NULL, this);
	Connect (ID_X_BIT_PRIME_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onx_bit_prime_ButtonClick);
	Connect (ID_TO_N_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onto_n_ButtonClick);
	Connect (ID_MEMORY_RECALL_1_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnmemoryRecall1ButtonClick);
	Connect (ID_MEMORY_RECALL_2_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnmemoryRecall2ButtonClick);
	Connect (ID_DIV_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OndivButtonClick);
	Connect (ID_MOD_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnmodButtonClick);
	Connect (ID_GCD_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngcdButtonClick);
	Connect (ID_MEMORY_CLEAR_1_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnmemoryClear1ButtonClick);
	Connect (ID_MEMORY_CLEAR_2_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnmemoryClear2ButtonClick);
	Connect (ID_POWER_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnpowerButtonClick);
	Connect (ID_POWER_MOD_N_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Onpower_mod_n_ButtonClick);
	Connect (ID_LEAST_COMMON_MULTIPLE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnleastCommonMultipleButtonClick);
	Connect (ID_INVERSE_MOD_N_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::Oninverse_a_mod_n_ButtonClick);
	Connect (ID_COPRIME_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncoprimeButtonClick);
	Connect (ID_RESULT_MAGNITUDE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnresultMagnitudeButtonClick);
	Connect (ID_SPELL_RESULT_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnspellResultButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_LIST_PERMUTATIONS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncountingAndEnumerationListPermutationsButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_LIST_COMBINATIONS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncountingAndEnumerationListCombinationsButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_FACTORIAL_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncountingAndEnumerationFactorialButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_PERMUTATIONS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncountingAndEnumerationPermutationsButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_COMBINATIONS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncountingAndEnumerationCombinationsButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_FIBONACCI_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncountingAndEnumerationFibonacciButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_CATALAN_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncountingAndEnumerationCatalanButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_SCHRODER_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncountingAndEnumerationSchroderButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_BELL_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncountingAndEnumerationBellButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_STIRLING_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncountingAndEnumerationStirlingButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_RESULT_MAGNITUDE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnresultMagnitudeButtonClick);
	Connect (ID_COUNTING_AND_ENUMERATION_ASYMPTOTIC_ESTIMATE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnAsymptoticEstimateButtonClick);
	Connect (ID_TRIANGLES_PASCAL_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnpascalTriangleButtonClick);
	Connect (ID_TRIANGLES_HOSOYA_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OntrianglesHosoyaButtonClick);
	Connect (ID_TRIANGLES_CATALAN_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncatalanTriangleButtonClick);
	Connect (ID_TRIANGLES_SCHRODER_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnschroderTriangleButtonClick);
	Connect (ID_TRIANGLES_BELL_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnbellTriangleButtonClick);
	Connect (ID_SUBSETS_NUMBER_OF_SUBSETS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnsubsetsNumberOfSubsetsButtonClick);
	Connect (ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnsubsetsNumberOfKElementSubsetsButtonClick);
	Connect (ID_SUBSETS_LIST_ALL_SUBSETS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnsubsetsListAllSubsetsButtonClick);
	Connect (ID_SUBSETS_LIST_ALL_K_ELEMENT_SUBSETS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnsubsetsListAllKElementSubsetsButtonClick);
	Connect (ID_SUBSETS_ASYMPTOTIC_ESTIMATE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnAsymptoticEstimateButtonClick);
	Connect (ID_SUBSETS_RESULT_MAGNITUDE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnresultMagnitude2ButtonClick);
	Connect (ID_PARTITIONS_BELL_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnpartitionsBellButtonClick);
	Connect (ID_PARTITIONS_STIRLING_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnpartitionsStirlingButtonClick);
	Connect (ID_PARTITONS_LIST_PARTITIONS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnpartitionsListPartitionsButtonClick);
	Connect (ID_PARTITIONS_LIST_STIRLING_PARTITIONS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnpartitionsListStirlingPartitionsButtonClick);
	Connect (ID_PARTITIONS_RESULT_MAGNITUDE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnresultMagnitude2ButtonClick);
	Connect (ID_PARTITIONS_ASYMPTOTIC_ESTIMATE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnAsymptoticEstimateButtonClick);
	Connect (ID_BINARY_TREES_ENTER_BTR_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnbinaryTreesEnterButtonClick);
	Connect (ID_BINARY_TREES_ENTER_BTR_FROM_FILE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnbinaryTreesEnterBTRFromFileButtonClick);
	Connect (ID_BINARY_TREES_ENTER_GML_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEnterGMLFile);
	Connect (ID_BINARY_TREES_HUFFMAN_TREE_ENTER_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnbinaryTreesHuffmanTreeEnterButtonClick);
	Connect (ID_BINARY_TREES_HUFFMAN_TREE_FROM_GML_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEnterGMLFile);
	Connect (ID_BINARY_TREES_EXPRESSION_TREE_ENTER_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnbinaryTreesExpressionTreeEnterButtonClick);
	Connect (ID_BINARY_TREES_EXPRESSION_TREE_FROM_GML_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEnterGMLFile);
	Connect (ID_BINARY_SEARCH_TREES_ENTER_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnDataInputButtonClick);
	Connect (ID_BINARY_TREES_SEARCH_TREE_ENTER_FROM_FILE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEnterFromTextfileButtonClick);
	Connect (ID_BINARY_SEARCH_TREES_RANDOM_DATA_ENTER_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnRandomDataEnterButtonClick);
	Connect (ID_BINARY_SEARCH_TREES_FROM_GML_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEnterGMLFile);
	Connect (ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_ENTER_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnDataInputButtonClick);
	Connect (ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_TEXTFILE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEnterFromTextfileButtonClick);
	Connect (ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RANDOM_DATA_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnRandomDataEnterButtonClick);
	Connect (ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_GML_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEnterGMLFile);
	Connect (ID_ENTER_HEAP_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnDataInputButtonClick);
	Connect (ID_ENTER_RANDOM_HEAP_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnRandomDataEnterButtonClick);
	Connect (ID_ENTER_HEAP_FROM_TEXTFILE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEnterFromTextfileButtonClick);
	Connect (ID_HEAP_ENTER_FROM_GML_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEnterGMLFile);
	Connect (ID_GENERAL_TREE_ENTER_FROM_TEXTFILE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngeneralTreeEnterButtonClick);
	Connect (ID_GENERAL_TREE_ENTER_FROM_GTR_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngeneralTreeEnterFromFileButtonClick);
	Connect (ID_GENERAL_TREES_ENTER_FROM_GML_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngeneralTreeEnterFromFileButtonClick);
	Connect (ID_GRAPHS_PREVIEW_SLIDER,
	         wxEVT_SCROLL_THUMBTRACK,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsPreviewSliderCmdScrollChanged);
	/*Connect (ID_GRAPHS_WEIGHTED_CHECKBOX,
	         wxEVT_COMMAND_CHECKBOX_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsWeightedCheckBoxClick);
	Connect (ID_GRAPHS_HIDE_EDGES_CHECKBOX,
	         wxEVT_COMMAND_CHECKBOX_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsHideEdgesCheckBoxClick); */
	Connect (ID_GRAPHS_ALLOW_PARALLEL_EDGES_CHECKBOX,
	         wxEVT_COMMAND_CHECKBOX_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsAllowParallelEdgesCheckBoxClick);
	Connect (ID_GRAPHS_ENTER_FROM_DIALOG_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsEnterFromDialogButtonClick);
	Connect (ID_GRAPHS_ENTER_FROM_GRPH_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsEnterFromGRPHButtonClick);
	Connect (ID_GRAPHS_ENTER_FROM_GML_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsEnterFromGMLButtonClick);
	Connect (ID_GRAPHS_FROM_TSP_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsEnterFromTSPButtonClick);
	Connect (ID_GRAPH_REDRAW_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsRedrawButtonClick);
	Connect (ID_GRAPHS_ADD_VERTEX_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsAddVertexButtonClick);
	Connect (ID_GRAPHS_ERASE_VERTEX_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsEraseVertexButtonClick);
	Connect (ID_GRAPHS_ADD_EDGE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsAddEdgeButtonClick);
	Connect (ID_GRAPHS_ERASE_EDGE_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsEraseEdgeButtonClick);
	Connect (ID_SUBGRAPH_ISOMORPHISM_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnSubgraphIsomorphismButtonPress);
	Connect (ID_GRAPH_ISOMORPHISM_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnGraphIsomorphismButtonPress);
	Connect (ID_CLEAR_GRAPH_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnClearGraphButtonClick);
	Connect (ID_ANALYZE_GRAPH_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnAnalyzeGraphButtonClick);
	Connect (ID_TRANSPOSE_GRAPH_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OntransposeGraphButtonClick);
	Connect (ID_COMPLEMENTARY_GRAPH_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncomplementaryGraphButtonClick);
	Connect (ID_GRAPH_RUN_ALGORITHMS_BUTTON,
	         wxEVT_COMMAND_BUTTON_CLICKED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsRunAlgorithmsButtonClick);
	Connect (ID_DISCRETE_MATH_CALCULATOR_NOTEBOOK,
	         wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OndiscreteMathCalculatorNotebookPageChanged);
	Connect (ID_CREATE_TSP_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncreateTSPMenuItemSelected);
	Connect (ID_CREATE_RANDOM_GRAPH_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncreateRandomGraphMenuItemSelected);
	Connect (ID_DIGRAPHICAL_SEQUENCE_GRAPH_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnDigraphicalSequenceGraphMenuItemSelected);
	Connect (ID_GRID_GRAPH_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnGridGraphMenuItemSelected);
	Connect (ID_GRAPHICAL_SEQUENCE_GRAPH_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnGraphSequenceGraphMenuItemSelected);
	Connect (ID_DRAW_KN_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OndrawKnMenuItemSelected);
	Connect (ID_GRAPHS_MAKE_CLIQUE,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsMakeGraphWithACliqueMenuItemSelected);
	Connect (ID_HYPERCUBE_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnhypercubeMenuItemSelected);
	Connect (ID_STABLE_MARRIAGE_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnstableMarriageMenuItemSelected);
	Connect (ID_CREATE_HAMCYCLE_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OncreateHamCycleMenuItemSelected);
	Connect (ID_CREATE_GENERALIZED_PETERSON_GRAPH_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngeneralizedPetersonGraphMenuItemSelected);
	Connect (ID_DISCRETE_MATH_CALCULATOR_HELP_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         wxCommandEventHandler (DiscreteMathCalculatorFrame::OndiscreteMathCalcuatorHelpMenuItemSelected) );
	Connect (ID_DISCRETE_MATH_CALCULATOR_HELP_ABOUT_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         wxCommandEventHandler (DiscreteMathCalculatorFrame::OnAbout) );
	Connect (ID_GRAPHS_PREVIEW_WINDOW_PRINT_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::handleMenuChoice);
	Connect (ID_GRAPHS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::handleMenuChoice);
	Connect (ID_GRAPHS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::handleMenuChoice);
	Connect (ID_GRAPHS_PREVIEW_WINDOW_SAVE_AS_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::handleMenuChoice);
	Connect  (ID_ECDIGITALSIGNATUREAPPLICATIONMENUITEM,
	          wxEVT_COMMAND_MENU_SELECTED,
	          (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEllipticCurveECDLPMenuItemSelected);
	Connect  (ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_FACTOR_MENUITEM,
	          wxEVT_COMMAND_MENU_SELECTED,
	          (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnIntegerFactorMenuItemSelected);
	Connect  (ID_ELLIPTIC_CURVE_DIGITAL_SIGNATURE_MENUITEM,
	          wxEVT_COMMAND_MENU_SELECTED,
	          (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnEllipticCurveDigitalSignatureMenuItemSelected);
	Connect  (ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DISCRETE_LOGARITHM_MENUITEM,
	          wxEVT_COMMAND_MENU_SELECTED,
	          (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnBigIntegerDiscreteLogarithmMenuItemSelected);
	Connect  (ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_RSA_MENUITEM,
	          wxEVT_COMMAND_MENU_SELECTED,
	          (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnBigIntegerRSAGuideMenuItemSelected);

	/*Connect  (ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_PUBLIC_KEY_ENCRYPTION_MENUITEM,
	          wxEVT_COMMAND_MENU_SELECTED,
	          (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnBigIntegerRSAEncryptionMenuItemSelected);*/

	Connect  (ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DIFFIE_HELLMANKEYEXCHANGE_MENUITEM,
	          wxEVT_COMMAND_MENU_SELECTED,
	          (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnBigIntegerDiffieHellmanMenuItemSelected);
	/*	Connect  (ID_DISCRETE_MATH_CALCULATOR_ELLIPTIC_DIFFIE_HELLMANKEYEXCHANGE_MENUITEM,
		          wxEVT_COMMAND_MENU_SELECTED,
		          (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnElliptic_DiffieHellmanMenuItemSelected);*/

	Connect  (ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DIGITAL_SIGNATURE_MENUITEM,
	          wxEVT_COMMAND_MENU_SELECTED,
	          (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnBigIntegerDigitalSignatureMenuItemSelected);
	/* _____________________________________________________________________________*/

// Relations and Partial Orders Examples menu

// Relations and Partial Orders Examples menu

	Connect(
		ID_RELATIONS_EXAMPLES_MENUITEM,
		wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction)
		&DiscreteMathCalculatorFrame::OnCreateRelationExamplesMenuItemSelected
	);

	Connect(
		ID_PARTIAL_ORDER_EXAMPLES_MENUITEM,
		wxEVT_COMMAND_MENU_SELECTED,
		(wxObjectEventFunction)
		&DiscreteMathCalculatorFrame::OnCreatePartialOrderExamplesMenuItemSelected
	);
	Connect (ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_MENUITEM,
		         wxEVT_COMMAND_MENU_SELECTED,
		         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::handleRelationGraphMenuChoice);
	
	Connect (ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::handleRelationGraphMenuChoice);
	Connect (ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::handleRelationGraphMenuChoice);
	Connect (ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_SAVE_AS_MENUITEM,
	         wxEVT_COMMAND_MENU_SELECTED,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::handleRelationGraphMenuChoice);
	/* _____________________________________________________________________________*/

	Connect (ID_GRAPH_LAYOUT_RADIOBOX,
	         wxEVT_RADIOBOX,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphLayoutRadioBoxSelect);
	Connect (ID_GRAPHS_TYPE_RADIOBOX,
	         wxEVT_RADIOBOX,
	         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OngraphsGraphTypeRadioBoxSelect);

	InputRelationEnterButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnRelationEnterButtonPress), NULL, this);
	RelationReflexiveClosureButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnRelationReflexiveClosureButtonPress), NULL, this);
	RelationSymmetricClosureButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnRelationSymmetricClosureButtonPress), NULL, this);
	RelationTransiveClosureButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnRelationTransitiveClosureButtonPress), NULL, this);
	RelationPropertiesButton->Connect(
		wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(
			DiscreteMathCalculatorFrame::OnRelationPropertiesButtonPress),
		NULL,
		this);
	HasseDiagramEnterButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler (DiscreteMathCalculatorFrame::OnPartialOrderInputButtonPress), NULL, this);
	OutputRelationGMLButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &DiscreteMathCalculatorFrame::OnRelationToGMLButtonPress, this);
	InputRelationFromGMLButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &DiscreteMathCalculatorFrame::OnRelationFromGMLButtonPress, this);
	InputRelationRELButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &DiscreteMathCalculatorFrame::OnInputRelationRELButtonPressed, this);
	InputPartialOrderFromGMLButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &DiscreteMathCalculatorFrame::OnPartialOrderFromGMLButtonPress, this);
	OutputPartialOrderGMLButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &DiscreteMathCalculatorFrame::OnPartialOrderOutputGMLButtonPress, this);
	relationPartialOrderSlider->Connect (wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler (DiscreteMathCalculatorFrame::OnShapeFrameworkDrawingCanvasSliderChange), NULL, this);
	ECCVerifyButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCVerifyButton), NULL, this);
	ECCCheckCurveButton->Connect(
		wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(DiscreteMathCalculatorFrame::OnECCCheckCurveButton),
		NULL,
		this);
	ECCToQButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCToQButtonPress), NULL, this);
	ECCTRandom160BitSeedButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCRandom160BitSeedButtonPress), NULL, this);
	ECCCreateSecureDomainParameters->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCreateSecureCurveDomain), NULL, this);
	ECCPlotSimpleECButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCPlotSimpleEllipticCurveButtonPressed), NULL, this);
	ECCPointPlotSimpleECButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCPointPlotSimpleEllipticCurveButtonPressed), NULL, this);
	ECCDiscreteLogarithmButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCDiscreteLogarithmButtonPressed), NULL, this);
	ECCRandomPointQButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCRandomPointButtonPress), NULL, this);
	ECCSaveCurveParametersButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCSaveCurveParametersButtonPressed), NULL, this);
	ECCReadCurveParametersButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCReadCurveParametersButtonPressed), NULL, this);
	ECCkBitPrimeButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCkBitPrimeButtonPress), NULL, this);
	ECCTopButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCTopButtonPressed), NULL,this);
	ECCtoSeedButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCToSeedButtonPressed), NULL, this);
	ECCClearButton->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCClearButtonPressed), NULL, this);
	Connect (ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_ECDLP_MENUITEM,
	          wxEVT_COMMAND_MENU_SELECTED,
	          (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnDiffieHellmanGFMenuItemSelected);
	Connect (ID_BINARY_TREE_EXAMPLES_MENUITEM,
         wxEVT_COMMAND_MENU_SELECTED,
         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnCreateTreeBTRFileExamplesMenuItemSelected);
	Connect (ID_GENERAL_TREE_EXAMPLES_MENUITEM,
         wxEVT_COMMAND_MENU_SELECTED,
         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnCreateTreeGTRFileExamplesMenuItemSelected);
	Connect (ID_SEARCH_TREE_EXAMPLES_MENUITEM,
         wxEVT_COMMAND_MENU_SELECTED,
         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnCreateSearchTreeFileExamplesMenuItemSelected);
	Connect (ID_HEAP_EXAMPLES_MENUITEM,
         wxEVT_COMMAND_MENU_SELECTED,
         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnCreateHeapFileExamplesMenuItemSelected);
	Connect (ID_HUFFMAN_TREE_EXAMPLES_MENUITEM,
         wxEVT_COMMAND_MENU_SELECTED,
         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnCreateHuffmanTreeFileExamplesMenuItemSelected);
   Connect (ID_EXPRESSION_TREE_EXAMPLES_MENUITEM,
         wxEVT_COMMAND_MENU_SELECTED,
         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnCreateExpressionTreeFileExamplesMenuItemSelected);
	Connect (ID_BALANCED_SEARCH_TREE_EXAMPLES_MENUITEM,
         wxEVT_COMMAND_MENU_SELECTED,
         (wxObjectEventFunction) &DiscreteMathCalculatorFrame::OnCreateBalancedTreeFileExamplesMenuItemSelected);

   // FINISH UP WITH VARIOUS THINGS

	SetSize(1280, 900);
	previewWindow->setSliderAddress (graphsPreviewSlider);
	comb_perm = NULL;
	triangles = NULL;
	subsets = NULL;
	partitions = NULL;
	RR::SetPrecision (4096);
	RR::SetOutputPrecision (5);
	searchTreeOpen = false;
	menuHandler = NULL;
	relationMenuHandler = NULL;
	myRelationDigraph = NULL;

	dirty = false;
	relationDrawingIsDirty = false;
	figureSaved = false;
	relationFigureSaved = false;
	previewWindow->getDirty (&dirty);
	relationPartialOrderDrawing->getDirty (&relationDrawingIsDirty);
	myGraph = NULL;
	allowParallelEdges = false;
	isDigraph = false;
	isWeighted = false;
	hideEdges = false;
	graphLayout = hierarchical;
	graphType = undir;
	memory = 0;
	page = 0;
	destroyCalled = false;
	clipboardChecked = false;
	gettimeofday (&tp, NULL);
	mslong = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000;
	miracl *mip = get_mip();
	irand (mslong);
	haveCurrentEllipticCurve = false;
	haveDefaultBase = true;
	defaultBase = 10;
	mip->IOBASE = 10;

	// if the clipboard is not empty, DiscreteMathCalculator may ask the user to save its
	// contents upon exit.
	wxTheClipboard->Clear();

	if (wxTheClipboard->IsOpened() )
		wxTheClipboard->Close();

Centre();

// Fit the main window within the usable screen area.
	FitFrameToScreen(this);
// Prevent the window from becoming too narrow.
	SetMinSize(wxSize(1100, 700));

}    // End of constructor

DiscreteMathCalculatorFrame::~DiscreteMathCalculatorFrame()
{
	relationPartialOrderSlider->Disconnect (wxEVT_SCROLL_CHANGED,
	                                        wxScrollEventHandler (DiscreteMathCalculatorFrame::OnShapeFrameworkDrawingCanvasSliderChange), NULL, this);
	InputRelationEnterButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnRelationEnterButtonPress), NULL, this);
	RelationReflexiveClosureButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnRelationReflexiveClosureButtonPress), NULL, this);
	RelationSymmetricClosureButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnRelationSymmetricClosureButtonPress), NULL, this);
	RelationTransiveClosureButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnRelationTransitiveClosureButtonPress), NULL, this);
	RelationPropertiesButton->Disconnect(
		wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(
			DiscreteMathCalculatorFrame::OnRelationPropertiesButtonPress),
		NULL,
		this);
	HasseDiagramEnterButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnPartialOrderInputButtonPress), NULL, this);
	OutputRelationGMLButton->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &DiscreteMathCalculatorFrame::OnRelationToGMLButtonPress, this);
	InputRelationFromGMLButton-Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &DiscreteMathCalculatorFrame::OnRelationFromGMLButtonPress, this);
	InputPartialOrderFromGMLButton-Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &DiscreteMathCalculatorFrame::OnPartialOrderFromGMLButtonPress, this);
	InputRelationRELButton->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &DiscreteMathCalculatorFrame::OnInputRelationRELButtonPressed, this);
	OutputPartialOrderGMLButton->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &DiscreteMathCalculatorFrame::OnPartialOrderOutputGMLButtonPress, this);
	ECCVerifyButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCVerifyButton), NULL, this);
	ECCCheckCurveButton->Disconnect(
    wxEVT_COMMAND_BUTTON_CLICKED,
    wxCommandEventHandler(DiscreteMathCalculatorFrame::OnECCCheckCurveButton),
    NULL,
    this);
	ECCToQButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCToQButtonPress), NULL, this);
	ECCRandomPointQButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCRandomPointButtonPress), NULL, this);
	ECCSaveCurveParametersButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCSaveCurveParametersButtonPressed), NULL, this);
	ECCReadCurveParametersButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCReadCurveParametersButtonPressed), NULL, this);
	ECCTRandom160BitSeedButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCRandom160BitSeedButtonPress), NULL, this);
	ECCCreateSecureDomainParameters->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCreateSecureCurveDomain), NULL, this);
	ECCPlotSimpleECButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCPlotSimpleEllipticCurveButtonPressed), NULL, this);
	ECCPointPlotSimpleECButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCPointPlotSimpleEllipticCurveButtonPressed), NULL, this);
	ECCDiscreteLogarithmButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCDiscreteLogarithmButtonPressed), NULL, this);
	ECCkBitPrimeButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCkBitPrimeButtonPress), NULL, this);
	ECCTopButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCTopButtonPressed), NULL,this);
	ECCtoSeedButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCToSeedButtonPressed), NULL, this);
	ECCClearButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnECCClearButtonPressed), NULL, this);
	BigIntegerCalculatorXbitIntegerButton->Disconnect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (DiscreteMathCalculatorFrame::OnBigIntegerCalculatorXbitRandomIntegerButtonPress), NULL, this);

}

bool DiscreteMathCalculatorFrame::ProcessEvent (wxEvent& event)
{
	try
	{
		return wxEvtHandler::ProcessEvent (event);
	}

	catch (const ntlException& ntle)
	{
		reportError (wxString (ntle.what().c_str(), wxConvUTF8), wxT ("Error") );
		return true;
	}

	catch (const useAsymptoticEstimateException& uaee)
	{
		::report (wxString (uaee.what().c_str(), wxConvUTF8), wxT ("Note") );

		return true;
	}

	catch (const listingTooLongException& ltle)
	{
		::report (wxString (ltle.what().c_str(), wxConvUTF8), wxT ("Note") );

		return true;
	}

	catch (const btrFileFormatException& btrffe)
	{
		::report (wxString (btrffe.what().c_str(), wxConvUTF8), wxT ("Note") );

		return true;
	}

	catch (const indexRangeError& ire)
	{
		::report (wxString (ire.what().c_str(), wxConvUTF8), wxT ("Note") );

		return true;
	}

	catch (const invalid_argument& ia)
	{
		::report (wxString (ia.what(), wxConvUTF8), wxT ("Note") );

		return true;
	}

	catch (const overflow_error& oe)
	{
		::report (wxString (oe.what(), wxConvUTF8), wxT ("Note") );
		return true;
	}

	catch (const underflow_error& ue)
	{
		::report (wxString (ue.what(), wxConvUTF8), wxT ("Note") );
		return true;
	}

	catch (const domain_error& de)
	{
		::report (wxString (de.what(), wxConvUTF8), wxT ("Note") );
		return true;
	}

	catch (const graphFileFormatException& gffe)
	{
		::report (wxString (gffe.what().c_str(), wxConvUTF8), wxT ("Note") );
		return true;
	}

	catch (const ResourceErrorObject& reo)
	{
		::reportError (wxString (reo.what(), wxConvUTF8), wxT ("Error") );
		return true;
	}

	catch (const ArithmeticErrorObject& aeo)
	{
		::reportError (wxString (aeo.what(), wxConvUTF8), wxT ("Error") );
		return true;
	}

	catch (const LogicErrorObject& leo)
	{
		::reportError (wxString (leo.what(), wxConvUTF8), wxT ("Error") );
		return true;
	}

	catch (const bad_alloc& ba)
	{
		::reportError (wxString (ba.what(), wxConvUTF8), wxT ("Error") );
		return true;
	}

	catch (const bad_alloc& ba)
	{
		::reportError (wxString (ba.what(), wxConvUTF8), wxT ("Error") );
		return true;
	}

	catch (const ErrorObject& eo)
	{
		::reportError (wxString (eo.what(), wxConvUTF8), wxT ("Error") );
		return true;
	}
}

void DiscreteMathCalculatorFrame::OnExit (wxCommandEvent& event)
{
	wxWindowList childWindow;
	wxWindowList::iterator iter;

	childWindow = GetChildren();
	iter = childWindow.begin();

	while (iter != childWindow.end() )
	{
		// if ((*iter)->IsBeingDeleted() == false)
		(*iter)->Close (true);
		iter++;
	}

	if (CleanUp() )
		wxTheApp->ExitMainLoop();
	else
		return;
}

void DiscreteMathCalculatorFrame::OnCloseWindow (wxCloseEvent& event)
{
	wxWindowList childWindow;
	wxWindowList::iterator iter;

	childWindow = GetChildren();
	iter = childWindow.begin();

	while (iter != childWindow.end() )
	{
		if ( (*iter)->IsBeingDeleted() == false)
			(*iter)->Close (true);

		iter++;
	}

	if (CleanUp() )
		wxTheApp->ExitMainLoop();
	else
		return;
}

bool DiscreteMathCalculatorFrame::CleanUp()
{
	if (!saveGraphPreview (true) )
		return false;

	if (!saveRelationPreview (true) )
		return false;

	if (!clipboardChecked)
	{
		clipboardChecked = true;

		if (wxTheClipboard->Open() )
		{
			if (wxTheClipboard->IsSupported (wxDF_BITMAP) )
			{
				wxString message, caption;
				int response;

				message.append (wxT ("You placed a bitmap on the Clipboard.\nDo you want this bitmap to be available to other applications after you quit Discrete Mathematics Calculator?") );
				caption.append (wxT ("Discrete Mathematics Calculator") );

				wxMessageDialog dialog (this, message, caption, wxYES_NO | wxICON_QUESTION);

				response = dialog.ShowModal();

				if (response == wxID_YES)
				{
					wxBitmapDataObject data;

					wxTheClipboard->GetData (data);
					wxTheClipboard->SetData (&data);
					wxTheClipboard->Flush();
					wxTheClipboard->Close();
				}
			}
		}
	}

	return true;
}

void DiscreteMathCalculatorFrame::OnAbout (wxCommandEvent& event)
{
	try
	{
		AboutDiscreteMathCalculator aboutDiscreteMathCalculator (this, 0);

		aboutDiscreteMathCalculator.ShowModal();
	}
	catch (FileErrorObject&)
	{
		return;
	}
}

int DiscreteMathCalculatorFrame::getint (wxTextCtrl* tctrl)
{
	long nValue;

	if (tctrl->GetValue().length() == 0)
		nValue = -1;
	else
		tctrl->GetValue().ToLong (&nValue);

	return nValue;
}

ZZ DiscreteMathCalculatorFrame::getResult()
{
	ZZ result;
	wxString resultValue;
	int pageNumber = discreteMathCalculatorNotebook->GetSelection();

	if (pageNumber == 0)
		resultValue = resultValueTextCtrl->GetValue();
	else
		resultValue = countingAndEnumerationResultTextCtrl->GetValue();

	if (resultValue.length() == 0)
		throw ntlException ("There is no result.");

	conv (result, (const char*) resultValue.c_str() );

	return result;
}

void DiscreteMathCalculatorFrame::setValue (ZZ n, wxTextCtrl* tctrl)
{
	// convert ZZ object result back to a string
	ostringstream numstr;

	numstr << n;
	wxString s = wxString (numstr.str().c_str(), wxConvUTF8);
	tctrl->ChangeValue (s);
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationFactorialButtonClick (wxCommandEvent& event)
{
	if (comb_perm == NULL)
		comb_perm = new CombinationsPermutations (countingAndEnumeration_n_TextCtrl,
		      countingAndEnumeration_k_TextCtrl,
		      countingAndEnumerationResultTextCtrl,
		      countingAndEnumeration_listing_TextCtrl);

	result_comb = comb_perm->factorial();
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationPermutationsButtonClick (wxCommandEvent& event)
{
	if (comb_perm == NULL)
		comb_perm = new CombinationsPermutations (countingAndEnumeration_n_TextCtrl,
		      countingAndEnumeration_k_TextCtrl,
		      countingAndEnumerationResultTextCtrl,
		      countingAndEnumeration_listing_TextCtrl);

	result_comb = comb_perm->permutation();
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationCombinationsButtonClick (wxCommandEvent& event)
{
	if (comb_perm == NULL)
		comb_perm = new CombinationsPermutations (countingAndEnumeration_n_TextCtrl,
		      countingAndEnumeration_k_TextCtrl,
		      countingAndEnumerationResultTextCtrl,
		      countingAndEnumeration_listing_TextCtrl);

	result_comb = comb_perm->combination();
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationFibonacciButtonClick (wxCommandEvent& event)
{
	int n = getint (countingAndEnumeration_n_TextCtrl);
	ZZ fn;

	if (n < 0)
		throw ntlException ("n has no value.");

	fn = fibonacci (n);
	setValue (fn, countingAndEnumerationResultTextCtrl);
	result_comb = fn;
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationCatalanButtonClick (wxCommandEvent& event)
{
	int n = getint (countingAndEnumeration_n_TextCtrl);
	ZZ cn;

	if (n < 0)
		throw ntlException ("n has no value.");

	cn = catalan (n);
	setValue (cn, countingAndEnumerationResultTextCtrl);
	result_comb = cn;
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationSchroderButtonClick (wxCommandEvent& event)
{
	int n = getint (countingAndEnumeration_n_TextCtrl);
	ZZ sn;

	if (n < 0)
		throw ntlException ("n has no value.");

	sn = schroder (n);
	setValue (sn, countingAndEnumerationResultTextCtrl);
	result_comb = sn;
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationBellButtonClick (wxCommandEvent& event)
{
	int n = getint (countingAndEnumeration_n_TextCtrl);
	ZZ bellNumber;

	if (n < 0)
		throw ntlException ("n has no value.");

	bellNumber = bell (n);
	setValue (bellNumber, countingAndEnumerationResultTextCtrl);
	result_comb = bellNumber;
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationStirlingButtonClick (wxCommandEvent& event)
{
	int n = getint (countingAndEnumeration_n_TextCtrl), k = getint (countingAndEnumeration_k_TextCtrl);
	ZZ stirlingNumber;

	if (n < 0)
		throw ntlException ("n has no value.");

	if (k < 0)
		throw ntlException ("k has no value.");

	stirlingNumber = stirling (n, k);
	setValue (stirlingNumber, countingAndEnumerationResultTextCtrl);
	result_comb = stirlingNumber;
}

void DiscreteMathCalculatorFrame::OnAsymptoticEstimateButtonClick (wxCommandEvent& event)
{
	if (comb_perm == NULL)
		comb_perm = new CombinationsPermutations (countingAndEnumeration_n_TextCtrl,
		      countingAndEnumeration_k_TextCtrl,
		      countingAndEnumerationResultTextCtrl,
		      countingAndEnumeration_listing_TextCtrl);

	AsymptoticEstimate* estimate = new AsymptoticEstimate (this, comb_perm, page);
	estimate->ShowModal();
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationListPermutationsButtonClick (wxCommandEvent& event)
{
	if (comb_perm == NULL)
		comb_perm = new CombinationsPermutations (countingAndEnumeration_n_TextCtrl,
		      countingAndEnumeration_k_TextCtrl,
		      countingAndEnumerationResultTextCtrl,
		      countingAndEnumeration_listing_TextCtrl);

	comb_perm->listPermutations();
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationListCombinationsButtonClick (wxCommandEvent& event)
{
	if (comb_perm == NULL)
		comb_perm = new CombinationsPermutations (countingAndEnumeration_n_TextCtrl,
		      countingAndEnumeration_k_TextCtrl,
		      countingAndEnumerationResultTextCtrl,
		      countingAndEnumeration_listing_TextCtrl);

	comb_perm->listCombinations();
}

void DiscreteMathCalculatorFrame::OnpascalTriangleButtonClick (wxCommandEvent& event)
{
	int lastrow = getint (trianglesDataTextCtrl);

	if (lastrow < 0)
		throw ntlException ("n has no value.");

	if (triangles == NULL)
		triangles = new Triangles (trianglesOutputTextCtrl);

	triangles->outputPascalTriangle (lastrow);
}

void DiscreteMathCalculatorFrame::OncatalanTriangleButtonClick (wxCommandEvent& event)
{
	int lastrow = getint (trianglesDataTextCtrl);

	if (lastrow < 0)
		throw ntlException ("n has no value.");

	if (triangles == NULL)
		triangles = new Triangles (trianglesOutputTextCtrl);

	triangles->outputCatalanTriangle (lastrow);
}

void DiscreteMathCalculatorFrame::OnschroderTriangleButtonClick (wxCommandEvent& event)
{
	int lastrow = getint (trianglesDataTextCtrl);

	if (lastrow < 0)
		throw ntlException ("n has no value.");

	if (triangles == NULL)
		triangles = new Triangles (trianglesOutputTextCtrl);

	triangles->outputSchroderTriangle (lastrow);
}

void DiscreteMathCalculatorFrame::OnbellTriangleButtonClick (wxCommandEvent& event)
{
	int lastrow = getint (trianglesDataTextCtrl);

	if (lastrow < 0)
		throw ntlException ("n has no value.");

	if (triangles == NULL)
		triangles = new Triangles (trianglesOutputTextCtrl);

	triangles->outputBellTriangle (lastrow);
}

void DiscreteMathCalculatorFrame::OntrianglesHosoyaButtonClick (wxCommandEvent& event)
{
	int lastrow = getint (trianglesDataTextCtrl);

	if (lastrow < 0)
		throw ntlException ("n has no value.");

	if (triangles == NULL)
		triangles = new Triangles (trianglesOutputTextCtrl);

	triangles->outputHosoyaTriangle (lastrow);
}

void DiscreteMathCalculatorFrame::OnsubsetsNumberOfSubsetsButtonClick (wxCommandEvent& event)
{
	int n = getint (subsetsSetSizeTextCtrl);

	if (n < 0)
		throw ntlException ("n has no value.");

	if (comb_perm == NULL)
		comb_perm = new CombinationsPermutations (countingAndEnumeration_n_TextCtrl,
		      countingAndEnumeration_k_TextCtrl,
		      countingAndEnumerationResultTextCtrl,
		      countingAndEnumeration_listing_TextCtrl);

	if (subsets == NULL)
		subsets = new Subsets (
		   subsetsListingsTextCtrl, subsetsNumberofSubsetsTextCtrl, subsetsNumberOfKElementSubsetsTextCtrl, comb_perm);

	subsets->numberOfSubsets (n);
}

void DiscreteMathCalculatorFrame::OnsubsetsNumberOfKElementSubsetsButtonClick (wxCommandEvent& event)
{
	int n = getint (subsetsSetSizeTextCtrl), k = getint (subsetsKTextCtrl);

	if (n < 0)
		throw ntlException ("n has no value.");

	if (k < 0)
		throw ntlException ("k has no value.");

	if (comb_perm == NULL)
		comb_perm = new CombinationsPermutations (countingAndEnumeration_n_TextCtrl,
		      countingAndEnumeration_k_TextCtrl,
		      countingAndEnumerationResultTextCtrl,
		      countingAndEnumeration_listing_TextCtrl);

	if (subsets == NULL)
		subsets = new Subsets (
		   subsetsListingsTextCtrl, subsetsNumberofSubsetsTextCtrl, subsetsNumberOfKElementSubsetsTextCtrl, comb_perm);

	subsets->numberOfKElementSubsets (n, k);
}

void DiscreteMathCalculatorFrame::OnsubsetsListAllSubsetsButtonClick (wxCommandEvent& event)
{
	int n = getint (subsetsSetSizeTextCtrl);

	if (n < 0)
		throw ntlException ("n has no value.");

	if (comb_perm == NULL)
		comb_perm = new CombinationsPermutations (countingAndEnumeration_n_TextCtrl,
		      countingAndEnumeration_k_TextCtrl,
		      countingAndEnumerationResultTextCtrl,
		      countingAndEnumeration_listing_TextCtrl);

	if (subsets == NULL)
		subsets = new Subsets (
		   subsetsListingsTextCtrl, subsetsNumberofSubsetsTextCtrl, subsetsNumberOfKElementSubsetsTextCtrl, comb_perm);

	subsets->listAllSubsets (n);
}

void DiscreteMathCalculatorFrame::OnsubsetsListAllKElementSubsetsButtonClick (wxCommandEvent& event)
{
	int n = getint (subsetsSetSizeTextCtrl), k = getint (subsetsKTextCtrl);

	if (n < 0)
		throw ntlException ("n has no value.");

	if (k < 0)
		throw ntlException ("k has no value.");

	if (comb_perm == NULL)
		comb_perm = new CombinationsPermutations (countingAndEnumeration_n_TextCtrl,
		      countingAndEnumeration_k_TextCtrl,
		      countingAndEnumerationResultTextCtrl,
		      countingAndEnumeration_listing_TextCtrl);

	if (subsets == NULL)
		subsets = new Subsets (
		   subsetsListingsTextCtrl, subsetsNumberofSubsetsTextCtrl, subsetsNumberOfKElementSubsetsTextCtrl, comb_perm);

	subsets->listAllKElementSubsets (n, k);
}

void DiscreteMathCalculatorFrame::OnpartitionsComputeBellNumberButtonClick (wxCommandEvent& event)
{
	int n = getint (partitions_n_TextCtrl);
	ZZ bellNumber;

	if (n < 0)
		throw ntlException ("n has no value.");

	bellNumber = bell (n);
	setValue (bellNumber, partitionsBellNumberTextCtrl);
}

void DiscreteMathCalculatorFrame::OnpartitionsListPartitionsButtonClick (wxCommandEvent& event)
{
	int n = getint (partitions_n_TextCtrl);

	if (n < 0)
		throw ntlException ("n has no value.");

	if (partitions == NULL)
		partitions = new Partition (partitonsListingTextCtrl);

	partitions->listAllPartitions (n);
}

void DiscreteMathCalculatorFrame::OnpartitionsListStirlingPartitionsButtonClick (wxCommandEvent& event)
{
	int n = getint (partitions_n_TextCtrl), k = getint (partitions_k_TextCtrl);

	if (n < 0)
		throw ntlException ("n has no value.");

	if (k < 0)
		throw ntlException ("k has no value.");

	if (partitions == NULL)
		partitions = new Partition (partitonsListingTextCtrl);

	partitions->listPartitionsSizeK (n, k);
}

void DiscreteMathCalculatorFrame::OnpartitionsBellButtonClick (wxCommandEvent& event)
{
	int n = getint (partitions_n_TextCtrl);
	ZZ bellNumber;

	if (n < 0)
		throw ntlException ("n has no value.");

	bellNumber = bell (n);
	setValue (bellNumber, partitionsBellNumberTextCtrl);
}

void DiscreteMathCalculatorFrame::OnpartitionsStirlingButtonClick (wxCommandEvent& event)
{
	int n = getint (partitions_n_TextCtrl), k = getint (partitions_k_TextCtrl);
	ZZ stirlingNumber;

	if (n < 0)
		throw ntlException ("n has no value.");

	if (k < 0)
		throw ntlException ("k has no value.");

	stirlingNumber = stirling (n, k);
	setValue (stirlingNumber, partitionsStirlingNumberTextCtrl);
}

void DiscreteMathCalculatorFrame::OnbinaryTreesEnterButtonClick (wxCommandEvent& event)
{
	wxString title = binaryTreesGeneralTreeTitleTextCtrl->GetValue();
	wxVector<wxString> fromBTRDialog;

	BinaryTreeDrawingCanvas* theBinaryTree;

	EnterBinaryTree enterTree (this, &fromBTRDialog);

	if (enterTree.ShowModal() == wxID_OK)
	{
		theBinaryTree = (BinaryTreeDrawingCanvas*) launchFrame (BINARY_TREE, title);
		theBinaryTree->makeTreeFromLines (fromBTRDialog);
	}
}

void DiscreteMathCalculatorFrame::inputTreeData (const wxString filename, wxVector<int>& dataInput)
{
	wxTextFile searchTreeInput (filename);
	wxString line, integerToken;
	wxStringTokenizer input;
	long value;

	if (!searchTreeInput.Open() )
	{
		string errMsg ("Cannot open ");

		errMsg += string ( (const char*) filename.c_str() );

		throw btrFileFormatException (errMsg);
	}

	for (line = searchTreeInput.GetFirstLine(); !searchTreeInput.Eof(); line = searchTreeInput.GetNextLine() )
	{
		input.SetString (line);

		while (input.HasMoreTokens() )
		{
			integerToken = input.GetNextToken();

			if (!integerToken.ToLong (&value) )
				throw btrFileFormatException ("Your file contains an invalid integer.");

			dataInput.push_back ( (int) value);
		}
	}
}

void DiscreteMathCalculatorFrame::OnbinaryTreesHuffmanTreeEnterButtonClick (wxCommandEvent& event)
{
	wxString title = binaryTreesHuffmanTreeTitleTextCtrl->GetValue();
	vector<HuffmanNode> fromHuffmanDialog;

	HuffmanDrawingCanvas* theHuffmanTree;

	EnterHuffmanData enterTree (this, -1, &fromHuffmanDialog);

	if (enterTree.ShowModal() == wxID_OK)
	{
		theHuffmanTree = (HuffmanDrawingCanvas*) launchFrame (HUFFMAN_TREE, title);
		theHuffmanTree->buildHuffmanTree (fromHuffmanDialog);
	}
}

void DiscreteMathCalculatorFrame::OnbinaryTreesExpressionTreeEnterButtonClick (wxCommandEvent& event)
{
	wxString title = binaryTreesExpressionTreeTitleTextCtrl->GetValue();
	stringstream parserOutput;
	wxVector<wxString> fromExpressionTreeDialog;
	long long int expValue;

	ExpressionTreeDrawingCanvas* theExressionTree;

	ExpressionTree enterTree (this, &parserOutput, &fromExpressionTreeDialog, &expValue);

	if (enterTree.ShowModal() == wxID_OK)
	{
		theExressionTree = (ExpressionTreeDrawingCanvas*) launchFrame (EXPRESSION_TREE, title);
		theExressionTree->buildExpressionTree (parserOutput, fromExpressionTreeDialog, expValue);
	}
}

void DiscreteMathCalculatorFrame::OngeneralTreeEnterButtonClick (wxCommandEvent& event)
{
	wxString title = generalTreeTitleTextCtrl->GetValue();
	wxVector<wxString> fromGTRDialog;
	GeneralTreeDrawingCanvas* theGeneralTree;

	EnterGeneralTree enterGeneralTree (this, &fromGTRDialog);

	if (enterGeneralTree.ShowModal() == wxID_OK)
	{
		theGeneralTree = (GeneralTreeDrawingCanvas*) launchFrame (GENERAL_TREE, title);
		theGeneralTree->makeTreeFromLines (fromGTRDialog);
	}
}

void DiscreteMathCalculatorFrame::OngeneralTreeEnterFromFileButtonClick (wxCommandEvent& event)
{
	int eventID = event.GetId();
	wxString filename;

	if (eventID == ID_GENERAL_TREES_ENTER_FROM_GML_BUTTON)
		filename = wxFileSelector (wxT ("Choose a general tree definition GML file"),
		                           wxT (""),
		                           wxT (""),
		                           wxT (""),
		                           "GML file (*.gml)|*.gml",
		                           wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	else
		filename = wxFileSelector (
		              wxT ("Choose a general tree definition GTR file"), wxT (""), wxT (""), wxT (""), wxT ("GTR file (*.gtr)|*.gtr") );

	if (filename.empty() )
		return;

	wxString title = generalTreeTitleTextCtrl->GetValue();
	GeneralTreeDrawingCanvas* theGeneralTree;

	wxString extension = filename.AfterLast ('.');
	extension = extension.MakeLower();

	theGeneralTree = (GeneralTreeDrawingCanvas*) launchFrame (GENERAL_TREE, title);

	if (extension == wxT ("gtr") )
		theGeneralTree->makeTreeFromGTRFile (filename);
	else
		theGeneralTree->makeTreeFromGMLFile (filename);
}

wxFrame* DiscreteMathCalculatorFrame::launchFrame (FrameType frameType,
      const wxString& title,
      SearchTreeDrawingCanvas::BalanceType balanceType,
      const wxVector<int>& inputData)
{
	wxFrame* canvas = NULL;
	wxString canvasTitle (title);

	switch (frameType)
	{
	case BINARY_TREE:
		canvas = new BinaryTreeDrawingCanvas (this, canvasTitle, wxID_ANY, wxT ("Binary tree") );
		break;

	case BINARY_SEARCH_TREE:
		if (balanceType != SearchTreeDrawingCanvas::NotBalanced)
			canvas = new SearchTreeDrawingCanvas (this, canvasTitle, balanceType, inputData, wxID_ANY, wxT ("Search tree") );

		break;

	case HUFFMAN_TREE:
		canvas = new HuffmanDrawingCanvas (this, canvasTitle, wxID_ANY, wxT ("Huffman tree") );
		break;

	case EXPRESSION_TREE:
		canvas = new ExpressionTreeDrawingCanvas (this, canvasTitle, wxID_ANY, wxT ("Expression tree") );
		break;

	case HEAP:
		canvas = new HeapDrawingCanvas (this, canvasTitle, inputData, wxID_ANY, wxT ("Heap") );
		break;

	case GENERAL_TREE:
		canvas = new GeneralTreeDrawingCanvas (this, canvasTitle, wxID_ANY, wxT ("General tree") );
		break;
	}

	canvas->Show();
	wxTheApp->SetTopWindow (canvas);

	return canvas;
}

void DiscreteMathCalculatorFrame::OnEnterGMLFile (wxCommandEvent& event)
{
	wxString filename = wxFileSelector (wxT ("Choose a GML file specifying your tree."),
	                                    wxT (""),
	                                    wxT (""),
	                                    wxT (""),
	                                    "GML file (*.gml)|*.gml",
	                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	// wxString filename = wxFileSelector (wxT ("Choose a GML file specifying your tree."));

	if (filename.empty() )
		return;

	FrameType treeType;
	SearchTreeDrawingCanvas::BalanceType balanceType;
	wxString title;
	vector<int> searchTreeDataValues;
	wxFrame* tree;
	long eventId = event.GetId();

	if (eventId == ID_BINARY_TREES_ENTER_GML_BUTTON)
	{
		treeType = BINARY_TREE;
		tree = launchFrame (treeType);
		( (BinaryTreeDrawingCanvas*) tree)->makeTreeFromGMLFile (filename);
	}
	else if (eventId == ID_BINARY_SEARCH_TREES_FROM_GML_BUTTON)
	{
		treeType = BINARY_SEARCH_TREE;
		balanceType = SearchTreeDrawingCanvas::SearchTree;
		tree = launchFrame (treeType, wxT (""), balanceType);
		( (SearchTreeDrawingCanvas*) tree)->makeTreeFromGMLFile (filename);
	}
	else if (eventId == ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_GML_BUTTON)
	{
		int position;

		treeType = BINARY_SEARCH_TREE;
		position = binarySearchTreesBalanceAlgorithmsRadioBox->GetSelection();

		if (position == 0)
			balanceType = SearchTreeDrawingCanvas::AVL;
		else
			balanceType = SearchTreeDrawingCanvas::RedBlack;

		tree = launchFrame (treeType, wxT (""), balanceType);
		( (SearchTreeDrawingCanvas*) tree)->makeTreeFromGMLFile (filename);
	}
	else if (eventId == ID_HEAP_ENTER_FROM_GML_BUTTON)
	{
		treeType = HEAP;
		tree = launchFrame (treeType);
		( (HeapDrawingCanvas*) tree)->makeTreeFromGMLFile (filename);
	}
	else if (eventId == ID_BINARY_TREES_EXPRESSION_TREE_FROM_GML_BUTTON)
	{
		treeType = EXPRESSION_TREE;
		tree = launchFrame (treeType);
		( (ExpressionTreeDrawingCanvas*) tree)->makeTreeFromGMLFile (filename);
	}
	else if (eventId == ID_BINARY_TREES_HUFFMAN_TREE_FROM_GML_BUTTON)
	{
		treeType = HUFFMAN_TREE;
		tree = launchFrame (treeType);
		( (HuffmanDrawingCanvas*) tree)->makeTreeFromGMLFile (filename);
	}
	else
	{
		treeType = GENERAL_TREE;
		tree = launchFrame (treeType);
		( (GeneralTreeDrawingCanvas*) tree)->makeTreeFromGMLFile (filename);
	}
}

void DiscreteMathCalculatorFrame::OnEnterFromTextfileButtonClick (wxCommandEvent& event)
{
	long eventId = event.GetId();
	SearchTreeDrawingCanvas::BalanceType balanceType;
	int position;
	wxString title, message;
	wxVector<int> dataValues;
	FrameType frameType;
	wxFrame* tree;
	wxString filename;
	HeapDrawingCanvas::HeapType heapType;

	if (eventId == ID_BINARY_TREES_SEARCH_TREE_ENTER_FROM_FILE_BUTTON)
	{
		balanceType = SearchTreeDrawingCanvas::SearchTree;
		message = wxT ("Choose a data file for a binary search tree.");
		title = binaryTreesSearchTreesTitleTextCtrl->GetValue();
		frameType = BINARY_SEARCH_TREE;
	}
	else if (eventId == ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_TEXTFILE_BUTTON)
	{
		position = binarySearchTreesBalanceAlgorithmsRadioBox->GetSelection();
		title = binarySearchTreesBalanceAlgorithmsTitleTextCtrl->GetValue();
		frameType = BINARY_SEARCH_TREE;

		if (position == 0)
		{
			balanceType = SearchTreeDrawingCanvas::AVL;
			message = wxT ("Choose a data file for an AVL tree.");
		}
		else
		{
			balanceType = SearchTreeDrawingCanvas::RedBlack;
			message = wxT ("Choose a data file for a Red-Black tree.");
		}
	}
	else
	{
		// ID_ENTER_HEAP_FROM_TEXTFILE_BUTTON
		message = wxT ("Choose a data file for a heap.");
		frameType = HEAP;
		position = enterHeapRadioBox->GetSelection();

		if (position == 0)
			heapType = HeapDrawingCanvas::MINIMUM;
		else
			heapType = HeapDrawingCanvas::MAXIMUM;

		balanceType = SearchTreeDrawingCanvas::NotBalanced;
		title = heapBuildTitleTextCtrl->GetValue();
	}

	filename = wxFileSelector (message, wxT (""), wxT (""), wxT (""), wxT ("*.*") );

	if (filename.empty() )
		return;

	inputTreeData (filename, dataValues);

	tree = launchFrame (frameType, title, balanceType, dataValues);

	// Do we have a heap?
	if (balanceType == SearchTreeDrawingCanvas::NotBalanced)
		( (HeapDrawingCanvas*) tree)->buildHeap (heapType);
}

void DiscreteMathCalculatorFrame::OnDataInputButtonClick (wxCommandEvent& event)
{
	SearchTreeDrawingCanvas::BalanceType balanceType;
	int position;
	wxString title, error;
	wxVector<int> dataValues;
	wxFrame* tree;
	long eventId = event.GetId();
	FrameType frameType;

	EnterData treeData (this, &dataValues, &error);

	if (treeData.ShowModal() == wxID_CANCEL)
	{
		if (error.length() != 0)
			throw btrFileFormatException (string ( (const char*) error.c_str() ) );

		return;
	}

	if (eventId == ID_BINARY_SEARCH_TREES_ENTER_BUTTON)
	{
		frameType = BINARY_SEARCH_TREE;
		balanceType = SearchTreeDrawingCanvas::SearchTree;
		title = binaryTreesSearchTreesTitleTextCtrl->GetValue();
		tree = launchFrame (frameType, title, balanceType, dataValues);
	}
	else if (eventId == ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_ENTER_BUTTON)
	{
		frameType = BINARY_SEARCH_TREE;
		position = binarySearchTreesBalanceAlgorithmsRadioBox->GetSelection();
		title = binarySearchTreesBalanceAlgorithmsTitleTextCtrl->GetValue();

		if (position == 0)
			balanceType = SearchTreeDrawingCanvas::AVL;
		else
			balanceType = SearchTreeDrawingCanvas::RedBlack;

		tree = launchFrame (frameType, title, balanceType, dataValues);
	}
	else
	{
		// heap
		frameType = HEAP;
		position = enterHeapRadioBox->GetSelection();
		balanceType = SearchTreeDrawingCanvas::NotBalanced;
		title = heapBuildTitleTextCtrl->GetValue();
		tree = launchFrame (frameType, title, balanceType, dataValues);
		( (HeapDrawingCanvas*) tree)->buildHeap (position == 0 ? HeapDrawingCanvas::MINIMUM : HeapDrawingCanvas::MAXIMUM);
	}
}

void DiscreteMathCalculatorFrame::OnRandomDataEnterButtonClick (wxCommandEvent& event)
{
	SearchTreeDrawingCanvas::BalanceType balanceType;
	int position;
	wxString title;
	wxVector<int> dataValues;
	wxFrame* tree;
	long eventId = event.GetId();
	RandomData *randomData;

	if (eventId == ID_BINARY_SEARCH_TREES_RANDOM_DATA_ENTER_BUTTON ||
	      eventId == ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RANDOM_DATA_BUTTON)
		randomData = new RandomData(this, &dataValues);
	else
		randomData = new RandomData(this, &dataValues, false);

	if (randomData->ShowModal() == wxID_CANCEL)
		return;

	if (dataValues.empty() )
		return;

	if (eventId == ID_BINARY_SEARCH_TREES_RANDOM_DATA_ENTER_BUTTON)
	{
		balanceType = SearchTreeDrawingCanvas::SearchTree;
		title = binaryTreesSearchTreesTitleTextCtrl->GetValue();
		tree = launchFrame (BINARY_SEARCH_TREE, title, balanceType, dataValues);
	}
	else if (eventId == ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RANDOM_DATA_BUTTON)
	{
		position = binarySearchTreesBalanceAlgorithmsRadioBox->GetSelection();
		title = binarySearchTreesBalanceAlgorithmsTitleTextCtrl->GetValue();

		if (position == 0)
			balanceType = SearchTreeDrawingCanvas::AVL;
		else
			balanceType = SearchTreeDrawingCanvas::RedBlack;

		tree = launchFrame (BINARY_SEARCH_TREE, title, balanceType, dataValues);
	}
	else
	{
		// ID_ENTER_RANDOM_HEAP_BUTTON
		title = heapBuildTitleTextCtrl->GetValue();
		tree = launchFrame (HEAP, title, SearchTreeDrawingCanvas::NotBalanced, dataValues);

		if (enterHeapRadioBox->GetSelection() == 0)
			( (HeapDrawingCanvas*) tree)->buildHeap (HeapDrawingCanvas::MINIMUM);
		else
			( (HeapDrawingCanvas*) tree)->buildHeap (HeapDrawingCanvas::MAXIMUM);
	}
}

void DiscreteMathCalculatorFrame::OnbinaryTreesEnterBTRFromFileButtonClick (wxCommandEvent& event)
{
	wxString filename = wxFileSelector (
	                       wxT ("Choose a binary tree definition file"), wxT (""), wxT (""), wxT (""), wxT ("BTR file (*.btr)|*.btr") );

	if (filename.empty() )
		return;

	wxString title = binaryTreesGeneralTreeTitleTextCtrl->GetValue();
	BinaryTreeDrawingCanvas* theBinaryTree;

	theBinaryTree = (BinaryTreeDrawingCanvas*) launchFrame (BINARY_TREE, title);
	theBinaryTree->makeTreeFromBTRFile (filename);
}

void DiscreteMathCalculatorFrame::OnCreateTreeBTRFileExamplesMenuItemSelected(wxCommandEvent& event)
{
    wxString filename =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/tree-files/binary-trees/binary-tree-help.btr");

    if (!wxFileExists(filename))
    {
        wxMessageBox(wxT("Binary Tree example file not found:\n") + filename);
        return;
    }

    BinaryTreeDrawingCanvas* theBinaryTree =
        (BinaryTreeDrawingCanvas*) launchFrame(BINARY_TREE, wxT("Binary Tree Example"));

    theBinaryTree->makeTreeFromBTRFile(filename);
}

void DiscreteMathCalculatorFrame::OnCreateTreeGTRFileExamplesMenuItemSelected(wxCommandEvent& event)
{
    wxString filename =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/tree-files/general-trees/general-tree.gtr");

    if (!wxFileExists(filename))
    {
        wxMessageBox(wxT("General Tree example file not found:\n") + filename);
        return;
    }

    GeneralTreeDrawingCanvas* theGeneralTree =
        (GeneralTreeDrawingCanvas*) launchFrame(GENERAL_TREE, wxT("General Tree Example"));

    theGeneralTree->makeTreeFromGTRFile(filename);
}

void DiscreteMathCalculatorFrame::OnCreateSearchTreeFileExamplesMenuItemSelected(wxCommandEvent& event)
{
    wxString filename =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/tree-files/binary-search-trees/binary-search-tree-help.txt");

    if (!wxFileExists(filename))
    {
        wxMessageBox(wxT("Binary Search Tree example file not found:\n") + filename);
        return;
    }

    wxVector<int> dataInput;

    inputTreeData(filename, dataInput);

    launchFrame(
        BINARY_SEARCH_TREE,
        wxT("Binary Search Tree Example"),
        SearchTreeDrawingCanvas::SearchTree,
        dataInput);
}

void DiscreteMathCalculatorFrame::OnCreateHeapFileExamplesMenuItemSelected(wxCommandEvent& event)
{
    wxString defaultDirectory =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/tree-files/heaps");

    wxDirDialog dirDialog(
        this,
        wxT("Select the type of heap."),
        defaultDirectory,
        wxDD_DIR_MUST_EXIST);

    if (dirDialog.ShowModal() != wxID_OK)
        return;

    wxString directoryForHeapType = dirDialog.GetPath();
    wxString typeOfHeap = directoryForHeapType.AfterLast('/');

    wxString filename;

    if (typeOfHeap == wxT("minimum"))
    {
        enterHeapRadioBox->SetSelection(0);
        filename = directoryForHeapType + wxT("/minimum-heap.gml");
    }
    else if (typeOfHeap == wxT("maximum"))
    {
        enterHeapRadioBox->SetSelection(1);
        filename = directoryForHeapType + wxT("/maximum-heap.gml");
    }
    else
    {
        wxMessageBox(wxT("Please select the minimum or maximum directory."));
        return;
    }

    if (!wxFileExists(filename))
    {
        wxMessageBox(wxT("Heap example file not found:\n") + filename);
        return;
    }

    wxFrame* tree = launchFrame(HEAP);

    ((HeapDrawingCanvas*) tree)->makeTreeFromGMLFile(filename);
}

void DiscreteMathCalculatorFrame::OnCreateHuffmanTreeFileExamplesMenuItemSelected(wxCommandEvent& event)
{
    wxString directory =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/tree-files/Huffman-compression/png-file/");

    wxString pngFile = directory + wxT("example.png");
    wxString bitcodeFile = directory + wxT("example.bitcode");

    if (!wxFileExists(pngFile) || !wxFileExists(bitcodeFile))
    {
        wxMessageBox(wxT("Huffman example files not found:\n") + directory);
        return;
    }

    // Open the Huffman tree image in Preview.
    if (!wxLaunchDefaultApplication(pngFile))
    {
        wxMessageBox(wxT("Unable to open Huffman image:\n") + pngFile);
        return;
    }

    // Read the Huffman compression codes.
    wxTextFile textFile(bitcodeFile);

    if (!textFile.Open())
    {
        wxMessageBox(wxT("Unable to open Huffman bitcode file."));
        return;
    }

    wxString contents;

    for (size_t i = 0; i < textFile.GetLineCount(); ++i)
    {
        contents += textFile.GetLine(i) + wxT("\n");
    }

    textFile.Close();

    // Display the codes in a separate window.
    wxFrame* frame = new wxFrame(
        this,
        wxID_ANY,
        wxT("Huffman Compression Codes"),
        wxDefaultPosition,
        wxSize(650, 500));

    new wxTextCtrl(
        frame,
        wxID_ANY,
        contents,
        wxDefaultPosition,
        wxDefaultSize,
        wxTE_MULTILINE | wxTE_READONLY);

    frame->Show(true);
}

void DiscreteMathCalculatorFrame::OnCreateExpressionTreeFileExamplesMenuItemSelected(wxCommandEvent& event)
{
    wxString filename =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/tree-files/expression-trees/expression.gml");

    if (!wxFileExists(filename))
    {
        wxMessageBox(wxT("Expression Tree example file not found:\n") + filename);
        return;
    }

    ExpressionTreeDrawingCanvas* tree =
        (ExpressionTreeDrawingCanvas*) launchFrame(
            EXPRESSION_TREE,
            wxT("Expression Tree Example"));

    tree->makeTreeFromGMLFile(filename);
}

void DiscreteMathCalculatorFrame::OnCreateBalancedTreeFileExamplesMenuItemSelected(wxCommandEvent& event)
{
    wxString defaultDirectory =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/tree-files/balanced-search-trees");

    wxDirDialog dirDialog(
        this,
        wxT("Select AVL-trees or red-black-trees"),
        defaultDirectory,
        wxDD_DIR_MUST_EXIST);

    if (dirDialog.ShowModal() != wxID_OK)
        return;

    wxString directory = dirDialog.GetPath();
    wxString typeOfTreeBalance = directory.AfterLast('/');

    wxString filename;
    SearchTreeDrawingCanvas::BalanceType balanceType;

    if (typeOfTreeBalance == wxT("AVL-trees"))
    {
        filename = directory + wxT("/AVL-tree.gml");
        balanceType = SearchTreeDrawingCanvas::AVL;
        binarySearchTreesBalanceAlgorithmsRadioBox->SetSelection(0);
    }
    else if (typeOfTreeBalance == wxT("red-black-trees"))
    {
        filename = directory + wxT("/Red-Black-tree.gml");
        balanceType = SearchTreeDrawingCanvas::RedBlack;
        binarySearchTreesBalanceAlgorithmsRadioBox->SetSelection(1);
    }
    else
    {
        wxMessageBox(wxT("Please select either AVL-trees or red-black-trees."));
        return;
    }

    if (!wxFileExists(filename))
    {
        wxMessageBox(wxT("Balanced Search Tree example file not found:\n") + filename);
        return;
    }

    wxFrame* tree = launchFrame(
        BINARY_SEARCH_TREE,
        wxT("Balanced Search Tree Example"),
        balanceType);

    ((SearchTreeDrawingCanvas*) tree)->makeTreeFromGMLFile(filename);
}

void DiscreteMathCalculatorFrame::OngraphsRunAlgorithmsButtonClick (wxCommandEvent& event)
{
	if (myGraph == NULL)
	{
		::report(wxT("There is no graph."), wxT("Note"));
		return;
	}

	myGraph->launchRunAlgorithms (this);
}

void DiscreteMathCalculatorFrame::resetSlider()
{
	graphsPreviewSlider->SetValue (50);
	previewWindow->SetScale (1.0);
	previewWindow->Refresh (true);
}

void DiscreteMathCalculatorFrame::OngraphsPreviewSliderCmdScrollChanged (wxScrollEvent& event)
{
	previewWindow->SetScale (double (graphsPreviewSlider->GetValue() ) / 50);
	previewWindow->Refresh (true);
}

void DiscreteMathCalculatorFrame::handleMenuChoice (wxCommandEvent& event)
{
	wxString graphicsFileName;


	if (menuHandler != NULL)
		delete menuHandler;

	if (event.GetId() == ID_GRAPHS_PREVIEW_WINDOW_SAVE_AS_MENUITEM)
		menuHandler = new MenuEventHandler (previewWindow,
		                                    ID_GRAPHS_PREVIEW_WINDOW_PRINT_MENUITEM,
		                                    ID_GRAPHS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM,
		                                    ID_GRAPHS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM,
		                                    ID_GRAPHS_PREVIEW_WINDOW_SAVE_AS_MENUITEM,
		                                    -1,
		                                    &figureSaved, graphType);
	else
		menuHandler = new MenuEventHandler (previewWindow,
		                                    ID_GRAPHS_PREVIEW_WINDOW_PRINT_MENUITEM,
		                                    ID_GRAPHS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM,
		                                    ID_GRAPHS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM,
		                                    ID_GRAPHS_PREVIEW_WINDOW_SAVE_AS_MENUITEM,
		                                    -1,
		                                    &figureSaved);
	if (page == 7)
		menuHandler->process (event, 3);
	else
		menuHandler->process(event, 4);
}

void DiscreteMathCalculatorFrame::handleRelationGraphMenuChoice (wxCommandEvent& event)
{
	wxString graphicsFileName;

	if (relationMenuHandler != NULL)
		delete relationMenuHandler;

	relationMenuHandler = new MenuEventHandler (relationPartialOrderDrawing,
	      ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_MENUITEM,
	      ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM,
	      ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM,
	      ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_SAVE_AS_MENUITEM,
	      -1,
	      &relationFigureSaved, graphType);

	relationMenuHandler->process (event,3);

	/*#ifdef __APPLE__

		if (event.GetId() == ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM)
		{
			buildRelationsPartialOrdersFileMenu();
		}
	#endif*/
}

void DiscreteMathCalculatorFrame::addBigIntegerCalculatorToolsMenu()
{
	toolsMenu = new wxMenu();
	discreteMathCalculatorBigIntegerFactorMenuItem = new wxMenuItem (BigIntegerToolsMenu, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_FACTOR_MENUITEM,
	      _ ("Factor..."), _ ("Factor an integer"), wxITEM_NORMAL);
	discreteMathCalculatorBigIntegerFactorMenuItem->Enable(true);
	toolsMenu->Append (discreteMathCalculatorBigIntegerFactorMenuItem);
	discreteMathCalculatorBigIntegerDiscreteLogarithmMenuItem = new wxMenuItem (BigIntegerToolsMenu, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DISCRETE_LOGARITHM_MENUITEM,
	      _ ("Discrete logarithm..."), _ ("Try to compute a discrete logarithm"), wxITEM_NORMAL);
	discreteMathCalculatorBigIntegerDiscreteLogarithmMenuItem->Enable(true);
	toolsMenu->Append (discreteMathCalculatorBigIntegerDiscreteLogarithmMenuItem);
	discreteMathCalculatorBigIntegerDiffieHellmanKeyExchangeMenuItem = new wxMenuItem (BigIntegerToolsMenu, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DIFFIE_HELLMANKEYEXCHANGE_MENUITEM,
	      _ ("Diffie-Hellman key exchange (with modular exponentiation(..."), _ ("Secure key exchange prior to transmitting data"), wxITEM_NORMAL);
	discreteMathCalculatorBigIntegerDiffieHellmanKeyExchangeMenuItem->Enable(true);
	toolsMenu->Append (discreteMathCalculatorBigIntegerDiffieHellmanKeyExchangeMenuItem);
	discreteMathCalculatorBigIntegerDigitalSignatureMenuItem = new wxMenuItem (BigIntegerToolsMenu, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DIGITAL_SIGNATURE_MENUITEM,
	      _ ("Digital signature..."), _ ("Create and use digital signature"), wxITEM_NORMAL);
	discreteMathCalculatorBigIntegerDigitalSignatureMenuItem->Enable(true);
	toolsMenu->Append (discreteMathCalculatorBigIntegerDigitalSignatureMenuItem);

	discreteMathCalculatorBigIntegerRSAMenuItem = new wxMenuItem (BigIntegerToolsMenu, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_RSA_MENUITEM,
	      _ ("RSA Guide..."), _ ("Sequence of steps for RSA algorithm"), wxITEM_NORMAL);
	discreteMathCalculatorBigIntegerRSAMenuItem->Enable(true);
	toolsMenu->Append (discreteMathCalculatorBigIntegerRSAMenuItem);
	discreteMathCalculatorMenuBar->Append (toolsMenu, _ ("&Applications") );
}

void DiscreteMathCalculatorFrame::removeAllMenusFromMenuBar()
{
	int numMenusOnMenuBar = discreteMathCalculatorMenuBar->GetMenuCount();
	wxMenu* menu;

	for (int i=0; i < numMenusOnMenuBar; i++)
	{
		menu = discreteMathCalculatorMenuBar->Remove(0);
		delete menu;
	}
}

void DiscreteMathCalculatorFrame::buildHelpMenu()
{
	discreteMathCalculatorHelpMenu = new wxMenu();
	discreteMathCalculatorHelpMenu->Append (wxID_ABOUT);
	discreteMathCalculatorHelpMenuItem = new wxMenuItem (discreteMathCalculatorHelpMenu,
	      ID_DISCRETE_MATH_CALCULATOR_HELP_MENUITEM,
	      _ ("View Help\tF1"),
	      _ ("Obtain help for the application."),
	      wxITEM_NORMAL);
	discreteMathCalculatorHelpMenu->Append (discreteMathCalculatorHelpMenuItem);
	// aboutMenuItem = new wxMenuItem(DiscreteMathCalculatorHelpMenu, wxID_ABOUT, _("About DiscreteMathCalculator..."),
	// _("Show info about this application"), wxITEM_NORMAL);
	discreteMathCalculatorMenuBar->Append (discreteMathCalculatorHelpMenu, _ ("&Help") );
}

void DiscreteMathCalculatorFrame::buildBigIntegerToolsMenu()
{
	BigIntegerToolsMenu = new wxMenu();
	discreteMathCalculatorBigIntegerFactorMenuItem = new wxMenuItem (BigIntegerToolsMenu, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_FACTOR_MENUITEM,
	      _ ("Factor..."), _ ("Factor an integer"), wxITEM_NORMAL);
	BigIntegerToolsMenu->Append (discreteMathCalculatorBigIntegerFactorMenuItem);
	discreteMathCalculatorBigIntegerDiscreteLogarithmMenuItem = new wxMenuItem (BigIntegerToolsMenu, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DISCRETE_LOGARITHM_MENUITEM,
	      _ ("Discrete logarithm..."), _ ("Try to compute a discrete logarithm"), wxITEM_NORMAL);
	BigIntegerToolsMenu->Append (discreteMathCalculatorBigIntegerDiscreteLogarithmMenuItem);

	discreteMathCalculatorBigIntegerDiffieHellmanKeyExchangeMenuItem = new wxMenuItem (BigIntegerToolsMenu, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DIFFIE_HELLMANKEYEXCHANGE_MENUITEM,
	      _ ("Diffie-Hellman key exchange (with modular exponentiation)..."), _ ("Secure key exchange prior to transmitting data"), wxITEM_NORMAL);
	BigIntegerToolsMenu->Append (discreteMathCalculatorBigIntegerDiffieHellmanKeyExchangeMenuItem);
	discreteMathCalculatorBigIntegerDigitalSignatureMenuItem = new wxMenuItem (BigIntegerToolsMenu, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DIGITAL_SIGNATURE_MENUITEM,
	      _ ("Digital signature..."), _ ("Create and use digital signature"), wxITEM_NORMAL);
	BigIntegerToolsMenu->Append (discreteMathCalculatorBigIntegerDigitalSignatureMenuItem);
	discreteMathCalculatorBigIntegerRSAMenuItem = new wxMenuItem (BigIntegerToolsMenu, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_RSA_MENUITEM,
	      _ ("RSA Guide..."), _ ("Sequence of steps for RSA algorithm"), wxITEM_NORMAL);
	BigIntegerToolsMenu->Append (discreteMathCalculatorBigIntegerRSAMenuItem);
	discreteMathCalculatorMenuBar->Append (BigIntegerToolsMenu, _ ("&Applications") );
}
void DiscreteMathCalculatorFrame::buildMenuBarFor(int tab)
{
	removeAllMenusFromMenuBar();
	switch(tab)
	{
	case 0:  // Big Integer Calculator
		buildFileMenu();
		buildHelpMenu();
		buildBigIntegerToolsMenu();
		break;

	case 1:  // Counting and Enumeration
		buildFileMenu();
		buildHelpMenu();
		break;

	case 2:  // Triangles
		buildFileMenu();
		buildHelpMenu();
		break;

	case 3: // Subsets
		buildFileMenu();
		buildHelpMenu();
		break;

	case 4: // Partitions
		buildFileMenu();
		buildHelpMenu();
		break;

	case 5: // Trees
		buildFileMenu();
		buildHelpMenu();
		buildTreeExamplesMenu();
		break;

	case 6: // Graphs
	   buildFileMenu();
	   buildCreateMenu();
	   buildHelpMenu();
	   buildGraphExamplesMenu();
		break;

	case 7: // Relations and Partial Orders
    	buildRelationsPartialOrdersFileMenu();
    	buildHelpMenu();
    	buildRelationsPartialOrderExamplesMenu();
		break;

	case 8: // Elliptic Cubic Curves
    	buildFileMenu();
    	buildHelpMenu();
    	buildGFpEllipticCurvesMenu();
    	buildEllipticCubicCurveExamplesMenu();
    	break;
	}
}

void DiscreteMathCalculatorFrame::buildFileMenu()
{
	fileMenu = new wxMenu();
	fileMenu->Append (wxID_EXIT, wxT ("&Exit\tAlt-F4"), wxT ("Quit DiscreteMathCalculator") );
	fileMenu->AppendSeparator();
	fileMenu->AppendSeparator();

	graphPreviewPrintMenuItem = new wxMenuItem (
	   fileMenu, ID_GRAPHS_PREVIEW_WINDOW_PRINT_MENUITEM, _ ("Print...\tCTRL-P"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append (graphPreviewPrintMenuItem);

	// After choosing, locks up the File menu. May look at this at a later time.
	/*graphPreviewPrintPreviewMenuItem = new wxMenuItem (
	   fileMenu, ID_GRAPHS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM,_ ("Print Preview\tCTRL-SHIFT-P"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append (graphPreviewPrintPreviewMenuItem);*/

	graphPreviewPageSetupMenuItem = new wxMenuItem (
	   fileMenu, ID_GRAPHS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM, _ ("Page Setup..."), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append (graphPreviewPageSetupMenuItem);

	graphPreviewSaveAsMenuItem = new wxMenuItem (fileMenu,
	      ID_GRAPHS_PREVIEW_WINDOW_SAVE_AS_MENUITEM,
	      _ ("Save as...\tCTRL-SHIFT-S"),
	      wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append (graphPreviewSaveAsMenuItem);

	discreteMathCalculatorMenuBar->Insert (0, fileMenu, _ ("&File") );
}

void DiscreteMathCalculatorFrame::buildRelationsPartialOrdersFileMenu()
{
	fileMenu = new wxMenu();
	fileMenu->Append (wxID_EXIT, wxT ("&Exit\tAlt-F4"), wxT ("Quit DiscreteMathCalculator") );
	fileMenu->AppendSeparator();
	fileMenu->AppendSeparator();

	graphPreviewPrintMenuItem = new wxMenuItem (
	   fileMenu, ID_GRAPHS_PREVIEW_WINDOW_PRINT_MENUITEM, _ ("Print...\tCTRL-P"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append (graphPreviewPrintMenuItem);

	// After choosing, locks up the File menu. May look at this at a later time.
	/*graphPreviewPrintPreviewMenuItem = new wxMenuItem (
	   fileMenu, ID_GRAPHS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM,_ ("Print Preview\tCTRL-SHIFT-P"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append (graphPreviewPrintPreviewMenuItem);*/

	graphPreviewPageSetupMenuItem = new wxMenuItem (
	   fileMenu, ID_GRAPHS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM, _ ("Page Setup..."), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append (graphPreviewPageSetupMenuItem);

	/*graphPreviewSaveAsMenuItem = new wxMenuItem (fileMenu,
	      ID_GRAPHS_PREVIEW_WINDOW_SAVE_AS_MENUITEM,
	      _ ("Save as...\tCTRL-SHIFT-S"),
	      wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append (graphPreviewSaveAsMenuItem);*/

	discreteMathCalculatorMenuBar->Insert (0, fileMenu, _ ("&File") );
}

void DiscreteMathCalculatorFrame::buildCreateMenu()
{
	createGraphMenu = new wxMenu();
	createRandomGraphMenuItem = new wxMenuItem (
	   createGraphMenu, ID_CREATE_RANDOM_GRAPH_MENUITEM, _ ("Random Graph..."), wxEmptyString, wxITEM_NORMAL);
	createGraphMenu->Append (createRandomGraphMenuItem);
	drawKnMenuItem = new wxMenuItem (createGraphMenu, ID_DRAW_KN_MENUITEM, _ ("Draw Kn..."), wxEmptyString, wxITEM_NORMAL);
	createGraphMenu->Append (drawKnMenuItem);
	graphsMakeGraphWithACliqueMenuItem = new wxMenuItem (
	   createGraphMenu, ID_GRAPHS_MAKE_CLIQUE, _ ("Make Graph with a Clique..."), wxEmptyString, wxITEM_NORMAL);
	createGraphMenu->Append (graphsMakeGraphWithACliqueMenuItem);
	hypercubeMenuItem =
	   new wxMenuItem (createGraphMenu, ID_HYPERCUBE_MENUITEM, _ ("Hypercube..."), wxEmptyString, wxITEM_NORMAL);
	createGraphMenu->Append (hypercubeMenuItem);
	stableMarriageMenuItem = new wxMenuItem (
	   createGraphMenu, ID_STABLE_MARRIAGE_MENUITEM, _ ("Stable Marriage Problem..."), wxEmptyString, wxITEM_NORMAL);
	createGraphMenu->Append (stableMarriageMenuItem);
	createTSPMenuItem =
	   new wxMenuItem (createGraphMenu, ID_CREATE_TSP_MENUITEM, _ ("TSP..."), wxEmptyString, wxITEM_NORMAL);
	createGraphMenu->Append (createTSPMenuItem);
	createHamCycleMenuItem = new wxMenuItem (
	   createGraphMenu, ID_CREATE_HAMCYCLE_MENUITEM, _ ("Test for Hamiltonian Cycle..."), wxEmptyString, wxITEM_NORMAL);
	createGraphMenu->Append (createHamCycleMenuItem);
	generalizedPetersonGraphMenuItem = new wxMenuItem (createGraphMenu,
	      ID_CREATE_GENERALIZED_PETERSON_GRAPH_MENUITEM,
	      _ ("Generalized Peterson Graph..."),
	      wxEmptyString,
	      wxITEM_NORMAL);
	createGraphMenu->Append (generalizedPetersonGraphMenuItem);

	createGridGraphMenuItem = new wxMenuItem (
	   createGraphMenu, ID_GRID_GRAPH_MENUITEM, _ ("Create a Grid Graph..."), wxEmptyString, wxITEM_NORMAL);
	createGraphMenu->Append (createGridGraphMenuItem);

	createGraphSequenceGraphMenuItem = new wxMenuItem (
	   createGraphMenu, ID_GRAPHICAL_SEQUENCE_GRAPH_MENUITEM, _ ("Test for Graphical Sequence..."), wxEmptyString, wxITEM_NORMAL);
	createGraphMenu->Append (createGraphSequenceGraphMenuItem);
	discreteMathCalculatorMenuBar->Insert (1, createGraphMenu, _ ("&Create") );
}

void DiscreteMathCalculatorFrame::buildGFpEllipticCurvesMenu()
{
	ECCCApplications = new wxMenu();

	discreteMathCalculatorBigIntegerECDLPMenuItem = new wxMenuItem (ECCCApplications, ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_ECDLP_MENUITEM,
	      _ ("Diffie-Hellman with Elliptic Curve..."), _ ("Secure key transfer"), wxITEM_NORMAL);
	ECCCApplications->Append (discreteMathCalculatorBigIntegerECDLPMenuItem);

	ellipticCurveDigitalSignatureMenuItem = new wxMenuItem (ECCCApplications, ID_ELLIPTIC_CURVE_DIGITAL_SIGNATURE_MENUITEM,
	      _ ("Digital Signature with Elliptic Curve..."), _ ("Secure document transfer"), wxITEM_NORMAL);
	ECCCApplications->Append (ellipticCurveDigitalSignatureMenuItem);

	discreteMathCalculatorMenuBar->Append (ECCCApplications, _ ("&Applications") );

	base = 10;
}

void DiscreteMathCalculatorFrame::buildRelationsPartialOrderExamplesMenu()
{
	RelationPartialOrderExamples = new wxMenu();

	RelationExamplesMenuItem = new wxMenuItem(
		RelationPartialOrderExamples,
		ID_RELATIONS_EXAMPLES_MENUITEM,
		_("Relations Examples..."),
		_("Relations examples"),
		wxITEM_NORMAL);

	RelationPartialOrderExamples->Append(RelationExamplesMenuItem);

	PartialOrderExamplesMenuItem = new wxMenuItem(
		RelationPartialOrderExamples,
		ID_PARTIAL_ORDER_EXAMPLES_MENUITEM,
		_("Partial Order Examples..."),
		_("Partial Order examples"),
		wxITEM_NORMAL);

	RelationPartialOrderExamples->Append(PartialOrderExamplesMenuItem);

	discreteMathCalculatorMenuBar->Append(
		RelationPartialOrderExamples,
		_("&Examples"));
}

void DiscreteMathCalculatorFrame::OnCreateRelationExamplesMenuItemSelected(
    wxCommandEvent& event)
{
    wxString imagePath;

#ifdef __APPLE__
    CFBundleRef bundle = CFBundleGetMainBundle();

    if (bundle == nullptr)
    {
        wxMessageBox("Cannot locate application bundle.");
        return;
    }

    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(bundle);

    if (resourcesURL == nullptr)
    {
        wxMessageBox("Cannot locate application resources.");
        return;
    }

    UInt8 path[PATH_MAX];

    bool success = CFURLGetFileSystemRepresentation(
        resourcesURL, true, path, sizeof(path));

    CFRelease(resourcesURL);

    if (!success)
    {
        wxMessageBox("Cannot determine resource directory.");
        return;
    }

    imagePath = wxString::FromUTF8(
        reinterpret_cast<const char*>(path));

    imagePath +=
        "/examples/relations-partial-orders/relations/Relations.png";
#else
    imagePath =
        "examples/relations-partial-orders/relations/Relations.png";
#endif

    if (!wxFileExists(imagePath))
    {
        wxMessageBox("Example image not found:\n" + imagePath);
        return;
    }

    if (!wxLaunchDefaultApplication(imagePath))
        wxMessageBox("Unable to open:\n" + imagePath);
}

void DiscreteMathCalculatorFrame::OnCreatePartialOrderExamplesMenuItemSelected(
    wxCommandEvent& event)
{
    wxString imagePath;

#ifdef __APPLE__
    CFBundleRef bundle = CFBundleGetMainBundle();

    if (bundle == nullptr)
    {
        wxMessageBox("Cannot locate application bundle.");
        return;
    }

    CFURLRef resourcesURL =
        CFBundleCopyResourcesDirectoryURL(bundle);

    if (resourcesURL == nullptr)
    {
        wxMessageBox("Cannot locate application resources.");
        return;
    }

    UInt8 path[PATH_MAX];

    bool success = CFURLGetFileSystemRepresentation(
        resourcesURL, true, path, sizeof(path));

    CFRelease(resourcesURL);

    if (!success)
    {
        wxMessageBox("Cannot determine resource directory.");
        return;
    }

    imagePath = wxString::FromUTF8(
        reinterpret_cast<const char*>(path));

    imagePath +=
        "/examples/relations-partial-orders/partial-orders/Partial-Order.png";

#else
    imagePath =
        "examples/relations-partial-orders/partial-orders/Partial-Order.png";
#endif

    if (!wxFileExists(imagePath))
    {
        wxMessageBox(
            "Example image not found:\n" + imagePath,
            "Partial Order Examples",
            wxOK | wxICON_ERROR,
            this
        );
        return;
    }

    if (!wxLaunchDefaultApplication(imagePath))
    {
        wxMessageBox(
            "Unable to open:\n" + imagePath,
            "Partial Order Examples",
            wxOK | wxICON_ERROR,
            this
        );
    }
}

void DiscreteMathCalculatorFrame::buildTreeExamplesMenu()
{
	TreeExamples = new wxMenu();

	BinaryTreeExamples = new wxMenuItem(
		TreeExamples,
		ID_BINARY_TREE_EXAMPLES_MENUITEM,
		_("Binary Tree Examples..."),
		_("Display a general binary tree."),
		wxITEM_NORMAL);
	TreeExamples->Append(BinaryTreeExamples);

	BinarySearchTreeExamples = new wxMenuItem(
		TreeExamples,
		ID_SEARCH_TREE_EXAMPLES_MENUITEM,
		_("Binary Search Tree Examples..."),
		_("Display a binary search tree."),
		wxITEM_NORMAL);
	TreeExamples->Append(BinarySearchTreeExamples);

	HeapExamples = new wxMenuItem(
		TreeExamples,
		ID_HEAP_EXAMPLES_MENUITEM,
		_("Heap Examples..."),
		_("Display a minimum or maximum heap."),
		wxITEM_NORMAL);
	TreeExamples->Append(HeapExamples);

	HuffmanTreeExamples = new wxMenuItem(
		TreeExamples,
		ID_HUFFMAN_TREE_EXAMPLES_MENUITEM,
		_("Huffman Tree Examples..."),
		_("Display a Huffman compression tree."),
		wxITEM_NORMAL);
	TreeExamples->Append(HuffmanTreeExamples);

	ExpressionTreeExamples = new wxMenuItem(
		TreeExamples,
		ID_EXPRESSION_TREE_EXAMPLES_MENUITEM,
		_("Expression Tree Examples..."),
		_("Display an expression tree."),
		wxITEM_NORMAL);
	TreeExamples->Append(ExpressionTreeExamples);

	BalancedBinarySearchTreeExamples = new wxMenuItem(
		TreeExamples,
		ID_BALANCED_SEARCH_TREE_EXAMPLES_MENUITEM,
		_("Balanced Binary Search Tree Examples..."),
		_("Display an AVL or red-black binary search tree."),
		wxITEM_NORMAL);
	TreeExamples->Append(BalancedBinarySearchTreeExamples);

	GeneralTreeExmples = new wxMenuItem(
		TreeExamples,
		ID_GENERAL_TREE_EXAMPLES_MENUITEM,
		_("General Tree Examples..."),
		_("Display a general tree."),
		wxITEM_NORMAL);
	TreeExamples->Append(GeneralTreeExmples);

	discreteMathCalculatorMenuBar->Append(
		TreeExamples,
		_("&Examples"));
}

void DiscreteMathCalculatorFrame::OnCreatGraphLayoutExamplesMenuItemSelected(
    wxCommandEvent& event)
{
    wxString defaultDirectory =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/graph-files/layouts");

    wxDirDialog dirDialog(
        this,
        wxT("Single click the layout you want to see."),
        defaultDirectory,
        wxDD_DIR_MUST_EXIST);

    if (dirDialog.ShowModal() != wxID_OK)
        return;

    wxString selectedDirectory = dirDialog.GetPath();

    // Determine the selected layout algorithm.
    wxString layoutName = selectedDirectory.AfterLast('/');

    if (layoutName == wxT("hierarchical"))
    {
        graphLayout = hierarchical;
        graphLayoutRadioBox->SetSelection(0);
    }
    else if (layoutName == wxT("force-directed"))
    {
        graphLayout = forcedirected;
        graphLayoutRadioBox->SetSelection(1);
    }
    else if (layoutName == wxT("orthogonal"))
    {
        graphLayout = orthogonal;
        graphLayoutRadioBox->SetSelection(2);
    }
    else if (layoutName == wxT("circular"))
    {
        graphLayout = circular;
        graphLayoutRadioBox->SetSelection(3);
    }
    else
    {
        wxMessageBox(
            wxT("Please select a layout directory: ")
            wxT("hierarchical, force-directed, orthogonal, or circular."));
        return;
    }

    // Determine whether this is a directed or undirected graph.
    wxString parentDirectory =
        selectedDirectory.BeforeLast('/');

    wxString graphCategory =
        parentDirectory.AfterLast('/');

    if (graphCategory == wxT("directed-graphs"))
    {
        graphType = digraph;
        isDigraph = true;
        isWeighted = false;
        graphsGraphTypeRadioBox->SetSelection(1);
    }
    else if (graphCategory == wxT("undirected-graphs"))
    {
        graphType = undir;
        isDigraph = false;
        isWeighted = false;
        graphsGraphTypeRadioBox->SetSelection(0);
    }
    else
    {
        wxMessageBox(wxT("Unrecognized graph category."));
        return;
    }

    graphsWeightedCheckBox->SetValue(false);
    graphsHideEdgesCheckBox->SetValue(false);
    hideEdges = false;

    wxString filename =
        parentDirectory + wxT("/template.grph");

    if (!wxFileExists(filename))
    {
        wxMessageBox(
            wxT("Graph template not found:\n") + filename);
        return;
    }

    if (!saveGraphPreview(false))
        return;

    if (myGraph != NULL)
        delete myGraph;

    myGraph = new BuildGraph(
        wxT(""),
        graphType,
        graphLayout,
        true,
        allowParallelEdges);

    myGraph->setDrawingCanvas(previewWindow);
    myGraph->buildFromGRPH(filename, false, true);

    previewWindow->setGA(myGraph->getGraphAttributes());
    previewWindow->RefreshCanvas(
        true, previewWindow->GetRect());

    figureSaved = false;
}

class GraphExampleFrame : public wxFrame
{
public:
    GraphExampleFrame(
        wxWindow* parent,
        const wxString& title,
        const wxString& filename,
        const wxPoint& position)
        : wxFrame(
              parent,
              wxID_ANY,
              title,
              position,
              wxSize(650, 550)),
          graph(nullptr)
    {
        wxPanel* panel = new wxPanel(this);

        wxBoxSizer* sizer =
            new wxBoxSizer(wxVERTICAL);

        canvas = new DrawingCanvas(
            &diagramManager,
            panel,
            wxID_ANY,
            wxDefaultPosition,
            wxDefaultSize,
            wxHSCROLL | wxVSCROLL);

        sizer->Add(
            canvas,
            1,
            wxEXPAND | wxALL,
            5);

        slider = new wxSlider(
            panel,
            wxID_ANY,
            50,
            2,
            99);

        canvas->setSliderAddress(slider);

        sizer->Add(
            slider,
            0,
            wxEXPAND | wxALL,
            5);

        panel->SetSizer(sizer);

        graph = new BuildGraph(
            wxT(""),
            undir,
            circular,
            true,
            false);

        graph->setDrawingCanvas(canvas);

        GraphType type = undir;
        bool parallelEdges = false;

        graph->buildFromGML(
            filename,
            &type,
            &parallelEdges,
            false);

        canvas->setGA(
            graph->getGraphAttributes());

        canvas->RefreshCanvas(
            true,
            canvas->GetRect());
    }

    ~GraphExampleFrame() override
    {
        delete graph;
    }

private:
    wxSFDiagramManager diagramManager;

    DrawingCanvas* canvas;
    wxSlider* slider;
    BuildGraph* graph;
};

void DiscreteMathCalculatorFrame::OnCreateTypesOfUndirectedGraphsExamplesMenuItemSelected(
    wxCommandEvent& event)
{
    wxString defaultDirectory =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/graph-files/types-of-graphs-undir-undirw");

    wxDirDialog dirDialog(
        this,
        wxT("Single click the type of undirected graph you want to see."),
        defaultDirectory,
        wxDD_DIR_MUST_EXIST);

    if (dirDialog.ShowModal() != wxID_OK)
        return;

    wxString selectedDirectory = dirDialog.GetPath();
    wxString graphName = selectedDirectory.AfterLast('/');
	 wxString displayName = graphName;
	if (graphName == wxT("strongly-connected"))
	    displayName = wxT("Strongly Connected Components");
	if (graphName == wxT("complementary"))
	{
    wxString original =
        selectedDirectory +
        wxT("/original-8-vertices-15-edges.gml");

    wxString complement =
        selectedDirectory +
        wxT("/complementary-graph.gml");

    if (!wxFileExists(original) ||
        !wxFileExists(complement))
    {
        wxMessageBox(
            wxT("Complementary graph files not found."));
        return;
    }

    GraphExampleFrame* originalFrame =
        new GraphExampleFrame(
            this,
            wxT("Original Graph"),
            original,
            wxPoint(100, 100));

    GraphExampleFrame* complementFrame =
        new GraphExampleFrame(
            this,
            wxT("Complementary Graph"),
            complement,
            wxPoint(780, 100));

    originalFrame->Show(true);
    complementFrame->Show(true);

    return;
}


// Display both Parallel Edges examples in separate windows.
    if (graphName == wxT("parallel-edges"))
    {
        wxString graph1 =
            selectedDirectory + wxT("/parallel-edges-1.gml");

        wxString graph2 =
            selectedDirectory + wxT("/parallel-edges-2.gml");

        if (!wxFileExists(graph1) || !wxFileExists(graph2))
        {
            wxMessageBox(
                wxT("Parallel Edges graph files not found."));
            return;
        }

        GraphExampleFrame* frame1 =
            new GraphExampleFrame(
                this,
                wxT("Parallel Edges - Graph 1"),
                graph1,
                wxPoint(100, 100));

        GraphExampleFrame* frame2 =
            new GraphExampleFrame(
                this,
                wxT("Parallel Edges - Graph 2"),
                graph2,
                wxPoint(780, 100));

        frame1->Show(true);
        frame2->Show(true);

        return;
    }

    // Handle the Stable Marriage example separately.
    if (graphName == wxT("stable-marriage"))
    {
        wxString preferences =
            selectedDirectory + wxT("/marriage-preferences.png");

        wxString couples =
            selectedDirectory + wxT("/matched-couples.png");

        if (!wxFileExists(preferences) || !wxFileExists(couples))
        {
            wxMessageBox(
                wxT("Stable Marriage illustrations not found."));
            return;
        }

        if (!wxLaunchDefaultApplication(preferences))
        {
            wxMessageBox(
                wxT("Unable to open marriage preferences illustration."));
            return;
        }

        if (!wxLaunchDefaultApplication(couples))
        {
            wxMessageBox(
                wxT("Unable to open matched couples illustration."));
        }

        return;
    }

    // Locate a GML file in the selected directory.
    wxDir directory(selectedDirectory);

    if (!directory.IsOpened())
    {
        wxMessageBox(wxT("Unable to open example directory."));
        return;
    }

    wxString gmlName;

    if (!directory.GetFirst(&gmlName, wxT("*.gml"), wxDIR_FILES))
    {
        wxMessageBox(
            wxT("No GML file found in:\n") + selectedDirectory);
        return;
    }

    wxString filename =
        selectedDirectory + wxT("/") + gmlName;

    // Choose the graph layout.
    if (graphName == wxT("planar"))
    {
        graphLayout = orthogonal;
        graphLayoutRadioBox->SetSelection(2);
    }
    else if (graphName == wxT("complete-bipartite"))
    {
        graphLayout = hierarchical;
        graphLayoutRadioBox->SetSelection(0);
    }
    else if (graphName == wxT("biconnected") ||
             graphName == wxT("hypercube"))
    {
        graphLayout = forcedirected;
        graphLayoutRadioBox->SetSelection(1);
    }
    else
    {
        graphLayout = circular;
        graphLayoutRadioBox->SetSelection(3);
    }

    if (!saveGraphPreview(false))
        return;

    GraphType gtype;
    bool parallelEdges = false;

    if (myGraph != NULL)
        delete myGraph;

    myGraph = new BuildGraph(
        wxT(""),
        graphType,
        graphLayout,
        true,
        allowParallelEdges);

    myGraph->setDrawingCanvas(previewWindow);

    myGraph->buildFromGML(
        filename,
        &gtype,
        &parallelEdges,
        false);

    graphType = gtype;
    allowParallelEdges = parallelEdges;
    hideEdges = false;

    isDigraph =
        (graphType == digraph || graphType == digraphw);

    isWeighted =
        (graphType == undirw || graphType == digraphw);

    graphsGraphTypeRadioBox->SetSelection(
        isDigraph ? 1 : 0);

    graphsWeightedCheckBox->SetValue(isWeighted);
    graphsHideEdgesCheckBox->SetValue(false);
    graphsAllowParallelEdgesCheckBox->SetValue(parallelEdges);

    previewWindow->setGA(myGraph->getGraphAttributes());

    previewWindow->RefreshCanvas(
        true,
        previewWindow->GetRect());

    figureSaved = true;
}

void DiscreteMathCalculatorFrame::buildGraphExamplesMenu()
{
	GraphExamples = new wxMenu();

	GraphLayoutsExamples = new wxMenuItem(
		GraphExamples,
		ID_GRAPH_LAYOUTS_EXAMPLES_MENUITEM,
		_("Graph Layouts Examples..."),
		_("Demonstrations of Graph Layouts"),
		wxITEM_NORMAL);
	GraphExamples->Append(GraphLayoutsExamples);
	GraphExamples->Bind(
	    wxEVT_MENU,
	    &DiscreteMathCalculatorFrame::OnCreatGraphLayoutExamplesMenuItemSelected,
	    this,
	    ID_GRAPH_LAYOUTS_EXAMPLES_MENUITEM);
	TypesOfUndirectedGraphsExamples = new wxMenuItem(
		GraphExamples,
		ID_TYPES_OF_UNDIRECTED_GRAPHS_EXAMPLES_MENUITEM,
		_("Types of Undirected Graphs Examples..."),
		_("Types of Undirected Graphs Examples"),
		wxITEM_NORMAL);
	GraphExamples->Append(TypesOfUndirectedGraphsExamples);
	GraphExamples->Bind(
    	wxEVT_MENU,
    	&DiscreteMathCalculatorFrame::OnCreateTypesOfUndirectedGraphsExamplesMenuItemSelected,
    	this,
    	ID_TYPES_OF_UNDIRECTED_GRAPHS_EXAMPLES_MENUITEM);
	TypesOfDirectedGraphsExamples = new wxMenuItem(
		GraphExamples,
		ID_TYPES_OF_DIRECTED_GRAPHS_EXAMPLES_MENUITEM,
		_("Types of Directed Graphs Examples..."),
		_("Types of Directed Graphs Examples"),
		wxITEM_NORMAL);
	GraphExamples->Append(TypesOfDirectedGraphsExamples);
	
	GraphExamples->Bind(
	    wxEVT_MENU,
	    &DiscreteMathCalculatorFrame::OnCreateTypesOfDirectedGraphsExamplesMenuItemSelected,
	    this,
	    ID_TYPES_OF_DIRECTED_GRAPHS_EXAMPLES_MENUITEM);
	
	UndirectedGraphAlgorithmExamples = new wxMenuItem(
		GraphExamples,
		ID_UNDIRECTED_GRAPH_ALGORITHMS_EXAMPLES_MENUITEM,
		_("Undirected Graph Algorithm Examples..."),
		_("Undirected Graph Algorithm Examples"),
		wxITEM_NORMAL);
	GraphExamples->Append(UndirectedGraphAlgorithmExamples);
	GraphExamples->Bind(
    wxEVT_MENU,
    &DiscreteMathCalculatorFrame::OnCreateUndirectedGraphAlgorithmsExamplesMenuItemSelected,
    this,
    ID_UNDIRECTED_GRAPH_ALGORITHMS_EXAMPLES_MENUITEM);
	DirectedGraphAlgorithmExamples = new wxMenuItem(
		GraphExamples,
		ID_DIRECTED_GRAPH_ALGORITHMS_EXAMPLES_MENUITEM,
		_("Directed Graph Algorithm Examples..."),
		_("Directed Graph Algorithm Examples"),
		wxITEM_NORMAL);
	GraphExamples->Append(DirectedGraphAlgorithmExamples);
    GraphExamples->Bind(
        wxEVT_MENU,
        &DiscreteMathCalculatorFrame::OnCreateDirectedGraphAlgorithmsExamplesMenuItemSelected,
        this,
        ID_DIRECTED_GRAPH_ALGORITHMS_EXAMPLES_MENUITEM);

	discreteMathCalculatorMenuBar->Append(
		GraphExamples,
		_("&Examples"));
}

void DiscreteMathCalculatorFrame::buildEllipticCubicCurveExamplesMenu()
{
	ECCExamples = new wxMenu();

	ECCExamplesMenuItem = new wxMenuItem(
		ECCExamples,
		ID_ECC_SAMPLES_MENUITEM,
		_("Elliptic Cubic Curve Examples..."),
		_("Input curves"),
		wxITEM_NORMAL);

	ECCExamples->Append(ECCExamplesMenuItem);
	ECCExamples->Bind(
		wxEVT_MENU,
		&DiscreteMathCalculatorFrame::OnCreateEllipticCubicEncryptionExamplesMenuItemSelected,
		this,
		ID_ECC_SAMPLES_MENUITEM);
	discreteMathCalculatorMenuBar->Append(
		ECCExamples,
		_("&Examples"));
}

	void DiscreteMathCalculatorFrame::
	OnCreateEllipticCubicEncryptionExamplesMenuItemSelected(wxCommandEvent& event)
	{
		OnECCReadCurveParametersButtonPressed(event);
	}

void DiscreteMathCalculatorFrame::OndiscreteMathCalculatorNotebookPageChanged (wxNotebookEvent& event)
{
	wxSize size;
	int newPage = event.GetSelection();


#ifdef __APPLE__ || _WIN32
	int width, height;
	static int incr = 1;

	GetClientSize (&width, &height);
	GetClientSize (&width, &height);
	SetClientSize (width+incr, height);
	incr = -incr;
#endif

	page = newPage;
	buildMenuBarFor(newPage);
}

bool DiscreteMathCalculatorFrame::saveGraphPreview(bool exitApp)
{
	wxString graphTitle, message;
	ShapeList shapeList;
	int answer = wxYES;

	wxRect graphRect = (m_canvas.GetShapeCanvas() )->GetTotalBoundingBox();
	(m_canvas.GetShapeCanvas() )->GetShapesInside (graphRect, shapeList);

	if (shapeList.size() != 0 && !figureSaved)
	{
		if (exitApp)
			message.Printf (wxT ("Save changes to your graph before exiting the application?") );
		else
			message.Printf (wxT ("Save changes before building another graph?") );

		answer = wxMessageBox (message, wxT ("Graph figure"), wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

		if (answer == wxYES)
		{
			if (menuHandler != NULL)
				delete menuHandler;

			menuHandler = new MenuEventHandler (previewWindow,
			                                    ID_GRAPHS_PREVIEW_WINDOW_PRINT_MENUITEM,
			                                    ID_GRAPHS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM,
			                                    ID_GRAPHS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM,
			                                    ID_GRAPHS_PREVIEW_WINDOW_SAVE_AS_MENUITEM,
			                                    -1,
			                                    &figureSaved, graphType);

			if (!menuHandler->handleSaveAs(2))
				return false;
		}
	}

	return answer != wxCANCEL;
}

bool DiscreteMathCalculatorFrame::saveRelationPreview (bool exitApp)
{
	wxString graphTitle, message;
	ShapeList shapeList;
	int answer = wxYES;

	wxRect graphRect = (m_relationCanvas.GetShapeCanvas() )->GetTotalBoundingBox();
	(m_relationCanvas.GetShapeCanvas() )->GetShapesInside (graphRect, shapeList);

	if (shapeList.size() != 0 && !relationFigureSaved)
	{
		if (exitApp)
			message.Printf (wxT ("Save changes to your relation graph before exiting the application?") );
		else
			message.Printf (wxT ("Save changes before building another relation graph?") );

		answer = wxMessageBox (message, wxT ("Relation graph"), wxYES_NO | wxCANCEL | wxICON_EXCLAMATION, this);

		if (answer == wxYES)
		{
			if (relationMenuHandler != NULL)
				delete relationMenuHandler;

			relationMenuHandler = new MenuEventHandler (relationPartialOrderDrawing,
			      ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_MENUITEM,
			      ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM,
			      ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM,
			      ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_SAVE_AS_MENUITEM,
			      -1,
			      &relationFigureSaved, graphType);

			if (!relationMenuHandler->handleSaveAs(3) )
				return false;
		}
	}

	return answer != wxCANCEL;
}

void DiscreteMathCalculatorFrame::OngraphsEnterFromGRPHButtonClick (wxCommandEvent& event)
{
	ReadFromGRPH();
}

void DiscreteMathCalculatorFrame::OngraphsEnterFromTSPButtonClick (wxCommandEvent& event)
{
	if (!saveGraphPreview (false) )
		return;

	wxString filename = wxFileSelector (wxT ("Choose a TSP file specifying your graph."),
	                                    wxT (""),
	                                    wxT (""),
	                                    wxT (""),
	                                    wxT ("TSP file (*.tsp)|*.tsp"),
	                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (!filename.empty() )
	{
		// set all graph data to predefined values
		graphType = undirw;
		isWeighted = true;
		isDigraph = false;
		hideEdges = false;
		graphLayout = circular;
		graphsWeightedCheckBox->SetValue (true);
		graphsHideEdgesCheckBox->SetValue (false);
		graphsGraphTypeRadioBox->SetSelection (0);
		graphLayoutRadioBox->SetSelection (1);

		if (myGraph != NULL)
			delete myGraph;

		myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, false);

		myGraph->setDrawingCanvas (previewWindow);

		if (hideEdges)
			myGraph->buildFromTSP (filename, hideEdges);
		else
		{
			myGraph->buildFromTSP (filename, false);
			graphsHideEdgesCheckBox->SetValue (false);
			hideEdges = false;
		}

		previewWindow->setGA (myGraph->getGraphAttributes() );
		previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
		figureSaved = false;
	}
}

void DiscreteMathCalculatorFrame::OngraphsEnterFromGMLButtonClick(wxCommandEvent& event)
{
	GraphType gtype;
	bool parallelEdges;

	if (!saveGraphPreview (false) )
		return;

	wxString filename = wxFileSelector (wxT ("Choose a GML file specifying your graph."),
	                                    wxT (""),
	                                    wxT (""),
	                                    wxT (""),
	                                    wxT ("GML file (*.gml)|*.gml"),
	                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (!filename.empty() )
	{
		if (myGraph != NULL)
			delete myGraph;

		myGraph = new BuildGraph (wxT (""), graphType, graphLayout, true, allowParallelEdges);

		myGraph->setDrawingCanvas (previewWindow);
		myGraph->buildFromGML (filename, &gtype, &parallelEdges, hideEdges);

		if ( (gtype == digraph) || (gtype == digraphw) || (gtype == undir) )
		{
			graphsHideEdgesCheckBox->SetValue (false);
			hideEdges = false;
		}

		allowParallelEdges = parallelEdges;
		graphType = gtype;

		switch (graphType)
		{
		case undir:
			isDigraph = false;
			isWeighted = false;
			break;

		case undirw:
			isDigraph = false;
			isWeighted = true;
			break;

		case digraph:
			isDigraph = true;
			isWeighted = false;
			break;

		case digraphw:
			isDigraph = true;
			isWeighted = true;
			break;
		}

		switch (graphType)
		{
		case undir:
			graphsGraphTypeRadioBox->SetSelection (0);
			graphsWeightedCheckBox->SetValue (false);
			break;

		case undirw:
			graphsGraphTypeRadioBox->SetSelection (0);
			graphsWeightedCheckBox->SetValue (true);
			break;

		case digraph:
			graphsGraphTypeRadioBox->SetSelection (1);
			graphsWeightedCheckBox->SetValue (false);
			break;

		case digraphw:
			graphsGraphTypeRadioBox->SetSelection (1);
			graphsWeightedCheckBox->SetValue (true);
			break;
		}

		determineGraphType();

		if (parallelEdges)
			graphsAllowParallelEdgesCheckBox->SetValue (true);
		else
			graphsAllowParallelEdgesCheckBox->SetValue (false);

		previewWindow->setGA (myGraph->getGraphAttributes() );
		previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
		figureSaved = true;
	}
}

void DiscreteMathCalculatorFrame::OngraphsAddVertexButtonClick(wxCommandEvent& event)
{
	wxString vertexLabel = graphsAddEraseVertexTextCtrl->GetValue();

	if (vertexLabel.Length() == 0)
	{
		::report (wxT ("You have not specifed the vertex label."), wxT ("Note") );
		return;
	}

	if (myGraph == NULL)
	{
		determineGraphType();
		myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, allowParallelEdges);
		myGraph->setDrawingCanvas (previewWindow);
		previewWindow->setGA (myGraph->getGraphAttributes() );
		myGraph->eraseCanvas();
		myGraph->addVertexToEmptyGraph (vertexLabel);
		previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
		dirty = false;
		figureSaved = false;
		return;
	}
	else if (myGraph->addVertex (graphsAddEraseVertexTextCtrl->GetValue() ) )
		dirty = true;

	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
}

void DiscreteMathCalculatorFrame::OngraphsEraseVertexButtonClick(wxCommandEvent& event)
{
	wxString vertexLabel = graphsAddEraseVertexTextCtrl->GetValue();

	if (myGraph == NULL)
		return;

	if (vertexLabel.Length() == 0)
	{
		::report (wxT ("You have not specifed the vertex to erase."), wxT ("Note") );
		return;
	}

	if (myGraph->eraseVertex (vertexLabel) )
		dirty = true;
}

void DiscreteMathCalculatorFrame::OngraphsAddEdgeButtonClick(wxCommandEvent& event)
{
	long weight = -2147483647L;
	wxString strWeight;


	if (myGraph == NULL)
		return;

	determineGraphType();
	if (graphType == undirw || graphType == digraphw)
	{
		strWeight = graphEdgeWeightTextCtrl->GetValue();

		if (strWeight.length() == 0)
		{
			::report (wxT ("Your new edge has no weight."), wxT ("Note") );
			return;
		}

		if (!strWeight.ToLong (&weight) )
		{
			::report ( (wxT ("The edge weight is not in integer format.") ), wxT ("Note") );
			return;
		}
	}

	if (myGraph->addEdge (
	         graphsAddEraseEdgeSourceTextCtrl->GetValue(), graphsAddEraseEdgeDestTextCtrl->GetValue(), weight) )
		dirty = true;
}

void DiscreteMathCalculatorFrame::OngraphsEraseEdgeButtonClick(wxCommandEvent& event)
{
	if (myGraph == NULL)
		return;

	if (myGraph->eraseEdge (graphsAddEraseEdgeSourceTextCtrl->GetValue(), graphsAddEraseEdgeDestTextCtrl->GetValue() ) )
		dirty = true;
}

void DiscreteMathCalculatorFrame::OngraphsRedrawButtonClick (wxCommandEvent& event)
{
	GraphType currentGraphType;
	bool currentParallelEdges;

	if (myGraph == NULL)
		return;

	myGraph->getGraphType (currentGraphType, currentParallelEdges);

	determineGraphType();
	hideEdges = graphsHideEdgesCheckBox->GetValue();

	if (currentGraphType != graphType || currentParallelEdges != allowParallelEdges)
	{
		wxString message, caption;

		message.append (wxT ("Cannot redraw after changing a graph's\nproperties (type, weighted?, parallel edges?)") );
		caption.append (wxT ("Note") );

		wxMessageDialog dialog (this, message, caption, wxOK | wxICON_INFORMATION);
		dialog.ShowModal();

		return;
	}

	myGraph->redraw (graphType, graphLayout, previewWindow->GetScale(), hideEdges, this);
	// previewWindow->RefreshCanvas (true, previewWindow->GetRect());
}

void DiscreteMathCalculatorFrame::OncreateTSPMenuItemSelected (wxCommandEvent& event)
{
	wxString nText;
	long n;

	if (!saveGraphPreview (false) )
		return;

	nText = wxGetTextFromUser (wxT ("Enter n"), wxT ("Input text") );

	if (nText.length() == 0)
		return;

	if (!nText.ToLong (&n) )
	{
		::report (wxT ("n is not an integer."), wxT ("Note") );
		return;
	}

	if (n > 25)
	{
		wxString msg;

		msg.Printf (wxT ("For autogeneration of a TSP problem, n must be <= 25.") );
		::report (msg, wxT ("Note") );
		return;
	}

	graphType = undirw;
	isWeighted = true;
	isDigraph = false;
	graphLayout = circular;
	graphLayoutRadioBox->SetSelection (3);
	graphLayoutRadioBox->SetSelection (3);
	graphsGraphTypeRadioBox->SetSelection (0);
	graphsWeightedCheckBox->SetValue (true);
	graphsAllowParallelEdgesCheckBox->SetValue (false);
	determineGraphType();

	myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, false);
	myGraph->setDrawingCanvas (previewWindow);
	previewWindow->setGA (myGraph->getGraphAttributes() );
	myGraph->eraseCanvas();
	myGraph->drawTSP ( (int) n);
	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	dirty = false;
	figureSaved = false;
	myGraph->redraw (graphType, graphLayout, previewWindow->GetScale(), hideEdges, this);
}

void DiscreteMathCalculatorFrame::OncreateRandomGraphMenuItemSelected (wxCommandEvent& event)
{
	RandomGraphType rtype;
	RandomDigraphType rdtype;
	int m, n;

	if (!saveGraphPreview (false) )
		return;

	determineGraphType();
	RandomGraph enterRandomGraph (this, graphType, &rtype, &rdtype, &m, &n);

	if (enterRandomGraph.ShowModal() == wxID_CANCEL)
		return;

	// m_canvas.Clear();
	myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, allowParallelEdges);
	myGraph->setDrawingCanvas (previewWindow);
	previewWindow->setGA (myGraph->getGraphAttributes() );
	myGraph->eraseCanvas();

	if (isDigraph)
		myGraph->drawRandomDigraph (rdtype, m, n, graphLayoutRadioBox);
	else
	{

		// graphLayoutRadioBox->SetSelection(0);
		// graphLayout = hierarchical;
		if (rtype == RandomGraphType::planar)
		{
			graphLayoutRadioBox->SetSelection (2);
			graphLayout = orthogonal;
		}

		if (rtype == RandomGraphType::bipartite)
		{
			graphLayoutRadioBox->SetSelection (0);
			graphsWeightedCheckBox->SetValue (false);
			graphLayout = hierarchical;
		}

		myGraph->drawRandomGraph (rtype, m, n);
	}

	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	dirty = false;
	figureSaved = false;
}

void DiscreteMathCalculatorFrame::OndrawKnMenuItemSelected (wxCommandEvent& event)
{
	wxString nText;
	long n;

	if (!saveGraphPreview (false) )
		return;

	// m_canvas.Clear();

	nText = wxGetTextFromUser (wxT ("Enter n"), wxT ("Input text") );

	if (nText.length() == 0)
		return;

	if (!nText.ToLong (&n) )
	{
		::report (wxT ("n is not an integer."), wxT ("Note") );
		return;
	}

	// determineGraphType();
	graphType = undir;
	isWeighted = false;
	isDigraph = false;
	graphLayout = forcedirected;
	graphsGraphTypeRadioBox->SetSelection (0);
	graphsWeightedCheckBox->SetValue (false);
	graphLayoutRadioBox->SetSelection (1);
	graphsAllowParallelEdgesCheckBox->SetValue (false);

	myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, false);
	myGraph->setDrawingCanvas (previewWindow);
	previewWindow->setGA (myGraph->getGraphAttributes() );
	myGraph->eraseCanvas();
	myGraph->drawKn ( (int) n);
	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	dirty = false;
	figureSaved = false;
}

void DiscreteMathCalculatorFrame::OngraphsGraphTypeRadioBoxSelect (wxCommandEvent& event)
{
	int typeSelected = graphsGraphTypeRadioBox->GetSelection();

	constructCreateMenu(typeSelected);
}

void DiscreteMathCalculatorFrame::constructCreateMenu(int typeSelected)
{

	if (typeSelected == 0)
		isDigraph = false;
	else
		isDigraph = true;

	discreteMathCalculatorMenuBar->Remove (1);
	createGraphMenu = new wxMenu();
	discreteMathCalculatorMenuBar->Insert (1, createGraphMenu, wxT ("Create") );

	if (!isDigraph)
	{
		createRandomGraphMenuItem = new wxMenuItem (
		   createGraphMenu, ID_CREATE_RANDOM_GRAPH_MENUITEM, _ ("Random graph..."), wxEmptyString, wxITEM_NORMAL);
		createGraphMenu->Append (createRandomGraphMenuItem);
		drawKnMenuItem =
		   new wxMenuItem (createGraphMenu, ID_DRAW_KN_MENUITEM, _ ("Draw Kn..."), wxEmptyString, wxITEM_NORMAL);
		createGraphMenu->Append (drawKnMenuItem);
		graphsMakeGraphWithACliqueMenuItem = new wxMenuItem (
		   createGraphMenu, ID_GRAPHS_MAKE_CLIQUE, _ ("Make Graph with a Clique..."), wxEmptyString, wxITEM_NORMAL);
		createGraphMenu->Append (graphsMakeGraphWithACliqueMenuItem);
		createRandomGraphMenuItem =
		   new wxMenuItem (createGraphMenu, ID_HYPERCUBE_MENUITEM, _ ("Hypercube..."), wxEmptyString, wxITEM_NORMAL);
		createGraphMenu->Append (createRandomGraphMenuItem);
		stableMarriageMenuItem = new wxMenuItem (createGraphMenu,
		      ID_STABLE_MARRIAGE_MENUITEM,
		      _ ("Stable Marriage Problem..."),
		      wxEmptyString,
		      wxITEM_NORMAL);
		createGraphMenu->Append (stableMarriageMenuItem);
		createTSPMenuItem =
		   new wxMenuItem (createGraphMenu, ID_CREATE_TSP_MENUITEM, _ ("TSP..."), wxEmptyString, wxITEM_NORMAL);
		createGraphMenu->Append (createTSPMenuItem);
		createHamCycleMenuItem = new wxMenuItem (createGraphMenu,
		      ID_CREATE_HAMCYCLE_MENUITEM,
		      _ ("Test for Hamiltonian Cycle..."),
		      wxEmptyString,
		      wxITEM_NORMAL);
		createGraphMenu->Append (createHamCycleMenuItem);
		generalizedPetersonGraphMenuItem = new wxMenuItem (createGraphMenu,
		      ID_CREATE_GENERALIZED_PETERSON_GRAPH_MENUITEM,
		      _ ("Generalized Peterson Graph..."),
		      wxEmptyString,
		      wxITEM_NORMAL);
		createGraphMenu->Append (generalizedPetersonGraphMenuItem);
		createGraphSequenceGraphMenuItem = new wxMenuItem (createGraphMenu,
		      ID_GRAPHICAL_SEQUENCE_GRAPH_MENUITEM,
		      _ ("Test for Graphical Sequence..."),
		      wxEmptyString,
		      wxITEM_NORMAL);
		createGraphMenu->Append ( createGraphSequenceGraphMenuItem);
	}
	else
	{
		createRandomGraphMenuItem = new wxMenuItem (
		   createGraphMenu, ID_CREATE_RANDOM_GRAPH_MENUITEM, _ ("Random digraph..."), wxEmptyString, wxITEM_NORMAL);
		createGraphMenu->Append (createRandomGraphMenuItem);
		digraphicalSequenceGraphMenuItem = new wxMenuItem (
		   createGraphMenu, ID_DIGRAPHICAL_SEQUENCE_GRAPH_MENUITEM, _ ("Test for Digraph sequence..."), wxEmptyString, wxITEM_NORMAL);
		createGraphMenu->Append (digraphicalSequenceGraphMenuItem);
	}
}

/*void DiscreteMathCalculatorFrame::OngraphsWeightedCheckBoxClick (wxCommandEvent& event)
{
	isWeighted = graphsWeightedCheckBox->GetValue();
}*/

/*void DiscreteMathCalculatorFrame::OngraphsHideEdgesCheckBoxClick (wxCommandEvent& event)
{
	hideEdges = graphsHideEdgesCheckBox->GetValue();
}*/

void DiscreteMathCalculatorFrame::OngraphLayoutRadioBoxSelect (wxCommandEvent& event)
{
	int buttonSelected = event.GetSelection();

	switch (buttonSelected)
	{
	case 0:
		graphLayout = hierarchical;
		break;

	case 1:
		graphLayout = forcedirected;
		break;

	case 2:
		graphLayout = orthogonal;
		break;

	case 3:
		graphLayout = circular;
		break;
	}
}

void DiscreteMathCalculatorFrame::OngraphsAllowParallelEdgesCheckBoxClick (wxCommandEvent& event)
{
	allowParallelEdges = graphsAllowParallelEdgesCheckBox->GetValue();
}

void DiscreteMathCalculatorFrame::determineGraphType()
{
	isWeighted = graphsWeightedCheckBox->GetValue();
	if (isDigraph)
		if (isWeighted)
			graphType = digraphw;
		else
			graphType = digraph;
	else if (isWeighted)
		graphType = undirw;
	else
		graphType = undir;
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationMemoryStoreButtonClick (wxCommandEvent& event)
{
	memory = getResult();
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationMemoryAddButtonClick (wxCommandEvent& event)
{
	memory = memory + getResult();
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationMemoryRecallButtonClick (wxCommandEvent& event)
{
	setValue (memory, countingAndEnumerationResultTextCtrl);
}

void DiscreteMathCalculatorFrame::OncountingAndEnumerationMemoryClearButtonClick (wxCommandEvent& event)
{
	memory = 0;
}

void DiscreteMathCalculatorFrame::OnclearNumbersButtonClick (wxCommandEvent& event)
{
	aValueTextCtrl->Clear();
	bValueTextCtrl->Clear();
	nValueTextCtrl->Clear();
	resultValueTextCtrl->Clear();
}

void DiscreteMathCalculatorFrame::OnisAPrimeButtonClick (wxCommandEvent& event)
{
	primeTest();
}

void DiscreteMathCalculatorFrame::OnaddButtonClick (wxCommandEvent& event)
{
	add();
}

void DiscreteMathCalculatorFrame::Onadd_Mod_n_ButtonClick (wxCommandEvent& event)
{
	addModn();
}

void DiscreteMathCalculatorFrame::OnphiNButtonClick (wxCommandEvent& event)
{
	eulerPhi();
}

void DiscreteMathCalculatorFrame::Onto_a_ButtonClick (wxCommandEvent& event)
{
	toa();
}

void DiscreteMathCalculatorFrame::OnmemoryStore1ButtonClick (wxCommandEvent& event)
{
	memoryStore (1);
}

void DiscreteMathCalculatorFrame::OnmemoryStore2ButtonClick (wxCommandEvent& event)
{
	memoryStore (2);
}

void DiscreteMathCalculatorFrame::OnsubButtonClick (wxCommandEvent& event)
{
	sub();
}

void DiscreteMathCalculatorFrame::Onsub_mod_n_ButtonClick (wxCommandEvent& event)
{
	subModn();
}

void DiscreteMathCalculatorFrame::Onphi_a_b_ButtonClick (wxCommandEvent& event)
{
	eulerPhipq();
}

void DiscreteMathCalculatorFrame::Onto_b_ButtonClick (wxCommandEvent& event)
{
	tob();
}

void DiscreteMathCalculatorFrame::Onmemory1PlusButtonClick (wxCommandEvent& event)
{
	int pageNumber = discreteMathCalculatorNotebook->GetSelection();

	if (pageNumber == 0)
		memory1 = memory1 + result_bigint;
	else
		memory1 = memory1 + result_comb;
}

void DiscreteMathCalculatorFrame::Onmemory2PlusButtonClick (wxCommandEvent& event)
{
	int pageNumber = discreteMathCalculatorNotebook->GetSelection();

	if (pageNumber == 0)
		memory2 = memory2 + result_bigint;
	else
		memory2 = memory2 + result_comb;
}

void DiscreteMathCalculatorFrame::OnmulButtonClick (wxCommandEvent& event)
{
	mul();
}

void DiscreteMathCalculatorFrame::Onmul_mod_n_ButtonClick (wxCommandEvent& event)
{
	mulModn();
}

void DiscreteMathCalculatorFrame::Onx_bit_prime_ButtonClick (wxCommandEvent& event)
{
	generatePrime();
}

void DiscreteMathCalculatorFrame::Onto_n_ButtonClick (wxCommandEvent& event)
{
	ton();
}

void DiscreteMathCalculatorFrame::OnmemoryRecall1ButtonClick (wxCommandEvent& event)
{
	memoryRecall (1);
}

void DiscreteMathCalculatorFrame::OnmemoryRecall2ButtonClick (wxCommandEvent& event)
{
	memoryRecall (2);
}

void DiscreteMathCalculatorFrame::OndivButtonClick (wxCommandEvent& event)
{
	div();
}

void DiscreteMathCalculatorFrame::OnmodButtonClick (wxCommandEvent& event)
{
	try
	{
		get_ab();
		result_bigint = a % b;
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::OngcdButtonClick (wxCommandEvent& event)
{
	gcd();
}

void DiscreteMathCalculatorFrame::OnmemoryClear1ButtonClick (wxCommandEvent& event)
{
	memory1 = 0;
}

void DiscreteMathCalculatorFrame::OnmemoryClear2ButtonClick (wxCommandEvent& event)
{
	memory2 = 0;
}

void DiscreteMathCalculatorFrame::OnpowerButtonClick (wxCommandEvent& event)
{
	intPower();
}

void DiscreteMathCalculatorFrame::Onpower_mod_n_ButtonClick (wxCommandEvent& event)
{
	intPowerModn();
}

void DiscreteMathCalculatorFrame::OnleastCommonMultipleButtonClick (wxCommandEvent& event)
{
	try
	{
		get_ab();
		result_bigint = (a * b) / GCD (a, b);
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::Oninverse_a_mod_n_ButtonClick (wxCommandEvent& event)
{
	inverseamodn();
}

void DiscreteMathCalculatorFrame::OncoprimeButtonClick (wxCommandEvent& event)
{
	findCoprime();
}

void DiscreteMathCalculatorFrame::OnresultMagnitudeButtonClick (wxCommandEvent& event)
{
	RR fltPointApprox;
	ostringstream fltPointStr;

	fltPointStr << to_RR (getResult() );

	wxString asymApprox (wxString (fltPointStr.str().c_str(), wxConvUTF8) );

	ResultMagnitude resultMagnitude (this);
	resultMagnitude.writeApproximation (asymApprox);

	resultMagnitude.ShowModal();
}

void DiscreteMathCalculatorFrame::OnresultMagnitude2ButtonClick (wxCommandEvent& event)
{
	wxString resultValueN, resultValueK;
	ZZ value;
	RR fltPointApprox;
	ostringstream fltPointStr;
	wxString approxStrN, approxStrK;

	ResultMagnitude2 resultMagnitude2 (this, page);

	if (page == 3)
	{
		resultValueN = subsetsNumberofSubsetsTextCtrl->GetValue();

		if (resultValueN.length() != 0)
		{
			conv (value, (const char*) resultValueN.c_str() );
			fltPointStr << to_RR (value);
			approxStrN = wxString (fltPointStr.str().c_str(), wxConvUTF8);
		}

		resultValueK = subsetsNumberOfKElementSubsetsTextCtrl->GetValue();

		if (resultValueK.length() != 0)
		{
			conv (value, (const char*) resultValueK.c_str() );
			fltPointStr.str ("");
			fltPointStr << to_RR (value);
			approxStrK = wxString (fltPointStr.str().c_str(), wxConvUTF8);
		}
	}
	else
	{
		resultValueN = partitionsBellNumberTextCtrl->GetValue();

		if (resultValueN.length() != 0)
		{
			conv (value, (const char*) resultValueN.c_str() );
			fltPointStr << to_RR (value);
			approxStrN = wxString (fltPointStr.str().c_str(), wxConvUTF8);
		}

		resultValueK = partitionsStirlingNumberTextCtrl->GetValue();

		if (resultValueK.length() != 0)
		{
			conv (value, (const char*) resultValueK.c_str() );
			fltPointStr.str ("");
			fltPointStr << to_RR (value);
			approxStrK = wxString (fltPointStr.str().c_str(), wxConvUTF8);
		}
	}

	if ( (approxStrN.length() != 0) || (approxStrK.length() != 0) )
		resultMagnitude2.writeApproximations (approxStrN, approxStrK);
	else
		throw ntlException ("There are no values available.");

	resultMagnitude2.ShowModal();
}

void DiscreteMathCalculatorFrame::OnEllipticCurveECDLPMenuItemSelected (wxCommandEvent& event)
{

}

void DiscreteMathCalculatorFrame::OnspellResultButtonClick (wxCommandEvent& event)
{
	ostringstream numstr;
	int length;
	bool floatingPoint = false;

	numstr << result_bigint;
	string numberText = numstr.str();
	length = numberText.length();

	if (length > 65)
	{
		wxString message, caption;

		floatingPoint = true;
		message.append (wxT ("The integer exceeds 65 digits.\n") );
		message.append (wxT ("We will spell a floating point approximation.") );
		caption.append (wxT ("Note") );

		wxMessageDialog dialog (this, message, caption, wxOK | wxICON_ERROR);
		dialog.ShowModal();
	}

	SpellNumberDialog* spellNumber = new SpellNumberDialog (this);

	if (floatingPoint)
	{
		RR fltPointApprox;
		ostringstream fltPointStr;

		RR::SetPrecision (512);
		RR::SetOutputPrecision (50);

		fltPointApprox = to_RR (result_bigint);
		fltPointStr << fltPointApprox;
		numberText = fltPointStr.str();
	}

	spellNumber->writeSpelledValue ( (char*) numberText.c_str() );
	spellNumber->ShowModal();
}

void DiscreteMathCalculatorFrame::add()
{
	try
	{
		get_ab();
		result_bigint = a + b;
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::addModn()
{
	try
	{
		get_ab();
		get_n();
		result_bigint = AddMod (a, b, n);
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::sub()
{
	try
	{
		get_ab();
		result_bigint = a - b;
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::subModn()
{
	try
	{
		get_ab();
		get_n();
		result_bigint = SubMod (a, b, n);
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::mul()
{
	try
	{
		get_ab();
		result_bigint = a * b;
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::mulModn()
{
	try
	{
		get_ab();
		get_n();
		result_bigint = MulMod (a, b, n);
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::div()
{
	try
	{
		get_ab();
		result_bigint = a / b;
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::intPower()
{
	try
	{
		get_ab();
		result_bigint = power (a, to_long (b) );
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::intPowerModn()
{
	try
	{
		get_ab();
		get_n();

		if (n <= 0)
			throw ntlException ("The modulus n must be greater than zero.");

		if (b < 0)
			throw ntlException ("The exponent b must not be negative.");

		result_bigint = PowerMod (a, b, n);
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::primeTest()
{
	try
	{
		get_a();

		if (ProbPrime (a) != 0)
			resultValueTextCtrl->ChangeValue (wxT ("Prime") );
		else
			resultValueTextCtrl->ChangeValue (wxT ("Not prime") );
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::eulerPhi()
{
	try
	{
		get_n();
		result_bigint = phi (n);
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::eulerPhipq()
{
	try
	{
		get_ab();
		result_bigint = (a - 1) * (b - 1);
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::generatePrime()
{
	wxString xValue;
	ZZ numBits;

	try
	{
		xValue = xTextCtrl->GetValue();
		conv (numBits, (const char*) xValue.c_str() );
		result_bigint = RandomPrime_ZZ (to_long (numBits) );
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::gcd()
{
	try
	{
		get_ab();
		result_bigint = GCD (a, b);
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::inverseamodn()
{
	try
	{
		get_a();
		get_n();
		if (a >= n)
		{
			::report(wxT("a must be less than n."), wxT("Note"));
			return;
		}
		result_bigint = InvMod (a, n);
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::findCoprime()
{
	long numberBitsIn_a;
	ZZ e;

	try
	{
		get_a();
		numberBitsIn_a = NumBits (a);

		do
		{
			// generate a random integer with 3/4*numberBitsIn_a
			e = RandomBits_ZZ (long (.75 * numberBitsIn_a) );
			// if InvModStatus(d, e, t) is 0, then d*e = 1 (mod t), or d = e^(-1) (mod t)
		}
		while (GCD (e, a) != 1);

		result_bigint = e;
		setValue (result_bigint, resultValueTextCtrl);
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::toa()
{
	try
	{
		result_bigint = getResult();
		a = result_bigint;
		aValueTextCtrl->ChangeValue (resultValueTextCtrl->GetValue() );
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::tob()
{
	try
	{
		result_bigint = getResult();
		b = result_bigint;
		bValueTextCtrl->ChangeValue (resultValueTextCtrl->GetValue() );
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::ton()
{
	try
	{
		result_bigint = getResult();
		n = result_bigint;
		nValueTextCtrl->ChangeValue (resultValueTextCtrl->GetValue() );
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::memoryStore (int buttonNumber)
{
	int pageNumber = discreteMathCalculatorNotebook->GetSelection();

	if (pageNumber == 0)
	{
		if (buttonNumber == 1)
			memory1 = result_bigint;
		else
			memory2 = result_bigint;
	}
	else
	{
		if (buttonNumber == 1)
			memory1 = result_comb;
		else
			memory2 = result_comb;
	}
}

void DiscreteMathCalculatorFrame::memoryRecall (int buttonNumber)
{
	int pageNumber = discreteMathCalculatorNotebook->GetSelection();

	try
	{
		if (pageNumber == 0)
		{
			result_bigint = (buttonNumber == 1 ? memory1 : memory2);
			setValue (result_bigint, resultValueTextCtrl);
		}
		else
		{
			result_comb = (buttonNumber == 1 ? memory1 : memory2);
			setValue (result_comb, countingAndEnumerationResultTextCtrl);
		}
	}

	catch (const ntlException& ntle)
	{
		throw ntlException (ntle);
	}
}

void DiscreteMathCalculatorFrame::get_ab()
{
	aValue = aValueTextCtrl->GetValue();

	if (aValue.length() == 0)
		throw ntlException ("a has no value.");

	conv (a, (const char*) aValue.c_str() );

	bValue = bValueTextCtrl->GetValue();

	if (bValue.length() == 0)
		throw ntlException ("b has no value.");

	conv (b, (const char*) bValue.c_str() );
}

void DiscreteMathCalculatorFrame::get_n()
{
	nValue = nValueTextCtrl->GetValue();

	if (nValue.length() == 0)
		throw ntlException ("n has no value");

	conv (n, (const char*) nValue.c_str() );
}

void DiscreteMathCalculatorFrame::get_a()
{
	aValue = aValueTextCtrl->GetValue();

	if (aValue.length() == 0)
		throw ntlException ("a has no value.");

	conv (a, (const char*) aValue.c_str() );
}

void DiscreteMathCalculatorFrame::OnhypercubeMenuItemSelected (wxCommandEvent& event)
{
	wxString nText;
	long n;

	if (!saveGraphPreview (false) )
		return;

	// m_canvas.Clear();

	nText = wxGetTextFromUser (wxT ("Enter n"), wxT ("Input text") );

	if (nText.length() == 0)
		return;

	nText.ToLong (&n);

	if (n > 6)
	{
		::report (wxT ("The dimension of the hypercube cannot exceed 6."), wxT ("Note") );
		return;
	}

	graphsGraphTypeRadioBox->SetSelection (0);
	graphLayoutRadioBox->SetSelection (1);

	isDigraph = false;
	graphType = undir;
	graphLayout = forcedirected;

	myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, false);
	myGraph->setDrawingCanvas (previewWindow);
	previewWindow->setGA (myGraph->getGraphAttributes() );
	myGraph->eraseCanvas();
	myGraph->drawHypergraph ( (int) n);
	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	dirty = false;
	figureSaved = false;
}

void DiscreteMathCalculatorFrame::OnstableMarriageMenuItemSelected (wxCommandEvent& event)
{
	DisplayAlgorithmResults *pairingGraph;

	StableMarriageDialog stableMarriageDialog (this, &pairingGraph);

	saveGraphPreview (false);

	if (stableMarriageDialog.ShowModal() == wxID_CANCEL)
		return;
	else
	{
		pairingGraph->Show ();
		wxTheApp->SetTopWindow (pairingGraph);
	}
}

void DiscreteMathCalculatorFrame::getHelp()
{
#ifdef __APPLE__
    wxFileName bundleHelp(
        wxStandardPaths::Get().GetResourcesDir(),
        wxEmptyString
    );

    bundleHelp.AppendDir("help");
    bundleHelp.SetFullName("DiscreteMathCalculator.html");

    if (bundleHelp.FileExists())
    {
        wxString url =
            wxFileName::FileNameToURL(bundleHelp.GetFullPath());

        if (!wxLaunchDefaultBrowser(url))
        {
            report(
                wxT("Failed to open the Help page:\n") + url,
                wxT("Help Error")
            );
        }

        return;
    }
#endif
	   wxFileName executable(
      wxStandardPaths::Get().GetExecutablePath()
    );

    wxString directory = executable.GetPath();

    for (int i = 0; i < 10; ++i)
    {
        // Construct the Help filename using directory components.
        wxFileName helpFile(directory, wxEmptyString);

        helpFile.AppendDir(wxT("usr"));
        helpFile.AppendDir(wxT("help"));
        helpFile.SetFullName(
            wxT("DiscreteMathCalculator.html")
        );

        if (helpFile.FileExists())
        {
            wxString url =
                wxFileName::FileNameToURL(
                    helpFile.GetFullPath()
                );

            if (!wxLaunchDefaultBrowser(url))
            {
                report(
                    wxT("Failed to open the Help page:\n") + url,
                    wxT("Help Error")
                );
            }

            return;
        }

        // Move up one directory.
        wxFileName parent(directory, wxEmptyString);

        if (parent.GetDirCount() == 0)
            break;

        parent.RemoveLastDir();

        wxString nextDirectory = parent.GetPath();

        if (nextDirectory == directory)
            break;

        directory = nextDirectory;
    }

    report(
        wxT("Cannot locate the Help file."),
        wxT("Help Error")
    );
}

void DiscreteMathCalculatorFrame::OndiscreteMathCalcuatorHelpMenuItemSelected (wxCommandEvent& event)
{
	getHelp();
}

void DiscreteMathCalculatorFrame::OngraphsEnterFromDialogButtonClick (wxCommandEvent& event)
{
	if (!saveGraphPreview (false) )
		return;

	determineGraphType();

	myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, allowParallelEdges);
	myGraph->setDrawingCanvas (previewWindow);

	EnterGraphDialog enterGraph (this, 0, graphType);

	if (enterGraph.ShowModal() == wxID_OK)
	{
		myGraph->setDrawingCanvas (previewWindow);
		previewWindow->setGA (myGraph->getGraphAttributes() );
		myGraph->drawMyDialogGraph (enterGraph.getGraphParameters() );
		// make the darn thing show after returning from the dialog
		previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
		figureSaved = false;
	}
	else
	{
		delete myGraph;
		myGraph = NULL;
	}
}

void DiscreteMathCalculatorFrame::OncreateHamCycleMenuItemSelected (wxCommandEvent& event)
{
	wxString nText;
	int numVertices, numEdges;

	if (!saveGraphPreview (false) )
		return;

	HamCycle enterHamCycleData (this, &numVertices, &numEdges);

	if (enterHamCycleData.ShowModal() == wxID_CANCEL)
		return;

	if (myGraph != NULL)
		delete myGraph;

	graphLayoutRadioBox->SetSelection (1);
	graphLayout = circular;
	graphType = undir;
	isWeighted = false;
	isDigraph = false;
	graphsGraphTypeRadioBox->SetSelection (0);
	graphsWeightedCheckBox->SetValue (false);
	graphsAllowParallelEdgesCheckBox->SetValue (false);

	myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, false);
	myGraph->setDrawingCanvas (previewWindow);
	previewWindow->setGA (myGraph->getGraphAttributes() );
	myGraph->eraseCanvas();
	myGraph->drawRandomGraph (triconnected, numVertices, numEdges);
	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	dirty = false;
	figureSaved = false;
}

void DiscreteMathCalculatorFrame::OngeneralizedPetersonGraphMenuItemSelected (wxCommandEvent& event)
{
	wxString nText;
	int n, k;

	if (!saveGraphPreview (false) )
		return;

	Peterson enterGeneralizedPetersonGraph (this, &n, &k);

	if (enterGeneralizedPetersonGraph.ShowModal() == wxID_CANCEL)
		return;

	if (myGraph != NULL)
		delete myGraph;

	graphLayoutRadioBox->SetSelection (1);
	graphLayout = forcedirected;
	graphType = undir;
	isWeighted = false;
	isDigraph = false;
	graphsGraphTypeRadioBox->SetSelection (0);
	graphsWeightedCheckBox->SetValue (false);
	graphsAllowParallelEdgesCheckBox->SetValue (false);

	myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, false);
	myGraph->setDrawingCanvas (previewWindow);
	previewWindow->setGA (myGraph->getGraphAttributes() );
	myGraph->eraseCanvas();
	myGraph->drawGeneralizedPetersonGraph (n, k);
	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	dirty = false;
	figureSaved = false;
}

void DiscreteMathCalculatorFrame::OngraphsMakeGraphWithACliqueMenuItemSelected (wxCommandEvent& event)
{
	wxString nText;
	long cliqueSize, numberOfVertices, numberOfEdges;

	if (!saveGraphPreview (false) )
		return;

	// m_canvas.Clear();

	RandomClique cliqueDialog (this, &cliqueSize, &numberOfVertices, &numberOfEdges);

	if (cliqueDialog.ShowModal() == wxID_CANCEL)
		return;

	graphLayoutRadioBox->SetSelection (3);
	graphType = undir;
	graphLayout = forcedirected;
	isWeighted = false;
	isDigraph = false;
	graphsGraphTypeRadioBox->SetSelection (0);
	graphsWeightedCheckBox->SetValue (false);
	graphsAllowParallelEdgesCheckBox->SetValue (false);

	myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, false);
	myGraph->setDrawingCanvas (previewWindow);
	previewWindow->setGA (myGraph->getGraphAttributes() );
	myGraph->eraseCanvas();
	myGraph->drawGraphWithClique ( (int) cliqueSize, (int) numberOfVertices, (int) numberOfEdges);
	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	dirty = false;
	figureSaved = false;
}

void DiscreteMathCalculatorFrame::OnClearGraphButtonClick (wxCommandEvent& event)
{
	if (myGraph == NULL)
		return;

	graphsGraphTypeRadioBox->SetSelection(0);
	graphsWeightedCheckBox->SetValue(false);
	graphsAllowParallelEdgesCheckBox->SetValue(false);
	graphsHideEdgesCheckBox->SetValue(false);
	graphLayoutRadioBox->SetSelection(0);
	graphsAddEraseEdgeSourceTextCtrl->SetValue(wxT(""));
	graphsAddEraseEdgeDestTextCtrl->SetValue(wxT(""));
	graphsAddEraseVertexTextCtrl->SetValue(wxT(""));
	graphsAddEraseEdgeDestTextCtrl->SetValue(wxT(""));
	graphEdgeWeightTextCtrl->SetValue(wxT(""));
	allowParallelEdges = false;
	isDigraph = false;
	isWeighted = false;
	hideEdges = false;
	graphLayout = hierarchical;
	graphType = undir;

	m_canvas.Clear();
	m_canvas.UpdateAll();
	myGraph->eraseCanvas (true);
	previewWindow->Refresh(true);
	constructCreateMenu(0);
}

void DiscreteMathCalculatorFrame::OntransposeGraphButtonClick (wxCommandEvent& event)
{
	if (myGraph == NULL)
	{
		::report (wxT ("You have not created a digraph yet."), wxT ("Note") );
		return;
	}

	determineGraphType();

	if (graphType != digraph && graphType != digraphw)
	{
		::report (wxT ("The transpose only applies to a digraph."), wxT ("Note") );
		return;
	}

	myGraph->drawTransposeGraph (this);
}

void DiscreteMathCalculatorFrame::OncomplementaryGraphButtonClick (wxCommandEvent& event)
{
	if (myGraph == NULL)
	{
		::report (wxT ("You have not created an undirected graph yet."), wxT ("Note") );
		return;
	}

	determineGraphType();

	if (graphType != undir)
	{
		::report (wxT ("The graph must be undirected and not weighted."), wxT ("Note") );
		return;
	}

	myGraph->drawComplementaryGraph (this);
}

void DiscreteMathCalculatorFrame::OnAnalyzeGraphButtonClick (wxCommandEvent& event)
{
	if (myGraph == NULL)
	{
		::report (wxT ("You have not created a graph yet."), wxT ("Note") );
		return;
	}

	determineGraphType();

	ogdf::NodeArray<int> component (* (myGraph->getGraph() ) );

	AnalyzeGraph::AnalyzeGraphData gdata;

	gdata.gtype = graphType;
	gdata.numVertices = myGraph->getGraph()->numberOfNodes();
	gdata.numEdges = myGraph->getGraph()->numberOfEdges();
	gdata.numSelfLoops = myGraph->numberOfSelfLoops();
	gdata.parallelEdges = myGraph->hasParallelEdges();
	gdata.glayout = myGraph->getGraphLayout();
	gdata.planar = ogdf::isPlanar (* (myGraph->getGraph() ) );
	gdata.acyclic = ( (graphType == digraph) || (graphType == digraphw) ) ?
	                ogdf::isAcyclic (* (myGraph->getGraph() ) ) :
	                ogdf::isAcyclicUndirected (* (myGraph->getGraph() ) );
	gdata.numComponents = ( (graphType == digraph) || (graphType == digraphw) ) ?
	                      ogdf::strongComponents (* (myGraph->getGraph() ), component) :
	                      ogdf::connectedComponents (* (myGraph->getGraph() ), component);

	AnalyzeGraph analyzeGraph (this, gdata);

	if (analyzeGraph.ShowModal() == wxID_CANCEL)
		return;
}

void DiscreteMathCalculatorFrame::OnSubgraphIsomorphismButtonPress (wxCommandEvent& event)
{
	wxFrame* canvas;

	canvas = new IsomorphismDrawingCanvas (this, true);

	canvas->Show();
	wxTheApp->SetTopWindow (canvas);
}

void DiscreteMathCalculatorFrame::OnGraphIsomorphismButtonPress (wxCommandEvent& event)
{
	wxFrame* canvas;
	GraphLayout graphLayout;

	graphLayout = (GraphLayout)graphsGraphTypeRadioBox->GetSelection();

	canvas = new IsomorphismDrawingCanvas (this, false, graphLayout);

	canvas->Show();
	wxTheApp->SetTopWindow (canvas);
}

void DiscreteMathCalculatorFrame::OnRelationEnterButtonPress (wxCommandEvent& event)
{
	if (!saveRelationPreview (false) )
		return;

	if (myRelationDigraph != NULL)
		delete myRelationDigraph;

	m_relationCanvas.Clear();

	myRelationDigraph = new BuildGraph (wxT (""), digraph, hierarchical, true, false);
	graphType = digraph;
	myRelationDigraph->setDrawingCanvas (relationPartialOrderDrawing);

	RelationInput relationInputDialog (this, "inputRelation.png");

	if (relationInputDialog.ShowModal() == wxID_OK)
	{
		myRelationDigraph->setDrawingCanvas (relationPartialOrderDrawing);
		relationPartialOrderDrawing->setGA (myRelationDigraph->getGraphAttributes() );
		myRelationDigraph->drawMyRelationGraph (relationInputDialog.getGraphParameters(), false, this);
		// make the darn thing show after returning from the dialog
		relationPartialOrderDrawing->RefreshCanvas (true, relationPartialOrderDrawing->GetRect() );
		relationFigureSaved = false;
	}
	else
	{
		delete myRelationDigraph;
		myRelationDigraph = NULL;
	}
}

void DiscreteMathCalculatorFrame::OnRelationReflexiveClosureButtonPress (wxCommandEvent& event)
{
	ClosureDisplay *reflexiveClosureDisplay;

	if (myRelationDigraph == NULL)
		return;
	else
		reflexiveClosureDisplay = new
		ClosureDisplay (myRelationDigraph->getGraphAttributes(),
		                myRelationDigraph->getGraph(),
		                ClosureDisplay::REFLEXIVE);

	reflexiveClosureDisplay->drawRequestedClosure();
}

void DiscreteMathCalculatorFrame::OnRelationSymmetricClosureButtonPress (wxCommandEvent& event)
{
	ClosureDisplay *symmetricClosureDisplay;

	if (myRelationDigraph == NULL)
		return;
	else
		symmetricClosureDisplay = new
		ClosureDisplay (myRelationDigraph->getGraphAttributes(),
		                myRelationDigraph->getGraph(),
		                ClosureDisplay::SYMMETRIC);

	symmetricClosureDisplay->drawRequestedClosure();
}

void DiscreteMathCalculatorFrame::OnRelationTransitiveClosureButtonPress (wxCommandEvent& event)
{
	ClosureDisplay *transitiveClosureDisplay;

	if (myRelationDigraph == NULL)
		return;
	else
		transitiveClosureDisplay = new
		ClosureDisplay (myRelationDigraph->getGraphAttributes(),
		                myRelationDigraph->getGraph(),
		                ClosureDisplay::TRANSITIVE);

	transitiveClosureDisplay->drawRequestedClosure();
}

void DiscreteMathCalculatorFrame::OnRelationPropertiesButtonPress(
    wxCommandEvent& event)
{
    if (myRelationDigraph == NULL)
    {
        wxMessageBox(
            "Please enter a relation first.",
            "Relation Properties",
            wxOK | wxICON_INFORMATION,
            this);
        return;
    }

    ogdf::Graph* G = myRelationDigraph->getGraph();
    ogdf::GraphAttributes* GA =
        myRelationDigraph->getGraphAttributes();

    std::vector<ogdf::node> vertices;
    std::map<ogdf::node, size_t> index;

    // Exclude auxiliary graph nodes.
    for (ogdf::node v : G->nodes)
    {
        if (GA->shape(v) == ogdf::Shape::Ellipse)
            continue;

        index[v] = vertices.size();
        vertices.push_back(v);
    }

    const size_t n = vertices.size();

    std::vector<std::vector<bool>> R(
        n, std::vector<bool>(n, false));

    // Build the adjacency matrix.
    for (ogdf::edge e : G->edges)
    {
        auto u = index.find(e->source());
        auto v = index.find(e->target());

        if (u != index.end() && v != index.end())
            R[u->second][v->second] = true;
    }
	// Hasse diagrams represent partial orders using covering edges.
	// Reconstruct the full relation before testing its properties.
	if (graphType == undir)
	{
		// Reflexive closure.
		for (size_t i = 0; i < n; ++i)
			R[i][i] = true;

		// Transitive closure (Warshall's algorithm).
		for (size_t k = 0; k < n; ++k)
		{
			for (size_t i = 0; i < n; ++i)
			{
					for (size_t j = 0; j < n; ++j)
					{
						if (R[i][k] && R[k][j])
							R[i][j] = true;
					}
			}
		}
	}
    bool reflexive = true;
    bool irreflexive = true;
    bool symmetric = true;
    bool antisymmetric = true;
    bool asymmetric = true;
    bool transitive = true;
    bool total = true;

    for (size_t i = 0; i < n; ++i)
    {
        if (!R[i][i])
            reflexive = false;

        if (R[i][i])
        {
            irreflexive = false;
            asymmetric = false;
        }

        for (size_t j = 0; j < n; ++j)
        {
            if (R[i][j] && !R[j][i])
                symmetric = false;

            if (i != j && R[i][j] && R[j][i])
            {
                antisymmetric = false;
                asymmetric = false;
            }

            if (i != j && !R[i][j] && !R[j][i])
                total = false;

            for (size_t k = 0; k < n; ++k)
            {
                if (R[i][j] && R[j][k] && !R[i][k])
                    transitive = false;
            }
        }
    }

    bool equivalence =
        reflexive && symmetric && transitive;

    bool partialOrder =
        reflexive && antisymmetric && transitive;

    bool totalOrder =
        partialOrder && total;

    auto yesNo = [](bool value) -> wxString
    {
        return value ? wxT("Yes") : wxT("No");
    };

    wxString report;

    report << "RELATION PROPERTIES\n\n";
    report << "Number of elements: "
           << static_cast<unsigned long>(n) << "\n\n";

    report << "Reflexive:             " << yesNo(reflexive) << "\n";
    report << "Irreflexive:           " << yesNo(irreflexive) << "\n";
    report << "Symmetric:             " << yesNo(symmetric) << "\n";
    report << "Antisymmetric:         " << yesNo(antisymmetric) << "\n";
    report << "Asymmetric:            " << yesNo(asymmetric) << "\n";
    report << "Transitive:            " << yesNo(transitive) << "\n\n";

    report << "Equivalence Relation:  " << yesNo(equivalence) << "\n";
    report << "Partial Order:         " << yesNo(partialOrder) << "\n";
    report << "Total Order:           " << yesNo(totalOrder);

	wxDialog dialog(
		this,
		wxID_ANY,
		"Relation Properties",
		wxDefaultPosition,
		wxSize(450, 400),
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
	);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxTextCtrl* results = new wxTextCtrl(
		&dialog,
		wxID_ANY,
		report,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP
	);

	// Use a fixed-width font so Yes/No values align.
	wxFont font = results->GetFont();
	font.SetFamily(wxFONTFAMILY_TELETYPE);
	results->SetFont(font);

	sizer->Add(results, 1, wxEXPAND | wxALL, 10);

	wxButton* closeButton = new wxButton(
		&dialog,
		wxID_OK,
		"Close"
	);

	sizer->Add(closeButton, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

	dialog.SetSizer(sizer);
	dialog.Centre();
	dialog.ShowModal();
}

void DiscreteMathCalculatorFrame::OnPartialOrderInputButtonPress (wxCommandEvent& event)
{
	PartialOrderDialog *partialOrderDialog;
	vector<RelationGraphEdge> relationEdge;
	// char src[10], dest[10];

	if (!saveRelationPreview (false) )
		return;

	if (myRelationDigraph != NULL)
		delete myRelationDigraph;

	m_relationCanvas.Clear();

	myRelationDigraph = new BuildGraph (wxT (""), undir, hierarchical, true, false);
	graphType = undir;
	myRelationDigraph->setDrawingCanvas (relationPartialOrderDrawing);

	partialOrderDialog = new
	PartialOrderDialog (this, "partialorder.png", &relationEdge);

	// RelationGraphEdge k;
	if (partialOrderDialog->ShowModal() == wxID_OK)
	{
		/*for(int i = 0;i < relationEdge.size();i++)
		{
			k = relationEdge[i];
			strcpy(src, k.edgeSource.mb_str());
			strcpy(dest, k.edgeDest.mb_str());
		}*/

		myRelationDigraph->setDrawingCanvas (relationPartialOrderDrawing);
		relationPartialOrderDrawing->setGA (myRelationDigraph->getGraphAttributes() );
		myRelationDigraph->drawMyRelationGraph (relationEdge, false, this, partialOrderDialog->getAllVertices());
		// make the darn thing show after returning from the dialog
		// relationPartialOrderDrawing->RefreshCanvas (true, relationPartialOrderDrawing->GetRect() );
		relationFigureSaved = false;
	}
	else
	{
		delete myRelationDigraph;
		myRelationDigraph = NULL;
	}
}

void DiscreteMathCalculatorFrame::OnShapeFrameworkDrawingCanvasSliderChange (wxScrollEvent& event)
{
	relationPartialOrderDrawing->SetScale (double (relationPartialOrderSlider->GetValue() ) / 50);
	relationPartialOrderDrawing->Refresh (true);
}

void DiscreteMathCalculatorFrame::OnGridGraphMenuItemSelected (wxCommandEvent& event)
{
	long m, n;

	if (!saveGraphPreview (false) )
		return;

	// m_canvas.Clear();

	GridGraphInputDialog  gridGraphDimensions (this, &m, &n);

	if (gridGraphDimensions.ShowModal() == wxID_CANCEL)
		return;

	// determineGraphType();
	graphType = undir;
	isWeighted = false;
	isDigraph = false;
	graphLayout = orthogonal;
	graphsGraphTypeRadioBox->SetSelection (0);
	graphsWeightedCheckBox->SetValue (false);
	graphLayoutRadioBox->SetSelection (2);
	graphsAllowParallelEdgesCheckBox->SetValue (false);

	myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, false);
	myGraph->setDrawingCanvas (previewWindow);
	previewWindow->setGA (myGraph->getGraphAttributes() );
	myGraph->eraseCanvas();
	myGraph->drawGridGraph (m, n);
	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	dirty = false;
	figureSaved = false;
}

void DiscreteMathCalculatorFrame::OnGraphSequenceGraphMenuItemSelected (wxCommandEvent& event)
{
	DisplayAlgorithmResults *GraphSequence;
	vector<int> dataValues, savedDataValues;

	if (!saveGraphPreview (false) )
		return;

	InputIntegerData  dataEntered (this, &dataValues, false);

	if (dataEntered.ShowModal() == wxID_CANCEL)
		return;

	if (dataValues.empty() )
		return;

	savedDataValues = dataValues;

	if (checkForGraphSequence (dataValues) == true)
		::report (wxT ("It is a graph sequence."), wxT ("Note") );
	else
	{
		::report (wxT ("It is not a graph sequence."), wxT ("Note") );
		return;
	}

	graphLayoutRadioBox->SetSelection (3);
	graphLayout = circular;
	graphType = undir;
	isWeighted = false;
	isDigraph = false;
	graphsGraphTypeRadioBox->SetSelection (0);
	graphsWeightedCheckBox->SetValue (false);
	graphsAllowParallelEdgesCheckBox->SetValue (false);
	drawGraphSequence (savedDataValues);
}

bool DiscreteMathCalculatorFrame::checkForGraphSequence (vector<int>& v)
{
	int i, n = v.size(), numberOfOddValues = 0, k, startIndex = 1, kcount;
	vector<wxString> w;
	bool allZero;

	// initial list must be in non-increasing order
	insertionSort (v, 0, w);

	for (i=0; i < n; i++)
	{
		if (v[i] >= n)
			return false;

		if (v[i] % 2 ==1)
			numberOfOddValues++;
	}

	if (numberOfOddValues % 2 == 1)
		return false;

	while (true)
	{
		allZero = true;

		for (i = startIndex-1; i < n; i++)
		{
			if (v[i] < 0)
				return false;

			if (v[i] != 0)
				allZero = false;
		}

		if (allZero)
			return true;

		\
		// sort in non-increasing order
		insertionSort (v, startIndex, w);

		k = v[startIndex-1];
		kcount = 0;

		for (i = startIndex; i < n; i++)
		{
			kcount++;

			if (kcount <= k)
				v[i]--;
			else
				break;
		}

		startIndex++;
	}
}

void DiscreteMathCalculatorFrame::drawGraphSequence (vector<int>& v)
{
	int i, n = v.size(), k, startIndex = 1, kcount;
	bool allZero;
	BuildGraph* myGraphSequenceAsGraph = new BuildGraph (wxT (""), undir,
	      circular, true, false);
	vector<wxString> vertexLabels;
	wxString tildeLabel;

	myGraph = myGraphSequenceAsGraph;

	myGraphSequenceAsGraph->setDrawingCanvas (previewWindow);
	// m_canvas.Clear();

	// build v.size() vertex labels "~i", 1 <= i <= v.size() and save in the
	// vector vertexLabels.
	for (i=0; i < v.size(); i++)
	{
		tildeLabel.Printf (wxT ("~%d"), i+1);
		vertexLabels.push_back (tildeLabel);
		myGraphSequenceAsGraph->insertEdge (vertexLabels.back(), wxT ("#"), wxT (""), wxNORMAL_PEN,
		                                    wxColour(255,215,0),wxColour(255,215,0));
	}

	// initial list must be in non-increasing order
	insertionSort (v, 0, vertexLabels);

	while (true)
	{
		allZero = true;

		for (i = startIndex-1; i < n; i++)
		{
			if (v[i] != 0)
				allZero = false;
		}

		if (allZero)
			break;

		// sort in non-increasing order
		insertionSort (v, startIndex, vertexLabels);

		k = v[startIndex-1];
		kcount = 0;

		for (i = startIndex; i < n; i++)
		{
			kcount++;

			if (kcount <= k)
			{
				wxString src, dest;

				src = vertexLabels[startIndex-1];
				dest = vertexLabels[i];
				myGraphSequenceAsGraph->insertEdge (vertexLabels[startIndex-1],
				                                    vertexLabels[i],
				                                    wxT (""), wxNORMAL_PEN, wxColour(255,215,0), wxColour(255,215,0) );
				v[i]--;
			}
			else
				break;
		}

		startIndex++;
	}

	myGraphSequenceAsGraph->circularLayout();
	previewWindow->setGA (myGraphSequenceAsGraph->getGraphAttributes() );

	DrawGraph *drawDisplayAlgorithmResults = new DrawGraph (myGraphSequenceAsGraph->getGraphAttributes(), myGraphSequenceAsGraph->getGraph(), &m_canvas, previewWindow,
	      undir, circular);
	drawDisplayAlgorithmResults->draw();
	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	figureSaved = false;
}

// insertion sort is stable
void DiscreteMathCalculatorFrame::insertionSort (vector<int>& v, int p, vector<wxString>& w)
{
	int i, j, n = v.size();
	int target;
	bool swapSecondVector = (w.size() != 0);
	wxString stringTarget;

	// place v[i] into the sublist
	//   v[0] ... v[i-1], 1 <= i < n,
	// so it is in the correct position
	for (i = p+1; i < n; i++)
	{
		// index j scans down list from v[i] looking for
		// correct position to locate target. assigns it to
		// v[j]
		j = i;
		target = v[i];

		if (swapSecondVector)
			stringTarget = w[i];

		// locate insertion point by scanning downward as long
		// as target < v[j-1] and we have not encountered the
		// beginning of the list
		while (j > 0 && target > v[j-1])
		{
			// shift elements up list to make room for insertion
			v[j] = v[j-1];

			if (swapSecondVector)
				w[j] = w[j-1];

			j--;
		}

		// the location is found; insert target
		v[j] = target;

		if (swapSecondVector)
			w[j] = stringTarget;
	}
}

void DiscreteMathCalculatorFrame::OnDigraphicalSequenceGraphMenuItemSelected (wxCommandEvent& event)
{
	DisplayAlgorithmResults *GraphSequence;
	vector<int> a, b;
	vector<pair<int,int> > v;

	DigraphicalSequence dataEntered (this, "digraphicalsequence.png", &a, &b);

	if (!saveGraphPreview (false) )
		return;

	if (dataEntered.ShowModal() == wxID_CANCEL)
		return;

	if (a.empty() || b.empty() )
		return;

	if (checkForDigraphicalSequence (a, b) == true)
		::report (wxT ("It is a digraphical sequence."), wxT ("Note") );
	else
	{
		::report (wxT ("It is not a digraphical sequence."), wxT ("Note") );
		return;
	}

	graphLayoutRadioBox->SetSelection (3);
	graphLayout = hierarchical;
	graphLayoutRadioBox->SetSelection (0);
	graphType = digraph;
	isWeighted = false;
	isDigraph = true;
	graphsGraphTypeRadioBox->SetSelection (1);
	graphsWeightedCheckBox->SetValue (false);
	graphsAllowParallelEdgesCheckBox->SetValue (false);

	for (int i = 0; i < a.size(); i++)
		v.push_back (pair<int,int> (a[i], b[i]) );

	drawDigraphicalSequence (v);
}

bool DiscreteMathCalculatorFrame::checkForDigraphicalSequence (vector<int>& a,vector<int>& b)
{
	int sum_a = 0, sum_b = 0, n = a.size(), i, k, j;
	int lhs, rhs1, rhs2;

	for (i=0; i < n; i++)
	{
		sum_a += a[i];
		sum_b += b[i];
	}

	if (sum_a != sum_b)
		return false;

	for (k=0; k < n; k++)
	{
		lhs = rhs1 = rhs2 = 0;

		for (i = 0; i <= k-1; i++)
		{
			lhs += a[i];
			rhs1 += min (b[i], k);
		}

		for (i=k; i < n; i++)
			rhs2 += min (b[i], k+1);

		if (lhs > rhs1 + rhs2)
			return false;
	}

	return true;
}

bool lexcomp (const DigraphicalSequenceStorage& lhs, const DigraphicalSequenceStorage& rhs)
{
	if (lhs.p.first > rhs.p.first)
		return true;
	else if (lhs.p.first <  rhs.p.first)
		return false;
	else
		return lhs.p.second >= rhs.p.second;
}

void DiscreteMathCalculatorFrame::insertionSort (vector<DigraphicalSequenceStorage>& v,
      bool comp (const DigraphicalSequenceStorage& lhs,
                 const DigraphicalSequenceStorage& rhs) )
{
	int i, j, n = v.size();
	DigraphicalSequenceStorage target;

	// place v[i] into the sublist
	//   v[0] ... v[i-1], 1 <= i < n,
	// so it is in the correct position
	for (i = 0; i < n; i++)
	{
		// index j scans down list from v[i] looking for
		// correct position to locate target. assigns it to
		// v[j]
		j = i;
		target = v[i];

		// locate insertion point by scanning downward as long
		// as target < v[j-1] and we have not encountered the
		// beginning of the list
		while (j > 0 && comp (target,v[j-1]) )
		{
			// shift elements up list to make room for insertion
			v[j] = v[j-1];
			j--;
		}

		// the location is found; insert target
		v[j] = target;
	}
}

void DiscreteMathCalculatorFrame::drawDigraphicalSequence (vector<pair<int,int> >& v)
{
	int i, j, n = v.size(), imax, k;
	bool allZero;
	BuildGraph* myDigraphicalSequenceAsGraph = new BuildGraph (wxT (""), digraph,
	      hierarchical, true, false);
	vector<wxString> vertexLabels;
	vector<DigraphicalSequenceStorage> forLexSort;
	wxString vertexLabel;
	wxString src, dest;

	myGraph = myDigraphicalSequenceAsGraph;

	myDigraphicalSequenceAsGraph->setDrawingCanvas (previewWindow);
	// m_canvas.Clear();

	// build v.size() vertex labels "vi", 1 <= i <= v.size() and save in the
	// vector vertexLabels.
	for (i=0; i < v.size(); i++)
	{
		vertexLabel.Printf (wxT ("v%d"), i+1);
		vertexLabels.push_back (vertexLabel);
		myDigraphicalSequenceAsGraph->insertEdge (vertexLabels.back(), wxT ("#"), wxT (""), wxNORMAL_PEN,
		      wxColour(255,215,0),wxColour(255,215,0));
	}

	while ( (i = exists_bi_gtr0 (v) ) != -1)
	{
		k = v[i].second;
		copyAllButVi (forLexSort, v, i);
		insertionSort (forLexSort, lexcomp);

		src = vertexLabels[i];

		for (j = 0; j < k; j++)
		{
			dest = vertexLabels[forLexSort[j].origIndex];
			myDigraphicalSequenceAsGraph->insertEdge (src, dest, wxT (""), wxNORMAL_PEN,
			      wxColour(255,215,0), wxColour(255,215,0));
			v[forLexSort[j].origIndex].first--;
		}

		v[i].second = 0;
	}

	myDigraphicalSequenceAsGraph->hierarchicalLayout();
	previewWindow->setGA (myDigraphicalSequenceAsGraph->getGraphAttributes() );

	DrawGraph *drawDisplayAlgorithmResults = new DrawGraph (myDigraphicalSequenceAsGraph->getGraphAttributes(), myDigraphicalSequenceAsGraph->getGraph(), &m_canvas, previewWindow,
	      digraph, hierarchical);
	drawDisplayAlgorithmResults->draw();
	previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
	figureSaved = false;
}

int DiscreteMathCalculatorFrame::exists_bi_gtr0 (const vector<pair<int,int> >&v)
{
	for (int i=0; i < v.size(); i++)
		if (v[i].second > 0)
			return i;

	return -1;
}

void DiscreteMathCalculatorFrame::copyAllButVi (vector<DigraphicalSequenceStorage>& lst,
      const vector<pair<int,int> >& v, int i)
{
	DigraphicalSequenceStorage s;

	lst.clear();

	for (int k=0; k < v.size(); k++)
	{
		if (k != i)
		{
			s.p = v[k];
			s.origIndex = k;
			lst.push_back (s);
		}
	}
}

void DiscreteMathCalculatorFrame::OnECCToPButtonPressed (wxCommandEvent& event)
{
	ECn result;

    if (!InitializeDisplayedEllipticCurve())
        return;

    result = GetPoint(
        ECCResult_x_coordinateTextCtrl,
        ECCResult_y_coordinateTextCtrl,
        wxT("There is no result."));

	if (result.iszero() )
		return;

	if (!pointOnCurve (result) )
		::report (wxT ("Application error: The result is not on the curve."), wxT ("Note") );

	WritePointResult (ECCBasePoint_x_CoordinateTextCtrl, ECCBasePoint_y_CoordinateTextCtrl, result);
}

/*void DiscreteMathCalculatorFrame::OnECCBasePointOrderButtonPressed (wxCommandEvent& event)
{
	ECn P;
	Big epointOrder, order;

	if (GetBigScalar (ECCCurveOrderTextCtrl,order) == false )
	{
		::report (wxT ("The curve order must be known."),
		          wxT ("Note") );
		return;
	}

	P = GetPoint (ECCBasePoint_x_CoordinateTextCtrl, ECCBasePoint_y_CoordinateTextCtrl, wxT ("There is no value for the base point.") );

	if (P.iszero() )
		return;

	try
	{
		pointOrder (P, order, epointOrder);
	}

	catch (const schoofException& factErr)
	{
		::report (wxT ("The curve order is too large to factor."), wxT ("Note") );
		return;
	}

	WriteScalarResult (ECCSubgroupOrderTextCtrl, epointOrder);
	currentCurve.n = epointOrder;
}*/

ECn DiscreteMathCalculatorFrame::GetPoint(
    wxTextCtrl *x,
    wxTextCtrl *y,
    const wxString& msg)
{
    ECn P;
    Big X, Y;

    if (!haveCurrentEllipticCurve)
    {
        ::report(
            wxT("There is no elliptic curve defined."),
            wxT("Note"));
        return P;
    }

    if (!GetBigScalar(x, X) || !GetBigScalar(y, Y))
    {
        ::report(msg, wxT("Note"));
        return P;
    }

    if (!P.set(X, Y))
    {
        ::report(
            wxT("The specified point is not on the elliptic curve."),
            wxT("Error"));
        return ECn();
    }

    return P;
}

bool DiscreteMathCalculatorFrame::InitializeDisplayedEllipticCurve()
{
    Big p, A, B;

    if (!GetBigScalar(ECCPrimeTextCtrl, p, false) ||
        !GetBigScalar(ECCCoefficientATextCtrl, A, false) ||
        !GetBigScalar(ECCCoefficientBTextCtrl, B, false))
    {
        ::report(
            wxT("The elliptic curve parameters p, A, and B are required."),
            wxT("Error"));
        return false;
    }

    if (p <= 3 || !isprime(p.getbig()))
    {
        ::report(
            wxT("The elliptic curve modulus must be prime and greater than 3."),
            wxT("Error"));
        return false;
    }

    Big discriminant = (4 * A * A * A + 27 * B * B) % p;

    if (discriminant == 0)
    {
        ::report(
            wxT("The elliptic curve is singular."),
            wxT("Error"));
        return false;
    }

	ecurve(A, B, p, MR_AFFINE);
    return true;
}

bool DiscreteMathCalculatorFrame::GetBigScalar(
    wxTextCtrl* t,
    Big& value,
    bool checkForExistingCurve)
{
    if (checkForExistingCurve && !haveCurrentEllipticCurve)
        return false;

    wxString str = t->GetValue();

    if (str.IsEmpty())
        return false;

    miracl* mip = get_mip();

    mip->IOBASE = (base == 10) ? 10 : 16;

    wxCharBuffer buffer = str.mb_str(wxConvUTF8);

    if (!buffer.data())
        return false;

    value = Big(buffer.data());

    return true;
}

void DiscreteMathCalculatorFrame::WritePointResult(
    wxTextCtrl* tx,
    wxTextCtrl* ty,
    const ECn& q)
{
    Big X, Y;

    q.get(X, Y);

    miracl* mip = get_mip();
    mip->IOBASE = (base == 10) ? 10 : 16;

    std::ostringstream xStream;
    std::ostringstream yStream;

    xStream << X;
    yStream << Y;

    tx->SetValue(wxString::FromUTF8(xStream.str().c_str()));
    ty->SetValue(wxString::FromUTF8(yStream.str().c_str()));
}

void DiscreteMathCalculatorFrame::WriteScalarResult(
    wxTextCtrl* t,
    const Big& v)
{
    miracl* mip = get_mip();

    mip->IOBASE = (base == 10) ? 10 : 16;

    std::ostringstream stream;
    stream << v;

    t->SetValue(
        wxString::FromUTF8(stream.str().c_str())
    );
}

void DiscreteMathCalculatorFrame::OnECCCheckCurveButton(
    wxCommandEvent& event)
{
    Big p, a, b;

    if (!GetBigScalar(ECCPrimeTextCtrl, p, false) ||
        !GetBigScalar(ECCCoefficientATextCtrl, a, false) ||
        !GetBigScalar(ECCCoefficientBTextCtrl, b, false))
    {
        reportError(
            wxT("Enter valid values for p, A, and B."),
            wxT("Invalid Curve"));
        return;
    }

    if (p <= 3)
    {
        reportError(
            wxT("The prime modulus must be greater than 3."),
            wxT("Invalid Curve"));
        return;
    }

	if (!prime(p))
	{
		wxMessageBox(
			wxT("Invalid elliptic curve: p is not prime."),
			wxT("Check Curve"),
			wxOK | wxICON_ERROR,
			this);
		return;
	}

    Big discriminant = (4 * a * a * a + 27 * b * b) % p;

    if (discriminant == 0)
    {
        wxMessageBox(
            wxT("Invalid elliptic curve: the discriminant is zero."),
            wxT("Check Curve"),
            wxOK | wxICON_ERROR,
            this);
        return;
    }

    wxMessageBox(
    wxT("Valid elliptic curve.\n")
    wxT("The modulus is prime and the discriminant is nonzero."),
    wxT("Check Curve"),
    wxOK | wxICON_INFORMATION,
    this);
}

void DiscreteMathCalculatorFrame::OnECCVerifyButton (wxCommandEvent& event)
{
	Big prime, a, b, seed;
	ECn g;
	bool one, two, three, four;

	one = GetBigScalar(ECCPrimeTextCtrl, prime, false);
	two = GetBigScalar(ECCSeedTextCtrl, seed, false);
	three = GetBigScalar(ECCCoefficientATextCtrl, a, false);
	four = GetBigScalar(ECCCoefficientBTextCtrl, b, false);

	if ( !one || !two || !three || !four )
	{
		::report (wxT ("Each of prime p, the seed, A, and B must be defined"),
		          wxT ("Note") );
		return;
	}

	if (verify_curve (prime, seed, a, b) == true)
		::report (wxT ("The current elliptic curve verifies."), wxT ("Note") );
	else
		::report (wxT ("The current elliptic curve cannot be verified."), wxT ("Note") );
}

void DiscreteMathCalculatorFrame::clearCurveData()
{
	ECCSeedTextCtrl->Clear();
	ECCPrimeTextCtrl->Clear();
	ECCCoefficientATextCtrl->Clear();
	ECCCoefficientBTextCtrl->Clear();
	ECCCurveOrderTextCtrl->Clear();
	ECCBasePoint_x_CoordinateTextCtrl->Clear();
	ECCBasePoint_y_CoordinateTextCtrl->Clear();
	ECCSubgroupOrderTextCtrl->Clear();
	ECCSubgroupCofactorTextCtrl->Clear();
	ECCPointQ_x_CoordinateTextCtrl->Clear();
	ECCPointQ_y_CoordinateTextCtrl->Clear();
	ECCResult_x_coordinateTextCtrl->Clear();
	ECCResult_y_coordinateTextCtrl->Clear();
	ECCNumberBitsInPrimeTextCtrl->Clear();
	haveCurrentEllipticCurve = false;
	currentCurve.seed = Big (0);
	currentCurve.p = Big (0);
	currentCurve.A = Big (0);
	currentCurve.B = Big (0);
	currentCurve.Gx = Big (0);
	currentCurve.Gy = Big (0);
	currentCurve.n = Big (0);
	currentCurve.cofactor = Big (0);
	currentCurve.order = Big (0);
}

void DiscreteMathCalculatorFrame::OnECCRandomPointButtonPress(
    wxCommandEvent& event)
{
    Big prime, a, b, x, y, n, k;
    ECn P, Q;


    // Read and validate the prime modulus.
    if (!GetBigScalar(ECCPrimeTextCtrl, prime, false) ||
        prime <= 3)
    {
        reportError(
            wxT("A valid prime modulus greater than 3 is required."),
            wxT("Error"));
        return;
    }

    // Read the base point coordinates.
    if (!GetBigScalar(ECCBasePoint_x_CoordinateTextCtrl, x, false) ||
        !GetBigScalar(ECCBasePoint_y_CoordinateTextCtrl, y, false))
    {
        reportError(
            wxT("Valid base point coordinates are required."),
            wxT("Error"));
        return;
    }

    // Read the base point order.
    if (!GetBigScalar(ECCSubgroupOrderTextCtrl, n, false) ||
        n <= 1)
    {
        reportError(
            wxT("The base point order must be greater than 1."),
            wxT("Error"));
        return;
    }

	// Read the displayed elliptic curve coefficients.
	if (!GetBigScalar(ECCCoefficientATextCtrl, a, false) ||
		!GetBigScalar(ECCCoefficientBTextCtrl, b, false))
	{
		reportError(
			wxT("Valid curve coefficients A and B are required."),
			wxT("Error"));
		return;
	}

	// Initialize MIRACL with the displayed curve parameters.
	// Verify that the modulus is prime.
	if (!isprime(prime.getbig()))	{
		reportError(
			wxT("The modulus p must be prime."),
			wxT("Invalid Curve"));
		return;
	}

	// Verify that the elliptic curve is nonsingular.
	Big discriminant =
		(4 * a * a * a + 27 * b * b) % prime;

if (discriminant == 0)
{
    reportError(
        wxT("Invalid elliptic curve: the discriminant is zero."),
        wxT("Invalid Curve"));
    return;
}

// Initialize the elliptic curve using affine coordinates.
ecurve(a, b, prime, MR_AFFINE);

// Verify that P belongs to the currently initialized curve.
if (!P.set(x, y))
{
    reportError(
        wxT("The base point is not on the elliptic curve."),
        wxT("Error"));
    return;
}

// Check that n annihilates the base point.
ECn orderCheck = n * P;

if (!orderCheck.iszero())
{
    reportError(
        wxT("The specified base point order is incorrect."),
        wxT("Error"));
    return;
}

// Choose a nonzero scalar less than n.
do
{
    k = rand(n);
}
while (k == 0);

// Calculate Q = kP.
Q = k * P;

// Display the resulting point.
WritePointResult(
    ECCResult_x_coordinateTextCtrl,
    ECCResult_y_coordinateTextCtrl,
    Q);
}

void DiscreteMathCalculatorFrame::OnECCPlotSimpleEllipticCurveButtonPressed (wxCommandEvent& event)
{
	PlotEllipticCurve *realPlotOfEllipticCurve;

	realPlotOfEllipticCurve = new PlotEllipticCurve (this, "ec1.png", "ec2.png");
	realPlotOfEllipticCurve->ShowModal();
}

void DiscreteMathCalculatorFrame::OnECCDiscreteLogarithmButtonPressed (wxCommandEvent& event)
{
	DiscreteLogarithmEc *ecdlp;
	Big p, n, A, B;
	ECn P, Q;
	bool one, two, three, four;

	if (haveCurrentEllipticCurve == false)
	{
		::report ("An elliptic curve has not been defined.", wxT ("Note") );
		return;
	}

	one = GetBigScalar (ECCPrimeTextCtrl,p);
	two = GetBigScalar (ECCCoefficientATextCtrl,A);
	three = GetBigScalar (ECCCoefficientBTextCtrl,B);
	four = GetBigScalar (ECCSubgroupOrderTextCtrl,n);

	if ( !one || !two || !three || !four)
	{
		::report ("The prime, curve coefficients and the basepoint order must be defined.", wxT ("Note") );
		return;
	}

	// Initialize MIRACL once for both elliptic-curve points.

	if (!InitializeDisplayedEllipticCurve())
		return;	P = GetPoint (ECCBasePoint_x_CoordinateTextCtrl, ECCBasePoint_y_CoordinateTextCtrl, wxT ("The basepoint must be defined.") );

	if (P.iszero() )
		return;

	Q = GetPoint (ECCPointQ_x_CoordinateTextCtrl, ECCPointQ_y_CoordinateTextCtrl, wxT ("The point Q must be defined.") );

	if (Q.iszero() )
		return;

	ecdlp = new DiscreteLogarithmEc (this, "pollardrho.png", p, A, B, n, P, Q, base);

	if (ecdlp->ShowModal() == wxID_CANCEL)
		return;
}

bool DiscreteMathCalculatorFrame::CheckOrderSize()
{
	int numBits;

	numBits = logb2 (currentCurve.p.getbig() );

	if (numBits > 160)
	{
		wxMessageDialog d (this, wxT ("It will take some time (perhaps a very long time) to find the curve order. Do you w to continue?"),
		                   wxT ("Note"), wxYES_NO | wxCANCEL);

		if (d.ShowModal() != wxID_YES)
			return false;
		else
			return true;
	}

	return true;
}

void DiscreteMathCalculatorFrame::OnECCreateSecureCurveDomain (wxCommandEvent& event)
{
	SecureDomain *dialog;
	Big vPrime, vSeed;
	vector<wxTextCtrl*> v = {ECCSeedTextCtrl, ECCPrimeTextCtrl, ECCSubgroupOrderTextCtrl,
	                         ECCSubgroupCofactorTextCtrl, ECCCoefficientATextCtrl,
	                         ECCCoefficientBTextCtrl, ECCCurveOrderTextCtrl,
	                         ECCBasePoint_x_CoordinateTextCtrl, ECCBasePoint_y_CoordinateTextCtrl
	                        };

	bool havePrimeInPrimeTextCtrl = GetBigScalar (ECCPrimeTextCtrl, vPrime, false);
	bool haveSeedTextCtrl = GetBigScalar (ECCSeedTextCtrl, vSeed, false);

	if (!havePrimeInPrimeTextCtrl || !haveSeedTextCtrl)
	{
		::report(wxT("You must define a prime and a seed before building a secure curve."), wxT("Note"));
		return;
	}

	dialog = new SecureDomain (this, v, base, vPrime, vSeed);
	dialog->SetClientSize(840,776);

	if (dialog->ShowModal() == wxID_CANCEL)
		return;

	haveCurrentEllipticCurve = true;
	numbitsInPrime = -1;
}

void DiscreteMathCalculatorFrame::OnECCRandom160BitSeedButtonPress (wxCommandEvent& event)
{
	big rnd;
	Big myBigSeed;
	miracl *mip = get_mip();

	if (base == 10)
		mip->IOBASE = 10;
	else
		mip->IOBASE = 16;

	rnd = mirvar (0);
	bigbits (160,rnd);
	myBigSeed = Big (rnd);
	WriteScalarResult (ECCResult_x_coordinateTextCtrl,myBigSeed);
}

void DiscreteMathCalculatorFrame::OnECCToQButtonPress (wxCommandEvent& event)
{
	ECn result;

	if (!InitializeDisplayedEllipticCurve())
      return;result = GetPoint (ECCResult_x_coordinateTextCtrl, ECCResult_y_coordinateTextCtrl, wxT ("There is no result.") );

	if (result.iszero() )
		return;

	WritePointResult (ECCPointQ_x_CoordinateTextCtrl, ECCPointQ_y_CoordinateTextCtrl, result);
}

CurveComponent DiscreteMathCalculatorFrame::parseCurveFileLine(const wxString& line)
{
	CurveComponent component;
	int digit;

	digit = line[0] - '0';
	switch (digit)
	{
	case 0:
		component = PRIME;
		break;

	case 1:
		component = ORDER;
		break;

	case 2:
		component = SEED;
		break;

	case 3:
		component = A;
		break;

	case 4:
		component = B;
		break;

	case 5:
		component = BASE_POINT_X;
		break;

	case 6:
		component = BASE_POINT_Y;
		break;

	case 7:
		component = BASE_POINT_ORDER;
		break;

	case 8:
		component = COFACTOR;
		break;
	}

	return component;
}

void DiscreteMathCalculatorFrame::OnECCkBitPrimeButtonPress (wxCommandEvent& event)
{
	wxString kValue;
	// long numbitsInPrime;
	ZZ numBits, ntlkbitprime;
	Big kbitprime;
	ostringstream numstr;
	wxString resultstr;
	char buf[500];
	miracl *mip = get_mip();

	kValue = ECCNumberBitsInPrimeTextCtrl->GetValue();
	if (kValue.Length() == 0)
	{
		::report (wxT ("There is no value for the number of bits in the prime."), wxT ("Note") );
		return;
	}

	if(!kValue.ToLong(&numbitsInPrime))
	{
		::report (wxT ("The value for the number of bits is not an integer."), wxT ("Note") );
		return;
	}

	while (numbitsInPrime > 1024)
	{
		::report (wxT("1024 is the maximum bit size."), wxT("Note"));
		return;
	}

	conv (numBits, (const char*) kValue.c_str() );
	ntlkbitprime = RandomPrime_ZZ (to_long (numBits) );
	numstr << ntlkbitprime;
	strcpy (buf, (char *) numstr.str().c_str() );
	mip->IOBASE = 10;
	kbitprime = Big (buf);

	if (base == 16)
	{
		resultstr = DecimalToHex (kbitprime);
		ECCResult_x_coordinateTextCtrl->SetValue (resultstr);
	}
	else
	{
		buf << kbitprime;
		ECCResult_x_coordinateTextCtrl->SetValue (buf);
	}

	mip->IOBASE = base;
}

void DiscreteMathCalculatorFrame::OnBigIntegerDiscreteLogarithmMenuItemSelected (wxCommandEvent& event)
{
	DiscreteLog *dl;

	dl = new DiscreteLog (this);
	if (dl->ShowModal() == wxID_CANCEL)
		return;
}

void DiscreteMathCalculatorFrame::OnBigIntegerRSAGuideMenuItemSelected (wxCommandEvent& event)
{
	bool loadedPage;
	wxString RSAHelp;

#ifdef __APPLE__
RSAHelp = wxStandardPaths::Get().GetResourcesDir();
RSAHelp += wxT("/resource/RSAEncryption.html");
#else
RSAHelp = getUsrLocation();
RSAHelp += wxT("/usr/resource/RSAEncryption.html");
#endif

	loadedPage = wxLaunchDefaultBrowser(RSAHelp);
	if (loadedPage == false)
		report(wxT("Failed to load RSA help."), wxT("Note"));
}

void DiscreteMathCalculatorFrame::OnECCToSeedButtonPressed (wxCommandEvent& event)
{
	Big result;

	if (GetBigScalar (ECCResult_x_coordinateTextCtrl, result, false) == false)
	{
		::report (wxT ("There is no result."), wxT ("Note") );
		return;
	}

	WriteScalarResult (ECCSeedTextCtrl, result);
}

void DiscreteMathCalculatorFrame::OnECCTopButtonPressed (wxCommandEvent& event)
{
	Big result;

	if (GetBigScalar (ECCResult_x_coordinateTextCtrl, result, false) == false)
	{
		::report (wxT ("There is no result."), wxT ("Note") );
		return;
	}

	WriteScalarResult (ECCPrimeTextCtrl, result);
}

void DiscreteMathCalculatorFrame::OnIntegerFactorMenuItemSelected (wxCommandEvent& event)
{
	FactorDialog dialog(this);

	if ( dialog.ShowModal() == wxID_CANCEL )
		return;
}

void DiscreteMathCalculatorFrame::OnBigIntegerDiffieHellmanMenuItemSelected (wxCommandEvent& event)
{
	wxArrayString DiffieHellmanProcessStdout, DiffieHellmanProcessStderr;
	wxString empty;
	long wxExecuteReturnValue;

	DiffieHellmanProcessStdout = runApplication (wxT ("Alice"), &empty, 1, false);
	DiffieHellmanProcessStdout = runApplication (wxT ("Bob"), &empty, 1, false);
}

void DiscreteMathCalculatorFrame::OnBigIntegerDigitalSignatureMenuItemSelected (wxCommandEvent& event)
{
	wxArrayString DigitalSignatureProcessStdout, DigitalSignatureProcessStderr;
	wxString empty;

	DigitalSignatureProcessStdout = runApplication (wxT ("Receiver"), &empty, 1, false);
	DigitalSignatureProcessStdout = runApplication (wxT ("Sender"), &empty, 1, false);
}

void DiscreteMathCalculatorFrame::OnECCClearButtonPressed (wxCommandEvent& event)
{
	clearCurveData();
}

void DiscreteMathCalculatorFrame::GetCurveParameter(
    wxTextCtrl* t, Big& parm)
{
    miracl* mip = get_mip();

    mip->IOBASE = (base == 10) ? 10 : 16;

    wxString s = t->GetValue();
    wxCharBuffer buffer = s.mb_str(wxConvUTF8);

    parm = Big(buffer.data());
}

wxString DiscreteMathCalculatorFrame::getUsrLocation()
{
	wxString usrPath;

#ifdef _WIN64
	wxString exeDirectory;
	wchar_t path[2048];
	wxString exePath;

	GetModuleFileName (NULL, path, 2048);
	exePath = wxString (path);

	exeDirectory = exePath.substr (0, exePath.rfind (wxT ("\\") ) );
	usrPath = exeDirectory;

#elif __APPLE__
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyBundleURL (mainBundle);
	CFStringRef str = CFURLCopyFileSystemPath (resourcesURL, kCFURLPOSIXPathStyle);
	CFRelease (resourcesURL);
	char path[PATH_MAX];

	CFStringGetCString (str, path, FILENAME_MAX, kCFStringEncodingASCII);
	CFRelease (str);
	usrPath = wxString(path);
   cout << "usrPath = " << usrPath << endl;
#else
	char path[PATH_MAX+1], *p;

	DiscreteMathCalculatorApp::exe_name (path);
	if ( (p = strrchr (path, '/') ) )
		* (p+1) = '\0';
	usrPath = wxString(path);
   cout << "usrPath = " << usrPath << endl;

#endif

	// int locationOfDiscreteMathCalculatorSubstring;

	// locationOfDiscreteMathCalculatorSubstring = usrPath.Find(wxT("DiscreteMathCalculator"));
	usrPath = usrPath.Left(29);
   cout << "usrPath = " << usrPath << endl;

	return usrPath;
}

void DiscreteMathCalculatorFrame::OnECCPointPlotSimpleEllipticCurveButtonPressed (wxCommandEvent& event)
{
    PointPlotEllipticCurve* integerPlotOfEllipticCurve =
        new PointPlotEllipticCurve (this, "ecpointplot.png");

    integerPlotOfEllipticCurve->ShowModal();
}

bool check(const Big& A, const Big& B, const Big& x, const Big& y, const Big&p)
{
	return (y*y)%p == (x*x*x + A*x + B)%p;
}

void DiscreteMathCalculatorFrame::OnEllipticCurveDigitalSignatureMenuItemSelected (wxCommandEvent& event)
{
	long wxExecuteReturnValue;
	ofstream primeVal;
	long bitsForEncryption;
	char buf[128];
	wxString empty, primeStr;
	wxArrayString ellipticCurveSetupResultsStdout, ellipticCurveSetupProcessStderr;
	wxArrayString DiffieHellmanGFProcessStdout;
	wxTextEntryDialog textEntryDialog(this, "",
	                                  "Enter an upper bound on the prime to use for code signing.");
	while (true)
	{
		if (textEntryDialog.ShowModal() == wxID_OK)
			primeStr = textEntryDialog.GetValue();
		else
			return;

		if (primeStr.Length() == 0)
		{
			report(wxT("Enter an upper bound on the prime to use for code signing."), wxT("Note"));
			continue;
		}
		else if (primeStr.ToLong(&bitsForEncryption) == false)
		{
			report(wxT("The number of bits is not a proper integer."), wxT("Note"));
			continue;
		}
		else if (bitsForEncryption <= 100)
		{
			report(wxT("The number of bits must be >= 101."), wxT("Note"));
			continue;
		}
		break;
	}

#ifdef __APPLE__
wxString primeDirectory =
    wxStandardPaths::Get().GetUserDataDir();

wxFileName::Mkdir(
    primeDirectory,
    wxS_DIR_DEFAULT,
    wxPATH_MKDIR_FULL);

wxFileName primeFile(primeDirectory, wxT("prime.dat"));
primeVal.open(primeFile.GetFullPath().mb_str());
#else
#ifdef FOR_DISTRIBUTION
primeVal.open("../usr/resource/prime.dat");
#else
primeVal.open("../../usr/resource/prime.dat");
#endif
#endif
	strncpy(buf, (const char*)primeStr.mb_str(wxConvUTF8), 128);
	primeVal << buf;
	primeVal << "\n";
	primeVal.close();

	DiffieHellmanGFProcessStdout = runApplication (wxT ("DocumentServer"), &empty, 1, false);
	waitForDocumentServerToBeReady();
	DiffieHellmanGFProcessStdout = runApplication (wxT ("DocumentRequester"), &empty, 1, false);
}

void DiscreteMathCalculatorFrame::waitForDocumentServerToBeReady()
{
#ifdef __APPLE__
wxString dataDirectory =
    wxStandardPaths::Get().GetUserDataDir();

wxFileName commonFile(dataDirectory, wxT("common.ecs"));
wxFileName publicFile(dataDirectory, wxT("public.ecs"));
wxFileName privateFile(dataDirectory, wxT("private.ecs"));

while (!wxFile::Exists(commonFile.GetFullPath()))
{
}

while (!wxFile::Exists(publicFile.GetFullPath()))
{
}

while (!wxFile::Exists(privateFile.GetFullPath()))
{
}
#else
while (true)
{
#ifdef FOR_DISTRIBUTION
if (wxFile::Exists(wxT("../usr/resource/common.ecs")))
break;
#else
if (wxFile::Exists(wxT("../../usr/resource/common.ecs")))
break;
#endif
}

while (true)
{
#ifdef FOR_DISTRIBUTION
if (wxFile::Exists(wxT("../usr/resource/public.ecs")))
break;
#else
if (wxFile::Exists(wxT("../../usr/resource/public.ecs")))
break;
#endif
}

while (true)
{
#ifdef FOR_DISTRIBUTION
if (wxFile::Exists(wxT("../usr/resource/private.ecs")))
break;
#else
if (wxFile::Exists(wxT("../../usr/resource/private.ecs")))
break;
#endif
}
#endif
}

void DiscreteMathCalculatorFrame::outputBigCurveParameters()
{
	string line;
	Big prime, a, b, order, curveOrder;
	char buf[256];
#ifdef FOR_DISTRIBUTION
	ifstream appoutputfile("../usr/resource/bigcurveparms.ecs");
#else
	ifstream appoutputfile("../../usr/resource/bigcurveparms.ecs");
#endif
	int i = 0, cofactor;
	Flash v;

	while (getline(appoutputfile, line))
	{
		switch (i)
		{
		case BIGCURVEBITS:
			break;

		case BIGCURVEP:
			ECCPrimeTextCtrl->AppendText(wxString(line));
			strcpy(buf, line.c_str());
			prime = Big(buf);
			break;

		case BIGCURVEA:
			ECCCoefficientATextCtrl->AppendText(wxString(line));
			strcpy(buf, line.c_str());
			a = Big(buf);
			break;

		case BIGCURVEB:
			ECCCoefficientBTextCtrl->AppendText(wxString(line));
			strcpy(buf, line.c_str());
			b = Big(buf);
			break;

		case BIGCURVEORDER:
			ECCSubgroupOrderTextCtrl->AppendText(wxString(line));
			strcpy(buf, line.c_str());
			order = Big(buf);
			break;

		case BIGCURVEX:
			ECCBasePoint_x_CoordinateTextCtrl->AppendText(wxString(line));
			break;

		case BIGCURVEY:
			ECCBasePoint_y_CoordinateTextCtrl->AppendText(wxString(line));
			break;
		}
		i++;
	}

	buf << orderOfEllipticCurve;
	ECCCurveOrderTextCtrl->AppendText(buf);
	v = Flash(orderOfEllipticCurve)/Flash(order);
	if (v <= Flash(1))
		cofactor = 1;
	else if (v <= Flash(2))
		cofactor = 2;
	else if (v <= Flash(3))
		cofactor = 3;
	else if (v <= Flash(4))
		cofactor = 4;
	else if (v <= Flash(5))
		cofactor = 5;
	else if (v <= Flash(6))
		cofactor = 6;
	else if (v <= Flash(7))
		cofactor = 7;
	else if (v <= Flash(8))
		cofactor = 8;
	else
		cofactor = 0;

	if (cofactor != 0)
		ECCSubgroupCofactorTextCtrl->AppendText(wxString(char('0' + cofactor)));

	haveCurrentEllipticCurve = true;
}

int DiscreteMathCalculatorFrame::findStartOfBigCurveParameter(wxString s)
{
	int i = 0;

	while (!isdigit(s[i]))
		i++;

	return i;
}

void DiscreteMathCalculatorFrame::OnBigIntegerCalculatorXbitRandomIntegerButtonPress( wxCommandEvent& event )
{
	char buf[512];
	Big xBitInteger;
	wxString xBitIntegerStr;
	long numberOfBits;

	xBitIntegerStr = BigIntegerCalculatorXbitIntegerTextCtrl->GetValue();
	if (xBitIntegerStr.Length() == 0)
	{
		::report(wxT("The number of bits for the integer is not defined."), wxT("Note"));
		return;
	}
	if (!xBitIntegerStr.ToLong(&numberOfBits))
	{
		::report(wxT("The number of bits is not in correct integer format."), wxT("Note"));
		return;
	}

	xBitInteger = generateBig(numberOfBits);
	buf << xBitInteger;
	resultValueTextCtrl->SetValue(wxString(buf));
}


void DiscreteMathCalculatorFrame::OnRelationToGMLButtonPress (wxCommandEvent& event)
{
	wxString formatSelection;

	if (myRelationDigraph == NULL)
	{
		::report(wxT("There is no relation digraph."), wxT("Note"));
		return;
	}

	formatSelection.Append ("GML files (*.gml)|*.gml|" );
	wxString filename = wxFileSelector ("Select a format.",wxEmptyString, wxEmptyString,wxEmptyString, formatSelection,
	                                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (filename.empty() )
		return;

	wxString extension = filename.AfterLast ('.');

	if (extension == wxT(""))
	{
		//  extension not specified
		wxMessageBox (wxT ("An extension must be added to the file name."), wxT ("Discrete Mathematics Calculator"),
		              wxOK | wxICON_WARNING);
		return;
	}

	saveRelationAsGML (filename);
}

void DiscreteMathCalculatorFrame::OnInputRelationRELButtonPressed(wxCommandEvent& event)
{
	ReadFromREL();
}

void DiscreteMathCalculatorFrame::OnPartialOrderOutputGMLButtonPress(wxCommandEvent& event)
{
	wxString formatSelection;

	if (myRelationDigraph == NULL)
	{
		::report(wxT("There is no partial order graph."), wxT("Note"));
		return;
	}

	formatSelection.Append ("GML files (*.gml)|*.gml|" );
	wxString filename = wxFileSelector ("Select a format.",wxEmptyString, wxEmptyString,wxEmptyString, formatSelection,
	                                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (filename.empty() )
		return;

	wxString extension = filename.AfterLast ('.');

	if (extension == wxT(""))
	{
		//  extension not specified
		wxMessageBox (wxT ("An extension must be added to the file name."), wxT ("Discrete Mathematics Calculator"),
		              wxOK | wxICON_WARNING);
		return;
	}

	savePartialOrderAsGML(filename);
}

void DiscreteMathCalculatorFrame::OnPartialOrderFromPOSETButtonPress(wxCommandEvent& event)
{
	ReadFromPOSET();
}

void DiscreteMathCalculatorFrame::ReadFromREL()
{
	wxString filename, srcVertex, destVertex, fileLine;
	vector<RelationGraphEdge> diGraphToBuild;

	if (!saveRelationPreview (false) )
		return;

	filename = wxFileSelector (wxT ("Choose a REL file specifying your relation."),
	                           wxT (""),
	                           wxT (""),
	                           wxT (""),
	                           wxT ("REL file (*.rel)|*.rel"),
	                           wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if ( filename.empty())
		return;

	if (myRelationDigraph != NULL)
		delete myRelationDigraph;
	m_relationCanvas.Clear();

	myRelationDigraph = new BuildGraph (wxT (""), digraph, hierarchical, true, false);
	graphType = digraph;
	myRelationDigraph->setDrawingCanvas (relationPartialOrderDrawing);

	// read the relation or poset edges and insert them into an element of diGraphToBuild
	wxTextFile fin(filename);
	fin.Open();

	for ( fileLine = fin.GetFirstLine(); !fin.Eof(); fileLine = fin.GetNextLine() )
	{
		// extract the source vertex followed by the destination vertex
		// from fileLine
		srcVertex = fileLine.BeforeFirst((wxUniChar)'\t', &srcVertex);
		// cout << "srcVertex = " << srcVertex << endl;
		destVertex = fileLine.AfterLast((wxUniChar)'\t');
		// cout << "destVertex = " << destVertex << endl;
		diGraphToBuild.push_back(RelationGraphEdge(srcVertex, destVertex));
	}

	relationPartialOrderDrawing->setGA (myRelationDigraph->getGraphAttributes() );
	myRelationDigraph->drawMyRelationGraph (diGraphToBuild, false, this);
	relationPartialOrderDrawing->RefreshCanvas (true, relationPartialOrderDrawing->GetRect() );
	relationFigureSaved = false;
}

void DiscreteMathCalculatorFrame::ReadFromPOSET()
{
	wxString filename;
	if (!saveRelationPreview (false) )
		return;

	filename = wxFileSelector (wxT ("Choose a POSET file specifying your partial order."),
	                           wxT (""),
	                           wxT (""),
	                           wxT (""),
	                           wxT ("POSET file (*.poset)|*.poset"),
	                           wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if ( filename.empty())
		return;

	if (myRelationDigraph != NULL)
		delete myRelationDigraph;
	m_relationCanvas.Clear();

	myRelationDigraph = new BuildGraph (wxT (""), undir, hierarchical, true, false);
	graphType = undir;
	myRelationDigraph->setDrawingCanvas (relationPartialOrderDrawing);

	// read the relation or poset edges and insert them into an element of diGraphToBuild
	/*wxTextFile fin(filename);
	fin.Open();

	for ( fileLine = fin.GetFirstLine(); !fin.Eof(); fileLine = fin.GetNextLine() )
	{
	    // extract the source vertex followed by the destination vertex
	    // from fileLine
	    srcVertex = fileLine.BeforeFirst((wxUniChar)' ', &srcVertex);
	    // cout << "srcVertex = " << srcVertex << endl;
	    destVertex = fileLine.AfterLast((wxUniChar)' ');
	    // cout << "destVertex = " << destVertex << endl;
	    diGraphToBuild.push_back(RelationGraphEdge(srcVertex, destVertex));
	}*/
	myRelationDigraph->buildFromGRPH (filename, false, true);
	// cout << "Built myRelationDigraph" << endl;

	relationPartialOrderDrawing->setGA (myRelationDigraph->getGraphAttributes() );
	relationPartialOrderDrawing->RefreshCanvas (true, relationPartialOrderDrawing->GetRect() );
	relationFigureSaved = false;
}

void DiscreteMathCalculatorFrame::ReadFromGRPH()
{
	wxString filename;

	if (!saveGraphPreview (false) )
		return;

	filename = wxFileSelector (wxT ("Choose a GRPH file specifying your graph."),
	                           wxT (""),
	                           wxT (""),
	                           wxT (""),
	                           wxT ("GRPH file (*.grph)|*.grph"),
	                           wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if ( !filename.empty() )
	{
		if (myGraph != NULL)
			delete myGraph;

		// m_canvas.Clear();
		determineGraphType();

		myGraph = new BuildGraph (wxT(""), graphType, graphLayout, true, allowParallelEdges);

		myGraph->setDrawingCanvas (previewWindow);

		if (hideEdges && ( (graphType == undir) || (graphType == undirw) || (graphType == undir) ) )
			myGraph->buildFromGRPH (filename, hideEdges, true);
		else
		{
			myGraph->buildFromGRPH (filename, false, true);
			graphsHideEdgesCheckBox->SetValue (false);
			hideEdges = false;
		}

		previewWindow->setGA (myGraph->getGraphAttributes() );
		previewWindow->RefreshCanvas (true, previewWindow->GetRect() );
		figureSaved = false;
	}
}

void DiscreteMathCalculatorFrame::saveRelationAsGML (const wxString& filename)
{
	// convert filename to a std::string
	string file ( (const char *) filename.c_str() );
	wxTextFile gmlFile;
	std::ofstream gmlOut ( (const char *) file.c_str() );
	wxString typeOfGraph = wxT ("# digraph"), lastLine;
	GraphAttributes *GA;

	GA = myRelationDigraph->getGraphAttributes();
	GraphIO::writeGML (*GA, gmlOut);
	gmlOut.close();

	if (!gmlFile.Open (filename) )
	{
		wxString message, caption;

		message.append (
		   wxT ("Failure to properly create the GML file for the graph.") );
		caption.append (wxT ("Error") );

		wxMessageDialog dialog (this, message, caption, wxOK | wxICON_ERROR);
		dialog.ShowModal();

		return;
	}

	lastLine = gmlFile.GetLastLine();
	gmlFile.AddLine(typeOfGraph);
	gmlFile.Write();
	gmlFile.Close();
}

void DiscreteMathCalculatorFrame::savePartialOrderAsGML (const wxString& filename)
{
	// convert filename to a std::string
	string file ( (const char *) filename.c_str() );
	wxTextFile gmlFile;
	std::ofstream gmlOut ( (const char *) file.c_str() );
	wxString typeOfGraph = wxT ("# undir"), lastLine;
	GraphAttributes *GA;

	GA = myRelationDigraph->getGraphAttributes();
	GraphIO::writeGML (*GA, gmlOut);
	gmlOut.close();

	if (!gmlFile.Open (filename) )
	{
		wxString message, caption;

		message.append (
		   wxT ("Failure to properly create the GML file for the graph.") );
		caption.append (wxT ("Error") );

		wxMessageDialog dialog (this, message, caption, wxOK | wxICON_ERROR);
		dialog.ShowModal();

		return;
	}

	lastLine = gmlFile.GetLastLine();
	gmlFile.AddLine(typeOfGraph);
	gmlFile.Write();
	gmlFile.Close();
}

void DiscreteMathCalculatorFrame::OnRelationFromGMLButtonPress(wxCommandEvent& event)
{
	GraphType graphType = digraph;
	GraphLayout graphLayout = hierarchical;
	BuildGraph *relationGraph;
	bool allowParallelEdges = false;

	if (!saveRelationPreview (false) )
		return;

	wxString filename = wxFileSelector (wxT ("Choose a GML file specifying your relation."),
	                                    wxT (""),
	                                    wxT (""),
	                                    wxT (""),
	                                    wxT ("GML file (*.gml)|*.gml"),
	                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	isDigraph = true;
	isWeighted = false;

	if (!filename.empty() )
	{
		if (myRelationDigraph != NULL)
			delete myRelationDigraph;
		// m_relationCanvas.Clear();

		myRelationDigraph = new BuildGraph (wxT (""), graphType, graphLayout, true, allowParallelEdges);
		myRelationDigraph->setDrawingCanvas (relationPartialOrderDrawing);
		myRelationDigraph->buildFromGML (filename, &graphType, &allowParallelEdges, false);

		relationPartialOrderDrawing->setGA (relationGraph->getGraphAttributes() );
		relationPartialOrderDrawing->RefreshCanvas (true, relationPartialOrderDrawing->GetRect() );
		relationFigureSaved = true;
	}
}

void DiscreteMathCalculatorFrame::OnPartialOrderFromGMLButtonPress(wxCommandEvent& event)
{
	GraphType graphType = undir;
	GraphLayout graphLayout = hierarchical;
	BuildGraph *partialOrderGraph;
	bool allowParallelEdges = false;

	if (!saveRelationPreview (false) )
		return;

	wxString filename = wxFileSelector (wxT ("Choose a GML file specifying your partial order."),
	                                    wxT (""),
	                                    wxT (""),
	                                    wxT (""),
	                                    wxT ("GML file (*.gml)|*.gml"),
	                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	isDigraph = false;
	isWeighted = false;

	if (!filename.empty() )
	{
		if (myRelationDigraph != NULL)
			delete myRelationDigraph;
		// m_relationCanvas.Clear();

		myRelationDigraph = new BuildGraph (wxT (""), graphType, graphLayout, true, allowParallelEdges);
		myRelationDigraph->setDrawingCanvas (relationPartialOrderDrawing);
		myRelationDigraph->buildFromGML (filename, &graphType, &allowParallelEdges, false);

		relationPartialOrderDrawing->setGA (partialOrderGraph->getGraphAttributes() );
		relationPartialOrderDrawing->RefreshCanvas (true, relationPartialOrderDrawing->GetRect() );
		relationFigureSaved = true;
	}
}

void DiscreteMathCalculatorFrame::OnECCSaveCurveParametersButtonPressed(wxCommandEvent& event)
{
	char buf[256];
	wxString primeStr;
	Big prime;
	int numBits;

	if (haveCurrentEllipticCurve == false)
	{
		::report(wxT("There is no elliptic curve defined."), wxT("Note"));
		return;
	}

	wxFileDialog saveFileDialog(this, _("Save ECS file"), "", "",
	                            "ECS files (*.ecs)|*.ecs", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	wxString str = saveFileDialog.GetPath(), saveCurveLine;
	wxTextFile ellipticCurveComponents(saveFileDialog.GetPath());
	miracl *mip = get_mip();

	haveDefaultBase = true;
	defaultBase = 10;
	mip->IOBASE = 10;

	if (!ellipticCurveComponents.Create(str))
	{
		wxLogError("Cannot save current contents in file '%s'.", str);
		return;
	}

	saveCurveLine = wxT("");

	// bits
	primeStr = ECCPrimeTextCtrl->GetValue();
	strncpy(buf, (const char*)primeStr.mb_str(wxConvUTF8), 256);
	prime = Big(buf);
	numBits = logb2(prime.fn);
	ellipticCurveComponents.AddLine(wxString(itoa (numBits)), wxTextFileType_Unix);

	// p
	saveCurveLine = ECCPrimeTextCtrl->GetValue();
	ellipticCurveComponents.AddLine(saveCurveLine, wxTextFileType_Unix);

	// A
	saveCurveLine = ECCCoefficientATextCtrl->GetValue();
	ellipticCurveComponents.AddLine(saveCurveLine, wxTextFileType_Unix);

	// B
	saveCurveLine = ECCCoefficientBTextCtrl->GetValue();
	ellipticCurveComponents.AddLine(saveCurveLine, wxTextFileType_Unix);

	// basepoint order
	saveCurveLine = ECCSubgroupOrderTextCtrl->GetValue();
	ellipticCurveComponents.AddLine(saveCurveLine, wxTextFileType_Unix);

	// X
	saveCurveLine = ECCBasePoint_x_CoordinateTextCtrl->GetValue();
	ellipticCurveComponents.AddLine(saveCurveLine, wxTextFileType_Unix);

	// Y
	saveCurveLine = ECCBasePoint_y_CoordinateTextCtrl->GetValue();
	ellipticCurveComponents.AddLine(saveCurveLine, wxTextFileType_Unix);

	// order
	saveCurveLine = ECCCurveOrderTextCtrl->GetValue();
	ellipticCurveComponents.AddLine(saveCurveLine, wxTextFileType_Unix);

	// seed
	saveCurveLine = ECCSeedTextCtrl->GetValue();
	ellipticCurveComponents.AddLine(saveCurveLine, wxTextFileType_Unix);

	// cofactor
	saveCurveLine = ECCSubgroupCofactorTextCtrl->GetValue();
	ellipticCurveComponents.AddLine(saveCurveLine, wxTextFileType_Unix);

	ellipticCurveComponents.Write();
	ellipticCurveComponents.Close();
}

void DiscreteMathCalculatorFrame::OnECCReadCurveParametersButtonPressed(wxCommandEvent& event)
{
   Big p, A, B;
   char buf[256];

	wxString initialDirectory;

	if (event.GetId() == ID_ECC_SAMPLES_MENUITEM)
	{
#ifdef __APPLE__
		wxFileName bundleExamples(
			wxStandardPaths::Get().GetResourcesDir(),
			wxEmptyString);

		bundleExamples.AppendDir("examples");
		bundleExamples.AppendDir("elliptic-curves");

		if (wxDirExists(bundleExamples.GetPath()))
		{
			initialDirectory = bundleExamples.GetPath();
		}
#endif
			wxFileName executable(
			wxStandardPaths::Get().GetExecutablePath());

		wxString directory = executable.GetPath();

		while (initialDirectory.IsEmpty() && !directory.IsEmpty())		{
			wxFileName examplesPath(
					directory, wxEmptyString);

			examplesPath.AppendDir("examples");
			examplesPath.AppendDir("elliptic-curves");

			if (wxDirExists(examplesPath.GetPath()))
			{
					initialDirectory = examplesPath.GetPath();
					break;
			}

			wxFileName parent(directory, wxEmptyString);
			if (parent.GetDirCount() == 0)
				break;

			parent.RemoveLastDir();
			wxString nextDirectory = parent.GetPath();

			if (nextDirectory == directory)
					break;

			directory = nextDirectory;
		}

		if (initialDirectory.IsEmpty())
		{
			wxMessageBox(
					"Cannot locate examples/elliptic-curves.",
					"Elliptic Curve Examples",
					wxOK | wxICON_ERROR,
					this);
			return;
		}
	}

	wxFileDialog inputFileDialog(
		this,
		_("Read ECS file"),
		initialDirectory,
		"",
		"ECS files (*.ecs)|*.ecs",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		wxString curveComponent;

	if (inputFileDialog.ShowModal() == wxID_CANCEL)
		return;

	wxString str = inputFileDialog.GetPath(), inputCurveLine;
	wxTextFile ellipticCurveComponents(inputFileDialog.GetPath());
	miracl *mip = get_mip();

	haveDefaultBase = true;
	defaultBase = 10;
	mip->IOBASE = 10;

	if (!ellipticCurveComponents.Open(str))
	{
		wxLogError("Cannot open current contents in file '%s'.", str);
		return;
	}

	// bits. not used yet
	curveComponent = ellipticCurveComponents.GetFirstLine();

	// p
	curveComponent = ellipticCurveComponents.GetNextLine();
	ECCPrimeTextCtrl->SetValue(curveComponent);
   strncpy(buf, (const char*)curveComponent.mb_str(wxConvUTF8), 255);
   p = Big(buf);

	// A
	curveComponent = ellipticCurveComponents.GetNextLine();
	ECCCoefficientATextCtrl->SetValue(curveComponent);
   strncpy(buf, (const char*)curveComponent.mb_str(wxConvUTF8), 255);
   A = Big(buf);

	// B
	curveComponent = ellipticCurveComponents.GetNextLine();
	ECCCoefficientBTextCtrl->SetValue(curveComponent);
   strncpy(buf, (const char*)curveComponent.mb_str(wxConvUTF8), 255);
   B = Big(buf);

	// basepoint order
	curveComponent = ellipticCurveComponents.GetNextLine();
	ECCSubgroupOrderTextCtrl->SetValue(curveComponent);

	// X
	curveComponent = ellipticCurveComponents.GetNextLine();
	ECCBasePoint_x_CoordinateTextCtrl->SetValue(curveComponent);

	// Y
	curveComponent = ellipticCurveComponents.GetNextLine();
	ECCBasePoint_y_CoordinateTextCtrl->SetValue(curveComponent);

	// order
	curveComponent = ellipticCurveComponents.GetNextLine();
	ECCCurveOrderTextCtrl->SetValue(curveComponent);

	// seed
	curveComponent = ellipticCurveComponents.GetNextLine();
	ECCSeedTextCtrl->SetValue(curveComponent);

	// cofactor
	curveComponent = ellipticCurveComponents.GetNextLine();
	ECCSubgroupCofactorTextCtrl->SetValue(curveComponent);

	ellipticCurveComponents.Close();

   ecurve (A,B,p,MR_AFFINE);   // initialise Elliptic Curve

	haveCurrentEllipticCurve = true;
}

void DiscreteMathCalculatorFrame::OnDiffieHellmanGFMenuItemSelected (wxCommandEvent& event)
{
	long wxExecuteReturnValue, bitsForEncryption;
	wxString empty;
	wxString ellipticCurveSetupProcess, primeStr;
	wxArrayString ellipticCurveSetupResultsStdout, ellipticCurveSetupProcessStderr;
	wxArrayString DiffieHellmanGFProcessStdout, DiffieHellmanGFProcessStderr;
	wxTextEntryDialog textEntryDialog(this, "Prime", "Enter the prime p");

	while (true)
	{
		if (textEntryDialog.ShowModal() == wxID_OK)
			primeStr = textEntryDialog.GetValue();
		else
			return;

		if (primeStr.Length() == 0)
		{
			report(wxT("Designate the number of bits to use for encryption of messages."), wxT("Note"));
			continue;
		}

		if (primeStr.ToLong(&bitsForEncryption) == false)
		{
			report(wxT("The number of bits is not a proper integer."), wxT("Note"));
			continue;
		}

        if ((primeStr > wxT("128")) || (primeStr < wxT("100")))
        {
            report(wxT("The number of bits cannot exceed 128 and must not be less than 100."), wxT("Note"));
            continue;
        }

		break;
	}

#ifdef __APPLE__
wxString resourceDirectory =
    wxStandardPaths::Get().GetResourcesDir();
wxString dataDirectory =
    wxStandardPaths::Get().GetUserDataDir();

wxString setupExecutable =
    resourceDirectory + wxT("/elliptic-curve-setup");

ellipticCurveSetupProcess = wxString::Format(
    wxT("\"%s\" %s \"%s\""),
    setupExecutable.c_str(),
    primeStr.c_str(),
    dataDirectory.c_str());
#else
#ifdef FOR_DISTRIBUTION
ellipticCurveSetupProcess = wxT("../usr/resource/elliptic-curve-setup ") + primeStr;
#else
ellipticCurveSetupProcess = wxT("../../usr/resource/elliptic-curve-setup ") + primeStr;
#endif
#endif

wxBeginBusyCursor();
#ifndef _WIN64
wxExecuteReturnValue = wxExecute (ellipticCurveSetupProcess,
                                  ellipticCurveSetupResultsStdout,
                                  ellipticCurveSetupProcessStderr,
                                  wxEXEC_SYNC|wxEXEC_NODISABLE);
#else
wxExecuteReturnValue = wxExecute (ellipticCurveSetupProcess,
                                  ellipticCurveSetupResultsStdout,
                                  ellipticCurveSetupProcessStderr,
                                  wxEXEC_SYNC|wxEXEC_NOEVENTS);
#endif

#ifdef __APPLE__
wxString aliceExecutable =
    resourceDirectory + wxT("/AliceGF");
wxString bobExecutable =
    resourceDirectory + wxT("/BobGF");

wxExecuteReturnValue = wxExecute(
    wxT("\"") + aliceExecutable + wxT("\""),
    wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE);

wxExecuteReturnValue = wxExecute(
    wxT("\"") + bobExecutable + wxT("\""),
    wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE);
#else
#ifdef FOR_DISTRIBUTION
wxExecuteReturnValue = wxExecute(wxT("../usr/resource/AliceGF"),
                                 wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE);
wxExecuteReturnValue = wxExecute(wxT("../usr/resource/BobGF"),
                                 wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE);
#else
wxExecuteReturnValue = wxExecute(wxT("../../usr/resource/AliceGF"),
                                 wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE);
wxExecuteReturnValue = wxExecute(wxT("../../usr/resource/BobGF"),
                                 wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE);
#endif
#endif

	//DiffieHellmanGFProcessStderr = runApplication (wxT ("AliceGF"), &empty, 1, false);
	//DiffieHellmanGFProcessStdout = runApplication (wxT ("BobGF"), &empty, 1, false);
	wxEndBusyCursor();
}

void DiscreteMathCalculatorFrame::
OnCreateTypesOfDirectedGraphsExamplesMenuItemSelected(wxCommandEvent& event)
{
    wxString examplesDirectory =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/graph-files/types-of-directed-graphs-digraph-digraphw");

    wxDirDialog dialog(
        this,
        wxT("Single click the type of directed graph you want to see."),
        examplesDirectory,
        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST
    );

    if (dialog.ShowModal() != wxID_OK)
        return;

    wxString selectedDirectory = dialog.GetPath();

    wxString graphName = selectedDirectory.AfterLast('/');

    // Special case: display original and transpose simultaneously.
    if (graphName == wxT("transpose"))
    {
        wxString original =
            selectedDirectory + wxT("/original.gml");

        wxString transpose =
            selectedDirectory + wxT("/transpose.gml");

        if (!wxFileExists(original) || !wxFileExists(transpose))
        {
            wxMessageBox(
                wxT("One or both transpose graph files are missing."),
                wxT("Directed Graph Examples"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        GraphExampleFrame* originalFrame =
            new GraphExampleFrame(
                this,
                wxT("Original Directed Graph"),
                original,
                wxPoint(100, 100)
            );

        GraphExampleFrame* transposeFrame =
            new GraphExampleFrame(
                this,
                wxT("Transpose Directed Graph"),
                transpose,
                wxPoint(780, 100)
            );

        originalFrame->Show();
        transposeFrame->Show();

        return;
    }

    // Find the first GML file in the selected directory.
    wxDir directory(selectedDirectory);

    wxString filename;

    if (!directory.IsOpened() ||
        !directory.GetFirst(&filename, wxT("*.gml"), wxDIR_FILES))
    {
        wxMessageBox(
            wxT("No GML graph file was found in this directory."),
            wxT("Directed Graph Examples"),
            wxOK | wxICON_INFORMATION,
            this
        );
        return;
    }

    filename = selectedDirectory + wxT("/") + filename;

    // Choose an appropriate layout.
    GraphLayout graphLayout = circular;
	if (graphName == wxT("parallel-edges"))
	    graphLayout = hierarchical;
    if (graphName == wxT("acyclic"))
        graphLayout = hierarchical;
    else if (graphName == wxT("strongly-connected-components"))
        graphLayout = forcedirected;
    else if (graphName == wxT("weakly-connected"))
        graphLayout = forcedirected;

    GraphType graphType = digraph;

    bool allowParallelEdges =
        (graphName == wxT("parallel-edges"));
	 saveGraphPreview(false);
	 graphsAllowParallelEdgesCheckBox->SetValue(allowParallelEdges);
    if (myGraph != nullptr)
    {
        delete myGraph;
        myGraph = nullptr;
    }

    myGraph = new BuildGraph(
        wxT(""),
        graphType,
        graphLayout,
        true,
        allowParallelEdges
    );

    myGraph->setDrawingCanvas(previewWindow);

    GraphType loadedGraphType = graphType;
    bool parallelEdges = allowParallelEdges;

    myGraph->buildFromGML(
        filename,
        &loadedGraphType,
        &parallelEdges,
        false
    );
	// Update the graph type controls to match the loaded GML file.
	graphsGraphTypeRadioBox->SetSelection(
    (loadedGraphType == digraph || loadedGraphType == digraphw) ? 1 : 0
	);	
	graphsHideEdgesCheckBox->SetValue(false);
	graphsAllowParallelEdgesCheckBox->SetValue(parallelEdges);
	
previewWindow->setGA(myGraph->getGraphAttributes());

    // Update all graph shapes before refreshing.
    previewWindow->getSFDiagramManager()->UpdateAll();

    previewWindow->RefreshCanvas(
        true,
        previewWindow->GetRect()
    );

    // Open the Strongly Connected Components PNG illustration.
    if (graphName == wxT("strongly-connected-components"))
    {
        wxString pngFile =
            selectedDirectory +
            wxT("/strongly-connected-components.png");

        if (!wxFileExists(pngFile))
        {
            wxMessageBox(
                wxT("Illustration not found:\n") + pngFile
            );
        }
        else if (!wxLaunchDefaultApplication(pngFile))
        {
            wxMessageBox(
                wxT("Unable to open illustration:\n") + pngFile
            );
        }
    }
}

// BEGIN DIRECTED CYCLE DETECTION HANDLER
void DiscreteMathCalculatorFrame::
OnCreateDirectedGraphAlgorithmsExamplesMenuItemSelected(wxCommandEvent& event)
{
    wxString examplesDirectory =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/graph-files/algorithms-digraph");

    wxDirDialog dialog(this,
        wxT("Select a directed graph algorithm example."),
        examplesDirectory,
        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    if (dialog.ShowModal() != wxID_OK)
        return;

    wxString algorithmName = dialog.GetPath().AfterLast('/');
    // BEGIN NP-COMPLETE DIGRAPH CHOOSER
    if (algorithmName == wxT("NP-complete"))
    {
        wxArrayString choices;
        choices.Add(wxT("Directed Hamiltonian Cycle (Exact)"));
        choices.Add(wxT("Directed Hamiltonian Path (Exact)"));
        choices.Add(wxT("Directed Feedback Vertex Set (Exact)"));
        choices.Add(wxT("Directed Feedback Arc Set (Exact)"));
        choices.Add(wxT("Directed Dominating Set (Exact)"));

        wxSingleChoiceDialog algorithmDialog(
            this,
            wxT("Choose an NP-complete directed graph algorithm:"),
            wxT("NP-complete Directed Graph Algorithms"),
            choices);
        if (algorithmDialog.ShowModal() != wxID_OK)
            return;

        switch (algorithmDialog.GetSelection())
        {
            case 0:
                algorithmName = wxT("directed-hamiltonian-cycle");
                break;
            case 1:
                algorithmName = wxT("directed-hamiltonian-path");
                break;
            case 2:
                algorithmName = wxT("directed-feedback-vertex-set");
                break;
            case 3:
                algorithmName = wxT("directed-feedback-arc-set");
                break;
            case 4:
                algorithmName = wxT("directed-dominating-set");
                break;
            default:
                return;
        }
    }
    // END NP-COMPLETE DIGRAPH CHOOSER

    // BEGIN DIRECTED DOMINATING SET
    if (algorithmName == wxT("directed-dominating-set"))
    {
        wxTextEntryDialog inputDialog(this,
            wxT("Enter vertex count (1 to 20), then directed edges 'from to', one per line.\n")
            wxT("Out-dominating convention: selected vertices cover themselves and their outgoing neighbors."),
            wxT("Directed Dominating Set (Exact)"), wxEmptyString,
            wxOK | wxCANCEL | wxTE_MULTILINE);
        inputDialog.SetSize(wxSize(660, 540));
        if (inputDialog.ShowModal() != wxID_OK) return;

        std::istringstream input(std::string(inputDialog.GetValue().mb_str()));
        int n = 0;
        if (!(input >> n) || n < 1 || n > 20)
        {
            wxMessageBox(wxT("Vertex count must be between 1 and 20."),
                wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }
        std::vector<std::pair<int, int> > edges;
        std::vector<unsigned int> cover(n, 0);
        for (int i = 0; i < n; ++i) cover[i] = (1u << i);
        int a, b;
        while (input >> a)
        {
            if (!(input >> b) || a < 1 || a > n || b < 1 || b > n)
            {
                wxMessageBox(wxT("Each edge must have two vertex numbers between 1 and n."),
                    wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
                return;
            }
            --a; --b;
            edges.push_back(std::make_pair(a, b));
            cover[a] |= (1u << b);
        }
        if (!input.eof())
        {
            wxMessageBox(wxT("Invalid edge input."),
                wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }

        const unsigned int all = (1u << n) - 1u;
        unsigned int best = all;
        int bestSize = n + 1;
        // Exhaustive search with cardinality pruning: exact for n <= 20.
        for (unsigned int subset = 0; subset <= all; ++subset)
        {
            int count = 0;
            unsigned int tmp = subset;
            while (tmp) { tmp &= tmp - 1u; ++count; }
            if (count >= bestSize) continue;
            unsigned int dominated = 0;
            for (int v = 0; v < n; ++v)
                if (subset & (1u << v)) dominated |= cover[v];
            if (dominated == all) { best = subset; bestSize = count; }
        }

        wxString result = wxString::Format(
            wxT("DIRECTED DOMINATING SET (EXACT)\n\nMinimum dominating set size: %d\nDominating vertices:"),
            bestSize);
        for (int v = 0; v < n; ++v)
            if (best & (1u << v)) result += wxString::Format(wxT(" %d"), v + 1);
        result += wxT("\n\nRed vertices belong to the minimum dominating set.\n")
                  wxT("A vertex is dominated if it is selected or has an incoming edge from a selected vertex.");

        wxDialog output(this, wxID_ANY, wxT("Directed Dominating Set (Exact)"),
                        wxDefaultPosition, wxSize(850, 720),
                        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* report = new wxTextCtrl(&output, wxID_ANY, result,
            wxDefaultPosition, wxSize(-1, 140),
            wxTE_MULTILINE | wxTE_READONLY);
        layout->Add(report, 0, wxEXPAND | wxALL, 8);
        wxPanel* drawing = new wxPanel(&output, wxID_ANY);
        drawing->SetBackgroundStyle(wxBG_STYLE_PAINT);
        drawing->Bind(wxEVT_PAINT, [n, edges, best, drawing](wxPaintEvent&)
        {
            wxPaintDC dc(drawing);
            dc.SetBackground(*wxWHITE_BRUSH);
            dc.Clear();
            wxSize sz = drawing->GetClientSize();
            const double pi = 3.14159265358979323846;
            const double cx = sz.x / 2.0, cy = sz.y / 2.0;
            const double radius = std::max(45.0, std::min(sz.x, sz.y) * 0.39);
            std::vector<wxPoint> pts;
            for (int i = 0; i < n; ++i)
            {
                double angle = -pi / 2.0 + 2.0 * pi * i / n;
                pts.push_back(wxPoint(int(cx + radius * std::cos(angle)),
                                      int(cy + radius * std::sin(angle))));
            }
            for (size_t e = 0; e < edges.size(); ++e)
            {
                int u = edges[e].first, v = edges[e].second;
                dc.SetPen(wxPen(wxColour(110, 120, 130), 1));
                if (u == v)
                {
                    dc.SetBrush(*wxTRANSPARENT_BRUSH);
                    dc.DrawCircle(pts[u].x + 20, pts[u].y - 20, 12);
                    continue;
                }
                double dx = pts[v].x - pts[u].x;
                double dy = pts[v].y - pts[u].y;
                double len = std::sqrt(dx * dx + dy * dy);
                if (len < 0.001) continue;
                double ux = dx / len, uy = dy / len;
                double off = 5.0;
                double x1 = pts[u].x + ux * 18 - uy * off;
                double y1 = pts[u].y + uy * 18 + ux * off;
                double x2 = pts[v].x - ux * 20 - uy * off;
                double y2 = pts[v].y - uy * 20 + ux * off;
                dc.DrawLine(int(x1), int(y1), int(x2), int(y2));
                dc.DrawLine(int(x2), int(y2), int(x2 - ux * 11 - uy * 5),
                            int(y2 - uy * 11 + ux * 5));
                dc.DrawLine(int(x2), int(y2), int(x2 - ux * 11 + uy * 5),
                            int(y2 - uy * 11 - ux * 5));
            }
            for (int i = 0; i < n; ++i)
            {
                bool selected = (best & (1u << i)) != 0;
                dc.SetPen(*wxBLACK_PEN);
                dc.SetBrush(wxBrush(selected ? wxColour(220, 65, 65)
                                             : wxColour(250, 217, 91)));
                dc.DrawCircle(pts[i], 16);
                dc.SetTextForeground(selected ? *wxWHITE : *wxBLACK);
                wxString label = wxString::Format(wxT("%d"), i + 1);
                wxSize extent = dc.GetTextExtent(label);
                dc.DrawText(label, pts[i].x - extent.x / 2,
                            pts[i].y - extent.y / 2);
            }
        });
        layout->Add(drawing, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
        wxButton* closeButton = new wxButton(&output, wxID_OK, wxT("Close"));
        layout->Add(closeButton, 0, wxALIGN_RIGHT | wxALL, 8);
        output.SetSizer(layout);
        output.ShowModal();
        return;
    }
    // END DIRECTED DOMINATING SET

    // BEGIN DIRECTED FEEDBACK ARC SET
    if (algorithmName == wxT("directed-feedback-arc-set"))
    {
        wxTextEntryDialog inputDialog(this,
            wxT("Enter vertex count (1 to 16), then directed edges as 'from to', one per line.\n")
            wxT("Finds a MINIMUM set of directed edges to remove to make a DAG."),
            wxT("Directed Feedback Arc Set (Exact)"), wxEmptyString,
            wxOK | wxCANCEL | wxTE_MULTILINE);
        inputDialog.SetSize(wxSize(660, 540));
        if (inputDialog.ShowModal() != wxID_OK) return;
        std::istringstream input(std::string(inputDialog.GetValue().mb_str()));
        int n = 0;
        if (!(input >> n) || n < 1 || n > 16)
        {
            wxMessageBox(wxT("Vertex count must be between 1 and 16."),
                wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }
        std::vector<std::vector<int> > adj(n, std::vector<int>(n, 0));
        std::string line;
        std::getline(input, line);
        int lineNumber = 1;
        while (std::getline(input, line))
        {
            ++lineNumber;
            if (line.find_first_not_of(" \t\r") == std::string::npos) continue;
            std::istringstream row(line);
            int a = 0, b = 0;
            std::string extra;
            if (!(row >> a >> b) || (row >> extra) ||
                a < 1 || a > n || b < 1 || b > n)
            {
                wxMessageBox(wxString::Format(
                    wxT("Invalid edge on line %d. Use: from to"), lineNumber),
                    wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
                return;
            }
            adj[a - 1][b - 1] = 1; // Duplicate edges are treated as one arc.
        }
        std::vector<std::pair<int,int> > edges;
        for (int a = 0; a < n; ++a)
            for (int b = 0; b < n; ++b)
                if (adj[a][b]) edges.push_back(std::make_pair(a,b));

        // Exact subset DP: dp[mask] = minimum backward arcs in an
        // ordering of vertices in mask. Appending v incurs arcs v -> u
        // for all u already in mask. Self-loops must always be removed.
        const int count = 1 << n;
        const int infinity = 1000000000;
        std::vector<int> dp(count, infinity), previous(count, -1);
        dp[0] = 0;
        for (int mask = 1; mask < count; ++mask)
        {
            for (int v = 0; v < n; ++v)
            {
                if (!(mask & (1 << v))) continue;
                const int rest = mask ^ (1 << v);
                int cost = 0;
                for (int u = 0; u < n; ++u)
                    if ((rest & (1 << u)) && adj[v][u]) ++cost;
                const int candidate = dp[rest] + cost;
                if (candidate < dp[mask])
                {
                    dp[mask] = candidate;
                    previous[mask] = v;
                }
            }
        }
        std::vector<int> order;
        for (int mask = count - 1; mask; )
        {
            int v = previous[mask];
            order.push_back(v);
            mask ^= 1 << v;
        }
        std::reverse(order.begin(), order.end());
        std::vector<int> position(n);
        for (int i = 0; i < n; ++i) position[order[i]] = i;
        std::vector<std::pair<int,int> > removed;
        for (size_t e = 0; e < edges.size(); ++e)
        {
            const int a = edges[e].first, b = edges[e].second;
            if (a == b || position[a] > position[b]) removed.push_back(edges[e]);
        }
        wxString report = wxT("DIRECTED FEEDBACK ARC SET (EXACT)\n\n");
        report += wxString::Format(wxT("Vertices: %d   Edges: %lu\n"),
            n, static_cast<unsigned long>(edges.size()));
        report += wxString::Format(wxT("Minimum arcs to remove: %lu\n\n"),
            static_cast<unsigned long>(removed.size()));
        report += wxT("Arcs to remove:\n");
        if (removed.empty()) report += wxT("None (already acyclic).\n");
        for (size_t i = 0; i < removed.size(); ++i)
            report += wxString::Format(wxT("%d -> %d\n"),
                removed[i].first + 1, removed[i].second + 1);
        report += wxT("\nTopological ordering after removal:\n");
        for (int i = 0; i < n; ++i)
        {
            if (i) report += wxT(" -> ");
            report += wxString::Format(wxT("%d"), order[i] + 1);
        }
        report += wxT("\n");

        wxDialog output(this, wxID_ANY, wxT("Directed Feedback Arc Set (Exact)"),
            wxDefaultPosition, wxSize(940, 740),
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* results = new wxTextCtrl(&output, wxID_ANY, report,
            wxDefaultPosition, wxSize(-1, 220),
            wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
        layout->Add(results, 0, wxEXPAND | wxALL, 8);
        wxPanel* drawing = new wxPanel(&output, wxID_ANY,
            wxDefaultPosition, wxSize(-1, 400));
        drawing->SetBackgroundColour(*wxWHITE);
        drawing->Bind(wxEVT_PAINT,
            [n, edges, removed, drawing](wxPaintEvent&)
            {
                wxPaintDC dc(drawing);
                dc.SetBackground(*wxWHITE_BRUSH);
                dc.Clear();
                const wxSize sz = drawing->GetClientSize();
                const double pi = 3.14159265358979323846;
                const double cx = sz.x / 2.0, cy = sz.y / 2.0;
                const double radius = std::max(35.0,
                    std::min(sz.x, sz.y) * 0.40);
                std::vector<wxPoint> pts;
                for (int i = 0; i < n; ++i)
                {
                    const double theta = -pi / 2 + 2 * pi * i / n;
                    pts.push_back(wxPoint(int(cx + radius * std::cos(theta)),
                                          int(cy + radius * std::sin(theta))));
                }
                for (size_t e = 0; e < edges.size(); ++e)
                {
                    const int a = edges[e].first, b = edges[e].second;
                    const bool cut = std::find(removed.begin(), removed.end(),
                                               edges[e]) != removed.end();
                    dc.SetPen(wxPen(cut ? wxColour(210, 35, 35)
                                        : wxColour(115, 125, 135), cut ? 3 : 1));
                    if (a == b)
                    {
                        dc.SetBrush(*wxTRANSPARENT_BRUSH);
                        dc.DrawCircle(pts[a].x + 17, pts[a].y - 17, 11);
                        continue;
                    }
                    const double dx = pts[b].x - pts[a].x;
                    const double dy = pts[b].y - pts[a].y;
                    const double len = std::sqrt(dx*dx + dy*dy);
                    if (len < 0.001) continue;
                    const double ux = dx / len, uy = dy / len;
                    const double offset = 5.0;
                    const double x1 = pts[a].x + ux*18 - uy*offset;
                    const double y1 = pts[a].y + uy*18 + ux*offset;
                    const double x2 = pts[b].x - ux*20 - uy*offset;
                    const double y2 = pts[b].y - uy*20 + ux*offset;
                    dc.DrawLine(int(x1), int(y1), int(x2), int(y2));
                    dc.DrawLine(int(x2), int(y2),
                        int(x2 - ux*11 - uy*5), int(y2 - uy*11 + ux*5));
                    dc.DrawLine(int(x2), int(y2),
                        int(x2 - ux*11 + uy*5), int(y2 - uy*11 - ux*5));
                }
                for (int i = 0; i < n; ++i)
                {
                    dc.SetPen(*wxBLACK_PEN);
                    dc.SetBrush(wxBrush(wxColour(250, 217, 91)));
                    dc.DrawCircle(pts[i], 16);
                    dc.SetTextForeground(*wxBLACK);
                    const wxString label = wxString::Format(wxT("%d"), i+1);
                    const wxSize ext = dc.GetTextExtent(label);
                    dc.DrawText(label, pts[i].x - ext.x/2, pts[i].y - ext.y/2);
                }
            });
        layout->Add(drawing, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
        layout->Add(new wxStaticText(&output, wxID_ANY,
            wxT("Red arrows = removed arcs; gray arrows = retained arcs.")),
            0, wxLEFT | wxRIGHT | wxBOTTOM, 8);
        wxButton* closeButton = new wxButton(&output, wxID_OK, wxT("Close"));
        layout->Add(closeButton, 0, wxALIGN_RIGHT | wxALL, 8);
        output.SetSizer(layout);
        output.ShowModal();
        return;
    }
    // END DIRECTED FEEDBACK ARC SET

    // BEGIN DIRECTED FEEDBACK VERTEX SET
    if (algorithmName == wxT("directed-feedback-vertex-set"))
    {
        wxTextEntryDialog inputDialog(this,
            wxT("Enter the number of vertices (1 to 16), followed by directed edges,\n")
            wxT("one 'from to' pair per line. Vertices are numbered 1 through n.\n")
            wxT("Finds a MINIMUM set of vertices whose removal makes the graph acyclic."),
            wxT("Directed Feedback Vertex Set (Exact)"), wxEmptyString,
            wxOK | wxCANCEL | wxTE_MULTILINE);
        inputDialog.SetSize(wxSize(660, 540));
        if (inputDialog.ShowModal() != wxID_OK)
            return;

        std::istringstream input(std::string(inputDialog.GetValue().mb_str()));
        int n = 0;
        if (!(input >> n) || n < 1 || n > 16)
        {
            wxMessageBox(wxT("Vertex count must be between 1 and 16."),
                wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }
        std::vector<std::vector<int> > adj(n, std::vector<int>(n, 0));
        std::string line;
        std::getline(input, line);
        int lineNumber = 1;
        while (std::getline(input, line))
        {
            ++lineNumber;
            if (line.find_first_not_of(" \t\r") == std::string::npos)
                continue;
            std::istringstream row(line);
            int a = 0, b = 0;
            std::string extra;
            if (!(row >> a >> b) || (row >> extra) ||
                a < 1 || a > n || b < 1 || b > n)
            {
                wxMessageBox(wxString::Format(
                    wxT("Invalid directed edge on line %d. Use: from to"),
                    lineNumber), wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
                return;
            }
            adj[a - 1][b - 1] = 1; // Self-loops matter for feedback sets.
        }
        std::vector<std::pair<int, int> > edges;
        for (int a = 0; a < n; ++a)
            for (int b = 0; b < n; ++b)
                if (adj[a][b]) edges.push_back(std::make_pair(a, b));

        // Exhaustively enumerate removed-vertex sets (n <= 16).
        // For each set, Kahn's algorithm checks whether the remaining graph is a DAG.
        const unsigned int limit = 1u << n;
        unsigned int bestMask = limit - 1u;
        int bestSize = n;
        unsigned long long examined = 0;
        for (unsigned int mask = 0; mask < limit; ++mask)
        {
            int removed = 0;
            for (int v = 0; v < n; ++v)
                if (mask & (1u << v)) ++removed;
            if (removed >= bestSize) continue;
            ++examined;
            std::vector<int> indegree(n, 0);
            for (int a = 0; a < n; ++a)
                if (!(mask & (1u << a)))
                    for (int b = 0; b < n; ++b)
                        if (!(mask & (1u << b)) && adj[a][b])
                            ++indegree[b];
            std::vector<int> queue;
            for (int v = 0; v < n; ++v)
                if (!(mask & (1u << v)) && indegree[v] == 0)
                    queue.push_back(v);
            int visited = 0;
            for (size_t head = 0; head < queue.size(); ++head)
            {
                const int a = queue[head];
                ++visited;
                for (int b = 0; b < n; ++b)
                    if (!(mask & (1u << b)) && adj[a][b])
                        if (--indegree[b] == 0) queue.push_back(b);
            }
            if (visited == n - removed)
            {
                bestSize = removed;
                bestMask = mask;
                if (bestSize == 0) break;
            }
        }
        wxString report = wxT("MINIMUM DIRECTED FEEDBACK VERTEX SET (EXACT)\n\n");
        report += wxString::Format(wxT("Vertices: %d   Directed edges: %lu\n"),
            n, static_cast<unsigned long>(edges.size()));
        report += wxString::Format(wxT("Candidate sets tested: %llu\n"), examined);
        report += wxString::Format(wxT("Minimum vertices to remove: %d\n"), bestSize);
        report += wxT("Remove vertices: ");
        if (bestSize == 0) report += wxT("none (already acyclic)");
        else
        {
            bool first = true;
            for (int v = 0; v < n; ++v)
                if (bestMask & (1u << v))
                {
                    if (!first) report += wxT(", ");
                    report += wxString::Format(wxT("%d"), v + 1);
                    first = false;
                }
        }
        report += wxT("\nRed vertices are the minimum removal set.\n")
                  wxT("Gray arrows touch a removed vertex; dark arrows remain.\n");

        wxDialog output(this, wxID_ANY,
            wxT("Directed Feedback Vertex Set (Exact)"),
            wxDefaultPosition, wxSize(940, 740),
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* results = new wxTextCtrl(&output, wxID_ANY, report,
            wxDefaultPosition, wxSize(-1, 180),
            wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
        layout->Add(results, 0, wxEXPAND | wxALL, 8);
        wxPanel* drawing = new wxPanel(&output, wxID_ANY,
            wxDefaultPosition, wxSize(-1, 400));
        drawing->SetBackgroundColour(*wxWHITE);
        drawing->Bind(wxEVT_PAINT,
            [n, edges, bestMask, drawing](wxPaintEvent&)
            {
                wxPaintDC dc(drawing);
                dc.SetBackground(*wxWHITE_BRUSH);
                dc.Clear();
                const wxSize sz = drawing->GetClientSize();
                const double pi = 3.14159265358979323846;
                const double cx = sz.x / 2.0, cy = sz.y / 2.0;
                const double radius = std::max(35.0,
                    std::min(sz.x, sz.y) * 0.39);
                std::vector<wxPoint> pts;
                for (int v = 0; v < n; ++v)
                {
                    const double angle = -pi / 2 + 2 * pi * v / n;
                    pts.push_back(wxPoint(
                        int(cx + radius * std::cos(angle)),
                        int(cy + radius * std::sin(angle))));
                }
                for (size_t e = 0; e < edges.size(); ++e)
                {
                    const int a = edges[e].first, b = edges[e].second;
                    const bool removed = (bestMask & (1u << a)) ||
                                         (bestMask & (1u << b));
                    dc.SetPen(wxPen(removed ? wxColour(185, 190, 195)
                                            : wxColour(55, 75, 95),
                                    removed ? 1 : 2));
                    if (a == b)
                    {
                        const int x = pts[a].x, y = pts[a].y;
                        dc.SetBrush(*wxTRANSPARENT_BRUSH);
                        dc.DrawCircle(x + 17, y - 17, 11);
                        continue;
                    }
                    const double dx = pts[b].x - pts[a].x;
                    const double dy = pts[b].y - pts[a].y;
                    const double len = std::sqrt(dx * dx + dy * dy);
                    if (len < 0.001) continue;
                    const double ux = dx / len, uy = dy / len;
                    const double offset = 5.0;
                    const double x1 = pts[a].x + ux * 18 - uy * offset;
                    const double y1 = pts[a].y + uy * 18 + ux * offset;
                    const double x2 = pts[b].x - ux * 20 - uy * offset;
                    const double y2 = pts[b].y - uy * 20 + ux * offset;
                    dc.DrawLine(int(x1), int(y1), int(x2), int(y2));
                    dc.DrawLine(int(x2), int(y2),
                        int(x2 - ux * 11 - uy * 5),
                        int(y2 - uy * 11 + ux * 5));
                    dc.DrawLine(int(x2), int(y2),
                        int(x2 - ux * 11 + uy * 5),
                        int(y2 - uy * 11 - ux * 5));
                }
                for (int v = 0; v < n; ++v)
                {
                    const bool removed = (bestMask & (1u << v)) != 0;
                    dc.SetPen(*wxBLACK_PEN);
                    dc.SetBrush(wxBrush(removed ? wxColour(235, 95, 95)
                                                : wxColour(250, 217, 91)));
                    dc.DrawCircle(pts[v], 16);
                    dc.SetTextForeground(*wxBLACK);
                    wxString label = wxString::Format(wxT("%d"), v + 1);
                    wxSize extent = dc.GetTextExtent(label);
                    dc.DrawText(label, pts[v].x - extent.x / 2,
                        pts[v].y - extent.y / 2);
                }
            });
        layout->Add(drawing, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
        wxButton* closeButton = new wxButton(&output, wxID_OK, wxT("Close"));
        layout->Add(closeButton, 0, wxALIGN_RIGHT | wxALL, 8);
        output.SetSizer(layout);
        output.ShowModal();
        return;
    }
    // END DIRECTED FEEDBACK VERTEX SET

    // BEGIN DIRECTED HAMILTONIAN PATH
    if (algorithmName == wxT("directed-hamiltonian-path"))
    {
        wxTextEntryDialog inputDialog(this,
            wxT("Enter vertex count (2 to 20), followed by directed edges,\n")
            wxT("one 'from to' pair per line. Vertices are numbered 1 through n.\n")
            wxT("Exact backtracking with a search limit of 5 million states."),
            wxT("Directed Hamiltonian Path (Exact)"), wxEmptyString,
            wxOK | wxCANCEL | wxTE_MULTILINE);
        inputDialog.SetSize(wxSize(660, 540));
        if (inputDialog.ShowModal() != wxID_OK)
            return;

        std::istringstream input(std::string(inputDialog.GetValue().mb_str()));
        int n = 0;
        if (!(input >> n) || n < 2 || n > 20)
        {
            wxMessageBox(wxT("Vertex count must be between 2 and 20."),
                wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }
        std::vector<std::vector<int> > adj(n, std::vector<int>(n, 0));
        std::string line;
        std::getline(input, line); // Consume remainder of vertex-count line.
        int lineNumber = 1;
        while (std::getline(input, line))
        {
            ++lineNumber;
            if (line.find_first_not_of(" \t\r") == std::string::npos)
                continue;
            std::istringstream row(line);
            int a = 0, b = 0;
            std::string extra;
            if (!(row >> a >> b) || (row >> extra) ||
                a < 1 || a > n || b < 1 || b > n)
            {
                wxMessageBox(wxString::Format(
                    wxT("Invalid directed edge on line %d. Use: from to"),
                    lineNumber), wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
                return;
            }
            if (a != b) adj[a - 1][b - 1] = 1;
        }

        std::vector<std::pair<int, int> > edges;
        for (int a = 0; a < n; ++a)
            for (int b = 0; b < n; ++b)
                if (adj[a][b])
                    edges.push_back(std::make_pair(a, b));
        // A path can start at any vertex. Try each start vertex in turn.
        std::vector<int> path;
        std::vector<int> used(n, 0);
        const unsigned long long maxStates = 5000000ULL;
        unsigned long long states = 0;
        bool exhausted = false;
        std::function<bool(int)> search = [&](int v) -> bool
        {
            if (++states > maxStates)
            {
                exhausted = true;
                return false;
            }
            if (static_cast<int>(path.size()) == n)
                return true;
            for (int next = 0; next < n; ++next)
            {
                if (!adj[v][next] || used[next]) continue;
                used[next] = 1;
                path.push_back(next);
                if (search(next)) return true;
                path.pop_back();
                used[next] = 0;
                if (exhausted) return false;
            }
            return false;
        };
        bool found = false;
        for (int start = 0; start < n && !found && !exhausted; ++start)
        {
            path.assign(1, start);
            std::fill(used.begin(), used.end(), 0);
            used[start] = 1;
            found = search(start);
        }
        wxString report = wxT("DIRECTED HAMILTONIAN PATH (EXACT BACKTRACKING)\n\n");
        report += wxString::Format(wxT("Vertices: %d   Directed edges: %lu\n"),
            n, static_cast<unsigned long>(edges.size()));
        report += wxString::Format(wxT("Search states examined: %llu\n\n"), states);
        if (found)
        {
            report += wxT("HAMILTONIAN PATH FOUND\n\n");
            for (size_t k = 0; k < path.size(); ++k)
            {
                if (k) report += wxT(" -> ");
                report += wxString::Format(wxT("%d"), path[k] + 1);
            }
            report += wxT("\n");
        }
        else if (exhausted)
            report += wxT("SEARCH LIMIT REACHED. Existence is UNDETERMINED.\n")
                      wxT("No conclusion can be drawn from this incomplete search.\n");
        else
            report += wxT("NO DIRECTED HAMILTONIAN PATH EXISTS.\n");

        wxDialog output(this, wxID_ANY,
            wxT("Directed Hamiltonian Path (Exact)"),
            wxDefaultPosition, wxSize(940, 740),
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* results = new wxTextCtrl(&output, wxID_ANY, report,
            wxDefaultPosition, wxSize(-1, 190),
            wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
        layout->Add(results, 0, wxEXPAND | wxALL, 8);
        wxPanel* drawing = new wxPanel(&output, wxID_ANY,
            wxDefaultPosition, wxSize(-1, 400));
        drawing->SetBackgroundColour(*wxWHITE);
        drawing->Bind(wxEVT_PAINT,
            [n, edges, path, found, drawing](wxPaintEvent&)
            {
                wxPaintDC dc(drawing);
                dc.SetBackground(*wxWHITE_BRUSH);
                dc.Clear();
                const wxSize sz = drawing->GetClientSize();
                const double pi = 3.14159265358979323846;
                const double cx = sz.x / 2.0, cy = sz.y / 2.0;
                const double radius = std::max(35.0,
                    std::min(sz.x, sz.y) * 0.40);
                std::vector<wxPoint> pts;
                for (int i = 0; i < n; ++i)
                {
                    double angle = -pi / 2 + 2 * pi * i / n;
                    pts.push_back(wxPoint(
                        int(cx + radius * std::cos(angle)),
                        int(cy + radius * std::sin(angle))));
                }
                for (size_t e = 0; e < edges.size(); ++e)
                {
                    const int a = edges[e].first, b = edges[e].second;
                    bool highlighted = false;
                    if (found)
                        for (size_t k = 1; k < path.size(); ++k)
                            if (path[k - 1] == a && path[k] == b)
                                highlighted = true;
                    dc.SetPen(wxPen(highlighted ? wxColour(210, 35, 35)
                                                   : wxColour(125, 135, 145),
                                    highlighted ? 3 : 1));
                    const double dx = pts[b].x - pts[a].x;
                    const double dy = pts[b].y - pts[a].y;
                    const double len = std::sqrt(dx * dx + dy * dy);
                    if (len < 0.001) continue;
                    const double ux = dx / len, uy = dy / len;
                    // Opposite arcs receive opposite visual offsets.
                    const double offset = 5.0;
                    const double x1 = pts[a].x + ux * 18 - uy * offset;
                    const double y1 = pts[a].y + uy * 18 + ux * offset;
                    const double x2 = pts[b].x - ux * 20 - uy * offset;
                    const double y2 = pts[b].y - uy * 20 + ux * offset;
                    dc.DrawLine(int(x1), int(y1), int(x2), int(y2));
                    dc.DrawLine(int(x2), int(y2),
                        int(x2 - ux * 11 - uy * 5),
                        int(y2 - uy * 11 + ux * 5));
                    dc.DrawLine(int(x2), int(y2),
                        int(x2 - ux * 11 + uy * 5),
                        int(y2 - uy * 11 - ux * 5));
                }
                for (int i = 0; i < n; ++i)
                {
                    dc.SetPen(*wxBLACK_PEN);
                    dc.SetBrush(wxBrush(wxColour(250, 217, 91)));
                    dc.DrawCircle(pts[i], 16);
                    dc.SetTextForeground(*wxBLACK);
                    wxString label = wxString::Format(wxT("%d"), i + 1);
                    wxSize extent = dc.GetTextExtent(label);
                    dc.DrawText(label, pts[i].x - extent.x / 2,
                        pts[i].y - extent.y / 2);
                }
            });
        layout->Add(drawing, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
        wxButton* closeButton = new wxButton(&output, wxID_OK, wxT("Close"));
        layout->Add(closeButton, 0, wxALIGN_RIGHT | wxALL, 8);
        output.SetSizer(layout);
        output.ShowModal();
        return;
    }
    // END DIRECTED HAMILTONIAN PATH

    // BEGIN DIRECTED HAMILTONIAN CYCLE
    if (algorithmName == wxT("directed-hamiltonian-cycle"))
    {
        wxTextEntryDialog inputDialog(this,
            wxT("Enter vertex count (2 to 20), followed by directed edges,\n")
            wxT("one 'from to' pair per line. Vertices are numbered 1 through n.\n")
            wxT("Exact backtracking with a search limit of 5 million states."),
            wxT("Directed Hamiltonian Cycle (Exact)"), wxEmptyString,
            wxOK | wxCANCEL | wxTE_MULTILINE);
        inputDialog.SetSize(wxSize(660, 540));
        if (inputDialog.ShowModal() != wxID_OK)
            return;

        std::istringstream input(std::string(inputDialog.GetValue().mb_str()));
        int n = 0;
        if (!(input >> n) || n < 2 || n > 20)
        {
            wxMessageBox(wxT("Vertex count must be between 2 and 20."),
                wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }
        std::vector<std::vector<int> > adj(n, std::vector<int>(n, 0));
        std::string line;
        std::getline(input, line); // Consume remainder of vertex-count line.
        int lineNumber = 1;
        while (std::getline(input, line))
        {
            ++lineNumber;
            if (line.find_first_not_of(" \t\r") == std::string::npos)
                continue;
            std::istringstream row(line);
            int a = 0, b = 0;
            std::string extra;
            if (!(row >> a >> b) || (row >> extra) ||
                a < 1 || a > n || b < 1 || b > n)
            {
                wxMessageBox(wxString::Format(
                    wxT("Invalid directed edge on line %d. Use: from to"),
                    lineNumber), wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
                return;
            }
            if (a != b) adj[a - 1][b - 1] = 1;
        }

        std::vector<std::pair<int, int> > edges;
        std::vector<int> inDegree(n, 0), outDegree(n, 0);
        for (int a = 0; a < n; ++a)
            for (int b = 0; b < n; ++b)
                if (adj[a][b])
                {
                    edges.push_back(std::make_pair(a, b));
                    ++outDegree[a];
                    ++inDegree[b];
                }

        bool possible = true;
        for (int v = 0; v < n; ++v)
            if (!inDegree[v] || !outDegree[v]) possible = false;

        // Fix the first vertex to 1: rotating a cycle does not change it.
        std::vector<int> path(1, 0);
        std::vector<int> used(n, 0);
        used[0] = 1;
        const unsigned long long maxStates = 5000000ULL;
        unsigned long long states = 0;
        bool exhausted = false;
        std::function<bool(int)> search = [&](int v) -> bool
        {
            if (++states > maxStates)
            {
                exhausted = true;
                return false;
            }
            if (static_cast<int>(path.size()) == n)
                return adj[v][0] != 0;
            for (int next = 1; next < n; ++next)
            {
                if (!adj[v][next] || used[next]) continue;
                used[next] = 1;
                path.push_back(next);
                if (search(next)) return true;
                path.pop_back();
                used[next] = 0;
                if (exhausted) return false;
            }
            return false;
        };
        const bool found = possible && search(0);
        wxString report = wxT("DIRECTED HAMILTONIAN CYCLE (EXACT BACKTRACKING)\n\n");
        report += wxString::Format(wxT("Vertices: %d   Directed edges: %lu\n"),
            n, static_cast<unsigned long>(edges.size()));
        report += wxString::Format(wxT("Search states examined: %llu\n\n"), states);
        if (found)
        {
            report += wxT("HAMILTONIAN CYCLE FOUND\n\n");
            for (size_t k = 0; k < path.size(); ++k)
            {
                if (k) report += wxT(" -> ");
                report += wxString::Format(wxT("%d"), path[k] + 1);
            }
            report += wxT(" -> 1\n");
        }
        else if (exhausted)
            report += wxT("SEARCH LIMIT REACHED. Existence is UNDETERMINED.\n")
                      wxT("No conclusion can be drawn from this incomplete search.\n");
        else
            report += wxT("NO DIRECTED HAMILTONIAN CYCLE EXISTS.\n");

        wxDialog output(this, wxID_ANY,
            wxT("Directed Hamiltonian Cycle (Exact)"),
            wxDefaultPosition, wxSize(940, 740),
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* results = new wxTextCtrl(&output, wxID_ANY, report,
            wxDefaultPosition, wxSize(-1, 190),
            wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
        layout->Add(results, 0, wxEXPAND | wxALL, 8);
        wxPanel* drawing = new wxPanel(&output, wxID_ANY,
            wxDefaultPosition, wxSize(-1, 400));
        drawing->SetBackgroundColour(*wxWHITE);
        drawing->Bind(wxEVT_PAINT,
            [n, edges, path, found, drawing](wxPaintEvent&)
            {
                wxPaintDC dc(drawing);
                dc.SetBackground(*wxWHITE_BRUSH);
                dc.Clear();
                const wxSize sz = drawing->GetClientSize();
                const double pi = 3.14159265358979323846;
                const double cx = sz.x / 2.0, cy = sz.y / 2.0;
                const double radius = std::max(35.0,
                    std::min(sz.x, sz.y) * 0.40);
                std::vector<wxPoint> pts;
                for (int i = 0; i < n; ++i)
                {
                    double angle = -pi / 2 + 2 * pi * i / n;
                    pts.push_back(wxPoint(
                        int(cx + radius * std::cos(angle)),
                        int(cy + radius * std::sin(angle))));
                }
                for (size_t e = 0; e < edges.size(); ++e)
                {
                    const int a = edges[e].first, b = edges[e].second;
                    bool highlighted = false;
                    if (found)
                        for (size_t k = 0; k < path.size(); ++k)
                            if (path[k] == a && path[(k + 1) % path.size()] == b)
                                highlighted = true;
                    dc.SetPen(wxPen(highlighted ? wxColour(210, 35, 35)
                                                   : wxColour(125, 135, 145),
                                    highlighted ? 3 : 1));
                    const double dx = pts[b].x - pts[a].x;
                    const double dy = pts[b].y - pts[a].y;
                    const double len = std::sqrt(dx * dx + dy * dy);
                    if (len < 0.001) continue;
                    const double ux = dx / len, uy = dy / len;
                    // Opposite arcs receive opposite visual offsets.
                    const double offset = 5.0;
                    const double x1 = pts[a].x + ux * 18 - uy * offset;
                    const double y1 = pts[a].y + uy * 18 + ux * offset;
                    const double x2 = pts[b].x - ux * 20 - uy * offset;
                    const double y2 = pts[b].y - uy * 20 + ux * offset;
                    dc.DrawLine(int(x1), int(y1), int(x2), int(y2));
                    dc.DrawLine(int(x2), int(y2),
                        int(x2 - ux * 11 - uy * 5),
                        int(y2 - uy * 11 + ux * 5));
                    dc.DrawLine(int(x2), int(y2),
                        int(x2 - ux * 11 + uy * 5),
                        int(y2 - uy * 11 - ux * 5));
                }
                for (int i = 0; i < n; ++i)
                {
                    dc.SetPen(*wxBLACK_PEN);
                    dc.SetBrush(wxBrush(wxColour(250, 217, 91)));
                    dc.DrawCircle(pts[i], 16);
                    dc.SetTextForeground(*wxBLACK);
                    wxString label = wxString::Format(wxT("%d"), i + 1);
                    wxSize extent = dc.GetTextExtent(label);
                    dc.DrawText(label, pts[i].x - extent.x / 2,
                        pts[i].y - extent.y / 2);
                }
            });
        layout->Add(drawing, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
        wxButton* closeButton = new wxButton(&output, wxID_OK, wxT("Close"));
        layout->Add(closeButton, 0, wxALIGN_RIGHT | wxALL, 8);
        output.SetSizer(layout);
        output.ShowModal();
        return;
    }
    // END DIRECTED HAMILTONIAN CYCLE

    // BEGIN FLOYD WARSHALL
    if (algorithmName == wxT("floyd-warshall"))
    {
        wxTextEntryDialog inputDialog(this,
            wxT("Enter vertex count (1 to 100), then directed weighted edges:\n")
            wxT("from to weight   (one edge per line).\n")
            wxT("Weights may be negative. Example: 1 2 5"),
            wxT("All-Pairs Shortest Paths (Floyd-Warshall)"), wxEmptyString,
            wxOK | wxCANCEL | wxTE_MULTILINE);
        inputDialog.SetSize(wxSize(660, 540));
        if (inputDialog.ShowModal() != wxID_OK)
            return;

        std::istringstream input(std::string(inputDialog.GetValue().mb_str()));
        int n = 0;
        if (!(input >> n) || n < 1 || n > 100)
        {
            wxMessageBox(wxT("Vertex count must be between 1 and 100."),
                         wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }
        const long long INF = 4000000000000000LL;
        std::vector<std::vector<long long> > dist(
            n, std::vector<long long>(n, INF));
        std::vector<std::vector<int> > next(
            n, std::vector<int>(n, -1));
        struct WeightedEdge { int from, to; long long weight; };
        std::vector<WeightedEdge> edges;
        for (int i = 0; i < n; ++i)
        {
            dist[i][i] = 0;
            next[i][i] = i;
        }
        std::string line;
        std::getline(input, line); // finish first line
        int lineNumber = 1;
        while (std::getline(input, line))
        {
            ++lineNumber;
            if (line.find_first_not_of(" \t\r") == std::string::npos)
                continue;
            std::istringstream row(line);
            int a = 0, b = 0;
            long long w = 0;
            std::string extra;
            if (!(row >> a >> b >> w) || (row >> extra) ||
                a < 1 || a > n || b < 1 || b > n ||
                w <= -1000000000LL || w >= 1000000000LL)
            {
                wxMessageBox(wxString::Format(
                    wxT("Invalid edge on input line %d. Use: from to weight; weight magnitude below 1 billion."),
                    lineNumber), wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
                return;
            }
            --a; --b;
            if (w < dist[a][b])
            {
                dist[a][b] = w;
                next[a][b] = b;
            }
        }
        // Preserve only the minimum-weight parallel edge for drawing.
        for (int a = 0; a < n; ++a)
            for (int b = 0; b < n; ++b)
                if (dist[a][b] != INF && (a != b || dist[a][b] != 0))
                    edges.push_back(WeightedEdge{a, b, dist[a][b]});

        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                if (dist[i][k] != INF)
                    for (int j = 0; j < n; ++j)
                        if (dist[k][j] != INF &&
                            dist[i][k] + dist[k][j] < dist[i][j])
                        {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            next[i][j] = next[i][k];
                        }
        bool negativeCycle = false;
        for (int i = 0; i < n; ++i)
            if (dist[i][i] < 0)
                negativeCycle = true;

        wxString result;
        if (negativeCycle)
        {
            result = wxT("NEGATIVE-WEIGHT CYCLE DETECTED.\n")
                     wxT("Shortest distances are not well-defined for paths that can traverse a negative cycle.\n")
                     wxT("The matrix and path reconstruction are therefore withheld.\n");
        }
        else
        {
            result = wxT("Floyd-Warshall shortest-distance matrix (INF = unreachable):\n\n");
            result += wxT("from\\to");
            for (int j = 0; j < n; ++j)
                result += wxString::Format(wxT("\t%d"), j + 1);
            result += wxT("\n");
            for (int i = 0; i < n; ++i)
            {
                result += wxString::Format(wxT("%d"), i + 1);
                for (int j = 0; j < n; ++j)
                    result += (dist[i][j] == INF)
                        ? wxT("\tINF")
                        : wxString::Format(wxT("\t%lld"), dist[i][j]);
                result += wxT("\n");
            }
        }
        // Allow the user to choose a path; cancel leaves just the matrix.
        std::vector<int> chosenPath;
        if (!negativeCycle)
        {
            wxTextEntryDialog pathDialog(this,
                wxT("Optional: enter source and destination vertices (e.g. 1 4).\n")
                wxT("Cancel to display the matrix without a highlighted path."),
                wxT("Select Shortest Path"), wxT("1 4"));
            if (pathDialog.ShowModal() == wxID_OK)
            {
                std::istringstream pathInput(std::string(pathDialog.GetValue().mb_str()));
                int a = 0, b = 0;
                std::string extra;
                if (!(pathInput >> a >> b) || (pathInput >> extra) ||
                    a < 1 || a > n || b < 1 || b > n)
                {
                    wxMessageBox(wxT("Enter two vertex numbers between 1 and n."),
                        wxT("Invalid Path"), wxOK | wxICON_ERROR, this);
                    return;
                }
                --a; --b;
                if (next[a][b] == -1)
                    result += wxString::Format(
                        wxT("\nNo path from %d to %d.\n"), a + 1, b + 1);
                else
                {
                    result += wxString::Format(
                        wxT("\nShortest path %d to %d (weight %lld): "),
                        a + 1, b + 1, dist[a][b]);
                    chosenPath.push_back(a);
                    int v = a;
                    for (int step = 0; v != b && step < n; ++step)
                    {
                        v = next[v][b];
                        if (v < 0) break;
                        chosenPath.push_back(v);
                    }
                    for (size_t t = 0; t < chosenPath.size(); ++t)
                    {
                        if (t) result += wxT(" -> ");
                        result += wxString::Format(wxT("%d"), chosenPath[t] + 1);
                    }
                    result += wxT("\n");
                }
            }
        }

        wxDialog output(this, wxID_ANY,
            wxT("All-Pairs Shortest Paths (Floyd-Warshall)"),
            wxDefaultPosition, wxSize(950, 750),
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* report = new wxTextCtrl(&output, wxID_ANY, result,
            wxDefaultPosition, wxSize(-1, 330),
            wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
        layout->Add(report, 1, wxEXPAND | wxALL, 8);
        if (n <= 20)
        {
            wxPanel* drawing = new wxPanel(&output, wxID_ANY,
                wxDefaultPosition, wxSize(-1, 280));
            drawing->SetBackgroundColour(*wxWHITE);
            drawing->Bind(wxEVT_PAINT,
                [n, edges, chosenPath, drawing](wxPaintEvent&)
                {
                    wxPaintDC dc(drawing);
                    dc.SetBackground(*wxWHITE_BRUSH);
                    dc.Clear();
                    wxSize sz = drawing->GetClientSize();
                    const double pi = 3.14159265358979323846;
                    const double cx = sz.x / 2.0, cy = sz.y / 2.0;
                    const double radius = std::max(45.0,
                        std::min(sz.x, sz.y) * 0.39);
                    std::vector<wxPoint> pts;
                    for (int i = 0; i < n; ++i)
                    {
                        double theta = -pi / 2 + 2 * pi * i / n;
                        pts.push_back(wxPoint(
                            int(cx + radius * std::cos(theta)),
                            int(cy + radius * std::sin(theta))));
                    }
                    for (size_t e = 0; e < edges.size(); ++e)
                    {
                        int a = edges[e].from, b = edges[e].to;
                        bool highlighted = false;
                        for (size_t k = 1; k < chosenPath.size(); ++k)
                            if (chosenPath[k - 1] == a && chosenPath[k] == b)
                                highlighted = true;
                        dc.SetPen(wxPen(highlighted ? wxColour(210, 35, 35)
                                                     : wxColour(110, 120, 130),
                                        highlighted ? 3 : 1));
                        double dx = pts[b].x - pts[a].x;
                        double dy = pts[b].y - pts[a].y;
                        double len = std::sqrt(dx * dx + dy * dy);
                        if (len < 0.001) continue; // self-loop is in matrix
                        double ux = dx / len, uy = dy / len;
                        // Slight perpendicular offset separates reverse edges.
                        double offset = 5.0;
                        double x1 = pts[a].x + ux * 17 - uy * offset;
                        double y1 = pts[a].y + uy * 17 + ux * offset;
                        double x2 = pts[b].x - ux * 19 - uy * offset;
                        double y2 = pts[b].y - uy * 19 + ux * offset;
                        dc.DrawLine(int(x1), int(y1), int(x2), int(y2));
                        dc.DrawLine(int(x2), int(y2),
                            int(x2 - ux * 11 - uy * 5),
                            int(y2 - uy * 11 + ux * 5));
                        dc.DrawLine(int(x2), int(y2),
                            int(x2 - ux * 11 + uy * 5),
                            int(y2 - uy * 11 - ux * 5));
                        dc.SetTextForeground(highlighted ? wxColour(210, 35, 35)
                                                           : *wxBLACK);
                        // Opaque label prevents an edge from obscuring a minus sign.
                        wxString weightLabel =
                            wxString::Format(wxT("%lld"), edges[e].weight);
                        wxSize weightSize = dc.GetTextExtent(weightLabel);
                        int labelCx = int((x1 + x2) / 2 - uy * 9);
                        int labelCy = int((y1 + y2) / 2 + ux * 9);
                        int labelX = labelCx - weightSize.x / 2;
                        int labelY = labelCy - weightSize.y / 2;
                        dc.SetPen(*wxTRANSPARENT_PEN);
                        dc.SetBrush(*wxWHITE_BRUSH);
                        dc.DrawRectangle(labelX - 3, labelY - 2,
                                         weightSize.x + 6, weightSize.y + 4);
                        dc.DrawText(weightLabel, labelX, labelY);
                    }
                    for (int i = 0; i < n; ++i)
                    {
                        dc.SetPen(*wxBLACK_PEN);
                        dc.SetBrush(wxBrush(wxColour(250, 217, 91)));
                        dc.DrawCircle(pts[i], 16);
                        dc.SetTextForeground(*wxBLACK);
                        wxString label = wxString::Format(wxT("%d"), i + 1);
                        wxSize extent = dc.GetTextExtent(label);
                        dc.DrawText(label, pts[i].x - extent.x / 2,
                            pts[i].y - extent.y / 2);
                    }
                });
            layout->Add(drawing, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
        }
        else
            layout->Add(new wxStaticText(&output, wxID_ANY,
                wxT("Graphical display is limited to 20 vertices.")),
                0, wxALL, 8);
        wxButton* closeButton = new wxButton(&output, wxID_OK, wxT("Close"));
        layout->Add(closeButton, 0, wxALIGN_RIGHT | wxALL, 8);
        output.SetSizer(layout);
        output.ShowModal();
        return;
    }
    // END FLOYD WARSHALL

    // BEGIN TRANSITIVE CLOSURE
    if (algorithmName == wxT("transitive-closure"))
    {
        wxTextEntryDialog inputDialog(this,
            wxT("Enter the number of vertices on the first line, then directed edges\n")
            wxT("as ordered pairs 'from to', one pair per line. Vertices: 1 through n.\n")
            wxT("Maximum: 100 vertices. Self-loops are allowed."),
            wxT("Directed Transitive Closure (Warshall)"), wxEmptyString,
            wxOK | wxCANCEL | wxTE_MULTILINE);
        inputDialog.SetSize(wxSize(650, 550));
        if (inputDialog.ShowModal() != wxID_OK)
            return;

        std::istringstream input(std::string(inputDialog.GetValue().mb_str()));
        int n = 0;
        if (!(input >> n) || n < 1 || n > 100)
        {
            wxMessageBox(wxT("Vertex count must be between 1 and 100."),
                         wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }
        std::vector<std::vector<int> > adjacency(n, std::vector<int>(n, 0));
        int a, b;
        while (input >> a)
        {
            if (!(input >> b) || a < 1 || a > n || b < 1 || b > n)
            {
                wxMessageBox(wxT("Invalid edge: use two vertex numbers per line."),
                             wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
                return;
            }
            adjacency[a - 1][b - 1] = 1;
        }
        if (!input.eof())
        {
            wxMessageBox(wxT("Invalid characters in graph input."),
                         wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }

        std::vector<std::vector<int> > reach = adjacency;
        // Reflexive reachability: a vertex is reachable from itself
        // by a path of length zero.
        for (int i = 0; i < n; ++i)
            reach[i][i] = 1;
        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                if (reach[i][k])
                    for (int j = 0; j < n; ++j)
                        if (reach[k][j]) reach[i][j] = 1;

        wxString report = wxT("TRANSITIVE CLOSURE (Warshall algorithm)\n")
            wxT("A value of 1 means the column vertex is reachable from the row vertex.\n")
            wxT("Diagonal entries include paths of length zero.\n\n");
        auto appendMatrix = [&](const wxString& heading,
                                const std::vector<std::vector<int> >& matrix)
        {
            report += heading + wxT("\n");
            report += wxT("       ");
            for (int j = 0; j < n; ++j)
                report += wxString::Format(wxT("%3d"), j + 1);
            report += wxT("\n");
            for (int i = 0; i < n; ++i)
            {
                report += wxString::Format(wxT("%5d: "), i + 1);
                for (int j = 0; j < n; ++j)
                    report += wxString::Format(wxT("%3d"), matrix[i][j]);
                report += wxT("\n");
            }
            report += wxT("\n");
        };
        appendMatrix(wxT("ADJACENCY MATRIX"), adjacency);
        appendMatrix(wxT("REACHABILITY MATRIX"), reach);
        int newEdges = 0;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (reach[i][j] && !adjacency[i][j]) ++newEdges;
        report += wxString::Format(
            wxT("Additional reachability pairs (including reflexive pairs): %d\n"),
            newEdges);

        wxDialog resultDialog(this, wxID_ANY,
            wxT("Directed Transitive Closure (Warshall)"),
            wxDefaultPosition, wxSize(1050, 800),
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* matrixText = new wxTextCtrl(&resultDialog, wxID_ANY, report,
            wxDefaultPosition, wxDefaultSize,
            wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
        matrixText->SetFont(wxFont(11, wxFONTFAMILY_TELETYPE,
                                  wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        sizer->Add(matrixText, 1, wxEXPAND | wxALL, 10);

        if (n <= 20)
        {
            wxStaticText* legend = new wxStaticText(&resultDialog, wxID_ANY,
                wxT("Graphical closure: gray arrows = original edges; red arrows = added reachability. ")
                wxT("Reflexive loops are omitted from the drawing."));
            sizer->Add(legend, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
            wxPanel* panel = new wxPanel(&resultDialog);
            panel->SetMinSize(wxSize(500, 300));
            panel->SetBackgroundStyle(wxBG_STYLE_PAINT);
            panel->Bind(wxEVT_PAINT, [panel, n, adjacency, reach](wxPaintEvent&)
            {
                wxAutoBufferedPaintDC dc(panel);
                dc.SetBackground(*wxWHITE_BRUSH);
                dc.Clear();
                wxSize sz = panel->GetClientSize();
                const double pi = 3.14159265358979323846;
                const double cx = sz.x / 2.0, cy = sz.y / 2.0;
                const double radius = std::max(30.0,
                    std::min(sz.x, sz.y) / 2.0 - 40.0);
                std::vector<wxPoint> pos(n);
                for (int i = 0; i < n; ++i)
                {
                    double angle = -pi / 2.0 + 2.0 * pi * i / n;
                    pos[i] = wxPoint(static_cast<int>(cx + radius * std::cos(angle)),
                                     static_cast<int>(cy + radius * std::sin(angle)));
                }
                for (int i = 0; i < n; ++i)
                    for (int j = 0; j < n; ++j)
                    {
                        if (i == j || !reach[i][j]) continue;
                        bool added = !adjacency[i][j];
                        dc.SetPen(wxPen(added ? wxColour(205, 55, 55)
                                              : wxColour(135, 145, 155),
                                        added ? 2 : 1));
                        double dx = pos[j].x - pos[i].x;
                        double dy = pos[j].y - pos[i].y;
                        double len = std::sqrt(dx*dx + dy*dy);
                        if (len < 1.0) continue;
                        double ux = dx/len, uy = dy/len;
                        // Each direction is shifted to its own side.
                        double ox = -uy * 5.0, oy = ux * 5.0;
                        double sx = pos[i].x + ux*19 + ox;
                        double sy = pos[i].y + uy*19 + oy;
                        double tx = pos[j].x - ux*21 + ox;
                        double ty = pos[j].y - uy*21 + oy;
                        dc.DrawLine(static_cast<int>(sx), static_cast<int>(sy),
                                    static_cast<int>(tx), static_cast<int>(ty));
                        dc.DrawLine(static_cast<int>(tx), static_cast<int>(ty),
                                    static_cast<int>(tx - ux*9 - uy*5),
                                    static_cast<int>(ty - uy*9 + ux*5));
                        dc.DrawLine(static_cast<int>(tx), static_cast<int>(ty),
                                    static_cast<int>(tx - ux*9 + uy*5),
                                    static_cast<int>(ty - uy*9 - ux*5));
                    }
                dc.SetFont(wxFont(11, wxFONTFAMILY_SWISS,
                                  wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
                for (int i = 0; i < n; ++i)
                {
                    dc.SetPen(wxPen(*wxBLACK, 1));
                    dc.SetBrush(wxBrush(wxColour(255, 220, 0)));
                    dc.DrawCircle(pos[i], 17);
                    wxString label = wxString::Format(wxT("%d"), i + 1);
                    wxSize ext = dc.GetTextExtent(label);
                    dc.SetTextForeground(*wxBLACK);
                    dc.DrawText(label, pos[i].x - ext.x/2, pos[i].y - ext.y/2);
                }
            });
            sizer->Add(panel, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
        }
        else
            sizer->Add(new wxStaticText(&resultDialog, wxID_ANY,
                wxT("Graphical display is limited to 20 vertices; matrices support 100.")),
                0, wxALL, 10);
        sizer->Add(new wxButton(&resultDialog, wxID_OK, wxT("Close")),
                   0, wxALIGN_CENTER | wxALL, 10);
        resultDialog.SetSizer(sizer);
        resultDialog.CentreOnParent();
        resultDialog.ShowModal();
        return;
    }
    // END TRANSITIVE CLOSURE

    if (algorithmName != wxT("directed-cycle-detection"))
    {
        wxMessageBox(
            wxT("This directed graph example is not yet connected to an interactive handler."),
            wxT("Directed Graph Algorithms"), wxOK | wxICON_INFORMATION, this);
        return;
    }

    wxTextEntryDialog inputDialog(this,
        wxT("Enter the number of vertices on the first line, then directed edges\n")
        wxT("as ordered pairs 'from to', one pair per line. Vertices: 1 through n.\n")
        wxT("Maximum: 100 vertices. Self-loops are allowed."),
        wxT("Directed Cycle Detection (DFS)"), wxEmptyString,
        wxOK | wxCANCEL | wxTE_MULTILINE);
    inputDialog.SetSize(wxSize(650, 550));
    if (inputDialog.ShowModal() != wxID_OK)
        return;

    std::istringstream input(std::string(inputDialog.GetValue().mb_str()));
    int n = 0;
    if (!(input >> n) || n < 1 || n > 100)
    {
        wxMessageBox(wxT("Vertex count must be between 1 and 100."),
                     wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
        return;
    }

    std::vector<std::vector<int> > adj(n);
    std::vector<std::pair<int,int> > edges;
    int a, b;
    while (input >> a)
    {
        if (!(input >> b) || a < 1 || a > n || b < 1 || b > n)
        {
            wxMessageBox(wxT("Invalid directed edge: use two vertex numbers per line."),
                         wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }
        --a; --b;
        if (std::find(adj[a].begin(), adj[a].end(), b) == adj[a].end())
        {
            adj[a].push_back(b);
            edges.push_back(std::make_pair(a, b));
        }
    }
    if (!input.eof())
    {
        wxMessageBox(wxT("Invalid characters in graph input."),
                     wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
        return;
    }

    std::vector<int> state(n, 0), parent(n, -1), cycle;
    std::function<bool(int)> dfs = [&](int u) -> bool
    {
        state[u] = 1;
        for (size_t k = 0; k < adj[u].size(); ++k)
        {
            int v = adj[u][k];
            if (state[v] == 0)
            {
                parent[v] = u;
                if (dfs(v)) return true;
            }
            else if (state[v] == 1)
            {
                std::vector<int> reversePath;
                for (int x = u; x != v; x = parent[x])
                    reversePath.push_back(x);
                cycle.push_back(v);
                for (std::vector<int>::reverse_iterator it = reversePath.rbegin();
                     it != reversePath.rend(); ++it)
                    cycle.push_back(*it);
                cycle.push_back(v);
                return true;
            }
        }
        state[u] = 2;
        return false;
    };
    for (int u = 0; u < n && cycle.empty(); ++u)
        if (state[u] == 0) dfs(u);

    wxString result;
    if (cycle.empty())
        result = wxT("ACYCLIC: This directed graph is a DAG.");
    else
    {
        result = wxT("CYCLIC: Directed cycle found: ");
        for (size_t i = 0; i < cycle.size(); ++i)
        {
            if (i) result += wxT(" -> ");
            result += wxString::Format(wxT("%d"), cycle[i] + 1);
        }
    }

    wxDialog resultDialog(this, wxID_ANY, wxT("Directed Cycle Detection (DFS)"),
                          wxDefaultPosition, wxSize(1200, 850),
                          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(new wxStaticText(&resultDialog, wxID_ANY, result),
               0, wxALIGN_CENTER | wxALL, 12);
    wxPanel* panel = new wxPanel(&resultDialog);
    panel->SetBackgroundStyle(wxBG_STYLE_PAINT);
    panel->Bind(wxEVT_PAINT, [panel, n, edges, cycle](wxPaintEvent&)
    {
        wxAutoBufferedPaintDC dc(panel);
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();
        wxSize sz = panel->GetClientSize();
        const double pi = 3.14159265358979323846;
        const double cx = sz.x / 2.0, cy = sz.y / 2.0;
        const double radius = std::max(35.0,
            std::min(sz.x, sz.y) / 2.0 - 75.0);
        std::vector<wxPoint> pos(n);
        for (int i = 0; i < n; ++i)
        {
            double angle = -pi / 2.0 + 2.0 * pi * i / n;
            pos[i] = wxPoint(static_cast<int>(cx + radius * std::cos(angle)),
                             static_cast<int>(cy + radius * std::sin(angle)));
        }
        std::set<std::pair<int,int> > cycleEdges;
        for (size_t i = 1; i < cycle.size(); ++i)
            cycleEdges.insert(std::make_pair(cycle[i-1], cycle[i]));

        for (size_t i = 0; i < edges.size(); ++i)
        {
            int u = edges[i].first, v = edges[i].second;
            bool highlight = cycleEdges.count(edges[i]) != 0;
            wxColour color = highlight ? wxColour(215, 45, 45) : wxColour(135, 145, 155);
            dc.SetPen(wxPen(color, highlight ? 4 : 2));
            double dx = pos[v].x - pos[u].x;
            double dy = pos[v].y - pos[u].y;
            if (u == v)
            {
                dc.DrawCircle(pos[u].x + 21, pos[u].y - 22, 13);
                // Small arrowhead for a self-loop.
                dc.DrawLine(pos[u].x + 33, pos[u].y - 25,
                            pos[u].x + 25, pos[u].y - 31);
                dc.DrawLine(pos[u].x + 33, pos[u].y - 25,
                            pos[u].x + 34, pos[u].y - 34);
                continue;
            }
            double length = std::sqrt(dx*dx + dy*dy);
            if (length < 1.0) continue;
            double ux = dx/length, uy = dy/length;
            // Offset opposite directed edges to make both arrows visible.
            double offset = 0.0;
            if (std::find(edges.begin(), edges.end(), std::make_pair(v,u)) != edges.end())
                offset = (u < v ? 8.0 : -8.0);
            double ox = -uy * offset, oy = ux * offset;
            double sx = pos[u].x + ux*22 + ox, sy = pos[u].y + uy*22 + oy;
            double tx = pos[v].x - ux*24 + ox, ty = pos[v].y - uy*24 + oy;
            dc.DrawLine(static_cast<int>(sx), static_cast<int>(sy),
                        static_cast<int>(tx), static_cast<int>(ty));
            const double wing = 10.0;
            dc.DrawLine(static_cast<int>(tx), static_cast<int>(ty),
                static_cast<int>(tx - ux*wing - uy*wing*0.55),
                static_cast<int>(ty - uy*wing + ux*wing*0.55));
            dc.DrawLine(static_cast<int>(tx), static_cast<int>(ty),
                static_cast<int>(tx - ux*wing + uy*wing*0.55),
                static_cast<int>(ty - uy*wing - ux*wing*0.55));
        }
        dc.SetFont(wxFont(12, wxFONTFAMILY_SWISS,
                          wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        for (int i = 0; i < n; ++i)
        {
            dc.SetPen(wxPen(*wxBLACK, 2));
            dc.SetBrush(wxBrush(wxColour(255, 220, 0)));
            dc.DrawCircle(pos[i], 19);
            wxString label = wxString::Format(wxT("%d"), i + 1);
            wxSize ext = dc.GetTextExtent(label);
            dc.SetTextForeground(*wxBLACK);
            dc.DrawText(label, pos[i].x - ext.x/2, pos[i].y - ext.y/2);
        }
    });
    sizer->Add(panel, 1, wxEXPAND | wxALL, 10);
    sizer->Add(new wxButton(&resultDialog, wxID_OK, wxT("Close")),
               0, wxALIGN_CENTER | wxALL, 10);
    resultDialog.SetSizer(sizer);
    resultDialog.CentreOnParent();
    resultDialog.ShowModal();
}
// END DIRECTED CYCLE DETECTION HANDLER

// ============================================================
// Undirected Graph Algorithm Examples: BFS and DFS
// ============================================================

void DiscreteMathCalculatorFrame::
OnCreateUndirectedGraphAlgorithmsExamplesMenuItemSelected(
    wxCommandEvent& event)
{
    wxString examplesDirectory =
        wxStandardPaths::Get().GetResourcesDir() +
        wxT("/examples/graph-files/algorithms-undir");

    wxDirDialog dialog(
        this,
        wxT("Select an undirected graph algorithm example."),
        examplesDirectory,
        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST
    );

    if (dialog.ShowModal() != wxID_OK)
        return;

    wxString selectedDirectory = dialog.GetPath();
    wxString algorithmName = selectedDirectory.AfterLast('/');


    // BEGIN ACYCLIC GRAPH TEST
    if (algorithmName == wxT("acyclic-graph-test"))
    {
        wxTextEntryDialog inputDialog(
            this,
            wxT("Enter the number of vertices on the first line,\n")
            wxT("followed by undirected edges (one pair per line).\n")
            wxT("Vertices are numbered 1 through n."),
            wxT("Acyclic Graph Test - Graph Input"),
            wxEmptyString,
            wxOK | wxCANCEL | wxTE_MULTILINE
        );

        inputDialog.SetSize(wxSize(650, 550));

        if (inputDialog.ShowModal() != wxID_OK)
            return;

        std::istringstream input(
            std::string(inputDialog.GetValue().mb_str())
        );

        int n = 0;

        if (!(input >> n) || n < 1 || n > 100)
        {
            wxMessageBox(
                wxT("Number of vertices must be between 1 and 100."),
                wxT("Invalid Graph"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        std::vector<std::pair<int,int>> edges;
        std::vector<std::vector<int>> adjacency(n);

        int a, b;

        while (input >> a)
        {
            if (!(input >> b))
            {
                wxMessageBox(
                    wxT("Each edge must contain two vertices."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            if (a < 1 || a > n ||
                b < 1 || b > n || a == b)
            {
                wxMessageBox(
                    wxT("Invalid edge endpoints or self-loop."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            --a;
            --b;

            bool duplicate = false;

            for (int v : adjacency[a])
            {
                if (v == b)
                {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate)
            {
                adjacency[a].push_back(b);
                adjacency[b].push_back(a);
                edges.push_back(std::make_pair(a,b));
            }
        }

        if (!input.eof())
        {
            wxMessageBox(
                wxT("Graph input contains invalid data."),
                wxT("Invalid Graph"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        // Detect cycles in an undirected graph using DFS.

        std::vector<int> visited(n, 0);
        std::vector<int> parent(n, -1);
        std::vector<int> cycle;

        std::function<bool(int,int)> dfs;

        dfs = [&](int u, int p) -> bool
        {
            visited[u] = 1;
            parent[u] = p;

            for (int v : adjacency[u])
            {
                if (v == p)
                    continue;

                if (visited[v] == 0)
                {
                    if (dfs(v, u))
                        return true;
                }
                else if (visited[v] == 1)
                {
                    cycle.push_back(v);

                    int current = u;

                    while (current != v && current != -1)
                    {
                        cycle.push_back(current);
                        current = parent[current];
                    }

                    cycle.push_back(v);
                    return true;
                }
            }

            visited[u] = 2;
            return false;
        };

        bool cyclic = false;

        for (int i = 0; i < n; ++i)
        {
            if (visited[i] == 0 && dfs(i, -1))
            {
                cyclic = true;
                break;
            }
        }

        wxString result;

        if (cyclic)
        {
            result = wxT("CYCLIC - Graph contains a cycle\nCycle: ");

            for (size_t i = 0; i < cycle.size(); ++i)
            {
                if (i != 0)
                    result += wxT(" -> ");

                result += wxString::Format(
                    wxT("%d"), cycle[i] + 1
                );
            }
        }
        else
        {
            result = wxT("ACYCLIC - Graph is a forest");
        }

        wxDialog resultDialog(
            this,
            wxID_ANY,
            wxT("Acyclic Graph Test (Cycle Detection)"),
            wxDefaultPosition,
            wxSize(1200, 850),
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
        );

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        sizer->Add(
            new wxStaticText(
                &resultDialog,
                wxID_ANY,
                result
            ),
            0,
            wxALIGN_CENTER | wxALL,
            12
        );

        wxPanel* panel = new wxPanel(&resultDialog);
        panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

        panel->Bind(
            wxEVT_PAINT,
            [panel, n, edges, cycle](wxPaintEvent&)
            {
                wxAutoBufferedPaintDC dc(panel);

                dc.SetBackground(*wxWHITE_BRUSH);
                dc.Clear();

                const wxSize size = panel->GetClientSize();

                const double pi = 3.14159265358979323846;
                const double cx = size.x / 2.0;
                const double cy = size.y / 2.0;

                const double radius = std::max(
                    40.0,
                    std::min(size.x, size.y) / 2.0 - 65.0
                );

                std::vector<wxPoint> points(n);

                for (int i = 0; i < n; ++i)
                {
                    double angle =
                        -pi / 2.0 + 2.0 * pi * i / n;

                    points[i] = wxPoint(
                        static_cast<int>(
                            cx + radius * std::cos(angle)
                        ),
                        static_cast<int>(
                            cy + radius * std::sin(angle)
                        )
                    );
                }

                // Draw all edges in gray.

                dc.SetPen(
                    wxPen(wxColour(170,170,170), 2)
                );

                for (const auto& edge : edges)
                {
                    dc.DrawLine(
                        points[edge.first],
                        points[edge.second]
                    );
                }

                // Highlight the detected cycle in red.

                dc.SetPen(
                    wxPen(wxColour(220,30,30), 5)
                );

                for (size_t i = 1; i < cycle.size(); ++i)
                {
                    dc.DrawLine(
                        points[cycle[i-1]],
                        points[cycle[i]]
                    );
                }

                // Draw yellow vertices over the edges.

                dc.SetPen(wxPen(*wxBLACK, 2));
                dc.SetBrush(
                    wxBrush(wxColour(255,220,0))
                );

                dc.SetFont(wxFont(
                    13,
                    wxFONTFAMILY_SWISS,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_BOLD
                ));

                dc.SetTextForeground(*wxBLACK);

                for (int i = 0; i < n; ++i)
                {
                    dc.DrawCircle(points[i], 22);

                    wxString label =
                        wxString::Format(wxT("%d"), i + 1);

                    wxSize extent = dc.GetTextExtent(label);

                    dc.DrawText(
                        label,
                        points[i].x - extent.x / 2,
                        points[i].y - extent.y / 2
                    );
                }
            }
        );

        sizer->Add(
            panel,
            1,
            wxEXPAND | wxALL,
            10
        );

        wxButton* closeButton =
            new wxButton(
                &resultDialog,
                wxID_OK,
                wxT("Close")
            );

        sizer->Add(
            closeButton,
            0,
            wxALIGN_CENTER | wxALL,
            10
        );

        resultDialog.SetSizer(sizer);
        resultDialog.Layout();
        resultDialog.CentreOnParent();
        resultDialog.ShowModal();

        return;
    }
    // END ACYCLIC GRAPH TEST



    // BEGIN NP-COMPLETE PROBLEMS EXAMPLES
    if (algorithmName == wxT("NP-complete-problems"))
    {
        wxArrayString choices;

        choices.Add(wxT("Maximum Clique"));
        choices.Add(wxT("Independent Set / Vertex Cover"));
        choices.Add(wxT("Hamiltonian Cycle"));
        choices.Add(wxT("Hamiltonian Path"));
        choices.Add(wxT("Traveling Salesman Problem (TSP)"));
        choices.Add(wxT("Graph Coloring"));
        wxSingleChoiceDialog selection(
            this,
            wxT("Select an NP-complete problem example."),
            wxT("NP-complete Problems"),
            choices
        );

        if (selection.ShowModal() != wxID_OK)
            return;
        int selectedChoice = selection.GetSelection();

        // Hamiltonian Path exercise: draw a five-vertex path graph.
        if (selectedChoice == 3)
        {
            // Interactive demonstration of an undirected Hamiltonian path.
            // All state belongs to this modal dialog; no graph algorithm is changed.
            wxDialog dialog(
                this, wxID_ANY, wxT("Hamiltonian Path - Interactive Example"),
                wxDefaultPosition, wxSize(950, 650),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

            wxBoxSizer* outer = new wxBoxSizer(wxVERTICAL);
            wxStaticText* instructions = new wxStaticText(
                &dialog, wxID_ANY,
                wxT("Find a path that visits every vertex exactly once. "
                    "A Hamiltonian path does not need to return to its starting vertex."));
            instructions->Wrap(850);
            outer->Add(instructions, 0, wxEXPAND | wxALL, 12);

            wxPanel* graphPanel = new wxPanel(&dialog, wxID_ANY);
            graphPanel->SetMinSize(wxSize(450, 300));
            graphPanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
            outer->Add(graphPanel, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 12);

            // 0 = graph only, 1 = solution highlighted.
            // Shared state is kept alive until the modal dialog closes.
            int displayMode = 0;
            wxStaticText* result = new wxStaticText(
                &dialog, wxID_ANY,
                wxT("Graph shown. Try finding a Hamiltonian path before revealing the answer."));
            outer->Add(result, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 12);

            wxBoxSizer* buttons = new wxBoxSizer(wxHORIZONTAL);
            wxButton* findPath = new wxButton(&dialog, wxID_ANY, wxT("Find Hamiltonian Path"));
            wxButton* reset = new wxButton(&dialog, wxID_ANY, wxT("Reset"));
            wxButton* close = new wxButton(&dialog, wxID_OK, wxT("Close"));
            buttons->Add(findPath, 0, wxALL, 5);
            buttons->Add(reset, 0, wxALL, 5);
            buttons->AddStretchSpacer();
            buttons->Add(close, 0, wxALL, 5);
            outer->Add(buttons, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);

            graphPanel->Bind(wxEVT_PAINT, [graphPanel, &displayMode](wxPaintEvent&)
            {
                wxAutoBufferedPaintDC dc(graphPanel);
                dc.SetBackground(*wxWHITE_BRUSH);
                dc.Clear();
                wxSize size = graphPanel->GetClientSize();
                const int width = size.GetWidth();
                const int height = size.GetHeight();
                const int cy = height / 2;
                const int left = 65;
                const int right = width - 65;
                const int spacing = (right - left) / 4;
                const int radius = 23;

                dc.SetPen(wxPen(displayMode == 1 ? wxColour(218, 158, 27)
                                                     : wxColour(130, 130, 130),
                                    displayMode == 1 ? 6 : 3));
                for (int j = 0; j < 4; ++j)
                    dc.DrawLine(left + j * spacing, cy,
                                left + (j + 1) * spacing, cy);

                dc.SetFont(wxFont(15, wxFONTFAMILY_DEFAULT,
                                  wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
                for (int j = 0; j < 5; ++j)
                {
                    const int x = left + j * spacing;
                    dc.SetPen(wxPen(wxColour(55, 100, 160), 2));
                    dc.SetBrush(wxBrush(wxColour(221, 235, 252)));
                    dc.DrawCircle(x, cy, radius);
                    wxString label = wxString::Format(wxT("%c"), 'A' + j);
                    wxCoord tw = 0, th = 0;
                    dc.GetTextExtent(label, &tw, &th);
                    dc.SetTextForeground(wxColour(20, 55, 100));
                    dc.DrawText(label, x - tw / 2, cy - th / 2);
                }
            });

            findPath->Bind(wxEVT_BUTTON, [graphPanel, result, &displayMode](wxCommandEvent&)
            {
                displayMode = 1;
                result->SetLabel(wxT("Hamiltonian path: A -> B -> C -> D -> E. "
                                     "All five vertices are visited once. "
                                     "There is no edge E--A, so there is no Hamiltonian cycle."));
                graphPanel->Refresh();
            });
            reset->Bind(wxEVT_BUTTON, [graphPanel, result, &displayMode](wxCommandEvent&)
            {
                displayMode = 0;
                result->SetLabel(wxT("Reset complete. Find the Hamiltonian path before clicking Find Hamiltonian Path."));
                graphPanel->Refresh();
            });

            dialog.SetSizer(outer);
            dialog.Layout();
            dialog.CentreOnParent();
            dialog.ShowModal();
            return;
        }

        // Map the new six-item selector to the original dispatch indices.
        const int oldChoiceIndices[] = {0, 3, 2, -1, 5, 1};
        if (selectedChoice < 0 || selectedChoice >= 6)
            return;
        int choice = oldChoiceIndices[selectedChoice];

    
    
    // BEGIN MINIMUM CONNECTED VERTEX COVER EXACT
    if (choice == 15)
    {
        wxTextEntryDialog inputDialog(
            this,
            wxT("Enter the number of vertices on the first line,\n")
            wxT("followed by undirected edges, one pair per line.\n")
            wxT("Vertices are numbered 1 through n.\n")
            wxT("Maximum: 20 vertices."),
            wxT("Minimum Connected Vertex Cover (Exact)"),
            wxEmptyString,
            wxOK | wxCANCEL | wxTE_MULTILINE
        );
        inputDialog.SetSize(wxSize(650, 550));
        if (inputDialog.ShowModal() != wxID_OK)
            return;

        std::istringstream input{
            std::string(inputDialog.GetValue().mb_str())
        };
        int n = 0;
        if (!(input >> n) || n < 1 || n > 20)
        {
            wxMessageBox(wxT("Enter a vertex count between 1 and 20."),
                         wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }

        std::vector<std::pair<int, int>> edges;
        std::vector<unsigned int> adjacency(n, 0u);
        int a, b;
        while (input >> a)
        {
            if (!(input >> b))
            {
                wxMessageBox(wxT("Each edge must have two vertex numbers."),
                             wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
                return;
            }
            if (a < 1 || a > n || b < 1 || b > n || a == b)
            {
                wxMessageBox(wxT("Invalid edge or self-loop."),
                             wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
                return;
            }
            --a;
            --b;
            if ((adjacency[a] & (1u << b)) == 0u)
            {
                adjacency[a] |= (1u << b);
                adjacency[b] |= (1u << a);
                edges.push_back(std::make_pair(a, b));
            }
        }
        if (!input.eof())
        {
            wxMessageBox(wxT("Invalid characters in graph input."),
                         wxT("Invalid Graph"), wxOK | wxICON_ERROR, this);
            return;
        }

        const unsigned int fullMask = (1u << n) - 1u;
        auto connected = [&](unsigned int mask) -> bool
        {
            if (mask == 0u)
                return false;
            unsigned int reached = mask & (~mask + 1u);
            unsigned int frontier = reached;
            while (frontier != 0u)
            {
                unsigned int next = 0u;
                for (int i = 0; i < n; ++i)
                    if (frontier & (1u << i))
                        next |= adjacency[i];
                next &= mask;
                next &= ~reached;
                if (next == 0u)
                    break;
                reached |= next;
                frontier = next;
            }
            return reached == mask;
        };

        unsigned int bestMask = 0u;
        int bestSize = n + 1;
        if (edges.empty())
            bestSize = 0; // By convention, the empty cover is connected.
        else
        {
            for (unsigned int mask = 1u; mask <= fullMask; ++mask)
            {
                int count = 0;
                for (int i = 0; i < n; ++i)
                    if (mask & (1u << i))
                        ++count;
                if (count >= bestSize)
                    continue;
                bool coversAllEdges = true;
                for (const auto& edge : edges)
                {
                    if (!(mask & (1u << edge.first)) &&
                        !(mask & (1u << edge.second)))
                    {
                        coversAllEdges = false;
                        break;
                    }
                }
                if (!coversAllEdges || !connected(mask))
                    continue;
                bestMask = mask;
                bestSize = count;
            }
        }

        wxString result;
        if (bestSize == n + 1)
            result = wxT("No connected vertex cover exists.\n")
                     wxT("No vertex subset satisfies both conditions.");
        else
        {
            result = wxString::Format(
                wxT("Minimum Connected Vertex Cover Size: %d\n")
                wxT("Selected vertices: {"), bestSize);
            bool first = true;
            for (int i = 0; i < n; ++i)
            {
                if (bestMask & (1u << i))
                {
                    if (!first)
                        result += wxT(", ");
                    result += wxString::Format(wxT("%d"), i + 1);
                    first = false;
                }
            }
            result += wxT("}");
        }

        wxDialog resultDialog(this, wxID_ANY,
            wxT("Minimum Connected Vertex Cover (Exact)"),
            wxDefaultPosition, wxSize(1200, 850),
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(new wxStaticText(&resultDialog, wxID_ANY, result),
                   0, wxALIGN_CENTER | wxALL, 12);
        wxPanel* panel = new wxPanel(&resultDialog);
        panel->SetBackgroundStyle(wxBG_STYLE_PAINT);
        panel->Bind(wxEVT_PAINT,
            [panel, n, edges, bestMask](wxPaintEvent&)
            {
                wxAutoBufferedPaintDC dc(panel);
                dc.SetBackground(*wxWHITE_BRUSH);
                dc.Clear();
                wxSize size = panel->GetClientSize();
                const double pi = 3.14159265358979323846;
                const double cx = size.x / 2.0;
                const double cy = size.y / 2.0;
                const double radius = std::max(
                    40.0, std::min(size.x, size.y) / 2.0 - 65.0);
                std::vector<wxPoint> points(n);
                for (int i = 0; i < n; ++i)
                {
                    double angle = -pi / 2.0 + 2.0 * pi * i / n;
                    points[i] = wxPoint(
                        static_cast<int>(cx + radius * std::cos(angle)),
                        static_cast<int>(cy + radius * std::sin(angle)));
                }
                dc.SetPen(wxPen(wxColour(170, 170, 170), 2));
                for (const auto& edge : edges)
                    dc.DrawLine(points[edge.first], points[edge.second]);
                dc.SetPen(wxPen(wxColour(35, 95, 210), 4));
                for (const auto& edge : edges)
                    if ((bestMask & (1u << edge.first)) &&
                        (bestMask & (1u << edge.second)))
                        dc.DrawLine(points[edge.first], points[edge.second]);
                dc.SetFont(wxFont(13, wxFONTFAMILY_SWISS,
                                  wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
                for (int i = 0; i < n; ++i)
                {
                    dc.SetPen(wxPen(*wxBLACK, 2));
                    if (bestMask & (1u << i))
                        dc.SetBrush(wxBrush(wxColour(50, 120, 235)));
                    else
                        dc.SetBrush(wxBrush(wxColour(255, 220, 0)));
                    dc.DrawCircle(points[i], 22);
                    wxString label = wxString::Format(wxT("%d"), i + 1);
                    wxSize extent = dc.GetTextExtent(label);
                    dc.SetTextForeground((bestMask & (1u << i))
                                             ? *wxWHITE : *wxBLACK);
                    dc.DrawText(label, points[i].x - extent.x / 2,
                                      points[i].y - extent.y / 2);
                }
            });
        sizer->Add(panel, 1, wxEXPAND | wxALL, 10);
        wxButton* closeButton = new wxButton(
            &resultDialog, wxID_OK, wxT("Close"));
        sizer->Add(closeButton, 0, wxALIGN_CENTER | wxALL, 10);
        resultDialog.SetSizer(sizer);
        resultDialog.Layout();
        resultDialog.CentreOnParent();
        resultDialog.ShowModal();
        return;
    }
    // END MINIMUM CONNECTED VERTEX COVER EXACT

    // BEGIN MINIMUM CONNECTED DOMINATING SET EXACT
    if (choice == 14)
    {
        wxTextEntryDialog inputDialog(
            this,
            wxT("Enter the number of vertices on the first line,\n")
            wxT("followed by undirected edges, one pair per line.\n")
            wxT("Vertices are numbered 1 through n.\n")
            wxT("Maximum: 20 vertices."),
            wxT("Minimum Connected Dominating Set (Exact)"),
            wxEmptyString,
            wxOK | wxCANCEL | wxTE_MULTILINE
        );

        inputDialog.SetSize(wxSize(650, 550));

        if (inputDialog.ShowModal() != wxID_OK)
            return;

        std::istringstream input{
            std::string(inputDialog.GetValue().mb_str())
        };

        int n = 0;

        if (!(input >> n) || n < 1 || n > 20)
        {
            wxMessageBox(
                wxT("Enter a vertex count between 1 and 20."),
                wxT("Invalid Graph"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        std::vector<std::pair<int,int>> edges;
        std::vector<unsigned int> adjacency(n, 0);

        int a, b;

        while (input >> a)
        {
            if (!(input >> b))
            {
                wxMessageBox(
                    wxT("Each edge must contain two vertex numbers."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            if (a < 1 || a > n || b < 1 || b > n || a == b)
            {
                wxMessageBox(
                    wxT("Invalid edge endpoints or self-loop."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            --a;
            --b;

            if ((adjacency[a] & (1u << b)) == 0)
            {
                adjacency[a] |= (1u << b);
                adjacency[b] |= (1u << a);
                edges.push_back(std::make_pair(a, b));
            }
        }

        if (!input.eof())
        {
            wxMessageBox(
                wxT("Graph input contains invalid characters."),
                wxT("Invalid Graph"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        const unsigned int fullMask = (1u << n) - 1u;

        // Check whether the entire graph is connected.
        auto connected = [&](unsigned int mask) -> bool
        {
            if (mask == 0)
                return false;

            unsigned int reached = mask & (~mask + 1u);
            unsigned int frontier = reached;

            while (frontier != 0)
            {
                unsigned int next = 0;

                for (int i = 0; i < n; ++i)
                {
                    if (frontier & (1u << i))
                        next |= adjacency[i];
                }

                next &= mask;
                next &= ~reached;

                if (next == 0)
                    break;

                reached |= next;
                frontier = next;
            }

            return reached == mask;
        };

        const bool graphConnected = connected(fullMask);

        unsigned int bestMask = 0;
        int bestSize = n + 1;

        if (graphConnected)
        {
            // Exact enumeration of all nonempty vertex subsets.
            for (unsigned int mask = 1; mask <= fullMask; ++mask)
            {
                int count = 0;

                for (int i = 0; i < n; ++i)
                {
                    if (mask & (1u << i))
                        ++count;
                }

                if (count >= bestSize)
                    continue;

                // A dominating set covers every vertex.
                unsigned int dominated = mask;

                for (int i = 0; i < n; ++i)
                {
                    if (mask & (1u << i))
                        dominated |= adjacency[i];
                }

                if (dominated != fullMask)
                    continue;

                // The induced subgraph must also be connected.
                if (!connected(mask))
                    continue;

                bestMask = mask;
                bestSize = count;
            }
        }

        wxString result;

        if (!graphConnected)
        {
            result =
                wxT("No connected dominating set exists.\n")
                wxT("The input graph is disconnected.");
        }
        else
        {
            result = wxString::Format(
                wxT("Minimum Connected Dominating Set Size: %d\n")
                wxT("Selected vertices: {"),
                bestSize
            );

            bool first = true;

            for (int i = 0; i < n; ++i)
            {
                if (bestMask & (1u << i))
                {
                    if (!first)
                        result += wxT(", ");

                    result += wxString::Format(wxT("%d"), i + 1);
                    first = false;
                }
            }

            result += wxT("}");
        }

        wxDialog resultDialog(
            this,
            wxID_ANY,
            wxT("Minimum Connected Dominating Set (Exact)"),
            wxDefaultPosition,
            wxSize(1200, 850),
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
        );

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        sizer->Add(
            new wxStaticText(
                &resultDialog,
                wxID_ANY,
                result
            ),
            0,
            wxALIGN_CENTER | wxALL,
            12
        );

        wxPanel* panel = new wxPanel(&resultDialog);
        panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

        panel->Bind(
            wxEVT_PAINT,
            [panel, n, edges, bestMask](wxPaintEvent&)
            {
                wxAutoBufferedPaintDC dc(panel);

                dc.SetBackground(*wxWHITE_BRUSH);
                dc.Clear();

                wxSize size = panel->GetClientSize();

                const double pi = 3.14159265358979323846;
                const double cx = size.x / 2.0;
                const double cy = size.y / 2.0;

                const double radius = std::max(
                    40.0,
                    std::min(size.x, size.y) / 2.0 - 65.0
                );

                std::vector<wxPoint> points(n);

                for (int i = 0; i < n; ++i)
                {
                    double angle =
                        -pi / 2.0 + 2.0 * pi * i / n;

                    points[i] = wxPoint(
                        static_cast<int>(
                            cx + radius * std::cos(angle)
                        ),
                        static_cast<int>(
                            cy + radius * std::sin(angle)
                        )
                    );
                }

                // Draw all graph edges in gray.
                dc.SetPen(wxPen(wxColour(170,170,170), 2));

                for (const auto& edge : edges)
                {
                    dc.DrawLine(
                        points[edge.first],
                        points[edge.second]
                    );
                }

                // Highlight edges within the selected set.
                dc.SetPen(wxPen(wxColour(35,95,210), 4));

                for (const auto& edge : edges)
                {
                    if ((bestMask & (1u << edge.first)) &&
                        (bestMask & (1u << edge.second)))
                    {
                        dc.DrawLine(
                            points[edge.first],
                            points[edge.second]
                        );
                    }
                }

                dc.SetFont(wxFont(
                    13,
                    wxFONTFAMILY_SWISS,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_BOLD
                ));

                for (int i = 0; i < n; ++i)
                {
                    dc.SetPen(wxPen(*wxBLACK, 2));

                    if (bestMask & (1u << i))
                        dc.SetBrush(wxBrush(wxColour(50,120,235)));
                    else
                        dc.SetBrush(wxBrush(wxColour(255,220,0)));

                    dc.DrawCircle(points[i], 22);

                    wxString label =
                        wxString::Format(wxT("%d"), i + 1);

                    wxSize extent = dc.GetTextExtent(label);

                    dc.SetTextForeground(
                        (bestMask & (1u << i))
                            ? *wxWHITE
                            : *wxBLACK
                    );

                    dc.DrawText(
                        label,
                        points[i].x - extent.x / 2,
                        points[i].y - extent.y / 2
                    );
                }
            }
        );

        sizer->Add(panel, 1, wxEXPAND | wxALL, 10);

        wxButton* closeButton =
            new wxButton(&resultDialog, wxID_OK, wxT("Close"));

        sizer->Add(
            closeButton,
            0,
            wxALIGN_CENTER | wxALL,
            10
        );

        resultDialog.SetSizer(sizer);
        resultDialog.Layout();
        resultDialog.CentreOnParent();
        resultDialog.ShowModal();

        return;
    }
    // END MINIMUM CONNECTED DOMINATING SET EXACT

// BEGIN GENERAL MINIMUM WEIGHT HAMILTONIAN PATH
    if (choice == 7)
    {
        struct Edge { int a, b; double w; };
        int n = 0;
        std::vector<Edge> edges;
        wxArrayString methods;
        methods.Add(wxT("Load graph from edge-list file"));
        methods.Add(wxT("Enter graph interactively"));
        wxSingleChoiceDialog methodDialog(this,
            wxT("Choose graph input method"),
            wxT("General Minimum-Weight Hamiltonian Path"), methods);
        if (methodDialog.ShowModal() != wxID_OK) return;
        wxString graphText;
        if (methodDialog.GetSelection() == 0)
        {
            wxFileDialog fileDialog(this, wxT("Open weighted graph edge list"),
                wxEmptyString, wxEmptyString,
                wxT("Graph text files (*.txt;*.graph;*.edges)|*.txt;*.graph;*.edges|All files (*.*)|*.*"),
                wxFD_OPEN | wxFD_FILE_MUST_EXIST);
            if (fileDialog.ShowModal() != wxID_OK) return;
            wxFile file(fileDialog.GetPath());
            if (!file.IsOpened() || !file.ReadAll(&graphText))
            {
                wxMessageBox(wxT("Unable to read the selected graph file."),
                    wxT("Graph Input Error"), wxOK | wxICON_ERROR, this);
                return;
            }
        }
        else
        {
            wxDialog entry(this, wxID_ANY, wxT("Enter Weighted Graph"),
                wxDefaultPosition, wxSize(670, 530),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
            wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
            layout->Add(new wxStaticText(&entry, wxID_ANY,
                wxT("First line: number of vertices (2-16). Subsequent lines: vertex1 vertex2 weight. Only listed edges exist. Vertex numbers start at 1.")),
                0, wxALL | wxEXPAND, 10);
            wxTextCtrl* edit = new wxTextCtrl(&entry, wxID_ANY,
                wxT("6 1 2 2 1 4 3 2 3 4 2 5 1 3 6 2 4 5 5 5 6 6"),
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
            layout->Add(edit, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
            layout->Add(entry.CreateSeparatedButtonSizer(wxOK | wxCANCEL),
                0, wxEXPAND | wxALL, 10);
            entry.SetSizer(layout);
            if (entry.ShowModal() != wxID_OK) return;
            graphText = edit->GetValue();
        }
        // Parse a simple, explicit edge-list format.
        std::istringstream stream(graphText.ToStdString());
        std::string line;
        bool haveVertexCount = false;
        std::set<std::pair<int,int>> seen;
        int lineNumber = 0;
        wxString error;
        while (std::getline(stream, line))
        {
            ++lineNumber;
            const auto comment = line.find('#');
            if (comment != std::string::npos) line.erase(comment);
            std::istringstream row(line);
            std::string first;
            if (!(row >> first)) continue;
            if (!haveVertexCount)
            {
                std::istringstream firstRow(line);
                std::string extra;
                if (!(firstRow >> n) || (firstRow >> extra) || n < 2 || n > 16)
                {
                    error = wxT("First nonempty line must be a vertex count from 2 to 16.");
                    break;
                }
                haveVertexCount = true;
                continue;
            }
            std::istringstream edgeRow(line);
            int u, v;
            double w;
            std::string extra;
            if (!(edgeRow >> u >> v >> w) || (edgeRow >> extra) ||
                u < 1 || v < 1 || u > n || v > n || u == v || !std::isfinite(w))
            {
                error = wxString::Format(wxT("Invalid edge at line %d."), lineNumber);
                break;
            }
            if (u > v) std::swap(u, v);
            if (!seen.insert({u,v}).second)
            {
                error = wxString::Format(wxT("Duplicate edge at line %d."), lineNumber);
                break;
            }
            edges.push_back({u-1,v-1,w});
        }
        if (!haveVertexCount && error.IsEmpty())
            error = wxT("Graph file is empty.");
        if (!error.IsEmpty())
        {
            wxMessageBox(error, wxT("Graph Input Error"), wxOK | wxICON_ERROR, this);
            return;
        }
        const double INF = std::numeric_limits<double>::infinity();
        std::vector<std::vector<double>> weights(n, std::vector<double>(n, INF));
        for (const auto& e : edges)
            weights[e.a][e.b] = weights[e.b][e.a] = e.w;
        const int states = 1 << n;
        std::vector<double> dp(static_cast<size_t>(states)*n, INF);
        std::vector<int> parent(static_cast<size_t>(states)*n, -1);
        auto at = [n](int mask, int v) -> size_t {
            return static_cast<size_t>(mask)*n + v;
        };
        for (int v=0; v<n; ++v) dp[at(1<<v,v)] = 0.0;
        for (int mask=1; mask<states; ++mask)
        {
            for (int u=0; u<n; ++u)
            {
                if (!(mask & (1<<u)) || !std::isfinite(dp[at(mask,u)])) continue;
                for (int v=0; v<n; ++v)
                {
                    if ((mask & (1<<v)) || !std::isfinite(weights[u][v])) continue;
                    const int next = mask | (1<<v);
                    const double cost = dp[at(mask,u)] + weights[u][v];
                    if (cost < dp[at(next,v)])
                    {
                        dp[at(next,v)] = cost;
                        parent[at(next,v)] = u;
                    }
                }
            }
        }
        int last = -1;
        double best = INF;
        for (int v=0; v<n; ++v)
            if (dp[at(states-1,v)] < best)
            {
                best = dp[at(states-1,v)];
                last = v;
            }
        if (last < 0)
        {
            wxMessageBox(wxT("No Hamiltonian path exists in this graph."),
                wxT("Minimum-Weight Hamiltonian Path"), wxOK | wxICON_INFORMATION, this);
            return;
        }
        std::vector<int> path;
        int mask = states-1;
        while (last >= 0)
        {
            path.push_back(last);
            const int previous = parent[at(mask,last)];
            mask &= ~(1<<last);
            last = previous;
        }
        std::reverse(path.begin(),path.end());
        wxString pathLabel = wxT("Path: ");
        for (size_t i=0; i<path.size(); ++i)
        {
            if (i) pathLabel += wxT(" -> ");
            pathLabel += wxString::Format(wxT("%d"),path[i]+1);
        }
        wxDialog result(this,wxID_ANY,wxT("General Minimum-Weight Hamiltonian Path"),
            wxDefaultPosition,wxSize(1200,850),
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
        wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
        layout->Add(new wxStaticText(&result,wxID_ANY,pathLabel),
            0,wxALIGN_CENTER | wxALL,8);
        layout->Add(new wxStaticText(&result,wxID_ANY,
            wxString::Format(wxT("Minimum total path weight: %.10g"),best)),
            0,wxALIGN_CENTER | wxBOTTOM,8);
        wxPanel* panel = new wxPanel(&result);
        panel->SetBackgroundStyle(wxBG_STYLE_PAINT);
        panel->Bind(wxEVT_PAINT,[panel,n,edges,path](wxPaintEvent&)
        {
            wxAutoBufferedPaintDC dc(panel);
            dc.SetBackground(*wxWHITE_BRUSH);
            dc.Clear();
            const wxSize size=panel->GetClientSize();
            const double cx=size.x/2.0,cy=size.y/2.0;
            const double radius=std::max(40.0,
                std::min(size.x,size.y)/2.0-65.0);
            const double pi=3.14159265358979323846;
            std::vector<wxPoint> pos(n);
            for (int i=0;i<n;++i)
            {
                const double angle=-pi/2+2*pi*i/n;
                pos[i]=wxPoint(static_cast<int>(cx+radius*std::cos(angle)),
                    static_cast<int>(cy+radius*std::sin(angle)));
            }
            auto selected=[&path](int u,int v)
            {
                for (size_t k=1;k<path.size();++k)
                    if ((path[k-1]==u && path[k]==v) ||
                        (path[k-1]==v && path[k]==u)) return true;
                return false;
            };
            for (const auto& e:edges)
            {
                const bool on=selected(e.a,e.b);
                dc.SetPen(wxPen(on?wxColour(210,20,20):wxColour(165,165,165),on?4:1));
                dc.DrawLine(pos[e.a],pos[e.b]);
            }

            
            // BEGIN HAMILTONIAN EDGE LABEL COLLISION FIX

            dc.SetFont(wxFont(
                12, wxFONTFAMILY_SWISS,
                wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD
            ));

            struct LabelBox {
                int x, y, w, h;
            };

            std::vector<LabelBox> occupied;

            auto overlaps = [](const LabelBox& a, const LabelBox& b) {
                return a.x < b.x+b.w &&
                       a.x+a.w > b.x &&
                       a.y < b.y+b.h &&
                       a.y+a.h > b.y;
            };

            for (int pass=0; pass<2; ++pass)
            {
                for (const auto& e : edges)
                {
                    bool on = selected(e.a,e.b);
                    if ((pass==0)!=on) continue;

                    const wxPoint a=pos[e.a], b=pos[e.b];
                    const double dx=b.x-a.x, dy=b.y-a.y;
                    const double length=std::hypot(dx,dy);
                    if (length<1) continue;

                    const double nx=-dy/length;
                    const double ny=dx/length;

                    wxString label=wxString::Format(
                        wxT("%.5g"),e.w
                    );

                    wxSize ext=dc.GetTextExtent(label);

                    const double fractions[] = {
                        0.22, 0.78, 0.30, 0.70,
                        0.38, 0.62
                    };

                    const double offsets[] = {
                        16, -16, 28, -28, 40, -40
                    };

                    LabelBox chosen{};
                    bool found=false;

                    for (double f : fractions)
                    {
                        if (found) break;

                        for (double offset : offsets)
                        {
                            double x=a.x+f*dx+offset*nx;
                            double y=a.y+f*dy+offset*ny;

                            LabelBox candidate{
                                static_cast<int>(x-ext.x/2)-5,
                                static_cast<int>(y-ext.y/2)-4,
                                ext.x+10,
                                ext.y+8
                            };

                            if (candidate.x<2 ||
                                candidate.y<2 ||
                                candidate.x+candidate.w>size.x-2 ||
                                candidate.y+candidate.h>size.y-2)
                                continue;

                            bool collision=false;

                            for (const auto& box : occupied)
                            {
                                if (overlaps(candidate,box))
                                {
                                    collision=true;
                                    break;
                                }
                            }

                            if (!collision)
                            {
                                for (const auto& p : pos)
                                {
                                    LabelBox vertex{
                                        p.x-24,p.y-24,48,48
                                    };

                                    if (overlaps(candidate,vertex))
                                    {
                                        collision=true;
                                        break;
                                    }
                                }
                            }

                            if (!collision)
                            {
                                chosen=candidate;
                                found=true;
                                break;
                            }
                        }
                    }

                    if (!found)
                    {
                        chosen={
                            static_cast<int>(a.x+0.22*dx+
                                16*nx-ext.x/2)-5,
                            static_cast<int>(a.y+0.22*dy+
                                16*ny-ext.y/2)-4,
                            ext.x+10,
                            ext.y+8
                        };
                    }

                    occupied.push_back(chosen);

                    dc.SetPen(*wxTRANSPARENT_PEN);
                    dc.SetBrush(*wxWHITE_BRUSH);
                    dc.DrawRectangle(
                        chosen.x,chosen.y,chosen.w,chosen.h
                    );

                    dc.SetTextForeground(
                        on ? wxColour(190,0,0)
                           : wxColour(0,45,140)
                    );

                    dc.DrawText(
                        label,chosen.x+5,chosen.y+4
                    );
                }
            }

            // END HAMILTONIAN EDGE LABEL COLLISION FIX


            dc.SetPen(wxPen(*wxBLACK,2));
            dc.SetBrush(wxBrush(wxColour(255,220,70)));
            dc.SetFont(wxFont(12,wxFONTFAMILY_SWISS,
                wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));
            for (int i=0;i<n;++i)
            {
                dc.DrawCircle(pos[i],18);
                const wxString label=wxString::Format(wxT("%d"),i+1);
                const wxSize ext=dc.GetTextExtent(label);
                dc.SetTextForeground(*wxBLACK);
                dc.DrawText(label,pos[i].x-ext.x/2,pos[i].y-ext.y/2);
            }
        });
        layout->Add(panel,1,wxEXPAND | wxLEFT | wxRIGHT,10);
        layout->Add(new wxButton(&result,wxID_CANCEL,wxT("Close")),
            0,wxALIGN_RIGHT | wxALL,10);
        result.SetSizer(layout);
        result.CentreOnParent();
        result.ShowModal();
        return;
    }
    // END GENERAL MINIMUM WEIGHT HAMILTONIAN PATH


        // BEGIN SUBGRAPH AND TSP VISUALIZATION UPGRADE

        // Subgraph Isomorphism
        if (choice == 4)
        {
            wxArrayString pairs;
            pairs.Add(wxT("Graphs M1 and M2"));
            pairs.Add(wxT("Graphs M3 and M4"));

            wxSingleChoiceDialog pairSelection(
                this,
                wxT("Select a graph pair."),
                wxT("Subgraph Isomorphism"),
                pairs
            );

            if (pairSelection.ShowModal() != wxID_OK)
                return;

            const int pairIndex = pairSelection.GetSelection();

            wxString imageName = pairIndex == 0
                ? wxT("subgraph-isomorphism/")
                  wxT("subgraph-isomorphism-graphs-M1-M2.png")
                : wxT("subgraph-isomorphism/")
                  wxT("subgraph-isomorphism-graphs-M3-M4.png");

            wxString imagePath =
                selectedDirectory + wxT("/") + imageName;

            if (!wxFileExists(imagePath))
            {
                wxMessageBox(
                    wxT("Image not found:\n") + imagePath,
                    wxT("Subgraph Isomorphism"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            if (!wxLaunchDefaultApplication(imagePath))
            {
                wxMessageBox(
                    wxT("Unable to open:\n") + imagePath,
                    wxT("Subgraph Isomorphism"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            struct VertexPair
            {
                wxString first;
                wxString second;
            };

            const std::vector<VertexPair> mapping1 = {
                {wxT("1"), wxT("A")},
                {wxT("2"), wxT("B")},
                {wxT("3"), wxT("C")}
            };

            const std::vector<VertexPair> mapping2 = {
                {wxT("1"), wxT("A")},
                {wxT("2"), wxT("B")},
                {wxT("3"), wxT("C")},
                {wxT("4"), wxT("D")},
                {wxT("5"), wxT("F")},
                {wxT("6"), wxT("G")}
            };

            const auto& mapping =
                pairIndex == 0 ? mapping1 : mapping2;

            wxDialog dialog(
                this,
                wxID_ANY,
                wxT("Subgraph Isomorphism Correspondences"),
                wxDefaultPosition,
                wxSize(440, 390),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);

            layout->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    wxT("Valid vertex mapping preserving all ")
                    wxT("edges of the first graph:")
                ),
                0, wxALL, 12
            );

            wxListCtrl* table = new wxListCtrl(
                &dialog,
                wxID_ANY,
                wxDefaultPosition,
                wxDefaultSize,
                wxLC_REPORT
            );

            table->InsertColumn(
                0,
                wxT("First graph"),
                wxLIST_FORMAT_LEFT,
                160
            );

            table->InsertColumn(
                1,
                wxT("Second graph"),
                wxLIST_FORMAT_LEFT,
                160
            );

            for (const auto& entry : mapping)
            {
                long row = table->InsertItem(
                    table->GetItemCount(),
                    entry.first
                );

                table->SetItem(row, 1, entry.second);
            }

            layout->Add(
                table,
                1,
                wxEXPAND | wxLEFT | wxRIGHT,
                12
            );

            wxButton* closeButton = new wxButton(
                &dialog,
                wxID_CANCEL,
                wxT("Close")
            );

            layout->Add(
                closeButton,
                0,
                wxALIGN_RIGHT | wxALL,
                12
            );

            dialog.SetSizer(layout);
            dialog.CentreOnParent();
            dialog.ShowModal();

            return;
        }

        
        
        
        
        
        // BEGIN MAXIMUM INDEPENDENT SET
        if (choice == 9)
        {
            wxTextEntryDialog entry(
                this,
                wxT("Enter the number of vertices on the first line,\n")
                wxT("followed by one undirected edge per line.\n")
                wxT("Vertices are numbered 1 through n.\n")
                wxT("Maximum: 20 vertices."),
                wxT("Maximum Independent Vertex Set - Graph Input"),
                wxEmptyString,
                wxOK | wxCANCEL | wxTE_MULTILINE
            );

            entry.SetSize(wxSize(650, 550));

            if (entry.ShowModal() != wxID_OK)
                return;

            std::istringstream input{
                std::string(entry.GetValue().mb_str())
            };

            int n = 0;

            if (!(input >> n) || n < 1 || n > 20)
            {
                wxMessageBox(
                    wxT("Enter a vertex count from 1 to 20."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            std::vector<std::pair<int,int>> edges;
            std::vector<unsigned int> adjacent(n, 0u);

            int a, b;

            while (input >> a)
            {
                if (!(input >> b))
                {
                    wxMessageBox(
                        wxT("Every edge must have two endpoints."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                if (a < 1 || a > n ||
                    b < 1 || b > n || a == b)
                {
                    wxMessageBox(
                        wxT("Invalid edge endpoints."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                --a;
                --b;

                if ((adjacent[a] & (1u << b)) == 0u)
                {
                    adjacent[a] |= (1u << b);
                    adjacent[b] |= (1u << a);
                    edges.push_back(std::make_pair(a,b));
                }
            }

            if (!input.eof())
            {
                wxMessageBox(
                    wxT("Graph input contains invalid characters."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            // Exact maximum independent set search.
            unsigned int bestMask = 0u;
            int bestCount = 0;

            const unsigned int limit = 1u << n;

            std::vector<unsigned char> independent(limit, 0);
            independent[0] = 1;

            for (unsigned int mask = 1u; mask < limit; ++mask)
            {
                const unsigned int bit = mask & (~mask + 1u);
                const unsigned int rest = mask ^ bit;

                int vertex = 0;
                unsigned int temp = bit;

                while (temp > 1u)
                {
                    temp >>= 1;
                    ++vertex;
                }

                if (independent[rest] &&
                    (adjacent[vertex] & rest) == 0u)
                {
                    independent[mask] = 1;

                    int count = 0;
                    unsigned int bits = mask;

                    while (bits)
                    {
                        bits &= bits - 1u;
                        ++count;
                    }

                    if (count > bestCount)
                    {
                        bestCount = count;
                        bestMask = mask;
                    }
                }
            }

            wxString result = wxT("Maximum independent set: {");

            bool first = true;

            for (int i = 0; i < n; ++i)
            {
                if ((bestMask & (1u << i)) != 0u)
                {
                    if (!first)
                        result += wxT(", ");

                    result += wxString::Format(wxT("%d"), i+1);
                    first = false;
                }
            }

            result += wxT("}");

            wxDialog dialog(
                this,
                wxID_ANY,
                wxT("Maximum Independent Vertex Set - Unweighted Graph"),
                wxDefaultPosition,
                wxSize(1200, 850),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    result
                ),
                0,
                wxALIGN_CENTER | wxALL,
                12
            );

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    wxString::Format(
                        wxT("Independent set size: %d vertices"),
                        bestCount
                    )
                ),
                0,
                wxALIGN_CENTER | wxALL,
                8
            );

            wxPanel* panel = new wxPanel(&dialog);
            panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

            panel->Bind(
                wxEVT_PAINT,
                [panel, n, edges, bestMask](wxPaintEvent&)
                {
                    wxAutoBufferedPaintDC dc(panel);
                    dc.SetBackground(*wxWHITE_BRUSH);
                    dc.Clear();

                    const wxSize size = panel->GetClientSize();

                    const double pi = 3.14159265358979323846;
                    const double cx = size.x / 2.0;
                    const double cy = size.y / 2.0;

                    const double radius = std::max(
                        40.0,
                        std::min(size.x, size.y) / 2.0 - 65.0
                    );

                    std::vector<wxPoint> points(n);

                    for (int i = 0; i < n; ++i)
                    {
                        const double angle =
                            -pi / 2.0 + 2.0 * pi * i / n;

                        points[i] = wxPoint(
                            static_cast<int>(
                                cx + radius * std::cos(angle)
                            ),
                            static_cast<int>(
                                cy + radius * std::sin(angle)
                            )
                        );
                    }

                    // Draw graph edges.
                    dc.SetPen(
                        wxPen(wxColour(155,155,155), 2)
                    );

                    for (const auto& e : edges)
                    {
                        dc.DrawLine(
                            points[e.first],
                            points[e.second]
                        );
                    }

                    // Draw vertices.
                    dc.SetPen(wxPen(*wxBLACK, 2));

                    dc.SetFont(wxFont(
                        13,
                        wxFONTFAMILY_SWISS,
                        wxFONTSTYLE_NORMAL,
                        wxFONTWEIGHT_BOLD
                    ));

                    for (int i = 0; i < n; ++i)
                    {
                        const bool selected =
                            (bestMask & (1u << i)) != 0u;

                        dc.SetBrush(
                            wxBrush(
                                selected
                                    ? wxColour(80,210,100)
                                    : wxColour(255,220,0)
                            )
                        );

                        dc.DrawCircle(points[i], 22);

                        const wxString label =
                            wxString::Format(wxT("%d"), i+1);

                        const wxSize extent =
                            dc.GetTextExtent(label);

                        dc.SetTextForeground(*wxBLACK);

                        dc.DrawText(
                            label,
                            points[i].x - extent.x/2,
                            points[i].y - extent.y/2
                        );
                    }
                }
            );

            sizer->Add(
                panel,
                1,
                wxEXPAND | wxALL,
                10
            );

            wxButton* closeButton =
                new wxButton(&dialog, wxID_OK, wxT("Close"));

            sizer->Add(
                closeButton,
                0,
                wxALIGN_CENTER | wxALL,
                10
            );

            dialog.SetSizer(sizer);
            dialog.Layout();
            dialog.CentreOnParent();
            dialog.ShowModal();

            return;
        }
        // END MAXIMUM INDEPENDENT SET


        // BEGIN MINIMUM VERTEX COVER EXACT
        if (choice == 10)
        {
            wxTextEntryDialog entry(
                this,
                wxT("Enter the number of vertices on the first line,\n")
                wxT("followed by one undirected edge per line.\n")
                wxT("Vertices are numbered 1 through n.\n")
                wxT("Maximum: 20 vertices."),
                wxT("Minimum Vertex Cover - Graph Input"),
                wxEmptyString,
                wxOK | wxCANCEL | wxTE_MULTILINE
            );

            entry.SetSize(wxSize(650, 550));

            if (entry.ShowModal() != wxID_OK)
                return;

            std::istringstream input{
                std::string(entry.GetValue().mb_str())
            };

            int n = 0;

            if (!(input >> n) || n < 1 || n > 20)
            {
                wxMessageBox(
                    wxT("Enter a vertex count from 1 to 20."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            std::vector<std::pair<int,int>> edges;
            std::vector<unsigned int> adjacent(n, 0u);

            int a, b;

            while (input >> a)
            {
                if (!(input >> b))
                {
                    wxMessageBox(
                        wxT("Each edge must contain two vertex numbers."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                if (a < 1 || a > n ||
                    b < 1 || b > n || a == b)
                {
                    wxMessageBox(
                        wxT("Invalid edge endpoints."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                --a;
                --b;

                if ((adjacent[a] & (1u << b)) == 0u)
                {
                    adjacent[a] |= (1u << b);
                    adjacent[b] |= (1u << a);
                    edges.push_back(std::make_pair(a,b));
                }
            }

            if (!input.eof())
            {
                wxMessageBox(
                    wxT("Graph input contains invalid characters."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            // A vertex cover is the complement of
            // an independent vertex set.
            //
            // Therefore, the complement of a maximum
            // independent set is a minimum vertex cover.
            //
            // This exact subset algorithm supports n <= 20.

            const unsigned int limit = 1u << n;

            std::vector<unsigned char> independent(limit, 0);
            independent[0] = 1;

            unsigned int maximumIndependentMask = 0u;
            int maximumIndependentSize = 0;

            for (unsigned int mask = 1u; mask < limit; ++mask)
            {
                const unsigned int bit = mask & (~mask + 1u);
                const unsigned int rest = mask ^ bit;

                int vertex = 0;
                unsigned int temp = bit;

                while (temp > 1u)
                {
                    temp >>= 1;
                    ++vertex;
                }

                if (independent[rest] &&
                    (adjacent[vertex] & rest) == 0u)
                {
                    independent[mask] = 1;

                    int count = 0;
                    unsigned int bits = mask;

                    while (bits != 0u)
                    {
                        bits &= bits - 1u;
                        ++count;
                    }

                    if (count > maximumIndependentSize)
                    {
                        maximumIndependentSize = count;
                        maximumIndependentMask = mask;
                    }
                }
            }

            const unsigned int allVertices = limit - 1u;

            const unsigned int coverMask =
                allVertices ^ maximumIndependentMask;

            const int coverSize =
                n - maximumIndependentSize;

            wxString result =
                wxT("Minimum vertex cover: {");

            bool first = true;

            for (int i = 0; i < n; ++i)
            {
                if ((coverMask & (1u << i)) != 0u)
                {
                    if (!first)
                        result += wxT(", ");

                    result += wxString::Format(
                        wxT("%d"), i + 1
                    );

                    first = false;
                }
            }

            result += wxT("}");

            wxDialog dialog(
                this,
                wxID_ANY,
                wxT("Minimum Vertex Cover (Exact)"),
                wxDefaultPosition,
                wxSize(1200, 850),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    result
                ),
                0,
                wxALIGN_CENTER | wxALL,
                12
            );

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    wxString::Format(
                        wxT("Minimum vertex cover size: %d vertices"),
                        coverSize
                    )
                ),
                0,
                wxALIGN_CENTER | wxALL,
                8
            );

            wxPanel* panel = new wxPanel(&dialog);
            panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

            panel->Bind(
                wxEVT_PAINT,
                [panel, n, edges, coverMask](wxPaintEvent&)
                {
                    wxAutoBufferedPaintDC dc(panel);

                    dc.SetBackground(*wxWHITE_BRUSH);
                    dc.Clear();

                    const wxSize size = panel->GetClientSize();

                    const double pi =
                        3.14159265358979323846;

                    const double cx = size.x / 2.0;
                    const double cy = size.y / 2.0;

                    const double radius = std::max(
                        40.0,
                        std::min(size.x, size.y) / 2.0 - 65.0
                    );

                    std::vector<wxPoint> points(n);

                    for (int i = 0; i < n; ++i)
                    {
                        const double angle =
                            -pi / 2.0 + 2.0 * pi * i / n;

                        points[i] = wxPoint(
                            static_cast<int>(
                                cx + radius * std::cos(angle)
                            ),
                            static_cast<int>(
                                cy + radius * std::sin(angle)
                            )
                        );
                    }

                    // Draw the undirected edges.
                    dc.SetPen(
                        wxPen(wxColour(150,150,150), 2)
                    );

                    for (const auto& e : edges)
                    {
                        dc.DrawLine(
                            points[e.first],
                            points[e.second]
                        );
                    }

                    // Draw selected vertices in blue.
                    // Unselected vertices are yellow.

                    dc.SetPen(wxPen(*wxBLACK, 2));

                    dc.SetFont(wxFont(
                        13,
                        wxFONTFAMILY_SWISS,
                        wxFONTSTYLE_NORMAL,
                        wxFONTWEIGHT_BOLD
                    ));

                    for (int i = 0; i < n; ++i)
                    {
                        const bool selected =
                            (coverMask & (1u << i)) != 0u;

                        dc.SetBrush(
                            wxBrush(
                                selected
                                    ? wxColour(70,140,240)
                                    : wxColour(255,220,0)
                            )
                        );

                        dc.DrawCircle(points[i], 22);

                        const wxString label =
                            wxString::Format(wxT("%d"), i+1);

                        const wxSize extent =
                            dc.GetTextExtent(label);

                        dc.SetTextForeground(
                            selected
                                ? *wxWHITE
                                : *wxBLACK
                        );

                        dc.DrawText(
                            label,
                            points[i].x - extent.x/2,
                            points[i].y - extent.y/2
                        );
                    }
                }
            );

            sizer->Add(
                panel,
                1,
                wxEXPAND | wxALL,
                10
            );

            wxButton* closeButton =
                new wxButton(
                    &dialog, wxID_OK, wxT("Close")
                );

            sizer->Add(
                closeButton,
                0,
                wxALIGN_CENTER | wxALL,
                10
            );

            dialog.SetSizer(sizer);
            dialog.Layout();
            dialog.CentreOnParent();
            dialog.ShowModal();

            return;
        }
        // END MINIMUM VERTEX COVER EXACT


        // BEGIN MINIMUM DOMINATING SET EXACT
        if (choice == 11)
        {
            wxTextEntryDialog entry(
                this,
                wxT("Enter the number of vertices on the first line,\n")
                wxT("followed by one undirected edge per line.\n")
                wxT("Vertices are numbered 1 through n.\n")
                wxT("Maximum: 20 vertices."),
                wxT("Minimum Dominating Set - Graph Input"),
                wxEmptyString,
                wxOK | wxCANCEL | wxTE_MULTILINE
            );

            entry.SetSize(wxSize(650, 550));

            if (entry.ShowModal() != wxID_OK)
                return;

            std::istringstream input{
                std::string(entry.GetValue().mb_str())
            };

            int n = 0;

            if (!(input >> n) || n < 1 || n > 20)
            {
                wxMessageBox(
                    wxT("Enter a vertex count from 1 to 20."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            std::vector<std::pair<int,int>> edges;
            std::vector<unsigned int> adjacent(n, 0u);

            int a, b;

            while (input >> a)
            {
                if (!(input >> b))
                {
                    wxMessageBox(
                        wxT("Each edge must contain two vertex numbers."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                if (a < 1 || a > n ||
                    b < 1 || b > n || a == b)
                {
                    wxMessageBox(
                        wxT("Invalid edge endpoints."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                --a;
                --b;

                if ((adjacent[a] & (1u << b)) == 0u)
                {
                    adjacent[a] |= (1u << b);
                    adjacent[b] |= (1u << a);
                    edges.push_back(std::make_pair(a,b));
                }
            }

            if (!input.eof())
            {
                wxMessageBox(
                    wxT("Graph input contains invalid characters."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            // Each selected vertex dominates itself
            // and all of its adjacent vertices.
            //
            // Find a minimum-cardinality subset whose
            // combined closed neighborhoods cover
            // every vertex.
            //
            // Exact subset dynamic programming:
            // O(2^n) time and O(2^n) memory.

            const unsigned int limit = 1u << n;
            const unsigned int allVertices = limit - 1u;

            std::vector<unsigned int> closed(n);

            for (int i = 0; i < n; ++i)
                closed[i] = adjacent[i] | (1u << i);

            std::vector<unsigned int> dominated(limit, 0u);

            unsigned int bestMask = allVertices;
            int bestSize = n;

            // Iterate subsets in increasing numerical order.
            // Their domination masks are computed from
            // a previously processed subset.

            for (unsigned int mask = 1u; mask < limit; ++mask)
            {
                const unsigned int bit = mask & (~mask + 1u);
                const unsigned int rest = mask ^ bit;

                int vertex = 0;
                unsigned int temp = bit;

                while (temp > 1u)
                {
                    temp >>= 1;
                    ++vertex;
                }

                dominated[mask] =
                    dominated[rest] | closed[vertex];

                if (dominated[mask] == allVertices)
                {
                    int count = 0;
                    unsigned int bits = mask;

                    while (bits != 0u)
                    {
                        bits &= bits - 1u;
                        ++count;
                    }

                    if (count < bestSize)
                    {
                        bestSize = count;
                        bestMask = mask;
                    }
                }
            }

            wxString result =
                wxT("Minimum dominating set: {");

            bool first = true;

            for (int i = 0; i < n; ++i)
            {
                if ((bestMask & (1u << i)) != 0u)
                {
                    if (!first)
                        result += wxT(", ");

                    result += wxString::Format(
                        wxT("%d"), i + 1
                    );

                    first = false;
                }
            }

            result += wxT("}");

            wxDialog dialog(
                this,
                wxID_ANY,
                wxT("Minimum Dominating Set (Exact)"),
                wxDefaultPosition,
                wxSize(1200, 850),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    result
                ),
                0,
                wxALIGN_CENTER | wxALL,
                12
            );

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    wxString::Format(
                        wxT("Minimum dominating set size: %d vertices"),
                        bestSize
                    )
                ),
                0,
                wxALIGN_CENTER | wxALL,
                8
            );

            wxPanel* panel = new wxPanel(&dialog);
            panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

            panel->Bind(
                wxEVT_PAINT,
                [panel, n, edges, bestMask](wxPaintEvent&)
                {
                    wxAutoBufferedPaintDC dc(panel);

                    dc.SetBackground(*wxWHITE_BRUSH);
                    dc.Clear();

                    const wxSize size = panel->GetClientSize();

                    const double pi =
                        3.14159265358979323846;

                    const double cx = size.x / 2.0;
                    const double cy = size.y / 2.0;

                    const double radius = std::max(
                        40.0,
                        std::min(size.x, size.y) / 2.0 - 65.0
                    );

                    std::vector<wxPoint> points(n);

                    for (int i = 0; i < n; ++i)
                    {
                        const double angle =
                            -pi / 2.0 + 2.0 * pi * i / n;

                        points[i] = wxPoint(
                            static_cast<int>(
                                cx + radius * std::cos(angle)
                            ),
                            static_cast<int>(
                                cy + radius * std::sin(angle)
                            )
                        );
                    }

                    // Draw graph edges.

                    dc.SetPen(
                        wxPen(wxColour(150,150,150), 2)
                    );

                    for (const auto& e : edges)
                    {
                        dc.DrawLine(
                            points[e.first],
                            points[e.second]
                        );
                    }

                    // Blue = selected dominating vertex.
                    // Yellow = unselected vertex.

                    dc.SetPen(wxPen(*wxBLACK, 2));

                    dc.SetFont(wxFont(
                        13,
                        wxFONTFAMILY_SWISS,
                        wxFONTSTYLE_NORMAL,
                        wxFONTWEIGHT_BOLD
                    ));

                    for (int i = 0; i < n; ++i)
                    {
                        const bool selected =
                            (bestMask & (1u << i)) != 0u;

                        dc.SetBrush(
                            wxBrush(
                                selected
                                    ? wxColour(70,140,240)
                                    : wxColour(255,220,0)
                            )
                        );

                        dc.DrawCircle(points[i], 22);

                        const wxString label =
                            wxString::Format(wxT("%d"), i + 1);

                        const wxSize extent =
                            dc.GetTextExtent(label);

                        dc.SetTextForeground(
                            selected
                                ? *wxWHITE
                                : *wxBLACK
                        );

                        dc.DrawText(
                            label,
                            points[i].x - extent.x / 2,
                            points[i].y - extent.y / 2
                        );
                    }
                }
            );

            sizer->Add(
                panel,
                1,
                wxEXPAND | wxALL,
                10
            );

            wxButton* closeButton =
                new wxButton(
                    &dialog,
                    wxID_OK,
                    wxT("Close")
                );

            sizer->Add(
                closeButton,
                0,
                wxALIGN_CENTER | wxALL,
                10
            );

            dialog.SetSizer(sizer);
            dialog.Layout();
            dialog.CentreOnParent();
            dialog.ShowModal();

            return;
        }
        // END MINIMUM DOMINATING SET EXACT


        // BEGIN MINIMUM EDGE DOMINATING SET EXACT
        if (choice == 12)
        {
            wxTextEntryDialog entry(
                this,
                wxT("Enter the number of vertices on the first line,\n")
                wxT("followed by one undirected edge per line.\n")
                wxT("Vertices are numbered 1 through n.\n")
                wxT("Maximum: 20 vertices and 20 edges."),
                wxT("Minimum Edge Dominating Set - Graph Input"),
                wxEmptyString,
                wxOK | wxCANCEL | wxTE_MULTILINE
            );

            entry.SetSize(wxSize(650, 550));

            if (entry.ShowModal() != wxID_OK)
                return;

            std::istringstream input{
                std::string(entry.GetValue().mb_str())
            };

            int n = 0;

            if (!(input >> n) || n < 1 || n > 20)
            {
                wxMessageBox(
                    wxT("Enter a vertex count from 1 to 20."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            std::vector<std::pair<int,int>> edges;
            std::vector<unsigned int> adjacent(n, 0u);

            int a, b;

            while (input >> a)
            {
                if (!(input >> b))
                {
                    wxMessageBox(
                        wxT("Each edge must contain two vertex numbers."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                if (a < 1 || a > n ||
                    b < 1 || b > n || a == b)
                {
                    wxMessageBox(
                        wxT("Invalid edge endpoints."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                --a;
                --b;

                if ((adjacent[a] & (1u << b)) == 0u)
                {
                    adjacent[a] |= (1u << b);
                    adjacent[b] |= (1u << a);
                    edges.push_back(std::make_pair(a,b));
                }

                if (edges.size() > 20)
                {
                    wxMessageBox(
                        wxT("This exact algorithm supports at most 20 edges."),
                        wxT("Graph Too Large"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }
            }

            if (!input.eof())
            {
                wxMessageBox(
                    wxT("Graph input contains invalid characters."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            const int m = static_cast<int>(edges.size());

            // Each edge dominates itself and every edge
            // sharing at least one endpoint with it.
            //
            // Enumerate edge subsets exactly.
            // O(2^m * m) time, O(2^m) memory.

            const unsigned int limit = 1u << m;
            const unsigned int allEdges = limit - 1u;

            std::vector<unsigned int> closed(m, 0u);

            for (int i = 0; i < m; ++i)
            {
                for (int j = 0; j < m; ++j)
                {
                    if (edges[i].first == edges[j].first ||
                        edges[i].first == edges[j].second ||
                        edges[i].second == edges[j].first ||
                        edges[i].second == edges[j].second)
                    {
                        closed[i] |= (1u << j);
                    }
                }
            }

            std::vector<unsigned int> dominated(limit, 0u);

            unsigned int bestMask = allEdges;
            int bestSize = m;

            for (unsigned int mask = 1u; mask < limit; ++mask)
            {
                const unsigned int bit = mask & (~mask + 1u);
                const unsigned int rest = mask ^ bit;

                int edgeIndex = 0;
                unsigned int temp = bit;

                while (temp > 1u)
                {
                    temp >>= 1;
                    ++edgeIndex;
                }

                dominated[mask] =
                    dominated[rest] | closed[edgeIndex];

                if (dominated[mask] == allEdges)
                {
                    int count = 0;
                    unsigned int bits = mask;

                    while (bits != 0u)
                    {
                        bits &= bits - 1u;
                        ++count;
                    }

                    if (count < bestSize)
                    {
                        bestSize = count;
                        bestMask = mask;
                    }
                }
            }

            wxString result =
                wxT("Minimum edge dominating set: {");

            bool first = true;

            for (int i = 0; i < m; ++i)
            {
                if ((bestMask & (1u << i)) != 0u)
                {
                    if (!first)
                        result += wxT(", ");

                    result += wxString::Format(
                        wxT("(%d,%d)"),
                        edges[i].first + 1,
                        edges[i].second + 1
                    );

                    first = false;
                }
            }

            result += wxT("}");

            wxDialog dialog(
                this,
                wxID_ANY,
                wxT("Minimum Edge Dominating Set (Exact)"),
                wxDefaultPosition,
                wxSize(1200, 850),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    result
                ),
                0,
                wxALIGN_CENTER | wxALL,
                12
            );

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    wxString::Format(
                        wxT("Minimum edge dominating set size: %d edges"),
                        bestSize
                    )
                ),
                0,
                wxALIGN_CENTER | wxALL,
                8
            );

            wxPanel* panel = new wxPanel(&dialog);
            panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

            panel->Bind(
                wxEVT_PAINT,
                [panel, n, edges, bestMask](wxPaintEvent&)
                {
                    wxAutoBufferedPaintDC dc(panel);

                    dc.SetBackground(*wxWHITE_BRUSH);
                    dc.Clear();

                    const wxSize size = panel->GetClientSize();

                    const double pi =
                        3.14159265358979323846;

                    const double cx = size.x / 2.0;
                    const double cy = size.y / 2.0;

                    const double radius = std::max(
                        40.0,
                        std::min(size.x, size.y) / 2.0 - 65.0
                    );

                    std::vector<wxPoint> points(n);

                    for (int i = 0; i < n; ++i)
                    {
                        const double angle =
                            -pi / 2.0 + 2.0 * pi * i / n;

                        points[i] = wxPoint(
                            static_cast<int>(
                                cx + radius * std::cos(angle)
                            ),
                            static_cast<int>(
                                cy + radius * std::sin(angle)
                            )
                        );
                    }

                    // Draw unselected edges in gray.

                    dc.SetPen(
                        wxPen(wxColour(170,170,170), 2)
                    );

                    for (int i = 0; i < static_cast<int>(edges.size()); ++i)
                    {
                        if ((bestMask & (1u << i)) == 0u)
                        {
                            dc.DrawLine(
                                points[edges[i].first],
                                points[edges[i].second]
                            );
                        }
                    }

                    // Draw selected edges in red.

                    dc.SetPen(
                        wxPen(wxColour(220,30,30), 5)
                    );

                    for (int i = 0; i < static_cast<int>(edges.size()); ++i)
                    {
                        if ((bestMask & (1u << i)) != 0u)
                        {
                            dc.DrawLine(
                                points[edges[i].first],
                                points[edges[i].second]
                            );
                        }
                    }

                    // Draw vertices in yellow.

                    dc.SetPen(wxPen(*wxBLACK, 2));
                    dc.SetBrush(
                        wxBrush(wxColour(255,220,0))
                    );

                    dc.SetFont(wxFont(
                        13,
                        wxFONTFAMILY_SWISS,
                        wxFONTSTYLE_NORMAL,
                        wxFONTWEIGHT_BOLD
                    ));

                    dc.SetTextForeground(*wxBLACK);

                    for (int i = 0; i < n; ++i)
                    {
                        dc.DrawCircle(points[i], 22);

                        const wxString label =
                            wxString::Format(wxT("%d"), i + 1);

                        const wxSize extent =
                            dc.GetTextExtent(label);

                        dc.DrawText(
                            label,
                            points[i].x - extent.x / 2,
                            points[i].y - extent.y / 2
                        );
                    }
                }
            );

            sizer->Add(
                panel,
                1,
                wxEXPAND | wxALL,
                10
            );

            wxButton* closeButton =
                new wxButton(
                    &dialog,
                    wxID_OK,
                    wxT("Close")
                );

            sizer->Add(
                closeButton,
                0,
                wxALIGN_CENTER | wxALL,
                10
            );

            dialog.SetSizer(sizer);
            dialog.Layout();
            dialog.CentreOnParent();
            dialog.ShowModal();

            return;
        }
        // END MINIMUM EDGE DOMINATING SET EXACT


        // BEGIN MINIMUM FEEDBACK VERTEX SET EXACT
        if (choice == 13)
        {
            wxTextEntryDialog entry(
                this,
                wxT("Enter the number of vertices on the first line,\n")
                wxT("followed by one undirected edge per line.\n")
                wxT("Vertices are numbered 1 through n.\n")
                wxT("Maximum: 20 vertices."),
                wxT("Minimum Feedback Vertex Set - Graph Input"),
                wxEmptyString,
                wxOK | wxCANCEL | wxTE_MULTILINE
            );

            entry.SetSize(wxSize(650, 550));

            if (entry.ShowModal() != wxID_OK)
                return;

            std::istringstream input{
                std::string(entry.GetValue().mb_str())
            };

            int n = 0;

            if (!(input >> n) || n < 1 || n > 20)
            {
                wxMessageBox(
                    wxT("Enter a vertex count from 1 to 20."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            std::vector<std::pair<int,int>> edges;
            std::vector<unsigned int> adjacent(n, 0u);

            int a, b;

            while (input >> a)
            {
                if (!(input >> b))
                {
                    wxMessageBox(
                        wxT("Each edge must contain two vertex numbers."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                if (a < 1 || a > n ||
                    b < 1 || b > n || a == b)
                {
                    wxMessageBox(
                        wxT("Invalid edge endpoints."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                --a;
                --b;

                if ((adjacent[a] & (1u << b)) == 0u)
                {
                    adjacent[a] |= (1u << b);
                    adjacent[b] |= (1u << a);
                    edges.push_back(std::make_pair(a,b));
                }
            }

            if (!input.eof())
            {
                wxMessageBox(
                    wxT("Graph input contains invalid characters."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            // An undirected graph is acyclic if and only if
            // every connected component is a tree.
            //
            // A forest with k vertices and c connected
            // components has exactly k-c edges.
            //
            // Compute which vertex subsets induce forests.
            // Find the largest induced forest.
            // Its complement is a minimum feedback vertex set.
            //
            // Exact subset dynamic programming:
            // O(n * 2^n) time, O(2^n) memory.

            const unsigned int limit = 1u << n;
            const unsigned int allVertices = limit - 1u;

            std::vector<unsigned char> isForest(limit, 0);
            std::vector<unsigned char> vertexCount(limit, 0);
            std::vector<unsigned char> componentCount(limit, 0);
            std::vector<unsigned short> edgeCount(limit, 0);

            isForest[0] = 1;

            unsigned int bestForestMask = 0u;
            int bestForestSize = 0;

            for (unsigned int mask = 1u; mask < limit; ++mask)
            {
                const unsigned int bit = mask & (~mask + 1u);
                const unsigned int rest = mask ^ bit;

                int vertex = 0;
                unsigned int temp = bit;

                while (temp > 1u)
                {
                    temp >>= 1;
                    ++vertex;
                }

                vertexCount[mask] =
                    static_cast<unsigned char>(
                        vertexCount[rest] + 1
                    );

                // Count edges from the new vertex to
                // vertices already in the subset.

                unsigned int neighbors =
                    adjacent[vertex] & rest;

                int newEdges = 0;

                while (neighbors != 0u)
                {
                    neighbors &= neighbors - 1u;
                    ++newEdges;
                }

                edgeCount[mask] =
                    static_cast<unsigned short>(
                        edgeCount[rest] + newEdges
                    );

                // Count connected components of the
                // induced subgraph using bitmask traversal.

                unsigned int unseen = mask;
                int components = 0;

                while (unseen != 0u)
                {
                    ++components;

                    const unsigned int start =
                        unseen & (~unseen + 1u);

                    unseen ^= start;

                    unsigned int frontier = start;

                    while (frontier != 0u)
                    {
                        const unsigned int currentBit =
                            frontier & (~frontier + 1u);

                        frontier ^= currentBit;

                        int currentVertex = 0;
                        unsigned int currentTemp = currentBit;

                        while (currentTemp > 1u)
                        {
                            currentTemp >>= 1;
                            ++currentVertex;
                        }

                        const unsigned int newlyReached =
                            adjacent[currentVertex] & unseen;

                        unseen &= ~newlyReached;
                        frontier |= newlyReached;
                    }
                }

                componentCount[mask] =
                    static_cast<unsigned char>(components);

                // Forest condition:
                // edges = vertices - components.

                if (edgeCount[mask] ==
                    vertexCount[mask] - componentCount[mask])
                {
                    isForest[mask] = 1;

                    if (vertexCount[mask] > bestForestSize)
                    {
                        bestForestSize = vertexCount[mask];
                        bestForestMask = mask;
                    }
                }
            }

            const unsigned int feedbackMask =
                allVertices ^ bestForestMask;

            const int feedbackSize =
                n - bestForestSize;

            wxString result =
                wxT("Minimum feedback vertex set: {");

            bool first = true;

            for (int i = 0; i < n; ++i)
            {
                if ((feedbackMask & (1u << i)) != 0u)
                {
                    if (!first)
                        result += wxT(", ");

                    result += wxString::Format(
                        wxT("%d"), i + 1
                    );

                    first = false;
                }
            }

            result += wxT("}");

            wxDialog dialog(
                this,
                wxID_ANY,
                wxT("Minimum Feedback Vertex Set (Exact)"),
                wxDefaultPosition,
                wxSize(1200, 850),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    result
                ),
                0,
                wxALIGN_CENTER | wxALL,
                12
            );

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    wxString::Format(
                        wxT("Minimum feedback vertex set size: %d vertices"),
                        feedbackSize
                    )
                ),
                0,
                wxALIGN_CENTER | wxALL,
                8
            );

            wxPanel* panel = new wxPanel(&dialog);
            panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

            panel->Bind(
                wxEVT_PAINT,
                [panel, n, edges, feedbackMask](wxPaintEvent&)
                {
                    wxAutoBufferedPaintDC dc(panel);

                    dc.SetBackground(*wxWHITE_BRUSH);
                    dc.Clear();

                    const wxSize size = panel->GetClientSize();

                    const double pi =
                        3.14159265358979323846;

                    const double cx = size.x / 2.0;
                    const double cy = size.y / 2.0;

                    const double radius = std::max(
                        40.0,
                        std::min(size.x, size.y) / 2.0 - 65.0
                    );

                    std::vector<wxPoint> points(n);

                    for (int i = 0; i < n; ++i)
                    {
                        const double angle =
                            -pi / 2.0 + 2.0 * pi * i / n;

                        points[i] = wxPoint(
                            static_cast<int>(
                                cx + radius * std::cos(angle)
                            ),
                            static_cast<int>(
                                cy + radius * std::sin(angle)
                            )
                        );
                    }

                    // Draw original graph edges in gray.

                    dc.SetPen(
                        wxPen(wxColour(170,170,170), 2)
                    );

                    for (const auto& e : edges)
                    {
                        dc.DrawLine(
                            points[e.first],
                            points[e.second]
                        );
                    }

                    // Blue = vertex selected for removal.
                    // Yellow = vertex retained in the forest.

                    dc.SetPen(wxPen(*wxBLACK, 2));

                    dc.SetFont(wxFont(
                        13,
                        wxFONTFAMILY_SWISS,
                        wxFONTSTYLE_NORMAL,
                        wxFONTWEIGHT_BOLD
                    ));

                    for (int i = 0; i < n; ++i)
                    {
                        const bool selected =
                            (feedbackMask & (1u << i)) != 0u;

                        dc.SetBrush(
                            wxBrush(
                                selected
                                    ? wxColour(70,140,240)
                                    : wxColour(255,220,0)
                            )
                        );

                        dc.DrawCircle(points[i], 22);

                        const wxString label =
                            wxString::Format(wxT("%d"), i + 1);

                        const wxSize extent =
                            dc.GetTextExtent(label);

                        dc.SetTextForeground(
                            selected
                                ? *wxWHITE
                                : *wxBLACK
                        );

                        dc.DrawText(
                            label,
                            points[i].x - extent.x / 2,
                            points[i].y - extent.y / 2
                        );
                    }
                }
            );

            sizer->Add(
                panel,
                1,
                wxEXPAND | wxALL,
                10
            );

            wxButton* closeButton =
                new wxButton(
                    &dialog,
                    wxID_OK,
                    wxT("Close")
                );

            sizer->Add(
                closeButton,
                0,
                wxALIGN_CENTER | wxALL,
                10
            );

            dialog.SetSizer(sizer);
            dialog.Layout();
            dialog.CentreOnParent();
            dialog.ShowModal();

            return;
        }
        // END MINIMUM FEEDBACK VERTEX SET EXACT

// BEGIN SHORTEST HAMILTONIAN PATH
        if (choice == 6)
        {
            // Weighted complete graph K6.
            // Vertex positions are independent of edge weights.
            const int circularOrder[6] = {
                4, 5, 3, 2, 1, 6
            };

            double weights[7][7] = {};

            for (int i = 0; i < 6; ++i)
            {
                for (int j = i + 1; j < 6; ++j)
                {
                    const int separation =
                        std::min(j - i, 6 - (j - i));

                    const double weight =
                        separation == 1 ? 1.0 :
                        separation == 2 ? 1.73205 : 2.0;

                    const int a = circularOrder[i];
                    const int b = circularOrder[j];

                    weights[a][b] = weight;
                    weights[b][a] = weight;
                }
            }

            // Exhaustively examine all 6! vertex orders.
            std::vector<int> permutation = {
                1, 2, 3, 4, 5, 6
            };

            std::vector<int> bestPath;
            double bestDistance =
                std::numeric_limits<double>::infinity();

            do
            {
                double distance = 0.0;

                for (int k = 0; k < 5; ++k)
                {
                    distance += weights[
                        permutation[k]
                    ][
                        permutation[k + 1]
                    ];
                }

                if (distance < bestDistance)
                {
                    bestDistance = distance;
                    bestPath = permutation;
                }

            } while (
                std::next_permutation(
                    permutation.begin(),
                    permutation.end()
                )
            );

            wxDialog dialog(
                this,
                wxID_ANY,
                wxT("Shortest Hamiltonian Path"),
                wxDefaultPosition,
                wxSize(950, 850),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

            wxPanel* panel = new wxPanel(&dialog);
            panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

            panel->Bind(
                wxEVT_PAINT,
                [panel, weights, bestPath](wxPaintEvent&)
                {
                    wxAutoBufferedPaintDC dc(panel);
                    dc.SetBackground(*wxWHITE_BRUSH);
                    dc.Clear();

                    const wxSize size = panel->GetClientSize();

                    const double pi =
                        3.14159265358979323846;

                    const double cx = size.x / 2.0;
                    const double cy = size.y / 2.0;

                    const double radius = std::max(
                        80.0,
                        std::min(size.x, size.y) / 2.0 - 80.0
                    );

                    wxPoint points[7];

                    for (int i = 1; i <= 6; ++i)
                    {
                        const double angle =
                            -pi / 2.0 +
                            (i - 1) * pi / 3.0;

                        points[i] = wxPoint(
                            static_cast<int>(
                                cx + radius * std::cos(angle)
                            ),
                            static_cast<int>(
                                cy + radius * std::sin(angle)
                            )
                        );
                    }

                    auto onPath = [&](int a, int b)
                    {
                        for (int k = 0; k < 5; ++k)
                        {
                            if (
                                (bestPath[k] == a &&
                                 bestPath[k + 1] == b) ||
                                (bestPath[k] == b &&
                                 bestPath[k + 1] == a)
                            )
                                return true;
                        }

                        return false;
                    };

                    // Draw all 15 edges.
                    for (int i = 1; i <= 6; ++i)
                    {
                        for (int j = i + 1; j <= 6; ++j)
                        {
                            const bool selected = onPath(i, j);

                            dc.SetPen(
                                wxPen(
                                    selected
                                        ? wxColour(220, 0, 0)
                                        : wxColour(170, 170, 170),
                                    selected ? 5 : 1
                                )
                            );

                            dc.DrawLine(points[i], points[j]);
                        }
                    }

                    // Edge weights intentionally omitted from graph display.

// Draw vertices last.
                    dc.SetPen(wxPen(*wxBLACK, 2));
                    dc.SetBrush(
                        wxBrush(wxColour(255, 220, 0))
                    );

                    dc.SetFont(
                        wxFont(
                            13,
                            wxFONTFAMILY_SWISS,
                            wxFONTSTYLE_NORMAL,
                            wxFONTWEIGHT_BOLD
                        )
                    );

                    for (int i = 1; i <= 6; ++i)
                    {
                        dc.DrawCircle(points[i], 22);

                        const wxString label =
                            wxString::Format(wxT("%d"), i);

                        const wxSize extent =
                            dc.GetTextExtent(label);

                        dc.SetTextForeground(*wxBLACK);

                        dc.DrawText(
                            label,
                            points[i].x - extent.x / 2,
                            points[i].y - extent.y / 2
                        );
                    }
                }
            );

            wxString pathText = wxT("Path: ");

            for (size_t i = 0; i < bestPath.size(); ++i)
            {
                if (i != 0)
                    pathText += wxT(" -> ");

                pathText += wxString::Format(
                    wxT("%d"),
                    bestPath[i]
                );
            }

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    pathText
                ),
                0,
                wxALIGN_CENTER | wxALL,
                12
            );




            sizer->Add(
                panel,
                1,
                wxEXPAND | wxALL,
                10
            );            sizer->Add(
                new wxButton(
                    &dialog,
                    wxID_CANCEL,
                    wxT("Close")
                ),
                0,
                wxALIGN_RIGHT | wxALL,
                10
            );

            dialog.SetSizer(sizer);
            dialog.CentreOnParent();
            dialog.ShowModal();

            return;
        }
        // END SHORTEST HAMILTONIAN PATH


        // BEGIN LONGEST SIMPLE PATH
        if (choice == 8)
        {
            wxTextEntryDialog entry(
                this,
                wxT("Enter the number of vertices on the first line,\\n")
                wxT("followed by one undirected edge per line.\\n")
                wxT("Vertices are numbered 1 through n.\\n")
                wxT("Maximum: 12 vertices."),
                wxT("Longest Simple Path - Graph Input"),
                wxEmptyString,
                wxOK | wxCANCEL | wxTE_MULTILINE
            );

            entry.SetSize(wxSize(650, 550));

            if (entry.ShowModal() != wxID_OK)
                return;

            std::istringstream input{
                std::string(entry.GetValue().mb_str())
            };

            int n = 0;

            if (!(input >> n) || n < 1 || n > 12)
            {
                wxMessageBox(
                    wxT("The first line must contain a vertex count from 1 to 12."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            std::vector<std::vector<int>> adjacency(n);
            std::vector<std::pair<int,int>> edges;

            int a, b;

            while (input >> a)
            {
                if (!(input >> b))
                {
                    wxMessageBox(
                        wxT("Each edge must contain two vertex numbers."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                if (a < 1 || a > n || b < 1 || b > n || a == b)
                {
                    wxMessageBox(
                        wxT("Invalid edge: endpoints must be distinct vertices from 1 to n."),
                        wxT("Invalid Graph"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                --a;
                --b;

                bool duplicate = false;

                for (const auto& e : edges)
                {
                    if ((e.first == a && e.second == b) ||
                        (e.first == b && e.second == a))
                    {
                        duplicate = true;
                        break;
                    }
                }

                if (!duplicate)
                {
                    adjacency[a].push_back(b);
                    adjacency[b].push_back(a);
                    edges.push_back({a,b});
                }
            }

            if (!input.eof())
            {
                wxMessageBox(
                    wxT("Graph input contains invalid characters."),
                    wxT("Invalid Graph"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            // Exact longest-simple-path search.
            // A simple path never repeats a vertex.
            std::vector<int> bestPath;
            std::vector<int> currentPath;
            std::vector<bool> visited(n, false);

            std::function<void(int)> dfs;

            dfs = [&](int u)
            {
                visited[u] = true;
                currentPath.push_back(u);

                if (currentPath.size() > bestPath.size())
                    bestPath = currentPath;

                for (int v : adjacency[u])
                {
                    if (!visited[v])
                        dfs(v);
                }

                currentPath.pop_back();
                visited[u] = false;
            };

            for (int startVertex = 0; startVertex < n; ++startVertex)
                dfs(startVertex);

            wxDialog dialog(
                this,
                wxID_ANY,
                wxT("Longest Simple Path - Unweighted Graph"),
                wxDefaultPosition,
                wxSize(1200, 850),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

            wxString pathText = wxT("Longest simple path: ");

            for (size_t i = 0; i < bestPath.size(); ++i)
            {
                if (i > 0)
                    pathText += wxT(" -> ");

                pathText += wxString::Format(
                    wxT("%d"), bestPath[i] + 1
                );
            }

            sizer->Add(
                new wxStaticText(
                    &dialog, wxID_ANY, pathText
                ),
                0,
                wxALIGN_CENTER | wxALL,
                12
            );

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    wxString::Format(
                        wxT("Path length: %d edges (%d vertices)"),
                        static_cast<int>(bestPath.size()) - 1,
                        static_cast<int>(bestPath.size())
                    )
                ),
                0,
                wxALIGN_CENTER | wxALL,
                8
            );

            wxPanel* panel = new wxPanel(&dialog);
            panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

            panel->Bind(
                wxEVT_PAINT,
                [panel, n, edges, bestPath](wxPaintEvent&)
                {
                    wxAutoBufferedPaintDC dc(panel);
                    dc.SetBackground(*wxWHITE_BRUSH);
                    dc.Clear();

                    const wxSize size = panel->GetClientSize();
                    const double pi = 3.14159265358979323846;

                    const double cx = size.x / 2.0;
                    const double cy = size.y / 2.0;

                    const double radius = std::max(
                        40.0,
                        std::min(size.x, size.y) / 2.0 - 65.0
                    );

                    std::vector<wxPoint> points(n);

                    for (int i = 0; i < n; ++i)
                    {
                        const double angle =
                            -pi / 2.0 + 2.0 * pi * i / n;

                        points[i] = wxPoint(
                            static_cast<int>(
                                cx + radius * std::cos(angle)
                            ),
                            static_cast<int>(
                                cy + radius * std::sin(angle)
                            )
                        );
                    }

                    auto onPath = [&bestPath](int u, int v)
                    {
                        for (size_t k = 1; k < bestPath.size(); ++k)
                        {
                            if ((bestPath[k-1] == u && bestPath[k] == v) ||
                                (bestPath[k-1] == v && bestPath[k] == u))
                                return true;
                        }

                        return false;
                    };

                    // Draw nonselected edges first.
                    dc.SetPen(wxPen(wxColour(165,165,165), 2));

                    for (const auto& e : edges)
                    {
                        if (!onPath(e.first, e.second))
                            dc.DrawLine(
                                points[e.first], points[e.second]
                            );
                    }

                    // Draw the longest path in red.
                    dc.SetPen(wxPen(wxColour(220,0,0), 5));

                    for (size_t k = 1; k < bestPath.size(); ++k)
                    {
                        dc.DrawLine(
                            points[bestPath[k-1]],
                            points[bestPath[k]]
                        );
                    }

                    // Draw numbered vertices.
                    dc.SetPen(wxPen(*wxBLACK, 2));
                    dc.SetBrush(wxBrush(wxColour(255,220,0)));

                    dc.SetFont(wxFont(
                        13,
                        wxFONTFAMILY_SWISS,
                        wxFONTSTYLE_NORMAL,
                        wxFONTWEIGHT_BOLD
                    ));

                    for (int i = 0; i < n; ++i)
                    {
                        dc.DrawCircle(points[i], 22);

                        const wxString label =
                            wxString::Format(wxT("%d"), i+1);

                        const wxSize extent =
                            dc.GetTextExtent(label);

                        dc.SetTextForeground(*wxBLACK);

                        dc.DrawText(
                            label,
                            points[i].x - extent.x/2,
                            points[i].y - extent.y/2
                        );
                    }
                }
            );

            sizer->Add(
                panel,
                1,
                wxEXPAND | wxALL,
                10
            );

            wxButton* closeButton =
                new wxButton(&dialog, wxID_OK, wxT("Close"));

            sizer->Add(
                closeButton,
                0,
                wxALIGN_CENTER | wxALL,
                10
            );

            dialog.SetSizer(sizer);
            dialog.Layout();
            dialog.CentreOnParent();
            dialog.ShowModal();

            return;
        }
        // END LONGEST SIMPLE PATH



// Traveling Salesman Problem
        if (choice == 5)
        {
            wxDialog dialog(
                this,
                wxID_ANY,
                wxT("Traveling Salesman Problem"),
                wxDefaultPosition,
                wxSize(950, 850),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

            wxPanel* panel = new wxPanel(&dialog);
            panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

            panel->Bind(
                wxEVT_PAINT,
                [panel](wxPaintEvent&)
                {
                    wxAutoBufferedPaintDC dc(panel);
                    dc.SetBackground(*wxWHITE_BRUSH);
                    dc.Clear();

                    wxSize size = panel->GetClientSize();

                    const double pi =
                        3.14159265358979323846;

                    const double cx = size.x / 2.0;
                    const double cy = size.y / 2.0;

                    const double radius = std::max(
                        80.0,
                        std::min(size.x, size.y) / 2.0 - 80.0
                    );

                    // Vertex positions follow circle6.png.
                    const int order[6] = {1, 2, 3, 4, 5, 6};

                    wxPoint p[7];

                    for (int k = 0; k < 6; ++k)
                    {
                        double angle =
                            -pi / 2.0 + k * pi / 3.0;

                        p[order[k]] = wxPoint(
                            static_cast<int>(
                                cx + radius * std::cos(angle)
                            ),
                            static_cast<int>(
                                cy + radius * std::sin(angle)
                            )
                        );
                    }

                    // Actual complete-graph edge weights.
                    // Vertex ordering follows the original
                    // six-vertex circle example.

                    const double weights[7][7] = {
                        {0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 1, 1.73205, 1.73205, 2, 1},
                        {0, 1, 0, 1, 2, 1.73205, 1.73205},
                        {0, 1.73205, 1, 0, 1.73205, 1, 2},
                        {0, 1.73205, 2, 1.73205, 0, 1, 1},
                        {0, 2, 1.73205, 1, 1, 0, 1.73205},
                        {0, 1, 1.73205, 2, 1, 1.73205, 0}
                    };

                    const int tour[7] = {
                        4, 5, 3, 2, 1, 6, 4
                    };

                    auto onTour = [&](int a, int b)
                    {
                        for (int k = 0; k < 6; ++k)
                        {
                            if (
                                (tour[k] == a &&
                                 tour[k + 1] == b) ||
                                (tour[k] == b &&
                                 tour[k + 1] == a)
                            )
                                return true;
                        }

                        return false;
                    };

                    // Draw all 15 edges.
                    for (int i = 1; i <= 6; ++i)
                    {
                        for (int j = i + 1; j <= 6; ++j)
                        {
                            bool selected = onTour(i, j);

                            dc.SetPen(
                                wxPen(
                                    selected
                                        ? wxColour(220, 0, 0)
                                        : wxColour(170, 170, 170),
                                    selected ? 5 : 1
                                )
                            );

                            dc.DrawLine(p[i], p[j]);
                        }
                    }

                    // Draw edge weights.
                    dc.SetFont(
                        wxFont(
                            11,
                            wxFONTFAMILY_SWISS,
                            wxFONTSTYLE_NORMAL,
                            wxFONTWEIGHT_BOLD
                        )
                    );

                    for (int i = 1; i <= 6; ++i)
                    {
                        for (int j = i + 1; j <= 6; ++j)
                        {
                            double w = weights[i][j];

                            wxString label;

                            if (w < 1.1)
                                label = wxT("1");
                            else if (w > 1.9)
                                label = wxT("2");
                            else
                                label = wxT("1.73205");

                            double dx = p[j].x - p[i].x;
                            double dy = p[j].y - p[i].y;

                            double length = std::hypot(dx, dy);

                            if (length < 1.0)
                                continue;

                            double nx = -dy / length;
                            double ny = dx / length;

                            int distance = std::min(
                                j - i,
                                6 - (j - i)
                            );

                            double fraction = 0.5;
                            double offset = 15.0;

                            if (distance == 2)
                            {
                                fraction =
                                    ((i + j) % 2) ? 0.4 : 0.6;
                                offset = 18.0;
                            }
                            else if (distance == 3)
                            {
                                fraction = 0.35;
                                offset = 15.0;
                            }

                            double x =
                                p[i].x + fraction * dx
                                + offset * nx;

                            double y =
                                p[i].y + fraction * dy
                                + offset * ny;

                            wxSize extent =
                                dc.GetTextExtent(label);

                            int tx = static_cast<int>(
                                x - extent.x / 2.0
                            );

                            int ty = static_cast<int>(
                                y - extent.y / 2.0
                            );

                            dc.SetPen(*wxTRANSPARENT_PEN);
                            dc.SetBrush(*wxWHITE_BRUSH);

                            dc.DrawRectangle(
                                tx - 3,
                                ty - 2,
                                extent.x + 6,
                                extent.y + 4
                            );

                            dc.SetTextForeground(
                                onTour(i, j)
                                    ? wxColour(190, 0, 0)
                                    : *wxBLACK
                            );

                            dc.DrawText(label, tx, ty);
                        }
                    }

                    // Draw vertices last.
                    dc.SetPen(wxPen(*wxBLACK, 2));
                    dc.SetBrush(
                        wxBrush(wxColour(255, 220, 0))
                    );

                    dc.SetFont(
                        wxFont(
                            13,
                            wxFONTFAMILY_SWISS,
                            wxFONTSTYLE_NORMAL,
                            wxFONTWEIGHT_BOLD
                        )
                    );

                    for (int i = 1; i <= 6; ++i)
                    {
                        dc.DrawCircle(p[i], 22);

                        wxString label =
                            wxString::Format(wxT("%d"), i);

                        wxSize extent =
                            dc.GetTextExtent(label);

                        dc.SetTextForeground(*wxBLACK);

                        dc.DrawText(
                            label,
                            p[i].x - extent.x / 2,
                            p[i].y - extent.y / 2
                        );
                    }
                }
            );

            sizer->Add(
                panel,
                1,
                wxEXPAND | wxALL,
                10
            );

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    wxT("Tour: 4 -> 5 -> 3 -> 2 -> 1 -> 6 -> 4")
                ),
                0,
                wxALIGN_CENTER | wxALL,
                12
            );

            sizer->Add(
                new wxStaticText(
                    &dialog,
                    wxID_ANY,
                    wxT("Tour distance: 6")
                ),
                0,
                wxALIGN_CENTER | wxALL,
                12
            );

            sizer->Add(
                new wxButton(
                    &dialog,
                    wxID_CANCEL,
                    wxT("Close")
                ),
                0,
                wxALIGN_RIGHT | wxALL,
                10
            );

            dialog.SetSizer(sizer);
            dialog.CentreOnParent();
            dialog.ShowModal();

            return;
        }

// END SUBGRAPH AND TSP VISUALIZATION UPGRADE



        wxString relativeImage;
        wxString relativeResult;
        wxString exampleTitle;

        switch (choice)
        {
            case 0:
                exampleTitle = wxT("Maximum Clique");
                relativeImage =
                    wxT("clique/maximum-clique-help-example.png");
                relativeResult =
                    wxT("clique/maximum-clique-help-example.txt");
                break;

            case 1:
                exampleTitle = wxT("Graph Coloring");
                relativeImage =
                    wxT("graph-coloring/graph-coloring-help-example.png");
                relativeResult =
                    wxT("graph-coloring/graph-coloring-help-example.txt");
                break;

            case 2:
                exampleTitle = wxT("Hamiltonian Cycle");
                relativeImage =
                    wxT("Hamiltonian-cycle/hamiltonian-cycle.png");
                relativeResult =
                    wxT("Hamiltonian-cycle/hamiltonian-cycle.txt");
                break;

            case 3:
                exampleTitle = wxT("Independent Set / Vertex Cover");
                relativeImage =
                    wxT("independent-set-vertex-cover/")
                    wxT("independent-set-vertex-cover-help-example.png");
                relativeResult =
                    wxT("independent-set-vertex-cover/")
                    wxT("independent-set-vertex-cover-help-example.txt");
                break;

            case 4:
            {
                exampleTitle = wxT("Subgraph Isomorphism");

                wxArrayString pairs;
                pairs.Add(wxT("Graphs M1 and M2"));
                pairs.Add(wxT("Graphs M3 and M4"));

                wxSingleChoiceDialog pairSelection(
                    this,
                    wxT("Choose the graph pair to display."),
                    wxT("Subgraph Isomorphism"),
                    pairs
                );

                if (pairSelection.ShowModal() != wxID_OK)
                    return;

                relativeImage =
                    pairSelection.GetSelection() == 0
                    ? wxT("subgraph-isomorphism/")
                      wxT("subgraph-isomorphism-graphs-M1-M2.png")
                    : wxT("subgraph-isomorphism/")
                      wxT("subgraph-isomorphism-graphs-M3-M4.png");

                break;
            }

            case 5:
                exampleTitle = wxT("Traveling Salesman Problem");
                relativeImage = wxT("TSP/circle6.png");
                relativeResult = wxT("TSP/circle6.txt");
                break;

            default:
                return;
        }

        wxString imageFile =
            selectedDirectory + wxT("/") + relativeImage;

        if (!wxFileExists(imageFile))
        {
            wxMessageBox(
                wxT("Illustration not found:\n") + imageFile,
                exampleTitle,
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        if (!wxLaunchDefaultApplication(imageFile))
        {
            wxMessageBox(
                wxT("Unable to open illustration:\n") + imageFile,
                exampleTitle,
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        // Subgraph isomorphism has illustrations but no TXT result.
        if (choice == 4)
            return;

        wxMessageDialog question(
            this,
            wxT("The example illustration has opened.\n\n")
            wxT("Would you like to see the algorithm results?"),
            exampleTitle,
            wxYES_NO | wxICON_QUESTION
        );

        question.SetYesNoLabels(
            wxT("Show Algorithm Results"),
            wxT("Close")
        );

        if (question.ShowModal() != wxID_YES)
            return;

        wxString resultFile =
            selectedDirectory + wxT("/") + relativeResult;

        wxTextFile results;

        if (!results.Open(resultFile))
        {
            wxMessageBox(
                wxT("Unable to read results:\n") + resultFile,
                exampleTitle,
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        wxString resultText;

        for (size_t i = 0; i < results.GetLineCount(); ++i)
        {
            wxString line = results.GetLine(i);
            line.Trim(true).Trim(false);

            if (line.IsEmpty())
                continue;

            // Convert sequences separated by spaces into arrows
            // for clique, Hamiltonian cycle and independent sets.
            if (choice == 0 && line != wxT("Maximum clique:"))
            {
                wxStringTokenizer tokens(line, wxT(" \t\r\n"));
                wxString vertices;

                while (tokens.HasMoreTokens())
                {
                    if (!vertices.IsEmpty())
                        vertices += wxT(", ");
                    vertices += tokens.GetNextToken();
                }

                resultText += vertices + wxT("\n");
            }
            else if (choice == 2 &&
                     line.StartsWith(wxT("Hamiltonian cycle:")))
            {
                wxString vertices = line.AfterFirst(':');
                wxStringTokenizer tokens(vertices, wxT(" \t\r\n"));
                wxString cycle;

                while (tokens.HasMoreTokens())
                {
                    if (!cycle.IsEmpty())
                        cycle += wxT(" -> ");
                    cycle += tokens.GetNextToken();
                }

                resultText +=
                    wxT("Hamiltonian Cycle:\n") +
                    cycle + wxT("\n");
            }
            else
            {
                resultText += line + wxT("\n");
            }
        }

        results.Close();

        if (resultText.IsEmpty())
            resultText = wxT("No results found.");

        wxMessageDialog resultDialog(
            this,
            resultText,
            exampleTitle + wxT(" Results"),
            wxOK | wxICON_INFORMATION
        );

        resultDialog.ShowModal();

        // The TSP directory includes additional exact solutions.
        if (choice == 5)
        {
            wxMessageDialog more(
                this,
                wxT("Would you like to see the exact solution ")
                wxT("costs for the additional TSP examples?"),
                wxT("Additional TSP Solutions"),
                wxYES_NO | wxICON_QUESTION
            );

            more.SetYesNoLabels(
                wxT("Show Additional Solutions"),
                wxT("Close")
            );

            if (more.ShowModal() == wxID_YES)
            {
                wxString additionalFile =
                    selectedDirectory +
                    wxT("/TSP/additional-examples/")
                    wxT("Exact TSP solutions.txt");

                wxTextFile additional;

                if (!additional.Open(additionalFile))
                {
                    wxMessageBox(
                        wxT("Unable to read:\n") + additionalFile,
                        wxT("Additional TSP Solutions"),
                        wxOK | wxICON_ERROR,
                        this
                    );
                    return;
                }

                wxString additionalText =
                    wxT("Exact TSP solution costs\n\n");

                for (size_t i = 0;
                     i < additional.GetLineCount(); ++i)
                {
                    additionalText +=
                        additional.GetLine(i) + wxT("\n");
                }

                additional.Close();

                wxMessageBox(
                    additionalText,
                    wxT("Additional TSP Solutions"),
                    wxOK | wxICON_INFORMATION,
                    this
                );
            }
        }

        return;
    }
    // END NP-COMPLETE PROBLEMS EXAMPLES



    // BEGIN MINIMUM PATH / MINIMUM SPANNING TREE EXAMPLES
    if (algorithmName ==
        wxT("minimum-path-minimum-spanning-tree"))
    {
        wxArrayString choices;

        choices.Add(wxT("Minimum Path"));
        choices.Add(wxT("Minimum Spanning Tree"));

        wxSingleChoiceDialog selection(
            this,
            wxT("Select the graph algorithm example."),
            wxT("Minimum Path / Minimum Spanning Tree"),
            choices
        );

        if (selection.ShowModal() != wxID_OK)
            return;

        const bool minimumPath =
            (selection.GetSelection() == 0);

        wxString exampleName = minimumPath
            ? wxT("help-minimum-path")
            : wxT("help-minimum-spanning-tree");

        wxString imageFile =
            selectedDirectory + wxT("/") +
            exampleName + wxT(".png");

        wxString resultFile =
            selectedDirectory + wxT("/") +
            exampleName + wxT(".txt");

        if (!wxFileExists(imageFile))
        {
            wxMessageBox(
                wxT("Example illustration not found:\n") +
                imageFile,
                wxT("Graph Algorithm Example"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        if (!wxLaunchDefaultApplication(imageFile))
        {
            wxMessageBox(
                wxT("Unable to open illustration:\n") +
                imageFile,
                wxT("Graph Algorithm Example"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        wxMessageDialog question(
            this,
            minimumPath
                ? wxT("The Minimum Path illustration has opened."
                      "\n\nShow the minimum path results?")
                : wxT("The Minimum Spanning Tree illustration "
                      "has opened.\n\nShow the spanning tree results?"),
            wxT("Graph Algorithm Results"),
            wxYES_NO | wxICON_QUESTION
        );

        question.SetYesNoLabels(
            wxT("Show Algorithm Results"),
            wxT("Close")
        );

        if (question.ShowModal() != wxID_YES)
            return;

        wxTextFile results;

        if (!results.Open(resultFile))
        {
            wxMessageBox(
                wxT("Unable to read result file:\n") +
                resultFile,
                wxT("Graph Algorithm Results"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        wxString resultText;

        for (size_t i = 0; i < results.GetLineCount(); ++i)
        {
            wxString line = results.GetLine(i);
            line.Trim(true).Trim(false);

            if (line.IsEmpty())
                continue;

            if (minimumPath)
            {
                if (line.StartsWith(wxT("minPathLength")))
                {
                    wxString value =
                        line.AfterFirst('=');

                    value.Trim(true).Trim(false);

                    resultText +=
                        wxT("Minimum Path Length: ") +
                        value + wxT("\n\n");
                }
                else if (line.StartsWith(
                    wxT("Minimum path:")))
                {
                    wxString path =
                        line.AfterFirst(':');

                    path.Trim(true).Trim(false);

                    wxStringTokenizer tokenizer(
                        path,
                        wxT(" \t\r\n")
                    );

                    wxString formattedPath;

                    while (tokenizer.HasMoreTokens())
                    {
                        if (!formattedPath.IsEmpty())
                            formattedPath += wxT(" -> ");

                        formattedPath +=
                            tokenizer.GetNextToken();
                    }

                    resultText +=
                        wxT("Minimum Path:\n") +
                        formattedPath + wxT("\n");
                }
                else
                {
                    resultText += line + wxT("\n");
                }
            }
            else
            {
                if (line.StartsWith(wxT("Total weight:")))
                {
                    wxString weight =
                        line.AfterFirst(':');

                    weight.Trim(true).Trim(false);

                    resultText +=
                        wxT("Minimum Spanning Tree\n\n")
                        wxT("Total Weight: ") +
                        weight + wxT("\n");
                }
                else
                {
                    resultText += line + wxT("\n");
                }
            }
        }

        results.Close();

        if (resultText.IsEmpty())
            resultText =
                wxT("The result file contains no results.");

        wxMessageDialog resultDialog(
            this,
            resultText,
            minimumPath
                ? wxT("Minimum Path Results")
                : wxT("Minimum Spanning Tree Results"),
            wxOK | wxICON_INFORMATION
        );

        resultDialog.ShowModal();

        return;
    }
    // END MINIMUM PATH / MINIMUM SPANNING TREE EXAMPLES



    // Maximum Independent Set directory:
    // Examples demonstrate maximum-cardinality and weighted matching.
    if (algorithmName == wxT("maximum-independent-set"))
    {
        wxArrayString choices;
        choices.Add(wxT("Maximum Cardinality Matching"));
        choices.Add(wxT("Weighted Matching"));

        wxSingleChoiceDialog selection(
            this,
            wxT("Choose a matching example."),
            wxT("Matching Examples"),
            choices
        );

        if (selection.ShowModal() != wxID_OK)
            return;

        wxString folder;
        wxString filename;

        if (selection.GetSelection() == 0)
        {
            folder = wxT("cardinality-matching");
            filename =
                wxT("maximum-independent-set-cardinality-matching");
        }
        else
        {
            folder = wxT("weighted-matching");
            filename =
                wxT("maximum-independent-set-weighted-matching");
        }

        wxString base = selectedDirectory +
            wxT("/") + folder + wxT("/") + filename;

        wxString imageFile = base + wxT(".png");
        wxString resultFile = base + wxT(".txt");

        if (!wxFileExists(imageFile))
        {
            wxMessageBox(
                wxT("Illustration not found:\n") + imageFile,
                wxT("Matching Example"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        if (!wxLaunchDefaultApplication(imageFile))
        {
            wxMessageBox(
                wxT("Unable to open:\n") + imageFile,
                wxT("Matching Example"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        wxMessageDialog question(
            this,
            wxT("The illustration shows the matching example.\n\n")
            wxT("Would you like to see the selected edges?"),
            wxT("Matching Results"),
            wxYES_NO | wxICON_QUESTION
        );

        question.SetYesNoLabels(
            wxT("Show Selected Edges"),
            wxT("Close")
        );

        if (question.ShowModal() == wxID_YES)
        {
            wxTextFile results;

            if (!results.Open(resultFile))
            {
                wxMessageBox(
                    wxT("Unable to read results:\n") + resultFile,
                    wxT("Matching Results"),
                    wxOK | wxICON_ERROR,
                    this
                );
                return;
            }

            wxDialog tableDialog(
                this,
                wxID_ANY,
                wxT("Selected Matching Edges"),
                wxDefaultPosition,
                wxSize(420, 350),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);

            wxStaticText* explanation = new wxStaticText(
                &tableDialog,
                wxID_ANY,
                wxT("Edges selected by the matching algorithm:")
            );

            layout->Add(explanation, 0, wxALL, 12);

            wxListCtrl* table = new wxListCtrl(
                &tableDialog,
                wxID_ANY,
                wxDefaultPosition,
                wxDefaultSize,
                wxLC_REPORT | wxLC_SINGLE_SEL
            );

            table->InsertColumn(
                0, wxT("Edge"), wxLIST_FORMAT_LEFT, 100
            );
            table->InsertColumn(
                1, wxT("Endpoints"), wxLIST_FORMAT_LEFT, 200
            );

            int edgeNumber = 0;

            for (size_t i = 0; i < results.GetLineCount(); ++i)
            {
                wxString line = results.GetLine(i);
                line.Trim(true).Trim(false);

                // Skip explanatory text and blank lines.
                if (line.IsEmpty())
                    continue;

                if (line.Find(wxT(" - ")) == wxNOT_FOUND)
                    continue;

                ++edgeNumber;

                long row = table->InsertItem(
                    table->GetItemCount(),
                    wxString::Format(wxT("%d"), edgeNumber)
                );

                table->SetItem(row, 1, line);
            }

            results.Close();

            layout->Add(
                table, 1, wxEXPAND | wxLEFT | wxRIGHT, 12
            );

            wxButton* closeButton = new wxButton(
                &tableDialog, wxID_CANCEL, wxT("Close")
            );

            layout->Add(
                closeButton, 0, wxALIGN_RIGHT | wxALL, 12
            );

            tableDialog.SetSizer(layout);
            tableDialog.CentreOnParent();
            tableDialog.ShowModal();
        }

        return;
    }



    // Matching example: Stable Marriage Problem.
    // Display illustrations without modifying the graph preview.
    if (algorithmName == wxT("matching"))
    {
        wxString preferencesFile =
            selectedDirectory + wxT("/marriage-preferences.png");

        wxString marriagesFile =
            selectedDirectory + wxT("/resulting-marriages.jpg");

        if (!wxFileExists(preferencesFile) ||
            !wxFileExists(marriagesFile))
        {
            wxMessageBox(
                wxT("One or more Matching illustrations are missing."),
                wxT("Matching Example"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        if (!wxLaunchDefaultApplication(preferencesFile))
        {
            wxMessageBox(
                wxT("Unable to open:\n") + preferencesFile,
                wxT("Matching Example"),
                wxOK | wxICON_ERROR,
                this
            );
            return;
        }

        wxMessageDialog question(
            this,
            wxT("The illustration shows the marriage preferences.\n\n")
            wxT("Would you like to see the resulting marriages?"),
            wxT("Stable Marriage Problem"),
            wxYES_NO | wxICON_QUESTION
        );

        question.SetYesNoLabels(
            wxT("Show Resulting Marriages"),
            wxT("Close")
        );

        if (question.ShowModal() == wxID_YES)
        {
            if (!wxLaunchDefaultApplication(marriagesFile))
            {
                wxMessageBox(
                    wxT("Unable to open:\n") + marriagesFile,
                    wxT("Matching Example"),
                    wxOK | wxICON_ERROR,
                    this
                );
            }
        }

        return;
    }



    if (algorithmName != wxT("bfs") &&
        algorithmName != wxT("dfs") &&
        algorithmName != wxT("connected-components") &&
        algorithmName != wxT("Euler-tour") &&
        algorithmName != wxT("graphical-sequence") &&
        algorithmName != wxT("isomorphism"))
    {
        wxMessageBox(
            wxT("Select bfs, dfs, connected-components, Euler-tour, or graphical-sequence.")
        );
        return;
    }

    wxString graphFile;
    wxString pngFile;
    wxString resultFile;
    bool isomorphismExample = false;

    if (algorithmName == wxT("connected-components"))
    {
        graphFile = selectedDirectory +
            wxT("/graph-to-look-for-components.gml");

        pngFile = selectedDirectory +
            wxT("/connected-components.png");
    }
    else if (algorithmName == wxT("isomorphism"))
    {
        wxArrayString choices;
        choices.Add(wxT("Graph Isomorphism"));
        choices.Add(wxT("Tree Isomorphism"));

        wxSingleChoiceDialog choiceDialog(
            this,
            wxT("Choose an isomorphism example."),
            wxT("Isomorphism Examples"),
            choices
        );

        if (choiceDialog.ShowModal() != wxID_OK)
            return;

        isomorphismExample = true;

        if (choiceDialog.GetSelection() == 0)
        {
            graphFile = selectedDirectory +
                wxT("/help-isomorphism1.grph");
            pngFile = selectedDirectory +
                wxT("/help-isomorphism-graphs.png");
            resultFile = selectedDirectory +
                wxT("/help-isomorphism-result-of-test.txt");
        }
        else
        {
            graphFile = selectedDirectory +
                wxT("/tree-isomorphism-1.grph");
            pngFile = selectedDirectory +
                wxT("/help-tree-isomorphism-trees.png");
            resultFile = selectedDirectory +
                wxT("/tree-isomorphism-result-of-test.txt");
        }
    }
    else if (algorithmName == wxT("graphical-sequence"))
    {
        graphFile = selectedDirectory +
            wxT("/graphical-sequence.gml");

        pngFile = selectedDirectory +
            wxT("/graphical-sequence.png");
    }
    else if (algorithmName == wxT("Euler-tour"))
    {
        wxArrayString choices;
        choices.Add(wxT("Euler Tour Example 1"));
        choices.Add(wxT("Euler Tour Example 2"));

        wxSingleChoiceDialog choiceDialog(
            this,
            wxT("Choose an Euler Tour example."),
            wxT("Euler Tour"),
            choices
        );

        if (choiceDialog.ShowModal() != wxID_OK)
            return;

        if (choiceDialog.GetSelection() == 0)
        {
            graphFile = selectedDirectory +
                wxT("/Euler-tour-help-example.grph");

            pngFile = selectedDirectory +
                wxT("/Euler-tour-of-graph.png");
        }
        else
        {
            graphFile = selectedDirectory +
                wxT("/graph-for-Euler-tour-2.grph");

            pngFile = selectedDirectory +
                wxT("/Euler-tour-of-graph-2.png");
        }
    }
    else
    {
        graphFile = examplesDirectory +
            wxT("/bfs-dfs/graph-for-BFS-DFS.gml");

        if (algorithmName == wxT("bfs"))
            pngFile = selectedDirectory + wxT("/BFS-of-graph.png");
        else
            pngFile = selectedDirectory + wxT("/DFS-of-graph.png");
    }

    if (!wxFileExists(graphFile))
    {
        wxMessageBox(wxT("Graph file not found:\n") + graphFile);
        return;
    }

    if (!wxFileExists(pngFile))
    {
        wxMessageBox(wxT("Illustration not found:\n") + pngFile);
        return;
    }

    if (isomorphismExample)
    {
        if (!wxLaunchDefaultApplication(pngFile))
        {
            wxMessageBox(
                wxT("Unable to open illustration:\n") + pngFile
            );
            return;
        }

        wxMessageDialog question(
            this,
            wxT("The illustration shows both graphs.\n\n")
            wxT("Would you like to see the vertex correspondence?"),
            wxT("Isomorphism Example"),
            wxYES_NO | wxICON_QUESTION
        );

        question.SetYesNoLabels(
            wxT("Show Vertex Correspondence"),
            wxT("Close")
        );

        if (question.ShowModal() == wxID_YES)
        {
            wxTextFile results;

            if (!results.Open(resultFile))
            {
                wxMessageBox(
                    wxT("Unable to read correspondence:\n") + resultFile
                );
                return;
            }

            wxDialog tableDialog(
                this,
                wxID_ANY,
                wxT("Vertex Correspondence"),
                wxDefaultPosition,
                wxSize(420, 400),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
            );

            wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);

            wxStaticText* explanation = new wxStaticText(
                &tableDialog,
                wxID_ANY,
                wxT("Corresponding vertices in the two graphs:")
            );

            layout->Add(explanation, 0, wxALL, 12);

            wxListCtrl* table = new wxListCtrl(
                &tableDialog,
                wxID_ANY,
                wxDefaultPosition,
                wxDefaultSize,
                wxLC_REPORT | wxLC_SINGLE_SEL
            );

            table->InsertColumn(0, wxT("Graph 1"), wxLIST_FORMAT_LEFT, 160);
            table->InsertColumn(1, wxT("Graph 2"), wxLIST_FORMAT_LEFT, 160);

            for (size_t i = 0; i < results.GetLineCount(); ++i)
            {
                wxString line = results.GetLine(i);
                int arrow = line.Find(wxT("-->"));

                if (arrow == wxNOT_FOUND)
                    continue;

                wxString left = line.Left(arrow);
                wxString right = line.Mid(arrow + 3);

                left.Trim(true).Trim(false);
                right.Trim(true).Trim(false);

                long row = table->InsertItem(
                    table->GetItemCount(), left
                );
                table->SetItem(row, 1, right);
            }

            results.Close();

            layout->Add(table, 1, wxEXPAND | wxLEFT | wxRIGHT, 12);

            wxButton* closeButton = new wxButton(
                &tableDialog, wxID_CANCEL, wxT("Close")
            );

            layout->Add(
                closeButton, 0, wxALIGN_RIGHT | wxALL, 12
            );

            tableDialog.SetSizer(layout);
            tableDialog.CentreOnParent();
            tableDialog.ShowModal();
        }

        return;
    }

    GraphType graphType = undir;
    GraphLayout graphLayout = forcedirected;
    bool allowParallelEdges = false;

    saveGraphPreview(false);
    graphsAllowParallelEdgesCheckBox->SetValue(false);

    if (myGraph != nullptr)
    {
        delete myGraph;
        myGraph = nullptr;
    }

    myGraph = new BuildGraph(
        wxT(""),
        graphType,
        graphLayout,
        true,
        allowParallelEdges
    );

    myGraph->setDrawingCanvas(previewWindow);

    GraphType loadedGraphType = graphType;
    bool parallelEdges = false;

    if (algorithmName == wxT("Euler-tour") ||
        algorithmName == wxT("isomorphism"))
    {
        myGraph->buildFromGRPH(graphFile, false, true);
    }
    else
    {
        myGraph->buildFromGML(
            graphFile,
            &loadedGraphType,
            &parallelEdges,
            false
        );
    }

    graphsGraphTypeRadioBox->SetSelection(
        (loadedGraphType == digraph ||
         loadedGraphType == digraphw) ? 1 : 0
    );

    graphsHideEdgesCheckBox->SetValue(false);
    graphsAllowParallelEdgesCheckBox->SetValue(parallelEdges);

    previewWindow->setGA(myGraph->getGraphAttributes());
    previewWindow->getSFDiagramManager()->UpdateAll();

    previewWindow->RefreshCanvas(
        true,
        previewWindow->GetRect()
    );

    if (!wxLaunchDefaultApplication(pngFile))
    {
        wxMessageBox(
            wxT("Unable to open illustration:\n") + pngFile
        );
    }

    if (algorithmName == wxT("graphical-sequence"))
    {
        std::vector<int> dataValues;

        InputIntegerData dataEntered(this, &dataValues, false);

        dataEntered.EnableRepeatedGraphicalSequenceTests();
        dataEntered.ShowModal();
    }

}
