/******************************************************************************
 * @file   mutant.h
 * @author Federico Iannucci
 * @date   14 ott 2015
 * @brief  Mutant class
 ******************************************************************************/
#ifndef INCLUDE_MUTANT_H_
#define INCLUDE_MUTANT_H_

namespace chimera {
namespace mutant {

using IdType = unsigned int;

/**
 * @brief Generic Mutant Class
 */
class Mutant {
  // Typedefs and using
 public:
 private:
  IdType id; /**< Unique identifier */
};

}  // End chimera::mutant namespace
}  // End chimera namespace

#endif /* INCLUDE_MUTANT_H_ */
