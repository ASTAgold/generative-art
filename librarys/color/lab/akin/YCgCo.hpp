#ifndef color_lab_akin_YCgCo
#define color_lab_akin_YCgCo

#include "../../generic/akin/lab.hpp"
#include "../category.hpp"
#include "../../YCgCo/category.hpp"

namespace color
 {
  namespace akin
   {

    template
     <
       typename tag_name
      ,::color::constant::lab::reference_enum reference_number

     >
     struct lab< ::color::category::YCgCo< tag_name >, reference_number >
      {
       public:
         typedef ::color::category::lab< tag_name, reference_number > akin_type;
      };

   }
 }

#endif
