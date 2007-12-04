#ifndef DataFormats_Common_RefCoreGet_h
#define DataFormats_Common_RefCoreGet_h

/*----------------------------------------------------------------------
  
RefCoreGet: Free function to get the pointer to a referenced product.

$Id: RefCoreGet.h,v 1.3 2007/08/15 04:26:28 wmtan Exp $

----------------------------------------------------------------------*/

#include "DataFormats/Common/interface/RefCore.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Wrapper.h"

namespace edm {

  namespace refcore {
    template <typename T>
    inline
    T const* 
    getProductPtr_(RefCore const& ref) {
      //if (isNull()) throwInvalidReference();
      EDProduct const* product = ref.getProductPtr();
      if (product == 0) {
	throw edm::Exception(errors::ProductNotFound)
	  << "RefCore: A request to resolve a reference to a product of type: "
	  << typeid(T).name()
	  << "\ncan not be satisfied because the product cannot be found."
	  << "\nProbably the branch containing the product is not stored in the input file.\n";
      }
      Wrapper<T> const* wrapper = dynamic_cast<Wrapper<T> const*>(product);

      if (wrapper == 0) { 
	throw edm::Exception(errors::InvalidReference,"WrongType")
	  << "RefCore: A request to convert a contained product of type: "
	  << typeid(product).name() << "\n"
	  << " to type " << typeid(T).name()
	  << "\ncan not be satisfied\n";
      }
      ref.setProductPtr(wrapper->product());
      return wrapper->product();
    }
  }

  template <typename T>
  inline
  T const*
  getProduct(RefCore const & ref) {
    T const* p = static_cast<T const *>(ref.productPtr());
    return (p != 0) ? p : refcore::getProductPtr_<T>(ref);
  }
}
#endif
