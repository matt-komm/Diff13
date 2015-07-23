############################################################################
# CMakeLists.txt file for building ROOT tmva package
# @author Pere Mato, CERN
############################################################################

find_package(ROOT REQUIRED)


set(headers1 Configurable.h Event.h Factory.h MethodBase.h MethodCompositeBase.h
	     MethodANNBase.h MethodTMlpANN.h MethodRuleFit.h MethodCuts.h MethodFisher.h
	     MethodKNN.h MethodCFMlpANN.h MethodCFMlpANN_Utils.h MethodLikelihood.h
	     MethodHMatrix.h MethodPDERS.h MethodBDT.h MethodDT.h MethodSVM.h MethodBayesClassifier.h
	     MethodFDA.h MethodMLP.h MethodBoost.h
	     MethodPDEFoam.h MethodLD.h MethodCategory.h)
set(headers2 TSpline2.h TSpline1.h PDF.h BinaryTree.h BinarySearchTreeNode.h BinarySearchTree.h 
	     Timer.h RootFinder.h CrossEntropy.h DecisionTree.h DecisionTreeNode.h MisClassificationError.h 
	     Node.h SdivSqrtSplusB.h SeparationBase.h RegressionVariance.h Tools.h Reader.h 
	     GeneticAlgorithm.h GeneticGenes.h GeneticPopulation.h GeneticRange.h GiniIndex.h 
	     GiniIndexWithLaplace.h SimulatedAnnealing.h QuickMVAProbEstimator.h)
set(headers3 Config.h KDEKernel.h Interval.h LogInterval.h FitterBase.h MCFitter.h GeneticFitter.h 
         SimulatedAnnealingFitter.h MinuitFitter.h MinuitWrapper.h IFitterTarget.h
         PDEFoam.h PDEFoamDecisionTree.h PDEFoamDensityBase.h PDEFoamDiscriminantDensity.h
         PDEFoamEventDensity.h PDEFoamTargetDensity.h PDEFoamDecisionTreeDensity.h PDEFoamMultiTarget.h
         PDEFoamVect.h PDEFoamCell.h PDEFoamDiscriminant.h PDEFoamEvent.h PDEFoamTarget.h
         PDEFoamKernelBase.h PDEFoamKernelTrivial.h PDEFoamKernelLinN.h PDEFoamKernelGauss.h
         BDTEventWrapper.h CCTreeWrapper.h
         CCPruner.h CostComplexityPruneTool.h SVEvent.h OptimizeConfigParameters.h)
set(headers4 TNeuron.h TSynapse.h TActivationChooser.h TActivation.h TActivationSigmoid.h TActivationIdentity.h
         TActivationTanh.h TActivationRadial.h TNeuronInputChooser.h TNeuronInput.h TNeuronInputSum.h
	     TNeuronInputSqSum.h TNeuronInputAbs.h Types.h Ranking.h RuleFit.h RuleFitAPI.h IMethod.h MsgLogger.h
	     VariableTransformBase.h VariableIdentityTransform.h VariableDecorrTransform.h VariablePCATransform.h 
	     VariableGaussTransform.h VariableNormalizeTransform.h VariableRearrangeTransform.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/internal/TMVA/inc)

foreach(hs headers1 headers2 headers3 headers4)
  foreach(h ${${hs}})
    set(t${hs} ${t${hs}} TMVA/${h})
  endforeach()
endforeach()

#ROOT_GENERATE_DICTIONARY(G__TMVA1 ${theaders1} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/inc/LinkDef1.h)
#ROOT_GENERATE_DICTIONARY(G__TMVA2 ${theaders2} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/inc/LinkDef2.h)
#ROOT_GENERATE_DICTIONARY(G__TMVA3 ${theaders3} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/inc/LinkDef3.h)
#ROOT_GENERATE_DICTIONARY(G__TMVA4 ${theaders4} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/inc/LinkDef4.h)

#ROOT_GENERATE_ROOTMAP(TMVA LINKDEF LinkDef1.h LinkDef2.h LinkDef3.h LinkDef4.h DEPENDENCIES RIO Hist Matrix Tree Graf Gpad TreePlayer MLP Minuit MathCore XMLIO)

#ROOT_LINKER_LIBRARY(TMVA *.cxx G__TMVA1.cxx G__TMVA2.cxx G__TMVA3.cxx G__TMVA4.cxx LIBRARIES Core Cint  DEPENDENCIES RIO Hist Tree MLP Minuit XMLIO)

#add_custom_target(DICTS)
#add_dependencies(DICTS G__TMVA1)# G__TMVA2 G__TMVA3 G__TMVA4)

file(GLOB SRCFILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
)

add_library(TMVA SHARED ${SRCFILES})# G__TMVA1.cxx G__TMVA2.cxx G__TMVA3.cxx G__TMVA4.cxx)
target_link_libraries(TMVA ${ROOT_LIBRARIES} -lCint -lRIO -lHist -lTree -lMLP -lMinuit -lXMLIO)

#install(DIRECTORY inc/TMVA/ DESTINATION include/TMVA
#                            PATTERN ".svn" EXCLUDE
#                           REGEX "LinkDef" EXCLUDE )
#if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_INSTALL_PREFIX)
#  install(DIRECTORY test DESTINATION tmva PATTERN ".svn" EXCLUDE)
#endif()


