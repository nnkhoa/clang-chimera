/******************************************************************************
 * @file   chimera_main.cpp
 * @author Federico Iannucci
 * @date   14 ott 2015
 * @brief  The application main
 ******************************************************************************/

#include "Operators/FLAP/Operators.h"
#include "Tooling/ChimeraTool.h"

int main(int argc, const char **argv) {
  // Create a Chimera Tool
  ::chimera::ChimeraTool chimeraTool;

  //  chimeraTool.registerMutationOperator(::chimera::getDefaultOperator());
  //  chimeraTool.registerMutationOperator(::chimera::getStuckIfOperator());
  //  chimeraTool.registerMutationOperator(
  //      ::chimera::getAssignmentControlOperator());
  //  chimeraTool.registerMutationOperator(::chimera::getDeadClauseOperator());
  //  chimeraTool.registerMutationOperator(::chimera::getDeadProcessOperator());
  //  chimeraTool.registerMutationOperator(::chimera::getMicroOperationOperator());
  //  chimeraTool.registerMutationOperator(::chimera::getLocalStuckData());
  //  chimeraTool.registerMutationOperator(::chimera::getGlobalStuckData());

  chimeraTool.registerMutationOperator(::iideaa::getFLAPOperator());
  chimeraTool.registerMutationOperator(::iideaa::getFLAPVivadoOperator());

  return chimeraTool.run(argc, argv);
}
