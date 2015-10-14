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

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/internal/TMVA/inc ${ROOT_INCLUDE_DIR}) 

foreach(n 1 2 3 4)
    set(headers_full${n})
    foreach(header ${headers${n}})
        set(headers_full${n} ${headers_full${n}} ${PROJECT_SOURCE_DIR}/internal/TMVA/inc/TMVA/${header})
    endforeach(header)

    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/dict${n}.C PROPERTIES GENERATED TRUE)
    add_custom_target(generatedicts${n} ALL ${ROOT_DICT_EXECUTABLE}
        -f ${CMAKE_CURRENT_BINARY_DIR}/dict${n}.C
        -c ${headers_full${n}} 
        ${PROJECT_SOURCE_DIR}/internal/TMVA/inc/LinkDef${n}.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach(n)


file(GLOB SRCFILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
)

add_library(TMVA SHARED ${SRCFILES} dict1.C dict2.C dict3.C dict4.C)
add_dependencies(generatedicts1 generatedicts2 generatedicts3 generatedicts4)
target_link_libraries(TMVA ${ROOT_LIBRARIES} -lRIO -lHist -lTree -lMLP -lMinuit -lXMLIO)

