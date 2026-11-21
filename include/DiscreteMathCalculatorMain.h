/////////////////////////////////////////////////////////////////////////////
// Name:        DiscreteMathCalculatorMain.h
// Purpose:
// Author:      William Ford (wford333@gmail.com)
// Created:
// Copyright:   (c) 2023 William Ford
// Licence:     GNU General Public Licence version 3
/////////////////////////////////////////////////////////////////////////////

/***************************************************************
 * Name:      DiscreteMathCalculatorMain.h
 * Purpose:   Defines Application Frame
 * Author:    William Ford (wford@pacific.edu)
 * Created:   2009-04-27
 * Copyright: William Ford (http://www1.stk.pacific.edu/~wford/FordWWW)
 * License:
 **************************************************************/

#ifndef DISCRETEMATHCALCULATORMAIN_H
#define DISCRETEMATHCALCULATORMAIN_H

#include "DiscreteMathCalculatorApp.h"

//Headers(DiscreteMathCalculatorFrame)
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include "common/DrawingCanvas.h"
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
#include <wx/event.h>
#include <wx/thread.h>
#include <wx/string.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/dirdlg.h>
#include <wx/dir.h>
#include <wx/scrolwin.h>
#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include "counting-enumeration/CombinationsPermutations.h"
#include "counting-enumeration/AsymptoticEstimate.h"
#include "triangles/Triangles.h"
#include "subsets/Subsets.h"
#include "partitions/Partition.h"
#include "trees/binary/BinaryTree.h"
#include "trees/search/stree.h"
#include "trees/huffman/HuffmanTree.h"
#include "trees/huffman/HuffmanDrawingCanvas.h"
#include "trees/search/AVLTree.h"
#include "trees/search/RBTree.h"
#include "trees/search/SearchTreeDrawingCanvas.h"
#include "common/DrawingCanvas.h"
#include "common/MenuEventHandler.h"
#include "graphs/GraphType.h"
#include "graphs/build/BuildGraph.h"
#include "graphs/build/GridGraphInput.h"
#include "wx/wxsf/wxShapeFramework.h"
#include "elliptic-curves/DiscreteLogarithmEc.h"
#include "elliptic-curves/read_save_curve.h"
#include "elliptic-curves/ecurvelib.h"
#include "elliptic-curves/schoof.h"

NTL_CLIENT

struct DigraphicalSequenceStorage
{
	pair<int, int> p;
	int origIndex;
};

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

#include <miracl/big.h>
#include <miracl/gf2m.h>
#include <miracl/flash.h>

#include "elliptic-curves/ecurvelib.h"

void setLayoutRadioButton(int selection);

class DiscreteMathCalculatorFrame: public wxFrame
{
public:

#if defined(__WIN64__) || defined(__linux__)
	DiscreteMathCalculatorFrame (wxWindow* parent,wxWindowID id = -1);
#else
	DiscreteMathCalculatorFrame (wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxT ("frame") );
#endif

	virtual ~DiscreteMathCalculatorFrame();

	void resetSlider();
	void StartThread ();
	void StopThread ();
	void ResumeThread ();
	void PauseThread ();
	void getHelp();

private:

	//(*Handlers(DiscreteMathCalculatorFrame)
	bool CleanUp();
	void OnAbout (wxCommandEvent& event);
	void OncountingAndEnumerationFactorialButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationPermutationsButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationCombinationsButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationFibonacciButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationCatalanButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationSchroderButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationBellButtonClick (wxCommandEvent& event);
	void OnAsymptoticEstimateButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationListPermutationsButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationListCombinationsButtonClick (wxCommandEvent& event);
	void pascalTriangle (int lastrow);
	void OnpascalTriangleButtonClick (wxCommandEvent& event);
	void OncatalanTriangleButtonClick (wxCommandEvent& event);
	void OnschroderTriangleButtonClick (wxCommandEvent& event);
	void OnbellTriangleButtonClick (wxCommandEvent& event);
	void OntrianglesHosoyaButtonClick (wxCommandEvent& event);
	void OnsubsetsNumberOfSubsetsButtonClick (wxCommandEvent& event);
	void OnsubsetsNumberOfKElementSubsetsButtonClick (wxCommandEvent& event);
	void OnsubsetsListAllSubsetsButtonClick (wxCommandEvent& event);
	void OnsubsetsListAllKElementSubsetsButtonClick (wxCommandEvent& event);
	void OnpartitionsComputeBellNumberButtonClick (wxCommandEvent& event);
	void OnpartitionsListPartitionsButtonClick (wxCommandEvent& event);
	void OnpartitionsListStirlingPartitionsButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationStirlingButtonClick (wxCommandEvent& event);
	void OnpartitionsBellButtonClick (wxCommandEvent& event);
	void OnpartitionsStirlingButtonClick (wxCommandEvent& event);
	void OnbinaryTreesEnterButtonClick (wxCommandEvent& event);
	void OnbinaryTreesHuffmanTreeEnterButtonClick (wxCommandEvent& event);
	void OnbinaryTreesExpressionTreeEnterButtonClick (wxCommandEvent& event);
	void OnenterRandomHeapButtonClick (wxCommandEvent& event);
	void OngeneralTreeEnterButtonClick (wxCommandEvent& event);
	void OngeneralTreeEnterFromFileButtonClick (wxCommandEvent& event);
	void OnbinarySearchTreesTreeBalanceAlgorithmsEnterButtonClick (wxCommandEvent& event);
	void OnbinarySearchTreesTreeBalanceAlgorithmsFromFileButtonClick (wxCommandEvent& event);
	void OnbinarySearchTreesTreeBalanceAlgorithmsRandomDataButtonClick (wxCommandEvent& event);
	void OnEnterGMLFile (wxCommandEvent& event);
	void OnEnterFromTextfileButtonClick (wxCommandEvent& event);
	void OngraphsEnterFromTSPButtonClick (wxCommandEvent& event);
	void OnDataInputButtonClick (wxCommandEvent& event);
	void OnRandomDataEnterButtonClick (wxCommandEvent& event);
	void OnbinaryTreesEnterBTRFromFileButtonClick (wxCommandEvent& event);
	void OngraphsRunAlgorithmsButtonClick (wxCommandEvent& event);
	void OngraphsPreviewSliderCmdScrollChanged (wxScrollEvent& event);
	void removeAllMenusFromMenuBar();
	void buildFileMenu();
	void buildHelpMenu();
	void buildBigIntegerToolsMenu();
	void addBigIntegerCalculatorToolsMenu();
	void buildRelationsPartialOrdersFileMenu();
	void buildCreateMenu();
	void buildRelationsPartialOrderMenu();
	void buidHelpMenu();
	void OndiscreteMathCalculatorNotebookPageChanged (wxNotebookEvent& event);
	void OngraphsEnterFromDialogButtonClick (wxCommandEvent& event);
	void OngraphsEnterFromGRPHButtonClick (wxCommandEvent& event);
	void OngraphsEnterFromGMLButtonClick (wxCommandEvent& event);
	void OngraphsAddVertexButtonClick (wxCommandEvent& event);
	void OngraphsEraseVertexButtonClick (wxCommandEvent& event);
	void OngraphsAddEdgeButtonClick (wxCommandEvent& event);
	void OngraphsEraseEdgeButtonClick (wxCommandEvent& event);
	void OngraphsRedrawButtonClick (wxCommandEvent& event);
	void OncreateRandomGraphMenuItemSelected (wxCommandEvent& event);
	void OnGridGraphMenuItemSelected  (wxCommandEvent& event);
	void OnGraphSequenceGraphMenuItemSelected (wxCommandEvent& event);
	void OnDigraphicalSequenceGraphMenuItemSelected (wxCommandEvent& event);
	void OncomplementaryGraphButtonClick (wxCommandEvent& event);
	void OngraphLayoutRadioBoxSelect (wxCommandEvent& event);
	void OndrawKnMenuItemSelected (wxCommandEvent& event);
	void OncreateTSPMenuItemSelected (wxCommandEvent& event);
	void OngraphsGraphTypeRadioBoxSelect (wxCommandEvent& event);
	void OngraphsWeightedCheckBoxClick (wxCommandEvent& event);
	// void OngraphsHideEdgesCheckBoxClick (wxCommandEvent& event);
	void OngraphsAllowParallelEdgesCheckBoxClick (wxCommandEvent& event);
	void OncountingAndEnumerationMemoryStoreButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationMemoryAddButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationMemoryRecallButtonClick (wxCommandEvent& event);
	void OncountingAndEnumerationMemoryClearButtonClick (wxCommandEvent& event);
	void OnclearNumbersButtonClick (wxCommandEvent& event);
	void OnisAPrimeButtonClick (wxCommandEvent& event);
	void OnaddButtonClick (wxCommandEvent& event);
	void Onadd_Mod_n_ButtonClick (wxCommandEvent& event);
	void OnphiNButtonClick (wxCommandEvent& event);
	void Onto_a_ButtonClick (wxCommandEvent& event);
	void OnmemoryStore1ButtonClick (wxCommandEvent& event);
	void OnmemoryStore2ButtonClick (wxCommandEvent& event);
	void OnsubButtonClick (wxCommandEvent& event);
	void Onsub_mod_n_ButtonClick (wxCommandEvent& event);
	void Onphi_a_b_ButtonClick (wxCommandEvent& event);
	void Onto_b_ButtonClick (wxCommandEvent& event);
	void Onmemory1PlusButtonClick (wxCommandEvent& event);
	void Onmemory2PlusButtonClick (wxCommandEvent& event);
	void OnmulButtonClick (wxCommandEvent& event);
	void Onmul_mod_n_ButtonClick (wxCommandEvent& event);
	void Onx_bit_prime_ButtonClick (wxCommandEvent& event);
	void Onto_n_ButtonClick (wxCommandEvent& event);
	void OnmemoryRecall1ButtonClick (wxCommandEvent& event);
	void OnmemoryRecall2ButtonClick (wxCommandEvent& event);
	void OndivButtonClick (wxCommandEvent& event);
	void OnmodButtonClick (wxCommandEvent& event);
	void OngcdButtonClick (wxCommandEvent& event);
	void OnmemoryClear1ButtonClick (wxCommandEvent& event);
	void OnmemoryClear2ButtonClick (wxCommandEvent& event);
	void OnpowerButtonClick (wxCommandEvent& event);
	void Onpower_mod_n_ButtonClick (wxCommandEvent& event);
	void OnleastCommonMultipleButtonClick (wxCommandEvent& event);
	void Oninverse_a_mod_n_ButtonClick (wxCommandEvent& event);
	void OncoprimeButtonClick (wxCommandEvent& event);
	void OnresultMagnitudeButtonClick (wxCommandEvent& event);
	void OnresultMagnitude2ButtonClick (wxCommandEvent& event);
	void OnspellResultButtonClick (wxCommandEvent& event);
	void OnhypercubeMenuItemSelected (wxCommandEvent& event);
	void OnstableMarriageMenuItemSelected (wxCommandEvent& event);
	void OndiscreteMathCalcuatorHelpMenuItemSelected (wxCommandEvent& event);
	void OncreateHamCycleMenuItemSelected (wxCommandEvent& event);
	void OngeneralizedPetersonGraphMenuItemSelected (wxCommandEvent& event);
	void OngraphsMakeGraphWithACliqueMenuItemSelected (wxCommandEvent& event);
	void OntransposeGraphButtonClick (wxCommandEvent& event);
	void OnClearGraphButtonClick (wxCommandEvent& event);
	void OnSubgraphIsomorphismButtonPress (wxCommandEvent& event);
	void OnGraphIsomorphismButtonPress (wxCommandEvent& event);
	void OnRelationEnterButtonPress (wxCommandEvent& event);
	void OnRelationReflexiveClosureButtonPress (wxCommandEvent& event);
	void OnRelationSymmetricClosureButtonPress (wxCommandEvent& event);
	void OnRelationTransitiveClosureButtonPress (wxCommandEvent& event);
	void OnRelationPropertiesButtonPress (wxCommandEvent& event);
	void OnPartialOrderInputButtonPress (wxCommandEvent& event);
	void OnRelationToGMLButtonPress(wxCommandEvent& event);
	void OnInputRelationRELButtonPressed(wxCommandEvent& event);
	void OnPartialOrderFromPOSETButtonPress(wxCommandEvent& event);
	void OnPartialOrderOutputGMLButtonPress(wxCommandEvent& event);
	void OnPartialOrderFromGMLButtonPress(wxCommandEvent& event);
	void OnRelationFromGMLButtonPress(wxCommandEvent& event);
	void OnShapeFrameworkDrawingCanvasSliderChange (wxScrollEvent& event);
	void OnECCToPButtonPressed (wxCommandEvent& event);
	void OnECCBasePointOrderButtonPressed (wxCommandEvent& event);
	void OnECCSaveCurveParametersButtonPressed(wxCommandEvent& event);
	void OnECCReadCurveParametersButtonPressed(wxCommandEvent& event);
	void OnECCVerifyButton (wxCommandEvent& event);
	void OnECCCheckCurveButton(wxCommandEvent& event);
	void OnECCToQButtonPress (wxCommandEvent& event);
	void OnECCRandomPointButtonPress (wxCommandEvent& event);
	void OnECCRandom160BitSeedButtonPress (wxCommandEvent& event);
	void OnECCreateSecureCurveDomain (wxCommandEvent& event);
	void OnECCPlotSimpleEllipticCurveButtonPressed (wxCommandEvent& event);
	void OnECCDiscreteLogarithmButtonPressed (wxCommandEvent& event);
	void OnEllipticCurveDigitalSignatureMenuItemSelected (wxCommandEvent& event);
	void OnECCkBitPrimeButtonPress (wxCommandEvent& event);
	void OnECCToSeedButtonPressed (wxCommandEvent& event);
	void OnEllipticCurveECDLPMenuItemSelected (wxCommandEvent& event);
	void OnDigitalSignatureMenuItemSelected (wxCommandEvent& event);
	void OnIntegerFactorMenuItemSelected (wxCommandEvent& event);
	void OnBigIntegerDiscreteLogarithmMenuItemSelected (wxCommandEvent& event);
	void OnBigIntegerRSAGuideMenuItemSelected (wxCommandEvent& event);
	void OnBigIntegerRSAEncryptionMenuItemSelected (wxCommandEvent& event);
	void OnBigIntegerDiffieHellmanMenuItemSelected (wxCommandEvent& event);
	void OnBigIntegerDigitalSignatureMenuItemSelected (wxCommandEvent& event);
	void OnECCTopButtonPressed(wxCommandEvent& event);
	void OnECCClearButtonPressed(wxCommandEvent& event);
	void OnECCCBuildSecureCurvePress(wxCommandEvent& event);
	void OnECCPointPlotSimpleEllipticCurveButtonPressed (wxCommandEvent& event);
   void OnDiffieHellmanGFMenuItemSelected (wxCommandEvent& event);
	void OnBuildLargeCurve(wxCommandEvent& event);
	void OnCreateGraphExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateRelationExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreatePartialOrderExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateEllipticCubicEncryptionExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateTreeBTRFileExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateTreeGTRFileExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreatGraphLayoutExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateTypesOfUndirectedGraphsExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateTypesOfDirectedGraphsExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateUndirectedGraphAlgorithmsExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateDirectedGraphAlgorithmsExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateSearchTreeFileExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateHeapFileExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateHuffmanTreeFileExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateExpressionTreeFileExamplesMenuItemSelected(wxCommandEvent& event);
	void OnCreateBalancedTreeFileExamplesMenuItemSelected(wxCommandEvent& event);
	void OnExit (wxCommandEvent& event);
	void OnCloseWindow (wxCloseEvent& event);
	void OnBigIntegerCalculatorXbitRandomIntegerButtonPress( wxCommandEvent& event );
	void handleMenuChoice (wxCommandEvent& event);
	void handleRelationGraphMenuChoice (wxCommandEvent& event);
	bool checkForGraphSequence (vector<int>& dataValues);
	bool checkForDigraphicalSequence (vector<int>& a,vector<int>& b);
	void insertionSort (vector<int>& v, int p, vector<wxString>& w);
	void insertionSort (vector<DigraphicalSequenceStorage>& v,
	                    bool comp (const DigraphicalSequenceStorage& lhs,
	                               const DigraphicalSequenceStorage& rhs) );
	int exists_bi_gtr0 (const vector<pair<int,int> >& v);
	void copyAllButVi (vector<DigraphicalSequenceStorage>& lst,
	                   const vector<pair<int,int> >& v, int i);
	void OnAnalyzeGraphButtonClick (wxCommandEvent& event);

	//(*Identifiers(DiscreteMathCalculatorFrame)
	static const long ID_A_VALUE_STATICTEXT;
	static const long ID_A_VALUE_TEXTCTRL;
	static const long ID_B_VALUE_STATICTEXT;
	static const long ID_B_VALUE_TEXTCTRL;
	static const long ID_N_VALUE_STATICTEXT;
	static const long ID_N_VALUE_TEXTCTRL;
	static const long ID_RESULT_VALUE_STATICTEXT;
	static const long ID_RESULT_VALUE_TEXTCTRL;
	static const long ID_DATA_PANEL;
	static const long ID_SPECIAL_OPERATIONS_STATICTEXT1;
	static const long ID_CLEAR_BUTTON;
	static const long ID_BASIC_OPERATORS_STATICTEXT;
	static const long ID_IS_A_PRIME_BUTTON;
	static const long ID_TRANSFER_STATICTEXT;
	static const long ID_MEMORY_STATICTEXT;
	static const long ID_ADD_BUTTON;
	static const long ID_ADD_MOD_N_BUTTON;
	static const long ID_PHI_N_BUTTON;
	static const long ID_TO_A_BUTTON;
	static const long ID_MEMORY_STORE_1_BUTTON;
	static const long ID_MEMORY_STORE_2_BUTTON;
	static const long ID_SUB_BUTTON;
	static const long ID_SUB_MOD_N_BUTTON;
	static const long ID_NULL_FILL_STATICTEXT;
	static const long ID_PHI_A_B_BUTTON;
	static const long ID_TO_B_BUTTON;
	static const long ID_MEMORY_1_PLUS_BUTTON;
	static const long ID_MEMORY_2_PLUS_BUTTON;
	static const long ID_MUL_BUTTON;
	static const long ID_MUL_MOD_N_BUTTON;
	static const long ID_X_TEXTCTRL;
	static const long ID_X_BIT_PRIME_BUTTON;
	static const long ID_TO_N_BUTTON;
	static const long ID_MEMORY_RECALL_1_BUTTON;
	static const long ID_MEMORY_RECALL_2_BUTTON;
	static const long ID_DIV_BUTTON;
	static const long ID_MOD_BUTTON;
	static const long ID_GCD_BUTTON;
	static const long ID_MEMORY_CLEAR_1_BUTTON;
	static const long ID_MEMORY_CLEAR_2_BUTTON;
	static const long ID_POWER_BUTTON;
	static const long ID_POWER_MOD_N_BUTTON;
	static const long ID_LEAST_COMMON_MULTIPLE_BUTTON;
	static const long ID_INVERSE_MOD_N_BUTTON;
	static const long ID_COPRIME_BUTTON;
	static const long ID_RESULT_MAGNITUDE_BUTTON;
	static const long ID_SPELL_RESULT_BUTTON;
	static const long ID_OPERATORS_PANEL;
	static const long ID_NUMBER_THEORETIC_PANEL;
	static const long ID_COUNTING_AND_ENUMERATION_N_STATICTEXT;
	static const long ID_COUNTING_AND_ENUMERATION_N_TEXTCTRL;
	static const long ID_COUNTING_AND_ENUMERATION_K_STATICTEXT;
	static const long ID_COUNTING_AND_ENUMERATION_K_TEXTCTRL;
	static const long ID_COUNTING_END_ENUMERATION_RESULT_STATICTEXT;
	static const long ID_COUNTING_AND_ENUMERATION_RESULT_TEXTCTRL;
	static const long ID_COUNTING_AND_ENUMERATION_RESULT_STATICTEXT;
	static const long ID_COUNTING_AND_ENUMERATION_ESTIMATE_PANEL;
	static const long ID_COUNTING_AND_ENUMERATION_COUNTING_TEXTCTRL;
	static const long ID_STATICLINE1;
	static const long ID_COUNTING_AND_ENUMERATION_LIST_PERMUTATIONS_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_LIST_COMBINATIONS_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_LIST_BUTTONS_PANEL;
	static const long ID_COUNTING_AND_ENUMERATION_FACTORIAL_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_PERMUTATIONS_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_COMBINATIONS_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_FIBONACCI_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_CATALAN_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_SCHRODER_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_BELL_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_STIRLING_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_ASYMPTOTIC_ESTIMATE_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_NUMBER_COMPUTATION_PANEL;
	static const long ID_COUNTING_AND_ENUMERATION_RESULT_MAGNITUDE_BUTTON;
	static const long ID_COUNTING_AND_ENUMERATION_PANEL;
	static const long ID_TRIANGLES_DATA_STATICTEXT;
	static const long ID_TRIANGLES_DATA_TEXTCTRL;
	static const long ID_TRIANGLES_DATA_PANEL;
	static const long ID_TRIANGLES_OUTPUT_TEXTCTRL;
	static const long ID_TRIANGLES_PASCAL_BUTTON;
	static const long ID_TRIANGLES_HOSOYA_BUTTON;
	static const long ID_TRIANGLES_CATALAN_BUTTON;
	static const long ID_TRIANGLES_SCHRODER_BUTTON;
	static const long ID_TRIANGLES_BELL_BUTTON;
	static const long ID_TRIANGLES_BUTTONS_PANEL;
	static const long ID_TRIANGLES_PANEL;
	static const long ID_SUBSETS_SET_SIZE_STATICTEXT;
	static const long ID_SUBSETS_SET_SIZE_TEXTCTRL;
	static const long ID_SUBSETS_K_STATICTEXT;
	static const long ID_SUBSETS_K_TEXTCTRL;
	static const long ID_SUBSETS_SIZE_K_PANEL;
	static const long ID_SUBSETS_COMPLETE_SIZE_K_PANEL;
	static const long ID_SUBSETS_NUMBER_OF_SUBSETS_STATICTEXT;
	static const long ID_SUBSETS_NUMBER_OF_SUBSETS_TEXTCTRL;
	static const long ID_SUBSETS_NUMBER_OF_SUBSSETS_PANEL;
	static const long ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_STATICTEXT;
	static const long ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_TEXTCTRL;
	static const long ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_PANEL;
	static const long ID_SUBSETS_ESTIMATE_PANEL;
	static const long ID_SUBSETS_LISTINGS_TEXTCTRL;
	static const long ID_SUBSETS_NUMBER_OF_SUBSETS_BUTTON;
	static const long ID_SUBSETS_NUMBER_OF_K_ELEMENT_SUBSETS_BUTTON;
	static const long ID_SUBSETS_LIST_ALL_SUBSETS_BUTTON;
	static const long ID_SUBSETS_LIST_ALL_K_ELEMENT_SUBSETS_BUTTON;
	static const long ID_SUBSETS_RESULT_MAGNITUDE_BUTTON;
	static const long ID_SUBSETS_ASYMPTOTIC_ESTIMATE_BUTTON;
	static const long ID_SUBSETS_BUTTONS_PANEL;
	static const long ID_SUBSETS_PANEL;
	static const long ID_PARTITIONS_N_STATICTEXT;
	static const long ID_PARTITIONS_N_TEXTCTRL;
	static const long ID_PARTITIONS_K_STATICTEXT;
	static const long ID_PARTITIONS_K_TEXTCTRL;
	static const long ID_PARTITIONS_N_K_PANEL;
	static const long ID_PARTITONS_BELL_NUMBER_STATICTEXT;
	static const long ID_PARTITIONS_BELL_NUMBER_TEXTCTRL;
	static const long ID_PARTITIONS_BELL_NUMBER_PANEL;
	static const long ID_PARTITIONS_STIRLING_NUMBER_STATICTEXT;
	static const long ID_PARTITIONS_STIRLING_NUMBER_TEXTCTRL;
	static const long ID_PARTITIONS_STIRLING_PANEL;
	static const long ID_PARTITIONS_LISTING_STATICTEXT;
	static const long ID_PARTITIONS_LISTING_LABEL_PANEL;
	static const long ID_PARTITIONS_LISTING_TEXTCTRL;
	static const long ID_PARTITIONS_BELL_BUTTON;
	static const long ID_PARTITIONS_STIRLING_BUTTON;
	static const long ID_PARTITONS_LIST_PARTITIONS_BUTTON;
	static const long ID_PARTITIONS_LIST_STIRLING_PARTITIONS_BUTTON;
	static const long ID_PARTITIONS_BUTTONS_PANEL;
	static const long ID_PARTITONS_PANEL;
	static const long ID_PARTITIONS_ESTIMATE_PANEL;
	static const long ID_PARTITIONS_RESULT_MAGNITUDE_BUTTON;
	static const long ID_PARTITIONS_ASYMPTOTIC_ESTIMATE_BUTTON;
	static const long ID_BINARY_TREES_GENERAL_TREE_STATICTEXT;
	static const long ID_BINARY_TREES_GENERAL_BINARY_TREE_ENTER_STATICTEXT;
	static const long ID_BINARY_TREES_GENERAL_TREE_TITLE_STATICTEXT;
	static const long ID_BINARY_TREES_GENERAL_TREE_TITLE_TEXTCTRL;
	static const long ID_BINARY_TREES_TITLE_PANEL;
	static const long ID_BINARY_TREES_ENTER_BTR_BUTTON;
	static const long ID_BINARY_TREES_ENTER_BTR_FROM_FILE_BUTTON;
	static const long ID_BINARY_TREES_BTR_PANEL;
	static const long ID_BINARY_TREES_STATICLINE;
	static const long ID_BINARY_TREES_ENTER_GML_BUTTON;
	static const long ID_BINARY_TREES_ENTER_GENERAL_TREE_PANEL;
	static const long ID_BINARY_TREES_HUFFMAN_TREE_STATICTEXT;
	static const long ID_STATICTEXT2;
	static const long ID_BINARY_TREE_HUFFMAN_TREE_TITLE_TEXTCTRL;
	static const long ID_BINARY_TREES_HUFFMAN_TREE_TITLE_PANEL;
	static const long ID_BINARY_TREES_HUFFMAN_TREE_ENTER_BUTTON;
	static const long ID_BINARY_TREES_HUFFMAN_TREE_STATICLINE;
	static const long ID_BINARY_TREES_HUFFMAN_TREE_FROM_GML_BUTTON;
	static const long ID_BINARY_TREES_HUFFMAN_TREE_ENTER_PANEL;
	static const long ID_BINARY_TREES_HUFFMAN_TREE_PANEL;
	static const long ID_BINARY_TREES_EXPRESSION_TREE_STATICTEXT;
	static const long ID_STATICTEXT3;
	static const long ID_BINARY_TREES_EXPRESSION_TREE_TITLE_TEXTCTRL;
	static const long ID_BINARY_TREES_EXPRESSION_TREE_TITLE_PANEL;
	static const long ID_BINARY_TREES_EXPRESSION_TREE_ENTER_BUTTON;
	static const long ID_STATICLINE2;
	static const long ID_BINARY_TREES_EXPRESSION_TREE_FROM_GML_BUTTON;
	static const long ID_BINARY_TREES_EXPRESSION_TREE_ENTER_PANEL;
	static const long ID_BINARY_TREES_EXPRESSION_TREE_PANEL;
	static const long ID_BINARY_TREES_HUFFMAN_PANEL;
	static const long ID_BINARY_TREES_STATICLINE1;
	static const long ID_BINARY_TREES_STATICLINE2;
	static const long ID_BINARY_TREES_STATICLINE3;
	static const long ID_BINARY_TREES_SEARCH_TREE_STATICTEXT;
	static const long ID_BINARY_SEARCH_TREES_STATICTEXT;
	static const long ID_BINARY_SEARCH_TREES_TITLE_STATICTEXT;
	static const long ID_BINARY_TREES_SEARCH_TREES_TITLE_TEXTCTRL;
	static const long ID_BINARY_SEARCH_TREES_TITLE_PANEL;
	static const long ID_BINARY_SEARCH_TREES_ENTER_BUTTON;
	static const long ID_BINARY_TREES_SEARCH_TREE_ENTER_FROM_FILE_BUTTON;
	static const long ID_BINARY_SEARCH_TREES_RANDOM_DATA_ENTER_BUTTON;
	static const long ID_BINARY_SEARCH_TREES_CONTROL_PANEL;
	static const long ID_BINARY_SEARCH_TREES_STATICLINE;
	static const long ID_BINARY_SEARCH_TREES_FROM_GML_BUTTON;
	static const long ID_BINARY_SEARCH_TREES_ENTER_PANEL;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RADIOBOX;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_STATICTEXT;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_TEXTCTRL;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_TITLE_PANEL;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_TITLE_PANEL;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_SELECTION_PANEL;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_ENTER_BUTTON;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_TEXTFILE_BUTTON;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_RANDOM_DATA_BUTTON;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_CONTROL_PANEL;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_STATICLINE;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_FROM_GML_BUTTON;
	static const long ID_BINARY_SEARCH_TREES_BALANCE_ALGORITHMS_PANEL;
	static const long ID_BINARY_TREES_CONTROL_PANEL;
	static const long ID_HEAP_STATICTEXT;
	static const long ID_HEAP_TITLE_STATICTEXT;
	static const long ID_HEAP_TITLE_TEXTCTRL;
	static const long ID_BUILD_HEAP_PANEL;
	static const long ID_ENTER_HEAP_RADIOBOX;
	static const long ID_ENTER_HEAP_BUTTON;
	static const long ID_ENTER_RANDOM_HEAP_BUTTON;
	static const long ID_ENTER_HEAP_FROM_TEXTFILE_BUTTON;
	static const long ID_ENTER_HEAP_PANEL;
	static const long ID_HEAP_STATICLINE;
	static const long ID_HEAP_ENTER_FROM_GML_BUTTON;
	static const long ID_PANEL2;
	static const long ID_GENERAL_TREE_STATICTEXT;
	static const long ID_GENERAL_TREE_TITLE_STATICTEXT;
	static const long ID_GENERAL_TREE_TITLE_TEXTCTRL;
	static const long ID_GENERAL_TREE_TITLE_PANEL;
	static const long ID_GENERAL_TREE_ENTER_FROM_TEXTFILE_BUTTON;
	static const long ID_GENERAL_TREE_ENTER_PANEL;
	static const long ID_GENERAL_TREE_STATICLINE;
	static const long ID_GENERAL_TREE_ENTER_FROM_GTR_BUTTON;
	static const long ID_GENERAL_TREES_ENTER_FROM_GML_BUTTON;
	static const long ID_PANEL1;
	static const long ID_GENERAL_TREE_PANEL;
	static const long ID_HEAP_GENERAL_TREE_PANEL;
	static const long ID_BINARY_TREES_PANEL;
	static const long ID_GRAPHS_PREVIEW_WINDOW;
	static const long ID_RELATIONS_PARTIAL_ORDERS_DRAWING_WINDOW;
	static const long ID_GRAPHS_PREVIEW_SLIDER;
	static const long ID_GRAPHS_PREVIEW_PANEL;
	static const long ID_GRAPHS_TYPE_RADIOBOX;
	static const long ID_GRAPHS_WEIGHTED_STATICTEXT;
	static const long ID_GRAPHS_WEIGHTED_CHECKBOX;
	static const long ID_GRAPHS_HIDE_EDGES_STATICTEXT;
	static const long ID_GRAPHS_HIDE_EDGES_CHECKBOX;
	static const long ID_GRAPHS_ALLOW_PARALLEL_EDGES_STATICTEXT;
	static const long ID_GRAPHS_ALLOW_PARALLEL_EDGES_CHECKBOX;
	static const long ID_GRAPHS_LAYOUT_PANEL;
	static const long ID_GRAPH_LAYOUT_ALGORITHM_STATICTEXT;
	static const long ID_GRAPH_LAYOUT_RADIOBOX;
	static const long ID_GRAPHS_LAYOUT_ALGORITHM_PANEL;
	static const long ID_GRAPHS_WEIGHTED_LAYOUT_PANEL;
	static const long ID_GRAPHS_TYPE_PANEL;
	static const long ID_GRAPHS_TITLE_STATICTEXT;
	static const long ID_GRAPHS_TITLE_TEXTCTRL;
	static const long ID_GRAPHS_TITLE_PANEL;
	static const long ID_GRAPHS_ENTER_FROM_DIALOG_BUTTON;
	static const long ID_GRAPHS_ENTER_FROM_GRPH_BUTTON;
	static const long ID_GRAPHS_ENTER_FROM_GML_BUTTON;
	static const long ID_GRAPHS_ENTER_FROM_PANEL;
	static const long ID_GRAPH_REDRAW_BUTTON;
	static const long ID_REDRAW_RANDOM_PANEL;
	static const long ID_GRAPHS_ENTER_PANEL;
	static const long ID_GRAPHS_ADD_ERASE_VERTEX_STATICTEXT;
	static const long ID_GRAPHS_ADD_VERTEX_BUTTON;
	static const long ID_GRAPHS_ERASE_VERTEX_BUTTON;
	static const long ID_GRAPHS_ADD_ERASE_VERTEX_BUTTON_PANEL;
	static const long ID_GRAPHS_ADD_ERASE_VERTEX_TEXTCTRL;
	static const long ID_GRAPHS_ADD_ERASE_VERTEX_PANEL;
	static const long ID_GRAPHS_ADD_ERASE_EDGE_STATICTEXT;
	static const long ID_GRAPHS_ADD_EDGE_BUTTON;
	static const long ID_GRAPHS_ERASE_EDGE_BUTTON;
	static const long ID_GRAPHS_ADD_ERASE_EDGE_PANEL;
	static const long ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_STATICTEXT;
	static const long ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_TEXTCTRL;
	static const long ID_GRAPHS_ADD_ERASE_EDGE_SOURCE_PANEL;
	static const long ID_GRAPHS_ADD_ERASE_EDGE_DEST_STATICTEXT;
	static const long ID_GRAPHS_ADD_ERASE_EDGE_DEST_TEXTCTRL;
	static const long ID_GRAPHS_ADD_ERASE_EDGE_DEST_PANEL;
	static const long ID_GRAPH_EDGE_WEIGHT_STATICTEXT;
	static const long ID_GRAPH_EDGE_WEIGHT_TEXTCTRL;
	static const long ID_GRAPH_EDGE_WEIGHT_PANEL;
	static const long ID_GRAPHS_FROM_TSP_BUTTON;
	static const long ID_GRAPHS_ADD_ERASE_EDGE_CHOICE_PANEL;
	static const long ID_ADD_ERASE_EDGE_PANEL;
	static const long ID_GRAPH_BUTTON_PANEL;
	static const long ID_SUBGRAPH_ISOMORPHISM_BUTTON;
	static const long ID_GRAPH_ISOMORPHISM_BUTTON;
	static const long ID_GRID_GRAPH_MENUITEM;
	static const long ID_GRAPHICAL_SEQUENCE_GRAPH_MENUITEM;
	static const long ID_DIGRAPHICAL_SEQUENCE_GRAPH_MENUITEM;
	static const long ID_ANALYZE_GRAPH_BUTTON;
	static const long ID_TRANSPOSE_GRAPH_BUTTON;
	static const long ID_CLEAR_GRAPH_BUTTON;
	static const long ID_GRAPH_BUTTONS_PANEL;
	static const long ID_COMPLEMENTARY_GRAPH_BUTTON;
	static const long ID_GRAPH_RUN_ALGORITHMS_BUTTON;
	static const long ID_GRAPHS_BUTTON_PANEL;
	static const long ID_GRAPHS_PANEL;
	static const long ID_DISCRETE_MATH_CALCULATOR_NOTEBOOK;
	static const long ID_DISCRETE_MATH_CALCULATOR_PANEL;
	static const long idMenuQuit;
	static const long ID_CREATE_RANDOM_GRAPH_MENUITEM;
	static const long ID_DRAW_KN_MENUITEM;
	static const long ID_GRAPHS_MAKE_CLIQUE;
	static const long ID_HYPERCUBE_MENUITEM;
	static const long ID_STABLE_MARRIAGE_MENUITEM;
	static const long ID_CREATE_HAMCYCLE_MENUITEM;
	static const long ID_CREATE_GENERALIZED_PETERSON_GRAPH_MENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_HELP_MENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_HELP_ABOUT_MENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_STATUSBAR;
	//*)
	static const long ID_CREATE_TSP_MENUITEM;
	static const long ID_GRAPHS_PREVIEW_WINDOW_PRINT_MENUITEM;
	static const long ID_GRAPHS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM;
	static const long ID_GRAPHS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM;
	static const long ID_GRAPHS_PREVIEW_WINDOW_SAVE_AS_MENUITEM;
	static const long ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_MENUITEM;
	static const long ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PRINT_PREVIEW_MENUITEM;
	static const long ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_PAGE_SETUP_MENUITEM;
	static const long ID_RELATIONS_PARTIAL_ORDERS_PREVIEW_WINDOW_SAVE_AS_MENUITEM;
	static const long ID_RELATIONS_AND_PARTIALORDERS_PANEL;
	static const long ID_GF_P_ELLIPTIC_CURVE_PANEL;
	static const long ID_BUILD_GF_p_SAVECURVEPARAMETERSMENUITEM;
	static const long ID_BUILD_GF_p_READCURVEPARAMETERSMENUITEM;
	static const long ID_BUILD_GF_p_DECIMALMENUITEM;
	static const long ID_ECCPUBLICKEYAPPLICATIONMENUITEM;
	static const long ID_ECDIGITALSIGNATUREAPPLICATIONMENUITEM;
	static const long ID_PAIRINGSAPPLICATIONMENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DISCRETE_LOGARITHM_MENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DIGITAL_SIGNATURE_MENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_RSA_MENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_PUBLIC_KEY_ENCRYPTION_MENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_FACTOR_MENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_DIFFIE_HELLMANKEYEXCHANGE_MENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_ECDLP_MENUITEM;
	static const long ID_ELLIPTIC_CURVE_DIGITAL_SIGNATURE_MENUITEM;
	static const long ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_SAVE_CURVE_BUTTON;
	static const long ID_DISCRETE_MATH_CALCULATOR_BIG_INTEGER_READ_CURVE_BUTTON;
	static const long ID_BINARY_TREE_EXAMPLES_MENUITEM;
	static const long ID_TXT_TREE_EXAMPLES_MENUITEM;
	static const long ID_GML_TREE_EXAMPLES_MENUITEM;
	static const long ID_GENERAL_TREE_EXAMPLES_MENUITEM;
	static const long ID_GRAPH_EXAMPLES_MENUITEM;
	static const long ID_RELATIONS_EXAMPLES_MENUITEM;
	static const long ID_PARTIAL_ORDER_EXAMPLES_MENUITEM;
	static const long ID_ECC_SAMPLES_MENUITEM;
	static const long ID_GRAPH_LAYOUTS_EXAMPLES_MENUITEM;
	static const long ID_TYPES_OF_UNDIRECTED_GRAPHS_EXAMPLES_MENUITEM;
	static const long ID_TYPES_OF_DIRECTED_GRAPHS_EXAMPLES_MENUITEM;
	static const long ID_UNDIRECTED_GRAPH_ALGORITHMS_EXAMPLES_MENUITEM;
	static const long ID_DIRECTED_GRAPH_ALGORITHMS_EXAMPLES_MENUITEM;
	static const long ID_SEARCH_TREE_EXAMPLES_MENUITEM;
	static const long ID_HEAP_EXAMPLES_MENUITEM;
	static const long ID_HUFFMAN_TREE_EXAMPLES_MENUITEM;
	static const long ID_EXPRESSION_TREE_EXAMPLES_MENUITEM;
	static const long ID_BALANCED_SEARCH_TREE_EXAMPLES_MENUITEM;
	static const long ID_ENCRYPTION_PANEL;

	//(*Declarations(DiscreteMathCalculatorFrame)
	wxButton* sub_mod_n_Button;
	wxButton* binarySearchTreesEnterButton;
	wxButton* complementaryGraphButton;
	wxPanel* graphEdgeWeightPanel;
	wxButton* binaryTreesEnterBTRFromFileButton;
	wxStaticLine* binaryTreesExpressionTreeStaticLine;
	wxPanel* subsetsPanel;
	wxPanel* graphsAddEraseVertexPanel;
	wxTextCtrl* bValueTextCtrl;
	wxPanel* binaryTreesHuffmanTreeTitlePanel;
	wxPanel* trianglesDataPanel;
	wxButton* generalTreesEnterFromGMLButton;
	wxPanel* generalTreePanel;
	wxStaticLine* binaryTreesStaticLine;
	wxButton* memoryRecall1Button;
	wxMenu* discreteMathCalculatorMenu;
	wxMenuItem* discreteMathCalculatorHelpMenuItem;
	wxMenuItem* aboutMenuItem;
	wxStaticText* graphsLayoutAlgorithmStaticText;
	wxStaticText* countingAndEnumeration_n_StaticText;
	wxRadioBox* graphsGraphTypeRadioBox;
	wxButton* memory1PlusButton;
	wxButton* triangelsPascalButton;
	wxMenuItem* graphsMakeGraphWithACliqueMenuItem;
	wxPanel* graphsAddEraseEdgeButtonsPanel;
	wxNotebook* discreteMathCalculatorNotebook;
	wxStaticText* graphsAddEraseEdgeStaticText;
	wxStaticText* aValueStaticText;
	wxButton* binaryTreesHuffmanTreeFromGMLButton;
	wxButton* to_n_Button;
	wxButton* partitionsListPartitionsButton;
	wxButton* binaryTreesExpressionTreeEnterButton;
	wxPanel* partitionsPanel;
	wxPanel* graphsEnterPanel;
	wxButton* powerButton;
	wxStaticText* subsetsNumberOfSubsetsStaticText;
	wxButton* power_mod_n_Button;
	wxButton* graphsEnterFromDialogButton;
	wxButton* addButton;
	wxStaticText* subsetsSetSizeStaticText;
	wxRadioBox* binarySearchTreesBalanceAlgorithmsRadioBox;
	wxScrolledWindow* countingAndEnumerationPanel;
	wxStaticText* bValueStaticText;
	wxTextCtrl* heapBuildTitleTextCtrl;
	wxButton* memoryClear2Button;
	wxPanel* partitions_n_k_Panel;
	DrawingCanvas* previewWindow;
	DrawingCanvas* relationPartialOrderDrawing;
	wxSlider* relationPartialOrderSlider;
	wxPanel* partitionsBellNumberPanel;
	wxTextCtrl* nValueTextCtrl;
	wxButton* x_bit_prime_Button;
	wxMenuItem* stableMarriageMenuItem;
	wxTextCtrl* countingAndEnumeration_listing_TextCtrl;
	wxPanel* Panel1;
	wxTextCtrl* binaryTreesSearchTreesTitleTextCtrl;
	wxTextCtrl* partitionsStirlingNumberTextCtrl;
	wxStaticText* graphsAddEraseVertexStaticText;
	wxButton* clearButton;
	wxPanel* enterHeapPanel;
	wxPanel* binaryTreesHuffmanPanel;
	wxButton* leastCommonMultipleButton;
	wxButton* enterRandomHeapButton;
	wxStaticText* graphsAllowParallelEdgesStaticText;
	wxStaticText *graphsHideEdgesStaticText;
	wxButton* memoryStore1Button;
	wxPanel* discreteMathCalculatorPanel;
	wxButton* binaryTreesEnterBTRButton;
	wxButton* spellResultButton;
	wxStaticText* binaryTreesGeneralBinaryTreeEnterStaticText;
	wxPanel* binaryTreesEnterGeneralTreePanel;
	wxPanel* graphsEnterFromPanel;
	wxTextCtrl* subsetsKTextCtrl;
	wxTextCtrl* partitions_k_TextCtrl;
	wxPanel* dataPanel;
	wxButton* memoryStore2Button;
	wxPanel* graphsWeightedLayoutPanel;
	wxTextCtrl* binaryTreesHuffmanTreeTitleTextCtrl;
	wxButton* trianglesHosoyaButton;
	wxStaticText* resultValueStaticText;
	wxTextCtrl* trianglesDataTextCtrl;
	wxPanel* graphsAddEraseEdgePanel;
	wxStaticText* binaryTreesGeneralTreeTitleStaticText;
	wxMenu* discreteMathCalculatorHelpMenu;
	wxMenu* createGraphMenu;
	void buildMenuBarFor(int tab);
	wxTextCtrl* aValueTextCtrl;
	wxButton* subsetsNumberOfSubsetsButton;
	wxStaticLine* binarySearchTreesStaticLine;
	wxPanel* graphControlPanel;
	wxPanel* binaryTreesExpressionTreeEnterPanel;
	wxPanel* binarySearchTreesBalanceAlgorithmsControlPanel;
	wxPanel* generalTreeEnterPanel;
	wxPanel* binarySearchTreesTitlePanel;
	wxStaticText* partitonsStirlingNumberStaticText;
	wxStaticLine* binaryTreesStaticLine2;
	wxPanel* binaryTreesBTRPanel;
	wxStaticText* graphsAddEraseEdgeSourceStaticText;
	wxButton* partitionsBellButton;
	wxStaticText* countingAndEnumeration_Result_StaticText;
	wxStaticText* partitions_k_StaticText;
	wxPanel* numberTheoreticPanel;
	wxTextCtrl* binarySearchTreesBalanceAlgorithmsTitleTextCtrl;
	wxStaticText* binaryTreeesHuffmanTreeStaticText;
	wxButton* mulButton;
	wxPanel* subsetsSetSizeKPanel;
	wxButton* countingAndEnumerationAsymptoticEstimateButton;
	wxStaticText* heapBuildTitleStaticText;
	wxPanel* binarySearchTreesEnterControlPanel;
	wxStaticLine* binaryTreesHuffmanTreeStaticLine;
	wxButton* trianglesResultMagnitudeButton;
	wxButton* trianglesAsymptoticEstimateButton;
	wxButton* phiNButton;
	wxStaticLine* binaryTreesStaticLine3;
	wxPanel* binarySearchTreesEnterPanel;
	wxButton* binaryTreesExpressionTreeFromGMLButton;
	wxButton* memoryClear1Button;
	wxButton* graphRedrawButton;
	wxButton* inverse_mod_a_Button;
	wxButton* isAPrimeButton;
	wxPanel* binarySearchTreesBalanceTitlePanel;
	wxButton* trianglesBellButton;
	wxButton* countingAndEnumerationStirlingButton;
	wxButton* generalTreeEnterButton;
	wxStaticText* generalTreeStaticText;
	wxTextCtrl* partitonsListingTextCtrl;
	wxButton* graphsEnterFromGRPHButton;
	wxStaticText* binaryTreesExpressionTreeTitleStaticText;
	wxPanel* treesPanel;
	wxPanel* binarySearchTreesBalanceAlgorithmsSelectionPanel;
	wxTextCtrl* binaryTreesExpressionTreeTitleTextCtrl;
	wxPanel* binaryTreesExpressionTreePanel;
	wxPanel* operatorsPanel;
	wxTextCtrl* countingAndEnumeration_k_TextCtrl;
	wxPanel* graphsWeightedPanel;
	wxTextCtrl* graphsAddEraseVertexTextCtrl;
	wxTextCtrl* subsetsListingsTextCtrl;
	wxRadioBox* graphLayoutRadioBox;
	wxPanel* graphsRedrawRandomPanel;
	wxButton* graphsEraseEdgeButton;
	wxButton* countingAndEnumerationCatalanButton;
	wxStaticText* basicOperatorsStaticText;
	wxButton* enterHeapButton;
	wxStaticText* subsetsNumberOfKElementSubsetsStaticText;
	wxMenuItem* createRandomGraphMenuItem;
	wxMenuItem* digraphicalSequenceGraphMenuItem;
	wxPanel* graphsLayoutPanel;
	wxButton* partitionsStirlingButton;
	wxTextCtrl* resultValueTextCtrl;
	wxButton* heapEnterFromGMLButton;
	wxPanel* binaryTreesControlPanel;
	wxTextCtrl* graphsAddEraseEdgeDestTextCtrl;
	wxPanel* binaryTreesExpressionTreeTitlePanel;
	wxButton* countingAndEnumerationListPermutationsButton;
	wxPanel* trianglesButtonsPanel;
	wxButton* countingAndEnumerationSchroderButton;
	wxButton* trianlgesCatalanButton;
	wxButton* graphsEnterFromGMLButton;
	wxTextCtrl* subsetsNumberofSubsetsTextCtrl;
	wxTextCtrl* binaryTreesGeneralTreeTitleTextCtrl;
	wxButton* binaryTreesEnterGMLFileButton;
	wxButton* binarySearchTreesBalanceAlgorithmsFromGMLButton;
	wxPanel* binarySearchTreesBalanceAlgorithmsTitlePanel;
	wxStaticText* binarySearchTreesStaticText;
	wxPanel* generalTreeTitlePanel;
	wxStaticText* binaryTreesGeneralTreeStaticText;
	wxStaticText* countingAndEnumeration_result_StaticText;
	wxStaticText* specialOperationsStaticText;
	wxButton* to_b_Button;
	wxButton* countingAndEnumerationFactorialButton;
	wxStaticLine* binaryTreesStaticLine1;
	wxStaticText* graphsAddEraseEdgeDestStaticText;
	wxTextCtrl* generalTreeTitleTextCtrl;
	wxButton* graphRunAlgorithmsButton;
	wxPanel* graphsPanel;
	wxButton* generalTreeEnterFromGTRButton;
	wxButton* subButton;
	wxTextCtrl* partitionsBellNumberTextCtrl;
	wxPanel* partitionsEstimatePanel;
	wxButton* partitionsResultMagnitudeButton;
	wxButton* partitionsAsymptoticEstimateButton;
	wxButton* binaryTreesHuffmanTreeEnterButton;
	wxMenuBar* discreteMathCalculatorMenuBar;
	wxButton* enterHeapFromTextfileButton;
	wxButton* coprimeButton;
	wxButton* binarySearchTreesRandomDataEnterButton;
	wxTextCtrl* countingAndEnumeration_n_TextCtrl;
	wxPanel* heapPanel;
	wxButton* countingAndEnumerationListCombinationsButton;
	wxButton* to_a_Button;
	wxPanel* subsetsCompleteSizeKPanel;
	wxStaticLine* binarySearchTreesBalanceAlgorithmsStaticLine;
	wxTextCtrl* partitions_n_TextCtrl;
	wxTextCtrl* subsetsSetSizeTextCtrl;
	wxStaticText* graphsWeightedStaticText;
	wxPanel* partitionsButtonsPanel;
	wxStaticText* transferStaticText;
	wxButton* modButton;
	wxButton* countingAndEnumerationCombinationsButton;
	wxButton* subsetsNumberOfKElementSubsetsButton;
	wxButton* phi_a_b_Button;
	wxButton* countingAndEnumerationPermutationsButton;
	wxStaticText* heapStaticText;
	wxTextCtrl* graphsAddEraseEdgeSourceTextCtrl;
	wxStaticText* graphEdgeWeightStaticText;
	wxTextCtrl* subsetsNumberOfKElementSubsetsTextCtrl;
	wxMenuItem* generalizedPetersonGraphMenuItem;
	wxMenuItem* createGridGraphMenuItem;
	wxMenuItem* createGraphSequenceGraphMenuItem;
	wxButton* binarySearchTreesFromGMLButton;
	wxPanel* partitionsListingLabelPanel;
	wxButton* subGraphIsomorphismButton;
	wxButton* graphIsomorphismButton;
	wxButton* clearGraphButton;
	wxButton* analyzeGraphButton;
	wxButton* transposeGraphButton;
	wxButton* gcdButton;
	wxPanel* trianglesPanel;
	wxStaticLine* heapStaticLine;
	wxRadioBox* enterHeapRadioBox;
	wxPanel* graphsTitlePanel;
	wxPanel* countingAndEnumerationListButtonsPanel;
	wxMenuItem* createHamCycleMenuItem;
	wxButton* binarySearchTreesBalanceAlgorithmsFromTextfileButton;
	wxPanel* graphsPreviewPanel;
	wxPanel* graphsAddEraseEdgeDestPanel;
	wxPanel* heapBuildTitlePanel;
	wxButton* divButton;
	wxButton* graphsAddEdgeButton;
	wxStaticText* partitionsBellNumberStaticText;
	wxButton* graphsEraseVertexButton;
	wxStaticText* trianglesDataStaticText;
	wxTextCtrl* trianglesOutputTextCtrl;
	wxCheckBox* graphsAllowParallelEdgesCheckBox;
	wxCheckBox *graphsHideEdgesCheckBox;
	wxButton* graphsEnterFromTSP;
	wxButton* memoryRecall2Button;
	wxButton* BobRSANumberOfBitsToUseForEncryptionTextCtrl;
	wxStaticText* binaryTreesHuffmanTreeTitleStaticText;
	wxSlider* graphsPreviewSlider;
	wxPanel* actionButtonsPanel;
	wxButton* binarySearchTreesBalanceAlgorithmsEnterButton;
	wxStatusBar* DiscreteMathCalculatorStatusBar;
	wxTextCtrl* xTextCtrl;
	wxTextCtrl* graphEdgeWeightTextCtrl;
	wxPanel* subsetsButtonsPanel;
	wxStaticText* binaryTreesSearchTreeStaticText;
	wxPanel* graphsAddEraseVertexButtonPanel;
	wxButton* partitionsListStirlingPartitionsButton;
	wxPanel* graphsAddEraseEdgeChoicePanel;
	wxButton* graphsAddVertexButton;
	wxStaticText* generalTreeTitleStaticText;
	wxPanel* binarySearchTreesBalanceAlgorithmsPanel;
	wxStaticText* binarySearchTreesTitleStaticText;
	wxStaticText* partitionsListingStaticText;
	wxStaticText* countingAndEnumeration_k_StaticText;
	wxPanel* heapGeneralTreePanel;
	wxPanel* countingAndEnumerationNumberComputationPanel;
	wxButton* trianglesSchroderButton;
	wxButton* memory2PlusButton;
	wxTextCtrl* countingAndEnumerationResultTextCtrl;
	wxStaticText* binaryTreesExpressionTreeStaticText;
	wxMenuItem* drawKnMenuItem;
	wxButton* mul_mod_n_Button;
	wxButton* add_Mod_n_Button;
	wxPanel* graphsAddEraseEdgeSourcePanel;
	wxButton* binarySearchTreesBalanceAlgorithmsRandomDataButton;
	wxStaticText* binarySearchTreesBalanceAlgorithmsTitleStaticText;
	wxPanel* binaryTreesTitlePanel;
	wxPanel* subsetsEstimatePanel;
	wxButton* subsetsAsymptoticEstimateButton;
	wxPanel* subsetsNumberOfKElementSubsetsPanel;
	wxButton* subsetsListAllSubsetsButton;
	wxButton* subsetsListAllKElementSubsetsButton;
	wxPanel* countingAndEnumerationEstimatePanel;
	wxPanel* subsetsNumberOfSubsetsPanel;
	wxButton* binarySearchTreesEnterFromFileButton;
	wxPanel* binaryTreesHuffmanTreePanel;
	wxButton* countingAndEnumerationResultMagnitudeButton;
	wxButton* countingAndEnumerationFibonacciButton;
	wxButton* resultMagnitudeButton;
	wxStaticLine* generalTreeStaticLine;
	wxStaticText* nullFillStaticText;
	wxStaticText* partitions_n_StaticText;
	wxButton* countingAndEnumerationBellButton;
	wxStaticText* memoryStaticText;
	wxStaticText* subsetsKStaticText;
	wxPanel* graphsTypePanel;
	wxMenuItem* hypercubeMenuItem;
	wxPanel* partitionsStirlingPanel;
	wxButton* subsetsResultMagnitudeButton;
	wxStaticText* nValueStaticText;
	wxCheckBox* graphsWeightedCheckBox;
	wxPanel* binaryTreesHuffmanTreeEnterPanel;
	wxPanel* relationsAndPartialOrdersPanel;
	wxPanel* RelationsPartialOrdersButtonsPanel;
	wxPanel* InputRelationPanel;
	wxStaticText* InputRelationStaticText;
	wxPanel* InputRelationButtonsPanel;
	wxButton* InputRelationEnterButton;
	wxPanel* RelationClosurePanel;
	wxStaticText* RelationClosureStaticText;
	wxPanel* RelationClosureButtonsPanel;
	wxButton* RelationReflexiveClosureButton;
	wxButton* RelationSymmetricClosureButton;
	wxButton* RelationTransiveClosureButton;
	wxButton* RelationPropertiesButton;
	wxPanel* PartialOrderInputPanel;
	wxStaticText* PartialOrderInputPanelStaticText;
	wxPanel* PartialOrderInputButtonsPanel;
	wxButton* HasseDiagramEnterButton;
	wxButton* OutputRelationGMLButton;
	wxButton* InputRelationRELButton;
	wxButton* OutputPartialOrderGMLButton;
	wxButton* InputRelationFromGMLButton;
	wxButton* InputPartialOrderFromGMLButton;
	wxPanel* EllipticCurvePanel;
	wxPanel* EncryptionPanel;
	wxPanel* ECCDataPanel;
	wxStaticText* ECCEllipticCurveLabelStaticText;
	wxPanel* ECCEllipticCurvePanel;
	wxStaticText* ECCPrimeStaticText;
	wxStaticText* ECCSeedStaticText;
	wxTextCtrl* ECCSeedTextCtrl;
	wxTextCtrl* ECCPrimeTextCtrl;
   wxButton* ECCSaveCurveParametersButton;
	wxButton* ECCReadCurveParametersButton;
	wxStaticText* ECCCurveOrderStaticText;
	wxTextCtrl* ECCCurveOrderTextCtrl;
	wxPanel* ECCSeedABPanel;
	wxStaticText* ECCCoefficientAStaticText;
	wxTextCtrl* ECCCoefficientATextCtrl;
	wxStaticText* ECCCoefficientBStaticText;
	wxTextCtrl* ECCCoefficientBTextCtrl;
	wxStaticText* ECCBasePointStaticText;
	wxPanel* ECCBasePointPanel;
	wxStaticText* ECCBasePoint_x_CoordinateStaticText;
	wxTextCtrl* ECCBasePoint_x_CoordinateTextCtrl;
	wxStaticText* ECCBasePoint_y_CoordinateStaticText;
	wxTextCtrl* ECCBasePoint_y_CoordinateTextCtrl;
	wxPanel* ECCOrderCofactorPanel;
	wxStaticText* ECCSubgroupOrderStaticText;
	wxTextCtrl* ECCSubgroupOrderTextCtrl;
	wxStaticText* ECCSubgroupCofactorStaticText;
	wxTextCtrl* ECCSubgroupCofactorTextCtrl;
	wxStaticText* ECCPointQStaticText;
	wxPanel* ECCPointQPanel;
	wxStaticText* ECCPointQ_x_CoordinateStaticText;
	wxTextCtrl* ECCPointQ_x_CoordinateTextCtrl;
	wxStaticText* ECCPointQ_y_CoordinateStaticText;
	wxTextCtrl* ECCPointQ_y_CoordinateTextCtrl;
	wxStaticText* ECCPointRStaticText;
	wxPanel* ECCPointRPanel;
	wxStaticText* ECCScalarMultipleStaticText;
	wxPanel* ECCBigIntegerPanel;
	wxStaticText* ECCkBigintStaticText;
	wxTextCtrl* ECCkBigintTextCtrl;
	wxStaticText* ECCrBigintStaticText;
	wxTextCtrl* ECCrBigintTextCtrl;
	wxStaticText* ECCResultStaticText;
	wxPanel* ECCResultPanel;
	wxStaticText* ECCResult_x_coordinateStaticText;
	wxTextCtrl* ECCResult_x_coordinateTextCtrl;
	wxStaticText* ECCResult_y_coordinateStaticText;
	wxTextCtrl* ECCResult_y_coordinateTextCtrl;
	wxPanel* ECCOperatorPanel;
	wxButton* ECCOrderButton;
	wxButton* ECCAddButton;
	wxButton* ECCDoubleButton;
	wxButton* ECCTokButton;
	wxButton* ECCBasePointOrderButton;
	wxButton* ECCScalarMultiplicationButton;
	wxButton* ECCVerifyButton;
	wxButton* ECCCheckCurveButton;
	wxButton* ECCToQButton;
	wxButton* ECCTorButton;
	wxButton* ECCFindNewBasePoint;
	wxButton* ECCTRandom160BitSeedButton;
	wxButton* ECCToRButton;
	wxButton* ECCAddBigsModButton;
	wxButton* ECCMultiplyBigsButton;
	wxButton* ECCBigInverseButton;
	wxButton* ECCRandomScalarButton;
	wxButton* ECCRToQButton;
	wxButton* ECCtoSeedButton;
	wxStaticLine* ECCStaticLine1;
	wxStaticLine* ECCStaticLine2;
	wxStaticLine* ECCStaticLine3;
	wxStaticLine* ECCStaticLine4;
	wxStaticLine* ECCStaticLine5;
	wxPanel* ECCCurvePanel;
	wxButton *ECCChooseOwnCurveButton;
	wxButton* ECCCreateSecureDomainParameters;
	wxButton* ECCPlotSimpleECButton;
	wxButton* ECCPointPlotSimpleECButton;
	wxButton* ECCDiscreteLogarithmButton;
	wxPanel* ECCRandomkBitPrimePanel;
	wxStaticText* ECCNumberBitsInPrimeStaticText;
	wxTextCtrl* ECCNumberBitsInPrimeTextCtrl;
	wxButton* ECCkBitPrimeButton;
	wxButton* ECCRandomPointQButton;
	wxButton* ECCTopButton;
	wxButton* ECCClearButton;
	wxPanel* EncryptionSelectionPanel;
	wxRadioBox* EncryptionAlgorithmRadioBox;
	wxRadioBox* InputTypeRadioBox;
	wxRadioBox* DisplayInputRadioBox;
	wxButton* ReadFileButton;
	wxTextCtrl* FileToEncryptTextCtrl;
	wxPanel* PrepareToEncryptPanel;
	wxButton* ReadKeyButton;
	wxStaticText* KeyForPrepareEncryptionStaticText;
	wxTextCtrl* KeyTextCtrl;
	wxButton* SaveKeyButton;
	wxPanel* InitializationVectorPanel;
	wxButton* ReadVectorButton;
	wxStaticText* InitializationVectorStaticText;
	wxTextCtrl* InitializationVectorTextCtrl;
	wxButton* SaveVectorButton;
	wxPanel* PerformEncryptionPanel;
	wxButton* PerformEncryptionButton;
	wxTextCtrl* EncryptedTextCtrl;
	wxButton* SaveEncryptedTextButton;
	wxPanel* DecryptionPanel;
	wxButton* DecryptButton;
	wxTextCtrl* DecryptedTextCtrl;
	wxPanel* ExitPanel;
	wxButton* BigIntegerCalculatorXbitIntegerButton;
	wxTextCtrl* BigIntegerCalculatorXbitIntegerTextCtrl;
	wxMenuItem* createTSPMenuItem;
	wxMenu* buildGF_p_EllipticCurvesMenu;
	wxMenuItem* GF_p_SaveCurveParametersMenuItem;
	wxMenuItem* GF_p_ReadCurveParametersMenuItem;
	wxMenu* ECCNumberBaseMenu;
	wxMenuItem* ECCBase10MenuItem;
	wxMenuItem* ECCBase16MenuItem;
	wxMenu* ECCChooseNumberBaseSubmenu;
	wxMenuItem* ECCDefaultNumberBaseDecimal;
	wxMenuItem* ECCDefaultNumberBaseHexadecimal;
	wxMenu* ECCCApplications;
	wxMenuItem* ECCPublicKeyEncryption;
	wxMenuItem* ECCDigitalSignature;
	wxMenu* ECCExamples;
	wxMenuItem* ECCExamplesMenuItem;

	wxMenu* RelationPartialOrderExamples;
	wxMenuItem* RelationPartialOrderExamplesMenuItem;

	wxMenu* TreeExamples;
	wxMenu* GraphExamples;

	wxMenuItem* BinaryTreeExamples;
	wxMenuItem* BinarySearchTreeExamples;
	wxMenuItem* HeapExamples;
	wxMenuItem* HuffmanTreeExamples;
	wxMenuItem* ExpressionTreeExamples;
	wxMenuItem* BalancedBinarySearchTreeExamples;
	wxMenuItem* GeneralTreeExmples;
	wxMenu* BigIntegerToolsMenu;
	wxMenu* toolsMenu;
	wxMenuItem* discreteMathCalculatorBigIntegerDiscreteLogarithmMenuItem;
	wxMenuItem* discreteMathCalculatorBigIntegerRSAMenuItem;
	wxMenuItem* discreteMathCalculatorBigIntegerPublicKeyEncryptionMenuItem;
	wxMenuItem* discreteMathCalculatorBigIntegerFactorMenuItem;
	wxMenuItem* discreteMathCalculatorBigIntegerDiffieHellmanKeyExchangeMenuItem;
	wxMenuItem* discreteMathCalculatorBigIntegerDigitalSignatureMenuItem;
	wxMenuItem* discreteMathCalculatorEllipticCurveDiffieHellman;
	wxMenuItem* discreteMathCalculatorBigIntegerECDLPMenuItem;
	wxMenuItem* ellipticCurveDigitalSignatureMenuItem;

private:

	enum FrameType
	{
		BINARY_TREE,
		BINARY_SEARCH_TREE,
		HUFFMAN_TREE,
		EXPRESSION_TREE,
		HEAP,
		GENERAL_TREE
	};

	enum ComputationPage
	{
		BgIntCalculator,
		CombCalc
	};

	enum
	{
		BIGCURVEBITS,
		BIGCURVEP,
		BIGCURVEA,
		BIGCURVEB,
		BIGCURVEORDER,
		BIGCURVEX,
		BIGCURVEY
	};

	bool ProcessEvent (wxEvent& event);
	int getint (wxTextCtrl* tctrl);
	void setValue (ZZ n, wxTextCtrl *tctrl);
	ZZ getResult();
	void GetCurveParameter (wxTextCtrl* t, Big& parm);
	void inputTreeData (const wxString filename, wxVector<int>& dataInput);
	wxFrame *launchFrame (FrameType frameType, const wxString& title = wxT (""),
	                      SearchTreeDrawingCanvas::BalanceType balanceType = SearchTreeDrawingCanvas::NotBalanced,
	                      const wxVector<int>& inputData = wxVector<int>() );
	bool saveGraphPreview (bool exitApp);
	bool saveRelationPreview (bool exitApp);
	void GraphSequence();
	void determineGraphType();
	void get_ab();
	void get_n();
	void get_a();
	void add();
	void sub();
	void mul();
	void div();
	void intPower();
	void inverseamodn();
	void eulerPhi();
	void generatePrime();
	void gcd();
	void addModn();
	void subModn();
	void mulModn();
	void intPowerModn();
	void primeTest();
	void eulerPhipq();
	void toa();
	void tob();
	void ton();
	void memoryStore (int buttonNumber);
	void memoryRecall (int buttonNumber);
	void findCoprime();
	void get_polyab();
	void get_polya();
	void drawGraphSequence (vector<int>& v);
	void drawDigraphicalSequence (vector<pair<int,int> >& v);
	void polyMemoryStore (int buttonNumber);
	void polyMemoryRecall (int buttonNumber);
	void clearCurveData();
	void buildGFpEllipticCurvesMenu();
	void buildTreeExamplesMenu();
	void buildGraphExamplesMenu();
	void buildRelationsPartialOrderExamplesMenu();
	void buildEllipticCubicCurveExamplesMenu();
   void constructCreateMenu(int typeSelected);
	CurveComponent parseCurveFileLine(const wxString& line);
	CombinationsPermutations *comb_perm;
	Triangles *triangles;
	Subsets *subsets;
	Partition *partitions;
	bool searchTreeOpen;
	wxSFDiagramManager m_canvas;
	wxSFDiagramManager m_relationCanvas;
	bool dirty;
	bool relationDrawingIsDirty;
	MenuEventHandler *menuHandler;
	MenuEventHandler *relationMenuHandler;
	wxMenu *fileMenu;
	wxMenuItem *graphPreviewPrintMenuItem;
	wxMenuItem *graphPreviewPrintPreviewMenuItem;
	wxMenuItem *graphPreviewPageSetupMenuItem;
	wxMenuItem *graphPreviewSaveAsMenuItem;
	wxMenuItem *graphRelationsPartialOrdersPreviewPrintMenuItem;
	wxMenuItem *graphRelationsPartialOrdersPreviewPrintPreviewMenuItem;
	wxMenuItem *graphRelationsPartialOrdersPreviewPageSetupMenuItem;
	wxMenuItem *graphRelationsPartialOrdersPreviewSaveAsMenuItem;
	wxMenuItem* RelationExamplesMenuItem;
	wxMenuItem* GraphLayoutsExamples;
	wxMenuItem* TypesOfUndirectedGraphsExamples;
	wxMenuItem* TypesOfDirectedGraphsExamples;
	wxMenuItem* UndirectedGraphAlgorithmExamples;
	wxMenuItem* DirectedGraphAlgorithmExamples;
	wxMenuItem* PartialOrderExamplesMenuItem;
	wxString graphTitle;
	BuildGraph *myGraph;
	BuildGraph *myRelationDigraph;
	bool allowParallelEdges;
	bool isDigraph;
	bool isWeighted;
	bool hideEdges;
	GraphLayout graphLayout;
	GraphType graphType;
	ZZ memory;
	ZZ a, b, n, result_bigint, result_comb, memory1, memory2;
	wxString aValue, bValue, nValue, resultValue;
	bool figureSaved;
	bool relationFigureSaved;
	int page;
	bool destroyCalled;
	bool clipboardChecked;
	ECn GetPoint (wxTextCtrl *x, wxTextCtrl *y, const wxString& msg);
	void WriteScalarResult (wxTextCtrl* t, const Big& v);
	void WritePointResult (wxTextCtrl* tx, wxTextCtrl* ty, const ECn& q);
	bool GetBigScalar (wxTextCtrl* t, Big& value, bool checkForExistingCurve = true);
	bool InitializeDisplayedEllipticCurve();
	bool CheckOrderSize();
	bool check(const Big& A, const Big& B, const Big& x, const Big& y, const Big&p);
	void waitForDocumentServerToBeReady();
	void outputBigCurveParameters();
	wxString getUsrLocation();
	void ReadFromGRPH();
	void ReadFromREL();
	void ReadFromPOSET();
	void saveRelationAsGML (const wxString& filename);
	void savePartialOrderAsGML (const wxString& filename);
	void readRelationFromGML (const wxString& filename);
	void readPartialOrderFromGML (const wxString& filename);
	long defaultBase;
	bool haveDefaultBase;
	struct CurveData currentCurve;
	bool haveCurrentEllipticCurve;
	int base;
	long numbitsInPrime;
	wxConfigBase *pConfig;
	wxString modulusString;
	Big orderOfEllipticCurve;
    Big primeForReadCurve;
	int findStartOfBigCurveParameter(wxString s);

	wxDECLARE_EVENT_TABLE();
};

#endif // DISCRETEMATHCALCULATORMAIN_H
